#ifndef G_H
#define G_H

#include <LCompositor.h>

#define TOPBAR_HEIGHT 32
#define TOPBAR_PADDING 4
#define THUMBNAIL_MARGIN 4
#define THUMBNAIL_HEIGHT (TOPBAR_HEIGHT - 2 * TOPBAR_PADDING)

using namespace Louvre;

class ECompositor;
class EOutput;
class ESurface;

class G
{
public:
    static inline ECompositor *compositor()
    {
        return (ECompositor*)LCompositor::compositor();
    }

    static inline std::vector<EOutput*> &outputs()
    {
        return (std::vector<EOutput*>&)LCompositor::compositor()->outputs();
    }

    static inline std::list<ESurface*> &surfaces()
    {
        return (std::list<ESurface*>&)LCompositor::compositor()->surfaces();
    }

    static LScene *scene();

    static void reparentSurfaceAndChildrenViews(ESurface *surface, LView *newParent, bool subsurfaceRoleOnly = false);
};

#endif // G_H
