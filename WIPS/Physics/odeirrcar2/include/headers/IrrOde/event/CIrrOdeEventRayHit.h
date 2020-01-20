#ifndef _IRR_ODE_EVENT_RAY_HIT
  #define _IRR_ODE_EVENT_RAY_HIT

  #include <irrlicht.h>
  #include <event/IIrrOdeEvent.h>
  #include <CIrrOdeBody.h>
  #include <geom/CIrrOdeGeomRay.h>

namespace irr {
namespace ode {

/**
 * @class CIrrOdeEventRayHit
 * This event is posted whenever a ray hits a geom or a body
 */
class CIrrOdeEventRayHit : public IIrrOdeEvent {
  protected:
    CIrrOdeBody *m_pBody;
    CIrrOdeGeom *m_pGeom;
    CIrrOdeGeomRay *m_pRay;

    vector3df m_cPos;
    f32 m_fDistance;

  public:
    CIrrOdeEventRayHit(CIrrOdeBody *pBody, CIrrOdeGeom *pGeom, CIrrOdeGeomRay *pRay, vector3df pos, f32 fDistance);
    CIrrOdeEventRayHit(CSerializer *pData, ISceneManager *pSmgr);
    CIrrOdeEventRayHit(IIrrOdeEvent *pIn);

    virtual u16 getType();

    CIrrOdeBody *getBody();
    CIrrOdeGeom *getGeom();
    CIrrOdeGeomRay *getRay();
    vector3df getPosition();
    f32 getDistance();

    virtual CSerializer *serialize();
    virtual const c8 *toString();

    virtual IIrrOdeEvent *clone() { return new CIrrOdeEventRayHit(this); }
};

} //namespace ode
} //namespace irr

#endif
