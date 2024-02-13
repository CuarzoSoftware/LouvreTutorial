#include <LPainter.h>
#include <LTexture.h>
#include <LOpenGL.h>
#include <LLog.h>
#include <LDNDManager.h>
#include <LCompositor.h>
#include <LCursor.h>
#include <LSeat.h>
#include <LTextureView.h>
#include "EOutput.h"
#include "ESurface.h"
#include "EToplevel.h"
#include "Global.h"

EOutput::EOutput(const void *params) : LOutput(params)
{
    /* Texture views have a default nullptr translucent region, which means they are considered
     * completely translucent by the scene. However, setting an empty LRegion (not nullptr) indicates
     * to the scene that the view is completely opaque. */
    const LRegion emptyRegion;
    wallpaperView.setTranslucentRegion(&emptyRegion);
}

void EOutput::initializeGL()
{
    updateWallpaper();
    topbar.copyThumbnailsFromAnotherTopbar();
    G::scene()->handleInitializeGL(this);
    repaint();
}

void EOutput::moveGL()
{
    updateToplevelsPos();
    wallpaperView.setPos(pos());
    topbar.updateGUI();
    G::scene()->handleMoveGL(this);
    repaint();
}

void EOutput::resizeGL()
{
    Int32 totalWidth { 0 };

    for (EOutput *o : G::outputs())
    {
        o->setPos(LPoint(totalWidth, 0));
        totalWidth += o->size().w();
    }

    updateToplevelsSize();
    updateWallpaper();
    topbar.updateGUI();
    G::scene()->handleResizeGL(this);
    repaint();
}

void EOutput::paintGL()
{
    // Not all outputs may have hardware cursor support
    if (cursor()->hasHardwareSupport(this))
        G::compositor()->softwareCursor.setTexture(nullptr);
    else
    {
        G::compositor()->softwareCursor.setTexture(cursor()->texture());
        G::compositor()->softwareCursor.setPos(cursor()->rect().pos());
        G::compositor()->softwareCursor.setDstSize(cursor()->rect().size());
        G::compositor()->softwareCursor.setVisible(cursor()->visible());
    }

    EToplevel *fullscreenToplevel { findFullscreenToplevel() };
    enableFractionalOversampling( fullscreenToplevel == nullptr);
    enableVSync(fullscreenToplevel == nullptr ? true : fullscreenToplevel->surf()->preferVSync());
    G::scene()->handlePaintGL(this);
}

void EOutput::uninitializeGL()
{
    rescueViewsFromVoid();
    G::scene()->handleUninitializeGL(this);

    /* When a texture used by an LTextureView is destroyed,
     * the view's texture is automatically set to nullptr. */
    if (wallpaperView.texture())
        delete wallpaperView.texture();

    topbar.destroyThumbnails();
}

void EOutput::setGammaRequest(LClient *client, const LGammaTable *gamma)
{
    /* Note: This is quite unsafe, you should only permit well-known
     * clients to set gamma correction curves. */
    L_UNUSED(client)

    setGamma(gamma);
}

EToplevel *EOutput::findFullscreenToplevel() const
{
    for (ESurface *surface : G::surfaces())
        if (surface->tl() && surface->tl()->fullscreen() && surface->tl()->output == this)
            return surface->tl();

    return nullptr;
}

void EOutput::updateToplevelsPos()
{
    for (ESurface *surface : G::surfaces())
        if (surface->tl() && surface->tl()->output == this)
            surface->tl()->updateGeometry(false);
}

void EOutput::updateToplevelsSize()
{
    for (ESurface *surface : G::surfaces())
    {
        EToplevel *tl = { surface->tl() };

        if (tl && tl->output == this)
        {
            if (tl->pendingStates() & LToplevelRole::Maximized)
                tl->setMaximized();
            else
                tl->setFullscreen();
        }
    }
}

void EOutput::rescueViewsFromVoid()
{
    // Search another initialized output
    const auto isDiff = [this](const EOutput *output) { return this != output; };
    const auto it { std::find_if(G::outputs().begin(), G::outputs().end(), isDiff) };
    EOutput *fallbackOutput { it != G::outputs().end() ? *it : nullptr };

    for (ESurface *surface : G::surfaces())
    {
        if (surface->outputBeforeMinimize == this)
            surface->outputBeforeMinimize = fallbackOutput;
        else if (surface->tl() && surface->tl()->output == this)
        {
            surface->tl()->output = nullptr;
            surface->setPos(200, 200);
            G::reparentSurfaceAndChildrenViews(surface, &G::compositor()->surfacesLayer);
            surface->tl()->configure(surface->tl()->prevSize, LToplevelRole::Activated);
        }
        else if(surface->primaryOutput() == this)
            surface->setPos(200, 200);
    }
}

void EOutput::updateWallpaper()
{
    if (wallpaperView.texture())
    {
        /* If the current wallpaper size is equal to the output size
         * then the output simply changed its scale */
        if (wallpaperView.texture()->sizeB() == sizeB())
        {
            wallpaperView.setBufferScale(scale());
            wallpaperView.setPos(pos());
            return;
        }

        delete wallpaperView.texture();
    }

    LTexture *originalWallpaper {
        LOpenGL::loadTexture(compositor()->defaultAssetsPath() / "wallpaper.png") };

    if (!originalWallpaper)
    {
        LLog::error("[louvre-example] Failed to load wallpaper.");
        return;
    }

    const LSize &originalSize { originalWallpaper->sizeB() };

    /* This section calculates the source wallpaper rect that needs to be copied and scaled to cover the entire output
     * while maintaining the image's aspect ratio. */

    // Source rect within the texture to clip
    LRect srcRect { 0 };

    // Calculate the texture's width if it were scaled to the output's height, while preserving the aspect ratio
    const Float32 scaledWidth { Float32(size().w() * originalSize.h()) / Float32(size().h()) };

    // If the scaled width is greater than or equal to the output's width, we clip the texture's left and right sides
    if (scaledWidth >= originalSize.w())
    {
        srcRect.setW(originalSize.w());
        srcRect.setH((originalSize.w() * size().h()) / size().w());
        srcRect.setY((originalSize.h() - srcRect.h()) / 2);
    }
    // Otherwise, we clip the texture's top and bottom
    else
    {
        srcRect.setH(originalSize.h());
        srcRect.setW((originalSize.h() * size().w()) / size().h());
        srcRect.setX((originalSize.w() - srcRect.w()) / 2);
    }

    // Copy the srcRect of the original wallpaper and scale it to match the output buffer size
    wallpaperView.setTexture(originalWallpaper->copyB(sizeB(), srcRect));

    // Set the buffer scale of wallpaperView to match the output scale
    wallpaperView.setBufferScale(scale());

    // Delete the original wallpaper texture since we are using the scaled copy
    delete originalWallpaper;
    wallpaperView.setPos(pos());
}
