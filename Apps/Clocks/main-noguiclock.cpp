/** (c) 2008/09 by Acki B.
  !¡!¡!!¡!¡!¡!¡!¡!¡!¡!¡!¡!¡!¡!¡!¡!¡!¡!¡!¡!¡!¡!¡!¡!¡!¡
  !¡!¡! Code::Blocks-Template - 2006 by Acki B. !¡!¡!
  !¡!¡!¡!¡!¡¡!¡!¡!¡!¡!¡!¡!¡!¡!¡!¡!¡!¡!¡!¡!¡!¡!¡!¡!¡!¡
**/

#include <irrlicht.h>
#include "CGUIClock.h"
#include "CClockSceneNode.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

int main(){
	IrrlichtDevice *device = createDevice(EDT_OPENGL, dimension2d<s32>(640, 480), 16, false, false, false, 0);
	IVideoDriver* driver = device->getVideoDriver();
	ISceneManager* smgr = device->getSceneManager();
	IGUIEnvironment* guienv = device->getGUIEnvironment();

  device->getFileSystem()->addZipFileArchive("media.zip");

  // load textures for GUI-Clock
  driver->setTextureCreationFlag(ETCF_CREATE_MIP_MAPS, false);
  ITexture* cBody = driver->getTexture("c1body.bmp");
  driver->makeColorKeyTexture(cBody, position2d<s32>(1,1));
  ITexture* cNeedleH = driver->getTexture("c1hour.bmp");
  driver->makeColorKeyTexture(cNeedleH, position2d<s32>(1,1));
  ITexture* cNeedleM = driver->getTexture("c1minute.bmp");
  driver->makeColorKeyTexture(cNeedleM, position2d<s32>(1,1));
  ITexture* cNeedleS = driver->getTexture("c1second.bmp");
  driver->makeColorKeyTexture(cNeedleS, position2d<s32>(1,1));
  driver->setTextureCreationFlag(ETCF_CREATE_MIP_MAPS, true);
  // create GUI clock
  /*
  CGUIClock* guiClock = new CGUIClock(rect<s32>(10,10,150,150), guienv, guienv->getRootGUIElement());
  guiClock->drop();
  // set clock properties
  guiClock->setBodyTexture(cBody);
  guiClock->setNeedleTexture(cNeedleH, cNeedleM, cNeedleS);
  guiClock->setSecondsForDay(3600); // 1 day per hour
  guiClock->setTime(21, 30, 0);
  guiClock->setUpdatesPerSecond(20);
  guiClock->setAutoUpdate(true);
  guiClock->setShowSeconds(true);
  guiClock->setUseRealTime(false);
*/
  // load meshes for Scene-Clock
  IAnimatedMesh* mBody = smgr->getMesh("mbody.x");
  IAnimatedMesh* mHour = smgr->getMesh("mhour.x");
  IAnimatedMesh* mMinute = smgr->getMesh("mminute.x");
  IAnimatedMesh* mSecond = smgr->getMesh("msecond.x");
  // create nodes for Scene-Clock
  ISceneNode* nBody = smgr->addAnimatedMeshSceneNode(mBody);
  ISceneNode* nHour = smgr->addAnimatedMeshSceneNode(mHour, nBody);
  ISceneNode* nMinute = smgr->addAnimatedMeshSceneNode(mMinute, nBody);
  ISceneNode* nSecond = smgr->addAnimatedMeshSceneNode(mSecond, nBody);
  nBody->setPosition(vector3df(-20,-20,100));
  nBody->setScale(vector3df(2,2,2));
  nBody->setMaterialFlag(EMF_LIGHTING, false);
  nHour->setMaterialFlag(EMF_LIGHTING, false);
  nMinute->setMaterialFlag(EMF_LIGHTING, false);
  nSecond->setMaterialFlag(EMF_LIGHTING, false);
  // create Scene clock
/* NOTE (Acki#1#): I know this should be an animator and not a node,
   but I'm to lazy to learn how to use animators !!! ;)) */
  CClockSceneNode* nodeClock = new CClockSceneNode(device, nHour, nMinute, nSecond);
  nodeClock->drop();
  // sync with another clock
//  nodeClock->setSyncClock(guiClock);
/* or set clock properties
  nodeClock->setSecondsForDay(3600); // 1 day per hour
  nodeClock->setTime(21, 30, 0);
  nodeClock->setUpdatesPerSecond(20);
  nodeClock->setAutoUpdate(true);
  nodeClock->setUseRealTime(false);
*/

	// setup and run Irrlicht render loop
  smgr->addCameraSceneNodeFPS();
  device->getCursorControl()->setVisible(false);
	while(device->run()){
		driver->beginScene(true, true, video::SColor(0,220,220,220));
		smgr->drawAll();
		guienv->drawAll();
		driver->endScene();
//    device->setWindowCaption(guiClock->getGameTimeStringW().c_str());
	}
	device->drop();
	return 0;
}
