#include <irrlicht.h>
#include "CWorldSceneNode.h"

using namespace irr;

#ifdef _MSC_VER
#pragma comment(lib, "Irrlicht.lib")
#endif


int main() {
   video::E_DRIVER_TYPE driverType = video::EDT_OPENGL;

   if (driverType==video::EDT_COUNT)
      return 1;

   IrrlichtDevice *device = createDevice(driverType,core::dimension2d<u32>(800, 600), 16, false, false, false);

   // could not create selected driver.
   if (device == 0)
      return 1;

   // create engine and camera
   device->setWindowCaption(L"Custom Scene Node - Irrlicht Engine Demo");

   video::IVideoDriver* driver = device->getVideoDriver();
   scene::ISceneManager* smgr = device->getSceneManager();
   scene::ILightSceneNode* light = smgr->addLightSceneNode(0,core::vector3df(0,0,-2800));

   light->setRadius(4000);

   video::ITexture *texture =  driver->getTexture("wall.bmp");
   texture->grab();
   video::IImage * myimage = driver->createImage(texture,core::vector2d<s32>(0,0),core::vector2d<u32>(1024,512));

   // Create new sphere
   CWorldSceneNode *world = new CWorldSceneNode(smgr->getRootSceneNode(),
                                     smgr,
                                        100,               // ID
                                     myimage);             // bump map

   //world->setRadius(20);

   video::ITexture *color =  driver->getTexture("wall.bmp");
   world->getMaterial(0).setTexture (0, color);

   world->setMaterialFlag(video::EMF_WIREFRAME, true);

   scene::ICameraSceneNode* cam = smgr->addCameraSceneNodeFPS(0,50,0.001);
   cam->setNearValue(0.001);
   smgr->setActiveCamera (cam);

   /*
    Now draw everything and finish.
    */

   u32 frames=0;

   while(device->run())
   {

      driver->beginScene(true, true, video::SColor(0,100,100,100));

      smgr->drawAll();

      driver->endScene();

      device->sleep(5);


   }


   device->drop();

   return 0;
}
