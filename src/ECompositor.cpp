#include <LSeat.h>
#include <LKeyboard.h>
#include "ECompositor.h"
#include "EOutput.h"
#include "ESurface.h"
#include "EPointer.h"
#include "EKeyboard.h"
#include "EToplevel.h"
 
ECompositor::ECompositor()
{
    // Set black as the scene clear color
    scene.mainView()->setClearColor(0.f, 0.f, 0.f, 1.f);

    // This allow us to define a custom size to an LTextureView
    softwareCursor.enableDstSize(true);
}
 
void ECompositor::initialized()
{
    // Set a keyboard map with "latam" layout
    seat()->keyboard()->setKeymap(nullptr, nullptr, "latam", nullptr);

    Int32 totalWidth { 0 };
 
    // Initialize all avaliable outputs
    for (LOutput *output : seat()->outputs())
    {
        // Set double scale to outputs with DPI >= 200
        output->setScale(output->dpi() >= 200 ? 2.f : 1.f);

        // Assuming your outputs are not rotated or flipped
        output->setTransform(LFramebuffer::Normal);
 
        output->setPos(LPoint(totalWidth, 0));
        totalWidth += output->size().w();
 
        addOutput(output);
        output->repaint();
    }
}

LOutput *ECompositor::createOutputRequest(const void *params)
{
    return new EOutput(params);
}

LSurface *ECompositor::createSurfaceRequest(const void *params)
{
    return new ESurface(params);
}

LPointer *ECompositor::createPointerRequest(const void *params)
{
    return new EPointer(params);
}

LKeyboard *ECompositor::createKeyboardRequest(const void *params)
{
    return new EKeyboard(params);
}

LToplevelRole *ECompositor::createToplevelRoleRequest(const void *params)
{
    return new EToplevel(params);
}
