#ifndef _IRR_ODE_SERVO
  #define _IRR_ODE_SERVO

 #include <motors/IIrrOdeMotor.h>

namespace irr {
namespace ode {

const int IRR_ODE_SERVO_ID=MAKE_IRR_ID('i','o','s','v');
class CIrrOdeJoint;

const wchar_t IRR_ODE_SERVO_NAME[0xFF]=L"CIrrOdeServo";

/**
 * @class CIrrOdeServo
 * the CIrrOdeServo class can be used to set a joint to a desired position, e.g. for steering of a car
 */
class CIrrOdeServo : public IIrrOdeMotor {
  protected:
    CIrrOdeJoint *m_pJoint;   /**< the joint the servo is attached to */
    u32 m_iAxis;              /**< the axis the servo affects */
    f32 m_fServoPos;          /**< the position of the servo */

  public:
    /**
     * Standard Irrlicht scenenode constructor
     */
    CIrrOdeServo(ISceneNode *parent,ISceneManager *mgr,s32 id = -1,
                 const vector3df &position=core::vector3df(0,0,0),
		             const vector3df &rotation = core::vector3df(0,0,0),
		             const vector3df &scale = core::vector3df(1.0f, 1.0f, 1.0f));

    ~CIrrOdeServo();

    void setAxis(int iAxis);        /**< set the axis of the servo */
    void setServoPos(f32 fPos);     /**< set the position of the servo */

    u32 getAxis();     /**< get the axis of the servo */
    f32 getServoPos(); /**< get the position of the servo */

    void setJoint(CIrrOdeJoint *pJoint);  /**< attach the servo to a joint */
    CIrrOdeJoint *getJoint();             /**< get the servo's joint       */

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
