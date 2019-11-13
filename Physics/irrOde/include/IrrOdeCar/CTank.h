#ifndef _C_TANK
  #define _C_TANK

  #include <irrlicht.h>
  #include <IrrOde.h>

  #include "CVehicle.h"

using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

class CProjectile;

class CTank : public CVehicle, public IEventReceiver, public IIrrOdeEventListener {
  protected:
    s32 m_iAcc,m_iSteer,m_iTurretTurn,m_iCannon;
    bool m_bBrake,m_bFlip,m_bFollowTurret,m_bFastCollision;
    bool m_bParamInc,m_bParamDec;
    bool m_bHelp,m_bFire,m_bFollowBullet;
    ITexture *m_pTextures[2];

    IGUIStaticText *m_pInfo;
    f32 m_fSteer,m_fActSteer;

    CIrrOdeWorld *m_pWorld;
    CIrrOdeMotor *m_pMotor[4];
    CIrrOdeBody *m_pTankBody,*m_pTurret,*m_pCannon;
    CIrrOdeServo *m_pTurretServo,*m_pCannonServo;

    ICameraSceneNode *m_pCam;

    int getSteer();
    int getAcc  ();

  public:
    CTank(IrrlichtDevice *pDevice);
    virtual ~CTank();

    virtual void activate();
    virtual void deactivate();
    virtual u32 update();

    virtual bool OnEvent(const SEvent &event);

    virtual bool onEvent(IIrrOdeEvent *pEvent);
    virtual bool handlesEvent(IIrrOdeEvent *pEvent);
};

#endif
