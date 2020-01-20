#ifndef _IRR_ODE_JOINT_BALL_SOCKET
  #define _IRR_ODE_JOINT_BALL_SOCKET

  #include <joints/CIrrOdeJoint.h>

namespace irr {
namespace ode {

const int IRR_ODE_JOINT_BALL_SOCKET_ID=MAKE_IRR_ID('i','o','j','b');
const wchar_t IRR_ODE_JOINT_BALL_SOCKET_NAME[0xFF]=L"CIrrOdeJointBallSocket";

class CIrrOdeJointBallSocket : public CIrrOdeJoint {
  protected:
    vector3df m_pAnchor1,
              m_pAnchor2;

  public:
    CIrrOdeJointBallSocket(ISceneNode *parent,ISceneManager *mgr,s32 id = -1,
                           const vector3df &position=core::vector3df(0,0,0),
		                       const vector3df &rotation = core::vector3df(0,0,0),
		                       const vector3df &scale = core::vector3df(1.0f, 1.0f, 1.0f));

    virtual ~CIrrOdeJointBallSocket();

    virtual void render();
    virtual void OnRegisterSceneNode();
    virtual void initPhysics();

    virtual s32 getID() const;
    virtual ESCENE_NODE_TYPE getType() const;
    virtual const wchar_t *getTypeName();

	  virtual u16 numParamGroups() const;

    vector3df getAnchor1();
    vector3df getAnchor2();

    virtual ISceneNode *clone(ISceneNode* newParent=0, ISceneManager* newManager=0);
    virtual void copyParams(CIrrOdeSceneNode *pDest, bool bRecurse=true);

    virtual void serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options) const;
    virtual void deserializeAttributes(io::IAttributes* in, io::SAttributeReadWriteOptions* options);
};

} //namespace ode
} //namespace irr

#endif
