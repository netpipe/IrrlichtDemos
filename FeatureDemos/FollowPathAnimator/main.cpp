  #include <irrlicht.h>
  #include <CFollowPathAnimator.h>

using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

int main(int argc, char** argv) {
  IrrlichtDevice *device=createDevice(EDT_OPENGL,dimension2d<u32>(1024,600),16,false,false,false,0);

  device->setWindowCaption(L"FollowPathAnimator");

  IVideoDriver* driver = device->getVideoDriver();
  ISceneManager* smgr = device->getSceneManager();
  IGUIEnvironment* guienv = device->getGUIEnvironment();

  smgr->loadScene("data/irrOdeMarble_level_.xml");
  ICameraSceneNode *pCam=smgr->addCameraSceneNode(NULL,vector3df(0.0f,0.0f,0.0f),vector3df(1.0f,0.0f,0.0f));

  CFollowPathAnimator *myAnimator=new CFollowPathAnimator(device,"pathToFollow",120,500);
  pCam->addAnimator(myAnimator);
  myAnimator->setCamParameter(pCam);

  while(device->run()) {
    driver->beginScene(true, true, SColor(0,200,200,200));

    smgr->drawAll();
    guienv->drawAll();

    driver->endScene();
  }

  device->drop();
  return 0;
}

