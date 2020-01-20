#ifndef _IRR_ODE_EVENT_BODY_MOVED
  #define _IRR_ODE_EVENT_BODY_MOVED

  #include <irrlicht.h>
  #include <event/IIrrOdeEvent.h>
  #include <CIrrOdeBody.h>
  #include <geom/CIrrOdeGeom.h>

namespace irr {
namespace ode {

const u16 siIrrOdeEventPosition=1;
const u16 siIrrOdeEventRotation=2;
const u16 siIrrOdeEventLinearVel=4;
const u16 siIrrOdeEventAngularVel=8;
const u16 siIrrOdeEventBodyTouching=16;
const u16 siIrrOdeEventDamping=32;

/**
 * @class CIrrOdeEventBodyMoved
 * This event is posted whenever a body moved
 */
class CIrrOdeEventBodyMoved : public IIrrOdeEvent {
  protected:
    CIrrOdeBody *m_pBody;     /**< the body */
    CIrrOdeGeom *m_pTouch;    /**< the geom the body is touching */
    s32 m_iBodyId,            /**< the ID of the body */
        m_iTouchId,           /**< the ID of the geom the body is touching */
        m_iColMaterial;       /**< the material of the collision point */
    u16 m_iAngularDamping,
        m_iLinearDamping;
    u8 m_iFlags;              /**< flags */
    vector3df m_vNewPos,      /**< the new position */
              m_vNewRot,      /**< the new rotation */
              m_vNewLinVel,   /**< the new linear velocity */
              m_vNewAngVel,   /**< the new angular velocity */
              m_vColPoint;    /**< the collision point */

  public:
    CIrrOdeEventBodyMoved(CIrrOdeBody *pBody, vector3df &vPos, vector3df &vRot, vector3df &vLinVel, vector3df &vAngVel);
    CIrrOdeEventBodyMoved(CSerializer *pData, ISceneManager *pSmgr);
    CIrrOdeEventBodyMoved(IIrrOdeEvent *pIn);

    vector3df &getNewPosition() { return m_vNewPos; }
    vector3df &getNewRotation() { return m_vNewRot; }
    vector3df &getNewLinearVelocity() { return m_vNewLinVel; }
    vector3df &getNewAngularVelocity() { return m_vNewAngVel; }

    bool positionChanged() { return m_iFlags&siIrrOdeEventPosition; }
    bool rotationChanged() { return m_iFlags&siIrrOdeEventRotation; }
    bool linearVelChanged() { return m_iFlags&siIrrOdeEventLinearVel; }
    bool angularVelChanged() { return m_iFlags&siIrrOdeEventAngularVel; }
    bool isTouching() { return m_iFlags&siIrrOdeEventBodyTouching; }
    bool dampingChanged() { return m_iFlags&siIrrOdeEventDamping; }

    u8 getFlags() { return m_iFlags; }

    s32 getBodyId() { return m_iBodyId; }
    s32 getTouchId() { return m_iTouchId; }

    virtual u16 getType();

    CIrrOdeBody *getBody();
    void setBody(CIrrOdeBody *pBody);

    CIrrOdeGeom *getTouched();
    void setTouched(CIrrOdeGeom *pGeom);

    s32 getCollisionMaterial() { return m_iColMaterial; }
    void setCollisionMaterial(s32 i) { m_iColMaterial=i; }

    vector3df getCollisionPoint() { return m_vColPoint; }
    void setCollisionPoint(vector3df v) { m_vColPoint=v; }

    virtual CSerializer *serialize();

    void merge(CIrrOdeEventBodyMoved *pOther);
    virtual const c8 *toString();

    f32 getLinearDamping();
    f32 getAngularDamping();

    u16 getLinearDampingInt();
    u16 getAngularDampingInt();

    void setLinearDamping(f32 f);
    void setAngularDamping(f32 f);

    virtual IIrrOdeEvent *clone() { return new CIrrOdeEventBodyMoved(this); }

    virtual bool isObservable() { return true; }
};

} //namespace ode
} //namespace irr

#endif
