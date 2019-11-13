#include <irrlicht.h>
#include <IrrOde.h>

using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;
using namespace ode;

int main(int argc, char** argv) {
  IrrlichtDevice *device=createDevice(EDT_DIRECT3D9, dimension2d<u32>(640,480),16,false,false,false,0);
  device->setWindowCaption(L"Hello World! - Irrlicht-ODE Engine Demo");

  IVideoDriver *driver = device->getVideoDriver();
  ISceneManager *smgr = device->getSceneManager();
  IGUIEnvironment *guienv = device->getGUIEnvironment();
  ICursorControl *pCursor=device->getCursorControl();
  pCursor->setVisible(false);

  CIrrOdeSceneNodeFactory cFactory(smgr);
  smgr->registerSceneNodeFactory(&cFactory);

  smgr->loadScene("../../data/IrrOdEdit_example.xml");

  //init the ODE
  CIrrOdeManager::getSharedInstance()->initODE();
  CIrrOdeManager::getSharedInstance()->initPhysics();

  //set IrrODE's timer
  ITimer *pTimer=device->getTimer();
  CIrrOdeManager::getSharedInstance()->setTimer(pTimer);

  ICameraSceneNode *cam=reinterpret_cast<ICameraSceneNode *>(smgr->getSceneNodeFromName("theCamera"));
  cam->setTarget(vector3df(0,0,0));

  while (pTimer->isStopped()) pTimer->start();

  //let's run the loop
  while(device->run()) {
    //step the simulation
    CIrrOdeManager::getSharedInstance()->step();

    //now for the normal Irrlicht stuff ... begin, draw and end scene and update window caption
    driver->beginScene(true, true, SColor(0,200,200,200));

    smgr->drawAll();
    guienv->drawAll();

    driver->endScene();
  }

  //drop the world so it is destroyed
  device->drop();

  return 0;
}

