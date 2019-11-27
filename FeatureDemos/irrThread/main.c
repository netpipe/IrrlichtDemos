
#include <pthread.h>
#include <windows.h>
#include <stdio.h>
#include <irrlicht.h>

#pragma comment(lib,"pthreadVC2.lib")
#pragma comment(lib,"irrlicht.lib")

using namespace irr;
using namespace scene;
using namespace core;
using namespace video;

struct tStruct
{
   IrrlichtDevice* dev;
   ISceneNode* node;
   IAnimatedMesh* mesh;
}

void* doOtherStuff(void * stuff)
{
   tStruct mytStruct = *((tStruct*)stuff);

   mytStruct.dev->getSceneManager()->createOctTreeTriangleSelector(mytStruct.mesh->getMesh(0),mytStruct.node);

   pthread_exit(NULL);
   return (void*)0;
}

int main()
{
   IrrlichtDevice* device = createDevice(video::EDT_OPENGL,core::dimension2d<s32>(640,480));

   device->getSceneManager()->addCameraSceneNode();
   device->getFileSystem()->addZipFileArchive("../../media/map-20kdm2.pk3");
   
   scene::IAnimatedMesh* q3levelmesh = smgr->getMesh("20kdm2.bsp");
   scene::ISceneNode* q3node = 0;
   
   if (q3levelmesh)
      q3node = smgr->addOctTreeSceneNode(q3levelmesh->getMesh(0));

   pthread_t* myThread2 = new pthread_t();
   
   tStruct mytStruct;
   mytStruct.dev = device;
   mytStruct.mesh = q3levelmesh;
   mytStruct.node = q3node;

   pthread_create(myThread2,NULL,doOtherStuff,(void*)&mytStruct);

   ISceneNode* cuby = device->getSceneManager()->addCubeSceneNode(10,0,-1,vector3df(0,0,30));
   cuby->setMaterialFlag(EMF_LIGHTING,false);
   ISceneNodeAnimator* anim = device->getSceneManager()->createRotationAnimator(vector3df(0.5f,0.5f,0.5f));

   cuby->addAnimator(anim);
   
   // Show something cool while it's loading.
   while(device->run())
   {
      device->getVideoDriver()->beginScene(true,true,video::SColor(255,255,0,0));
      device->getSceneManager()->drawAll();
      device->getVideoDriver()->endScene();
      
      Sleep(10);
   }

   device->drop();

   pthread_exit(NULL);
}
