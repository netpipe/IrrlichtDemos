#include <irrlicht.h>
#include "AgEngine.h"

#include <iostream>
#include <string.h>
#include "CHUD.h"


// FPSDemo25 , just a simple FPS framework with lua support - incomplete and mostly for research


//#include "world.cpp"

using namespace std;

using namespace irr;
using namespace agEngine;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

using namespace audio;
using namespace entities;


int main(int argc, char** argv)
{
    //Initialisers
    CHUD* CHUD2 = new CHUD();
    // Set up the video
    IrrlichtDevice *device =
        createDevice(EDT_OPENGL, dimension2d<s32>(640, 480), 16,
            false, false, false, 0);

    device->setWindowCaption(L"Agnostic Games - FPS Demo");



/*
WorldBSPName
PakName
PalName

LoadWorld(int ScreenWidth, int ScreenHeight)

GetStartPosition(GLfloat* camerapos)

DrawWorld(int ScreenWidth, int ScreenHeight, GLfloat* camerapos) in your rendering routine

WM_CLOSE
*/


    IVideoDriver* driver = device->getVideoDriver();
    ISceneManager* smgr = device->getSceneManager();
    IGUIEnvironment* guienv = device->getGUIEnvironment();

    // Set up the audio
    CAudioDevice* adevice = new CAudioDevice();

    // Set up the entity manager
    CEntityManager* emgr = new CEntityManager(smgr, device->getFileSystem(), device->getLogger());

    // Get the entity manager to produce us a playerentity
    CPlayerEntity* player = emgr->addPlayerEntity();

    // Create a new camera scene node and associate it with the player entity
    player->associateWithSceneNode(smgr->addCameraSceneNode(0, vector3df(0,30,-40), vector3df(0,5,0)));

    CHUD2->RegisterDevice(device);
    CHUD2->LoadHUD("HUD.xml");

    while(device->run())
    {
        adevice->playAll();
        driver->beginScene(true, true, SColor(0,200,200,200));
         CHUD2->DisplayHUD();
        smgr->drawAll();
        guienv->drawAll();
        driver->endScene();
        device->sleep(11,0);
    }

    emgr->drop();
    adevice->drop();
    device->drop();

    return 0;
}

