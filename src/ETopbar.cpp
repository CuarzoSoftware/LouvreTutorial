#include "ETopbar.h"
#include "EOutput.h"
#include "EThumbnail.h"
#include "Global.h"

ETopbar::ETopbar(EOutput *output) : output(output) {}

ETopbar::~ETopbar()
{
    destroyThumbnails();
}

void ETopbar::copyThumbnailsFromAnotherTopbar()
{
    for (EOutput *o : G::outputs())
    {
        if (o == output)
            continue;

        for (EThumbnail *item : (std::list<EThumbnail *>&)o->topbar.view.children())
            new EThumbnail(this, item->surface);

        break;
    }

    updateGUI();
}

void ETopbar::destroyThumbnails()
{
    while (!view.children().empty())
        delete view.children().back();
}

void ETopbar::updateGUI()
{
    view.setSize(output->size().w(), TOPBAR_HEIGHT);
    view.setPos(output->pos());

    Int32 x { TOPBAR_PADDING };
    for (EThumbnail *thumbnail : (std::list<EThumbnail*>&)view.children())
    {
        thumbnail->setPos(x, TOPBAR_PADDING);
        x += thumbnail->size().w() + THUMBNAIL_MARGIN;
    }

    output->repaint();
}
