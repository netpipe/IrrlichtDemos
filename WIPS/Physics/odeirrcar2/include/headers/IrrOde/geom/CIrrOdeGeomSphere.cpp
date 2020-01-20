  #include <geom/CIrrOdeGeomSphere.h>
  #include <CIrrOdeSpace.h>
  #include <CIrrOdeWorld.h>
  #include <CIrrOdeSpace.h>
  #include <CIrrOdeBody.h>
  #include <IIrrOdeDevice.h>

namespace irr {
namespace ode {

CIrrOdeGeomSphere::CIrrOdeGeomSphere(ISceneNode *parent,ISceneManager *mgr,s32 id,
                                     const vector3df &position, const vector3df &rotation, const vector3df &scale) :
                                     CIrrOdeGeom(parent, mgr, id, position, rotation, scale) {

  #ifdef _TRACE_CONSTRUCTOR_DESTRUCTOR
    printf("CIrrOdeGeomSphere constructor\n");
  #endif

  m_fMass=1.0f;
  m_iGeomId=0;
  m_fRadius=0.0f;

  #ifdef _IRREDIT_PLUGIN
    if (m_pMesh) {
	  c8 sFileName[1024];
	  sprintf(sFileName,"%sIrrOdeGeomSphere.png",m_sResources);
      m_cMat.setTexture(0,m_pSceneManager->getVideoDriver()->getTexture(sFileName));
    }
  #endif
}

CIrrOdeGeomSphere::~CIrrOdeGeomSphere() {
  #ifdef _TRACE_CONSTRUCTOR_DESTRUCTOR
    printf("CIrrOdeGeomSphere destructor\n");
  #endif
}

void CIrrOdeGeomSphere::OnRegisterSceneNode() {
  if (IsVisible) SceneManager->registerNodeForRendering(this);
  ISceneNode::OnRegisterSceneNode();
}

void CIrrOdeGeomSphere::render() {
  CIrrOdeSceneNode::render();
  #ifdef _DRAW_BOUNDING_BOXES
    m_pVideoDriver->setTransform(video::ETS_WORLD, AbsoluteTransformation);
    m_pVideoDriver->setMaterial(m_cMat);
    m_pVideoDriver->draw3DBox(m_cBoundingBox,SColor(100,0x40,0x40,0x40));
  #endif
}

void CIrrOdeGeomSphere::initPhysics() {
  if (m_bPhysicsInitialized) return;

  updateAbsolutePosition();
  m_pSpace=reinterpret_cast<CIrrOdeSpace *>(getAncestorOfType((ESCENE_NODE_TYPE)IRR_ODE_SPACE_ID));
  if (!m_pSpace) m_pSpace=m_pWorld->getSpace();

  if (m_fRadius==0.0f) {
    IAnimatedMeshSceneNode *pParent=reinterpret_cast<IAnimatedMeshSceneNode *>(getParent());
    IMesh *pMesh=pParent->getMesh()->getMesh(0);

    m_fRadius=pMesh->getBoundingBox().getExtent().X*getParent()->getScale().X/2;

    #ifdef _TRACE_INIT_PHYSICS
      printf("CIrrOdeGeomSphere::initPhysics: getting radius from parent node\n");
      printf("CIrrOdeGeomSphere::initPhysics: radius=%.2f\n",m_fRadius);
    #endif
  }

  m_iGeomId=m_pOdeDevice->geomCreateSphere(m_pSpace->getSpaceId(),m_fRadius);

  if (m_iGeomId) {
    m_pOdeDevice->geomSetPosition(m_iGeomId,getAbsolutePosition());

    if (m_pBody) {
      if (m_fMass) {
        m_pOdeDevice->massSetZero(m_iMass);
        m_pOdeDevice->massSetSphereTotal(m_iMass,m_fMass,m_fRadius);
        m_pBody->addMass(m_iMass);
        //m_pOdeDevice->bodySetMass(m_pBody->getBodyId(),m_iMass);
      }
      setBody(m_pBody);
    }
  }
  #ifdef _TRACE_INIT_PHYSICS
    printf("CIrrOdeGeomSphere::initPhysics: %i\n",(int)m_iGeomId);
    printf("CIrrOdeGeomSphere radius: %.2f, mass: %.2f\n",m_fRadius,m_fMass);
  #endif

  #ifdef _DRAW_BOUNDING_BOXES
    m_cBoundingBox=getParent()->getBoundingBox();
  #endif

  m_pOdeDevice->geomSetData(m_iGeomId,this);
  CIrrOdeGeom::initPhysics();
}

s32 CIrrOdeGeomSphere::getID() const {
  return ID;
}

ESCENE_NODE_TYPE CIrrOdeGeomSphere::getType() const {
  return (ESCENE_NODE_TYPE)IRR_ODE_GEOM_SPHERE_ID;
}

const wchar_t *CIrrOdeGeomSphere::getTypeName() {
  return IRR_ODE_GEOM_SPHERE_NAME;
}

void CIrrOdeGeomSphere::setMassTotal(f32 fMass) {
  m_fMass=fMass;
}

void CIrrOdeGeomSphere::setRadius(f32 fRadius) {
  m_fRadius=fRadius;
}

f32 CIrrOdeGeomSphere::getRadius() {
  return m_fRadius;
}

void CIrrOdeGeomSphere::serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options) const {
  CIrrOdeGeom::serializeAttributes(out,options);
  out->addFloat("Radius",m_fRadius);
}

void CIrrOdeGeomSphere::deserializeAttributes(IAttributes* in, SAttributeReadWriteOptions* options) {
  CIrrOdeGeom::deserializeAttributes(in,options);
  m_fRadius=in->getAttributeAsFloat("Radius");
}

ISceneNode *CIrrOdeGeomSphere::clone(ISceneNode* newParent, ISceneManager* newManager) {
  CIrrOdeGeomSphere *pRet=new CIrrOdeGeomSphere(newParent?newParent:getParent(),newManager?newManager:m_pSceneManager);
  copyParams(pRet);
  CIrrOdeSceneNode::cloneChildren(pRet,newManager);
  return pRet;
}

void CIrrOdeGeomSphere::copyParams(CIrrOdeSceneNode *pDest, bool bRecurse) {
  if (bRecurse) CIrrOdeGeom::copyParams(pDest);
  CIrrOdeGeomSphere *pDst=(CIrrOdeGeomSphere *)pDest;
  pDst->setRadius(m_fRadius);
}

} //namespace ode
} //namespace irr
