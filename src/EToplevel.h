#ifndef ETOPLEVEL_H
#define ETOPLEVEL_H

#include <LToplevelRole.h>
#include <LSolidColorView.h>
#include "ECompositor.h"
#include "Global.h"

using namespace Louvre;

class ESurface;
class EOutput;

class EToplevel : public LToplevelRole
{
public:
    EToplevel(const void *params);

    inline ESurface *surf() const
    {
        return (ESurface*) surface();
    };

    void configureRequest() override;
    void setMinimizedRequest() override;
    void startMoveRequest() override;
    void startResizeRequest(ResizeEdge edge) override;
    void geometryChanged() override;

    void statesChanged() override;
    bool updateGeometry(bool fallbackToNormalOnFail);

    void setMaximized();
    void setMaximizedRequest() override;
    void unsetMaximizedRequest() override;
    void maximizedChanged() override
    { /* Using statesChanged() instead */ };

    void setFullscreen();
    void setFullscreenRequest(LOutput *preferredOutput) override;
    void unsetFullscreenRequest() override;
    void fullscreenChanged() override
    { /* Using statesChanged() instead */ };

    void saveNormalState();

    LSolidColorView fullscreenBackground {
        0.f, 0.f, 0.f, 1.f,
        &G::compositor()->fullscreenLayer
    };

    EOutput *output         { nullptr };
    LPointF prevRelativePos { 200.f, 200.f};
    LSize prevSize          { 0, 0};
};

#endif // ETOPLEVEL_H
