  #include <irrlicht.h>
  #include <CIrrOdeSpace.h>
  #include <IIrrOdeDevice.h>
  #include <CIrrOdeWorld.h>

namespace irr {
namespace ode {

CIrrOdeSpace::CIrrOdeSpace(ISceneNode *parent,ISceneManager *mgr,s32 id,
                           const vector3df &position, const vector3df &rotation, const vector3df &scale) :
                           CIrrOdeSceneNode(parent, mgr, id, position, rotation, scale) {

  #ifdef _TRACE_CONSTRUCTOR_DESTRUCTOR
    printf("CIrrOdeBody contructor\n");
  #endif

  m_iSpaceId=0;
  m_iType=eIrrOdeSpaceSimple;
  m_pParentSpace=reinterpret_cast<CIrrOdeSpace *>(getAncestorOfType((ESCENE_NODE_TYPE)IRR_ODE_SPACE_ID));
  m_pWorld=reinterpret_cast<CIrrOdeWorld *>(getAncestorOfType((ESCENE_NODE_TYPE)IRR_ODE_WORLD_ID));
  if (m_pWorld) m_pWorld->addSpace(this);

  if (!m_pParentSpace) if (m_pWorld && m_pWorld->getSpace()!=this) m_pParentSpace=m_pWorld->getSpace();

  m_cCenter=vector3df(0,0,0);
  m_cExtents=vector3df(0,0,0);
  m_iDepth=0;

  #ifdef _IRREDIT_PLUGIN
    if (m_pMesh) {
	  c8 sFileName[1024];
	  sprintf(sFileName,"%sIrrOdeSpace.png",m_sResources);
      m_cMat.setTexture(0,m_pSceneManager->getVideoDriver()->getTexture(sFileName));
    }
  #endif
}

CIrrOdeSpace::~CIrrOdeSpace() {
  if (m_iSpaceId) m_pOdeDevice->spaceDestroy(m_iSpaceId);
}

u32 CIrrOdeSpace::getSpaceId() {
  return m_iSpaceId;
}

void CIrrOdeSpace::initPhysics() {
  if (m_bPhysicsInitialized) return;
  CIrrOdeSceneNode::initPhysics();

  switch (m_iType) {
    case eIrrOdeSpaceSimple:
      m_iSpaceId=m_pOdeDevice->spaceCreateSimple(m_pParentSpace?m_pParentSpace->getSpaceId():0);
      break;

    case eIrrOdeSpaceHash:
      m_iSpaceId=m_pOdeDevice->spaceCreateHash(m_pParentSpace?m_pParentSpace->getSpaceId():0);
      break;

    case eIrrOdeSpaceQuadTree:
      m_iSpaceId=m_pOdeDevice->spaceCreateQuadTree(m_pParentSpace?m_pParentSpace->getSpaceId():0,m_cCenter,m_cExtents,m_iDepth);
      break;

    default:
      break;
  }
}

void CIrrOdeSpace::setSpaceType(_IRR_ODE_SPACE_TYPE iType) {
  m_iType=iType;
}

_IRR_ODE_SPACE_TYPE CIrrOdeSpace::getSpaceType() {
  return m_iType;
}

void CIrrOdeSpace::setQuadTreeParams(vector3df cCenter, vector3df cExtents, s32 iDepth){
  m_cCenter=cCenter;
  m_cExtents=cExtents;
  m_iDepth=iDepth;
}

s32 CIrrOdeSpace::getID() {
  return ID;
}

void CIrrOdeSpace::render() {
  CIrrOdeSceneNode::render();
}

void CIrrOdeSpace::OnRegisterSceneNode() {
  if (IsVisible) SceneManager->registerNodeForRendering(this);
  ISceneNode::OnRegisterSceneNode();
}

ESCENE_NODE_TYPE CIrrOdeSpace::getType() const {
  return (ESCENE_NODE_TYPE)IRR_ODE_SPACE_ID;
}

const wchar_t *CIrrOdeSpace::getTypeName() {
  return IRR_ODE_SPACE_NAME;
}

void CIrrOdeSpace::setType(_IRR_ODE_SPACE_TYPE iType) {
  m_iType=iType;
}

_IRR_ODE_SPACE_TYPE CIrrOdeSpace::getType() {
  return m_iType;
}

void CIrrOdeSpace::serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options) const {
  CIrrOdeSceneNode::serializeAttributes(out,options);

  out->addInt("SpaceType",m_iType );
  out->addInt("Depth"    ,m_iDepth);

  out->addVector3d("Center" ,m_cCenter );
  out->addVector3d("Extents",m_cExtents);
}

void CIrrOdeSpace::deserializeAttributes(IAttributes* in, SAttributeReadWriteOptions* options) {
  CIrrOdeSceneNode::deserializeAttributes(in,options);

  m_iType =(_IRR_ODE_SPACE_TYPE)in->getAttributeAsInt("SpaceType");
  m_iDepth=in->getAttributeAsInt("Depth"    );

  m_cCenter =in->getAttributeAsVector3d("Center" );
  m_cExtents=in->getAttributeAsVector3d("Extents");
}

CIrrOdeSpace *CIrrOdeSpace::getParentSpace() {
  return m_pParentSpace;
}

vector3df &CIrrOdeSpace::getCenter() {
  return m_cCenter;
}

vector3df &CIrrOdeSpace::getExtents() {
  return m_cExtents;
}

s32 CIrrOdeSpace::getDepth() {
  return m_iDepth;
}

ISceneNode *CIrrOdeSpace::clone(ISceneNode* newParent, ISceneManager* newManager) {
  CIrrOdeSpace *pRet=new CIrrOdeSpace(newParent?newParent:getParent(),newManager?newManager:m_pSceneManager);
  copyParams(pRet);
  CIrrOdeSceneNode::cloneChildren(pRet,newManager);
  return pRet;
}

void CIrrOdeSpace::copyParams(CIrrOdeSceneNode *pDest, bool bRecurse) {
  #ifdef _TRACE_COPY_PARAMS
    printf("CIrrOdeBody copyParams\n");
  #endif
  if (bRecurse) CIrrOdeSceneNode::copyParams(pDest);

  CIrrOdeSpace *pDst=(CIrrOdeSpace *)pDest;
  m_iSpaceId=0;
  pDst->setSpaceType(m_iType);
  pDst->setQuadTreeParams(m_cCenter,m_cExtents,m_iDepth);
}

void CIrrOdeSpace::removeFromPhysics() {
  CIrrOdeSceneNode::removeFromPhysics();
  if (m_iSpaceId) {
    m_pOdeDevice->spaceDestroy(m_iSpaceId);
    m_iSpaceId=0;
  }
}

} //namespace ode
} //namespace irr
