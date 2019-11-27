#include <irrlicht.h>
#pragma comment(lib, "Irrlicht.lib")
#include <iostream>


using namespace irr;
using namespace irr::gui;
using namespace irr::core;
using namespace irr::scene;
using namespace irr::video;


IrrlichtDevice *device; // Only global needed for the camera rig
scene::IMetaTriangleSelector* metaSelector;
scene::ISceneNodeAnimatorCollisionResponse* anim;

bool underwter; // to see if underwater

//bool firstupdate;

// Camera Controller functions for the camera rig.
#include <gui/gamegui.h>
#include <game/gameinfo.h>
#include <camera/RiggedCamera.h>
#include <collision/metaselector.h>

class mainEvents : public IEventReceiver
{
      bool OnEvent(const SEvent& event)

    {Rig rig; //Use the rig class
     if(event.EventType == EET_KEY_INPUT_EVENT &&
            event.KeyInput.Key == KEY_ESCAPE &&
            !event.KeyInput.PressedDown)
            {
             device->closeDevice();
             return true;
            }
     rig.OnEvent(event); // pass the events to the class   (not working)
    }

};
////////////////////////////////////////////////////////////////////////////////
// FUNCTION to define the default Camera and apply the collision based on the
// Metatriangle selector
////////////////////////////////////////////////////////////////////////////////
void player()
    {
    Gameinfo game;
    Rig rig; // need info from the rig
    printf ("Get The scene manager\n");
    //scene::ISceneManager *smgr = game.smgr;
    // Prepare the basic collision system
    metaSelector = device->getSceneManager()->createMetaTriangleSelector();
    //game.camera=camera;
    printf("launching the recursion: adding to the metaselector for all the OCCtree scene nodes\n");
    recursiveFillMetaSelector(device->getSceneManager()->getRootSceneNode(), metaSelector );

    // Define the collision reponse for the FPS Camera
    anim = 0;
    anim = device->getSceneManager()->createCollisionResponseAnimator(metaSelector,rig.getCamera(), core::vector3df(10,26,10),
    //anim = smgr->createCollisionResponseAnimator(metaSelector, camera, core::vector3df(10,26,10),
      core::vector3df(0,-1.0f,0),
      core::vector3df(0,game.head,0));

      //core::vector3df(0,0,0));
      // Apply the collision response to the camera
      rig.getCamera()->addAnimator(anim);

    anim->drop();
    metaSelector->drop();
}

////////////////////////////////////////////////////////////////////////////////
// FUNCTION to create a water surface
//
////////////////////////////////////////////////////////////////////////////////
void waternode(f32 width=10, f32 height=10, vector3df pos=vector3df(0,0,0), u32 xdiv=10, u32 zdiv=10)
{
     ISceneNode* node;
     width/=xdiv; height/=zdiv;
     IAnimatedMesh *plane = device->getSceneManager()->addHillPlaneMesh("test",dimension2d<f32>(width,height),dimension2d<u32>(xdiv,zdiv));
     node = device->getSceneManager()->addWaterSurfaceSceneNode(plane->getMesh(0), 6, 1000, 10);
     node->setPosition(core::vector3df(pos));
     node->setMaterialFlag(EMF_LIGHTING,false);
     node->setMaterialFlag(EMF_BACK_FACE_CULLING,false);
     node->setMaterialFlag(EMF_FOG_ENABLE,true);
     node->setMaterialTexture(0, device->getVideoDriver()->getTexture("levels/media/agua.png"));
     node->getMaterial(0).MaterialType= EMT_TRANSPARENT_ALPHA_CHANNEL   ;
}

////////////////////////////////////////////////////////////////////////////////
// FUNCTION to update the water / underwater
// Need the camera position to update (taken from rig class)
////////////////////////////////////////////////////////////////////////////////
void underwater(f32 width=10, f32 height=10, vector3df pos=vector3df(0,0,0))
{
     Rig CamRig; // Need info from the rig (Cam position)
     video::IVideoDriver *driver=driver=device->getVideoDriver();
     float x0=pos.X; float y0=pos.Y; float z0=pos.Z;
     vector3df cam=CamRig.getCamera()->getPosition(); //underwter=true; else underwter=false;
     if ((cam.X>x0-(width/2)) && (cam.X<x0+(width/2)) && (cam.Z>z0-(height/2)) && (cam.Z<z0+(height/2)))
     {  if (cam.Y<y0)
           { driver->setFog(SColor(0,128,152,192), EFT_FOG_LINEAR, 0.0f, 360.0f, 0.1f, true );
             anim->setGravity(vector3df(0,0,0));
             underwter=true; CamRig.rigAction=(L"Underwater");
           }
        if (cam.Y<y0-1.0f && underwter) anim->setGravity(vector3df(0,0.00001f,0));
        if (cam.Y>y0+3 && underwter) {driver->setFog(SColor(0,128,128,128), EFT_FOG_LINEAR, 500.0f, 2500.0f, 0.01f, true );}
        if (cam.Y>y0+30 && underwter) {anim->setGravity(vector3df(0,-0.001,0));}
     } else {if (underwter) {underwter=false; CamRig.rigAction=(L"Back on ground");}}
}

////////////////////////////////////////////////////////////////////////////////
// Main() function
//
////////////////////////////////////////////////////////////////////////////////
int main()
{
    Gameinfo game; // retrieve the gameinformations class
    Rig CamRig;  // Class of the rig
    gamegui GGui; // Class of the GUI
    mainEvents main;


    // Main function of the game
    // To be revised
    // Other functions to define as loader screen, intro, main game, etc
    // Current code is for main game
    //MyEventReceiver receiver; // retrieve the event receiver class
    //game.param.EventReceiver = &receiver;
    game.param.EventReceiver = &CamRig; // temporary solution. The Event Received need to be separated from the Rig (Menu / Rig functions)
    game.param.Bits = 32;
    printf ("Create a device extended\n");
    game.Init(); // Initialize the game, open the device/screen with the parameters
    if (device == 0)  return 1;
    printf("Device Created\n");
    device->getVideoDriver()->beginScene(true, true, video::SColor(0,0,0,0));
    device->getVideoDriver()->setFog(SColor(0,128,128,128), EFT_FOG_LINEAR, 500.0f, 2500.0f, 0.01f, true );
    device->getVideoDriver()->endScene();
    core::stringw str = "First King: FPS Camera rig Demo - Loading, one moment please";
    device->setWindowCaption(str.c_str());
    game.loadscene(); // load the IRR Scene
    printf("Setup the camera rig\n");
    CamRig.firstupdate=true;
    CamRig.rigAction=(L"still");
    CamRig.createFPSrig(device); // Create the player rig camera (FPS Camera)
    printf("Setup the environnement collision system\n");
    player(); // Setup the camera collision
    waternode(2300,2000,vector3df(-2900,-20,-500)); // Create water

	device->getCursorControl()->setVisible(false); // hide the mouse cursor
    // Game loop
    int lastFPS = -1;
    stringw lastAction=L"";
    // Initialize the gui
    GGui.Init();
    GGui.change(CamRig.walksimmode,CamRig.smoothmode,CamRig.tiltmode,CamRig.slidemode);
    GGui.currentX=800;
    while(device->run())
   {

      if (device->isWindowActive())
      {
        CamRig.OnAnimate(device->getTimer()->getTime());    // This update the camera rig
        underwater(2300,2000,vector3df(-2900,-20,-500)); // This update the water
        device->getVideoDriver()->beginScene(true, true, video::SColor(0,0,0,0));
        device->getSceneManager()->drawAll();
        device->getGUIEnvironment()->drawAll();
        int fps = device->getVideoDriver()->getFPS();
        GGui.gamemode=CamRig.gamemode;
        if (GGui.update())  // Update the GUI
             {CamRig.changemode(GGui.walksim,GGui.smooth,GGui.tilt,GGui.slide);
              GGui.gamemode=CamRig.gamemode; // update the gamemode
             }
        device->getVideoDriver()->endScene();
		if ((lastFPS != fps) || (lastAction != CamRig.rigAction.c_str()))
		{
			core::stringw tmp(L"First King: FPS Camera rig Demo - FPS:");
			tmp += fps;
			tmp += L" Action: ";
			tmp += CamRig.rigAction.c_str();
			tmp += L" : TAB-Menu";
            //printf("action= %s \n",CamRig.rigAction.c_str());
			device->setWindowCaption(tmp.c_str());
			lastAction = CamRig.rigAction.c_str();
			lastFPS = fps;
		}

      }  else device->yield(); //Give time to refresh with the system(Very nice!)
   }
    device->closeDevice();
    return 0;
}
