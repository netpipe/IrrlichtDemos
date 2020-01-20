  #include <irrlicht.h>
  #include <CAdvancedParticleSystemNode.h>

  #include <stdio.h>
  #include <conio.h>

using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

class CAdvancedParticleDemo : IEventReceiver {
  protected:
    CAdvancedParticleSystemNode *m_pParticleNode,*m_pClone;
    IrrlichtDevice *m_pDevice;
    IGUIStaticText *m_pInfo;
    IGUIButton *m_pBtnRemove,*m_pBtnActive;
    IGUISpinBox *m_pMinParticles,*m_pMaxParticles;
    IVideoDriver *m_pDriver;

  public:
    virtual bool OnEvent(const SEvent &event) {
      bool bRet=false;
      if (event.EventType==EET_GUI_EVENT) {
        if (event.GUIEvent.EventType==EGET_BUTTON_CLICKED) {
          IGUIButton *p=(IGUIButton *)event.GUIEvent.Caller;

          s32 iMin=(s32)m_pMinParticles->getValue(),
              iMax=(s32)m_pMaxParticles->getValue();

          switch (p->getID()) {
            case 1: // point;
              m_pParticleNode->createPointEmitter(vector3df(0,0.02f,0),
                                                  iMin,
                                                  iMax,SColor(0,127,0,0),
                                                  SColor(0,255,127,127),800,2000,5,
                                                  dimension2df(0.25,0.25),dimension2df(0.75,0.75));
              m_pBtnActive->setEnabled(true);
              m_pParticleNode->setAtlasSize(0);
              m_pParticleNode->setMaterialTexture(0, m_pDriver->getTexture("../data/texture.jpg"));
              break;

            case 2: //box;
              m_pParticleNode->createBoxEmitter(
                      core::aabbox3d<f32>(-1,0,-1,1,1,1), // emitter size
                      core::vector3df(0.0f,0.02f,0.0f),   // initial direction
                      iMin,
                      iMax,                             // emit rate
                      video::SColor(0,127,0,0),       // darkest color
                      video::SColor(0,255,127,127),       // brightest color
                      800,2000,5,                         // min and max age, angle
                      dimension2df(0.25,0.25),           //min size
                      dimension2df(0.75,0.75));        // max size
              m_pBtnActive->setEnabled(true);
              m_pParticleNode->setAtlasSize(0);
              m_pParticleNode->setMaterialTexture(0, m_pDriver->getTexture("../data/texture.jpg"));
              break;

            case 3: //sphere
              m_pParticleNode->createSphereEmitter(vector3df(0,0,0),2,vector3df(0,0.02f,0),
                                                   iMin,
                                                   iMax,
                                                   SColor(0,127,0,0),SColor(0,255,127,127),
                                                   800,2000,5,
                                                   dimension2df(0.25,0.25),dimension2df(0.75,0.75));
              m_pBtnActive->setEnabled(true);
              m_pParticleNode->setAtlasSize(0);
              m_pParticleNode->setMaterialTexture(0, m_pDriver->getTexture("../data/texture.jpg"));
              break;

            case 4: //cylinder
              m_pParticleNode->createCylinderEmitter(vector3df(0,0,0),1,vector3df(0,1,0),3,true,
                                                     vector3df(0,0.02f,0),
                                                     iMin,
                                                     iMax,
                                                     SColor(0,127,0,0),SColor(0,255,127,127),
                                                     800,2000,5,dimension2df(0.25,0.25),dimension2df(0.75,0.75));
              m_pBtnActive->setEnabled(true);
              m_pParticleNode->setAtlasSize(0);
              m_pParticleNode->setMaterialTexture(0, m_pDriver->getTexture("../data/texture.jpg"));
              break;

            case 5: //ring
              m_pParticleNode->createRingEmitter(vector3df(0,0,0),2,0.01,vector3df(0,0.02f,0),
                                                 iMin,
                                                 iMax,
                                                 SColor(0,127,0,0),SColor(0,255,127,127),
                                                 800,2000,5,dimension2df(0.25,0.25),dimension2df(0.75,0.75));
              m_pBtnActive->setEnabled(true);
              m_pParticleNode->setAtlasSize(0);
              m_pParticleNode->setMaterialTexture(0, m_pDriver->getTexture("../data/texture.jpg"));
              break;

            case 6: //clone node
              if (m_pClone!=NULL) {
                m_pDevice->getSceneManager()->addToDeletionQueue(m_pClone);
              }
              m_pClone=(CAdvancedParticleSystemNode *)m_pParticleNode->clone();
              m_pClone->setPosition(core::vector3df(10.0f,0,0));
              m_pBtnRemove->setEnabled(true);
              break;

            case 7:   //remove clone
              if (m_pClone!=NULL) {
                m_pDevice->getSceneManager()->addToDeletionQueue(m_pClone);
              }
              m_pBtnRemove->setEnabled(false);
              break;

            case 8:   //activate
              if (m_pParticleNode->isActive()) {
                m_pParticleNode->setIsActive(false);
                m_pBtnActive->setText(L"Activate");
              }
              else {
                m_pParticleNode->setIsActive(true);
                m_pBtnActive->setText(L"Deactivate");
              }
              break;

            case 9:
              m_pParticleNode->createExplosionEmitter(vector3df(0,0,0),1.0f,vector3df(0.0f,0.01f,0.0f),
                                                      iMin,
                                                      iMax,
                                                      SColor(0,127,0,0),SColor(0,255,127,127),
                                                      500,1000,
                                                      dimension2df(10,10),dimension2df(15,15));
              m_pBtnActive->setEnabled(true);
              m_pParticleNode->setMaterialTexture(0, m_pDriver->getTexture("../data/explode.png"));
              m_pParticleNode->setAtlasSize(4);
              break;


            default: break;
          }
        }
      }
      return bRet;
    }

    virtual ~CAdvancedParticleDemo() {
    }

    void run() {
      IrrlichtDevice *device=createDevice(EDT_DIRECT3D9,dimension2d<u32>(800,600),16,false,false,false,0);
      m_pDevice=device;
      m_pDriver=device->getVideoDriver();
      m_pClone=NULL;

      device->setWindowCaption(L"ParticleClone");

      IVideoDriver* driver = device->getVideoDriver();
      ISceneManager* smgr = device->getSceneManager();
      IGUIEnvironment* guienv = device->getGUIEnvironment();

      device->setEventReceiver(this);

      position2di pos=position2di(5,65);
      dimension2du dim=dimension2du(90,20);
      guienv->addButton(rect<s32>(pos,dim),0,1,L"Point Emitter"    ); pos.Y+=30;
      guienv->addButton(rect<s32>(pos,dim),0,2,L"Box Emitter"      ); pos.Y+=30;
      guienv->addButton(rect<s32>(pos,dim),0,3,L"Sphere Emitter"   ); pos.Y+=30;
      guienv->addButton(rect<s32>(pos,dim),0,4,L"Cylinder Emitter" ); pos.Y+=30;
      guienv->addButton(rect<s32>(pos,dim),0,5,L"Ring Emitter"     ); pos.Y+=30;
      guienv->addButton(rect<s32>(pos,dim),0,9,L"Explosion Emitter"); pos.Y+=50;
      guienv->addButton(rect<s32>(pos,dim),0,6,L"Clone Node"       ); pos.Y+=30;
      m_pBtnRemove=guienv->addButton(rect<s32>(pos,dim),0,7,L"Remove Clone"); pos.Y+=30;
      m_pBtnActive=guienv->addButton(rect<s32>(pos,dim),0,8,L"Deactivate"); pos.Y+=30;
      guienv->addStaticText(L"Min Particles",rect<s32>(pos,dim)); pos.Y+=20;
      m_pMinParticles=guienv->addSpinBox(L"Min Particles",rect<s32>(pos,dim)); pos.Y+=30;
      guienv->addStaticText(L"Max Particles",rect<s32>(pos,dim)); pos.Y+=20;
      m_pMaxParticles=guienv->addSpinBox(L"Min Particles",rect<s32>(pos,dim)); pos.Y+=30;

      m_pMaxParticles->setStepSize(1.0f); m_pMaxParticles->setRange(5,2000);
      m_pMinParticles->setStepSize(1.0f); m_pMinParticles->setRange(5,2000);

      m_pMaxParticles->setValue(50);
      m_pMinParticles->setValue(30);

      m_pBtnRemove->setEnabled(false);
      m_pBtnActive->setEnabled(false);

      m_pInfo=guienv->addStaticText(L"AdvancedParticleSystemNode",rect<s32>(5,5,145,55),true,true,0,-1,true);

      CAdvancedParticleSystemNodeFactory *pFactory=new CAdvancedParticleSystemNodeFactory(smgr);
      smgr->registerSceneNodeFactory(pFactory);

      m_pParticleNode=(CAdvancedParticleSystemNode *)smgr->addSceneNode(ADVANCED_PARTICLE_NODE_NAME,smgr->getRootSceneNode());

      /*scene::IParticleAffector* paf = m_pParticleNode->createFadeOutParticleAffector(SColor(0xFF,127,0,0),750);

      m_pParticleNode->addAffector(paf); // same goes for the affector
      paf->drop();*/

      m_pParticleNode->setPosition(core::vector3df(-10.0f,0,0));
      m_pParticleNode->setScale(core::vector3df(1,1,1));
      m_pParticleNode->setMaterialFlag(video::EMF_LIGHTING, false);
      m_pParticleNode->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
      m_pParticleNode->setMaterialType(video::EMT_TRANSPARENT_VERTEX_ALPHA);
      ICameraSceneNode *pCam=smgr->addCameraSceneNode();
      pCam->setPosition(vector3df(0,15,-30));
      pCam->setTarget(vector3df(0,15,0));

      while(device->run()) {
        driver->beginScene(true, true, SColor(0,200,200,200));


        smgr->drawAll();
        guienv->drawAll();

        driver->endScene();

        int fps = driver->getFPS();

        core::stringw str=L"AdvancedParticleNode\n";
        str += fps;
        str += L" Frames per second\n";
        str += m_pParticleNode->getParticleCount();
        str += L" Particles\n";
        str += m_pParticleNode->isActive()?L"Node is active\n":L"Node inactive\n";

        if (m_pClone==NULL)
          str+=L"No Clone!\n";
        else {
          str+=L"Clone: ";
          str+=m_pClone->getParticleCount();
          str+=L" Particles.";
        }

        m_pInfo->setText(str.c_str());
      }

      device->drop();
    }
};

int main(int argc, char** argv) {
  CAdvancedParticleDemo pDemo;
  pDemo.run();
  return 0;
}

