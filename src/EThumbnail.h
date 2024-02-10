#ifndef ETHUMBNAIL_H
#define ETHUMBNAIL_H

#include <LTextureView.h>

using namespace Louvre;

class ETopbar;
class ESurface;

class EThumbnail : public LTextureView
{
public:
    EThumbnail(ETopbar *topbar, ESurface *surface);
    ~EThumbnail();

    void pointerButtonEvent(LPointer::Button button, LPointer::ButtonState state) override;

    ETopbar  *topbar;
    ESurface *surface;
};

#endif // ETHUMBNAIL_H
