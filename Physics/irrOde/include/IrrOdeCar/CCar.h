#ifndef _C_CAR
  #define _C_CAR

  #include <irrlicht.h>
  #include <IrrOde.h>

  #include "CVehicle.h"

using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

class frontWheelContact : public CIrrOdeSurfaceParameters {
  public:
    CIrrOdeBody *m_pCar;
    CIrrOdeJointHinge2 *m_pJoint;

    f32 f;
    bool m_bSteering;

    virtual f32 getSlip2();
    virtual s32 getMode();
};

class rearWheelContact : public CIrrOdeSurfaceParameters {
  public:
    CIrrOdeBody *m_pCar;

    f32 f;
    bool m_bSteering;

    virtual f32 getSlip2();
    virtual s32 getMode();
};

class CCar : public CVehicle, public IEventReceiver {
  protected:
    s32 m_iSteer,m_iAcc;
    bool m_bBrake,m_bBoost,m_bFlip;
    bool m_bAdaptSteer,m_bHelp,m_bFastCollision;

    IGUIStaticText *m_pInfo;
    f32 m_fSteer,m_fActSteer;

    CIrrOdeMotor *m_pMotor[2];
    CIrrOdeServo *m_pServo[2];
    CIrrOdeGeomSphere *m_pWheelGeoms[4];
    CIrrOdeBody *m_pCarBody;

    frontWheelContact *m_pFwc;
    rearWheelContact  *m_pRwc;

    ICameraSceneNode *m_pCam;

    int getSteer();
    int getAcc  ();

    bool getBrake();
    bool getBoost();

    bool getFlip();

    int paramSwitch();

    bool paramInc   ();
    bool paramDec   ();
    bool adaptSteer ();

    bool getHelp();

  public:
    CCar(IrrlichtDevice *pDevice);
    virtual ~CCar();

    virtual void activate();
    virtual void deactivate();
    virtual u32 update();

    virtual bool OnEvent(const SEvent &event);
};

#endif
