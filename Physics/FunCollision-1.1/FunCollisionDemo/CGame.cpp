// FunCollisionDemo -- Demo application for "FunCollision" -- Created by Peter Szollosi, 2009-2010.
// For conditions of distribution and use, see copyright notice in license.txt or main.cpp

#include <irrlicht.h>
using namespace irr;

#include <iostream>

#include "CGame.h"


namespace GAME
{

//! constructor
CGame::CGame()
{
    device = NULL;
    driver = NULL;
    smgr = NULL;
    guienv = NULL;

    cmgr = NULL;
    timer = NULL;
    iface = NULL;
    input = NULL;
    camera = NULL;
}

//! init
int CGame::init()
{
    // create device
    video::E_DRIVER_TYPE driverType = video::EDT_OPENGL;

	std::cout << "Please select the driver you want for this example:\n" <<
                 " (a) Direct3D 9.0c\n (b) Direct3D 8.1\n (c) OpenGL 1.5\n" <<
                 " (d) Software Renderer\n (e) Burning's Software Renderer\n" <<
                 " (f) NullDevice\n (otherKey) exit\n\n";

	char i;
	std::cin >> i;

	switch(i)
	{
		case 'a': driverType = video::EDT_DIRECT3D9; break;
		case 'b': driverType = video::EDT_DIRECT3D8; break;
		case 'c': driverType = video::EDT_OPENGL; break;
		case 'd': driverType = video::EDT_SOFTWARE; break;
		case 'e': driverType = video::EDT_BURNINGSVIDEO; break;
		case 'f': driverType = video::EDT_NULL; break;
		default: return 1;
	}

	device = irr::createDevice(driverType, core::dimension2d<u32>(800, 600), 32, false, false, false);
	if (!device) return 1;

    // create instances
	driver = device->getVideoDriver();
	smgr = device->getSceneManager();
	guienv = device->getGUIEnvironment();

	// set window caption
    core::stringw caption = L"FunCollision Demo [FunCollision v";
    caption += FUNCOLLISION_VERSION_MAJOR;
    caption += ".";
    caption += FUNCOLLISION_VERSION_MINOR;
    caption += " build: ";
    caption += FUNCOLLISION_VERSION_BUILD;
    caption += "]";
	device->setWindowCaption(caption.c_str());

	// ------ Create collision manager: (don't forget to delete it after you don't need it!)
	cmgr = createCollisionManager(device, 0.01f); // set up a normal gravity to the scene

	// create timer
	timer = new CGameTimer(device);

	// create interfesz
	iface = new CGameInterface(this);

	// create and set inputreceiver
	input = new CGameInput(device);
	device->setEventReceiver(input);

    // create camera
    camera = new CGameCamera(this);

    // hide cursor
	device->getCursorControl()->setVisible(false);

	return 0;
}

//! destructor
CGame::~CGame()
{
    // clean up
    if (camera) delete camera;
    if (input) delete input;
    if (iface) delete iface;
    if (timer) delete timer;

    // ----- Don't forget to drop the collision manager if you don't need it anymore'
    if (cmgr) cmgr->drop();

	if (device) device->drop();
}

//! run
int CGame::run()
{
    // main loop
    bool quit = false;

	while (device->run() && !quit)
	{
	    // do we have to restart the timer?
	    timer->checkTimer();

	    // do we have to restart the collision manager?
	    cmgr->checkTimeing();

		if (device->isWindowActive())
		{
		    input->setLockMouse(true);  // lock mouse to the center of the screen
		    timer->updateTimer();       // update the timer

            // 3D stuff
			driver->beginScene(true, true, video::SColor(255, 50, 150, 255));

            // ------ Update the collision manager:
            cmgr->update(timer->getElapsedTime());

            camera->update();

            smgr->drawAll();

            // 2D drawing
            iface->updateMeters();

            video::SMaterial unlit;
            unlit.Lighting = false;
            unlit.FogEnable = false;
            driver->setMaterial(unlit);
            driver->setTransform(video::ETS_WORLD, core::IdentityMatrix);

            camera->drawDebugData();

            guienv->drawAll();
            // \2D drawing

			driver->endScene();
			// \3D stuff

		    // update input
		    if (input->getKeyReleased(irr::KEY_ESCAPE)) quit = true;
		    input->update();

            // thread delay (uncomment it if you want to lock framerate to refresh rate)
            // timer->threadDelay();
		}
		else
		{
		    // if the window is inactive, we should pause the device and release the mouse
		    device->yield();
		    input->setLockMouse(false);
		}
	}

	return 0;
}

} // end namespace GAME

