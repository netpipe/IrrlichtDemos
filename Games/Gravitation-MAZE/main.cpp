#include <irrlicht.h>
#include <string.h>
#include <IMeshLoader.h>

#include "CBox2DSceneNodeFactory.h"
#include "CBox2DSceneNode.h"
#include "CBox2DImageSceneNode.h"
#include "CBox2DImageRootSceneNode.h"
#include "CBox2DAnimatedImageMesh.h"
#include "CBox2DMeshLoader.h"
#include "CBox2DManager.h"

using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

class theEventReceiver : public IEventReceiver {
  private:
    bool m_bDrawBox2D,m_bDrawBoundingBox,m_bEvent,m_bPosEvent,m_bPos,m_bRotEvent,m_bRot,m_bScaleEvent,m_bScale,m_bClearEvent,
         m_bImageEvent;
    vector3df m_pPos1,m_pPos2,m_pRot1,m_pRot2,m_pScale1,m_pScale2;
    int m_iImage;

    char m_sImages[3][0xFF];

  public:
    theEventReceiver() {
      strcpy(m_sImages[0],"levels/a_level.png");
      strcpy(m_sImages[2],"levels/level2.png");
      strcpy(m_sImages[1],"levels/big_level.png");

      m_iImage=0;
      m_bDrawBox2D=false;
      m_bDrawBoundingBox=false;
      m_bEvent=false;
      m_bPosEvent=false;
      m_bPos=false;
      m_bImageEvent=false;
      m_bClearEvent=false;

      m_pPos1=vector3df(0,0,0);
      m_pPos2=vector3df(500,0,0);

      m_pRot1=vector3df(0,0,0);
      m_pRot2=vector3df(45,45,45);

      m_pScale1=vector3df(0.5,0.5,0.5);
      m_pScale2=vector3df(2,2,2);
    }

    virtual bool OnEvent(const SEvent &event) {
      if (event.EventType==irr::EET_KEY_INPUT_EVENT) {
        if (event.KeyInput.Key==KEY_KEY_B && event.KeyInput.PressedDown) {
          m_bEvent=true;
          m_bDrawBoundingBox=!m_bDrawBoundingBox;
        }

        if (event.KeyInput.Key==KEY_KEY_W && event.KeyInput.PressedDown) {
          m_bEvent=true;
          m_bDrawBox2D=!m_bDrawBox2D;
        }

        if (event.KeyInput.Key==KEY_KEY_P && event.KeyInput.PressedDown) {
          m_bPosEvent=true;
          m_bPos=!m_bPos;
        }

        if (event.KeyInput.Key==KEY_KEY_S && event.KeyInput.PressedDown) {
          m_bScaleEvent=true;
          m_bScale=!m_bScale;
        }

        if (event.KeyInput.Key==KEY_KEY_R && event.KeyInput.PressedDown) {
          m_bRotEvent=true;
          m_bRot=!m_bRot;
        }

        if (event.KeyInput.Key==KEY_KEY_I && event.KeyInput.PressedDown) m_bImageEvent=true;
        if (event.KeyInput.Key==KEY_KEY_C && event.KeyInput.PressedDown) m_bClearEvent=true;
      }
      return false;
    }

    virtual ~theEventReceiver() {
    }

    bool event() {
      bool bRet=m_bEvent;
      m_bEvent=false;
      return bRet;
    }

    bool drawBox2D() {
      return m_bDrawBox2D;
    }

    bool drawBoundingBox() {
      return m_bDrawBoundingBox;
    }

    bool posEvent() {
      bool bRet=m_bPosEvent;
      return bRet;
    }

    vector3df &getPosition() {
      return m_bPos?m_pPos2:m_pPos1;
    }

    bool scaleEvent() {
      return m_bScaleEvent;
    }

    vector3df &getScale() {
      return m_bScale?m_pScale1:m_pScale2;
    }

    bool rotEvent() {
      return m_bRotEvent;
    }

    vector3df &getRot() {
      return m_bRot?m_pRot1:m_pRot2;
    }

    bool clearEvent() {
      bool bRet=m_bClearEvent;
      m_bClearEvent=false;
      return bRet;
    }

    bool imageEvent() {
      bool bRet=m_bImageEvent;
      m_bImageEvent=false;
      return bRet;
    }

    const char *imageName() {
      m_iImage++;
      if (m_iImage>2) m_iImage-=3;
      return m_sImages[m_iImage];
    }
};

int main(int argc, char** argv)
{
  theEventReceiver aReceiver;
  IrrlichtDevice *device=createDevice(EDT_OPENGL, dimension2du(800,600), 16, false, false, true, &aReceiver);
  device->setWindowCaption(L"Hello World! - Irrlicht Engine Demo");

  array <CBox2DImageSceneNode *> theB2DNodes;

  IVideoDriver* driver = device->getVideoDriver();
  ISceneManager* smgr = device->getSceneManager();
  IGUIEnvironment* guienv = device->getGUIEnvironment();

  CBox2DSceneNodeFactory* theFactory=new CBox2DSceneNodeFactory(smgr);
  smgr->registerSceneNodeFactory(theFactory);

  smgr->addExternalMeshLoader(new CBox2DMeshLoader(driver));
  CBox2DImageRootSceneNode *b2dnode=reinterpret_cast<CBox2DImageRootSceneNode *>(smgr->addSceneNode("CBox2DImageRootSceneNode",NULL));
 // b2dnode->loadImageMesh("levels/wmb.png");
    b2dnode->loadImageMesh("mazes/cretan-labyrinth-round_svg.png");
b2dnode->setRotation(vector3df(90,0,0));
//b2dnode->getsc

  IAnimatedMesh* mesh = smgr->getMesh("data/gravity-recon.3ds");
  IAnimatedMeshSceneNode* node = smgr->addAnimatedMeshSceneNode( mesh );

  if (node)
  {
    node->setMaterialFlag(EMF_LIGHTING, false);
    node->setFrameLoop(0, 310);
    node->setMaterialTexture( 0, driver->getTexture("data/_recon_body.bmp") );
  }

  ICameraSceneNode *camera = smgr->addCameraSceneNodeFPS();
camera->setFarValue (4000);
  int lastFPS=-1;
  while(device->run())
  {
    driver->beginScene(true, true, SColor(0,200,200,200));

    smgr->drawAll();
    guienv->drawAll();

    driver->endScene();

//    if (aReceiver.event()) {
//      b2dnode->setDrawBox2D(aReceiver.drawBox2D());
//      b2dnode->setDrawBoundingBox(aReceiver.drawBoundingBox());
//    }
//
//    if (aReceiver.posEvent()) {
//      b2dnode->setPosition(aReceiver.getPosition());
//    }
//
//    if (aReceiver.rotEvent()) {
//      b2dnode->setRotation(aReceiver.getRot());
//    }
//
//    if (aReceiver.scaleEvent()) {
//      b2dnode->setScale(aReceiver.getScale());
//    }

//    if (aReceiver.clearEvent()) b2dnode->clear();

//    if (aReceiver.imageEvent()) b2dnode->loadImageMesh(aReceiver.imageName());
//  b2dnode->setDrawBoundingBox(aReceiver.drawBoundingBox());
    int fps = driver->getFPS();
    if (lastFPS != fps)
    {
      core::stringw str = L"Irrlicht Engine - [";
      str += driver->getName();
      str += "] FPS:";
      str += fps;
      str += " Primitives drawn: ";
      str += driver->getPrimitiveCountDrawn();
      device->setWindowCaption(str.c_str());
      lastFPS = fps;
    }
    device->sleep(5,0);
  }

  device->drop();

  return 0;
}

