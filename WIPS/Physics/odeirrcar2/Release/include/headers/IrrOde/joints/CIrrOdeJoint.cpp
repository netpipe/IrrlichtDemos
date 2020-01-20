  #include <joints/CIrrOdeJoint.h>
  #include <CIrrOdeWorld.h>
  #include <IIrrOdeDevice.h>
  #include <motors/CIrrOdeMotor.h>
  #include <motors/CIrrOdeServo.h>

namespace irr {
namespace ode {

CIrrOdeJoint::CIrrOdeJoint(ISceneNode *parent,ISceneManager *mgr,s32 id,
                           const vector3df &position,const vector3df &rotation,const vector3df &scale) :
                           CIrrOdeSceneNode(parent, mgr, id, position, rotation, scale) {

  m_iJointId=0;
  m_pBody1=NULL;
  m_pBody2=NULL;

  m_pWorld=reinterpret_cast<CIrrOdeWorld *>(getAncestorOfType((ESCENE_NODE_TYPE)IRR_ODE_WORLD_ID));

  #ifdef _TRACE_CONSTRUCTOR_DESTRUCTOR
    printf("CIrrOdeGeom constructor: m_pWorld=%i\n",m_pWorld->getID());
  #endif

  ISceneNode *pNode=getParent();
  if (pNode->getType()==IRR_ODE_BODY_ID) {
    #ifdef _TRACE_INIT_PHYSICS
      printf("CIrrOdeJoint::initLinkedObjects: ancestor of type CIrrOdeBody ... use as body 1\n");
    #endif
    setBody1(reinterpret_cast<CIrrOdeBody *>(pNode));
  }

  for (u16 i=0; i<CIrrOdeJoint::eParamEnd; i++)
    for (u16 j=0; j<3; j++) {
      m_bParamUsed[i][j]=false;
      m_fParam[i][j]=0.0f;
    }
}

CIrrOdeJoint::~CIrrOdeJoint() {
  #ifdef _TRACE_CONSTRUCTOR_DESCRUTOR
    printf("CIrrOdeJoint destructor\n");
  #endif
  if (!m_pWorld->getWorldId()) {
    #ifdef _TRACE_INIT_PHYSICS
      printf("CIrrOdeJoint destructor: world already destroyed ==> no need to destroy\n");
    #endif
  }
  else if (m_iJointId) m_pOdeDevice->jointDestroy(m_iJointId);
}

void CIrrOdeJoint::setBody1(CIrrOdeBody *pBody1) {
  #ifdef _TRACE_INIT_PHYSICS
    printf("CIrrOdeJoint::setBody1: attach %i to joint\n",pBody1?pBody1->getID():-23);
  #endif
  m_pBody1=pBody1;
  if (m_pBody1) m_pBody1->addJoint(this);
}

void CIrrOdeJoint::setBody2(CIrrOdeBody *pBody2) {
  #ifdef _TRACE_INIT_PHYSICS
    printf("CIrrOdeJoint::setBody2: attach %i to joint\n",pBody2?pBody2->getID():-23);
  #endif
  m_pBody2=pBody2;
  if (m_pBody2) m_pBody2->addJoint(this);
}

CIrrOdeBody *CIrrOdeJoint::getBody1() {
  return m_pBody1;
}

CIrrOdeBody *CIrrOdeJoint::getBody2() {
  return m_pBody2;
}

void CIrrOdeJoint::initLinkedObjects() {
  if (m_pBody2) {
    #ifdef _TRACE_INIT_PHYSICS
      printf("CIrrOdeJoint::initLinkedObjects: body2 already set.\n");
    #endif
    return;
  }

  list<ISceneNode *> children=getChildren();
  list<ISceneNode *>::Iterator i;
  int iCnt=0;
  for (i=children.begin(); i!=children.end(); i++) {
    if ((*i)->getType()==IRR_ODE_BODY_ID) {
      #ifdef _TRACE_INIT_PHYSICS
        printf("CIrrOdeJoint::initLinkedObjects: child #%i of type CIrrOdeBody ... use as body 2\n",iCnt);
      #endif
      setBody2(reinterpret_cast<CIrrOdeBody *>(*i));
    }

    if ((*i)->getType()==IRR_ODE_MOTOR_ID) {
      #ifdef _TRACE_INIT_PHYSICS
        printf("CIrrOdeJoint::initLinkedObjects: motor found ... attaching to joint\n");
      #endif
      CIrrOdeMotor *pMotor=(CIrrOdeMotor *)(*i);
      pMotor->setJoint(this);
      pMotor->initPhysics();
    }

    if ((*i)->getType()==IRR_ODE_SERVO_ID) {
      #ifdef _TRACE_INIT_PHYSICS
        printf("CIrrOdeJoint::initLinkedObjects: servo found ... attaching to joint\n");
      #endif
      CIrrOdeServo *pServo=(CIrrOdeServo *)(*i);
      pServo->setJoint(this);
      pServo->initPhysics();
    }
    iCnt++;
  }
}

void CIrrOdeJoint::setParameter(u16 iGroup, eJointParameter iParam, f32 fValue) {
  #ifdef _TRACE_INIT_PHYSICS
    printf("CIrrOdeJoint::setParameter\n");
  #endif
  m_bParamUsed[iParam][iGroup]=true;
  m_fParam[iParam][iGroup]=fValue;
}

f32 CIrrOdeJoint::getParameter(u16 iGroup, eJointParameter iParam) {
  return 0.0f;
}

void CIrrOdeJoint::setPosition(const vector3df &newPos) {
  ISceneNode::setPosition(newPos);
  updateAbsolutePosition();
}


void CIrrOdeJoint::serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options) const {
  CIrrOdeSceneNode::serializeAttributes(out,options);

  for (int x=0; x<numParamGroups(); x++)
    for (int y=0; y<eParamEnd; y++) {
      c8 sName[0xFF];
      stringc sParamName=CIrrOdeSceneNode::nodeNameToC8(IRR_ODE_PARAM_NAMES[y]);
      sprintf(sName,"use_%s_group_%i",sParamName.c_str(),x);
      out->addBool(sName,m_bParamUsed[y][x]);
      if (m_bParamUsed[y][x]) {
        sprintf(sName,"%s_group_%i",sParamName.c_str(),x);
        out->addFloat(sName,m_fParam[y][x]);
      }
    }
}

void CIrrOdeJoint::deserializeAttributes(IAttributes* in, SAttributeReadWriteOptions* options) {
  CIrrOdeSceneNode::deserializeAttributes(in,options);

  for (int x=0; x<numParamGroups(); x++)
    for (int y=0; y<eParamEnd; y++) {
      c8 sName[0xFF];
      stringc sParamName=CIrrOdeSceneNode::nodeNameToC8(IRR_ODE_PARAM_NAMES[y]);
      sprintf(sName,"use_%s_group_%i",sParamName.c_str(),x);
      m_bParamUsed[y][x]=in->getAttributeAsBool(sName);
      sprintf(sName,"%s_group_%i",sParamName.c_str(),x);
      m_fParam[y][x]=in->getAttributeAsFloat(sName);
    }
}

void CIrrOdeJoint::copyParams(CIrrOdeSceneNode *pDest, bool bRecurse) {
  if (bRecurse) CIrrOdeSceneNode::copyParams(pDest);
  pDest->setName(getName());
  CIrrOdeJoint *pDst=(CIrrOdeJoint *)pDest;

  for (int i=0; i<eParamEnd; i++)
    for (int j=0; j<numParamGroups(); j++) {
      pDst->m_bParamUsed[i][j]=m_bParamUsed[i][j];
      pDst->m_fParam[i][j]=m_fParam[i][j];
    }
}

void CIrrOdeJoint::removeFromPhysics() {
  CIrrOdeSceneNode::removeFromPhysics();
  if (m_iJointId) {
    m_pOdeDevice->jointDestroy(m_iJointId);
    m_iJointId=0;
  }
}

void CIrrOdeJoint::activateBodies() {
  if (m_pBody1 && m_pBody1->isEnabled()==false) m_pBody1->setEnabled(true);
  if (m_pBody2 && m_pBody2->isEnabled()==false) m_pBody2->setEnabled(true);
}

void CIrrOdeJoint::initPhysics() {
  if (m_bPhysicsInitialized) return;
  CIrrOdeSceneNode::initPhysics();
}

} //namespace ode
} //namespace irr
