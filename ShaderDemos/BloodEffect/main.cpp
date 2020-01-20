#include <Irrlicht.h>
#include "CBloodEffect.h"
#include "CBloodShader.h"

using namespace irr;
using namespace video;
using namespace core;
using namespace scene;
using namespace io;
using namespace gui;

IrrlichtDevice *device;
IVideoDriver   *driver;
ISceneManager  *smgr;

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
          case KEY_KEY_1 :
          {
            // Create a blood effect like this
            // It destroys itself when it's done, no need to store a pointer
            new CBloodEffect(smgr, EGL_MILD, vector3df(0, 50.0f, 0), vector3df(0, 0.0f, 1.0f));
          } break;

          case KEY_KEY_2 :
          {
            new CBloodEffect(smgr, EGL_MEDIUM, vector3df(0, 50.0f, 0), vector3df(0, 0.0f, 1.0f));
          } break;

          case KEY_KEY_3 :
          {
            new CBloodEffect(smgr, EGL_BRUTAL, vector3df(0, 50.0f, 0), vector3df(0, 0.0f, 1.0f));
          } break;

          case KEY_KEY_4 :
          {
            new CBloodEffect(smgr, EGL_INSANE, vector3df(0, 50.0f, 0), vector3df(0, 0.0f, 1.0f));
          } break;

          case KEY_KEY_5 :
          {
            new CBloodEffect(smgr, EGL_MEDIUM, vector3df(0, 50.0f, 0), vector3df(0, 0.7, 1.0f), 250, 0.075f, 0.3f);
            new CBloodEffect(smgr, EGL_MEDIUM, vector3df(0, 50.0f, 0), vector3df(0, 0.7, -1.0f), 250, 0.055f, 0.3f);
            new CBloodEffect(smgr, EGL_BRUTAL, vector3df(0, 50.0f, 0), vector3df(-1.0f, 0.7, 0), 250, 0.035f, 0.3f);
            new CBloodEffect(smgr, EGL_MEDIUM, vector3df(0, 50.0f, 0), vector3df(1.0f, 0.7, 0), 250, 0.025f, 0.3f);
            new CBloodEffect(smgr, EGL_MILD, vector3df(0, 50.0f, 0), vector3df(1, 0.9, 1.0f), 250);
            new CBloodEffect(smgr, EGL_MILD, vector3df(0, 50.0f, 0), vector3df(-1, 0.9, -1.0f), 250);
            new CBloodEffect(smgr, EGL_MILD, vector3df(0, 50.0f, 0), vector3df(-1, 0.9, 1.0f), 250);
            new CBloodEffect(smgr, EGL_MILD, vector3df(0, 50.0f, 0), vector3df(1, 0.9, -1.0f), 250);
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
            // collision check
            line3df     collRay = smgr->getSceneCollisionManager()->getRayFromScreenCoordinates(device->getCursorControl()->getPosition(), smgr->getActiveCamera());
            vector3df   collPoint;
            triangle3df collTriangle;
            ISceneNode* collNode = smgr->getSceneCollisionManager()->getSceneNodeAndCollisionPointFromRay(collRay, collPoint, collTriangle);

            if (collNode)
            {
              // get normal vector of triangle, no need to normalize()
              vector3df normalVec = collTriangle.getNormal();

              // create blood from collision point in normal direction
              new CBloodEffect(smgr, EGL_BRUTAL, collPoint, normalVec);
            }
          } break;
        }
      } break;
    }

    return false;
  }
};

int main()
{
  SIrrlichtCreationParameters deviceParams;

  //deviceParams.DriverType = EDT_OPENGL;
  deviceParams.DriverType = EDT_DIRECT3D9;
  deviceParams.WindowSize = dimension2du(800, 600);
  deviceParams.Bits       = 32;
  deviceParams.Fullscreen = false;
  deviceParams.Vsync      = false;
  deviceParams.AntiAlias  = 16;

  device = createDeviceEx(deviceParams);
  driver = device->getVideoDriver();
  smgr   = device->getSceneManager();

  device->getFileSystem()->addFileArchive("media/", true, true, EFAT_FOLDER);

  CEvents er;
  device->setEventReceiver(&er);

  ISceneNode* skybox = smgr->addSkyBoxSceneNode(
    driver->getTexture("irrlicht2_up.jpg"),
    driver->getTexture("irrlicht2_dn.jpg"),
    driver->getTexture("irrlicht2_lf.jpg"),
    driver->getTexture("irrlicht2_rt.jpg"),
    driver->getTexture("irrlicht2_ft.jpg"),
    driver->getTexture("irrlicht2_bk.jpg"));

  ICameraSceneNode *cam = smgr->addCameraSceneNodeFPS();
  cam->setPosition(vector3df(50,60,0));
  cam->setTarget  (vector3df(0,50,0));

  IMeshSceneNode* head = smgr->addSphereSceneNode(5);
  head->setPosition(vector3df(0,50,0));
  head->setMaterialFlag(EMF_LIGHTING, false);
  head->setRotation(vector3df(0,180,0));
  head->setMaterialTexture(0, driver->getTexture("face.png"));
  head->setMaterialType(EMT_SOLID);

  // Create TriangleSelector for onHit effects
  ITriangleSelector * tri = smgr->createTriangleSelector(head->getMesh(), head);
  head->setTriangleSelector(tri);
  tri->drop();

  // Make sure to create blood material somewhere
  CBloodShader::instance().createMaterial(device);

  s32 lastFPS = -1;
  while(device->run())
  {
    driver->beginScene(true, true, SColor(0, 0, 0, 0));
      smgr->drawAll();
    driver->endScene();

    s32 fps = driver->getFPS();

    if (lastFPS != fps)
    {
      stringw str  = L"Blood Effects | ";
              str += driver->getName();
              str += L" | ";
              str += fps;
              str += L" FPS";

      device->setWindowCaption(str.c_str());
      lastFPS = fps;
    }
  }

  device->drop();

  return 0;
} 