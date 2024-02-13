#ifndef EPOINTER_H
#define EPOINTER_H

#include <LPointer.h>

using namespace Louvre;

class EPointer : public LPointer
{
public:
    EPointer(const void *params);

    void pointerMoveEvent(Float32 x, Float32 y, bool isAbsolute) override;
    void pointerButtonEvent(Button button, ButtonState state) override;
    void pointerAxisEvent(Float64 axisX, Float64 axisY, Int32 discreteX, Int32 discreteY, AxisSource source) override;
    void setCursorRequest(LCursorRole *cursorRole) override;
};

#endif // EPOINTER_H
