#include <LSeat.h>
#include <LPointer.h>
#include <LKeyboard.h>
#include <LDataSource.h>
#include "EDNDManager.h"

EDNDManager::EDNDManager(const void *params) : LDNDManager(params) {}

void EDNDManager::startDragRequest()
{
    // Let the client start the session only if the origin surface has pointer focus
    if (origin()->hasPointerFocus())
        seat()->pointer()->setDraggingSurface(nullptr);
    else
        cancel();
}
