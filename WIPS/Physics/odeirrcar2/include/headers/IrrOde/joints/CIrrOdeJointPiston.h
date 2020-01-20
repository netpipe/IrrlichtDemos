#ifndef _IRR_ODE_JOINT_PISTON
  #define _IRR_ODE_JOINT_PISTON

  #include <joints/CIrrOdeJoint.h>

namespace irr {
namespace ode {

const int IRR_ODE_JOINT_PISTON_ID=MAKE_IRR_ID('i','o','j','p');
const wchar_t IRR_ODE_JOINT_PISTON_NAME[0xFF]=L"CIrrOdeJointPiston";

class CIrrOdeJointPiston : public CIrrOdeJoint {
  protected:
    vector3df m_pAxis;

  public:
    CIrrOdeJointPiston(ISceneNode *parent,ISceneManager *mgr,s32 id = -1,
                       const vector3df &position=core::vector3df(0,0,0),
		                   const vector3df &rotation = core::vector3df(0,0,0),
		                   const vector3df &scale = core::vector3df(1.0f, 1.0f, 1.0f));

    virtual ~CIrrOdeJointPiston();

    virtual void render();
    virtual void OnRegisterSceneNode();
    virtual void initPhysics();

    virtual s32 getID() const;
    virtual ESCENE_NODE_TYPE getType() const;
    virtual const wchar_t *getTypeName();

    void setAxis(vector3df pAxis);
    vector3df getAxis();

    void setAnchor(vector3df pAnchor);
    vector3df getAnchor();

    f32 getPistonPosition();
    f32 getPistonPositionRate();

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
