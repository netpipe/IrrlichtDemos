///////////////////////////////////////////////////////////////////////////////
//  The first king camera rig system, By Christian Clavet
//  Here are the commands that can be used:
////////////////////////////////////////////////////////////////////////////////
// COMMANDS
////////////////////////////////////////////////////////////////////////////////
// void createFPSrig(IrrlichtDevice * deviceparm)
//
// Create a FPS rig and get the DEVICE pointer for the entire class
// This FPS rig is composed of a Camera and 2 empty nodes for controls
//------------------------------------------------------------------------------
// void OnAnimate(u32 TimeMS)
//
// Time based animation function
// This will animate the rig and calculate the time
// This "animate function" do the Rig assembly
//------------------------------------------------------------------------------
// void changemode(bool wsm, bool smm, bool tlm, bool slm)
//
// Activate/Deactivate certain mode function of the rig
// Here are the modes:
// wsm = walksim mode (Default TRUE)
// smm = smooth movement mode (Default TRUE)
// tlm = Tilt when strafe (Default TRUE)
// slm = Slide when stop moving (Default TRUE)
// use example: CamRig.changemode(true, true, true, true)
//      This will activate all the features offered by the rig
//------------------------------------------------------------------------------
// void setRigSpeed(int mode, float move, float strafe, float jump)
//
// Set the defined speed for each action movement
// modes are: 0=run, 1=walk, 2=crouch, 3=crawl
// use example: CamRig.setRigSpeed(0,5.0f,3.0f,5.0f);
//      This will set the speed for RUN to:
//                                   5 units forward/backward
//                                   3 units left/right
//                                   5 units for jumping
//------------------------------------------------------------------------------
// void setRigTilt(int mode, float tilt, float speed)
//
// Set the tilt angle of the camera for certain actions
// modes are 0=Tilt angle when strafing, 1 Tilt angle when head tilt
// use example: CamRig.setRigTilt(0,4.5f,0.2f);
//       This will set the tilt on strafe to 4.5 degrees, with a speed value of
//       0.2 increments for the speed of the move
//------------------------------------------------------------------------------
// void setRigCrouch(int mode, float value, float speed)
//
// Set the crouch values
// modes are: 0=crouch, 1=crawl
// use example: CamRig.setRigCrouch(0,-30.0f,0);
//       This will set the crouch value to lower minus 30 unit lower the rig
//       when using that action
//------------------------------------------------------------------------------
// void setRigWalk(float amplitude)
//
// set the amplitude of the walk simulator (if used)
// use example: CamRig.setRigWalk(0.5f);
//       This will lower the amplitude in half of the default value
//       The parameter multiply the value
//------------------------------------------------------------------------------
// void setRigSceneUpDownMax(float up, float down)
//
// This define the limit in case of falling outside the level
// Will position the rig at the UP max if falling
//------------------------------------------------------------------------------
// void setRigTargetDistance(float distance)
//
// Set the target distance from the camera. It is recommended that you use
// a high value like (1000-2000, etc).
// Warning: could exibit weird behavior if distance is short and walksim is on.
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// FUNCTIONS
////////////////////////////////////////////////////////////////////////////////
// scene::ICameraSceneNode * getCamera()
//
// Return the current pointer of the camera
// use example: CamRig.getCamera()->setFOV(45*DEGTORAD);
//      The rig camera will change the Field of view to 45 degree
//------------------------------------------------------------------------------
// float getCurrentSpeed()
//
// Return the current rig movement speed
// use example: float speed=CamRig.getCurrentSpeed();
//         Will retrieve the current speed the rig moves (forward/backward)
//------------------------------------------------------------------------------
// stringw getRigState()
//
// Function to return the rig state in a text string (temporary)
// This will need to be changed to something more convenient
////////////////////////////////////////////////////////////////////////////////


#include <irrlicht.h>
#pragma comment(lib, "Irrlicht.lib")
#include <iostream>


using namespace irr;
using namespace irr::gui;
using namespace irr::core;
using namespace irr::scene;
using namespace irr::video;


IrrlichtDevice *device=0; // Only global needed for the camera rig
#include "RiggedCamera.h"  // This is the camera rig functions include
#include "eventReceiver.h" // Nice code from CuteAlien to have multiple events receiver


// Use this event handler to have the ESC quit the game
// Notice that the keys used by the camera rig are not defined here
// Thanks to CuteAlien for givin me the solution
class gameReceiver : public IEventReceiver
{
   public :
   bool OnEvent(const SEvent& event)
   {     //(ESC->quit)
         if(event.EventType == EET_KEY_INPUT_EVENT &&
            event.KeyInput.Key == KEY_ESCAPE &&
            !event.KeyInput.PressedDown)
            {
             device->closeDevice();
             return true;
            }
   }
};

// This only create a surface with the collision. Nothing special
void waternode(f32 width=10, f32 height=10, vector3df pos=vector3df(0,0,0), u32 xdiv=10, u32 zdiv=10)
{
     Rig CamRig;

     // Add Light
     device->getSceneManager()->addLightSceneNode(0,vector3df(0,350,0),SColor(0,255,255,255),1000);

     // Create the flat surface
     ISceneNode* node;
     width/=xdiv; height/=zdiv;
     IAnimatedMesh *plane = device->getSceneManager()->addHillPlaneMesh("test",dimension2d<f32>(width,height),
     dimension2d<u32>(xdiv,zdiv),0,0.0f,core::dimension2d< f32 >(0.0f, 0.0f),
     core::dimension2d< f32 >(100.0f, 100.0f));
     node = device->getSceneManager()->addWaterSurfaceSceneNode(plane->getMesh(0), 0, 1000, 2);
     node->setPosition(core::vector3df(pos));
     node->setMaterialFlag(EMF_LIGHTING,true);
     node->setMaterialFlag(EMF_BACK_FACE_CULLING,false);
     node->setMaterialFlag(EMF_FOG_ENABLE,false);
     node->setMaterialTexture(0, device->getVideoDriver()->getTexture("BricksLg.png"));
     node->getMaterial(0).MaterialType= EMT_SOLID;

     // create a ball
     ISceneNode* node1=device->getSceneManager()->addSphereSceneNode(2000,24);
     node1->setPosition(vector3df(0,50,0));
     node1->setMaterialFlag(EMF_LIGHTING,false);
     node1->setMaterialFlag(EMF_BACK_FACE_CULLING,false);
     node1->setMaterialFlag(EMF_FOG_ENABLE,false);
     node1->setMaterialTexture(0, device->getVideoDriver()->getTexture("agua.png"));
     node1->getMaterial(0).MaterialType= EMT_SOLID;
     // NEW -> Create so that the scene can collide with the cam (better demo)
     // Convert it to and occtree
     ISceneNode * ground = device->getSceneManager()->addOctreeSceneNode(plane,node);
     // Create a triangle selector for collision
     ITriangleSelector * selector = 0;
     selector=device->getSceneManager()->createOctTreeTriangleSelector(plane->getMesh(0),ground);
     // Now create an Collision response animator for the cam
     ISceneNodeAnimatorCollisionResponse * anim = 0;
     anim=device->getSceneManager()->createCollisionResponseAnimator(selector,CamRig.getCamera(),
     vector3df(10,26,10), vector3df(0,-1.0f,0), vector3df(0,34,0));
     // Apply the defined animation control on the camera rig
     CamRig.getCamera()->addAnimator(anim);
     // Set the ambiant color
     device->getVideoDriver()->setAmbientLight(SColor(0,128,255,0));

     selector->drop();
     anim->drop();



}

int main()
{
    // Define the classes
    MainEventReceiver receiver;                 // define the CuteAlien's class
    Rig CamRig;                                           // Init the rig class
    gameReceiver receiver1;  // define this class for using the ESC key to quit

    // Init the device
    video::E_DRIVER_TYPE driverType = video::EDT_OPENGL;
    device = createDevice(driverType, core::dimension2d<u32>(800, 600),
      16, false, true, false);
    if (device == 0)
      return 1;

    // Multiple Event receivers functionnality by CuteAlien
    device->setEventReceiver(&receiver); // Use the Cutealien's class as main event handler
    receiver.AddEventReceiver(&CamRig); // add the camera rig to it
    receiver.AddEventReceiver(&receiver1); // add the current event receiver to it

    // Create the player rig camera (FPS Camera)
    CamRig.createFPSrig(device);
    printf("Setup the camera rig\n");
    CamRig.getCamera()->setPosition(vector3df(0,150,0)); // set the new position of the camera
    CamRig.getCamera()->setFOV(45 * DEGTORAD);

   // Create the environnement
    waternode(3000,3000,vector3df(0,0,0));


    int lastFPS = -1;
    while(device->run())
   {
      if (device->isWindowActive())
      {
         //Update the camera rig
         CamRig.OnAnimate(device->getTimer()->getTime());
         // Start and draw the scene
         device->getVideoDriver()->beginScene(true, true, video::SColor(0,0,0,0));
         device->getSceneManager()->drawAll();
         device->getGUIEnvironment()->drawAll();
         int fps = device->getVideoDriver()->getFPS();
         device->getVideoDriver()->endScene();
         if (lastFPS != fps)
		 {
			core::stringw tmp(L"First King: FPS Camera rig Demo - FPS:");
			tmp += fps;
			device->setWindowCaption(tmp.c_str());
			lastFPS = fps;
		 }
      }
      else device->yield();
   }

   device->drop();
   return 0;
}

