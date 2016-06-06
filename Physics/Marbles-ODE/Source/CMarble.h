#ifndef _CMARBLE
  #define _CMARBLE

  #include <irrlicht.h>
//  #include <irrklang.h>
  #include <ode/ode.h>

  #include "ode/IIrrOdeEntity.h"
  #include "ode/CIrrOdeManager.h"

class CMarble : public irr::ode::IIrrOdeEntity
{
  private:
//    irr::audio::ISoundEngine *m_pSndEngine;
 //   irr::audio::ISound       *m_pBounce,*m_pRolling;

    int m_iJointCnt,m_iNextCheckPoint;

    irr::f32 m_fRespawn;

    bool m_bCheckPointHit;

  public:
    CMarble(irr::ode::CIrrOdeManager *pManager, irr::IrrlichtDevice *pDevice);
    ~CMarble();

    void collisionCallback(IIrrOdeEntity *pOpponent, int iContactCntd, dJointFeedback *pFeedback);
//    void setSoundEngine(irr::audio::ISoundEngine *pSound);
    void frameCallback();
    void stopRollingSound();

    void checkPointHit(int iNr);
    void setRespawn(irr::f32 fRespawn);

    bool checkPointHit();
    bool respawn();

    int getNextCheckPoint();
};

#endif
