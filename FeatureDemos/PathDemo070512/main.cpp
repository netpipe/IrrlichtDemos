// Small A* pathfinding demo
// writen by Michal Švantner
// date 11.05.2007
// for Irrlicht v. 1.3
//
// pathfinding is wery crude yet, no open list sorting, no post search path
// optimization, no binary heaps
//
// use right mouse button to add/remove wals
// use left mouse button to place player and set his destination

#include <irrlicht.h>
using namespace irr;

#include "MInput.h"
#include "APath.h"
#include "drawMap.h"

int main()
{
    core::dimension2d<s32> mapsize(15,10);
    s32 tilesize = 64;

    MapNode **map;

    PlayerNode *player = NULL;

    //create map nodes
    map = new MapNode*[mapsize.Width];
    for(s32 i=0; i<mapsize.Width; i++) map[i] = new MapNode[mapsize.Height];

    // initialize map nodes
    for(s32 j=0; j<mapsize.Height; j++)
        for(s32 i=0; i<mapsize.Width; i++) map[i][j].setPosition(i, j);

    MyEventReceiver receiver;
	IrrlichtDevice *device =
		createDevice(video::EDT_OPENGL, core::dimension2d<s32>(mapsize.Width * tilesize, mapsize.Height * tilesize), 32, false, false, false, &receiver);

	device->setWindowCaption(L"Pathfinding Demo version 1.0 by arras");

	video::IVideoDriver* driver = device->getVideoDriver();

	// this will draw everithing later
	Graphic graphic(device, map, mapsize.Width, mapsize.Height, tilesize);

	ITimer *timer = device->getTimer();
	u32 time = timer->getTime();
	u32 delta = 500; // controls howe fast will player node move

	while(device->run())
	{
        if(mouse.left_pressed_once())
        {
            if(!player) player = new PlayerNode(&map[mouse.X/tilesize][mouse.Y/tilesize]);
            else
            {
                player->clearWaipoints();
                player->setTarget(&map[mouse.X/tilesize][mouse.Y/tilesize]);
                findPath(map, mapsize.Width, mapsize.Height, player);
            }
        }

        if(mouse.right_pressed_once())
        {
            if(map[mouse.X/tilesize][mouse.Y/tilesize].capacity() == 100) map[mouse.X/tilesize][mouse.Y/tilesize].setCapacity(0);
            else map[mouse.X/tilesize][mouse.Y/tilesize].setCapacity(100);
        }

        if(player)
        {
            if(timer->getTime() > time)
            {
                time = timer->getTime() + delta;
                player->move();
            }
        }

		driver->beginScene(true, true, video::SColor(255,100,101,140));

		// draw everything
		graphic.drawMap(player);

		driver->endScene();
		device->sleep(10);
	}

	device->drop();

	// destroy map nodes
    for(s32 i=0; i<mapsize.Width; i++) delete [] map[i];
    delete [] map;

    if(player) delete player;

	return 0;
}
