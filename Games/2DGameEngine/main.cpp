#include "CTileNode.h"
#include "CTileSet.h"
#include "CTileManager.h"
#include <irrlicht.h>
//#include <irrFunctions.h>

#ifdef _MSC_VER
#pragma comment(lib, "Irrlicht.lib")
//#pragma comment(lib, "irrFunctions.lib")
#endif

using namespace irr;
using namespace zen;

int main()
{
	//video::E_DRIVER_TYPE driverType = video::videoFuncs::driverInit();

	IrrlichtDevice * device = createDevice(video::EDT_OPENGL);

	if (!device)
		return 1;

	device->setWindowCaption(L"Zen Engine Test - Tile Manager");

	video::IVideoDriver * driver = device->getVideoDriver();
	scene::ISceneManager * smngr = device->getSceneManager();

	video::ITexture * texture = driver->getTexture("texture.png");
	driver->makeColorKeyTexture(texture, core::vector2d<irr::s32>(0,0));

	tile::CTileManager * tileManager = new tile::CTileManager;

	tileManager->addTileSet(texture, 32, 32);

	irr::core::vector2df position(0,0);

	tileManager->addTileNode( position , 0, 1);

	while (device->run() && driver)
	{
		if (device->isWindowActive())
		{
			driver->beginScene(true, true, video::SColor(255,120,102,136));

			//draw fire and dragons background world

			driver->draw2DImage(tileManager->getTileSet(0)->getTileSetTexture(), core::vector2d<irr::s32>(0, 0),
				tileManager->getTileSet(0)->getTile(tileManager->getTileNode(0)->getTileID()), 0, video::SColor(255, 255, 255, 255),
				true);

			// draw transparent rectangle under cursor
			core::position2d<irr::s32> m = device->getCursorControl()->getPosition();
			driver->draw2DRectangle(video::SColor(100,255,255,255),
				core::rect<irr::s32>(m.X-20, m.Y-20, m.X+20, m.Y+20));
device->sleep(1);
			driver->endScene();
		}
	}

	device->drop();

	return 0;
}
