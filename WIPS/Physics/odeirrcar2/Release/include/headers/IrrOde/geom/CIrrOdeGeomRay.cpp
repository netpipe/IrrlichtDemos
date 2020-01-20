  #include <geom/CIrrOdeGeomRay.h>
  #include <CIrrOdeSpace.h>
  #include <CIrrOdeWorld.h>
  #include <CIrrOdeSpace.h>
  #include <CIrrOdeBody.h>
  #include <IIrrOdeDevice.h>

namespace irr {
namespace ode {

CIrrOdeGeomRay::CIrrOdeGeomRay(ISceneNode *parent,ISceneManager *mgr,s32 id,
                               const vector3df &position, const vector3df &rotation, const vector3df &scale) :
                               CIrrOdeGeom(parent, mgr, id, position, rotation, scale) {

  #ifdef _TRACE_CONSTRUCTOR_DESTRUCTOR
    printf("CIrrOdeGeomRay constructor\n");
  #endif

  m_iGeomId=0;
  m_fLength=10.0f;
  m_pBody=NULL;

  #ifdef _IRREDIT_PLUGIN
    if (m_pMesh) {
	  c8 sFileName[1024];
	  sprintf(sFileName,"%sIrrOdeGeomRay.png",m_sResources);
      m_cMat.setTexture(0,m_pSceneManager->getVideoDriver()->getTexture(sFileName));
    }
  #endif
}

CIrrOdeGeomRay::~CIrrOdeGeomRay() {
  #ifdef _TRACE_CONSTRUCTOR_DESTRUCTOR
    printf("CIrrOdeGeomRay destructor\n");
  #endif
}

void CIrrOdeGeomRay::OnRegisterSceneNode() {
  if (IsVisible) SceneManager->registerNodeForRendering(this);
  ISceneNode::OnRegisterSceneNode();
}

void CIrrOdeGeomRay::render() {
  CIrrOdeSceneNode::render();
}

void CIrrOdeGeomRay::set(vector3df pos, vector3df dir, f32 fLength) {
  m_pPos=pos;
  m_pDir=dir;
  m_fLength=fLength;

  if (m_iGeomId) m_pOdeDevice->geomRaySetData(m_iGeomId,m_pPos,m_pDir,m_fLength);
}

void CIrrOdeGeomRay::initPhysics() {
  if (m_bPhysicsInitialized) return;

  updateAbsolutePosition();
  m_pSpace=reinterpret_cast<CIrrOdeSpace *>(getAncestorOfType((ESCENE_NODE_TYPE)IRR_ODE_SPACE_ID));
  if (!m_pSpace) m_pSpace=m_pWorld->getSpace();

  m_iGeomId=m_pOdeDevice->geomCreateRay(m_pSpace->getSpaceId(),m_fLength);

  #ifdef _TRACE_INIT_PHYSICS
    printf("CIrrOdeGeomRay::initPhysics: %i\n",(int)m_iGeomId);
  #endif

  m_pOdeDevice->geomSetData(m_iGeomId,this);
  m_pOdeDevice->geomSetBackfaceCull(m_iGeomId,m_bBackfaceCull);
  CIrrOdeSceneNode::initPhysics();
}

s32 CIrrOdeGeomRay::getID() const {
  return ID;
}

ESCENE_NODE_TYPE CIrrOdeGeomRay::getType() const {
  return (ESCENE_NODE_TYPE)IRR_ODE_GEOM_RAY_ID;
}

void CIrrOdeGeomRay::bodyHit(CIrrOdeBody *pBody) {
  #ifdef _TRACE_RAY
    printf("body hit!\n");
  #endif
}

const wchar_t *CIrrOdeGeomRay::getTypeName() {
  return IRR_ODE_GEOM_RAY_NAME;
}

void CIrrOdeGeomRay::setMassTotal(f32 fMass) {
}

void CIrrOdeGeomRay::serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options) const {
  CIrrOdeGeom::serializeAttributes(out,options);

  out->addFloat("Length",m_fLength);

  out->addVector3d("Position" ,m_pPos);
  out->addVector3d("Direction",m_pDir);
}

void CIrrOdeGeomRay::deserializeAttributes(IAttributes* in, SAttributeReadWriteOptions* options) {
  CIrrOdeGeom::deserializeAttributes(in,options);

  m_fLength=in->getAttributeAsFloat("Length");

  m_pPos=in->getAttributeAsVector3d("Position" );
  m_pDir=in->getAttributeAsVector3d("Direction");
}

ISceneNode *CIrrOdeGeomRay::clone(ISceneNode* newParent, ISceneManager* newManager) {
  CIrrOdeGeomRay *pRet=new CIrrOdeGeomRay(newParent?newParent:getParent(),newManager?newManager:m_pSceneManager);
  copyParams(pRet);
  CIrrOdeSceneNode::cloneChildren(pRet,newManager);
  return pRet;
}

void CIrrOdeGeomRay::copyParams(CIrrOdeSceneNode *pDest, bool bRecurse) {
  if (bRecurse) CIrrOdeGeom::copyParams(pDest);
  CIrrOdeGeomRay *pDst=(CIrrOdeGeomRay *)pDest;
  pDst->set(m_pPos,m_pDir,m_fLength);
}

void CIrrOdeGeomRay::setBackfaceCull(bool b) {
  m_bBackfaceCull=b;
  if (m_iGeomId) m_pOdeDevice->geomSetBackfaceCull(m_iGeomId,b);
}

bool CIrrOdeGeomRay::getBackfaceCull() {
  return m_bBackfaceCull;
}

void CIrrOdeGeomRay::setAttachedBody(CIrrOdeBody *pBody) {
  m_pBody=pBody;
}

CIrrOdeBody *CIrrOdeGeomRay::getAttachedBody() {
  return m_pBody;
}

} //namespace ode
} //namespace irr
