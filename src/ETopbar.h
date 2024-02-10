#ifndef ETOPBAR_H
#define ETOPBAR_H

#include <LSolidColorView.h>
#include "ECompositor.h"
#include "Global.h"

using namespace Louvre;

class EOutput;

class ETopbar : public LObject
{
public:
    ETopbar(EOutput *output);
    ~ETopbar();

    void copyThumbnailsFromAnotherTopbar();
    void destroyThumbnails();
    void updateGUI();

    EOutput *output;
    LSolidColorView view {
        0.f,                            // Red
        0.f,                            // Green
        0.f,                            // Blue
        0.8f,                           // Alpha
        &G::compositor()->overlayLayer  // Parent view
    };
};

#endif // ETOPBAR_H
