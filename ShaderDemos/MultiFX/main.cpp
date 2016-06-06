/*

This Tutorial shows how to load a Quake 3 map into the

engine, create a SceneNode for optimizing the speed of

rendering and how to create a user controlled camera.



Lets start like the HelloWorld example: We include

the irrlicht header files and an additional file to be able

to ask the user for a driver type using the console.

*/

#include <irrlicht.h>

#include <iostream>


//Include the CEffects header

#include "CEffects.h"



/*

As already written in the HelloWorld example, in the Irrlicht

Engine, everything can be found in the namespace 'irr'.

To get rid of the irr:: in front of the name of every class,

we tell the compiler that we use that namespace from now on,

and we will not have to write that 'irr::'.

There are 5 other sub namespaces 'core', 'scene', 'video',

'io' and 'gui'. Unlike in the HelloWorld example,

we do not a 'using namespace' for these 5 other namespaces

because in this way you will see what can be found in which

namespace. But if you like, you can also include the namespaces

like in the previous example. Code just like you want to.

*/

using namespace irr;



/*

Again, to be able to use the Irrlicht.DLL file, we need to link with the

Irrlicht.lib. We could set this option in the project settings, but

to make it easy, we use a pragma comment lib:

*/

#pragma comment(lib, "Irrlicht.lib")



//Init the "RenderEffect" variable

int RenderEffect = 0;



class MyEventReceiver : public IEventReceiver

{

public:



   virtual bool OnEvent(const SEvent &event)

   {

      //Test if keyboard event and what key are pressed

      if (event.EventType == EET_KEY_INPUT_EVENT && event.KeyInput.PressedDown == false)

      {

         switch(event.KeyInput.Key)

         {

            case KEY_KEY_N: RenderEffect=0;

               return true;

            case KEY_KEY_G: RenderEffect=1;

               return true;

            case KEY_KEY_D: RenderEffect=2;

               return true;

            case KEY_KEY_V: RenderEffect=3;

               return true;

         }

      }

      return false;

   }

};





/*

Ok, lets start. Again, we use the main() method as start, not the

WinMain(), because its shorter to write.

*/

int main()

{

   // ask user for driver

   video::E_DRIVER_TYPE driverType;



   printf("Please select the driver you want for this example:\n"\

      " (a) Direct3D 9.0c\n (b) OpenGL 1.5\n");



   char i;

   std::cin >> i;



   //select the driver type with "a" or "b"

   switch(i)

   {

      case 'a': driverType = video::EDT_DIRECT3D9;break;

      case 'b': driverType = video::EDT_OPENGL;   break;

      default: return 1;

   }



   // create device and exit if creation failed

   IrrlichtDevice* device = createDevice(driverType, core::dimension2d<u32>(800, 600));



   //Class MyEventReceiver for the keyboard

   MyEventReceiver receiver;

   device->setEventReceiver(&receiver);



   if (device == 0)

      return 1; // could not create selected driver.



   /*

   Get a pointer to the video driver and the SceneManager so that

   we do not always have to write device->getVideoDriver() and

   device->getSceneManager().

   */

   video::IVideoDriver* driver = device->getVideoDriver();

   scene::ISceneManager* smgr = device->getSceneManager();



   /*

   To display the Quake 3 map, we first need to load it. Quake 3 maps

   are packed into .pk3 files wich are nothing other than .zip files.

   So we add the .pk3 file to our FileSystem. After it was added,

   we are able to read from the files in that archive as they would

   directly be stored on disk.

   */

   device->getFileSystem()->addZipFileArchive("media/map-20kdm2.pk3");



   /*

   Now we can load the mesh by calling getMesh(). We get a pointer returned

   to a IAnimatedMesh. As you know, Quake 3 maps are not really animated,

   they are only a huge chunk of static geometry with some materials

   attached. Hence the IAnimated mesh consists of only one frame,

   so we get the "first frame" of the "animation", which is our quake level

   and create an OctTree scene node with it, using addOctTreeSceneNode().

   The OctTree optimizes the scene a little bit, trying to draw only geometry

   which is currently visible. An alternative to the OctTree would be a

   AnimatedMeshSceneNode, which would draw always the complete geometry of

   the mesh, without optimization. Try it out: Write addAnimatedMeshSceneNode

   instead of addOctTreeSceneNode and compare the primitives drawed by the

   video driver. (There is a getPrimitiveCountDrawed() method in the

   IVideoDriver class). Note that this optimization with the Octree is only

   useful when drawing huge meshes consiting of lots of geometry.

   */

   scene::IAnimatedMesh* mesh = smgr->getMesh("20kdm2.bsp");

   scene::ISceneNode* node = 0;



   if (mesh)

      node = smgr->addOctTreeSceneNode(mesh->getMesh(0));



   /*

   Because the level was modelled not around the origin (0,0,0), we translate

   the whole level a little bit.

   */

   if (node)

      node->setPosition(core::vector3df(-1300,-144,-1249));



   //Add a camera type SceneNodeFPS to the scene

   scene::ICameraSceneNode* camera = smgr->addCameraSceneNodeFPS();



   //Hide the mouse cursor

   device->getCursorControl()->setVisible(false);




   //Here you create the effect you want to use
   CEffects effect;

   effect.CreateGlow(camera,smgr,driver,128);
   effect.CreateDream(camera,smgr,driver,128);
   effect.CreateVision(camera,smgr,driver,"media/noise.jpg",128);



   //initialise the lastFPS variable

   int lastFPS = -1;



   //Main Loop

   while(device->run())

   if (device->isWindowActive())

   {

      driver->beginScene(true, true, video::SColor(10,10,10,10));

        effect.RenderNothing();


        //Get the "RenderEffect" variable to draw the desired effect
      switch(RenderEffect)
      {
         //Here you Render the effect
         case 1:effect.RenderGlow(smgr,driver,10,10,10,10);break;
         case 2:effect.RenderDream(smgr,driver,10,10,10,10);break;
         case 3:effect.RenderVision(smgr,driver,10,10,10,10);
      }


      //Draw all

      smgr->drawAll();


      driver->endScene();



      int fps = driver->getFPS();



      //Draw the FPS in the window caption

      if (lastFPS != fps)

      {

         core::stringw str = L"Irrlicht Engine - Switch keys N.G.D.V [";

         str += driver->getName();

         str += "] FPS:";

         str += fps;



         device->setWindowCaption(str.c_str());

         lastFPS = fps;

      }

   }



   //In the end, delete the Irrlicht device.

   device->drop();



   return 0;

}
