#include <protocols/Wayland/GSeat.h>
#include <LSeat.h>
#include <LKeyboard.h>
#include <LCursor.h>
#include <LPositioner.h>
#include <LOutput.h>
#include "ECompositor.h"
#include "ESurface.h"
#include "EPopup.h"
#include "Global.h"

EPopup::EPopup(const void *params) : LPopupRole(params) {}

void EPopup::configureRequest()
{
    // If the bounding rect has zero area, constraints are disabled
    LRect boundingRect { 0 };

    // The cursor is always positioned within an output unless there are zero initialized outputs
    if (cursor()->output())
    {
        // Ensure the popup stays within the boundaries of the current output where the cursor is positioned
        boundingRect = cursor()->output()->rect();

        // If the popup is a child of a fullscreen toplevel, we can ignore the top bar area
        if (surf()->view.parent() == &G::compositor()->surfacesLayer)
            boundingRect += LRect(0, TOPBAR_HEIGHT, 0, -TOPBAR_HEIGHT);
    }

    setPositionerBounds(boundingRect);

    /* Calculate the relative position of the Popup with respect to its parent position,
     * calling rolePos() also determines LPositioner::unconstrainedSize() */
    const LPoint localParentPos { rolePos() - surface()->parent()->pos() };
    configure(LRect(localParentPos, positioner().unconstrainedSize()));
}

void EPopup::grabSeatRequest(Protocols::Wayland::GSeat *seatGlobal)
{
    /* The grabSeatRequest() is only triggered if the request has been
     * originated from some client event, such as a pointer click or key press*/
    seat()->keyboard()->setGrabbingSurface(surface(), seatGlobal->keyboardResource());
}
