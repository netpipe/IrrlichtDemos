// car_test.cpp : Defines the entry point for the console application.
//

#include <irrlicht.h>
#include <ode/ode.h>

#include "GameContext.h"

#ifdef _IRR_WINDOWS_
#pragma comment(lib, "Irrlicht.lib")
#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#endif

using namespace irr;
using namespace core;
using namespace gui;

int main()
{
	// init Irrlicht
	IrrlichtDevice *device = createDevice(video::EDT_OPENGL, core::dimension2d<u32>(1024,768), 16, false, false, false);
	if (!device)
		return 1;

	device->setWindowCaption(L"Cars and Walls");

	// init ODE
	dInitODE2(0);

	CGameContext * game = new CGameContext(device);

	game->mainMenu();

	while (device->run()){
		if (device->isWindowActive())
    		game->gameStep();
device->sleep(10);
}
	delete game;

	// destroy ODE
	dCloseODE();

	// destroy irricht
	device->drop();

	return 0;
}

