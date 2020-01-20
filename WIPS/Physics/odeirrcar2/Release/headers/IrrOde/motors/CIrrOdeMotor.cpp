  #include <motors/CIrrOdeMotor.h>
  #include <joints/CIrrOdeJoint.h>

namespace irr {
namespace ode {

CIrrOdeMotor::CIrrOdeMotor(ISceneNode *parent,ISceneManager *mgr,s32 id,
                           const vector3df &position,
		                       const vector3df &rotation,
          		             const vector3df &scale) : IIrrOdeMotor(parent, mgr, id, position, rotation, scale) {

  m_iAxis=0;
  m_fVelocity=0.0f;
  m_fForce=0.0f;
  m_pJoint=NULL;

  #ifdef _IRREDIT_PLUGIN
    if (m_pMesh) {
	  c8 sFileName[1024];
	  sprintf(sFileName,"%sIrrOdeMotor.png",m_sResources);
      m_cMat.setTexture(0,m_pSceneManager->getVideoDriver()->getTexture(sFileName));
    }
  #endif
}

CIrrOdeMotor::~CIrrOdeMotor() {
}

void CIrrOdeMotor::setAxis(int iAxis) {
  m_iAxis=iAxis;
}

void CIrrOdeMotor::setVelocity(f32 fVel) {
  m_fVelocity=fVel;
  if (m_bPhysicsInitialized && m_pJoint && m_bEnabled) {
    m_pJoint->setParameter(m_iAxis,CIrrOdeJoint::eVel,fVel);
    m_pJoint->activateBodies();
  }
}

void CIrrOdeMotor::setForce(f32 fForce) {
  m_fForce=fForce;
  if (m_bPhysicsInitialized && m_pJoint && m_bEnabled) {
    m_pJoint->setParameter(m_iAxis,CIrrOdeJoint::eMaxForce,fForce);
    m_pJoint->activateBodies();
  }
}

void CIrrOdeMotor::setEnabled(bool b) {
  m_bEnabled=b;
  if (m_bPhysicsInitialized && b) {
    m_pJoint->setParameter(m_iAxis,CIrrOdeJoint::eVel,m_fVelocity);
    m_pJoint->setParameter(m_iAxis,CIrrOdeJoint::eMaxForce,m_fForce);
    m_pJoint->activateBodies();
  }
}

u32 CIrrOdeMotor::getAxis() {
  return m_iAxis;
}

f32 CIrrOdeMotor::getVelocity() {
  return m_fVelocity;
}

f32 CIrrOdeMotor::getForce() {
  return m_fForce;
}

bool CIrrOdeMotor::isEnabled() {
  return m_bEnabled;
}

void CIrrOdeMotor::setJoint(CIrrOdeJoint *pJoint) {
  m_pJoint=pJoint;
}

CIrrOdeJoint *CIrrOdeMotor::getJoint() {
  return m_pJoint;
}

void CIrrOdeMotor::serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options) const {
  CIrrOdeSceneNode::serializeAttributes(out,options);

  out->addInt("Axis",m_iAxis);
  out->addFloat("Force",m_fForce);
  out->addFloat("Velocity",m_fVelocity);
  out->addBool("Enabled",m_bEnabled);
}

void CIrrOdeMotor::deserializeAttributes(io::IAttributes* in, io::SAttributeReadWriteOptions* options) {
  CIrrOdeSceneNode::deserializeAttributes(in,options);

  m_iAxis=in->getAttributeAsInt("Axis");
  m_fForce=in->getAttributeAsFloat("Force");
  m_fVelocity=in->getAttributeAsFloat("Velocity");
  m_bEnabled=in->getAttributeAsBool("Enabled");
}

void CIrrOdeMotor::render() {
  CIrrOdeSceneNode::render();
  #ifdef _DRAW_BOUNDING_BOXES
    m_pVideoDriver->setTransform(video::ETS_WORLD,AbsoluteTransformation);
    m_pVideoDriver->setMaterial(m_cMat);
    m_pVideoDriver->draw3DBox(m_cBoundingBox,SColor(100,0x40,0x40,0x40));
  #endif
}

void CIrrOdeMotor::OnRegisterSceneNode() {
  if (IsVisible) SceneManager->registerNodeForRendering(this);
  ISceneNode::OnRegisterSceneNode();
}

void CIrrOdeMotor::initPhysics() {
  if (m_bPhysicsInitialized) return;
  CIrrOdeSceneNode::initPhysics();
  #ifdef _TRACE_INIT_PHYSICS
    printf("CIrrOdeMotor::initPhysics (%i .. %.2f .. %.2f .. %s\n",m_iAxis,m_fVelocity,m_fForce,m_bEnabled?"enabled":"disabled");
  #endif

  #ifdef _DRAW_BOUNDING_BOXES
    //m_pBoundingBox=&(getParent()->getBoundingBox());
  #endif

  if (m_bEnabled) {
    m_pJoint->setParameter(m_iAxis,CIrrOdeJoint::eVel,m_fVelocity);
    m_pJoint->setParameter(m_iAxis,CIrrOdeJoint::eMaxForce,m_fForce);
  }
}

s32 CIrrOdeMotor::getID() const {
  return ID;
}

ESCENE_NODE_TYPE CIrrOdeMotor::getType() const {
  return (ESCENE_NODE_TYPE)IRR_ODE_MOTOR_ID;
}

const wchar_t *CIrrOdeMotor::getTypeName() {
  return IRR_ODE_MOTOR_NAME;
}

ISceneNode *CIrrOdeMotor::clone(ISceneNode* newParent, ISceneManager* newManager) {
	CIrrOdeMotor *pRet=new CIrrOdeMotor(newParent?newParent:getParent(),newManager?newManager:m_pSceneManager);
  copyParams(pRet);
  CIrrOdeSceneNode::cloneChildren(pRet,newManager);
  return pRet;
}

void CIrrOdeMotor::copyParams(CIrrOdeSceneNode *pDest, bool bRecurse) {
  if (bRecurse) CIrrOdeSceneNode::copyParams(pDest);
  pDest->setName(getName());
  CIrrOdeMotor *pDst=(CIrrOdeMotor *)pDest;
  pDst->setAxis(m_iAxis);
  pDst->setVelocity(m_fVelocity);
  pDst->setForce(m_fForce);
  pDst->setEnabled(m_bEnabled);
}

void CIrrOdeMotor::setPosition(const vector3df &newpos) {
	ISceneNode::setPosition(vector3df(0.0f,0.0f,0.0f));
}

} //namespace ode
} //namespace irr
