#include <LSeat.h>
#include <LKeyboard.h>
#include <LAnimation.h>
#include <LCursor.h>
#include <LXCursor.h>
#include "ECompositor.h"
#include "EOutput.h"
#include "ESurface.h"
#include "EPointer.h"
#include "EKeyboard.h"
#include "EToplevel.h"
#include "EPopup.h"
#include "ESeat.h"
#include "EDNDManager.h"

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

void ECompositor::cursorInitialized()
{
    defaultCursor = LXCursor::loadXCursorB("arrow");

    if (defaultCursor)
        cursor()->replaceDefaultB(defaultCursor->texture(), defaultCursor->hotspotB());
}

void ECompositor::uninitialized()
{
    if (defaultCursor)
    {
        delete defaultCursor;
        defaultCursor = nullptr;
    }
}

LOutput         *ECompositor::createOutputRequest       (const void *params) { return new EOutput(params);     }
LSurface        *ECompositor::createSurfaceRequest      (const void *params) { return new ESurface(params);    }
LPointer        *ECompositor::createPointerRequest      (const void *params) { return new EPointer(params);    }
LKeyboard       *ECompositor::createKeyboardRequest     (const void *params) { return new EKeyboard(params);   }
LToplevelRole   *ECompositor::createToplevelRoleRequest (const void *params) { return new EToplevel(params);   }
LPopupRole      *ECompositor::createPopupRoleRequest    (const void *params) { return new EPopup(params);      }
LSeat           *ECompositor::createSeatRequest         (const void *params) { return new ESeat(params);       }
LDNDManager     *ECompositor::createDNDManagerRequest   (const void *params) { return new EDNDManager(params); }

void ECompositor::destroyToplevelRoleRequest(LToplevelRole *toplevel)
{
    fadeOutSurface((ESurface*)toplevel->surface(), 400);
}

void ECompositor::destroyPopupRoleRequest(LPopupRole *popup)
{
    fadeOutSurface((ESurface*)popup->surface(), 50);
}

void ECompositor::fadeOutSurface(ESurface *surface, UInt32 ms)
{
    if (!surface)
        return;

    LTexture *capture = surface->capture(surface->size() * 2);
    LTextureView *fadeOutView = new LTextureView(capture, &cursorLayer);
    fadeOutView->setBufferScale(2);
    fadeOutView->enableParentOffset(false);
    fadeOutView->setPos(surface->rolePos());

    // Stack it below the software cursor view
    fadeOutView->insertAfter(nullptr);

    LAnimation::oneShot(ms,

    // On Update
    [this, fadeOutView](LAnimation *anim)
    {
        fadeOutView->setOpacity(1.f - anim->value());
        repaintAllOutputs();
    },

    // On Finish
    [this, fadeOutView](LAnimation *)
    {
        repaintAllOutputs();
        delete fadeOutView->texture();
        delete fadeOutView;
    });
}
