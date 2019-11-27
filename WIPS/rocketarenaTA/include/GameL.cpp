/*

Game Logic - cpp file

*/

#include "GameL.h"

// constructor
cGameL::cGameL(cInit *setInit) {
	
	init = setInit;
	
	device = init->getDevice();
	
	smgr = device->getSceneManager();
	
	driver = device->getVideoDriver();
	
	guienv = device->getGUIEnvironment();
	
	eventReceiver = init->getEventReceiver();
	
	// creating menu
	menu.Initialize(init);
	
	// creating player
	player.Initialize(init);
	
	// creating camera handle class
	polaroid.Initialize(init);
	
	// creating level
	world.Initialize(init);
	
	// creating rocket
	rocket.Initialize(init);

	// creating physic world, and collision responds
	physic.Initialize(init);
	physic.createWorld(&world, &player, &rocket);
	
	// adding flags to world animation
	core::array<core::vector3df> posArray;
	posArray=physic.getPolePositionArray();
	world.addFlags(posArray);

	// creating hud
	hud.Initialize(init);
	collisionM = smgr->getSceneCollisionManager();
	
	input.Initialize(init);
	
	// creating sky
	scene::ISceneNode *sky = smgr->addSkyDomeSceneNode(driver->getTexture("media/sky/skyday.png"), 8, 8, 1.f, 1.1f);
	sky->setRotation(core::vector3df(0,-67,0));
	
	time=0;
	timeTemp=0;
	timeTick=16;
	
	mMode=true;
	exit=false;
}

// destructor
cGameL::~cGameL() {
}

void cGameL::drawGame() {

	if (mMode) {
		
		driver->beginScene(true,true,video::SColor(255,0,0,0));
		
		guienv->drawAll();

		menu.drawMenu();
		menu.checkInput();
		
		driver->endScene();
		
		switch (menu.getExit()) {
		case -1:
			exit=true;
			break;
		case 1:
			mMode=false;
			break;
		}

	} else {

		timeTemp += (device->getTimer()->getTime() - time);
		time = device->getTimer()->getTime();

		// draw camera
		polaroid.drawPolaroid(&player);

		player.drawPlayer(&rocket);

//		while (timeTemp > timeTick) {

			// test for any keyboard/mouse input
			input.checkForInput(&player, &rocket, &hud);

			if (player.getPlayerMode()==false)
				rocket.drawRocket(player.getLookAt());

			player.animatePlayer();

			world.drawWorld((scene::ICameraSceneNode*) init->getCamera());

//			timeTemp -= timeTick;
//		}

		// updating physics
		physic.beginPhysic(&player, &rocket);

		// ending physics
		physic.endPhysic();

		u32 futureTime = device->getTimer()->getTime() + 15;
		while (device->getTimer()->getTime() < futureTime) {
			device->run();
		}

		driver->beginScene(true,true,video::SColor(255,62,48,38));
		
		smgr->drawAll();
		guienv->drawAll();
		
		// drawing hud
		hud.drawHud(device, init->getCamera(), &rocket, &player, physic.getPoles());

		driver->endScene();
			

		if (eventReceiver->checkKey(KEY_ESCAPE))
			mMode=true;
			
	}
	
	
	// frames per second
	s32 fps = driver->getFPS();

	if (lastFPS != fps) {
	  core::stringw str = L"Rocket Arena pre Alpha 0.1 - Irrlicht Engine [";
	  str += driver->getName();
	  str += "] FPS:";
	  str += fps;

	  device->setWindowCaption(str.c_str());
	  lastFPS = fps;
	}
	
}

const bool cGameL::getExit() {
	return exit;
}

