#include <btBulletDynamicsCommon.h>
#include <irrlicht.h>
#include <iostream>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

int main()
{
   IrrlichtDevice *device = createDevice(video::EDT_OPENGL, core::dimension2d<s32>(640, 480));

   if (device == 0)
      return 1; // could not create selected driver.

   video::IVideoDriver* driver = device->getVideoDriver();
   scene::ISceneManager* smgr = device->getSceneManager();

   device->getFileSystem()->addZipFileArchive("../../media/map-20kdm2.pk3");

   scene::IAnimatedMesh* mesh = smgr->getMesh("20kdm2.bsp");
   scene::ISceneNode* node = 0;

   if (mesh)
      node = smgr->addOctTreeSceneNode(mesh->getMesh(0), 0, -1, 128);

   if (node)
      node->setPosition(core::vector3df(-1350,-130,-1400));


   // add pins

   scene::IAnimatedMesh* aPinMesh = smgr->getMesh("./pin.x");
   scene::IMeshManipulator *man = smgr->getMeshManipulator();
   scene::IMesh* pm = aPinMesh->getMesh(0);
   man->scaleMesh(pm, core::vector3df(15, 15, 15));
   //((scene::SAnimatedMesh*)pm)->recalculateBoundingBox();
   //man->recalculateNormals(pm);


   core::vector3df aGroupPos(0,-53,0);
   for (int i = 0; i < 5; i++)
   {
      for (int j = 0; j < i; j++)
      {
         scene::ISceneNode* aPinSceneNode = smgr->addMeshSceneNode(pm);
         aPinSceneNode->setRotation(core::vector3df(0, 0, 0));
         aPinSceneNode->setPosition(aGroupPos + core::vector3df(-j*12.0f, -0, 0.0f + i*12.0f));
         aPinSceneNode->setMaterialType(video::EMT_SOLID);
         aPinSceneNode->setMaterialFlag(video::EMF_LIGHTING, true);
         //aPinSceneNode->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
         //aPinSceneNode->setScale(core::vector3df(15, 15, 15));
      }
   }

/*
   video::SMaterial material;
   material.setTexture(0, driver->getTexture("../../media/faerie2.bmp"));
   material.Lighting = true;

   scene::IAnimatedMeshSceneNode* fnode = 0;
   scene::IAnimatedMesh* faerie = smgr->getMesh("../../media/faerie.md2");

   if (faerie)
   {
      fnode = smgr->addAnimatedMeshSceneNode(faerie);
      fnode->setPosition(core::vector3df(-20,-30,12));
      fnode->setMD2Animation(scene::EMAT_RUN);
      fnode->getMaterial(0) = material;
   }
*/
   scene::ICameraSceneNode* iCameraSceneNode = smgr->addCameraSceneNodeFPS();
   iCameraSceneNode->setRotation(core::vector3df(20.833364f, -14.875068f, 0.000000f));
   iCameraSceneNode->setPosition(core::vector3df(53.800972f, 23.301916f, -174.089157f));

   // add light
   scene::ILightSceneNode* light = smgr->addLightSceneNode(0,
                                             core::vector3df(0, 30, 0),
                                             video::SColorf(1.0f, 1.0f, 1.0f),
                                             600);

   scene::IBillboardSceneNode * bill = smgr->addBillboardSceneNode(light);
   bill->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR );
   bill->setMaterialTexture(0, driver->getTexture("../../media/particle.bmp"));
   bill->setMaterialFlag(video::EMF_LIGHTING, false);
   bill->setMaterialFlag(video::EMF_ZBUFFER, false);
   bill->setSize(core::dimension2d<f32>(20.0f, 20.0f));
   device->getCursorControl()->setVisible(false);

   int lastFPS = -1;

   while(device->run())
   if (device->isWindowActive())
   {
      driver->beginScene(true, true, video::SColor(0,200,200,200));
      smgr->drawAll();
      driver->endScene();

      int fps = driver->getFPS();

      if (lastFPS != fps)
      {
         core::stringw str = L"Irrlicht Engine - Quake 3 Map example [";
         str += driver->getName();
         str += "] FPS:";
         str += fps;

         device->setWindowCaption(str.c_str());
         lastFPS = fps;
      }
   }

   device->drop();
   return 0;
}
