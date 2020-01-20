#ifndef _C_IRR_ODE_AERO_DRAG
  #define _C_IRR_ODE_AERO_DRAG

  #include <motors/IIrrOdeStepMotor.h>

namespace irr {
  namespace ode {

const int IRR_ODE_AERO_DRAG_ID=MAKE_IRR_ID('i','o','a','d');
const wchar_t IRR_ODE_AERO_DRAG_NAME[0xFF]=L"CIrrOdeAeroDrag";
/**
 * @class CIrrOdeAeroDrag
 * This class "simulates" all forces that are applied to an aircraft during flight. It's not even close
 * to reality, but imho it's fun.
 */
class CIrrOdeAeroDrag : public IIrrOdeStepMotor {
  protected:
    vector3df m_vForeward,  /*!< the foreward vector */
              m_vSideward,  /*!< the sideward vector */
              m_vUpward;    /*!< the upward vector */

    f32 m_fStallSpeed,    /*!< If the airplane is below this foreward velocity no up force is applied */
        m_fMaxUpSpeed,    /*!< This is the foreward speed where the highest up force is reached */
        m_fUpFactor,      /*!< uplift factor */
        m_fForewardDamp,  /*!< This is the foreward damping applied */
        m_fSidewardDamp,  /*!< The sideward damping applied */
        m_fUpwardDamp,    /*!< The damping applied from the angle of attack */
        m_fForewardVel;   /*!< This is the calculated foreward velocity. This value can't be set but queried */

  public:
    CIrrOdeAeroDrag(ISceneNode *parent,ISceneManager *mgr,s32 id = -1,
                    const vector3df &position=core::vector3df(0,0,0),
                    const vector3df &rotation = core::vector3df(0,0,0),
                    const vector3df &scale = core::vector3df(1.0f, 1.0f, 1.0f));
    virtual void step();
    virtual const wchar_t *getTypeName();

    virtual ESCENE_NODE_TYPE getType() const { return (ESCENE_NODE_TYPE)IRR_ODE_AERO_DRAG_ID; }

    void setForeward(vector3df v) { m_vForeward=v; }
    void setSideward(vector3df v) { m_vSideward=v; }
    void setUpward(vector3df v) { m_vUpward=v; }

    vector3df getForeward() { return m_vForeward; }
    vector3df getSideward() { return m_vSideward; }
    vector3df getUpward() { return m_vUpward; }

    void setStallSpeed(f32 f) { m_fStallSpeed=f; }
    void setMaxUpSpeed(f32 f) { m_fMaxUpSpeed=f; }
    void setForewardDamp(f32 f) { m_fForewardDamp=f; }
    void setSidewardDamp(f32 f) { m_fSidewardDamp=f; }
    void setUpwardDamp(f32 f) { m_fUpwardDamp=f; }
    void setUpFactor(f32 f) { m_fUpFactor=f; }

    f32 getStallSpeed() { return m_fStallSpeed; }
    f32 getMaxUpSpeed() { return m_fMaxUpSpeed; }
    f32 getForewardDamp() { return m_fForewardDamp; }
    f32 getSidewardDamp() { return m_fSidewardDamp; }
    f32 getUpwardDamp() { return m_fUpwardDamp; }
    f32 getForewardVel() { return m_fForewardVel; }
    f32 getUpFactor() { return m_fUpFactor; }

    virtual void serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options) const;
    virtual void deserializeAttributes(io::IAttributes* in, io::SAttributeReadWriteOptions* options);
    virtual ISceneNode *clone(ISceneNode* newParent=0, ISceneManager* newManager=0);
};

  } //namespace ode
}   //namespace irr
#endif

