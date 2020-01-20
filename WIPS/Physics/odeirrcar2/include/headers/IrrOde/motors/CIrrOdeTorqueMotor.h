#ifndef _C_IRR_ODE_TORQUE
  #define _C_IRR_ODE_TORQUE

  #include <motors/IIrrOdeStepMotor.h>

namespace irr {
  namespace ode {

const int IRR_ODE_TORQUE_MOTOR_ID=MAKE_IRR_ID('i','o','t','q');
const wchar_t IRR_ODE_TORQUE_MOTOR_NAME[0xFF]=L"CIrrOdeTorqueMotor";
/**
 * @class CIrrOdeTorqueMotor
 * This class can be used to simulate the rudders of a plane or controls of a helicopter. The member
 * "m_fMaxPower" of IIrrOdeStepMotor is used as a factor for the rudder whereas "m_fPower" can e.g.
 * be used to make the rudders more inefficient at higher altitudes.
 */
class CIrrOdeTorqueMotor : public IIrrOdeStepMotor {
  protected:
    vector3df m_vRollAxis,      /*!< the roll axis */
              m_vPitchAxis,     /*!< the pitch axis */
              m_vYawAxis;       /*!< the yaw axis */

    bool m_bAeroRudder;         /*!< does the foreward speed influence the rudders (e.g. plane) or not (e.g. helicopter) */

    f32 m_fMinVel,        /*!< below this foreward velocity the rudders have no effect */
        m_fMaxVel,        /*!< above this foreward velocity the rudders won't have more effect */
        m_fPitch,         /*!< the pitch rudder position (between -1.0 and 1.0) */
        m_fRoll,          /*!< the roll rudder position (between -1.0 and 1.0) */
        m_fYaw,           /*!< the yaw rudder position (between -1.0 and 1.0) */
        m_fForewardVel,   /*!< the foreward velocity. This attribute can't be set but queried */
        m_fPitchFactor,   /*!< factor for the pitch rotation */
        m_fYawFactor,     /*!< factor for the yaw fotation */
        m_fRollFactor;    /*!< factor for the roll rotation */

  public:
    CIrrOdeTorqueMotor(ISceneNode *parent,ISceneManager *mgr,s32 id = -1,
                      const vector3df &position=core::vector3df(0,0,0),
                      const vector3df &rotation = core::vector3df(0,0,0),
                      const vector3df &scale = core::vector3df(1.0f, 1.0f, 1.0f));
    virtual void step();
    virtual const wchar_t *getTypeName();

    virtual ESCENE_NODE_TYPE getType() const { return (ESCENE_NODE_TYPE)IRR_ODE_TORQUE_MOTOR_ID; }

    void setRollAxis(vector3df v) { m_vRollAxis=v; }
    void setPitchAxis(vector3df v) { m_vPitchAxis=v; }
    void setYawAxis(vector3df v) { m_vYawAxis=v; }

    void setAeroRudder(bool b) { m_bAeroRudder=b; }

    void setMinVel(f32 f) { m_fMinVel=f; }
    void setMaxVel(f32 f) { m_fMaxVel=f; }

    void setPitch(f32 f) { m_fPitch=f; }
    void setRoll(f32 f) { m_fRoll=f; }
    void setYaw(f32 f) { m_fYaw=f; }

    vector3df getRollAxis() { return m_vRollAxis; }
    vector3df getPitchAxis() { return m_vPitchAxis; }
    vector3df getYawAxis() { return m_vYawAxis; }

    bool isAeroRudder() { return m_bAeroRudder; }

    f32 getMinVel() { return m_fMinVel; }
    f32 getMaxVel() { return m_fMaxVel; }

    f32 getPitch() { return m_fPitch; }
    f32 getRoll() { return m_fRoll; }
    f32 getYaw() { return m_fYaw; }

    f32 getForewardVel() { return m_fForewardVel; }

    virtual void serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options) const;
    virtual void deserializeAttributes(io::IAttributes* in, io::SAttributeReadWriteOptions* options);
    virtual ISceneNode *clone(ISceneNode* newParent=0, ISceneManager* newManager=0);

    /**
     * Set the factors for the torque forces
     * @param fPitch the pitch factpr
     * @param fYaw the yaw factor
     * @param fRoll the roll factor
     */
    void setFactors(f32 fPitch, f32 fYaw, f32 fRoll);
};

  } //namespace ode
}   //namespace irr
#endif

