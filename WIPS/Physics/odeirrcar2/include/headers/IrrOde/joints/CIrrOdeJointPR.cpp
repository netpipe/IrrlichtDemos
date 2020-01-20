  #include <joints/CIrrOdeJointPR.h>
  #include <CIrrOdeWorld.h>
  #include <IIrrOdeDevice.h>

namespace irr {
namespace ode {

CIrrOdeJointPR::CIrrOdeJointPR(ISceneNode *parent,ISceneManager *mgr,s32 id,
                                     const vector3df &position,const vector3df &rotation,const vector3df &scale) :
                                     CIrrOdeJoint(parent, mgr, id, position, rotation, scale) {
  m_vAxis1=vector3df(0,0,1);
  m_vAxis2=vector3df(0,1,0);
  #ifdef _IRREDIT_PLUGIN
    if (m_pMesh) {
	  c8 sFileName[1024];
	  sprintf(sFileName,"%sIrrOdeJointPR.png",m_sResources);
      m_cMat.setTexture(0,m_pSceneManager->getVideoDriver()->getTexture(sFileName));
    }
  #endif
}


CIrrOdeJointPR::~CIrrOdeJointPR() {
}

void CIrrOdeJointPR::OnRegisterSceneNode() {
  if (IsVisible) SceneManager->registerNodeForRendering(this);
  ISceneNode::OnRegisterSceneNode();
}

void CIrrOdeJointPR::render() {
  CIrrOdeSceneNode::render();
  #ifdef _DRAW_JOINT_INFO
    m_pVideoDriver->setMaterial(m_cMat);
    m_pVideoDriver->setTransform(video::ETS_WORLD, core::matrix4());

    vector3df axisStart=getAbsolutePosition()-1*getAbsoluteTransformation().getRotationDegrees().rotationToDirection(m_vAxis1);
    vector3df axisEnd=axisStart+10*getAbsoluteTransformation().getRotationDegrees().rotationToDirection(m_vAxis1);
    m_pVideoDriver->draw3DLine(axisStart,axisEnd,SColor(128,0xFF,0,0));

    axisStart=getAbsolutePosition()-1*getAbsoluteTransformation().getRotationDegrees().rotationToDirection(m_vAxis2);
    axisEnd=axisStart+10*getAbsoluteTransformation().getRotationDegrees().rotationToDirection(m_vAxis2);
    m_pVideoDriver->draw3DLine(axisStart,axisEnd,SColor(128,0,0xFF,0xFF));

    vector3df pos1=m_pBody1?m_pBody1->getAbsolutePosition():getAbsolutePosition();
    vector3df pos2=m_pBody2?m_pBody2->getAbsolutePosition():getAbsolutePosition();
    m_pVideoDriver->draw3DLine(pos1,getAbsolutePosition(),SColor(128,0,0xFF,0));
    m_pVideoDriver->setTransform(video::ETS_WORLD, core::matrix4());
    m_pVideoDriver->draw3DLine(pos2,getAbsolutePosition(),SColor(128,0,0,0xFF));
  #endif
}

void CIrrOdeJointPR::initPhysics() {
  if (m_bPhysicsInitialized) return;
  CIrrOdeJoint::initPhysics();

  //create the joint
  m_iJointId=m_pOdeDevice->jointCreatePR(m_pWorld->getWorldId());

  //set the parent as body 1
  if (getParent()->getID()==IRR_ODE_BODY_ID)
    setBody1(reinterpret_cast<CIrrOdeBody *>(getParent()));

  vector3df rot=getAbsoluteTransformation().getRotationDegrees(),
            axis1=rot.rotationToDirection(m_vAxis1),
            axis2=rot.rotationToDirection(m_vAxis2);

  m_pOdeDevice->jointAttach(m_iJointId,m_pBody1,m_pBody2);
  m_pOdeDevice->jointSetPrAnchor(m_iJointId,getAbsolutePosition());
  m_pOdeDevice->jointSetPrAxis1(m_iJointId,axis1);
  m_pOdeDevice->jointSetPrAxis2(m_iJointId,axis2);

  #ifdef _TRACE_INIT_PHYSICS
    printf("CIrrOdeJointPR: %i: %.2f, %.2f, %.2f\n",ID,getAbsolutePosition().X,getAbsolutePosition().Y,getAbsolutePosition().Z);
  #endif

  for (s32 i=0; i<eParamEnd; i++)
    if (m_bParamUsed[i][0]) {
      #ifdef _TRACE_INIT_PHYSICS
        printf("CIrrOdeJointPR::initPhysics: parameter %i set: %.2f\n",i,m_fParam[i][0]);
      #endif
      setParameter(0,(eJointParameter)i,m_fParam[i][0]);
    }

}

s32 CIrrOdeJointPR::getID() const {
  return ID;
}

ESCENE_NODE_TYPE CIrrOdeJointPR::getType() const {
  return (ESCENE_NODE_TYPE)IRR_ODE_JOINT_PR_ID;
}

const wchar_t *CIrrOdeJointPR::getTypeName() {
  return IRR_ODE_JOINT_PR_NAME;
}

void CIrrOdeJointPR::setPrAxis1(vector3df pAxis) {
  m_vAxis1=pAxis;
  if (m_iJointId) m_pOdeDevice->jointSetPrAxis1(m_iJointId,m_vAxis1);
}

vector3df CIrrOdeJointPR::getPrAxis1() {
  return m_iJointId?m_pOdeDevice->jointGetPrAxis1(m_iJointId):m_vAxis1;
}

void CIrrOdeJointPR::setPrAxis2(vector3df pAxis) {
  m_vAxis2=pAxis;
  if (m_iJointId) m_pOdeDevice->jointSetPrAxis2(m_iJointId,pAxis);
}

vector3df CIrrOdeJointPR::getPrAxis2() {
  return m_iJointId?m_pOdeDevice->jointGetPrAxis2(m_iJointId):m_vAxis2;
}

f32 CIrrOdeJointPR::getPrPosition() {
  return m_iJointId?m_pOdeDevice->jointGetPrPosition(m_iJointId):0.0f;
}

f32 CIrrOdeJointPR::getPrAngle() {
  return m_iJointId?m_pOdeDevice->jointGetPrAngle(m_iJointId):0.0f;
}

f32 CIrrOdeJointPR::getPrAngleRate() {
  return m_iJointId?m_pOdeDevice->jointGetPrAngleRate(m_iJointId):0.0f;
}

u16 CIrrOdeJointPR::numParamGroups() const {
  return 2;
}

void CIrrOdeJointPR::setParameter(u16 iGroup, eJointParameter iParam, f32 fValue) {
  #ifdef _TRACE_INIT_PHYSICS
    printf("CIrrOdeJointPR::setParameter: %i, %i, %.2f\n",iGroup,(int)iParam,fValue);
  #endif
  CIrrOdeJoint::setParameter(iGroup,iParam,fValue);

  if (m_iJointId) m_pOdeDevice->jointSetPrParam(m_iJointId,iParam,fValue);
}

f32 CIrrOdeJointPR::getParameter(u16 iGroup, eJointParameter iParam) {
  return m_iJointId?m_pOdeDevice->jointGetPrParam(m_iJointId,iParam):0.0f;
}

void CIrrOdeJointPR::serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options) const {
  CIrrOdeJoint::serializeAttributes(out,options);
  out->addVector3d("Axis1",m_vAxis1);
  out->addVector3d("Axis2",m_vAxis2);
}

void CIrrOdeJointPR::deserializeAttributes(IAttributes* in, SAttributeReadWriteOptions* options) {
  CIrrOdeJoint::deserializeAttributes(in,options);
  m_vAxis1=in->getAttributeAsVector3d("Axis1");
  m_vAxis2=in->getAttributeAsVector3d("Axis2");
}

ISceneNode *CIrrOdeJointPR::clone(ISceneNode* newParent, ISceneManager* newManager) {
  CIrrOdeJointPR *pRet=new CIrrOdeJointPR(newParent?newParent:getParent(),newManager?newManager:m_pSceneManager);
  copyParams(pRet);
  CIrrOdeSceneNode::cloneChildren(pRet,newManager);
  return pRet;
}

void CIrrOdeJointPR::copyParams(CIrrOdeSceneNode *pDest, bool bRecurse) {
  if (bRecurse) CIrrOdeJoint::copyParams(pDest);
  CIrrOdeJointPR *pDst=(CIrrOdeJointPR *)pDest;
  pDst->setPrAxis1(m_vAxis1);
  pDst->setPrAxis2(m_vAxis2);
}

} //namespace ode
} //namespace irr

