  #include <joints/CIrrOdeJointBallSocket.h>
  #include <CIrrOdeWorld.h>
  #include <IIrrOdeDevice.h>

namespace irr {
namespace ode {

CIrrOdeJointBallSocket::CIrrOdeJointBallSocket(ISceneNode *parent,ISceneManager *mgr,s32 id,
                                              const vector3df &position,const vector3df &rotation,const vector3df &scale) :
                                              CIrrOdeJoint(parent, mgr, id, position, rotation, scale) {
  m_pAnchor1=vector3df(0,0,0);
  m_pAnchor2=vector3df(0,0,0);
  #ifdef _IRREDIT_PLUGIN
    if (m_pMesh) {
	  c8 sFileName[1024];
	  sprintf(sFileName,"%sIrrOdeJointBallSocket.png",m_sResources);
      m_cMat.setTexture(0,m_pSceneManager->getVideoDriver()->getTexture(sFileName));
    }
  #endif
}


CIrrOdeJointBallSocket::~CIrrOdeJointBallSocket() {
}

void CIrrOdeJointBallSocket::OnRegisterSceneNode() {
  if (IsVisible) SceneManager->registerNodeForRendering(this);
  ISceneNode::OnRegisterSceneNode();
}

void CIrrOdeJointBallSocket::render() {
  CIrrOdeSceneNode::render();
  #ifdef _DRAW_JOINT_INFO
    m_pVideoDriver->setMaterial(m_cMat);
    vector3df pos1=m_pBody1?m_pBody1->getAbsolutePosition():getAbsolutePosition();
    vector3df pos2=m_pBody2?m_pBody2->getAbsolutePosition():getAbsolutePosition();
    m_pVideoDriver->setTransform(video::ETS_WORLD, core::matrix4());
    m_pVideoDriver->draw3DLine(pos1,getAbsolutePosition(),SColor(128,0,0xFF,0));
    m_pVideoDriver->setTransform(video::ETS_WORLD, core::matrix4());
    m_pVideoDriver->draw3DLine(pos2,getAbsolutePosition(),SColor(128,0,0,0xFF));
  #endif
}

void CIrrOdeJointBallSocket::initPhysics() {
  if (m_bPhysicsInitialized) return;
  CIrrOdeJoint::initPhysics();

  updateAbsolutePosition();
  vector3df pos=getAbsolutePosition();

  m_iJointId=m_pOdeDevice->jointCreateBallSocket(m_pWorld->getWorldId());

  if (getParent()->getID()==IRR_ODE_BODY_ID)
    setBody1(reinterpret_cast<CIrrOdeBody *>(getParent()));

  m_pOdeDevice->jointAttach(m_iJointId,m_pBody1,m_pBody2);
  m_pOdeDevice->jointSetBallAnchor(m_iJointId,pos);
  m_pOdeDevice->jointSetData(m_iJointId,this);

  #ifdef _TRACE_INIT_PHYSICS
    printf("CIrrOdeJointBallSocket::initPhysics: anchor=%.2f, %.2f, %.2f\n",pos.X,pos.Y,pos.Z);
  #endif
}

s32 CIrrOdeJointBallSocket::getID() const {
  return ID;
}

ESCENE_NODE_TYPE CIrrOdeJointBallSocket::getType() const {
  return (ESCENE_NODE_TYPE)IRR_ODE_JOINT_BALL_SOCKET_ID;
}

const wchar_t *CIrrOdeJointBallSocket::getTypeName() {
  return IRR_ODE_JOINT_BALL_SOCKET_NAME;
}

vector3df CIrrOdeJointBallSocket::getAnchor1() {
  if (m_iJointId) m_pAnchor1=m_pOdeDevice->jointGetBallAnchor(m_iJointId);
  return m_pAnchor1;
}

vector3df CIrrOdeJointBallSocket::getAnchor2() {
  if (m_iJointId) m_pAnchor2=m_pOdeDevice->jointGetBallAnchor2(m_iJointId);
  return m_pAnchor2;
}

void CIrrOdeJointBallSocket::serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options) const {
  CIrrOdeJoint::serializeAttributes(out,options);
}

void CIrrOdeJointBallSocket::deserializeAttributes(IAttributes* in, SAttributeReadWriteOptions* options) {
  CIrrOdeJoint::deserializeAttributes(in,options);
}

ISceneNode *CIrrOdeJointBallSocket::clone(ISceneNode* newParent, ISceneManager* newManager) {
  CIrrOdeJointBallSocket *pRet=new CIrrOdeJointBallSocket(newParent?newParent:getParent(),newManager?newManager:m_pSceneManager);
  copyParams(pRet);
  CIrrOdeSceneNode::cloneChildren(pRet,newManager);
  return pRet;
}

void CIrrOdeJointBallSocket::copyParams(CIrrOdeSceneNode *pDest, bool bRecurse) {
  if (bRecurse) CIrrOdeJoint::copyParams(pDest);
}

u16 CIrrOdeJointBallSocket::numParamGroups() const {
  return 0;
}

} //namespace ode
} //namespace irr
