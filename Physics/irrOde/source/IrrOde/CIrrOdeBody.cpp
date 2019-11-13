  #include <CIrrOdeBody.h>
  #include <CIrrOdeWorld.h>
  #include <CIrrOdeManager.h>
  #include <IIrrOdeDevice.h>
  #include <joints/CIrrOdeJoint.h>
  #include <geom/CIrrOdeGeomRay.h>
  #include <event/CIrrOdeEventBodyCreated.h>
  #include <event/CIrrOdeEventBodyRemoved.h>
  #include <event/CIrrOdeEventActivationChanged.h>

namespace irr {
namespace ode {

/**
 * Standard Irrlicht constructor
 */
CIrrOdeBody::CIrrOdeBody(ISceneNode *parent,ISceneManager *mgr,s32 id,
                         const vector3df &position, const vector3df &rotation, const vector3df &scale) :
                         CIrrOdeDampable(parent, mgr, id, position, rotation, scale) {

  #ifdef _TRACE_CONSTRUCTOR_DESTRUCTOR
    printf("CIrrOdeBody contructor\n");
  #endif

  m_pWorld=reinterpret_cast<CIrrOdeWorld *>(getAncestorOfType((ESCENE_NODE_TYPE)IRR_ODE_WORLD_ID));
  if (m_pWorld) m_pWorld->addBody(this);
  m_iBodyId=0;
  m_bEnabled=false;
  m_vLinear=vector3df(0.0f,0.0f,0.0f);
  m_vAngular=vector3df(0.0f,0.0f,0.0f);
  m_vFiniteRotationAxis=vector3df(0.0f,1.0f,0.0f);
  m_pParentBody=reinterpret_cast<CIrrOdeBody *>(getAncestorOfType((ESCENE_NODE_TYPE)IRR_ODE_BODY_ID));
  m_iFiniteRotationMode=0;
  m_iGravityMode=1;
  m_bParamMaster=false;
  m_pTouching=NULL;
  m_bDampingChanged=false;

  #ifdef _IRREDIT_PLUGIN
    if (m_pMesh) {
	  c8 sFileName[1024];
	  sprintf(sFileName,"%sIrrOdeBody.png",m_sResources);
      m_cMat.setTexture(0,m_pSceneManager->getVideoDriver()->getTexture(sFileName));
    }
  #endif

  m_iMass=m_pOdeDevice->massCreate();
  m_pOdeDevice->massSetZero(m_iMass);
  m_bFastMoving=false;
  m_pRay=NULL;

  updateAbsolutePosition();
}

CIrrOdeBody::~CIrrOdeBody() {
  #ifdef _TRACE_CONSTRUCTOR_DESTRUCTOR
    printf("CIrrOdeBody destructor\n");
  #endif

  list<CIrrOdeGeom *>::Iterator i;

  for (i=m_pGeoms.begin(); i!=m_pGeoms.end(); i++) {
    #ifdef _TRACE_CONSTRUCTOR_DESTRUCTOR
      printf("CIrrOdeBody destructor: removing geom from body\n");
    #endif
    (*i)->setBody(NULL);
  }

  if (!m_pWorld->getWorldId()) {
    #ifdef _TRACE_CONSTRUCTOR_DESTRUCTOR
      printf("CIrrOdeBody destructor: world is already destroyed ==> no need to destroy\n");
    #endif
  }
  else if (m_iBodyId) m_pOdeDevice->bodyDestroy(m_iBodyId);
}

/**
 * get the body's ODE ID
 * @return the body's dBodyID
 */
u32 CIrrOdeBody::getBodyId() {
  return m_iBodyId;
}

/**
 * This method initializes the body's physics parameters, creates an ODE version of the body.All attached geoms, joints
 * and bodies will be initialized as well.
 */
void CIrrOdeBody::initPhysics() {
  #ifdef _TRACE_INIT_PHYSICS
    printf("init body: %s\n",getName());
  #endif
  if (m_bPhysicsInitialized) return;
  m_bPhysicsInitialized=true;
  #ifdef _TRACE_INIT_PHYSICS
    printf("CIrrOdeBody::initPhysics\n");
  #endif

  if (m_pWorld) {
    m_iBodyId=m_pOdeDevice->bodyCreate(m_pWorld->getWorldId());

    if (m_iBodyId) {
      updateAbsolutePosition();
      vector3df xxx=getAbsolutePosition();

      if (m_pParentBody) {
        u32 iBody=m_iBodyId;
        setParent(m_pWorld);
        m_pParentBody->updateAbsolutePosition();
        xxx=m_pParentBody->getAbsolutePosition();
        vector3df rot=getAbsoluteTransformation().getRotationDegrees();
        rot+=m_pParentBody->getAbsoluteTransformation().getRotationDegrees();
        setRotation(m_pParentBody->getAbsoluteTransformation().getRotationDegrees());
        m_iBodyId=iBody;
      }

      m_pOdeDevice->bodySetPosition(m_iBodyId,getAbsolutePosition());
      m_pOdeDevice->bodySetQuaternion(m_iBodyId,getAbsoluteTransformation().getRotationDegrees());
      m_pOdeDevice->bodySetData(m_iBodyId,this);

      setLinearDamping          (m_fDampingLinear          );
      setAngularDamping         (m_fDampingAngular         );
      setLinearDampingThreshold (m_fDampingLinearThreshold );
      setAngularDampingThreshold(m_fDampingAngularThreshold);

      if (m_iAutoDisableFlag) {
        setAutoDisableFlag            (m_iAutoDisableFlag            );
        setAutoDisableLinearThreshold (m_fAutoDisableLinearThreshold );
        setAutoDisableAngularThreshold(m_fAutoDisableAngularThreshold);
        setAutoDisableSteps           (m_iAutoDisableSteps           );
        setAutoDisableTime            (m_fAutoDisableTime            );
      }

      if (m_fMaxAngularSpeed!=_DEFAULT_MAX_ANGULAR_SPEED) setMaxAngularSpeed(m_fMaxAngularSpeed);

      if (m_iBodyId) {
        list<CIrrOdeGeom *>::Iterator i;
        for (i=m_pGeoms.begin(); i!=m_pGeoms.end(); i++) {
          CIrrOdeGeom *geom=(*i);
          #ifdef _TRACE_INIT_PHYSICS
            printf("CIrrOdeBody::initPhysics: init geom physics: %i\n",geom?geom->getID():-23);
          #endif
          geom->initPhysics();
        }

        list <CIrrOdeJoint *>::Iterator j;
        for (j=m_pJoints.begin(); j!=m_pJoints.end(); j++) {
          CIrrOdeJoint *joint=(*j);
          joint->initLinkedObjects();
          #ifdef _TRACE_INIT_PHYSICS
            printf("CIrrOdeBody::initPhysics: init joint physics: %i\n",joint?joint->getID():-23);
          #endif

          CIrrOdeBody *body=joint->getBody1();
          if (body && body!=this && !body->physicsInitialized()) {
            #ifdef _TRACE_INIT_PHYSICS
              printf("CIrrOdeBody::initPhysics: init body physics: %i\n",body->getID());
            #endif
            body->initPhysics();
          }

          body=joint->getBody2();
          if (body && body!=this && !body->physicsInitialized()) {
            #ifdef _TRACE_INIT_PHYSICS
              printf("CIrrOdeBody::initPhysics: init body physics: %i\n",body->getID());
            #endif
            body->initPhysics();
          }
          joint->initPhysics();
        }
      }

     m_pOdeDevice->bodySetMass(m_iBodyId,m_iMass);

     m_bEnabled=false;
     setFiniteRotationMode(m_iFiniteRotationMode);
     setGravityMode(m_iGravityMode);

     if (m_pRay) m_pRay->initPhysics();
     CIrrOdeEventBodyCreated cEvent(this);
     CIrrOdeManager::getSharedInstance()->postEvent(&cEvent);
    }
  }
}

/**
 * Set the ODE linear damping parameter
 * @param fScale the new value
 */
void CIrrOdeBody::setLinearDamping(f32 fScale) {
  m_bDampingChanged=m_bDampingChanged || (fScale!=m_fDampingLinear);
  m_fDampingLinear=fScale;
  if (m_iBodyId) m_pOdeDevice->bodySetLinearDamping(m_iBodyId,fScale);
}

/**
 * Get the current linear damping parameter
 * @return the current value
 */
f32 CIrrOdeBody::getLinearDamping() {
  return m_iBodyId?m_pOdeDevice->bodyGetLinearDamping(m_iBodyId):m_fDampingLinear;
}

/**
 * Set the angular damping parameter
 * @param fScale the new value
 */
void CIrrOdeBody::setAngularDamping(f32 fScale) {
  m_bDampingChanged=m_bDampingChanged || (fScale!=m_fDampingAngular);
  m_fDampingAngular=fScale;
  if (m_iBodyId) m_pOdeDevice->bodySetAngularDamping(m_iBodyId,fScale);
}

/**
 * Get the current angular damping parameter
 * @return the current value
 */
f32 CIrrOdeBody::getAngularDamping() {
  return m_iBodyId?m_pOdeDevice->bodyGetAngularDamping(m_iBodyId):m_fDampingAngular;
}

/**
 * Set the linear damping threshold parameter
 * @param fThreshold the new value
 */
void CIrrOdeBody::setLinearDampingThreshold(f32 fThreshold) {
  m_fDampingLinearThreshold=fThreshold;
  if (m_iBodyId) m_pOdeDevice->bodySetLinearDampingThreshold(m_iBodyId,fThreshold);
}

/**
 * Get the linear damping threshold
 * @return the current value
 */
f32 CIrrOdeBody::getLinearDampingThreshold() {
  return m_iBodyId?m_pOdeDevice->bodyGetLinearDampingThreshold(m_iBodyId):m_fDampingLinearThreshold;
}

/**
 * Set the angluar damping threshold
 * @param fThreshold the new value
 */
void CIrrOdeBody::setAngularDampingThreshold(f32 fThreshold) {
  m_fDampingAngularThreshold=fThreshold;
  if (m_iBodyId) m_pOdeDevice->bodySetAngularDampingThreshold(m_iBodyId,fThreshold);
}

/**
 * Get the angular damping threshold
 * @return the current value
 */
f32 CIrrOdeBody::getAngularDampingThreshold() {
  return m_iBodyId?m_pOdeDevice->bodyGetAngularDampingThreshold(m_iBodyId):m_fDampingAngularThreshold;
}

/**
 * Set the maximum angular speed of the body
 * @param fMaxSpeed the new value
 */
void CIrrOdeBody::setMaxAngularSpeed(f32 fMaxSpeed) {
  m_fMaxAngularSpeed=fMaxSpeed;
  if (m_iBodyId) m_pOdeDevice->bodySetMaxAngularSpeed(m_iBodyId,fMaxSpeed);
}

/**
 * Get the maximum angular speed
 * @return the current value
 */
f32 CIrrOdeBody::getMaxAngularSpeed() {
  return m_iBodyId?m_pOdeDevice->bodyGetMaxAngularSpeed(m_iBodyId):m_fMaxAngularSpeed;
}

/**
 * Set the auto disable flag
 * @param iFlag the new value
 */
void CIrrOdeBody::setAutoDisableFlag(int iFlag) {
  m_iAutoDisableFlag=iFlag;
  if (m_iBodyId) m_pOdeDevice->bodySetAutoDisableFlag(m_iBodyId,iFlag);
}

/**
 * Get the auto disable flag
 * @return the current value
 */
int CIrrOdeBody::getAutoDisableFlag() {
  return m_iBodyId?m_pOdeDevice->bodyGetAutoDisableFlag(m_iBodyId):m_iAutoDisableFlag;
}

/**
 * Set the auto disable linear threshold
 * @param fThreshold the new value
 */
void CIrrOdeBody::setAutoDisableLinearThreshold(f32 fThreshold) {
  m_fAutoDisableLinearThreshold=fThreshold;
  if (m_iBodyId) m_pOdeDevice->bodySetLinearAutoDisableThreshold(m_iBodyId,fThreshold);
}

/**
 * Get the auto disable linear threshold
 * @return the current value
 */
f32 CIrrOdeBody::getAutoDisableLinearThreshold() {
  return m_iBodyId?m_pOdeDevice->bodyGetLinearAutoDisableThreshold(m_iBodyId):m_fAutoDisableLinearThreshold;
}

/**
 * Set the auto disable angular threshold
 * @param fThreshold the new value
 */
void CIrrOdeBody::setAutoDisableAngularThreshold(f32 fThreshold) {
  m_fAutoDisableAngularThreshold=fThreshold;
  if (m_iBodyId) m_pOdeDevice->bodySetAngularAutoDisableThreshold(m_iBodyId,fThreshold);
}

/**
 * Get the auto disable angular threshold
 * @return the current value
 */
f32 CIrrOdeBody::getAutoDisableAngularThreshold() {
  return m_iBodyId?m_pOdeDevice->bodyGetAngularAutoDisableThreshold(m_iBodyId):m_fAutoDisableAngularThreshold;
}

/**
 * Set the auto disable steps
 * @param iSteps the new value
 */
void CIrrOdeBody::setAutoDisableSteps(int iSteps) {
  m_iAutoDisableSteps=iSteps;
  if (m_iBodyId) m_pOdeDevice->bodySetAutoDisableSteps(m_iBodyId,iSteps);
}

/**
 * Get the auto disable steps
 * @return the current value
 */
int CIrrOdeBody::getAutoDisableSteps() {
  return m_iBodyId?m_pOdeDevice->bodyGetAutoDisableSteps(m_iBodyId):m_iAutoDisableSteps;
}

/**
 * Set the auto disable time
 * @param fTime the new value
 */
void CIrrOdeBody::setAutoDisableTime(f32 fTime) {
  m_fAutoDisableTime=fTime;
  if (m_iBodyId) m_pOdeDevice->bodySetAutoDisableTime(m_iBodyId,fTime);
}

/**
 * Get the auto disable time
 * @return the current value
 */
f32 CIrrOdeBody::getAutoDisableTime() {
  return m_iBodyId?m_pOdeDevice->bodyGetAutoDisableTime(m_iBodyId):m_fAutoDisableTime;
}

/**
 * Set the finite rotation mode
 * @param iMode the new value
 */
void CIrrOdeBody::setFiniteRotationMode(int iMode) {
  m_iFiniteRotationMode=iMode;
  if (m_iBodyId) m_pOdeDevice->bodySetFiniteRotationMode(m_iBodyId,iMode);
}

/**
 * Get the finite rotation mode
 * @return the current value
 */
int CIrrOdeBody::getFiniteRotationMode() {
  return m_iBodyId?m_pOdeDevice->bodyGetFiniteRotationMode(m_iBodyId):m_iFiniteRotationMode;
}

/**
 * Set the finite rotation axis
 * @param pAxis the finite rotaion axis
 */
void CIrrOdeBody::setFiniteRotationAxis(vector3df &pAxis) {
  m_vFiniteRotationAxis=pAxis;
  if (m_iBodyId) m_pOdeDevice->bodySetFiniteRotationAxis(m_iBodyId,pAxis);
}

/**
 * Get the finite rotation axis
 * @return the finite rotation axis
 */
vector3df &CIrrOdeBody::getFiniteRotationAxis() {
  static vector3df v=m_pOdeDevice->bodyGetFiniteRotationAxis(m_iBodyId);
  return v;
}

/**
 * Get the body's CIrrOdeWorld
 * @return the body's CIrrOdeWorld
 */
CIrrOdeWorld *CIrrOdeBody::getWorld() {
  return m_pWorld;
}

/**
 * Set the body's gravity mode
 * @param the new value
 */
void CIrrOdeBody::setGravityMode(int iMode) {
  m_iGravityMode=iMode;
  if (m_iBodyId) m_pOdeDevice->bodySetGravityMode(m_iBodyId,iMode);
}

/**
 * Get the current gravity mode
 * @return the current value
 */
int CIrrOdeBody::getGravityMode() {
  return m_iBodyId?m_pOdeDevice->bodyGetGravityMode(m_iBodyId):m_iGravityMode;
}

/**
 * Get the body's first geom
 * @return the body's first geom, NULL if the body has no geoms
 */
CIrrOdeGeom *CIrrOdeBody::getFirstGeom() {
  return m_pOdeDevice->bodyGetFirstGeom(m_iBodyId);
}

/**
 * Get the body's next geom
 * @param pPrev the current geom
 * @return the body's next geom, NULL if no further geoms are available
 */
CIrrOdeGeom *CIrrOdeBody::getNextGeom(CIrrOdeGeom *pPrev) {
  return m_pOdeDevice->bodyGetNextGeom(pPrev);
}

/**
 * Get the body's ID
 * @return the body's ID
 */
s32 CIrrOdeBody::getID() {
  return ID;
}

/**
 * Render the body. Well ... just call CIrrOdeSceneNode::render to show something in case we are the IrrEdit plugin
 */
void CIrrOdeBody::render() {
  CIrrOdeSceneNode::render();
}

void CIrrOdeBody::OnRegisterSceneNode() {
  if (IsVisible) SceneManager->registerNodeForRendering(this);
  ISceneNode::OnRegisterSceneNode();
}

/**
 * Get the body's class identifier
 * @return IRR_ODE_BODY_ID
 */
ESCENE_NODE_TYPE CIrrOdeBody::getType() const {
  return (ESCENE_NODE_TYPE)IRR_ODE_BODY_ID;
}

/**
 * Add a geom to the body
 * @param pGeom the geom to be added
 */
void CIrrOdeBody::addGeom(CIrrOdeGeom *pGeom) {
  m_pGeoms.push_back(pGeom);
  #ifdef _TRACE_INIT_PHYSICS
    printf("CIrrOdeBody::addGeom: geom added to body\n");
  #endif
}

/**
 * Remove a geom from the body
 * @param pGeom the geom to be removed
 */
void CIrrOdeBody::removeGeom(CIrrOdeGeom *pGeom) {
  list<CIrrOdeGeom *>::Iterator i;
  for (i=m_pGeoms.begin(); i!=m_pGeoms.end(); i++)
    if ((*i)==pGeom) {
      #ifdef _TRACE_INIT_PHYSICS
        printf("CIrrOdeBody::removeGeom: removing geom from body\n");
      #endif
      m_pGeoms.erase(i);
      return;
    }
}

/**
 * Add a joint to the body
 * @param pJoint the joint to be added
 */
void CIrrOdeBody::addJoint(CIrrOdeJoint *pJoint) {
  list<CIrrOdeJoint *>::Iterator i;
  for (i=m_pJoints.begin(); i!=m_pJoints.end(); i++) if ((*i)==pJoint) return;
  m_pJoints.push_back(pJoint);
  #ifdef _TRACE_INIT_PHYSICS
    printf("CIrrOdeBody::addJoint: joint added to body\n");
  #endif
}

/**
 * Check whether or not this body is enabled
 * @return true if the body is enabled, false otherwise
 */
bool CIrrOdeBody::isEnabled() {
  return m_bEnabled;
}

/**
 * The frame update method. This method is called by the world object after each frame update. It is used to check for
 * activation changed. I might replace this method by an event ... I think I really should do that
 */
void CIrrOdeBody::frameUpdate() {
  if (!m_iBodyId) return;

  bool bEnabled=m_pOdeDevice->bodyIsEnabled(m_iBodyId);
  if (bEnabled!=m_bEnabled) {
    m_bEnabled=bEnabled;
    CIrrOdeEventActivationChanged cEvent(this,m_bEnabled);
    CIrrOdeManager::getSharedInstance()->postEvent(&cEvent);
  }
}

/**
 * Add a force at a specific position to the body
 * @param pos the position of the force to be added
 * @param force the force to be added
 */
void CIrrOdeBody::addForceAtPosition(vector3df pos, vector3df force) {
  m_pOdeDevice->bodyEnable(m_iBodyId);
  m_pOdeDevice->bodyAddForceAtPosition(m_iBodyId,force,pos);
}

/**
 * Add a force to the body at it's center of gravity
 * @param force the force to be added
 */
void CIrrOdeBody::addForce(vector3df force) {
  m_pOdeDevice->bodyEnable(m_iBodyId);
  m_pOdeDevice->bodyAddForce(m_iBodyId,force);
}

/**
 * Add a torque to the body
 * @param torque the torque to add
 */
void CIrrOdeBody::addTorque(vector3df torque) {
  m_pOdeDevice->bodyEnable(m_iBodyId);
  m_pOdeDevice->bodyAddTorque(m_iBodyId,torque);
}

/**
 * Set the body's torque
 * @param torque the new torque of the body
 */
void CIrrOdeBody::setTorque(vector3df torque) {
  m_pOdeDevice->bodyEnable(m_iBodyId);
  m_pOdeDevice->bodySetTorque(m_iBodyId,torque);
}

/**
 * Set linear velocity of the body
 * @param newVelocity the new linear velocity of the body
 */
void CIrrOdeBody::setLinearVelocity(vector3df newVelocity) {
  m_vLinear=newVelocity;
  if (m_bPhysicsInitialized)
    m_pOdeDevice->bodySetLinearVelocity(m_iBodyId,newVelocity);
}

/**
 * Set the angular velocity of the body
 * @param newVelocity the new angular velocity of the body
 */
void CIrrOdeBody::setAngularVelocity(vector3df newVelocity) {
  m_vAngular=newVelocity;
  if (m_bPhysicsInitialized)
    m_pOdeDevice->bodySetAngularVelocity(m_iBodyId,newVelocity);
}

/**
 * Get the linear velocity of the body
 * @return the current linear velocity of the body
 */
vector3df &CIrrOdeBody::getLinearVelocity() {
  if (m_bPhysicsInitialized) m_vLinear=m_pOdeDevice->bodyGetLinearVelocity(m_iBodyId);
  return m_vLinear;
}

/**
 * Get the angular velocity of the body
 * @return the current angular velocity of the body
 */
vector3df &CIrrOdeBody::getAngularVelocity() {
  if (m_bPhysicsInitialized) m_vAngular=m_pOdeDevice->bodyGetAngularVelocity(m_iBodyId);
  return m_vAngular;
}

/**
 * Set the Irrlicht position of the body. This method will update the absolute position if initPhysics was not yet called
 * @param newPos the new position
 */
void CIrrOdeBody::setPosition(const vector3df &newPos) {
  ISceneNode::setPosition(newPos);
  //since ODE only handles world coordinates we update the absolute position if physics is not yet initializes
  if (!m_bPhysicsInitialized)
    updateAbsolutePosition();
  else
    m_pOdeDevice->bodySetPosition(m_iBodyId,newPos);
}

/**
 * Set the Irrlicht rotation of the body. This method will update the absolute position if initPhysics was not yet called
 * @param newRot the new rotation
 */
void CIrrOdeBody::setRotation(const vector3df &newRot) {
  ISceneNode::setRotation(newRot);
  //since ODE only handles world coordinates we update the absolute position if physics is not yet initializes
  if (!m_bPhysicsInitialized) updateAbsolutePosition();
}

/**
 * Get the parent body attached by a joint to this one
 * @return the parent body attached by a joint, NULL if no parent body is attached
 */
CIrrOdeBody *CIrrOdeBody::getParentBody() {
  return m_pParentBody;
}

/**
 * Get the body's type name
 * @return IRR_ODE_BODY_NAME
 */
const wchar_t *CIrrOdeBody::getTypeName() {
  return IRR_ODE_BODY_NAME;
}

void CIrrOdeBody::addChild(ISceneNode *pChild) {
	ISceneNode::addChild(pChild);
	#ifdef _IRREDIT_PLUGIN
	  pChild->setPosition(vector3df(0.0f,0.0f,0.0f));
  #endif
}

/**
 * Add a child body to this body. A child body is another body connected by a joint
 * @param pBody the child body to be added
 */
void CIrrOdeBody::addChildBody(CIrrOdeBody *pBody) {
  #ifdef _TRACE_INIT_PHYSICS
    printf("CIrrOdeBody::addChildBody: adding body %i to list\n",pBody->getID());
  #endif
  m_pChildBodies.push_back(pBody);
}

/**
 * Get a list of the joints attached to the body
 * @return a list of the joints attached to the body
 */
list<CIrrOdeJoint *> CIrrOdeBody::getJoints() {
  return m_pJoints;
}

void CIrrOdeBody::serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options) const {
  CIrrOdeDampable::serializeAttributes(out,options);
  out->addBool("Enabled",m_bEnabled);
  out->addBool("ParamMaster",m_bParamMaster);

  out->addInt ("FiniteRotationMode",m_iFiniteRotationMode);
  out->addInt ("GravityMode"       ,m_iGravityMode       );
  out->addBool("FastMoving"        ,m_bFastMoving        );
}

void CIrrOdeBody::deserializeAttributes(IAttributes* in, SAttributeReadWriteOptions* options) {
  CIrrOdeDampable::deserializeAttributes(in,options);
  m_bEnabled=in->getAttributeAsBool("Enabled");
  m_bParamMaster=in->getAttributeAsBool("ParamMaster");

  m_iFiniteRotationMode=in->getAttributeAsInt ("FiniteRotationMode");
  m_iGravityMode       =in->getAttributeAsInt ("GravityMode"       );
  m_bFastMoving        =in->getAttributeAsBool("FastMoving"        );
}

ISceneNode *CIrrOdeBody::clone(ISceneNode* newParent, ISceneManager* newManager) {
  CIrrOdeBody *pRet=new CIrrOdeBody(newParent?newParent:getParent(),newManager?newManager:m_pSceneManager);

  CIrrOdeSceneNode::cloneChildren(pRet,newManager?newManager:m_pSceneManager);

  copyParams(pRet,true);
  return pRet;
}

/**
 * Set this body to be the parameter master. At physics initialization the body's parameters are copied to all other
 * bodies with the same OdeClassName
 * @param b "true" if the body should be parameter master, "false" otherwise
 */
void CIrrOdeBody::setParamMaster(bool b) {
  m_bParamMaster=b;
}

/**
 * Is this body the parameter master?
 * @return "true" is the body is parameter master, "false" otherwise
 */
bool CIrrOdeBody::isParamMaster() {
  return m_bParamMaster;
}

/**
 * Copy the bodies parameters to another body
 * @param pDest the body that retrieves the parameters
 * @param bRecurse flag to tell the body wether or not the parameter copying should be continued through the object hierarchy
 */
void CIrrOdeBody::copyParams(CIrrOdeSceneNode *pDest, bool bRecurse) {
  #ifdef _TRACE_CONSTRUCTOR_DESTRUCTOR
    printf("CIrrOdeBody copy contructor\n");
  #endif

  if (bRecurse) CIrrOdeDampable::copyParams(pDest);
  CIrrOdeBody *pDst=(CIrrOdeBody *)pDest;

  pDst->setFiniteRotationAxis(m_vFiniteRotationAxis);
  pDst->setFiniteRotationMode(m_iFiniteRotationMode);
  pDst->setGravityMode(m_iGravityMode);
  m_bParamMaster=false;

  #ifdef _IRREDIT_PLUGIN
    if (m_pMesh) {
	  c8 sFileName[1024];
	  sprintf(sFileName,"%sIrrOdeBody.png",m_sResources);
      m_cMat.setTexture(0,m_pSceneManager->getVideoDriver()->getTexture(sFileName));
    }
  #endif
}

/**
 * Remove the body from the physics. Note that only the body, not it's geoms or joints will be removed (change this?)
 */
void CIrrOdeBody::removeFromPhysics() {
  if (m_iBodyId) {
    m_pOdeDevice->bodyDestroy(m_iBodyId);
    m_iBodyId=0;

    CIrrOdeEventBodyRemoved cEvent(this);
    CIrrOdeManager::getSharedInstance()->postEvent(&cEvent);
  }

  if (m_pWorld) m_pWorld->removeBody(this);
}

/**
 * Add a geoms mass to the body
 * @param iMassId ID of the mass to be added (retrieved from CIrrOdeDevice)
 */
void CIrrOdeBody::addMass(u32 iMassId) {
  m_pOdeDevice->massAdd(m_iMass,iMassId);
}

void CIrrOdeBody::setEnabled(bool bEnabled) {
  if (bEnabled)
    m_pOdeDevice->bodyEnable(m_iBodyId);
  else
    m_pOdeDevice->bodyDisable(m_iBodyId);
}

void CIrrOdeBody::setCollision(bool b) {
  m_bCollision=b;
}

bool CIrrOdeBody::getCollision() {
  return m_bCollision;
}

void CIrrOdeBody::setIsTouching(CIrrOdeGeom *pTouch) {
  m_pTouching=pTouch;
}

CIrrOdeGeom *CIrrOdeBody::getTouched() {
  return m_pTouching;
}

void CIrrOdeBody::setNodePosition(vector3df newPos) {
  ISceneNode::setPosition(newPos);
}

void CIrrOdeBody::setNodeRotation(vector3df newRot) {
  ISceneNode::setRotation(newRot);
}

void CIrrOdeBody::setNodeLinearVelocity(vector3df newVel) {
  m_vLinear=newVel;
}

void CIrrOdeBody::setNodeAngularVelocity(vector3df newVel) {
  m_vAngular=newVel;
}

vector3df CIrrOdeBody::getNodeLinearVelocity() {
  return m_vLinear;
}

vector3df CIrrOdeBody::getNodeAngularVelocity() {
  return m_vAngular;
}

void CIrrOdeBody::setCollisionPoint(vector3df vPoint) {
  m_vCollisionPoint=vPoint;
}

vector3df CIrrOdeBody::getCollisionPoint() {
  return m_vCollisionPoint;
}

void CIrrOdeBody::setCollisionMaterial(s32 iIdx) {
  m_iCollisionMaterial=iIdx;
}

s32 CIrrOdeBody::getCollisionMaterial() {
  return m_iCollisionMaterial;
}

vector3df CIrrOdeBody::getPointVel(vector3df vPos) {
  return m_pOdeDevice->bodyGetPointVel(m_iBodyId,vPos);
}

vector3df CIrrOdeBody::getRelPointVel(vector3df vPos) {
  return m_pOdeDevice->bodyGetRelPointVel(m_iBodyId,vPos);
}

vector3df CIrrOdeBody::getRelPointPos(vector3df vPos) {
  return m_pOdeDevice->bodyGetRelPointPos(m_iBodyId,vPos);
}

bool CIrrOdeBody::dampingChanged() {
  return m_bDampingChanged;
}

void CIrrOdeBody::setNodeLinearDamping(f32 fScale) {
  m_bDampingChanged=m_bDampingChanged || (fScale!=m_fDampingLinear);
  m_fDampingLinear=fScale;
}

void CIrrOdeBody::setNodeAngularDamping(f32 fScale) {
  m_bDampingChanged=m_bDampingChanged || (fScale!=m_fDampingAngular);
  m_fDampingAngular=fScale;
}

f32 CIrrOdeBody::getNodeLinearDamping() {
  return m_fDampingLinear;
}

f32 CIrrOdeBody::getNodeAngularDamping() {
  return m_fDampingAngular;
}

void CIrrOdeBody::setIsFastMoving(bool b) {
  m_bFastMoving=b;
  if (m_pRay!=NULL) {
    m_pRay->drop();
    m_pRay=NULL;
  }

  if (b) {
    ISceneNode *pNode=m_pSceneManager->addSceneNode(CIrrOdeSceneNode::nodeNameToC8(IRR_ODE_GEOM_RAY_NAME),m_pWorld);
    m_pRay=reinterpret_cast<CIrrOdeGeomRay *>(pNode);
    m_pRay->setAttachedBody(this);
    if (m_bPhysicsInitialized) m_pRay->initPhysics();
  }
}

bool CIrrOdeBody::isFastMoving() {
  return m_bFastMoving;
}

void CIrrOdeBody::bodyMoved(vector3df newPos) {
  if (m_bFastMoving && m_pRay) {
    updateAbsolutePosition();
    vector3df dir=newPos-getAbsolutePosition();
    f32 fVel=dir.getLength();
    m_pRay->set(getAbsolutePosition(),dir.normalize(),fVel);
  }
}

} //namespace ode
} //namespace irr
