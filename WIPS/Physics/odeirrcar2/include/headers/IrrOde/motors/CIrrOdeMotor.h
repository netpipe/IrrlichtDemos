#ifndef _C_IRR_ODE_MOTOR
  #define _C_IRR_ODE_MOTOR

  #include <motors/IIrrOdeMotor.h>

namespace irr {
namespace ode {

const int IRR_ODE_MOTOR_ID=MAKE_IRR_ID('i','o','m','o');
const wchar_t IRR_ODE_MOTOR_NAME[0xFF]=L"CIrrOdeMotor";

class CIrrOdeJoint;

/**
 * @class CIrrOdeMotor
 * This class can be used as linear or angular motor. It has to be child of the joint it will affect
 */
class CIrrOdeMotor : public IIrrOdeMotor {
  protected:
    CIrrOdeJoint *m_pJoint;   /**< the joint the motor is attached to */
    u32 m_iAxis;              /**< the axis the motor affects */
    f32 m_fVelocity;          /**< the velocity the motor tries to reach */
    f32 m_fForce;             /**< the power the motor uses when trying to reach the velocity */
    bool m_bEnabled;          /**< turn the motor on and off */

  public:
    /**
     * Standard Irrlicht scenenode constructor
     */
    CIrrOdeMotor(ISceneNode *parent,ISceneManager *mgr,s32 id = -1,
                 const vector3df &position=core::vector3df(0,0,0),
		             const vector3df &rotation = core::vector3df(0,0,0),
		             const vector3df &scale = core::vector3df(1.0f, 1.0f, 1.0f));

    ~CIrrOdeMotor();

    void setAxis(int iAxis);        /**< set the axis of the motor */
    void setVelocity(f32 fVel);     /**< set the velocity of the motor */
    void setForce(f32 fForce);      /**< set the force of the motor */
    void setEnabled(bool b);        /**< turn the motor on and off  */

    u32 getAxis();     /**< get the axis of the motor */
    f32 getVelocity(); /**< get the velocity of the motor */
    f32 getForce();    /**< get the force of the motor */
    bool isEnabled();  /**< is the motor on or off? */

    void setJoint(CIrrOdeJoint *pJoint);  /**< attach the motor to a joint */
    CIrrOdeJoint *getJoint();             /**< get the motor's joint       */

		/**
		 * the "setPosition" method is overridden to make sure the IrrOdeGeom nodes always have a relative position of (0,0,0)
		 * @param newpos the new position. Will be discarded and set to (0,0,0)
		 */
		virtual void setPosition(const vector3df &newpos);

		virtual void render();
    virtual void OnRegisterSceneNode();
    virtual void initPhysics();

    virtual s32 getID() const;
    virtual ESCENE_NODE_TYPE getType() const;
    virtual const wchar_t *getTypeName();

    virtual ISceneNode *clone(ISceneNode* newParent=0, ISceneManager* newManager=0);
    virtual void copyParams(CIrrOdeSceneNode *pDest, bool bRecurse=true);

    virtual void serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options) const;
    virtual void deserializeAttributes(io::IAttributes* in, io::SAttributeReadWriteOptions* options);
    virtual void removeFromPhysics() { CIrrOdeSceneNode::removeFromPhysics(); }
};

} //namespace ode
} //namespace irr

#endif
