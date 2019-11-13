  #include <stdio.h>
  #include <time.h>
  #include <irrlicht.h>
  #include <IrrOde.h>

  #include "CCar.h"
  #include "CTank.h"
  #include "CMenu.h"
  #include "CHeli.h"
  #include "CPlane.h"
  #include "CVehicle.h"

using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

int main(int argc, char** argv) {
  CVehicle *pActive=NULL;
  array<CVehicle *> aVehicles;

  IrrlichtDevice *device=createDevice(EDT_OPENGL,irr::core::dimension2d<u32>(640,480),16,false,false,false,0);
  device->setWindowCaption(L"IrrODE car demo");

  IVideoDriver *driver = device->getVideoDriver();
  ISceneManager *smgr = device->getSceneManager();
  IGUIEnvironment *guienv = device->getGUIEnvironment();

  CIrrOdeSceneNodeFactory cFactory(smgr);
  smgr->registerSceneNodeFactory(&cFactory);

  dimension2d<u32> cSize=driver->getScreenSize();

  //init the ODE
  CIrrOdeManager::getSharedInstance()->initODE();

  //set IrrODE's timer
  ITimer *pTimer=device->getTimer();
  CIrrOdeManager::getSharedInstance()->setTimer(pTimer);

  smgr->loadScene("../../data/IrrOdeCar.xml");
  vector3df camPos=vector3df(1750,99,2500);
  ICameraSceneNode *cam=smgr->addCameraSceneNode();

  cam->setPosition(camPos);
  IAnimatedMeshSceneNode *pNode=(IAnimatedMeshSceneNode *)smgr->getSceneNodeFromName("car_segment");
  if (pNode) pNode->getMaterial(0).getTextureMatrix(0).setTextureScale(50.0f,50.0f);
  pNode=(IAnimatedMeshSceneNode *)smgr->getSceneNodeFromName("tank_segment");
  if (pNode) pNode->getMaterial(0).getTextureMatrix(0).setTextureScale(50.0f,50.0f);

  CIrrOdeManager::getSharedInstance()->initPhysics();

  int lastFPS=-1;
  srand(time(NULL));
  while (pTimer->isStopped()) pTimer->start();

  CMenu  *theMenu =new CMenu (device); aVehicles.push_back(theMenu );
  CCar   *theCar  =new CCar  (device); aVehicles.push_back(theCar  );
  CTank  *theTank =new CTank (device); aVehicles.push_back(theTank );
  CHeli  *theHeli =new CHeli (device); aVehicles.push_back(theHeli );
  CPlane *thePlane=new CPlane(device); aVehicles.push_back(thePlane);

  pActive=theMenu;
  pActive->activate();

  //let's run the loop
  while(device->run()) {
    CIrrOdeManager::getSharedInstance()->step();
    u32 iSwitch=pActive->update();

    if (iSwitch) {
      iSwitch--;
      pActive->deactivate();
      pActive=aVehicles[iSwitch];
      pActive->activate();
      iSwitch=0;
    }

    driver->beginScene(true, true, SColor(0,200,200,200));

    //now for the normal Irrlicht stuff ... begin, draw and end scene and update window caption
    smgr->drawAll();
    guienv->drawAll();

    driver->endScene();
    int fps = driver->getFPS();

    if (lastFPS != fps) {
      core::stringw str = L"Irrlicht Engine - IrrODE Car Demo [";
      str += driver->getName();
      str += "] FPS:";
      str += fps;

      device->setWindowCaption(str.c_str());
      lastFPS = fps;
    }
  }

  //drop the world so it is destroyed
  device->drop();

  delete theCar;
  delete theTank;

  return 0;
}

