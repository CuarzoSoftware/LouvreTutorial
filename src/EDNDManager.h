#ifndef EDNDMANAGER_H
#define EDNDMANAGER_H

#include <LDNDManager.h>

using namespace Louvre;

class EDNDManager : public LDNDManager
{
public:
    EDNDManager(const void *params);

    void startDragRequest() override;
};

#endif // EDNDMANAGER_H
