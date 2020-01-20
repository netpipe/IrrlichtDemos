#ifndef _IRR_ODE_JOINT_HINGE
  #define _IRR_ODE_JOINT_HINGE

  #include <joints/CIrrOdeJoint.h>

namespace irr {
namespace ode {

const int IRR_ODE_JOINT_HINGE_ID=MAKE_IRR_ID('i','o','j','h');
const wchar_t IRR_ODE_JOINT_HINGE_NAME[0xFF]=L"CIrrOdeJointHinge";

/**
 * @class CIrrOdeJointHinge
 * This class represents ODE's hinge joint.
 */
class CIrrOdeJointHinge : public CIrrOdeJoint {
  protected:
    vector3df m_pAxis,    /**< the joint's axis */
              m_pAnchor1, /**< the joint anchor on body 1 */
              m_pAnchor2; /**< the joint anchor on body 2 */

  public:
    /** standard constructor */
    CIrrOdeJointHinge(ISceneNode *parent,ISceneManager *mgr,s32 id = -1,
                      const vector3df &position=core::vector3df(0,0,0),
		                  const vector3df &rotation = core::vector3df(0,0,0),
		                  const vector3df &scale = core::vector3df(1.0f, 1.0f, 1.0f));

    virtual ~CIrrOdeJointHinge();

    virtual void render();       /**< render the scene node */
    virtual void OnRegisterSceneNode();
    virtual void initPhysics();  /**< init physics          */

    virtual s32 getID() const;                 /**< get the node's ID */
    virtual ESCENE_NODE_TYPE getType() const;  /**< get the node's type */
    virtual const wchar_t *getTypeName();      /**< get the node's type name */

    void setHingeAxis(vector3df pAxis); /**< set the node's hinge axis */
    vector3df getHingeAxis();           /**< get the node's hinge axis */

    vector3df getAnchor1();  /**< get the joint's anchor on body 1 */
    vector3df getAnchor2();  /**< get the joint's anchor on body 2 */

    f32 getHingeAngle();     /**< get the joint's hinge angle */
    f32 getHingeAngleRate(); /**< get the joint's hinge angle rate */

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
