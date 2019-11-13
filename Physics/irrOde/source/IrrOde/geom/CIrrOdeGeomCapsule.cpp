  #include <geom/CIrrOdeGeomCapsule.h>
  #include <CIrrOdeSpace.h>
  #include <CIrrOdeWorld.h>
  #include <CIrrOdeSpace.h>
  #include <CIrrOdeBody.h>
  #include <IIrrOdeDevice.h>

namespace irr {
namespace ode {

CIrrOdeGeomCapsule::CIrrOdeGeomCapsule(ISceneNode *parent,ISceneManager *mgr,s32 id,
                               const vector3df &position, const vector3df &rotation, const vector3df &scale) :
                               CIrrOdeGeom(parent, mgr, id, position, rotation, scale) {

  #ifdef _TRACE_CONSTRUCTOR_DESTRUCTOR
    printf("CIrrOdeGeomCapsule constructor\n");
  #endif

  m_fMass=1.0f;
  m_iGeomId=0;
  m_fRadius=0.0f;
  m_fLength=0.0f;

  #ifdef _IRREDIT_PLUGIN
    if (m_pMesh) {
	  c8 sFileName[1024];
	  sprintf(sFileName,"%sIrrOdeGeomCapsule.png",m_sResources);
      m_cMat.setTexture(0,m_pSceneManager->getVideoDriver()->getTexture(sFileName));
    }
  #endif
}

CIrrOdeGeomCapsule::~CIrrOdeGeomCapsule() {
  #ifdef _TRACE_CONSTRUCTOR_DESTRUCTOR
    printf("CIrrOdeGeomCapsule destructor\n");
  #endif
}

void CIrrOdeGeomCapsule::OnRegisterSceneNode() {
  if (IsVisible) SceneManager->registerNodeForRendering(this);
  ISceneNode::OnRegisterSceneNode();
}

void CIrrOdeGeomCapsule::render() {
  CIrrOdeSceneNode::render();
  #ifdef _DRAW_BOUNDING_BOXES
    m_pVideoDriver->setTransform(video::ETS_WORLD, AbsoluteTransformation);
    m_pVideoDriver->draw3DBox(m_cBoundingBox,SColor(100,0xFF,0,0));
  #endif
}

void CIrrOdeGeomCapsule::initPhysics() {
  if (m_bPhysicsInitialized) return;

  updateAbsolutePosition();
  m_pSpace=reinterpret_cast<CIrrOdeSpace *>(getAncestorOfType((ESCENE_NODE_TYPE)IRR_ODE_SPACE_ID));
  if (!m_pSpace) m_pSpace=m_pWorld->getSpace();

  if (m_fRadius==0.0f && m_fLength==0.0f) {
    IAnimatedMeshSceneNode *pParent=reinterpret_cast<IAnimatedMeshSceneNode *>(getParent());
    IMesh *pMesh=pParent->getMesh()->getMesh(0);

    m_fRadius=pMesh->getBoundingBox().getExtent().X*getParent()->getScale().X;
    m_fLength=pMesh->getBoundingBox().getExtent().Z*getParent()->getScale().Z-2*m_fRadius;

    #ifdef _TRACE_INIT_PHYSICS
      printf("CIrrOdeGeomCapsule::initPhysics: getting size from parent node\n");
      printf("CIrrOdeGeomCapsule::initPhysics: radius=%.2f, length=%.2f\n",m_fRadius,m_fLength);
    #endif
  }

  m_iGeomId=m_pOdeDevice->geomCreateCapsule(m_pSpace->getSpaceId(),m_fRadius,m_fLength);

  if (m_iGeomId) {
    m_pOdeDevice->geomSetPosition(m_iGeomId,getAbsolutePosition());

    if (m_pBody) {
      if (m_fMass) {
        m_pOdeDevice->massSetZero(m_iMass);
        m_pOdeDevice->massSetCapsuleTotal(m_iMass,m_fMass,3,m_fRadius,m_fLength);
        m_pBody->addMass(m_iMass);
        //m_pOdeDevice->bodySetMass(m_pBody->getBodyId(),m_iMass);
      }
      setBody(m_pBody);
    }
  }
  #ifdef _TRACE_INIT_PHYSICS
    printf("CIrrOdeGeomCapsule::initPhysics: %i\n",(int)m_iGeomId);
    printf("CIrrOdeGeomCapsule extent: %.2f, %.2f\n",m_fRadius,m_fLength);
    printf("CIrrOdeGeomCapsule mass: %.2f\n",m_fMass);
  #endif

  #ifdef _DRAW_BOUNDING_BOXES
    m_cBoundingBox=getParent()->getBoundingBox();
  #endif

  m_pOdeDevice->geomSetData(m_iGeomId,this);
  CIrrOdeGeom::initPhysics();
  m_bPhysicsInitialized=true;
}

s32 CIrrOdeGeomCapsule::getID() const {
  return ID;
}

ESCENE_NODE_TYPE CIrrOdeGeomCapsule::getType() const {
  return (ESCENE_NODE_TYPE)IRR_ODE_GEOM_CAPSULE_ID;
}

const wchar_t *CIrrOdeGeomCapsule::getTypeName() {
  return IRR_ODE_GEOM_CAPSULE_NAME;
}

void CIrrOdeGeomCapsule::setMassTotal(f32 fMass) {
  m_fMass=fMass;
}

void CIrrOdeGeomCapsule::setRadius(f32 fRadius) {
  m_fRadius=fRadius;
}

void CIrrOdeGeomCapsule::setLength(f32 fLength) {
  m_fLength=fLength;
}

void CIrrOdeGeomCapsule::serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options) const {
  CIrrOdeGeom::serializeAttributes(out,options);

  out->addFloat("Length",m_fLength);
  out->addFloat("Radius",m_fRadius);
}

void CIrrOdeGeomCapsule::deserializeAttributes(IAttributes* in, SAttributeReadWriteOptions* options) {
  CIrrOdeGeom::deserializeAttributes(in,options);

  m_fLength=in->getAttributeAsFloat("Length");
  m_fRadius=in->getAttributeAsFloat("Radius");
}

ISceneNode *CIrrOdeGeomCapsule::clone(ISceneNode* newParent, ISceneManager* newManager) {
  CIrrOdeGeomCapsule *pRet=new CIrrOdeGeomCapsule(newParent?newParent:getParent(),newManager?newManager:m_pSceneManager);
  copyParams(pRet);
  return pRet;
}

void CIrrOdeGeomCapsule::copyParams(CIrrOdeSceneNode *pDest, bool bRecurse) {
  if (bRecurse) CIrrOdeGeom::copyParams(pDest);
  CIrrOdeGeomCapsule *pDst=(CIrrOdeGeomCapsule *)pDest;
  pDst->setRadius(m_fRadius);
  pDst->setLength(m_fLength);
}

} //namespace ode
} //namespace irr
