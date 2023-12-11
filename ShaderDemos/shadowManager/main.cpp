  #include <irrlicht.h>

  #include <CManagedShadow.h>
  #include <CShadowManager.h>
  #include <CManagedShadowFactory.h>

using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

IGUIStaticText *g_pTxt;

class eventReceiver : public IEventReceiver {
  private:
    u32 m_iPrio;

  public:
    eventReceiver() {
      m_iPrio=3;
    }

    virtual ~eventReceiver() { }
    virtual bool OnEvent(const SEvent &event) {
      bool bRet=false;
      if (event.EventType==EET_KEY_INPUT_EVENT && !event.KeyInput.PressedDown) {
        if (event.KeyInput.Key==KEY_PLUS || event.KeyInput.Key==KEY_ADD) {
          if (m_iPrio<6) {
            m_iPrio++;
            CShadowManager::getSharedInstance()->setPriority(m_iPrio);
            wchar_t s[0xFF];
            swprintf(s,0xFF,L"Shadow Prio: %i",m_iPrio);
            g_pTxt->setText(s);
          }
          bRet=true;
        }

        if (event.KeyInput.Key==KEY_MINUS || event.KeyInput.Key==KEY_SUBTRACT) {
          if (m_iPrio>0) {
            m_iPrio--;
            CShadowManager::getSharedInstance()->setPriority(m_iPrio);
            wchar_t s[0xFF];
            swprintf(s,0xFF,L"Shadow Prio: %i",m_iPrio);
            g_pTxt->setText(s);
          }
          bRet=true;
        }
      }
      return bRet;
    }
};

int main(int argc, char** argv) {
  IrrlichtDevice *device=createDevice(EDT_OPENGL,dimension2du(640,480),16,false,true,false,0);

  device->setWindowCaption(L"Irrlicht Engine - Managed Shadows Demo");

  IVideoDriver* driver = device->getVideoDriver();
  ISceneManager* smgr = device->getSceneManager();
  IGUIEnvironment* guienv = device->getGUIEnvironment();

  ICameraSceneNode *pCam=smgr->addCameraSceneNodeFPS(0,50.0f,0.05f);
  pCam->setPosition(vector3df(-12.5f,36.25f,-48.25f));
  pCam->setTarget(vector3df(15.15f,-12.3f,-22.3f));

  eventReceiver myReceiver;
  device->setEventReceiver(&myReceiver);

  g_pTxt=guienv->addStaticText(L"Shadow Prio: 3",rect<s32>(5,5,155,15),true,false,0,-1,true);

  CManagedShadowFactory factory(smgr);
  smgr->registerSceneNodeFactory(&factory);

  smgr->loadScene("data/irrOdeMarble_level_.xml");

  CShadowManager::getSharedInstance()->setPriority(3);

  s32 lastFPS=-1;
  while(device->run()) {
    vector3df pos=pCam->getTarget();
    driver->beginScene(true, true, SColor(0,200,200,200));

    smgr->drawAll();
    guienv->drawAll();

    driver->endScene();
    s32 fps = driver->getFPS();

    if (lastFPS != fps) {
      core::stringw str = L"Irrlicht Engine - Managed Shadows Demo [";
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

