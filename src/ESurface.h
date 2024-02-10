#ifndef ESURFACE_H
#define ESURFACE_H

#include <LSurface.h>
#include <LSurfaceView.h>
#include "ECompositor.h"
#include "Global.h"

class EThumbnail;
class EOutput;
class EToplevel;

using namespace Louvre;

class ESurface : public LSurface
{
public:
    ESurface(const void *params);
    ~ESurface();

    inline EToplevel *tl()
    {
        return (EToplevel*)toplevel();
    };

    void roleChanged() override;
    void orderChanged() override;
    void minimizedChanged() override;
    void mappingChanged() override;
    void parentChanged() override;

    // Output where the surface is currently most visible
    EOutput *primaryOutput();

    // Take a snapshot of the surface
    LTexture *capture(const LSize &dstSizeB);

    LSurfaceView view { (LSurface*) this, &G::compositor()->surfacesLayer };

    bool firstMapping { true };

    // Single texture shared by all thumbnail views
    LTexture *thumbnailTexture { nullptr };

    // Thumbnail views, one for each output's topbar
    std::vector<EThumbnail*> thumbnailViews;

    // The output where the surface was most visible before being minimized
    EOutput *outputBeforeMinimize { nullptr };

    // The relative % surface position to outputBeforeMinimize
    LPointF relativePosBeforeMinimize;
};

#endif // ESURFACE_H
