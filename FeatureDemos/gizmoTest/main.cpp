#include <irrlicht.h>
#include "MyEventReceiver.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

#define selectedNode 4

int main()
{
   IrrlichtDevice *device = createDevice(video::EDT_OPENGL,core::dimension2d<u32>(1024, 768), 32, false, false, false, 0);
    if (!device)
                return 1;

   MyEventReceiver receiver(device);
   device->setEventReceiver(&receiver);


   IVideoDriver *driver      = device->getVideoDriver();
   ISceneManager *smgr      = device->getSceneManager();
   IGUIEnvironment *guienv      = device->getGUIEnvironment();

   device->setWindowCaption(L"Gizmo Test");

   ICameraSceneNode *camera = smgr->addCameraSceneNode(0, vector3df(50,30,-40), vector3df(0,5,0));
   receiver.getGizmoSceneManager()->addCameraSceneNode(0, camera->getPosition(), camera->getTarget());

   ISceneNode* node = smgr->addCubeSceneNode(20, 0, selectedNode);
   if (node)
   {
      node->setMaterialFlag(EMF_LIGHTING, false);
      node->setPosition(vector3df(0,0,-40));
      node->setMaterialTexture( 0, driver->getTexture("wall.jpg") );
      node->setMaterialFlag(EMF_WIREFRAME, true);
   }


   ISceneNode* node3 = smgr->addCubeSceneNode(20, 0, selectedNode);
   if (node)
   {
      node3->setMaterialFlag(EMF_LIGHTING, false);
      node3->setPosition(vector3df(0,20,-40));
      node3->setMaterialTexture( 0, driver->getTexture("wall.jpg") );
      node3->setMaterialFlag(EMF_WIREFRAME, true);
   }

   ISceneNode* node_2 = smgr->addCubeSceneNode(20, 0, selectedNode);
   if (node_2)
   {
      node_2->setMaterialFlag(EMF_LIGHTING, false);
      node_2->setPosition(vector3df(0,0,0));
      node_2->setMaterialTexture( 0, driver->getTexture("wall.jpg") );
      //node_2->setMaterialFlag(EMF_WIREFRAME, true);
   }

   while(device->run())
   {

      driver->beginScene(true, true, SColor(255,100,101,140));
        smgr->drawAll();
        driver->clearZBuffer();
        receiver.getGizmoSceneManager()->drawAll();
        guienv->drawAll();
        driver->endScene();
        device->sleep(10,0);
    }

   device->drop();

   return 0;
}
