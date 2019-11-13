  #include <CIrrOdeManager.h>
  #include <irrlicht.h>
  #include <CIrrOdeSurfaceParameters.h>
  #include <CIrrOdeWorld.h>
  #include <event/IIrrOdeEventListener.h>
  #include <event/CIrrOdeEventBodyMoved.h>
  #include <event/CIrrOdeEventInit.h>
  #include <event/CIrrOdeEventClose.h>
  #include <IIrrOdeDevice.h>

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
  #endif
}

CIrrOdeManager::~CIrrOdeManager() {
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
 * Add a world to the manager
 * @param pWorld the world to be added
 */
void CIrrOdeManager::addWorld(CIrrOdeWorld *pWorld) {
  m_pWorlds.push_back(pWorld);
}

/**
 * Get the number of worlds the manager manages
 * @return the number of worlds the manager manages
 */
u32 CIrrOdeManager::getWorldCount() {
  return m_pWorlds.size();
}

/**
 * Get the world at a specific index
 * @param iPos the index of the world
 * @return the world at the given index
 */
CIrrOdeWorld *CIrrOdeManager::getWorld(u32 iPos) {
  return iPos<m_pWorlds.size()?m_pWorlds[iPos]:NULL;
}

/**
 * Remove a world from the manager
 * @param pWorld the world to remove
 */
void CIrrOdeManager::removeWorld(CIrrOdeWorld *pWorld) {
  s32 idx=m_pWorlds.binary_search(pWorld);
  if (idx!=-1) m_pWorlds.erase(idx);
}

/**
 * Initialize ODE. This call needs to be done once
 */
void CIrrOdeManager::initODE() {
  CIrrOdeEventInit *pEvent=new CIrrOdeEventInit();
  postEvent(pEvent);
  delete pEvent;

  getOdeDevice()->initODE();
}

/**
 * Shutdown ODE
 */
void CIrrOdeManager::closeODE() {
  getOdeDevice()->closeODE();

  for (u32 i=0; i<m_pWorlds.size(); i++) m_pWorlds[i]->setPhysicsInitialized(false);
  list<CIrrOdeSceneNode *>::Iterator nit;
  for (nit=m_pSceneNodes.begin(); nit!=m_pSceneNodes.end(); nit++) (*nit)->setPhysicsInitialized(false);
  list<CIrrOdeGeom *>::Iterator git;
  for (git=m_pGeoms.begin(); git!=m_pGeoms.end(); git++) (*git)->setPhysicsInitialized(false);

  m_pWorlds.clear();
  m_pSceneNodes.clear();
  m_pGeoms.clear();
  m_lParamList.clear();

  CIrrOdeEventClose *pEvent=new CIrrOdeEventClose ();
  postEvent(pEvent);
  delete pEvent;
}

void CIrrOdeManager::clearODE() {
  getOdeDevice()->clearODE();
  m_pWorlds.clear();
  m_pSceneNodes.clear();
  m_pGeoms.clear();
  m_lParamList.clear();

  CIrrOdeEventClose *pEvent=new CIrrOdeEventClose ();
  postEvent(pEvent);
  delete pEvent;
}

/**
 * Set a timer for the manager
 */
void CIrrOdeManager::setTimer(ITimer *pTimer) {
  m_pTimer=pTimer;
}

/**
 * Step the worlds
 */
void CIrrOdeManager::step() {
  if (!m_pTimer) printf("###### CIrrOdeManager::step: timer==NULL!\n");
  u32 thisStep=m_pTimer->getTime();
  f32 fStep=((f32)(thisStep-m_iLastStep))/1000;
  m_iFrameNo++;

  for (u16 i=0; i<m_pWorlds.size(); i++)
    if (m_pWorlds[i]->physicsInitialized())
      m_pWorlds[i]->step(fStep);

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

  for (u16 i=0; i<m_pWorlds.size(); i++)
    if (m_pWorlds[i]->physicsInitialized())
      m_pWorlds[i]->quickStep(fStep);

  m_iLastStep=thisStep;
}

/**
 * Add an event listener
 */
void CIrrOdeManager::addEventListener(IIrrOdeEventListener *pListener) {
  m_pListeners.push_back(pListener);
}

void CIrrOdeManager::removeEventListener(IIrrOdeEventListener *pListener) {
	list<IIrrOdeEventListener *>::Iterator it;
	for (it=m_pListeners.begin(); it!=m_pListeners.end(); it++)
	  if (*it==pListener) {
			m_pListeners.erase(it);
			return;
	  }
}

/**
 * Post an event to all registered event listeners
 */
void CIrrOdeManager::postEvent(IIrrOdeEvent *pEvent) {
  //this is a special case: in order to apply movement, rotation and velocity
  //changes to a body it will instantly be posted to it's world
  if (pEvent->getType()==eIrrOdeEventBodyMoved) {
    CIrrOdeEventBodyMoved *p=(CIrrOdeEventBodyMoved *)pEvent;
    if (m_pWorlds.size()>0)
      m_pWorlds[0]->bodyMoved(p);
  }

  list<IIrrOdeEventListener *>::Iterator i;
  for (i=m_pListeners.begin(); i!=m_pListeners.end(); i++) {
    if ((*i)->handlesEvent(pEvent))
      (*i)->onEvent(pEvent);
  }
}

/**
 * Add a new ODE scene node
 */
void CIrrOdeManager::addOdeSceneNode(CIrrOdeSceneNode *pNode) {
  m_pSceneNodes.push_back(pNode);
}

/**
 * Remove an ODE scene node
 */
void CIrrOdeManager::removeOdeSceneNode(CIrrOdeSceneNode *pNode) {
  pNode->removeFromPhysics();
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
  #ifndef _USE_ODE_NULL_DEVICE
    return m_pOdeDevice;
  #else
    static CIrrOdeNullDevice cTheDevice;
    return &cTheDevice;
  #endif
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
  for (u32 i=0; i<m_pWorlds.size(); i++) {
    m_pWorlds[i]->initPhysics();
  }
}

/**
 * add a geom to the manager
 * @param pGeom the new geom
 */

void CIrrOdeManager::addGeom(CIrrOdeGeom *pGeom) {
  m_pGeoms.push_back(pGeom);
}

list<CIrrOdeSceneNode *> &CIrrOdeManager::getIrrOdeNodes() {
  return m_pSceneNodes;
}

void CIrrOdeManager::addSurfaceParameter(CIrrOdeSurfaceParameters *pParam) {
  m_lParamList.push_back(pParam);
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

} //namespace ode
} //namespace irr
