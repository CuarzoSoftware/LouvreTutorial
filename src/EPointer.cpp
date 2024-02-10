#include <LScene.h>
#include <LCursor.h>
#include <LView.h>
#include "EPointer.h"
#include "Global.h"

EPointer::EPointer(const void *params) : LPointer(params) {}

void EPointer::pointerMoveEvent(Float32 x, Float32 y, bool absolute)
{
    const LView *view { G::scene()->handlePointerMoveEvent(x, y, absolute) };

    if ((!view || view->type() != LView::Type::Surface) && !resizingToplevel() && !movingToplevel())
    {
        cursor()->useDefault();
        cursor()->setVisible(true);
    }
}

void EPointer::pointerButtonEvent(Button button, ButtonState state)
{
    G::scene()->handlePointerButtonEvent(button, state);
}

void EPointer::pointerAxisEvent(Float64 axisX, Float64 axisY, Int32 discreteX, Int32 discreteY, AxisSource source)
{
    G::scene()->handlePointerAxisEvent(-axisX, -axisY, -discreteX, -discreteY, source);
}
