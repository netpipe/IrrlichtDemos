#include "CTheoraPlayer.h"
#include <irrlicht.h>

using namespace irr;
using namespace scene;
using namespace core;
using namespace video;

IrrlichtDevice *device;
video::IVideoDriver* driver;
scene::ISceneManager* smgr;
gui::IGUIEnvironment* guienv;
CTheoraPlayer * test;

   irr::ITimer* timer;
   irr::u32 then;

//SIrrlichtCreationParameters	params;
//params = SIrrlichtCreationParameters();
//	params.DriverType = video::EDT_OGLES2;
//	params.WindowSize = core::dimension2d<u32>(640, 480);
//	params.Bits = 16;
//	params.Fullscreen = false;
//	params.Stencilbuffer = false;
//	params.Vsync = false;
//	params.EventReceiver = 0;
//	params.OGLES2ShaderPath = std::string("media/Shaders/").c_str();
//	device = createDeviceEx(params);

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif
int init;
//   CTheoraPlayer iTheoraPlayer(device);

void main_loop()
{


      const irr::u32 now = timer->getTime();
      const irr::u32 frameDeltaTime = now - then;

      then = now;
	device->run();

    test->update(frameDeltaTime);
    //render();

	driver->beginScene(true, true, video::SColor(255,200,200,200));
	guienv->drawAll();
	smgr->drawAll();
	driver->endScene();
}


int main()
{
  #ifdef __EMSCRIPTEN__
device = irr::createDevice(irr::video::EDT_OGLES2, irr::core::dimension2du(800, 600));
#else
device = irr::createDevice(irr::video::EDT_OPENGL, irr::core::dimension2du(800, 600));
#endif
//    device = irr::createDevice(irr::video::EDT_OGLES2, irr::core::dimension2du(800, 600));
    driver = device->getVideoDriver();





 smgr = device->getSceneManager();

  // add camera
    irr::scene::ICameraSceneNode* iCameraSceneNode;
  iCameraSceneNode =
    smgr->addCameraSceneNodeFPS();

  iCameraSceneNode->setRotation(core::vector3df(20,90,0));
  iCameraSceneNode->setPosition(core::vector3df(0,0,-200));


   // video::ITexture* texture = driver->getTexture("h.jpg");
  //iCubeSceneNode->setMaterialTexture(0, texture);

  irr::scene::ISceneNodeAnimator* anim = smgr->createRotationAnimator(
    core::vector3df(0.3f, 0.3f,0));



    timer = device->getTimer();
   then = timer->getTime();

  // iTheoraPlayer.setDevice(device);
//  guienv = device->getGUIEnvironment();
//   guienv->addImage(iTheoraPlayer.getTexture(), irr::core::position2di(0,0));



   #ifdef __EMSCRIPTEN__
      CTheoraPlayer iTheoraPlayer(device);
        iTheoraPlayer.load("./media/truk_kartochniy.ogg", true);
      test=&iTheoraPlayer;
        guienv = device->getGUIEnvironment();
   guienv->addImage(iTheoraPlayer.getTexture(), irr::core::position2di(0,0));
  scene::ISceneNode* iCubeSceneNode;
  iCubeSceneNode = smgr->addCubeSceneNode(50);
  iCubeSceneNode->setMaterialTexture(0, iTheoraPlayer.getTexture()); // set material of cube to render target
  iCubeSceneNode->setPosition(core::vector3df(0,0,0));
  iCubeSceneNode->setMaterialFlag(video::EMF_LIGHTING, false); // disable dynamic lighting
  iCubeSceneNode->addAnimator(anim);
  anim->drop();

	emscripten_set_main_loop(main_loop,0,1);
#else

   CTheoraPlayer iTheoraPlayer(device);
     iTheoraPlayer.load("./media/truk_kartochniy.ogg", true);
test=&iTheoraPlayer;
  guienv = device->getGUIEnvironment();
   guienv->addImage(iTheoraPlayer.getTexture(), irr::core::position2di(0,0));
  scene::ISceneNode* iCubeSceneNode;
  iCubeSceneNode = smgr->addCubeSceneNode(50);
  iCubeSceneNode->setMaterialTexture(0, iTheoraPlayer.getTexture()); // set material of cube to render target

  iCubeSceneNode->setPosition(core::vector3df(0,0,0));
  iCubeSceneNode->setMaterialFlag(video::EMF_LIGHTING, false); // disable dynamic lighting
  iCubeSceneNode->addAnimator(anim);
  anim->drop();

   while (device->run())
   {
      const irr::u32 now = timer->getTime();
      const irr::u32 frameDeltaTime = now - then;

      then = now;

      test->update(frameDeltaTime);
     // iTheoraPlayer.update(frameDeltaTime);

      driver->beginScene();

        smgr->drawAll();
      guienv->drawAll();
      driver->endScene();
   }
#endif
   device->drop();

   return 0;
}
