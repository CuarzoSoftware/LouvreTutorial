#ifndef EOUTPUT_H
#define EOUTPUT_H

#include <LOutput.h>
#include <LTextureView.h>
#include "ECompositor.h"
#include "ETopbar.h"
#include "Global.h"

using namespace Louvre;

class EToplevel;

class EOutput : public LOutput
{
public:
    EOutput(const void *params);

    void initializeGL() override;
    void moveGL() override;
    void resizeGL() override;
    void paintGL() override;
    void uninitializeGL() override;

    void setGammaRequest(LClient *client, const LGammaTable *gamma) override;

    EToplevel *findFullscreenToplevel() const;
    void updateToplevelsPos();
    void updateToplevelsSize();
    void rescueViewsFromVoid();

    void updateWallpaper();
    LTextureView wallpaperView { nullptr, &G::compositor()->backgroundLayer };

    ETopbar topbar { this };
};

#endif // EOUTPUT_H
