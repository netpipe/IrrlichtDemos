  #include <CIrrOdeManager.h>
  #include <irrlicht.h>
  #include <CIrrOdeSurfaceParameters.h>
  #include <CIrrOdeWorld.h>
  #include <event/IIrrOdeEventListener.h>
  #include <event/CIrrOdeEventBodyMoved.h>
  #include <event/CIrrOdeEventBodyRemoved.h>
  #include <event/CIrrOdeEventInit.h>
  #include <event/CIrrOdeEventClose.h>
  #include <event/CIrrOdeEventProgress.h>
  #include <event/CIrrOdeEventNodeCloned.h>
  #include <event/CIrrOdeEventLoadScene.h>
  #include <event/CIrrOdeEventQueue.h>
  #include <event/CIrrOdeEventNodeRemoved.h>
  #include <IIrrOdeDevice.h>
  #include <motors/IIrrOdeStepMotor.h>

  #ifndef _USE_ODE_NULL_DEVICE
    #include <CIrrOdeDevice.h>
  #else
    #include <CIrrOdeNullDevice.h>
  #endif

namespace irr {
namespace ode {

CIrrOdeManager::CIrrOdeManager() {
  m_iLastStep=0;
  m_pTimer=NULL;
  m_iFrameNo=0;
  #ifndef _USE_ODE_NULL_DEVICE
    m_pOdeDevice=new CIrrOdeDevice();
  #else
    m_pOdeDevice=new CIrrOdeNullDevice();
  #endif
  m_iNodesInitialized=0;
  m_bPhysicsInitialized=false;
  m_iNextOdeNodeId=20000;

  m_pQueue=new CIrrOdeEventQueue();
  m_pDevice=NULL;
  m_pSmgr=NULL;
  m_pTimer=NULL;
}

CIrrOdeManager::~CIrrOdeManager() {
  if(this->physicsInitialized()==true) this->closeODE();
  this->getIrrOdeNodes().clear();
  delete m_pOdeDevice;
}

/**
 * Get the shared singleton instance of the manager
 * @return the shared singleton instance of the manager
 */
CIrrOdeManager *CIrrOdeManager::getSharedInstance() {
  static CIrrOdeManager theManager;
  return &theManager;
}

/**
 * Initialize ODE. This call needs to be done once
 */
void CIrrOdeManager::initODE() {
  CIrrOdeEventInit *pEvent=new CIrrOdeEventInit();
  m_pQueue->postEvent(pEvent);

  getOdeDevice()->initODE();
}

void CIrrOdeManager::addWorld(CIrrOdeWorld *pWorld) {
  list<CIrrOdeWorld *>::Iterator it;
  for (it=m_lWorlds.begin(); it!=m_lWorlds.end(); it++)
    if ((*it)==pWorld)
      return;

  m_lWorlds.push_back(pWorld);
}

void CIrrOdeManager::removeWorld(CIrrOdeWorld *pWorld) {
  list<CIrrOdeWorld *>::Iterator it;
  for (it=m_lWorlds.begin(); it!=m_lWorlds.end(); it++)
    if ((*it)==pWorld) {
      m_lWorlds.erase(it);
      return;
    }
}

/**
 * Shutdown ODE
 */
void CIrrOdeManager::closeODE() {
  getOdeDevice()->closeODE();

  CIrrOdeEventClose *pEvent=new CIrrOdeEventClose();
  m_pQueue->postEvent(pEvent);

  list<CIrrOdeWorld *>::Iterator wit;
  for (wit=m_lWorlds.begin(); wit!=m_lWorlds.end(); wit++) (*wit)->setPhysicsInitialized(false);
  list<CIrrOdeSceneNode *>::Iterator nit;
  for (nit=m_pSceneNodes.begin(); nit!=m_pSceneNodes.end(); nit++) (*nit)->setPhysicsInitialized(false);
  list<IIrrOdeStepMotor *>::Iterator mit;
  for (mit=m_lStepMotors.begin(); mit!=m_lStepMotors.end(); mit++) (*mit)->setPhysicsInitialized(false);

  m_pSceneNodes.clear();
  m_lParamList.clear();
  m_lStepMotors.clear();
  m_lWorlds.clear();

  m_iNodesInitialized=0;
  m_bPhysicsInitialized=false;
}

void CIrrOdeManager::clearODE() {
  getOdeDevice()->clearODE();
  m_pSceneNodes.clear();
  m_lParamList.clear();

  m_iNodesInitialized=0;
  m_bPhysicsInitialized=false;

  CIrrOdeEventClose *pEvent=new CIrrOdeEventClose();
  m_pQueue->postEvent(pEvent);
}

void CIrrOdeManager::install(IrrlichtDevice *pDevice) {
  m_pDevice=pDevice;
  m_pSmgr=m_pDevice->getSceneManager();
  m_pTimer=m_pDevice->getTimer();
  while (m_pTimer->isStopped()) m_pTimer->start();

  m_pQueue->addEventListener(this);
}
/**
 * Step the worlds
 */
void CIrrOdeManager::step() {
  if (!m_pTimer) printf("###### CIrrOdeManager::step: timer==NULL!\n");
  u32 thisStep=m_pTimer->getTime();
  f32 fStep=((f32)(thisStep-m_iLastStep))/1000;
  m_iFrameNo++;

  list<CIrrOdeWorld *>::Iterator it;
  for (it=m_lWorlds.begin(); it!=m_lWorlds.end(); it++) (*it)->step(fStep);

  m_iLastStep=thisStep;
}

/**
 * Quickstep the worlds
 */
void CIrrOdeManager::quickStep() {
  if (!m_pTimer) printf("###### CIrrOdeManager::quickStep: timer==NULL!\n");
  u32 thisStep=m_pTimer->getTime();
  f32 fStep=((f32)(thisStep-m_iLastStep))/1000;
  m_iFrameNo++;

    list<CIrrOdeWorld *>::Iterator it;
  for (it=m_lWorlds.begin(); it!=m_lWorlds.end(); it++) (*it)->quickStep(fStep);

  m_iLastStep=thisStep;
}

/**
 * Add a new ODE scene node
 */
void CIrrOdeManager::addOdeSceneNode(CIrrOdeSceneNode *pNode) {
  if (pNode->getID()>m_iNextOdeNodeId) m_iNextOdeNodeId=pNode->getID();
  m_pSceneNodes.push_back(pNode);
}

/**
 * Remove an ODE scene node
 */
void CIrrOdeManager::removeOdeSceneNode(CIrrOdeSceneNode *pNode) {
  list<CIrrOdeSceneNode *>::Iterator it;
  for (it=m_pSceneNodes.begin(); it!=m_pSceneNodes.end(); it++)
    if ((*it)==pNode) {
      m_pSceneNodes.erase(it);
      return;
    }
}

/**
 * Retrieve the used ODE device
 */
IIrrOdeDevice *CIrrOdeManager::getOdeDevice() {
  return m_pOdeDevice;
}

/**
 * Set a new ODE device. Remember: you have to destroy the old device yourself, IrrODE will not do this!
 */
void CIrrOdeManager::setOdeDevice(IIrrOdeDevice *pDevice) {
  #ifndef _USE_ODE_NULL_DEVICE
    m_pOdeDevice=pDevice;
  #endif
}

/**
 * Initialize physics
 */
void CIrrOdeManager::initPhysics() {
  m_iNodesInitialized=0;
  m_bPhysicsInitialized=false;

  list<CIrrOdeWorld *>::Iterator itw;
  for (itw=m_lWorlds.begin(); itw!=m_lWorlds.end(); itw++)
    (*itw)->initPhysics();

  core::list<irr::ode::IIrrOdeStepMotor *>::Iterator it;
  for (it=m_lStepMotors.begin(); it!=m_lStepMotors.end(); it++) (*it)->initPhysics();

  m_bPhysicsInitialized=true;
}

list<CIrrOdeSceneNode *> &CIrrOdeManager::getIrrOdeNodes() {
  return m_pSceneNodes;
}

core::list<CIrrOdeWorld *> &CIrrOdeManager::getWorlds() {
  return m_lWorlds;
}

void CIrrOdeManager::addSurfaceParameter(CIrrOdeSurfaceParameters *pParam) {
  m_lParamList.push_back(pParam);
}

void CIrrOdeManager::removeSurfaceParameter(CIrrOdeSurfaceParameters *pParam) {
  list<CIrrOdeSurfaceParameters *>::Iterator it;
  for (it=m_lParamList.begin(); it!=m_lParamList.end(); it++)
    if ((*it)==pParam) {
      m_lParamList.erase(it);
      return;
    }
}

CIrrOdeSurfaceParameters *CIrrOdeManager::getSurfaceParameter(stringw sName) {
	list<CIrrOdeSurfaceParameters *>::Iterator it;

	#ifdef _TRACE_INIT_PHYSICS
	  printf("\tsearching for surface parameters \"%s\" ... ",stringc(sName).c_str());
  #endif

	if (sName=="") {
	  #ifdef _TRACE_INIT_PHYSICS
	    printf("no search name defined!\n");
    #endif
	  return NULL;
	}

	for (it=m_lParamList.begin(); it!=m_lParamList.end(); it++)
		if (sName==stringw((*it)->getName()) && !(*it)->doesUseDefined()) {
		  #ifdef _TRACE_INIT_PHYSICS
		    printf("OK\n");
      #endif
		  return *it;
		}

  #ifdef _TRACE_INIT_PHYSICS
    printf("not found!\n");
  #endif
	return NULL;
}

void CIrrOdeManager::addStepMotor(IIrrOdeStepMotor *pMotor) {
  list<IIrrOdeStepMotor *>::Iterator it;
  for (it=m_lStepMotors.begin(); it!=m_lStepMotors.end(); it++) if (*it==pMotor) return;
  m_lStepMotors.push_back(pMotor);
}

void CIrrOdeManager::removeStepMotor(IIrrOdeStepMotor *pMotor) {
  list<IIrrOdeStepMotor *>::Iterator it;
  for (it=m_lStepMotors.begin(); it!=m_lStepMotors.end(); it++) if (*it==pMotor) { m_lStepMotors.erase(it); return; }
}

void CIrrOdeManager::stepStepMotors() {
  list<IIrrOdeStepMotor *>::Iterator it;
  for (it=m_lStepMotors.begin(); it!=m_lStepMotors.end(); it++) (*it)->step();
}

void CIrrOdeManager::sceneNodeInitialized(CIrrOdeSceneNode *pNode) {
  if (!m_bPhysicsInitialized) {
    list<CIrrOdeSceneNode *>::Iterator it;
    bool b=false;
    for (it=m_pSceneNodes.begin(); it!=m_pSceneNodes.end() && !b; it++)
      if (*it==pNode) b=true;

    if (b) {
      m_iNodesInitialized++;
      CIrrOdeEventProgress *pPrg=new CIrrOdeEventProgress (m_iNodesInitialized,m_pSceneNodes.getSize());
      m_pQueue->postEvent(pPrg);
    }
  }
}

CIrrOdeDampable *CIrrOdeManager::getDampableWithParamName(const wchar_t *sName) {
  list<CIrrOdeWorld *>::Iterator wit;
  for (wit=m_lWorlds.begin(); wit!=m_lWorlds.end(); wit++) {
		CIrrOdeWorld *p=*wit;
		if (!wcscmp(sName,p->getParamName())) return p;
	}

	list<CIrrOdeSceneNode *>::Iterator it;
	for (it=m_pSceneNodes.begin(); it!=m_pSceneNodes.end(); it++) {
		if ((*it)->getType()==irr::ode::IRR_ODE_BODY_ID) {
			CIrrOdeBody *p=reinterpret_cast<CIrrOdeBody *>(*it);
      if (!wcscmp(sName,p->getParamName()) && !p->doesUseDefined()) return p;
		}
	}

	return NULL;
}

ISceneNode *CIrrOdeManager::cloneTree(ISceneNode *pSource, ISceneNode *newParent, ISceneManager *newSmgr) {
  ISceneNode *pRet=cloneOdeNode(pSource,newParent,newSmgr);

  CIrrOdeEventNodeCloned *pEvent=new CIrrOdeEventNodeCloned(pSource->getID(),pRet->getID());
  m_pQueue->postEvent(pEvent);

  return pRet;
}

ISceneNode *CIrrOdeManager::cloneOdeNode(ISceneNode *pSource, ISceneNode *newParent, ISceneManager *newSmgr, s32 iNewId) {
  ISceneNode *pRet=pSource->clone(newParent,newSmgr);
  pRet->setID(iNewId==-1?getNextId():iNewId);
  pRet->setParent(newParent);

  return pRet;
}


void CIrrOdeManager::removeTreeFromPhysics(ISceneNode *pNode) {
  list<ISceneNode *> ch=pNode->getChildren();
  list<ISceneNode *>::Iterator it;

  for (it=ch.begin(); it!=ch.end(); it++) {
    if (isRegisteredOdeSceneNode(*it)) {
      CIrrOdeSceneNode *p=(CIrrOdeSceneNode *)(*it);
      p->removeFromPhysics();
    }
    removeTreeFromPhysics(*it);
  }
}

void CIrrOdeManager::removeSceneNode(ISceneNode *pNode) {
  CIrrOdeEventNodeRemoved *p=new CIrrOdeEventNodeRemoved(pNode);
  m_pQueue->postEvent(p);
}

bool CIrrOdeManager::loadScene(const c8 *sScene, irr::scene::ISceneManager *pSmgr) {
  bool bRet=pSmgr->loadScene(sScene);
  if (bRet) {
    CIrrOdeEventLoadScene *pEvt=new CIrrOdeEventLoadScene(sScene);
    m_pQueue->postEvent(pEvt);
  }
  return bRet;
}

bool CIrrOdeManager::isRegisteredOdeSceneNode(irr::scene::ISceneNode *pNode) {
  core::list<irr::ode::CIrrOdeSceneNode *>::Iterator it;

  for (it=m_pSceneNodes.begin(); it!=m_pSceneNodes.end(); it++)
    if ((*it)==pNode)
      return true;

  return false;
}

bool CIrrOdeManager::onEvent(IIrrOdeEvent *pEvt) {
  if (pEvt==NULL) return false;

  if (pEvt->getType()==eIrrOdeEventBodyMoved) {
    CIrrOdeEventBodyMoved *pEvent=(CIrrOdeEventBodyMoved *)pEvt;
    CIrrOdeBody *pBody=pEvent->getBody();
    if (pBody==NULL) {
      pBody=reinterpret_cast<CIrrOdeBody *>(m_pSmgr->getSceneNodeFromId(pEvent->getBodyId()));
    }

    if (pBody!=NULL) {
      if (pEvent->positionChanged  ()) pBody->setNodePosition(pEvent->getNewPosition());
      if (pEvent->rotationChanged  ()) pBody->setNodeRotation(pEvent->getNewRotation());
      if (pEvent->linearVelChanged ()) pBody->setNodeLinearVelocity(pEvent->getNewLinearVelocity());
      if (pEvent->angularVelChanged()) pBody->setNodeAngularVelocity(pEvent->getNewAngularVelocity());
      if (pEvent->dampingChanged   ()) {
        pBody->setNodeAngularDamping(pEvent->getAngularDamping());
        pBody->setNodeLinearDamping (pEvent->getLinearDamping ());
      }

      if (pEvent->getTouchId()) {
        if (pEvent->getTouched())
          pBody->setIsTouching(pEvent->getTouched());
        else {
          ISceneNode *pNode=m_pSmgr->getSceneNodeFromId(pEvent->getTouchId());
          CIrrOdeGeom *pGeom=reinterpret_cast<CIrrOdeGeom *>(pNode);
          pBody->setIsTouching(pGeom);
        }
      }
      else pBody->setIsTouching(NULL);
    }
    return true;
  }

  if (pEvt->getType()==eIrrOdeEventBodyRemoved) {
    CIrrOdeEventBodyRemoved *pBr=(CIrrOdeEventBodyRemoved *)pEvt;
    if (pBr->getBody()!=NULL)
      pBr->getBody()->doRemoveFromPhysics();
    else {
      ISceneNode *pNode=m_pSmgr->getSceneNodeFromId(pBr->getBodyId());
      if (pNode!=NULL) {
        if (pNode->getType()==(ESCENE_NODE_TYPE)IRR_ODE_BODY_ID) {
          CIrrOdeBody *pBody=(CIrrOdeBody *)pNode;
          pBody->doRemoveFromPhysics();
        }
      }
    }
  }

  if (pEvt->getType()==eIrrOdeEventNodeRemoved) {
    CIrrOdeEventNodeRemoved *pNr=(CIrrOdeEventNodeRemoved *)pEvt;
    ISceneNode *pNode=pNr->getRemovedNode();
    if (pNode==NULL && pNr->getRemovedNodeId()!=-1) {
      pNode=m_pSmgr->getSceneNodeFromId(pNr->getRemovedNodeId());
    }

    if (pNode!=NULL) m_pSmgr->addToDeletionQueue(pNode);
  }
  return false;
}

bool CIrrOdeManager::handlesEvent(IIrrOdeEvent *pEvent) {
  return pEvent->getType()==eIrrOdeEventBodyMoved   ||
         pEvent->getType()==eIrrOdeEventBodyRemoved ||
         pEvent->getType()==eIrrOdeEventNodeRemoved;
}

} //namespace ode
} //namespace irr
