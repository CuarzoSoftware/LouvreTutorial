#include <LLauncher.h>
#include <LLog.h>
#include "ECompositor.h"
 
using namespace Louvre;
 
int main()
{
    setenv("LOUVRE_DEBUG", "4", 0);

    LLauncher::startDaemon();
 
    ECompositor compositor;
 
    if (!compositor.start())
    {
        LLog::fatal("[louvre-example] Failed to start compositor.");
        return 1;
    }
 
    while (compositor.state() != LCompositor::Uninitialized)
        compositor.processLoop(-1);
 
    return 0;
}
