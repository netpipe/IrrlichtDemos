#ifndef _IRR_ODE_JOINT
  #define _IRR_ODE_JOINT

  #include <CIrrOdeSceneNode.h>
  #include <CIrrOdeBody.h>

namespace irr {
namespace ode {

const wchar_t IRR_ODE_PARAM_NAMES[12][0xFF]= {
  L"HiStop", L"LoStop", L"Velocity", L"Max. Force", L"Fudge Factor", L"Bounce", L"CFM",
  L"Stop ERP", L"Stop CFM", L"Suspension ERP", L"Suspension CFM"
};

class CIrrOdeJoint : public CIrrOdeSceneNode {
  public:
    enum eJointParameter {
      eHiStop,
      eLoStop,
      eVel,
      eMaxForce,
      eFudge,
      eBounce,
      eCFM,
      eStopERP,
      eStopCFM,
      eSuspensionERP,
      eSuspensionCFM,
      eParamEnd
    };

  protected:
    u32 m_iJointId;

    CIrrOdeBody *m_pBody1,
                *m_pBody2;

    CIrrOdeWorld *m_pWorld;  /**< the IrrOde world the joint is in */

    f32 m_fParam[eParamEnd][3];
    bool m_bParamUsed[eParamEnd][3];

  public:
    CIrrOdeJoint(ISceneNode *parent,ISceneManager *mgr,s32 id = -1,
                 const vector3df &position=core::vector3df(0,0,0),
		             const vector3df &rotation = core::vector3df(0,0,0),
		             const vector3df &scale = core::vector3df(1.0f, 1.0f, 1.0f));

    virtual ~CIrrOdeJoint();

    void setBody1(CIrrOdeBody *pBody1);
    void setBody2(CIrrOdeBody *pBody2);

    CIrrOdeBody *getBody1();
    CIrrOdeBody *getBody2();

    void initLinkedObjects();

    virtual u16 numParamGroups() const =0;

    virtual f32 getParameter(u16 iGroup, eJointParameter iParam);
    virtual void setParameter(u16 iGroup, eJointParameter iParam, f32 fValue);
    virtual void setPosition(const vector3df &newPos);

    virtual void serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options) const;
    virtual void deserializeAttributes(io::IAttributes* in, io::SAttributeReadWriteOptions* options);

    virtual void copyParams(CIrrOdeSceneNode *pDest, bool bRecurse=true);
    virtual void removeFromPhysics();

    void activateBodies();

    virtual void initPhysics();
};

} //namespace ode
} //namespace irr

#endif
