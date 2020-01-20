  #include <joints/CIrrOdeJointHinge2.h>
  #include <CIrrOdeWorld.h>
  #include <IIrrOdeDevice.h>

namespace irr {
namespace ode {

CIrrOdeJointHinge2::CIrrOdeJointHinge2(ISceneNode *parent,ISceneManager *mgr,s32 id,
                                     const vector3df &position,const vector3df &rotation,const vector3df &scale) :
                                     CIrrOdeJoint(parent, mgr, id, position, rotation, scale) {
  m_pAxis1=vector3df(0,0,1);
  m_pAxis2=vector3df(1,0,0);
  #ifdef _IRREDIT_PLUGIN
    if (m_pMesh) {
	  c8 sFileName[1024];
	  sprintf(sFileName,"%sIrrOdeJointHinge2.png",m_sResources);
      m_cMat.setTexture(0,m_pSceneManager->getVideoDriver()->getTexture(sFileName));
    }
  #endif
}


CIrrOdeJointHinge2::~CIrrOdeJointHinge2() {
}

void CIrrOdeJointHinge2::OnRegisterSceneNode() {
  if (IsVisible) SceneManager->registerNodeForRendering(this);
  ISceneNode::OnRegisterSceneNode();
}

void CIrrOdeJointHinge2::render() {
  CIrrOdeSceneNode::render();
  #ifdef _DRAW_JOINT_INFO
    m_pVideoDriver->setMaterial(m_cMat);
    m_pVideoDriver->setTransform(video::ETS_WORLD, core::matrix4());
    vector3df axis=getAbsoluteTransformation().getRotationDegrees().rotationToDirection(m_pAxis1);
    vector3df axisStart=getAbsolutePosition()-axis;
    vector3df axisEnd=axisStart+10*axis;
    m_pVideoDriver->draw3DLine(axisStart,axisEnd,SColor(128,0xFF,0,0));

    axis=getAbsoluteTransformation().getRotationDegrees().rotationToDirection(m_pAxis2);
    axisStart=getAbsolutePosition()-axis;
    axisEnd=getAbsolutePosition()+10*axis;
    m_pVideoDriver->draw3DLine(axisStart,axisEnd,SColor(128,0XFF,0xFF,0));

    vector3df pos1=m_pBody1?m_pBody1->getAbsolutePosition():getAbsolutePosition();
    vector3df pos2=m_pBody2?m_pBody2->getAbsolutePosition():getAbsolutePosition();
    m_pVideoDriver->draw3DLine(pos1,getAbsolutePosition(),SColor(128,0,0xFF,0));
    m_pVideoDriver->setTransform(video::ETS_WORLD, core::matrix4());
    m_pVideoDriver->draw3DLine(pos2,getAbsolutePosition(),SColor(128,0,0,0xFF));
  #endif
}

void CIrrOdeJointHinge2::initPhysics() {
  if (m_bPhysicsInitialized) return;
  CIrrOdeJoint::initPhysics();

  //create the joint
  m_iJointId=m_pOdeDevice->jointCreateHinge2(m_pWorld->getWorldId());

  //set the parent as body 1
  if (getParent()->getType()==IRR_ODE_BODY_ID)
    setBody1(reinterpret_cast<CIrrOdeBody *>(getParent()));

  vector3df rot=getAbsoluteTransformation().getRotationDegrees(),
            axis1=rot.rotationToDirection(m_pAxis1),
            axis2=rot.rotationToDirection(m_pAxis2);

  m_pOdeDevice->jointAttach(m_iJointId,m_pBody1,m_pBody2);
  m_pOdeDevice->jointSetHinge2Anchor(m_iJointId,getAbsolutePosition());
  m_pOdeDevice->jointSetHinge2Axis1(m_iJointId,axis1);
  m_pOdeDevice->jointSetHinge2Axis2(m_iJointId,axis2);
  m_pOdeDevice->jointSetData(m_iJointId,this);

  for (s32 j=0; j<2; j++)
    for (s32 i=0; i<eParamEnd; i++)
      if (m_bParamUsed[i][0]) {
        #ifdef _TRACE_INIT_PHYSICS
          printf("CIrrOdeJointHinge::initPhysics: parameter %i (set %i) set: %.2f\n",i,j,m_fParam[i][0]);
        #endif
        setParameter(j,(eJointParameter)i,m_fParam[i][j]);
      }

  #ifdef _TRACE_INIT_PHYSICS
    printf("CIrrOdeJointHinge2: %i: %.2f, %.2f, %.2f\n",ID,getAbsolutePosition().X,getAbsolutePosition().Y,getAbsolutePosition().Z);
  #endif
}

s32 CIrrOdeJointHinge2::getID() const {
  return ID;
}

ESCENE_NODE_TYPE CIrrOdeJointHinge2::getType() const {
  return (ESCENE_NODE_TYPE)IRR_ODE_JOINT_HINGE2_ID;
}

const wchar_t *CIrrOdeJointHinge2::getTypeName() {
  return IRR_ODE_JOINT_HINGE2_NAME;
}

void CIrrOdeJointHinge2::setHingeAxis1(vector3df pAxis) {
  m_pAxis1=pAxis.normalize();
}

void CIrrOdeJointHinge2::setHingeAxis2(vector3df pAxis) {
  m_pAxis2=pAxis.normalize();
}

vector3df CIrrOdeJointHinge2::getHingeAxis1() {
  if (m_iJointId) m_pAxis1=m_pOdeDevice->jointGetHinge2Axis1(m_iJointId);
  return m_pAxis1;
}

vector3df CIrrOdeJointHinge2::getHingeAxis2() {
  if (m_iJointId) m_pAxis2=m_pOdeDevice->jointGetHinge2Axis2(m_iJointId);
  return m_pAxis2;
}

vector3df CIrrOdeJointHinge2::getAnchor1() {
  if (m_iJointId) m_pAnchor1=m_pOdeDevice->jointGetHinge2Anchor(m_iJointId);
  return m_pAnchor1;
}

vector3df CIrrOdeJointHinge2::getAnchor2() {
  if (m_iJointId) m_pAnchor2=m_pOdeDevice->jointGetHinge2Anchor2(m_iJointId);
  return m_pAnchor2;
}

f32 CIrrOdeJointHinge2::getHingeAngle1() {
  return m_iJointId?m_pOdeDevice->jointGetHinge2Angle1(m_iJointId):0.0f;
}

f32 CIrrOdeJointHinge2::getHingeAngle1Rate() {
  return m_iJointId?m_pOdeDevice->jointGetHinge2Angle1Rate(m_iJointId):0.0f;
}

f32 CIrrOdeJointHinge2::getHingeAngle2Rate() {
  return m_iJointId?m_pOdeDevice->jointGetHinge2Angle2Rate(m_iJointId):0.0f;
}

u16 CIrrOdeJointHinge2::numParamGroups() const {
  return 2;
}

void CIrrOdeJointHinge2::setParameter(u16 iGroup, eJointParameter iParam, f32 fValue) {
  #ifdef _TRACE_INIT_PHYSICS
    printf("CIrrOdeJointHinge2::setParameter: %i .. %i .. %.2f\n",iGroup,iParam,fValue);
  #endif
  CIrrOdeJoint::setParameter(iGroup,iParam,fValue);

  if (m_iJointId) m_pOdeDevice->jointSetHinge2Param(m_iJointId,iGroup,iParam,fValue);
}

f32 CIrrOdeJointHinge2::getParameter(u16 iGroup, eJointParameter iParam) {
  return m_iJointId?m_pOdeDevice->jointGetHinge2Param(m_iJointId,iGroup,iParam):0.0f;
}

void CIrrOdeJointHinge2::serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options) const {
  CIrrOdeJoint::serializeAttributes(out,options);
  out->addVector3d("Axis1",m_pAxis1);
  out->addVector3d("Axis2",m_pAxis2);
}

void CIrrOdeJointHinge2::deserializeAttributes(IAttributes* in, SAttributeReadWriteOptions* options) {
  CIrrOdeJoint::deserializeAttributes(in,options);
  m_pAxis1=in->getAttributeAsVector3d("Axis1");
  m_pAxis2=in->getAttributeAsVector3d("Axis2");
}

ISceneNode *CIrrOdeJointHinge2::clone(ISceneNode* newParent, ISceneManager* newManager) {
  CIrrOdeJointHinge2 *pRet=new CIrrOdeJointHinge2(newParent?newParent:getParent(),newManager?newManager:m_pSceneManager);
  copyParams(pRet);
  CIrrOdeSceneNode::cloneChildren(pRet,newManager);
  return pRet;
}

void CIrrOdeJointHinge2::copyParams(CIrrOdeSceneNode *pDest, bool bRecurse) {
  if (bRecurse) CIrrOdeJoint::copyParams(pDest);
  CIrrOdeJointHinge2 *pDst=(CIrrOdeJointHinge2 *)pDest;
  pDst->setHingeAxis1(m_pAxis1);
  pDst->setHingeAxis2(m_pAxis2);
}

} //namespace ode
} //namespace irr
