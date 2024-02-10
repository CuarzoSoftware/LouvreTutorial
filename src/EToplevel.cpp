#include <LSeat.h>
#include <LPointer.h>
#include <LCursor.h>
#include "ESurface.h"
#include "EToplevel.h"
#include "EOutput.h"
#include "Global.h"

EToplevel::EToplevel(const void *params) : LToplevelRole(params) {}

void EToplevel::configureRequest()
{
    // Request the client to draw its own window decorations
    setDecorationMode(ClientSide);

    // Configure the toplevel with (0,0) so that the client can decide the size
    configure(LSize(0, 0), pendingStates() | Activated);
}

void EToplevel::setMinimizedRequest()
{
    if (!fullscreen())
        surf()->setMinimized(true);
}

void EToplevel::startMoveRequest()
{
    if (!fullscreen() && !maximized() && surf()->hasPointerFocus())
        seat()->pointer()->startMovingToplevel(
            this,                    // The toplevel being dragged
            cursor()->pos(),         // The initial cursor position
            LPointer::EdgeDisabled,  // Left edge constraint
            TOPBAR_HEIGHT);          // Top edge constraint
}

void EToplevel::startResizeRequest(ResizeEdge edge)
{
    if (!fullscreen() && !maximized() && surf()->hasPointerFocus())
        seat()->pointer()->startResizingToplevel(
            this,                   // The toplevel being resized
            edge,                   // Edge or corner being dragged
            cursor()->pos(),        // Initial cursor position
            LSize(0, 0),            // Minimum allowed size
            LPointer::EdgeDisabled, // Left edge constraint
            TOPBAR_HEIGHT);         // Top edge constraint
}

void EToplevel::geometryChanged()
{
    /* When the toplevel size changes during interactive resizing
     * we need to update its position depending on the dragged edge/corner */
    if (resizing())
        updateResizingPos();
    else
        updateGeometry(true);
}

void EToplevel::statesChanged()
{
    fullscreenBackground.setVisible(fullscreen());

    if (!output)
        return;

    /* If output is set then the toplevel changed from maximized or fullscreen
     * to normal */
    if (!maximized() && !fullscreen())
    {
        G::reparentSurfaceAndChildrenViews(surf(), &G::compositor()->surfacesLayer);
        const LPoint prevPos { output->pos() + (prevRelativePos * output->size()) };
        surf()->setPos(prevPos);
        surf()->raise();
        output = nullptr;
        return;
    }

    if (!updateGeometry(true))
        return;

    if (maximized())
    {
        G::reparentSurfaceAndChildrenViews(surf(), &G::compositor()->surfacesLayer);
        surf()->raise();
    }
    else if (fullscreen())
        G::reparentSurfaceAndChildrenViews(surf(), &fullscreenBackground);

}

bool EToplevel::updateGeometry(bool fallbackToNormalOnFail)
{
    if (!output)
        return false;

    /* If the client did not adhere to our instructions, we may consider terminating it.
     * However, out of courtesy, we will instead reconfigure it back to its original
     * normal state. */

    if (maximized())
    {
        const LSize targetSize { output->size() - LSize(0, TOPBAR_HEIGHT) };

        if (fallbackToNormalOnFail && targetSize != size())
        {
            configure(prevSize, Activated);
            return false;
        }

        surf()->setPos(output->pos() + LPoint(0, TOPBAR_HEIGHT));
    }
    else if (fullscreen())
    {
        const LSize &targetSize { output->size() };

        if (fallbackToNormalOnFail && (size().w() > targetSize.w() || size().h() > targetSize.h()))
        {
            configure(prevSize, Activated);
            return false;
        }

        fullscreenBackground.setPos(output->pos());
        fullscreenBackground.setSize(output->size());

        const LPoint centeredPos { output->pos() + output->size() / 2 - size() / 2 };
        surf()->setPos(centeredPos);
    }

    return true;
}

void EToplevel::setMaximized()
{
    bool inNormalState = { output == nullptr };

    if (inNormalState)
        saveNormalState();

    // Halt if zero initialized outputs
    if (!output)
        return;

    LSize targetSize { output->size() - LSize(0, TOPBAR_HEIGHT) };

    /* If the toplevel does not support the target size, we will configure it with that size regardless.
     * However, we will set only the activated flag, ensuring it expands solely in size, keeping its
     * decorations */
    if (!sizeInRange(targetSize))
    {
        if (inNormalState)
            output = nullptr;

        configure(targetSize, Activated);
        return;
    }

    configure(targetSize, Activated | Maximized);
}

void EToplevel::setMaximizedRequest()
{
    if (pendingStates() & Maximized || !(surf()->hasKeyboardFocus() || surf()->hasPointerFocus()))
        return;

    setMaximized();
}

void EToplevel::unsetMaximizedRequest()
{
    if (~pendingStates() & Maximized || !(surf()->hasKeyboardFocus() || surf()->hasPointerFocus()))
        return;

    configure(prevSize, Activated);
}

void EToplevel::setFullscreen()
{
    bool inNormalState = { output == nullptr };

    if (inNormalState)
        saveNormalState();

    // Halt if zero initialized outputs
    if (!output)
        return;

    const LSize &targetSize { output->size() };

    /* Unlike maximizing, we can simply center the toplevel if its max size is smaller than the output size
     * on the other hand, if its min size is larger than the output size, then we simply ignore the request */
    if ( (minSize().w() > targetSize.w() && minSize().w() != 0 ) || (minSize().h() > targetSize.h() && minSize().h() != 0 ) )
    {
        if (inNormalState)
            output = nullptr;

        return;
    }

    configure(targetSize, Activated | Fullscreen);
}

void EToplevel::setFullscreenRequest(LOutput *preferredOutput)
{
    /* Clients can request to display the toplevel on a specific output
     * or pass nullptr if they have no preference. For simplicity we are
     * simply ignoring them */
    L_UNUSED(preferredOutput)

    if (pendingStates() & Fullscreen || !(surf()->hasKeyboardFocus() || surf()->hasPointerFocus()))
        return;

    setFullscreen();
}

void EToplevel::unsetFullscreenRequest()
{
    if (~pendingStates() & Fullscreen || !(surf()->hasKeyboardFocus() || surf()->hasPointerFocus()))
        return;

    configure(prevSize, Activated);
}

void EToplevel::saveNormalState()
{
    output = surf()->primaryOutput();

    if (output)
    {
        LPointF localPos { rolePos() - output->pos() };
        prevRelativePos = localPos / LSizeF(output->size());
    }
    else if (!G::outputs().empty())
    {
        output = G::outputs().front();
        prevRelativePos = LSizeF(0.25f, 0.25f);
    }

    prevSize = size();
}

