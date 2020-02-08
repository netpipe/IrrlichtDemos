#include "irrlicht.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;


void clearAllScene()
{

	ISceneManager* smgr = device->getSceneManager();
	gui::IGUIEnvironment* guienv = device->getGUIEnvironment();

//reset score and lives
scoreValuePoints = 0;
livesValuePoints = 3;

//safely remove elements
//channel = 0;
//channel->setMute(true);
channel1->stop();
channel2->stop();
channel3->stop();
systemFMOD->update();


//disable main menu background image
isBackImg = 0;


//first remove all models loaded, clear the scene, leave some textures

smgr->clear();

guienv->clear();

smgr->getMeshCache()->clear();

}
