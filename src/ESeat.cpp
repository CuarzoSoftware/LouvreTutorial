#include <LDataDevice.h>
#include <LPointer.h>
#include <LKeyboard.h>
#include "ESeat.h"
#include "EOutput.h"
#include "Global.h"

ESeat::ESeat(const void *params) : LSeat(params) {}

bool ESeat::setSelectionRequest(LDataDevice *device)
{
    // Let the client set the clipboard only if one of its surfaces has pointer or keyboard focus
    return (pointer()->focus() && pointer()->focus()->client() == device->client()) ||
           (keyboard()->focus() && keyboard()->focus()->client() == device->client());
}

void ESeat::outputPlugged(LOutput *output)
{
    output->setScale(output->dpi() >= 200 ? 2 : 1);

    if (G::outputs().empty())
        output->setPos(LPoint(0,0));
    else
        output->setPos(G::outputs().back()->pos() + LPoint(G::outputs().back()->size().w(), 0));

    compositor()->addOutput(output);
    compositor()->repaintAllOutputs();
}

void ESeat::outputUnplugged(LOutput *output)
{
    compositor()->removeOutput(output);

    Int32 totalWidth { 0 };

    for (EOutput *o : G::outputs())
    {
        o->setPos(LPoint(totalWidth, 0));
        totalWidth += o->size().w();
    }

    compositor()->repaintAllOutputs();
}

void ESeat::enabledChanged()
{
    if (!enabled())
        return;

    /* Synchronize LScene's output inter-frame damage tracking by performing
     * full damage on all outputs. */
    for (EOutput *output : G::outputs())
    {
        G::scene()->mainView()->damageAll(output);
        output->repaint();
    }
}
