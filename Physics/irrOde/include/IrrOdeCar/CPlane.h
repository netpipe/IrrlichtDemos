#ifndef _C_PLANE
  #define _C_PLANE

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

class CPlane : public CVehicle, public IEventReceiver, public IIrrOdeEventListener {
  protected:
    s32 m_iThrust,m_iYaw,m_iPitch,m_iRoll,m_iThrustDir;
    f32 m_fForewardVel;
    bool m_bFollowBombCam,
         m_bBackView;

    IGUIStaticText *m_pInfo;

    CIrrOdeWorld *m_pWorld;
    CIrrOdeBody *m_pPlaneBody;

    ICameraSceneNode *m_pCam;
    ITerrainSceneNode *m_pTerrain;

    CIrrOdeServo *m_pSteer;
    CIrrOdeBody *m_pRearWheels[2];

    ISceneNode *m_pRoll[2],*m_pPitch[2],*m_pYaw;

  public:
    CPlane(IrrlichtDevice *pDevice);
    virtual ~CPlane();

    virtual void activate();
    virtual void deactivate();
    virtual u32 update();

    virtual bool OnEvent(const SEvent &event);

    virtual bool onEvent(IIrrOdeEvent *pEvent);
    virtual bool handlesEvent(IIrrOdeEvent *pEvent);
};

#endif


