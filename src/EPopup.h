#ifndef EPOPUP_H
#define EPOPUP_H

#include <LPopupRole.h>

using namespace Louvre;

class ESurface;

class EPopup : public LPopupRole
{
public:
    EPopup(const void *params);

    // Cast LSurface to ESurface
    inline ESurface *surf() const
    {
        return (ESurface*) surface();
    };

    void configureRequest() override;
    void grabSeatRequest(Protocols::Wayland::GSeat *seatGlobal) override;
};

#endif // EPOPUP_H
