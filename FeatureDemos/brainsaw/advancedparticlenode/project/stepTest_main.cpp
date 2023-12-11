  #include <irrlicht.h>
  #include <CAdvancedParticleSystemNode.h>

  #include <stdio.h>
  #include <conio.h>

using namespace irr;

class CStepStest : IEventReceiver {
  protected:
    class CMovingEmitter {
      protected:
        CAdvancedParticleSystemNode *m_pParticle;
        u32 m_iState,
            m_iType,
            m_iThreshold;

      public:
        CMovingEmitter(IrrlichtDevice *pDevice, u32 iInitialState,video::SColor cStart, video::SColor cEnd) {
          scene::ISceneManager *pSmgr=pDevice->getSceneManager();

          m_iThreshold=200;
          m_iType=iInitialState;
          m_iState=iInitialState;
          m_pParticle=(CAdvancedParticleSystemNode *)pSmgr->addSceneNode(ADVANCED_PARTICLE_NODE_NAME,pSmgr->getRootSceneNode());
          m_pParticle->setPosition(core::vector3df(0.0f,0.0f,0.0f));
          m_pParticle->setMaterialFlag(video::EMF_LIGHTING, false);
          m_pParticle->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
          m_pParticle->setMaterialType(video::EMT_TRANSPARENT_VERTEX_ALPHA);
          m_pParticle->setInterpolate(m_iType);
          m_pParticle->setInterpolateThreshold(m_iThreshold);
          m_pParticle->createPointEmitter(vector3df(0,0.01f,0),70,100,
                                          cStart,cEnd,10000,15000,5,
                                          dimension2df(1.5f,1.5f),dimension2df(2.75f,2.75f));

        }

        void step() {
            core::vector3df vPos=m_pParticle->getPosition();
            switch (m_iState) {
              case 0: vPos.X+=10.0f; if (vPos.X> 200) m_iState=1; break;
              case 1: vPos.Z+=10.0f; if (vPos.Z> 200) m_iState=2; break;
              case 2: vPos.X-=10.0f; if (vPos.X<-200) m_iState=3; break;
              case 3: vPos.Z-=10.0f; if (vPos.Z<-200) m_iState=0; break;
            }
            m_pParticle->setPosition(vPos);
        }

        u32 stepThreshold() {
          if (m_iThreshold==200)
            m_iThreshold=500;
          else
            if (m_iThreshold==500)
              m_iThreshold=0;
            else
              if (m_iThreshold==0)
                m_iThreshold=50;
              else
                if (m_iThreshold==50)
                  m_iThreshold=125;
                else
                  m_iThreshold=200;

          m_pParticle->setInterpolateThreshold(m_iThreshold);
          return m_iThreshold;
        }

        bool toggleInterpolate() {
          if (m_iType==0) m_pParticle->setInterpolate(!m_pParticle->doesInterpolate());
          return m_pParticle->doesInterpolate();
        }

        u32 getParticleCount() {
          return m_pParticle->getParticleCount();
        }

        const core::vector3df getPosition() {
          return m_pParticle->getPosition();
        }
    };

    IrrlichtDevice       *m_pDevice;
    video::IVideoDriver  *m_pDrv;
    scene::ISceneManager *m_pSmgr;
    gui::IGUIEnvironment *m_pGui;

    core::list<CMovingEmitter *> m_lEmitters;
    core::list<CMovingEmitter *>::Iterator m_itTarget;

    scene::ICameraSceneNode *m_pCam;

    gui::IGUIStaticText *m_lblMoving,
                        *m_lblInterpolate,
                        *m_lblThreshold,
                        *m_lblInfo;

    bool m_bMoving;

  public:
    virtual bool OnEvent(const SEvent &event) {
      if (event.EventType==EET_KEY_INPUT_EVENT) {
        if (!event.KeyInput.PressedDown) {
          if (event.KeyInput.Key==KEY_TAB) {
            core::list<CMovingEmitter *>::Iterator it=m_lEmitters.begin();
            bool b=(*it)->toggleInterpolate();
            m_lblInterpolate->setText(b?L"Interpolate":L"--"     );
          }

          if (event.KeyInput.Key==KEY_SPACE) {
            m_bMoving=!m_bMoving;
            m_lblMoving->setText(m_bMoving?L"Moving":L"Stopped");
          }

          if (event.KeyInput.Key==KEY_RETURN) {
            u32 i=0;
            list<CMovingEmitter *>::Iterator it;
            for (it=m_lEmitters.begin(); it!=m_lEmitters.end(); it++) i=(*it)->stepThreshold();

            wchar_t s[0xFF];
            swprintf(s,0xFF,L"Threshold: %i",i);
            m_lblThreshold->setText(s);
          }

          if (event.KeyInput.Key==KEY_BACK) {
            m_itTarget++;
            if (m_itTarget==m_lEmitters.end()) m_itTarget=m_lEmitters.begin();
          }
        }
      }
      return false;
    }

    void run() {
      m_bMoving=true;

      m_pDevice=createDevice(video::EDT_OPENGL,dimension2d<u32>(800,600),16,false,false,false,0);

      m_pDrv =m_pDevice->getVideoDriver   ();
      m_pSmgr=m_pDevice->getSceneManager  ();
      m_pGui =m_pDevice->getGUIEnvironment();

      m_pCam=m_pSmgr->addCameraSceneNode();
      m_pCam->setPosition(core::vector3df(250.0f,50.0f,0.0f));
      m_pCam->setTarget(core::vector3df(0.0f,0.0f,0.0f));

      m_pDevice->setEventReceiver(this);

      core::position2di cPos=core::position2di(10,10),cOff=core::position2di(0,30);
      core::dimension2du cDim=core::dimension2du(150,25);

      m_lblMoving     =m_pGui->addStaticText(L"Moving"     ,core::rect<s32>(cPos,cDim),true,true,NULL,-1,true); cPos+=cOff;
      m_lblInterpolate=m_pGui->addStaticText(L"Interpolate",core::rect<s32>(cPos,cDim),true,true,NULL,-1,true); cPos+=cOff;
      m_lblThreshold  =m_pGui->addStaticText(L"Threshold"  ,core::rect<s32>(cPos,cDim),true,true,NULL,-1,true); cPos+=cOff;
      m_lblInfo       =m_pGui->addStaticText(L"Info"       ,core::rect<s32>(cPos,cDim),true,true,NULL,-1,true);

      m_lblMoving     ->setTextAlignment(gui::EGUIA_CENTER,gui::EGUIA_CENTER);
      m_lblInterpolate->setTextAlignment(gui::EGUIA_CENTER,gui::EGUIA_CENTER);
      m_lblThreshold  ->setTextAlignment(gui::EGUIA_CENTER,gui::EGUIA_CENTER);

      CAdvancedParticleSystemNodeFactory *pFactory=new CAdvancedParticleSystemNodeFactory(m_pSmgr);
      m_pSmgr->registerSceneNodeFactory(pFactory);

      CMovingEmitter *pEmitter=new CMovingEmitter(m_pDevice,0,video::SColor(255,255,192,128),video::SColor(96,96,0,0));
      m_lEmitters.push_back(pEmitter);

      pEmitter=new CMovingEmitter(m_pDevice,2,video::SColor(255,128,192,255),video::SColor(96,0,0,96));
      m_lEmitters.push_back(pEmitter);

      ITimer *pTimer=m_pDevice->getTimer();
      u32 iLastTime=pTimer->getTime(),iDelta=0;

      m_itTarget=m_lEmitters.begin();
      while(m_pDevice->run()) {
        u32 iThisTime=pTimer->getTime(),iDiffTime=iThisTime-iLastTime;
        iLastTime=iThisTime;
        iDelta+=iDiffTime;
        while (iDelta>75) {
          if (m_bMoving) {
            core::list<CMovingEmitter *>::Iterator it;
            for (it=m_lEmitters.begin(); it!=m_lEmitters.end(); it++) (*it)->step();
          }

          m_pCam->setTarget((*m_itTarget)->getPosition());
          iDelta-=75;
        }

        wchar_t s[0xFF];
        s32 iFps=m_pDrv->getFPS();

        u32 iCnt=0;
        core::list<CMovingEmitter *>::Iterator it;
        for (it=m_lEmitters.begin(); it!=m_lEmitters.end(); it++) iCnt+=(*it)->getParticleCount();
        swprintf(s,0xFF,L"%i Frame per Second\n%i Particles",iFps,iCnt);
        m_lblInfo->setText(s);

        m_pDrv->beginScene(true, true, SColor(0,200,200,200));

        m_pSmgr->drawAll();
        m_pGui ->drawAll();

        m_pDrv->endScene();
      }
    }
};

int main(void) {
  CStepStest cTest;
  cTest.run();
}
