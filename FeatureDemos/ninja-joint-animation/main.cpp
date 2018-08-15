#include <irrlicht.h>

using namespace irr;

using namespace core;
using namespace scene;
using namespace video;

#ifdef _IRR_WINDOWS_
#pragma comment(lib, "Irrlicht.lib")
#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#endif



int main()
{

   IrrlichtDevice *device =
      createDevice( video::EDT_OPENGL, dimension2d<u32>(800, 600));


   IVideoDriver* driver = device->getVideoDriver();
   ISceneManager* smgr = device->getSceneManager();

        smgr->addCameraSceneNode(0, vector3df(0,10,-10), vector3df(0,5,0));

        IAnimatedMesh* mesh = smgr->getMesh("ninja.b3d");
   IAnimatedMeshSceneNode* node = smgr->addAnimatedMeshSceneNode(mesh);
   node->setMaterialFlag(EMF_LIGHTING, false);
   node->setMaterialTexture( 0, driver->getTexture("nskinrd.jpg") );
        node->setRotation(vector3df(0,180,0)); // let ninja be in front to us

        node->setJointMode(EJUOR_CONTROL); // tell irrlicht that you want to control joint positions

        IBoneSceneNode* bone = node->getJointNode("Joint10"); // create IBoneSceneNode and select the desired bone

   while(device->run()) {
            bone->setRotation(bone->getRotation() + vector3df(1,0,0)); // rotate bone

            driver->beginScene();
            smgr->drawAll();
            driver->endScene();
   }


   device->drop();

   return 0;
}
