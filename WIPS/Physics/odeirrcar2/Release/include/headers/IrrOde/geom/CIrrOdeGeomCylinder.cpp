  #include <geom/CIrrOdeGeomCylinder.h>
  #include <CIrrOdeSpace.h>
  #include <CIrrOdeWorld.h>
  #include <CIrrOdeSpace.h>
  #include <CIrrOdeBody.h>
  #include <IIrrOdeDevice.h>

namespace irr {
namespace ode {

CIrrOdeGeomCylinder::CIrrOdeGeomCylinder(ISceneNode *parent,ISceneManager *mgr,s32 id,
                               const vector3df &position, const vector3df &rotation, const vector3df &scale) :
                               CIrrOdeGeom(parent, mgr, id, position, rotation, scale) {

  #ifdef _TRACE_CONSTRUCTOR_DESTRUCTOR
    printf("CIrrOdeGeomCylinder constructor\n");
  #endif

  m_fMass=1.0f;
  m_iGeomId=0;
  m_fRadius=0.0f;
  m_fLength=0.0f;

  #ifdef _IRREDIT_PLUGIN
    if (m_pMesh) {
	  c8 sFileName[1024];
	  sprintf(sFileName,"%sIrrOdeGeomCylinder.png",m_sResources);
      m_cMat.setTexture(0,m_pSceneManager->getVideoDriver()->getTexture(sFileName));
    }
  #endif
}

CIrrOdeGeomCylinder::~CIrrOdeGeomCylinder() {
  #ifdef _TRACE_CONSTRUCTOR_DESTRUCTOR
    printf("CIrrOdeGeomCylinder destructor\n");
  #endif
}

void CIrrOdeGeomCylinder::OnRegisterSceneNode() {
  if (IsVisible) SceneManager->registerNodeForRendering(this);
  ISceneNode::OnRegisterSceneNode();
}

void CIrrOdeGeomCylinder::render() {
  CIrrOdeSceneNode::render();
  #ifdef _DRAW_BOUNDING_BOXES
    m_pVideoDriver->setTransform(video::ETS_WORLD, AbsoluteTransformation);
    m_pVideoDriver->draw3DBox(m_cBoundingBox,SColor(100,0xFF,0,0));
  #endif
}

void CIrrOdeGeomCylinder::initPhysics() {
  if (m_bPhysicsInitialized) return;

  updateAbsolutePosition();
  m_pSpace=reinterpret_cast<CIrrOdeSpace *>(getAncestorOfType((ESCENE_NODE_TYPE)IRR_ODE_SPACE_ID));
  if (!m_pSpace) m_pSpace=m_pWorld->getSpace();

  if (m_fRadius==0.0f && m_fLength==0.0f) {
    IAnimatedMeshSceneNode *pParent=reinterpret_cast<IAnimatedMeshSceneNode *>(getParent());
    IMesh *pMesh=pParent->getMesh()->getMesh(0);

    m_fRadius=pMesh->getBoundingBox().getExtent().X*getParent()->getScale().X;
    m_fLength=pMesh->getBoundingBox().getExtent().Z*getParent()->getScale().Z;

    #ifdef _TRACE_INIT_PHYSICS
      printf("CIrrOdeGeomCylinder::initPhysics: getting size from parent node\n");
      printf("CIrrOdeGeomCylinder::initPhysics: radius=%.2f, length=%.2f\n",m_fRadius,m_fLength);
    #endif
  }

  m_iGeomId=m_pOdeDevice->geomCreateCylinder(m_pSpace->getSpaceId(),m_fRadius,m_fLength);

  if (m_iGeomId) {
    m_pOdeDevice->geomSetPosition(m_iGeomId,getAbsolutePosition());

    if (m_pBody) {
      if (m_fMass) {
        m_pOdeDevice->massSetZero(m_iMass);
        m_pOdeDevice->massSetCylinderTotal(m_iMass,m_fMass,3,m_fRadius,m_fLength);
        m_pBody->addMass(m_iMass);
        //m_pOdeDevice->bodySetMass(m_pBody->getBodyId(),m_iMass);
      }
      setBody(m_pBody);
    }
  }
  #ifdef _TRACE_INIT_PHYSICS
    printf("CIrrOdeGeomCylinder::initPhysics: %i\n",(int)m_iGeomId);
    printf("CIrrOdeGeomCylinder extent: %.2f, %.2f\n",m_fRadius,m_fLength);
    printf("CIrrOdeGeomCylinder mass: %.2f\n",m_fMass);
  #endif

  #ifdef _DRAW_BOUNDING_BOXES
    m_cBoundingBox=getParent()->getBoundingBox();
  #endif

  m_pOdeDevice->geomSetData(m_iGeomId,this);
  CIrrOdeGeom::initPhysics();
}

s32 CIrrOdeGeomCylinder::getID() const {
  return ID;
}

ESCENE_NODE_TYPE CIrrOdeGeomCylinder::getType() const {
  return (ESCENE_NODE_TYPE)IRR_ODE_GEOM_CYLINDER_ID;
}

const wchar_t *CIrrOdeGeomCylinder::getTypeName() {
  return IRR_ODE_GEOM_CYLINDER_NAME;
}

void CIrrOdeGeomCylinder::setMassTotal(f32 fMass) {
  m_fMass=fMass;
}

void CIrrOdeGeomCylinder::setRadius(f32 fRadius) {
  m_fRadius=fRadius;
}

void CIrrOdeGeomCylinder::setLength(f32 fLength) {
  m_fLength=fLength;
}

void CIrrOdeGeomCylinder::serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options) const {
  CIrrOdeGeom::serializeAttributes(out,options);

  out->addFloat("Radius",m_fRadius);
  out->addFloat("Length",m_fLength);
}

void CIrrOdeGeomCylinder::deserializeAttributes(IAttributes* in, SAttributeReadWriteOptions* options) {
  CIrrOdeGeom::deserializeAttributes(in,options);

  m_fRadius=in->getAttributeAsFloat("Radius");
  m_fLength=in->getAttributeAsFloat("Length");
}

ISceneNode *CIrrOdeGeomCylinder::clone(ISceneNode* newParent, ISceneManager* newManager) {
  CIrrOdeGeomCylinder *pRet=new CIrrOdeGeomCylinder(newParent?newParent:getParent(),newManager?newManager:m_pSceneManager);
  copyParams(pRet);
  CIrrOdeSceneNode::cloneChildren(pRet,newManager);
  return pRet;
}

void CIrrOdeGeomCylinder::copyParams(CIrrOdeSceneNode *pDest, bool bRecurse) {
  if (bRecurse) CIrrOdeGeom::copyParams(pDest);
  CIrrOdeGeomCylinder *pDst=(CIrrOdeGeomCylinder *)pDest;
  pDst->setRadius(m_fRadius);
  pDst->setLength(m_fLength);
}

} //namespace ode
} //namespace irr
