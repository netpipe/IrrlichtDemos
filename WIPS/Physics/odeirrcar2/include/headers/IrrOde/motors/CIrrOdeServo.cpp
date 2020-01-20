  #include <motors/CIrrOdeServo.h>
  #include <joints/CIrrOdeJoint.h>
  #define _USE_MATH_DEFINES
  #include <math.h>
  #undef _USE_MATH_DEFINES

namespace irr {
namespace ode {

CIrrOdeServo::CIrrOdeServo(ISceneNode *parent,ISceneManager *mgr,s32 id,
                           const vector3df &position,
		                       const vector3df &rotation,
          		             const vector3df &scale) : IIrrOdeMotor(parent, mgr, id, position, rotation, scale) {

  m_iAxis=0;
  m_fServoPos=0;
  m_pJoint=NULL;

  #ifdef _IRREDIT_PLUGIN
    if (m_pMesh) {
	  c8 sFileName[1024];
	  sprintf(sFileName,"%sIrrOdeServo.png",m_sResources);
      m_cMat.setTexture(0,m_pSceneManager->getVideoDriver()->getTexture(sFileName));
    }
  #endif
}

CIrrOdeServo::~CIrrOdeServo() {
}

void CIrrOdeServo::setAxis(int iAxis) {
  m_iAxis=iAxis;
}

void CIrrOdeServo::setServoPos(f32 fPos) {
  while (fPos> 180) fPos-=180;
  while (fPos<-180) fPos+=180;
  //printf("setServoPos: %.2f -- %i\n",fPos,(int)m_bPhysicsInitialized);

  m_fServoPos=fPos;

  if (m_bPhysicsInitialized) {
    m_pJoint->setParameter(m_iAxis,CIrrOdeJoint::eLoStop,(f32)(m_fServoPos*M_PI/180));
    m_pJoint->setParameter(m_iAxis,CIrrOdeJoint::eHiStop,(f32)(m_fServoPos*M_PI/180));
    m_pJoint->setParameter(m_iAxis,CIrrOdeJoint::eLoStop,(f32)(m_fServoPos*M_PI/180));

    m_pJoint->activateBodies();
  }
}

u32 CIrrOdeServo::getAxis() {
  return m_iAxis;
}

f32 CIrrOdeServo::getServoPos() {
  return m_fServoPos;
}

void CIrrOdeServo::setJoint(CIrrOdeJoint *pJoint) {
  m_pJoint=pJoint;
}

CIrrOdeJoint *CIrrOdeServo::getJoint() {
  return m_pJoint;
}

void CIrrOdeServo::serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options) const {
  CIrrOdeSceneNode::serializeAttributes(out,options);

  out->addInt("Axis",m_iAxis);
  out->addFloat("ServoPos",m_fServoPos);
}

void CIrrOdeServo::deserializeAttributes(io::IAttributes* in, io::SAttributeReadWriteOptions* options) {
  CIrrOdeSceneNode::deserializeAttributes(in,options);

  m_iAxis=in->getAttributeAsInt("Axis");
  m_fServoPos=in->getAttributeAsFloat("ServoPos");
}

void CIrrOdeServo::render() {
  CIrrOdeSceneNode::render();
  #ifdef _DRAW_BOUNDING_BOXES
    m_pVideoDriver->setTransform(video::ETS_WORLD,AbsoluteTransformation);
    m_pVideoDriver->setMaterial(m_cMat);
    m_pVideoDriver->draw3DBox(m_cBoundingBox,SColor(100,0x40,0x40,0x40));
  #endif
}

void CIrrOdeServo::OnRegisterSceneNode() {
  if (IsVisible) SceneManager->registerNodeForRendering(this);
  ISceneNode::OnRegisterSceneNode();
}

void CIrrOdeServo::initPhysics() {
  if (m_bPhysicsInitialized) return;
  CIrrOdeSceneNode::initPhysics();
  #ifdef _TRACE_INIT_PHYSICS
    printf("CIrrOdeServo::initPhysics (%i .. %.2f)\n",m_iAxis,m_fServoPos);
  #endif

  #ifdef _DRAW_BOUNDING_BOXES
    //m_pBoundingBox=&(getParent()->getBoundingBox());
  #endif

  m_pJoint->setParameter(m_iAxis,CIrrOdeJoint::eLoStop,m_fServoPos);
  m_pJoint->setParameter(m_iAxis,CIrrOdeJoint::eHiStop,m_fServoPos);
  m_pJoint->setParameter(m_iAxis,CIrrOdeJoint::eLoStop,m_fServoPos);
}

s32 CIrrOdeServo::getID() const {
  return ID;
}

ESCENE_NODE_TYPE CIrrOdeServo::getType() const {
  return (ESCENE_NODE_TYPE)IRR_ODE_SERVO_ID;
}

const wchar_t *CIrrOdeServo::getTypeName() {
  return IRR_ODE_SERVO_NAME;
}

ISceneNode *CIrrOdeServo::clone(ISceneNode* newParent, ISceneManager* newManager) {
  CIrrOdeServo *pRet=new CIrrOdeServo(newParent?newParent:getParent(),newManager?newManager:m_pSceneManager);
  copyParams(pRet);
  CIrrOdeSceneNode::cloneChildren(pRet,newManager);
  return pRet;
}

void CIrrOdeServo::copyParams(CIrrOdeSceneNode *pDest, bool bRecurse) {
  if (bRecurse) CIrrOdeSceneNode::copyParams(pDest);
  pDest->setName(getName());
  CIrrOdeServo *pDst=(CIrrOdeServo *)pDest;
  pDst->setAxis(m_iAxis);
  pDst->setServoPos(m_fServoPos);
}

void CIrrOdeServo::setPosition(const vector3df &newpos) {
	ISceneNode::setPosition(vector3df(0.0f,0.0f,0.0f));
}

} //namespace ode
} //namespace irr
