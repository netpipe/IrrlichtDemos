#ifndef _IRR_ODE_JOINT_UNIVERSAL
  #define _IRR_ODE_JOINT_UNIVERSAL

  #include <irrlicht.h>
  #include <joints/CIrrOdeJoint.h>

namespace irr {
namespace ode {

const int IRR_ODE_JOINT_UNIVERSAL_ID=MAKE_IRR_ID('i','o','j','u');
const wchar_t IRR_ODE_JOINT_UNIVERSAL_NAME[0xFF]=L"CIrrOdeJointUniversal";

class CIrrOdeJointUniversal : public CIrrOdeJoint {
  protected:
    vector3df m_pAnchor1,
              m_pAnchor2,
              m_pAxis1,
              m_pAxis2;

  public:
    CIrrOdeJointUniversal(ISceneNode *parent,ISceneManager *mgr,s32 id = -1,
                          const vector3df &position=core::vector3df(0,0,0),
		                      const vector3df &rotation = core::vector3df(0,0,0),
		                      const vector3df &scale = core::vector3df(1.0f, 1.0f, 1.0f));

    virtual ~CIrrOdeJointUniversal();

    virtual void render();
    virtual void OnRegisterSceneNode();
    virtual void initPhysics();

    virtual s32 getID() const;
    virtual ESCENE_NODE_TYPE getType() const;
    virtual const wchar_t *getTypeName();

    vector3df getAnchor1();
    vector3df getAnchor2();

    void setAxis1(vector3df pAxis1);
    void setAxis2(vector3df pAxis2);

    vector3df getAxis1();
    vector3df getAxis2();

    f32 getAngle1();
    f32 getAngle2();

    f32 getAngleRate1();
    f32 getAngleRate2();

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
