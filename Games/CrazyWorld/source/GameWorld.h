#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <irrlicht.h>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include "Player.h"
#include "AI.h"

using namespace irr;
using namespace core;

class GameWorld
{
public:
	GameWorld(void);
	GameWorld(IrrlichtDevice* device);	
	bool run();
	void gameLoop();
	void updateHud();
	~GameWorld(void);

	IrrlichtDevice* device;
	scene::ISceneManager* smgr;
	video::IVideoDriver* driver;
	gui::IGUIEnvironment* gui;
	scene::ICameraSceneNode* camera;

	gui::IGUIFont* font;

	Player* p;
	AI* gomgr;
};
