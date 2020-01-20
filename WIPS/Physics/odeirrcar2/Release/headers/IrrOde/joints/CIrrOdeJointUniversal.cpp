  #include <joints/CIrrOdeJointUniversal.h>
  #include <CIrrOdeWorld.h>
  #include <IIrrOdeDevice.h>

namespace irr {
namespace ode {

CIrrOdeJointUniversal::CIrrOdeJointUniversal(ISceneNode *parent,ISceneManager *mgr,s32 id,
                                              const vector3df &position,const vector3df &rotation,const vector3df &scale) :
                                              CIrrOdeJoint(parent, mgr, id, position, rotation, scale) {
  m_pAnchor1=vector3df(0,0,0);
  m_pAnchor2=vector3df(0,0,0);
  #ifdef _IRREDIT_PLUGIN
    if (m_pMesh) {
	  c8 sFileName[1024];
	  sprintf(sFileName,"%sIrrOdeJointUniversal.png",m_sResources);
      m_cMat.setTexture(0,m_pSceneManager->getVideoDriver()->getTexture(sFileName));
    }
  #endif
}


CIrrOdeJointUniversal::~CIrrOdeJointUniversal() {
}

void CIrrOdeJointUniversal::OnRegisterSceneNode() {
  if (IsVisible) SceneManager->registerNodeForRendering(this);
  ISceneNode::OnRegisterSceneNode();
}

void CIrrOdeJointUniversal::render() {
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

void CIrrOdeJointUniversal::initPhysics() {
  if (m_bPhysicsInitialized) return;
  CIrrOdeJoint::initPhysics();

  vector3df pos=getAbsolutePosition();

  m_iJointId=m_pOdeDevice->jointCreateUniversal(m_pWorld->getWorldId());

  if (getParent()->getID()==IRR_ODE_BODY_ID)
    setBody1(reinterpret_cast<CIrrOdeBody *>(getParent()));

  vector3df rot=getAbsoluteTransformation().getRotationDegrees(),
            axis1=rot.rotationToDirection(m_pAxis1),
            axis2=rot.rotationToDirection(m_pAxis2);

  m_pOdeDevice->jointAttach(m_iJointId,m_pBody1,m_pBody2);
  m_pOdeDevice->jointSetUniversalAnchor(m_iJointId,getPosition());
  m_pOdeDevice->jointSetUniversalAxis1(m_iJointId,axis1);
  m_pOdeDevice->jointSetUniversalAxis2(m_iJointId,axis2);
  m_pOdeDevice->jointSetData(m_iJointId,this);

  #ifdef _TRACE_INIT_PHYSICS
    printf("CIrrOdeJointUniversal::initPhysics: anchor=%.2f, %.2f, %.2f\n",pos.X,pos.Y,pos.Z);
    printf("CIrrOdeJointUniversal::initPhysics: axis 1=%.2f, %.2f, %.2f\n",m_pAxis1.X,m_pAxis1.Y,m_pAxis1.Z);
    printf("CIrrOdeJointUniversal::initPhysics: axis 2=%.2f, %.2f, %.2f\n",m_pAxis2.X,m_pAxis2.Y,m_pAxis2.Z);
  #endif
}

s32 CIrrOdeJointUniversal::getID() const {
  return ID;
}

ESCENE_NODE_TYPE CIrrOdeJointUniversal::getType() const {
  return (ESCENE_NODE_TYPE)IRR_ODE_JOINT_UNIVERSAL_ID;
}

const wchar_t *CIrrOdeJointUniversal::getTypeName() {
  return IRR_ODE_JOINT_UNIVERSAL_NAME;
}

vector3df CIrrOdeJointUniversal::getAnchor1() {
  if (m_iJointId) m_pAnchor1=m_pOdeDevice->jointGetUniversalAnchor(m_iJointId);
  return m_pAnchor1;
}

vector3df CIrrOdeJointUniversal::getAnchor2() {
  if (m_iJointId) m_pAnchor2=m_pOdeDevice->jointGetUniversalAnchor2(m_iJointId);
  return m_pAnchor2;
}

void CIrrOdeJointUniversal::setAxis1(vector3df pAxis1) {
  m_pAxis1=pAxis1;
  if (m_iJointId) m_pOdeDevice->jointSetUniversalAxis1(m_iJointId,m_pAxis1);
}

void CIrrOdeJointUniversal::setAxis2(vector3df pAxis2) {
  m_pAxis2=pAxis2;
  if (m_iJointId) m_pOdeDevice->jointSetUniversalAxis2(m_iJointId,m_pAxis2);
}

vector3df CIrrOdeJointUniversal::getAxis1() {
  if (m_iJointId) m_pAxis1=m_pOdeDevice->jointGetUniversalAxis1(m_iJointId);
  return m_pAxis1;
}

vector3df CIrrOdeJointUniversal::getAxis2() {
  if (m_iJointId) m_pAxis2=m_pOdeDevice->jointGetUniversalAxis2(m_iJointId);
  return m_pAxis2;
}

f32 CIrrOdeJointUniversal::getAngle1() {
  return m_iJointId?m_pOdeDevice->jointGetUniversalAngle1(m_iJointId):0.0f;
}

f32 CIrrOdeJointUniversal::getAngle2() {
  return m_iJointId?m_pOdeDevice->jointGetUniversalAngle2(m_iJointId):0.0f;
}

f32 CIrrOdeJointUniversal::getAngleRate1() {
  return m_iJointId?m_pOdeDevice->jointGetUniversalAngle1Rate(m_iJointId):0.0f;
}

f32 CIrrOdeJointUniversal::getAngleRate2() {
  return m_iJointId?m_pOdeDevice->jointGetUniversalAngle2Rate(m_iJointId):0.0f;
}

u16 CIrrOdeJointUniversal::numParamGroups() const {
  return 2;
}

void CIrrOdeJointUniversal::setParameter(u16 iGroup, eJointParameter iParam, f32 fValue) {
  #ifdef _TRACE_INIT_PHYSICS
    printf("CIrrOdeJointUniversal::setParameter: %i .. %i .. %.2f\n",iGroup,iParam,fValue);
  #endif

  if (m_iJointId) m_pOdeDevice->jointSetUniversalParam(m_iJointId,iGroup,iParam,fValue);
}

f32 CIrrOdeJointUniversal::getParameter(u16 iGroup, eJointParameter iParam) {
  return m_iJointId?m_pOdeDevice->jointGetUniversalParam(m_iJointId,iGroup,iParam):0.0f;
}

void CIrrOdeJointUniversal::serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options) const {
  CIrrOdeJoint::serializeAttributes(out,options);
  out->addVector3d("Axis1",m_pAxis1);
  out->addVector3d("Axis2",m_pAxis2);
}

void CIrrOdeJointUniversal::deserializeAttributes(IAttributes* in, SAttributeReadWriteOptions* options) {
  CIrrOdeJoint::deserializeAttributes(in,options);
  m_pAxis1=in->getAttributeAsVector3d("Axis1");
  m_pAxis2=in->getAttributeAsVector3d("Axis2");
}

ISceneNode *CIrrOdeJointUniversal::clone(ISceneNode* newParent, ISceneManager* newManager) {
  CIrrOdeJointUniversal *pRet=new CIrrOdeJointUniversal(newParent?newParent:getParent(),newManager?newManager:m_pSceneManager);
  copyParams(pRet);
  CIrrOdeSceneNode::cloneChildren(pRet,newManager);
  return pRet;
}

void CIrrOdeJointUniversal::copyParams(CIrrOdeSceneNode *pDest, bool bRecurse) {
  if (bRecurse) CIrrOdeJoint::copyParams(pDest);
  CIrrOdeJointUniversal *pDst=(CIrrOdeJointUniversal *)pDest;
  pDst->setAxis1(m_pAxis1);
  pDst->setAxis2(m_pAxis2);
}

} //namespace ode
} //namespace irr
