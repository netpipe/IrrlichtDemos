#ifndef _CLEVELOBJECT
  #define _CLEVELOBJECT

  #include <irrlicht.h>
//  #include <irrklang.h>
  #include "ode/IIrrOdeEntity.h"

  #include "CMarble.h"

class CLevelObject : public irr::ode::IIrrOdeEntity
{
  private:
    CMarble *m_pMarble;
    int       m_iCheckPoint;
    int       m_iDelay;
    irr::c8   m_sObjName[30],m_sClass[30];
    dMass     m_cMass;
    irr::f32  m_fMassValue;

    irr::scene::ISceneNode   *m_pCheckLight;
//    irr::audio::ISoundEngine *m_pSound;

    irr::gui::IGUIImage *m_pImg, *m_pImgOk;


  public:
    CLevelObject(irr::ode::CIrrOdeManager *pManager, irr::IrrlichtDevice *pDevice);
    void collisionCallback(IIrrOdeEntity *pOpponent, int iContactCnt, dJointFeedback *pFeedback);
    void setMarble(CMarble *pMarble);
    void setCheckPoint(int i);
    void setCheckLight(irr::scene::ISceneNode *pCheckLight);
    void setObjName(irr::c8 *sName) { strcpy(m_sObjName,sName ); }
    void setClass(irr::c8 *sClass ) { strcpy(m_sClass  ,sClass); }
    void setDelay(int iDelay) {m_iDelay=iDelay; }

    void setImages(irr::gui::IGUIImage *pImg, irr::gui::IGUIImage *pImgOk);

    irr::c8 *getObjName(void) { return m_sObjName; }
    irr::c8 *getClass  (void) { return m_sClass  ; }

    int getCheckPointNo();
    int getDelay(void) { return m_iDelay; }

    void startDelay(void) { m_iDelay=getDelay(); }
    void addTime(irr::u32 iDelta) { m_iDelay-=iDelta; }

//    void setSoundEngine(irr::audio::ISoundEngine *pSound) { m_pSound=pSound; }

    irr::f32 getMass(void) { return 0; /*(irr::f32)m_fMass; */}
};

#endif
