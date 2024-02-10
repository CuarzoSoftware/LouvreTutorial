#include <LLauncher.h>
#include <LLog.h>
#include "ECompositor.h"
 
using namespace Louvre;
 
int main()
{
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