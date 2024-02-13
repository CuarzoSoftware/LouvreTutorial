#ifndef ESEAT_H
#define ESEAT_H

#include <LSeat.h>

using namespace Louvre;

class ESeat : public LSeat
{
public:
    ESeat(const void *params);

    // Set clipboard request
    virtual bool setSelectionRequest(LDataDevice *device) override;

    void outputPlugged(LOutput *output) override;
    void outputUnplugged(LOutput *output) override;

    // TTY switching
    void enabledChanged() override;
};

#endif // ESEAT_H
