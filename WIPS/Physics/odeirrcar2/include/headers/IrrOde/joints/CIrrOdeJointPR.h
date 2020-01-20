#ifndef _IRR_ODE_JOINT_PR
  #define _IRR_ODE_JOINT_PR

  #include <joints/CIrrOdeJoint.h>

namespace irr {
namespace ode {

const int IRR_ODE_JOINT_PR_ID=MAKE_IRR_ID('i','j','p','r');
const wchar_t IRR_ODE_JOINT_PR_NAME[0xFF]=L"CIrrOdeJointPR";

/**
 * @class CIrrOdeJointHinge
 * This class represents ODE's hinge joint.
 */
class CIrrOdeJointPR : public CIrrOdeJoint {
  protected:
    vector3df m_vAxis1,  /**< the joint's axis 1 */
              m_vAxis2;  /**< the joint's axis 2 */

  public:
    /** standard constructor */
    CIrrOdeJointPR(ISceneNode *parent,ISceneManager *mgr,s32 id = -1,
                      const vector3df &position=core::vector3df(0,0,0),
		                  const vector3df &rotation = core::vector3df(0,0,0),
		                  const vector3df &scale = core::vector3df(1.0f, 1.0f, 1.0f));

    virtual ~CIrrOdeJointPR();

    virtual void render();       /**< render the scene node */
    virtual void OnRegisterSceneNode();
    virtual void initPhysics();  /**< init physics          */

    virtual s32 getID() const;                 /**< get the node's ID */
    virtual ESCENE_NODE_TYPE getType() const;  /**< get the node's type */
    virtual const wchar_t *getTypeName();      /**< get the node's type name */

    void setPrAxis1(vector3df pAxis); /**< set the node's hinge axis */
    vector3df getPrAxis1();           /**< get the node's hinge axis */

    void setPrAxis2(vector3df pAxis); /**< set the node's hinge axis */
    vector3df getPrAxis2();           /**< get the node's hinge axis */

    f32 getPrPosition();
    f32 getPrAngle();     /**< get the joint's hinge angle */
    f32 getPrAngleRate(); /**< get the joint's hinge angle rate */

    virtual u16 numParamGroups() const;

    virtual void setParameter(u16 iGroup, eJointParameter iParam, f32 fValue);
    virtual f32 getParameter(u16 iGroup, eJointParameter iParam);

    virtual ISceneNode *clone(ISceneNode* newParent=0, ISceneManager* newManager=0);
    virtual void copyParams(CIrrOdeSceneNode *pDest, bool bRecurse=true);

    virtual void serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options) const;
    virtual void deserializeAttributes(io::IAttributes* in, io::SAttributeReadWriteOptions* options);
};

} //namespace ode
} //namespace irr

#endif

