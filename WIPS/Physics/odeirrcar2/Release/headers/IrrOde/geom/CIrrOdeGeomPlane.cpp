  #include <geom/CIrrOdeGeomPlane.h>
  #include <CIrrOdeSpace.h>
  #include <CIrrOdeWorld.h>
  #include <CIrrOdeSpace.h>
  #include <CIrrOdeBody.h>
  #include <IIrrOdeDevice.h>

namespace irr {
namespace ode {

CIrrOdeGeomPlane::CIrrOdeGeomPlane(ISceneNode *parent,ISceneManager *mgr,s32 id, const vector3df &position,
		                               const vector3df &rotation, const vector3df &scale)
		                               : CIrrOdeGeom(parent,mgr,id,position,rotation,scale) {

  #ifdef _IRREDIT_PLUGIN
    if (m_pMesh) {
	  c8 sFileName[1024];
	  sprintf(sFileName,"%sIrrOdeGeomPlane.png",m_sResources);
      m_cMat.setTexture(0,m_pSceneManager->getVideoDriver()->getTexture(sFileName));
    }
  #endif
}

void CIrrOdeGeomPlane::OnRegisterSceneNode() {
  if (IsVisible) SceneManager->registerNodeForRendering(this);
  ISceneNode::OnRegisterSceneNode();
}

void CIrrOdeGeomPlane::render() {
  CIrrOdeSceneNode::render();
}

void CIrrOdeGeomPlane::initPhysics() {
  m_iGeomId=m_pOdeDevice->geomCreatePlane(m_pSpace->getSpaceId(),m_fA,m_fB,m_fC,m_fD);
  m_pOdeDevice->geomSetData(m_iGeomId,this);

  updateAbsolutePosition();
  m_pSpace=reinterpret_cast<CIrrOdeSpace *>(getAncestorOfType((ESCENE_NODE_TYPE)IRR_ODE_SPACE_ID));
  if (!m_pSpace) m_pSpace=m_pWorld->getSpace();

  #ifdef _TRACE_INIT_PHYSICS
    printf("CIrrOdeGeomPlane::initPhysics\n");
  #endif

  CIrrOdeGeom::initPhysics();
}

s32 CIrrOdeGeomPlane::getID() const {
  return ID;
}

ESCENE_NODE_TYPE CIrrOdeGeomPlane::getType() const {
  return (ESCENE_NODE_TYPE )IRR_ODE_GEOM_PLANE_ID;
}

const wchar_t *CIrrOdeGeomPlane::getTypeName() {
  return IRR_ODE_GEOM_PLANE_NAME;
}

void CIrrOdeGeomPlane::setMassTotal(f32 fMass) {
}

void CIrrOdeGeomPlane::serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options) const {
  CIrrOdeGeom::serializeAttributes(out,options);

  out->addFloat("paramA",m_fA);
  out->addFloat("paramB",m_fB);
  out->addFloat("paramC",m_fC);
  out->addFloat("paramD",m_fD);
}

void CIrrOdeGeomPlane::deserializeAttributes(IAttributes* in, SAttributeReadWriteOptions* options) {
  CIrrOdeGeom::deserializeAttributes(in,options);

  m_fA=in->getAttributeAsFloat("paramA");
  m_fB=in->getAttributeAsFloat("paramB");
  m_fC=in->getAttributeAsFloat("paramC");
  m_fD=in->getAttributeAsFloat("paramD");
}

ISceneNode *CIrrOdeGeomPlane::clone(ISceneNode* newParent, ISceneManager* newManager) {
  CIrrOdeGeomPlane *pRet=new CIrrOdeGeomPlane(newParent?newParent:getParent(),newManager?newManager:m_pSceneManager);
  copyParams(pRet);
  CIrrOdeSceneNode::cloneChildren(pRet,newManager);
  return pRet;
}

void CIrrOdeGeomPlane::setParams(f32 fA, f32 fB, f32 fC, f32 fD) {
  m_fA=fA;
  m_fB=fB;
  m_fC=fC;
  m_fD=fD;
}

void CIrrOdeGeomPlane::copyParams(CIrrOdeSceneNode *pDest, bool bRecurse) {
  if (bRecurse) CIrrOdeGeom::copyParams(pDest);
  CIrrOdeGeomPlane *pDst=(CIrrOdeGeomPlane *)pDest;
  pDst->setParams(m_fA,m_fB,m_fC,m_fD);
}

} //namespace ode
} //namespace irr
