#ifndef _IRR_ODE_JOINT_FIXED
  #define _IRR_ODE_JOINT_FIXED

  #include <joints/CIrrOdeJoint.h>

namespace irr {
namespace ode {

const int IRR_ODE_JOINT_FIXED_ID=MAKE_IRR_ID('i','o','j','f');
const wchar_t IRR_ODE_JOINT_FIXED_NAME[0xFF]=L"CIrrOdeJointFixed";

class CIrrOdeJointFixed : public CIrrOdeJoint {
  protected:

  public:
    CIrrOdeJointFixed(ISceneNode *parent,ISceneManager *mgr,s32 id = -1,
                      const vector3df &position=core::vector3df(0,0,0),
		                  const vector3df &rotation = core::vector3df(0,0,0),
		                  const vector3df &scale = core::vector3df(1.0f, 1.0f, 1.0f));

    virtual ~CIrrOdeJointFixed();

    virtual void render();
    virtual void OnRegisterSceneNode();
    virtual void initPhysics();

    virtual s32 getID() const;
    virtual ESCENE_NODE_TYPE getType() const;
    virtual const wchar_t *getTypeName();

  	virtual u16 numParamGroups() const;

    virtual ISceneNode *clone(ISceneNode* newParent=0, ISceneManager* newManager=0);
    virtual void copyParams(CIrrOdeSceneNode *pDest, bool bRecurse=true);

    virtual void serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options) const;
    virtual void deserializeAttributes(io::IAttributes* in, io::SAttributeReadWriteOptions* options);
};

} //namespace ode
} //namespace irr

#endif

