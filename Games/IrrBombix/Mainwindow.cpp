/*
 * based on The Game Loop by Koen Witters
 *
 */

#include "Mainwindow.h"
#include "GameIrrlicht.h"
#include "GameItemIrrlicht.h"
#include "irrlicht.h"
#include "IrrlichtEventReceiver.h"
#include "GameStateManager.h"
#include "States/StateInGame.h"
#include "States/StateBeforeGame.h"
#include "States/StateAfterGame.h"
#include "States/StatePause.h"
#include "States/StatePlayerSelectionboard.h"
#include "States/StateScoreboard.h"
#include "States/StateOptionsboard.h"
#include "States/StateArenaSelectboard.h"
#include "States/StateDrawGame.h"
#include "States/StateIntro.h"
#include "States/StateCredit.h"
#include "PlayerIrrlicht.h"
#include "ControllerManager.h"
#include "SoundManager.h"

#include "Serializer.h"
#include "Deserializer.h"

#include "PlayerItemIrrlicht.h"

// random
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sstream>

irr::IrrlichtDevice* Mainwindow::device;
bool Mainwindow::endloop=false;

Mainwindow::Mainwindow() {
	// Initialize the game

	// init game
	initMainwindow();

	// Setup the window


}

Mainwindow::~Mainwindow() {

}

void Mainwindow::readFrom(Deserializer *d) {
	d->beginAttribute("grafic");
	long tmp;
	d->readParameter("driver", tmp);
	driverType = (irr::video::E_DRIVER_TYPE) tmp;

	d->readParameter("width", tmp);
	screenwidth = tmp;
	d->readParameter("height", tmp);
	screenheight=tmp;
	d->readParameter("fullscreen", tmp);
	param.Fullscreen=tmp;


}


void Mainwindow::writeTo(Serializer *s) {
	s->beginAttribute("grafic");
	long tmp = driverType;
	s->writeParameter("driver", tmp);

	s->writeParameter("width", (long)screenwidth);
	s->writeParameter("height", (long)screenheight);
	s->writeParameter("fullscreen", (long)param.Fullscreen);
	s->endAttribute();
	s->endAttribute();
}


void Mainwindow::initMainwindow() {
	/* public variables */
	screenwidth = 1680;
	screenheight = 1050;

	screenwidth = 800;
	screenheight = 600;

	new SoundManager();

	// default
	param.Fullscreen = false;

	Deserializer* d = new Deserializer("./config/grafics.xml");
	if (d->fileok) {
		readFrom(d);
	} else {
		//screenwidth = 1024;
		//screenheight =768;
		printf("Please select the driver you want for the game:\n"
			" (a) OpenGL\n (b) Direct3D 9.0c\n (c) Direct3D 8.1\n"
			" (d) Burning's Software Renderer (not recommend)\n (e) Software Renderer(not recommend)\n"
			" (f) NullDevice(not recommend)\n (otherKey) exit\n\n");

		char i = 'a';
		std::cin >> i;

		switch (i) {
		case 'a':
			driverType = irr::video::EDT_OPENGL;
			break;
		case 'b':
			driverType = irr::video::EDT_DIRECT3D9;
			break;
		case 'c':
			driverType = irr::video::EDT_DIRECT3D8;
			break;
		case 'd':
			driverType = irr::video::EDT_BURNINGSVIDEO;
			break;
		case 'e':
			driverType = irr::video::EDT_SOFTWARE;
			break;
		case 'f':
			driverType = irr::video::EDT_NULL;
			break;
		default:
			return;
		}
		std::cout << "saving configs in ./config/grafics.xml\ndelete it to rerun this config\n";
		writeTo(new Serializer("./config/grafics.xml"));
	}
	eventreceiver = new IrrlichtEventReceiver;

	//irr::SIrrlichtCreationParameters param;

	param.DriverType = driverType;
	param.WindowSize = irr::core::dimension2d<irr::u32>(screenwidth,
			screenheight);
	// param.Fullscreen = true;
	param.Bits = 16;
	param.Stencilbuffer = false;
	param.Vsync = false;
	param.AntiAlias = true;
	param.EventReceiver = eventreceiver;

	device = createDeviceEx(param);

	// create device and exit if creation failed

	//device = irr::createDevice(driverType, irr::core::dimension2d<irr::u32>(
	//	screenwidth, screenheight), 16, true, false, true, eventreceiver);

	//device = irr::createDevice(driverType, irr::core::dimension2d<irr::u32>(
	//		screenwidth, screenheight), 16, false, false, false, eventreceiver);

	// inits all controllers too
	eventreceiver->setDevice(device);

	GameIrrlicht* game = new GameIrrlicht();

	srand( time(NULL));

	IrrlichtEventReceiver* tmp2 =
			static_cast<IrrlichtEventReceiver*> (device->getEventReceiver());
	ControllerManager* playerController = tmp2->getPlayerController();

	StateInGame* tmp = new StateInGame("InGame");
	tmp->m_game->playerController = playerController;
	tmp->m_game->setDevice(device);
	//tmp->m_game->initGame();

	statemanager.playerController = playerController;



	statemanager.addGameState(tmp, false);

	statemanager.addGameState(new StatePlayerSelectionboard(
			"PlayerSelectionboard"), false);
	statemanager.addGameState(new StateScoreboard("Scoreboard"), false);
	statemanager.addGameState(new StateBeforeGame("BeforeGame"), false);
	statemanager.addGameState(new StateAfterGame("AfterGame"), false);
	statemanager.addGameState(new StateArenaSelectboard("ArenaSelectboard"),
			false);
	statemanager.addGameState(new StateOptionsboard("Optionsboard"), false);
	// pause after Options because of cam
	// TODO fix start
	statemanager.addGameState(new StatePause("Pause"), false);
	statemanager.addGameState(new StateDrawGame("DrawGame"), false);
	statemanager.addGameState(new StateIntro("Intro"), false);
	statemanager.addGameState(new StateCredit("Credit"), true);
}

void Mainwindow::loopMainwindow() {

	int lastFPS = -1;

	int state = 0;

	int TICKS_PER_SECOND = 25;

	const int MAX_FRAMESKIP = 5;

	float next_game_tick = device->getTimer()->getTime();
	int loops;
	float interpolation;

	statemanager.playerController->startEventProcess();
	endloop = device->run();
	bool didwhile = false;

	while (endloop) {
		loops = 0;
		//std::cout << "Tack\t";
		didwhile = false;

		TICKS_PER_SECOND = statemanager.getUpdatesPerSecond();
		int SKIP_TICKS = 1000 / TICKS_PER_SECOND;

		while (device->getTimer()->getTime() > next_game_tick && loops
				< MAX_FRAMESKIP) {
			didwhile = true;
			statemanager.updateState();
			//std::cout << "Tick\t";
			next_game_tick += SKIP_TICKS;
			loops++;
			// clear the keys
			statemanager.playerController->startEventProcess();
			// update keys timer ..
			endloop = device->run();

		}
		// update once a loop timer
		if (!didwhile) {
			//statemanager.playerController->startEventProcess();
			endloop = device->run();
		}

		//interpolation = float( GetTickCount() + SKIP_TICKS - next_game_tick ) / float( SKIP_TICKS );
		//display_game( interpolation );

		//device->getVideoDriver()->beginScene(true, true, irr::video::SColor(255, 150, 201, 240));
		device->getVideoDriver()->beginScene(true, true, irr::video::SColor(
				255, 0, 0, 0));
		statemanager.render();
		device->getVideoDriver()->endScene();
		//GameIrrlicht::m_GameItemIrrlicht->update();


		int fps = Mainwindow::device->getVideoDriver()->getFPS();

		if (lastFPS != fps) {
			irr::core::stringw tmp(L"");

			tmp += L"fps: ";
			tmp += fps;

			device->setWindowCaption(tmp.c_str());
			lastFPS = fps;
		}

	}

}

