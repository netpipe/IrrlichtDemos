#include "CTheoraPlayer.h"
#include <irrlicht.h>

using namespace irr;
using namespace scene;
using namespace core;
using namespace video;

int main()
{
   irr::IrrlichtDevice* device = irr::createDevice(irr::video::EDT_OPENGL, irr::core::dimension2du(800, 600));
   irr::video::IVideoDriver* driver = device->getVideoDriver();

   CTheoraPlayer iTheoraPlayer(device);

   iTheoraPlayer.load("truk_kartochniy.ogg", true);

   irr::gui::IGUIEnvironment* guienv = device->getGUIEnvironment();
   guienv->addImage(iTheoraPlayer.getTexture(), irr::core::position2di(0,0));

scene::ISceneManager* smgr = device->getSceneManager();

  // add camera
    irr::scene::ICameraSceneNode* iCameraSceneNode;
  iCameraSceneNode =
    smgr->addCameraSceneNodeFPS();

  iCameraSceneNode->setRotation(core::vector3df(20,90,0));
  iCameraSceneNode->setPosition(core::vector3df(0,0,-200));

  scene::ISceneNode* iCubeSceneNode;
  iCubeSceneNode = smgr->addCubeSceneNode(50);
  iCubeSceneNode->setMaterialTexture(0, iTheoraPlayer.getTexture()); // set material of cube to render target

   // video::ITexture* texture = driver->getTexture("h.jpg");
  //iCubeSceneNode->setMaterialTexture(0, texture);

  irr::scene::ISceneNodeAnimator* anim = smgr->createRotationAnimator(
    core::vector3df(0.3f, 0.3f,0));

  iCubeSceneNode->setPosition(core::vector3df(0,0,0));
  iCubeSceneNode->setMaterialFlag(video::EMF_LIGHTING, false); // disable dynamic lighting
  iCubeSceneNode->addAnimator(anim);
  anim->drop();

   irr::ITimer* timer = device->getTimer();
   irr::u32 then = timer->getTime();

   while (device->run())
   {
      const irr::u32 now = timer->getTime();
      const irr::u32 frameDeltaTime = now - then;

      then = now;

      iTheoraPlayer.update(frameDeltaTime);

      driver->beginScene();

        smgr->drawAll();
      guienv->drawAll();
      driver->endScene();
   }

   device->drop();

   return 0;
}
