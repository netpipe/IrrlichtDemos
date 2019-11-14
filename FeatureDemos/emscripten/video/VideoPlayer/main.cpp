#include <irrlicht.h>
#include "CVideoMaster.h"


#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

using namespace irr;
using namespace video;
using namespace core;
using namespace scene;
using namespace io;
using namespace gui;

IrrlichtDevice  *device;
IVideoDriver    *driver;
ISceneManager   *smgr;
IGUIEnvironment *guienv;
CVideoMaster    *vidmaster;

class CEvents : public irr::IEventReceiver
{
  bool OnEvent (const irr::SEvent &e)
  {
    switch (e.EventType)
    {
      case EET_KEY_INPUT_EVENT :
      {
        switch (e.KeyInput.Key)
        {
          case KEY_BACK :
          {
            // Be careful with the parameters,
            // removing a texture currently in use will cause a crash
            vidmaster->removeVideoClipAll();
          } break;

          case KEY_ESCAPE :
          {
            device->closeDevice();
          } break;
        }
      } break;

      case EET_MOUSE_INPUT_EVENT :
      {
        switch (e.MouseInput.Event)
        {
          case EMIE_LMOUSE_PRESSED_DOWN :
          {
            // Add a new video clip to be used as a 2DImage
            vidmaster->addVideoClip("titan.ogg", "Titan", ESM_NONE, true);
            vidmaster->addVideoClip("YUVTest.ogg", "YUV", dimension2du(128,128), true);
          } break;

          case EMIE_RMOUSE_PRESSED_DOWN :
          {
            // Pause/Resume playback of Titan Video
            // Lots of different features can be accessed via the TheoClip pointer
            TheoraVideoClip* temp = vidmaster->getTheoClipByName("Titan");

            if (temp)
            {
              if (temp->isPaused()) temp->play();
              else                  temp->pause();
            }
          } break;
        }
      } break;
    }

    return false;
  }
};


void main_loop()
{
	device->run();
 // make sure to update each frame
    vidmaster->update();

    driver->beginScene(true, true, SColor(0, 0, 0, 0));
      smgr->drawAll();

      // get a texture by name and draw it
      ITexture* temp = vidmaster->getTextureByName("Titan");
      if (temp)
      {
        driver->draw2DImage(temp, vector2di(0, 0));
      }

      temp = vidmaster->getTextureByName("YUV");
      if (temp)
      {
        driver->draw2DImage(temp, vector2di(driver->getScreenSize().Width - 128, driver->getScreenSize().Height - 128));
      }

	guienv->drawAll();
	//manager->drawAll();
	driver->endScene();
}


int main()
{
  SIrrlichtCreationParameters deviceParams;

  deviceParams.DriverType = EDT_OGLES2;
 // deviceParams.DriverType = EDT_DIRECT3D9;
  deviceParams.WindowSize = dimension2du(800, 600);
  deviceParams.Bits       = 32;
  deviceParams.Fullscreen = false;
  deviceParams.Vsync      = false;
  deviceParams.AntiAlias  = 16;

  device = createDeviceEx(deviceParams);
  driver = device->getVideoDriver();
  smgr   = device->getSceneManager();
  guienv = device->getGUIEnvironment();

  CEvents eve;
  device->setEventReceiver(&eve);

  ICameraSceneNode* cam = smgr->addCameraSceneNodeFPS();
  cam->setPosition(vector3df(0, 0, 200.0f));

  // Create the video master with audio playback enabled using 1 decoder thread
  vidmaster = new CVideoMaster(device, true, 1);

  // Add a new video clip,
  // the returned texture can be used for pretty much everything
  ITexture* cubeDiffuse = vidmaster->addVideoClip("./media/YUVTest.ogg", "TV", ESM_NONE, true);

  IMeshSceneNode *TV = smgr->addCubeSceneNode(50);
  TV->setScale(vector3df(1.0f, 1.0f, 0.001f));
  // Set video texture as diffuse
  TV->setMaterialTexture(0, cubeDiffuse);
  TV->setMaterialFlag(EMF_LIGHTING, false);

  s32 lastFPS = -1;

  #ifdef __EMSCRIPTEN__
	emscripten_set_main_loop(main_loop,0,1);
  #else

//
//  while(device->run())
//  {
//    // make sure to update each frame
//    vidmaster->update();
//
//    driver->beginScene(true, true, SColor(0, 0, 0, 0));
//      smgr->drawAll();
//
//      // get a texture by name and draw it
//      ITexture* temp = vidmaster->getTextureByName("Titan");
//      if (temp)
//      {
//        driver->draw2DImage(temp, vector2di(0, 0));
//      }
//
//      temp = vidmaster->getTextureByName("YUV");
//      if (temp)
//      {
//        driver->draw2DImage(temp, vector2di(driver->getScreenSize().Width - 128, driver->getScreenSize().Height - 128));
//      }
//
//    driver->endScene();
//
//// --------------------------------------------------
//
//    s32 fps = driver->getFPS();
//    if (lastFPS != fps)
//    {
//      stringw str  = L"VideoPlayer | ";
//              str += driver->getName();
//              str += L" | ";
//              str += fps;
//              str += L" FPS";
//      device->setWindowCaption(str.c_str());
//      lastFPS = fps;
//    }
//  }
#endif

  delete vidmaster;

  device->drop();

  return 0;
}
