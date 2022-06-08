
#include <irrlicht/irrlicht.h>
#include <cv.h>
#include <highgui.h>

#include "CvIrrCamTexture.h"

using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

IrrlichtDevice *device;
IVideoDriver* driver;
ISceneManager* smgr;

int main()
{
   device = createDevice( video::EDT_OPENGL, dimension2du(640, 480), 16, false, false, false, 0);
   driver = device->getVideoDriver();
   smgr = device->getSceneManager();

   device->setWindowCaption(L"Hello World! - Irrlicht Engine Demo");

   //something for 3d reference
   IAnimatedMesh* mesh = smgr->getMesh("media/sydney.md2");
   IAnimatedMeshSceneNode* node = smgr->addAnimatedMeshSceneNode(mesh);
   if (node)
   {
      node->setMaterialFlag(EMF_LIGHTING, false);
      node->setMD2Animation ( scene::EMAT_STAND );
      node->setMaterialTexture( 0, driver->getTexture("media/sydney.bmp") );
      node->setPosition(vector3df(10,10,-10));
   }

   //and some more
   device->getFileSystem()->addZipFileArchive("media/map-20kdm2.pk3");
   IAnimatedMesh* mesh2 = smgr->getMesh("20kdm2.bsp");
   ISceneNode* node2 = 0;
   if (mesh2)
      node2 = smgr->addOctTreeSceneNode(mesh2->getMesh(0), 0, -1, 128);
   if (node2)
      node2->setPosition(vector3df(-1300,-144,-1249));

   //our billboard
   IBillboardSceneNode* our_bill_node = smgr->addBillboardSceneNode(NULL, dimension2d<f32>(25.0f, 25.0f), vector3df(0,0,0));
   our_bill_node->setMaterialFlag(video::EMF_LIGHTING, false);

   //camera stuff
   ICameraSceneNode* camera = smgr->addCameraSceneNodeFPS();
   camera->setPosition(vector3df(60,10,0));
   camera->setTarget(vector3df(0,0,0));

   //start the webcam
   CvIrrCamTexture* cv_text_mgr = new CvIrrCamTexture(driver,0,0);
   //set the texture
   our_bill_node->setMaterialTexture( 0, cv_text_mgr->getTexture() );

   while(device->run())
   {
      //update webcam texture
      cv_text_mgr->UpdateTexture();

      driver->beginScene(true, true, SColor(255,100,101,140));

      smgr->drawAll();

      driver->endScene();
   }

   device->drop();

   return 0;
}

