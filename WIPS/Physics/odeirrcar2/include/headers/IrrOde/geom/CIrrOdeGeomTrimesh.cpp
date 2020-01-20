  #include <geom/CIrrOdeGeomTrimesh.h>
  #include <CIrrOdeSpace.h>
  #include <CIrrOdeWorld.h>
  #include <CIrrOdeSpace.h>
  #include <CIrrOdeBody.h>
  #include <IIrrOdeDevice.h>
  #include <CIrrOdeManager.h>

namespace irr {
namespace ode {

CIrrOdeGeomTrimesh::CIrrOdeGeomTrimesh(ISceneNode *parent,ISceneManager *mgr,s32 id,
                                       const vector3df &position, const vector3df &rotation, const vector3df &scale) :
                                       CIrrOdeGeom(parent, mgr, id, position, rotation, scale) {

  #ifdef _TRACE_CONSTRUCTOR_DESTRUCTOR
    printf("CIrrOdeGeomTrimesh constructor\n");
  #endif

  if (parent) {
    while (m_aParams.size()<parent->getMaterialCount()) {
      CIrrOdeSurfaceParameters *pParams=new CIrrOdeSurfaceParameters();
      CIrrOdeManager::getSharedInstance()->addSurfaceParameter(pParams);
      pParams->setStatic(m_pBody==NULL);
      m_aParams.push_back(pParams);
    }

    while (m_aParams.size()>parent->getMaterialCount()) {
      m_pOdeManager->removeSurfaceParameter(m_aParams[m_aParams.size()-1]);
      delete m_aParams[m_aParams.size()-1];
      m_aParams.erase(m_aParams.size()-1);
    }
  }

  #ifdef _IRREDIT_PLUGIN
    if (m_pMesh) {
	  c8 sFileName[1024];
	  sprintf(sFileName,"%sIrrOdeGeomTrimesh.png",m_sResources);
      m_cMat.setTexture(0,m_pSceneManager->getVideoDriver()->getTexture(sFileName));
    }
  #endif
}

CIrrOdeGeomTrimesh::~CIrrOdeGeomTrimesh() {
  #ifdef _TRACE_CONSTRUCTOR_DESTRUCTOR
    printf("CIrrOdeGeomTrimesh destructor\n");
  #endif

  for (u32 i=0; i<m_aGeomIds.size(); i++)
    if (m_aGeomIds[i] && m_pWorld->getWorldId()) m_pOdeDevice->geomDestroy(m_aGeomIds[i]);
}

void CIrrOdeGeomTrimesh::OnRegisterSceneNode() {
  if (IsVisible) SceneManager->registerNodeForRendering(this);
  ISceneNode::OnRegisterSceneNode();
}

void CIrrOdeGeomTrimesh::render() {
  CIrrOdeSceneNode::render();
  #ifdef _DRAW_BOUNDING_BOXES
    m_pVideoDriver->setTransform(video::ETS_WORLD, AbsoluteTransformation);
    m_pVideoDriver->draw3DBox(m_cBoundingBox,SColor(100,0xFF,0,0));
  #endif
}

void CIrrOdeGeomTrimesh::initPhysics() {
  if (m_bPhysicsInitialized) return;

  getParent()->updateAbsolutePosition();
  updateAbsolutePosition();
  m_pSpace=reinterpret_cast<CIrrOdeSpace *>(getAncestorOfType((ESCENE_NODE_TYPE)IRR_ODE_SPACE_ID));
  if (!m_pSpace) m_pSpace=m_pWorld->getSpace();

  IMeshSceneNode *pNode=reinterpret_cast<IMeshSceneNode *>(getParent());
  if (getChildren().getSize()) {
    pNode=reinterpret_cast<IMeshSceneNode *>(*(getChildren().begin()));
    pNode->setVisible(false);
  }

  if (!pNode) return;

	#ifdef _DRAW_BOUNDING_BOXES
    m_cBoundingBox=pNode->getBoundingBox();
	#endif

  vector3df pos=m_pBody?m_pBody->getPosition():getAbsolutePosition(),
            rot=getAbsoluteTransformation().getRotationDegrees().normalize();

  if (m_pBody) {
    // build the trimesh data
    #ifdef _TRACE_INIT_PHYSICS
      printf("create single trimesh for node %s\n",pNode->getName());
    #endif
    u32 iData=m_pOdeDevice->geomTrimeshDataCreate();
    m_pOdeDevice->geomTrimeshDataBuildSimple(iData,pNode);
    m_iGeomId=m_pOdeDevice->geomCreateTrimesh(m_pSpace->getSpaceId(),iData);
    //m_aDataIds.push_back(iData);

    if (m_iGeomId) {
      #ifdef _TRACE_INIT_PHYSICS
        printf("position: %.2f, %.2f, %.2f\n",pos.X,pos.Y,pos.Z);
      #endif
      m_pOdeDevice->geomSetPosition(m_iGeomId,pos);
      //m_pOdeDevice->geomSetRotation(m_iGeomId,rot);
    }

    if (m_fMass) {
      m_pOdeDevice->massSetZero(m_iMass);
      vector3df vSize=pNode->getBoundingBox().getExtent();
      //dMassSetTrimeshTotal(&m_cMass,m_fMass,m_iGeomId);
      //dMassTranslate(&m_cMass,vSize.X/2,vSize.Y/2,vSize.Z/2);

      m_pOdeDevice->massSetSphereTotal(m_iMass,m_fMass,(vSize.X+vSize.Y+vSize.Z)/3);
      m_pBody->addMass(m_iMass);
    }
    m_pOdeDevice->geomSetData(m_iGeomId,this);
    m_aGeomIds.push_back(m_iGeomId);
    setBody(m_pBody);
  }
  else {
    #ifdef _TRACE_INIT_PHYSICS
      printf("create multi trimeshes for node %s\n",pNode->getName());
    #endif
    for (u32 i=0; i<pNode->getMaterialCount(); i++) {
      u32 iData=m_pOdeDevice->geomTrimeshDataCreate();
      m_pOdeDevice->geomTrimeshDataBuildFromMeshBuffer(iData,pNode,i);
      u32 iGeom=m_pOdeDevice->geomCreateTrimesh(m_pSpace->getSpaceId(),iData);

      #ifdef _TRACE_INIT_PHYSICS
        printf("create trimesh for material #%i (%i)\n",i,iGeom);
      #endif

      //m_aDataIds.push_back(iData);
      m_aGeomIds.push_back(iGeom);

      vector3df rot=getParent()->getRotation();
      #ifdef _TRACE_INIT_PHYSICS
        printf("setRotation2: %.2f, %.2f, %.2f\n",rot.X,rot.Y,rot.Z);
      #endif
      m_pOdeDevice->geomSetRotation(iGeom,rot);

      m_pOdeDevice->geomSetPosition(iGeom,pos);
      m_pOdeDevice->geomSetData(iGeom,this);
    }
  }

  CIrrOdeGeom::initPhysics();
}

ESCENE_NODE_TYPE CIrrOdeGeomTrimesh::getType() const {
  return (ESCENE_NODE_TYPE )IRR_ODE_GEOM_TRIMESH_ID;
}

s32 CIrrOdeGeomTrimesh::getID() const {
  return ID;
}

const wchar_t *CIrrOdeGeomTrimesh::getTypeName() {
  return IRR_ODE_GEOM_TRIMESH_NAME;
}

u32 CIrrOdeGeomTrimesh::getGeomId(u32 iNum) {
  return iNum<m_aGeomIds.size()?m_aGeomIds[iNum]:m_aGeomIds[0];
}

s32 CIrrOdeGeomTrimesh::getIndexOfGeom(u32 iGeom) {
  for (u32 i=0; i<m_aGeomIds.size(); i++) if (m_aGeomIds[i]==iGeom) return (s32)i;
  return -1;
}

void CIrrOdeGeomTrimesh::setMassTotal(f32 fMass) {
  m_fMass=fMass;
}

void CIrrOdeGeomTrimesh::serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options) const {
  CIrrOdeGeom::serializeAttributes(out,options);
}

void CIrrOdeGeomTrimesh::deserializeAttributes(IAttributes* in, SAttributeReadWriteOptions* options) {
  CIrrOdeGeom::deserializeAttributes(in,options);
}

ISceneNode *CIrrOdeGeomTrimesh::clone(ISceneNode* newParent, ISceneManager* newManager) {
  CIrrOdeGeomTrimesh *pRet=new CIrrOdeGeomTrimesh(newParent?newParent:getParent(),newManager?newManager:m_pSceneManager);
  copyParams(pRet);
  CIrrOdeSceneNode::cloneChildren(pRet,newManager);
  return pRet;
}

void CIrrOdeGeomTrimesh::copyParams(CIrrOdeSceneNode *pDest, bool bRecurse) {
  if (bRecurse) CIrrOdeGeom::copyParams(pDest);
}

void CIrrOdeGeomTrimesh::setParent(ISceneNode *pParent) {
  ISceneNode::setParent(pParent);

	CIrrOdeBody *pBody=reinterpret_cast<CIrrOdeBody *>(getAncestorOfType((ESCENE_NODE_TYPE)IRR_ODE_BODY_ID));

  if (pParent) {
    if (!pBody) {
      while (m_aParams.size()<pParent->getMaterialCount()) {
        CIrrOdeSurfaceParameters *pParams=new CIrrOdeSurfaceParameters();
        CIrrOdeManager::getSharedInstance()->addSurfaceParameter(pParams);
        pParams->setStatic(pBody==NULL);
        m_aParams.push_back(pParams);
      }
      while (m_aParams.size()>pParent->getMaterialCount()) {
        m_pOdeManager->removeSurfaceParameter(m_aParams[m_aParams.size()-1]);
        delete m_aParams[m_aParams.size()-1];
        m_aParams.erase(m_aParams.size()-1);
      }
    }
    else
      while (m_aParams.size()>1) {
        m_pOdeManager->removeSurfaceParameter(m_aParams[m_aParams.size()-1]);
        delete m_aParams[m_aParams.size()-1];
        m_aParams.erase(m_aParams.size()-1);
      }
  }
}

CIrrOdeSurfaceParameters *CIrrOdeGeomTrimesh::getSurfaceParameters(u32 iIdx) {
  return iIdx<m_aParams.size()?m_aParams[iIdx]:m_aParams[0];
}

void CIrrOdeGeomTrimesh::removeFromPhysics() {
  if (m_iGeomId)
    CIrrOdeGeom::removeFromPhysics();
  else {
    CIrrOdeSceneNode::removeFromPhysics();
    if (m_bPhysicsInitialized) {
      for (u32 i=0; i<m_aGeomIds.size(); i++)
        m_pOdeDevice->geomDestroy(m_aGeomIds[i]);

      m_aGeomIds.clear();
      m_bPhysicsInitialized=false;
    }
    else if (m_pBody) m_pBody->removeGeom(this); else m_pWorld->removeGeom(this);
  }
}

} //namespace ode
} //namespace irr
