#include "EThumbnail.h"
#include "ESurface.h"
#include "ETopbar.h"

EThumbnail::EThumbnail(ETopbar *topbar, ESurface *surface) :
    LTextureView(surface->thumbnailTexture, &topbar->view),
    topbar(topbar),
    surface(surface)
{
    surface->thumbnailViews.push_back(this);
    enableParentOpacity(false);
    setBufferScale(2);
    topbar->updateGUI();
    enableInput(true);
}

EThumbnail::~EThumbnail()
{
    /* This is a handy function used for efficiently removing an element from a
     * vector where order isn't essential, employing the back-swap approach */
    LVectorRemoveOneUnordered(surface->thumbnailViews, this);

    /* Before calling updateGUI(), it's essential to remove it from the children
     * list of the topbar view to ensure proper ordering of the remaining thumbnails */
    setParent(nullptr);

    topbar->updateGUI();
}

void EThumbnail::pointerButtonEvent(LPointer::Button button, LPointer::ButtonState state)
{
    if (button == LPointer::Button::Left && state == LPointer::ButtonState::Released)
        surface->setMinimized(false);
}
