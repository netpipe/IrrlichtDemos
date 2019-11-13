#ifndef _C_HELICOPTER
  #define _C_HELICOPTER

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

class CHeli : public CVehicle, public IEventReceiver, public IIrrOdeEventListener {
  protected:
    s32 m_iYaw,m_iPitch,m_iRoll,m_iThrust;
    f32 m_fThrust;
    bool m_bLeft,m_bMissileCam;

    IGUIStaticText *m_pInfo;

    CIrrOdeWorld *m_pWorld;
    CIrrOdeBody *m_pHeliBody;

    ICameraSceneNode *m_pCam;
    ITerrainSceneNode *m_pTerrain;

  public:
    CHeli(IrrlichtDevice *pDevice);
    virtual ~CHeli();

    virtual void activate();
    virtual void deactivate();
    virtual u32 update();

    virtual bool OnEvent(const SEvent &event);

    virtual bool onEvent(IIrrOdeEvent *pEvent);
    virtual bool handlesEvent(IIrrOdeEvent *pEvent);
};

#endif

