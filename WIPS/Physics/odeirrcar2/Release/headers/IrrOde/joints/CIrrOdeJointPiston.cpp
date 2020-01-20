  #include <joints/CIrrOdeJointPiston.h>
  #include <CIrrOdeWorld.h>
  #include <IIrrOdeDevice.h>

namespace irr {
namespace ode {

CIrrOdeJointPiston::CIrrOdeJointPiston(ISceneNode *parent,ISceneManager *mgr,s32 id,
                                     const vector3df &position,const vector3df &rotation,const vector3df &scale) :
                                     CIrrOdeJoint(parent, mgr, id, position, rotation, scale) {
  m_pAxis=vector3df(0,0,0);
  #ifdef _IRREDIT_PLUGIN
    if (m_pMesh) {
	  c8 sFileName[1024];
	  sprintf(sFileName,"%sIrrOdeJointPiston.png",m_sResources);
      m_cMat.setTexture(0,m_pSceneManager->getVideoDriver()->getTexture(sFileName));
    }
  #endif
}


CIrrOdeJointPiston::~CIrrOdeJointPiston() {
}

void CIrrOdeJointPiston::OnRegisterSceneNode() {
  if (IsVisible) SceneManager->registerNodeForRendering(this);
  ISceneNode::OnRegisterSceneNode();
}

void CIrrOdeJointPiston::render() {
  CIrrOdeSceneNode::render();
  #ifdef _DRAW_JOINT_INFO
    vector3df axis=getAbsoluteTransformation().getRotationDegrees().rotationToDirection(m_pAxis);
    vector3df axisStart=getAbsolutePosition()-axis;
    vector3df axisEnd=axisStart+10*axis;
	m_pVideoDriver->setTransform(video::ETS_WORLD, core::matrix4());
    m_pVideoDriver->draw3DLine(axisStart,axisEnd,SColor(128,0xFF,0,0));

	m_pVideoDriver->setMaterial(m_cMat);
    vector3df pos1=m_pBody1?m_pBody1->getAbsolutePosition():getAbsolutePosition();
    vector3df pos2=m_pBody2?m_pBody2->getAbsolutePosition():getAbsolutePosition();
    m_pVideoDriver->setTransform(video::ETS_WORLD, core::matrix4());
    m_pVideoDriver->draw3DLine(pos1,getAbsolutePosition(),SColor(128,0,0xFF,0));
    m_pVideoDriver->draw3DLine(pos2,getAbsolutePosition(),SColor(128,0,0,0xFF));
  #endif
}

void CIrrOdeJointPiston::initPhysics() {
  if (m_bPhysicsInitialized) return;
  CIrrOdeJoint::initPhysics();
  updateAbsolutePosition();

  m_iJointId=m_pOdeDevice->jointCreatePiston(m_pWorld->getWorldId());

  if (getParent()->getID()==IRR_ODE_BODY_ID)
    setBody1(reinterpret_cast<CIrrOdeBody *>(getParent()));

  m_pOdeDevice->jointAttach(m_iJointId,m_pBody1,m_pBody2);
  if (m_pAxis==vector3df(0,0,0)) {
    m_pAxis.X=(m_pBody1?m_pBody1->getAbsolutePosition().X:getAbsolutePosition().X)-(m_pBody2?m_pBody2->getAbsolutePosition().X:getAbsolutePosition().X);
    m_pAxis.Y=(m_pBody1?m_pBody1->getAbsolutePosition().Y:getAbsolutePosition().Y)-(m_pBody2?m_pBody2->getAbsolutePosition().Y:getAbsolutePosition().Y);
    m_pAxis.Z=(m_pBody1?m_pBody1->getAbsolutePosition().Z:getAbsolutePosition().Z)-(m_pBody2?m_pBody2->getAbsolutePosition().Z:getAbsolutePosition().Z);
  }
  #ifdef _TRACE_INIT_PHYSICS
    printf("CIrrOdeJointPiston::initPhysics: axis=(%.2f, %.2f, %.2f)\n",m_pAxis.X,m_pAxis.Y,m_pAxis.Z);
  #endif

  vector3df rot=getAbsoluteTransformation().getRotationDegrees(),
            axis=rot.rotationToDirection(m_pAxis);

  m_pOdeDevice->jointSetPistonAnchor(m_iJointId,getAbsolutePosition());
  m_pOdeDevice->jointSetPistonAxis(m_iJointId,axis);
  m_pOdeDevice->jointSetData(m_iJointId,this);

  for (s32 i=0; i<eParamEnd; i++)
    if (m_bParamUsed[i][0]) {
      #ifdef _TRACE_INIT_PHYSICS
        printf("CIrrOdeJointSlider::initPhysics: parameter %i set: %.2f\n",i,m_fParam[i][0]);
      #endif
      setParameter(0,(eJointParameter)i,m_fParam[i][0]);
    }
}

s32 CIrrOdeJointPiston::getID() const {
  return ID;
}

ESCENE_NODE_TYPE CIrrOdeJointPiston::getType() const {
  return (ESCENE_NODE_TYPE)IRR_ODE_JOINT_PISTON_ID;
}

const wchar_t *CIrrOdeJointPiston::getTypeName() {
  return IRR_ODE_JOINT_PISTON_NAME;
}

void CIrrOdeJointPiston::setAxis(vector3df pAxis) {
  m_pAxis=pAxis;
  if (m_iJointId) m_pOdeDevice->jointSetPistonAxis(m_iJointId,m_pAxis);
}

vector3df CIrrOdeJointPiston::getAxis() {
  if (m_iJointId) m_pAxis=m_pOdeDevice->jointGetPistonAxis(m_iJointId);
  return m_pAxis;
}

void CIrrOdeJointPiston::setAnchor(vector3df pAnchor) {
  setPosition(pAnchor);
  if (m_iJointId) m_pOdeDevice->jointSetPistonAnchor(m_iJointId,getAbsolutePosition());
}

vector3df CIrrOdeJointPiston::getAnchor() {
  return getAbsolutePosition();
}

f32 CIrrOdeJointPiston::getPistonPosition() {
  return m_iJointId?m_pOdeDevice->jointGetPistonPosition(m_iJointId):0.0f;
}

f32 CIrrOdeJointPiston::getPistonPositionRate() {
  return m_iJointId?m_pOdeDevice->jointGetPistonPositionRate(m_iJointId):0.0f;
}

u16 CIrrOdeJointPiston::numParamGroups() const {
  return 1;
}

void CIrrOdeJointPiston::setParameter(u16 iGroup, eJointParameter iParam, f32 fValue) {
  #ifdef _TRACE_INIT_PHYSICS
    printf("CIrrOdeJointPiston::setParameter: %i .. %i .. %.2f\n",iGroup,iParam,fValue);
  #endif
  CIrrOdeJoint::setParameter(iGroup,iParam,fValue);

  if (m_iJointId) m_pOdeDevice->jointSetPistonParam(m_iJointId,iGroup,iParam,fValue);
}

f32 CIrrOdeJointPiston::getParameter(u16 iGroup, eJointParameter iParam) {
  return m_iJointId?m_pOdeDevice->jointGetPistonParam(m_iJointId,iGroup,iParam):0.0f;
}

void CIrrOdeJointPiston::serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options) const {
  CIrrOdeJoint::serializeAttributes(out,options);
  out->addVector3d("Axis",m_pAxis);
}

void CIrrOdeJointPiston::deserializeAttributes(IAttributes* in, SAttributeReadWriteOptions* options) {
  CIrrOdeJoint::deserializeAttributes(in,options);
  m_pAxis=in->getAttributeAsVector3d("Axis");
}

ISceneNode *CIrrOdeJointPiston::clone(ISceneNode* newParent, ISceneManager* newManager) {
  CIrrOdeJointPiston *pRet=new CIrrOdeJointPiston(newParent?newParent:getParent(),newManager?newManager:m_pSceneManager);
  copyParams(pRet);
  CIrrOdeSceneNode::cloneChildren(pRet,newManager);
  return pRet;
}

void CIrrOdeJointPiston::copyParams(CIrrOdeSceneNode *pDest, bool bRecurse) {
  if (bRecurse) CIrrOdeJoint::copyParams(pDest);
  CIrrOdeJointPiston *pDst=(CIrrOdeJointPiston *)pDest;
  pDst->setAxis(m_pAxis);
}

} //namespace ode
} //namespace irr
