  #include <geom/CIrrOdeGeomHeightfield.h>
  #include <CIrrOdeSpace.h>
  #include <CIrrOdeWorld.h>
  #include <CIrrOdeSpace.h>
  #include <CIrrOdeBody.h>
  #include <IIrrOdeDevice.h>

namespace irr {
namespace ode {

CIrrOdeGeomHeightfield::CIrrOdeGeomHeightfield(ISceneNode *parent,ISceneManager *mgr,s32 id,
                                       const vector3df &position, const vector3df &rotation, const vector3df &scale) :
                                       CIrrOdeGeom(parent, mgr, id, position, rotation, scale) {

  #ifdef _TRACE_CONSTRUCTOR_DESTRUCTOR
    printf("CIrrOdeGeomHeightfield constructor\n");
  #endif

  m_iWidthSamples=256;
  m_iDepthSamples=256;
  m_fThickness=1.0f;

  #ifdef _IRREDIT_PLUGIN
    if (m_pMesh) {
	  c8 sFileName[1024];
	  sprintf(sFileName,"%sIrrOdeGeomHeightfield.png",m_sResources);
      m_cMat.setTexture(0,m_pSceneManager->getVideoDriver()->getTexture(sFileName));
    }
  #endif
}

CIrrOdeGeomHeightfield::~CIrrOdeGeomHeightfield() {
  #ifdef _TRACE_CONSTRUCTOR_DESTRUCTOR
    printf("CIrrOdeGeomHeightfield destructor\n");
  #endif
}

void CIrrOdeGeomHeightfield::OnRegisterSceneNode() {
  if (IsVisible) SceneManager->registerNodeForRendering(this);
  ISceneNode::OnRegisterSceneNode();
}

void CIrrOdeGeomHeightfield::render() {
  CIrrOdeSceneNode::render();
}

f32 CIrrOdeGeomHeightfield::getHeight(s32 x, s32 z) {
  ITerrainSceneNode *pParentNode=reinterpret_cast<ITerrainSceneNode *>(getParent());
  return pParentNode->getHeight((f32)x,(f32)z);
}

f32 CIrrOdeGeomHeightfield::getHeightCallback(void *data, s32 x, s32 z) {
  CIrrOdeGeomHeightfield *pGeom=(CIrrOdeGeomHeightfield *)data;
  f32 px=x*(pGeom->m_fWidth/(pGeom->getWidthSamples()-1))+pGeom->getParent()->getPosition().X,
      pz=z*(pGeom->m_fDepth/(pGeom->getDepthSamples()-1))+pGeom->getParent()->getPosition().Z;
  f32 fRet=pGeom->getHeight((s32)px,(s32)pz)-pGeom->getParent()->getPosition().Y;
  return fRet;
}

void CIrrOdeGeomHeightfield::initPhysics() {
  if (m_bPhysicsInitialized) return;

  updateAbsolutePosition();
  m_cBoundingBox.reset(getAbsolutePosition());
  m_pSpace=reinterpret_cast<CIrrOdeSpace *>(getAncestorOfType((ESCENE_NODE_TYPE)IRR_ODE_SPACE_ID));
  if (!m_pSpace) m_pSpace=m_pWorld->getSpace();

  ITerrainSceneNode *pParentNode=reinterpret_cast<ITerrainSceneNode *>(getParent());

  m_fWidth=pParentNode->getBoundingBox().MaxEdge.X-pParentNode->getBoundingBox().MinEdge.X;
  m_fDepth=pParentNode->getBoundingBox().MaxEdge.Z-pParentNode->getBoundingBox().MinEdge.Z;

  //try to automagically calculate the width and depth of the heightmap from the vertex count
  CDynamicMeshBuffer tmpMB(video::EVT_2TCOORDS, video::EIT_16BIT);
  pParentNode->getMeshBufferForLOD(tmpMB);
  u32 vc = tmpMB.getVertexCount();
  m_iWidthSamples = m_iDepthSamples = ((u32)sqrt((double)vc));
  #ifdef _TRACE_INIT_PHYSICS
    printf("Terrain heightfield samples:%d \n", m_iDepthSamples);
  #endif

  m_iDataId=m_pOdeDevice->geomHeightfieldDataCreate();
  m_pOdeDevice->geomHeightfieldDataCallback(m_iDataId,this,(void *)(CIrrOdeGeomHeightfield::getHeightCallback),m_fWidth,m_fDepth,m_iWidthSamples,m_iDepthSamples,1.0f,0.0f,m_fThickness,0);
  m_pOdeDevice->geomHeightfieldSetBounds(m_iDataId,-1000.0f,1000.0f);
  m_iGeomId=m_pOdeDevice->geomCreateHeightfield(m_pSpace->getSpaceId(),m_iDataId);
  pParentNode->updateAbsolutePosition();
  vector3df pos=pParentNode->getPosition();

  pos.X+=m_fWidth/2;
  pos.Z+=m_fDepth/2;

  m_pOdeDevice->geomSetPosition(m_iGeomId,pos);
  m_pOdeDevice->geomSetBody(m_iGeomId,NULL);
  m_pOdeDevice->geomSetData(m_iGeomId,this);
  #ifdef _TRACE_INIT_PHYSICS
    printf("CIrrOdeGeomHeightfield::initPhysics: terrain size: (%.2f, %.2f)\n",m_fWidth,m_fDepth);
    printf("CIrrOdeGeomHeightfield::initPhysics: terrain position: (%.2f, %.2f, %.2f)\n",pos.X,pos.Y,pos.Z);
  #endif

  CIrrOdeGeom::initPhysics();
}

s32 CIrrOdeGeomHeightfield::getID() const {
  return ID;
}

ESCENE_NODE_TYPE CIrrOdeGeomHeightfield::getType() const {
  return (ESCENE_NODE_TYPE)IRR_ODE_GEOM_HEIGHTFIELD_ID;
}

const wchar_t *CIrrOdeGeomHeightfield::getTypeName() {
  return IRR_ODE_GEOM_HEIGHTFIELD_NAME;
}

void CIrrOdeGeomHeightfield::setMassTotal(f32 fMass) {
}

void CIrrOdeGeomHeightfield::serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options) const {
  CIrrOdeGeom::serializeAttributes(out,options);

  out->addFloat("Width"    ,m_fWidth    );
  out->addFloat("Depth"    ,m_fDepth    );
  out->addFloat("Thickness",m_fThickness);

  out->addInt("WidthSamples",m_iWidthSamples);
  out->addInt("DepthSamples",m_iDepthSamples);
}

void CIrrOdeGeomHeightfield::deserializeAttributes(IAttributes* in, SAttributeReadWriteOptions* options) {
  CIrrOdeGeom::deserializeAttributes(in,options);

  m_fWidth    =in->getAttributeAsFloat("Width"    );
  m_fDepth    =in->getAttributeAsFloat("Depth"    );
  m_fThickness=in->getAttributeAsFloat("Thickness");

  m_iWidthSamples=in->getAttributeAsInt("WidthSamples");
  m_iDepthSamples=in->getAttributeAsInt("DepthSamples");
}

void CIrrOdeGeomHeightfield::setWidth(f32 fWidth) {
  m_fWidth=fWidth;
}

void CIrrOdeGeomHeightfield::setDepth(f32 fDepth) {
  m_fDepth=fDepth;
}

void CIrrOdeGeomHeightfield::setThickness(f32 fThickness) {
  m_fThickness=fThickness;
}

void CIrrOdeGeomHeightfield::setWidthSamples(u32 iWidthSamples) {
  m_iWidthSamples=iWidthSamples;
}

void CIrrOdeGeomHeightfield::setDepthSamples(u32 iDepthSamples) {
  m_iDepthSamples=iDepthSamples;
}

ISceneNode *CIrrOdeGeomHeightfield::clone(ISceneNode* newParent, ISceneManager* newManager) {
  CIrrOdeGeomHeightfield *pRet=new CIrrOdeGeomHeightfield(newParent?newParent:getParent(),newManager?newManager:m_pSceneManager);
  copyParams(pRet);
  CIrrOdeSceneNode::cloneChildren(pRet,newManager);
  return pRet;
}

void CIrrOdeGeomHeightfield::copyParams(CIrrOdeSceneNode *pDest, bool bRecurse) {
  if (bRecurse) CIrrOdeGeom::copyParams(pDest);
  CIrrOdeGeomHeightfield *pDst=(CIrrOdeGeomHeightfield *)pDest;
  pDst->setWidth(m_fWidth);
  pDst->setDepth(m_fDepth);
  pDst->setThickness(m_fThickness);
  pDst->setWidthSamples(m_iWidthSamples);
  pDst->setDepthSamples(m_iDepthSamples);
}

u32 CIrrOdeGeomHeightfield::getWidthSamples() { return m_iWidthSamples; }
u32 CIrrOdeGeomHeightfield::getDepthSamples() { return m_iDepthSamples; }

} //namespace ode
} //namespace irr
