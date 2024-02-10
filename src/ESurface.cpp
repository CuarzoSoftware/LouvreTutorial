#include <LCursor.h>
#include <LSeat.h>
#include <LKeyboard.h>
#include <LPointer.h>
#include "ESurface.h"
#include "EThumbnail.h"
#include "EOutput.h"
#include "EToplevel.h"

ESurface::ESurface(const void *params) : LSurface(params)
{
    // Detaches its position from its parent view
    view.enableParentOffset(false);
}

ESurface::~ESurface()
{
    while (!thumbnailViews.empty())
        delete thumbnailViews.back();

    if (thumbnailTexture)
        delete thumbnailTexture;
}


void ESurface::roleChanged()
{
    // Hide cursor surfaces because we use LCursor
    if (cursorRole())
        view.setVisible(false);

    // Move drag & drop icons to the cursor layer so they always
    // appear on top of other views
    else if (dndIcon())
    {
        // Ensure it is positioned behind 'softwareCursor'
        view.setParent(&G::compositor()->cursorLayer);
        view.insertAfter(nullptr, false);
        setPos(cursor()->pos());
    }
}

void ESurface::orderChanged()
{
    ESurface *prev { (ESurface*)prevSurface() };

    /* Surfaces are ordered in a single list, however, our views
     * can be children of the surface, cursor, or fullscreen layers.
     * Therefore, if 'prev' has a different parent, we need to continue searching. */

    while (prev && prev->view.parent() != view.parent())
        prev = (ESurface*)prev->prevSurface();

    if (prev)
        view.insertAfter(&prev->view, false);

    // If there is no prev surface with the same parent, insert it at the beginning of the current parent's children list
    else
        view.insertAfter(nullptr, false);
}

void ESurface::minimizedChanged()
{
    /* When a surface is minimized, all its children are also minimized
     * we only want to display toplevels in the topbar so we ignore the rest */
    if (!toplevel())
        goto toggleVisibility;

    if (minimized())
    {
        // We don't want the surface to continue receiving input events while minimized
        if (hasPointerFocus())
            seat()->pointer()->setFocus(nullptr);

        if (hasKeyboardFocus())
            seat()->keyboard()->setFocus(nullptr);

        /* Save the current surface position relative to the output position as a percentage so we can restore
         * it later even if the outputs arrangement changes or the given output is no longer available */
        if ( (outputBeforeMinimize = primaryOutput()) )
        {
            const LPointF localPos { rolePos() - outputBeforeMinimize->pos() };
            relativePosBeforeMinimize = localPos / LSizeF(outputBeforeMinimize->size());
        }

        /* In case the surface is not visible on any output, we select the first available output and position
         * the surface at 1/4 of the output as a fallback */
        else if (!G::outputs().empty())
        {
            outputBeforeMinimize = G::outputs().front();
            relativePosBeforeMinimize = LPointF(0.25f, 0.25f);
        }
        else
        {
            setMinimized(false);
            return;
        }

        const LSize thumbnailSizeB {
            2 * (THUMBNAIL_HEIGHT * size().w()) / size().h(),
            2 * THUMBNAIL_HEIGHT
        };

        thumbnailTexture = capture(thumbnailSizeB);

        for (EOutput *output : G::outputs())
            new EThumbnail(&output->topbar, this);
    }
    else
    {
        // If nullptr, then it was uninitialized while the surface was minimized
        if (!outputBeforeMinimize)
        {
            if (!G::outputs().empty())
            {
                outputBeforeMinimize = G::outputs().front();
                relativePosBeforeMinimize = LPointF(0.25f, 0.25f);
            }
        }

        // Bye bye thumbnails
        while (!thumbnailViews.empty())
            delete thumbnailViews.front();

        delete thumbnailTexture;
        thumbnailTexture = nullptr;

        // Restore back the previous unminimized position
        setPos(outputBeforeMinimize != nullptr ? outputBeforeMinimize->pos() + (relativePosBeforeMinimize * outputBeforeMinimize->size()) : LPoint(0, TOPBAR_HEIGHT));
        outputBeforeMinimize = nullptr;
        raise();
    }

toggleVisibility:
    view.setVisible(!minimized());
}

void ESurface::mappingChanged()
{
    if (mapped() && firstMapping)
    {
        firstMapping = false;

        // We only center toplevels the first time they are mapped
        if (tl())
        {
            // Use the output where the cursor is currently located
            LOutput *output { cursor()->output() };

            // If output == nullptr it means there is no initialized output
            if (!output)
                return;

            // Restrict the area to the output size - the topbar area
            const LSize availableSize { output->size() - LSize(0, TOPBAR_HEIGHT) };

            // Center the toplevel
            setPos( (output->pos() + LPoint(0, TOPBAR_HEIGHT)) + (availableSize - tl()->windowGeometry().size()) / 2);

            // Prevent positioning it under the topbar (when the toplevel height is greater than dstSize.h())
            if (pos().y() < output->pos().y() + TOPBAR_HEIGHT)
                setY(output->pos().y() + TOPBAR_HEIGHT);

            output->repaint();
        }

        raise();
    }

    /* Note: LSurfaceViews are automatically hidden when their surfaces are unmapped */

    compositor()->repaintAllOutputs();
}

void ESurface::parentChanged()
{
    if (parent())
    {
        const ESurface *p = { (ESurface*)parent() };
        view.setParent(p->view.parent());
    }
    else
        view.setParent(&G::compositor()->surfacesLayer);

    repaintOutputs();
}

EOutput *ESurface::primaryOutput()
{
    EOutput    *bestOutput   { nullptr };
    Int32       bestArea     { 0 };
    const LRect surfaceRect  { rolePos(), size() };

    /* Calculate the area of the surface intersected
     * with each output and return the one with the largest area */
    for (EOutput *output : G::outputs())
    {
        // We use LRegion to intersect both rects
        LRegion tmpRegion;
        tmpRegion.addRect(surfaceRect);
        tmpRegion.clip(output->rect());

        const LBox &extents { tmpRegion.extents() };
        const Int32 area { (extents.x2 - extents.x1) * (extents.y2 - extents.y1) };

        if (area > bestArea)
        {
            bestArea = area;
            bestOutput = output;
        }
    }

    return bestOutput;
}

LTexture *ESurface::capture(const LSize &dstSizeB)
{
    LSceneView tmpSceneView {
        size() * 2, // Framebuffer size : x2 the surface size to make it look good on HiDPI displays.
        2,          // Scaling factor   : Affects how the children views are rendered
        nullptr     // Parent view      : Not required because we are using it for offscreen rendering
    };

    /* Instead of moving each view to the scene view, we move the scene to the views position.
     * This is why disabling parent offset is required. */
    tmpSceneView.setPos(rolePos());

    G::reparentSurfaceAndChildrenViews(this, &tmpSceneView, true);
    tmpSceneView.render();
    G::reparentSurfaceAndChildrenViews(this, &G::compositor()->surfacesLayer, true);

    return tmpSceneView.texture()->copyB(
        dstSizeB, // Destination size in buffer coordiantes
        LRect(),  // Passing an empty src rect means copying the entire texture
        true      // Enable high quality downscaling
    );
}
