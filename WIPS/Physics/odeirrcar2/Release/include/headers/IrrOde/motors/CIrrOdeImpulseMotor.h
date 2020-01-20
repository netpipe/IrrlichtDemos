#ifndef _C_IRR_ODE_IMPULSE_MOTOR
  #define _C_IRR_ODE_IMPULSE_MOTOR

  #include <motors/IIrrOdeStepMotor.h>

namespace irr {
  namespace ode {

const int IRR_ODE_IMPULSE_MOTOR_ID=MAKE_IRR_ID('i','o','m','i');
const wchar_t IRR_ODE_IMPULSE_MOTOR_NAME[0xFF]=L"CIrrOdeImpulseMotor";
/**
 * @class CIrrOdeImpulseMotor
 * This class adds a certain force to the parent body at each step. Must be direct child of the affected body.
 * Is useful e.g. for planes or missiles.
 */
class CIrrOdeImpulseMotor : public IIrrOdeStepMotor {
  protected:
    vector3df m_vForeward;

  public:
    CIrrOdeImpulseMotor(ISceneNode *parent,ISceneManager *mgr,s32 id = -1,
                        const vector3df &position=core::vector3df(0,0,0),
                        const vector3df &rotation = core::vector3df(0,0,0),
                        const vector3df &scale = core::vector3df(1.0f, 1.0f, 1.0f));
    virtual void step();
    virtual const wchar_t *getTypeName();

    virtual ESCENE_NODE_TYPE getType() const { return (ESCENE_NODE_TYPE)IRR_ODE_IMPULSE_MOTOR_ID; }

    void setForeward(vector3df v) { m_vForeward=v; }
    vector3df getForeward() { return m_vForeward; }

    virtual void serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options) const;
    virtual void deserializeAttributes(io::IAttributes* in, io::SAttributeReadWriteOptions* options);
    virtual ISceneNode *clone(ISceneNode* newParent=0, ISceneManager* newManager=0);
};

  } //namespace ode
}   //namespace irr
#endif
