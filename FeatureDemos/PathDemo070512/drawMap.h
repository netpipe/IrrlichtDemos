#ifndef DRAWMAP_H
#define DRAWMAP_H

#include <irrlicht.h>
using namespace irr;
using namespace std;

#include "APath.h"

class Graphic
{
    video::IVideoDriver* driver;
    gui::IGUIEnvironment* guienv;
    gui::IGUIFont* font;

    MapNode** map;
    int mapsizeX;
    int mapsizeY;

    int tilesize;

    video::ITexture* grass;
    video::ITexture* wall;
    video::ITexture* player;
    video::ITexture* target;

public:
    Graphic(IrrlichtDevice *device, MapNode** imap, int width, int height, int itilesize)
    {
        driver = device->getVideoDriver();
        guienv = device->getGUIEnvironment();
        font = guienv->getBuiltInFont();

        map = imap;
        mapsizeX = width;
        mapsizeY = height;
        tilesize = itilesize;

        grass = driver->getTexture("media/grass.bmp");
        wall = driver->getTexture("media/wall.bmp");
        player = driver->getTexture("media/player.bmp");
        driver->makeColorKeyTexture(player, core::position2d<s32>(0,0));
        target = driver->getTexture("media/target.bmp");
        driver->makeColorKeyTexture(target, core::position2d<s32>(0,0));
    }
    ~Graphic(){}

    void drawMap(PlayerNode *node)
    {
        for(int i=0; i<mapsizeX; i++)
            for(int j=0; j<mapsizeY; j++)
            {
                if(map[i][j].capacity() == 100) driver->draw2DImage(grass, core::position2d<s32>(i * tilesize, j * tilesize));
                else driver->draw2DImage(wall, core::position2d<s32>(i * tilesize, j * tilesize));
            }

        if(node)
        {
            MapNode *last = node->getPosition();
            for(s32 i=0; i<node->getWaipointsCount(); i++)
            {
                MapNode *next = node->getWaipoint(i);
                driver->draw2DLine(core::position2d<s32>(last->X()*tilesize+tilesize/2, last->Y()*tilesize+tilesize/2),
                    core::position2d<s32>(next->X()*tilesize+tilesize/2, next->Y()*tilesize+tilesize/2), video::SColor(255,255,255,255));
                last = next;
            }

            if(node->getTarget())
                driver->draw2DImage(target, core::position2d<s32>(node->getTarget()->X()*tilesize, node->getTarget()->Y()*tilesize),
                    core::rect<s32>(0,0,tilesize,tilesize), 0, video::SColor(255,255,255,255), true);

            driver->draw2DImage(player, core::position2d<s32>(node->getPosition()->X()*tilesize, node->getPosition()->Y()*tilesize),
                core::rect<s32>(0,0,tilesize,tilesize), 0, video::SColor(255,255,255,255), true);
        }
    }
};

#endif
