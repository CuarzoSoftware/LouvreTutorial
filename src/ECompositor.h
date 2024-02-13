#ifndef ECOMPOSITOR_H
#define ECOMPOSITOR_H
 
#include <LCompositor.h>
#include <LScene.h>
#include <LSceneView.h>
#include <LLayerView.h>
#include <LTextureView.h>
 
using namespace Louvre;

class ESurface;
 
class ECompositor : public LCompositor
{
public:
    ECompositor();
 
    void initialized() override;
    void cursorInitialized() override;
    void uninitialized() override;

    // Virtual constructors
    LOutput *createOutputRequest(const void *params) override;
    LSurface *createSurfaceRequest(const void *params) override;
    LPointer *createPointerRequest(const void *params) override;
    LKeyboard *createKeyboardRequest(const void *params) override;
    LToplevelRole *createToplevelRoleRequest(const void *params) override;
    LPopupRole *createPopupRoleRequest(const void *params) override;
    LSeat *createSeatRequest(const void *params) override;
    LDNDManager *createDNDManagerRequest(const void *params) override;

    // Virtual destructors
    void destroyToplevelRoleRequest(LToplevelRole *toplevel) override;
    void destroyPopupRoleRequest(LPopupRole *popup) override;

    // Fade out surface animation
    void fadeOutSurface(ESurface *surface, UInt32 ms);

    // Scene and layers
    LScene scene;
    LLayerView backgroundLayer  { scene.mainView() };
    LLayerView surfacesLayer    { scene.mainView() };
    LLayerView overlayLayer     { scene.mainView() };
    LLayerView fullscreenLayer  { scene.mainView() };
    LLayerView cursorLayer      { scene.mainView() };
    LTextureView softwareCursor { nullptr, &cursorLayer };

    // Default cursor
    LXCursor *defaultCursor { nullptr };
};
 
#endif // ECOMPOSITOR_H
