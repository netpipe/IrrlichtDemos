/*

Game Logic - header file

*/

#ifndef __GameL_h
#define __GameL_h

#include <irrlicht.h>
#include <iostream>

using namespace irr;
using namespace gui;
using namespace scene;

#include <Init.h>
#include <Menu.h>
#include <Player.h>
#include <Polaroid.h>
#include <World.h>
#include <Rocket.h>
#include <Physic.h>
#include <Hud.h>
#include <Input.h>


class cGameL {
public:
	
	// constructor
	cGameL(cInit *setInit);
	
	// destructor
	~cGameL();
	
	void drawGame();
	
	// get exit status
	const bool getExit();
	
	
private:
	
	IrrlichtDevice *device;
	scene::ISceneManager *smgr;
	video::IVideoDriver *driver;
	IGUIEnvironment *guienv;
	cEventReceiver *eventReceiver;
	scene::ISceneCollisionManager *collisionM;
	cInput input;

	cHud hud;
	
	cInit *init;
	
	cMenu menu;
	
	cPlayer player;
	
	cRocket rocket;
	
	cPolaroid polaroid;
	
	cWorld world;
	
	cPhysic physic;

	u32 time;
	u32 timeTemp;
	u32 timeTick;

	bool mMode;
	bool exit;
	
	s32 lastFPS;
	
};

#endif
