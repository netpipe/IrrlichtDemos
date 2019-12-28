#include <irrlicht.h>
using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

#pragma comment(lib, "Irrlicht.lib")

#include "MetaballSceneNode.h"

IrrlichtDevice *device = 0;

int main() {
   IrrlichtDevice *device = createDevice(EDT_OPENGL, dimension2du(720, 480));
   if (device == 0)
      return 1;

   IVideoDriver *driver = device->getVideoDriver();
   ISceneManager *smgr = device->getSceneManager();

   ISceneNodeFactory *metaFactory = new MetaballSceneNodeFactory(smgr);
   smgr->registerSceneNodeFactory(metaFactory);

   ICameraSceneNode *cam = smgr->addCameraSceneNodeFPS();
   cam->setPosition(vector3df(0, 0, -400));

   MetaballSceneNode *meta = (MetaballSceneNode *)smgr->addSceneNode("MetaballSceneNode", 0);
   meta->setSampleWidth(.125);
   meta->setBound(2.25);
   meta->setThreshold(1);

   meta->addMetaball(vector3df(1, -1, 1), SColor(255, 0, 0, 255));
   meta->addMetaball(vector3df(-1, -1, -1), SColor(255, 0, 255, 0));
   meta->addMetaball(vector3df(0, 1, 0), SColor(255, 255, 0, 0));
   meta->triangulateVolume();

   meta->setScale(vector3df(100, 100, 100));
   meta->setRotation(vector3df(0, 45, 0));

   device->getCursorControl()->setVisible(false);
   int lastFPS = -1;
   while(device->run()) {
      driver->beginScene(true, true, SColor(0,200,200,200));
      smgr->drawAll();
      driver->endScene();
   }

   device->drop();
   return 0;
}
