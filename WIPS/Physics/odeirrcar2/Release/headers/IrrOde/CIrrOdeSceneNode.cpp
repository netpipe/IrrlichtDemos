#include <CIrrOdeSceneNode.h>
#include <CIrrOdeManager.h>
#include <CIrrOdeWorld.h>

#ifdef _IRREDIT_PLUGIN
  #include <windows.h>
#endif

namespace irr {
namespace ode {

CIrrOdeSceneNode::CIrrOdeSceneNode(ISceneNode *parent,ISceneManager *mgr,s32 id,
                                   const vector3df &position, const vector3df &rotation, const vector3df &scale) :
                                   ISceneNode(parent, mgr, id, position, rotation, scale) {

  m_cMat.setFlag(EMF_LIGHTING,false);

  m_pSceneManager=mgr;
  m_pVideoDriver=mgr->getVideoDriver();
  m_bPhysicsInitialized=false;
  m_sOdeClassName="";
  m_pSelector=NULL;
  m_pOdeManager=CIrrOdeManager::getSharedInstance();
  m_pOdeDevice=m_pOdeManager->getOdeDevice();

  CIrrOdeWorld *pWorld=reinterpret_cast<CIrrOdeWorld *>(getAncestorOfType((ESCENE_NODE_TYPE)IRR_ODE_WORLD_ID));
  if (pWorld || this->getType()==(ESCENE_NODE_TYPE)IRR_ODE_WORLD_ID) m_pOdeManager->addOdeSceneNode(this);

  m_iIdent=-1;
  m_pUserData=NULL;
  #ifdef _IRREDIT_PLUGIN
    m_bDrawEditorMesh=true;
  	GetModuleFileName(NULL,m_sResources,1024);
	  if (strstr(m_sResources,"irrEdit.exe")) *strstr(m_sResources,"irrEdit.exe")='\0';
  	strcat(m_sResources,"resources\\");
	  c8 sFileName[1024];
	  sprintf(sFileName,"%sIrrOde.3ds",m_sResources);
    m_pMesh=mgr->getMesh(sFileName);

    this->setID(m_pOdeManager->getNextId());

    if (pWorld!=NULL) {
      pWorld->addOdeChildNode(this);
    }
  #endif
}

CIrrOdeSceneNode::~CIrrOdeSceneNode() {
  m_pOdeManager->removeOdeSceneNode(this);
}

/**
 * Retrieve the scene node's bounding box
 * @return the scene node's bounding box
 */
const aabbox3df &CIrrOdeSceneNode::getBoundingBox() const {
  return m_cBoundingBox;
}

/**
 * Retreive the first ancestor of a given node type
 * @param iType the node type of the ancestor
 * @return the first ancestor of the given type or "NULL" if no node of the type was found
 */
ISceneNode *CIrrOdeSceneNode::getAncestorOfType(ESCENE_NODE_TYPE iType) {
  ISceneNode *pNode=this->getParent();
  while (pNode!=NULL && pNode->getType()!=iType) pNode=pNode->getParent();
  return pNode;
}

/**
 * Is the physics already initialized?
 * @return "true" if initPhysics was already called, "false" otherwise
 */
bool CIrrOdeSceneNode::physicsInitialized() {
  return m_bPhysicsInitialized;
}

/**
 * convert a wchar_t string to a c8 string
 * @param sName the wchar_t string
 * @return the c8 string
 */
const c8 *CIrrOdeSceneNode::nodeNameToC8(const wchar_t *sName) {
  static stringc sC8Name;
  sC8Name=stringc(sName);
  return sC8Name.c_str();
}

void CIrrOdeSceneNode::serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options) const {
  ISceneNode::serializeAttributes(out,options);
  out->addString("OdeClassName",m_sOdeClassName.c_str());
  out->addInt("Ident",m_iIdent);
}

void CIrrOdeSceneNode::deserializeAttributes(IAttributes* in, SAttributeReadWriteOptions* options) {
  ISceneNode::deserializeAttributes(in,options);
  m_sOdeClassName=in->getAttributeAsString("OdeClassName");
  m_iIdent=in->getAttributeAsInt("Ident");
  m_pOdeManager->updateNextId(getID());
}

/**
 * Get the irrOde classname of this node
 * @return the ODE classname of this node
 */
stringc CIrrOdeSceneNode::getOdeClassname() {
  return m_sOdeClassName;
}

/**
 * Change the irrOde classname of this node
 * @param sClassName the new ODE classname of this node
 */
void CIrrOdeSceneNode::setOdeClassname(stringc sClassName) {
  m_sOdeClassName=sClassName;
}

/**
 * Get the irrOde identifier
 * @return the irrOde identifier
 */
int CIrrOdeSceneNode::getIdent() {
  return m_iIdent;
}

/**
 * Set the irrOde identifier
 * @return the irrOde identifier
 */
void CIrrOdeSceneNode::setIdent(int iIdent) {
  m_iIdent=iIdent;
}

u32 CIrrOdeSceneNode::getMaterialCount() {
  return 1;
}

SMaterial &CIrrOdeSceneNode::getMaterial(u32 iIdx) {
  return m_cMat;
}

void CIrrOdeSceneNode::render() {
  #ifdef _IRREDIT_PLUGIN
    if (m_pMesh && m_pMesh->getMesh(0) && m_pMesh->getMesh(0)->getMeshBuffer(0) && m_bDrawEditorMesh) {
	  matrix4 tmpMatrix;
	  tmpMatrix.setTranslation(AbsoluteTransformation.getTranslation());
      m_pVideoDriver->setMaterial(m_cMat);
      m_pVideoDriver->setTransform(video::ETS_WORLD,tmpMatrix);
      m_pSceneManager->getVideoDriver()->drawMeshBuffer(m_pMesh->getMesh(0)->getMeshBuffer(0));
    }
  #endif
}

/**
 * Copy the parameters to another IrrOde scene node
 * @param pDest the receiver of the parameters
 * @param bRecurse switch to tell the node wether or not the copying should be continued in the object hierarchy
 */
void CIrrOdeSceneNode::copyParams(CIrrOdeSceneNode *pDest, bool bRecurse) {
  pDest->setOdeClassname(m_sOdeClassName);
  pDest->setPosition(getPosition());
  pDest->setScale(getScale());
  pDest->setRotation(getRotation());
}

void CIrrOdeSceneNode::addChild (ISceneNode *child) {
  ISceneNode::addChild(child);
  vector3df pos=child->getPosition();
  m_cBoundingBox.addInternalPoint(pos);
}

void CIrrOdeSceneNode::remove() {
  ISceneNode::remove();
  if (this->getReferenceCount()==1) removeFromPhysics();
  m_pOdeManager->removeOdeSceneNode(this);
}

ISceneManager *CIrrOdeSceneNode::getSceneManager() {
  return m_pSceneManager;
}

ITriangleSelector *CIrrOdeSceneNode::getTriangleSelector() {
  return m_pSelector;
}

void CIrrOdeSceneNode::setUserData(void *p) {
  m_pUserData=p;
}

void *CIrrOdeSceneNode::getUserData() {
  return m_pUserData;
}

#ifdef _IRREDIT_PLUGIN
  void CIrrOdeSceneNode::setDrawEditorMesh(bool b) { m_bDrawEditorMesh=b; }
#endif

void CIrrOdeSceneNode::setParent(ISceneNode *newParent) {
  CIrrOdeWorld *pWorld=reinterpret_cast<CIrrOdeWorld *>(getAncestorOfType((ESCENE_NODE_TYPE)IRR_ODE_WORLD_ID));
  #ifdef _IRREDIT_PLUGIN
    if (pWorld!=NULL) {
      pWorld->removeOdeChildNode(this);
    }
  #endif

  m_pOdeManager->removeOdeSceneNode(this);

  ISceneNode::setParent(newParent);

  pWorld=reinterpret_cast<CIrrOdeWorld *>(getAncestorOfType((ESCENE_NODE_TYPE)IRR_ODE_WORLD_ID));
  if (pWorld!=NULL || this->getType()==(ESCENE_NODE_TYPE)IRR_ODE_WORLD_ID) m_pOdeManager->addOdeSceneNode(this);

  #ifdef _IRREDIT_PLUGIN
    if (pWorld!=NULL) {
      pWorld->addOdeChildNode(this);
    }
  #endif
}

void CIrrOdeSceneNode::initPhysics() {
  m_bPhysicsInitialized=true;
  CIrrOdeManager::getSharedInstance()->sceneNodeInitialized(this);
}

void CIrrOdeSceneNode::removeFromPhysics() {
  m_pOdeManager->removeOdeSceneNode(this);
}

void CIrrOdeSceneNode::cloneChildren(ISceneNode *pNewParent, ISceneManager *pNewManager) {
  list<ISceneNode *> children=getChildren();
  list<ISceneNode *>::Iterator it;

  for (it=children.begin(); it!=children.end(); it++)
    (*it)->clone(pNewParent,pNewManager);
}

} //namespace ode
} //namespace irr
