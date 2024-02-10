#include "Global.h"
#include "ECompositor.h"
#include "ESurface.h"

LScene *G::scene()
{
    return &compositor()->scene;
}

void G::reparentSurfaceAndChildrenViews(ESurface *surface, LView *newParent, bool subsurfaceRoleOnly)
{
    surface->view.setParent(newParent);

    // The children of a surface are always positioned after their parent in the LCompositor::surfaces() list.
    ESurface *next { surface };

    if (subsurfaceRoleOnly)
    {
        while ((next = (ESurface*)next->nextSurface()))
            if (next->isSubchildOf(surface) && next->subsurface())
                next->view.setParent(newParent);
    }
    else
        while ((next = (ESurface*)next->nextSurface()))
            if (next->isSubchildOf(surface))
                next->view.setParent(newParent);
}
