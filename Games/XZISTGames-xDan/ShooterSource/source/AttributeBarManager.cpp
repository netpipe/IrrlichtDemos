
#include "AttributeBarManager.h"

namespace abm
{

AttributeBarManager::AttributeBarManager(IrrlichtDevice *device)
{
    this->device = device;
    smgr = device->getSceneManager();
    driver = device->getVideoDriver();

    time = 0;
    margin = 30;
    padding = 5;
}

bool AttributeBarManager::notChanging(int i)
{
    if (attributeBars.size() <= i) return true;
    return attributeBars[i].movingPercent == *attributeBars[i].percent;
}

void AttributeBarManager::render(double dt)
{
    time += dt;

    for (int i = 0; i < attributeBars.size(); i ++)
    {

        int pointDelta = (int) (dt*100.0);
        if (pointDelta < 1) pointDelta = 1;

        if (attributeBars[i].movingPercent < *attributeBars[i].percent)
        {
            attributeBars[i].movingPercent += pointDelta;
            if (attributeBars[i].movingPercent > *attributeBars[i].percent)
            {
                attributeBars[i].movingPercent = *attributeBars[i].percent;
            }
        }
        else if (attributeBars[i].movingPercent > *attributeBars[i].percent)
        {
            attributeBars[i].movingPercent -= pointDelta;
            if (attributeBars[i].movingPercent < *attributeBars[i].percent)
            {
                attributeBars[i].movingPercent = *attributeBars[i].percent;
            }
        }

        video::ITexture *texture = attributeBars[i].texture;
        int percent = attributeBars[i].movingPercent;//*attributeBars[i].percent;
        if (percent > 100) percent = 100;
        core::dimension2du bar = texture->getOriginalSize();
        core::dimension2du screen = driver->getScreenSize();

        video::SColor col,col2;
        col.set(255,100,0,0);
        col2.set(255,255,0,0);
        driver->draw2DRectangle(
            core::rect<s32> (
                margin+padding,
                screen.Height-margin-padding- ((bar.Height-padding*2) * percent)/100,
                margin+bar.Width-padding,
                screen.Height-margin-padding),
            col,col2,col,col2,
            0);

        driver->draw2DImage(texture,
            core::rect<s32> (
                margin,
                screen.Height-margin-bar.Height,
                margin+bar.Width,
                screen.Height-margin),
            core::rect<s32> (
                0,0,
                bar.Width,bar.Height),
            0,
            0,
            true);

    }
}

void AttributeBarManager::clear()
{
    attributeBars.clear();
}

void AttributeBarManager::add(video::ITexture *texture, int *percentFull, char hAlign, char vAlign)
{
    AttributeBar bar;
    bar.texture = texture;
    bar.percent = percentFull;
    bar.movingPercent = *percentFull;
    bar.hAlign = hAlign;
    bar.vAlign = vAlign;
    attributeBars.push_back(bar);
}

};
