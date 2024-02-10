#ifndef ECOMPOSITOR_H
#define ECOMPOSITOR_H
 
#include <LCompositor.h>
#include <LScene.h>
#include <LSceneView.h>
#include <LLayerView.h>
#include <LTextureView.h>
 
using namespace Louvre;
 
class ECompositor : public LCompositor
{
public:
    ECompositor();
 
    void initialized() override;

    // Virtual constructors
    LOutput *createOutputRequest(const void *params) override;
    LSurface *createSurfaceRequest(const void *params) override;
    LPointer *createPointerRequest(const void *params) override;
    LKeyboard *createKeyboardRequest(const void *params) override;
    LToplevelRole *createToplevelRoleRequest(const void *params) override;

    // Scene and layers
    LScene scene;
    LLayerView backgroundLayer  { scene.mainView() };
    LLayerView surfacesLayer    { scene.mainView() };
    LLayerView overlayLayer     { scene.mainView() };
    LLayerView fullscreenLayer  { scene.mainView() };
    LLayerView cursorLayer      { scene.mainView() };
    LTextureView softwareCursor { nullptr, &cursorLayer };
};
 
#endif // ECOMPOSITOR_H
