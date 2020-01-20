  #include <geom/CIrrOdeGeomBox.h>
  #include <CIrrOdeSpace.h>
  #include <CIrrOdeWorld.h>
  #include <CIrrOdeSpace.h>
  #include <CIrrOdeBody.h>
  #include <IIrrOdeDevice.h>

namespace irr {
namespace ode {

CIrrOdeGeomBox::CIrrOdeGeomBox(ISceneNode *parent,ISceneManager *mgr,s32 id,
                               const vector3df &position, const vector3df &rotation, const vector3df &scale) :
                               CIrrOdeGeom(parent, mgr, id, position, rotation, scale) {

  #ifdef _TRACE_CONSTRUCTOR_DESTRUCTOR
    printf("CIrrOdeGeomBox constructor\n");
  #endif

  m_iGeomId=0;
  m_fWidth=0.0f;
  m_fHeight=0.0f;
  m_fDepth=0.0f;
  m_fMass=1.0f;

  m_bUseAllMeshBuffers=true;

  #ifdef _IRREDIT_PLUGIN
    if (m_pMesh) {
	  c8 sFileName[1024];
	  sprintf(sFileName,"%sIrrOdeGeomBox.png",m_sResources);
      m_cMat.setTexture(0,m_pSceneManager->getVideoDriver()->getTexture(sFileName));
    }
  #endif
}

CIrrOdeGeomBox::~CIrrOdeGeomBox() {
  #ifdef _TRACE_CONSTRUCTOR_DESTRUCTOR
    printf("CIrrOdeGeomBox destructor\n");
  #endif
}

void CIrrOdeGeomBox::render() {
  CIrrOdeSceneNode::render();
  #ifdef _DRAW_BOUNDING_BOXES
    m_pVideoDriver->setTransform(video::ETS_WORLD,AbsoluteTransformation);
    m_pVideoDriver->setMaterial(m_cMat);
    m_pVideoDriver->draw3DBox(m_cBoundingBox,SColor(100,0x40,0x40,0x40));
  #endif
}

void CIrrOdeGeomBox::OnRegisterSceneNode() {
  if (IsVisible) SceneManager->registerNodeForRendering(this);
  ISceneNode::OnRegisterSceneNode();
}

void CIrrOdeGeomBox::initPhysics() {
  if (m_bPhysicsInitialized) return;

  getParent()->updateAbsolutePosition();
  updateAbsolutePosition();
  m_pSpace=reinterpret_cast<CIrrOdeSpace *>(getAncestorOfType((ESCENE_NODE_TYPE)IRR_ODE_SPACE_ID));
  if (!m_pSpace) m_pSpace=m_pWorld->getSpace();

  if (m_fWidth==0.0f && m_fHeight==0.0f && m_fDepth==0.0f) {
    if (this->m_bUseAllMeshBuffers) {
      IAnimatedMeshSceneNode *pParent=reinterpret_cast<IAnimatedMeshSceneNode *>(getParent());
      IMesh *pMesh=pParent->getMesh()->getMesh(0);

      m_fWidth =pMesh->getBoundingBox().getExtent().X*pParent->getScale().X,
      m_fHeight=pMesh->getBoundingBox().getExtent().Y*pParent->getScale().Y,
      m_fDepth =pMesh->getBoundingBox().getExtent().Z*pParent->getScale().Z;
      #ifdef _TRACE_INIT_PHYSICS
        printf("CIrrOdeGeomBox::initPhysics: getting size from parent node\n");
        printf("CIrrOdeGeomBox::initPhysics: size=(%.2f, %.2f, %.2f)\n",m_fWidth,m_fHeight,m_fDepth);
      #endif
    }
    else {
      IMesh *pMesh=NULL;
      CDynamicMeshBuffer cDynMeshBuffer(EVT_2TCOORDS,EIT_16BIT);
      aabbox3d<f32> theBox=aabbox3d<f32>(0.0f,0.0f,0.0f,0.0f,0.0f,0.0f);
      ISceneNode *pNode=reinterpret_cast<IAnimatedMeshSceneNode *>(getParent());
      switch (pNode->getType()) {
      case ESNT_MESH :
        pMesh=(reinterpret_cast<IMeshSceneNode *>(pNode))->getMesh();
        break;

      case ESNT_ANIMATED_MESH:
        pMesh=(reinterpret_cast<IAnimatedMeshSceneNode *>(pNode))->getMesh();
        break;

      default:
        printf("Non supported scene node type detected!\n");
        return;
      }
      if (pMesh) {
        for (u32 i=0; i<pMesh->getMeshBufferCount(); i++) {
          if (i>=m_aUseMeshBuffer.size() || m_aUseMeshBuffer[i]) {
            theBox.addInternalBox(pMesh->getMeshBuffer(i)->getBoundingBox());
          }
        }

        m_cBoundingBox=theBox;
        m_fWidth =theBox.getExtent().X*pNode->getScale().X;
        m_fHeight=theBox.getExtent().Y*pNode->getScale().Y;
        m_fDepth =theBox.getExtent().Z*pNode->getScale().Z;
      }
    }
  }

  m_iGeomId=m_pOdeDevice->geomCreateBox(m_pSpace->getSpaceId(),m_fWidth,m_fHeight,m_fDepth);

  if (m_iGeomId) {
    vector3df pos=getAbsolutePosition(),
              rot=getAbsoluteTransformation().getRotationDegrees();

    m_pOdeDevice->geomSetPosition(m_iGeomId,pos);
    m_pOdeDevice->geomSetRotation(m_iGeomId,rot);

    if (m_pBody) {
      if (m_fMass) {
        m_pOdeDevice->massSetZero(m_iMass);
        m_pOdeDevice->massSetBoxTotal(m_iMass,m_fMass,m_fWidth,m_fHeight,m_fDepth);
        m_pBody->addMass(m_iMass);
        //m_pOdeDevice->bodySetMass(m_pBody->getBodyId(),m_iMass);
      }
      setBody(m_pBody);
    }
  }
  #ifdef _TRACE_INIT_PHYSICS
    printf("CIrrOdeGeomBox::initPhysics: %i\n",(int)m_iGeomId);
    printf("CIrrOdeGeomBox extent: %.2f, %.2f, %.2f\n",m_fWidth,m_fHeight,m_fDepth);
    printf("CIrrOdeGeomBody mass: %.2f\n",m_fMass);
  #endif

  #ifdef _DRAW_BOUNDING_BOXES
	if (!this->m_bUseAllMeshBuffers)
      m_cBoundingBox=getParent()->getBoundingBox();
  #endif

  m_pOdeDevice->geomSetData(m_iGeomId,this);
  CIrrOdeGeom::initPhysics();
}

s32 CIrrOdeGeomBox::getID() const {
  return ID;
}

ESCENE_NODE_TYPE CIrrOdeGeomBox::getType() const {
  return (ESCENE_NODE_TYPE)IRR_ODE_GEOM_BOX_ID;
}

const wchar_t *CIrrOdeGeomBox::getTypeName() {
  return IRR_ODE_GEOM_BOX_NAME;
}

void CIrrOdeGeomBox::setMassTotal(f32 fMass) {
  m_fMass=fMass;
}

void CIrrOdeGeomBox::setWidth(f32 fWidth) {
  m_fWidth=fWidth;
}

void CIrrOdeGeomBox::setHeight(f32 fHeight) {
  m_fHeight=fHeight;
}

void CIrrOdeGeomBox::setDepht(f32 fDepth) {
  m_fDepth=fDepth;
}

void CIrrOdeGeomBox::serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options) const {
  CIrrOdeGeom::serializeAttributes(out,options);

  out->addFloat("Width" ,m_fWidth );
  out->addFloat("Height",m_fHeight);
  out->addFloat("Depth" ,m_fDepth );

  if (getParent() && (getParent()->getType()==ESNT_MESH || getParent()->getType()==ESNT_ANIMATED_MESH)) {
    out->addBool("useAllMaterials",m_bUseAllMeshBuffers);

    if (!m_bUseAllMeshBuffers) {
		for (u32 i=0; i<getParent()->getMaterialCount(); i++) {
        c8 s[0xFF];
        sprintf(s,"useMaterial%i",i+1);
		if (i<m_aUseMeshBuffer.size())
		  out->addBool(s,m_aUseMeshBuffer[i]);
		else
		  out->addBool(s,true);
      }
    }
  }
}

void CIrrOdeGeomBox::deserializeAttributes(IAttributes* in, SAttributeReadWriteOptions* options) {
  CIrrOdeGeom::deserializeAttributes(in,options);

  m_fWidth =in->getAttributeAsFloat("Width" );
  m_fHeight=in->getAttributeAsFloat("Height");
  m_fDepth =in->getAttributeAsFloat("Depth" );

  if (getParent() && (getParent()->getType()==ESNT_MESH || getParent()->getType()==ESNT_ANIMATED_MESH)) {
    if (in->existsAttribute("useAllMaterials"))
 	  m_bUseAllMeshBuffers=in->getAttributeAsBool("useAllMaterials");
    else
      m_bUseAllMeshBuffers=true;

    if (!m_bUseAllMeshBuffers) {
      m_aUseMeshBuffer.clear();
      ISceneNode *pParent=getParent();
      if (pParent)
	  for (u32 i=0; i<getParent()->getMaterialCount(); i++) {
        c8 s[0xFF];
        sprintf(s,"useMaterial%i",i+1);
        if (in->existsAttribute(s))
            m_aUseMeshBuffer.push_back(in->getAttributeAsBool(s));
        else
          m_aUseMeshBuffer.push_back(true);
        }
    }
  }
  else m_bUseAllMeshBuffers=true;

  #ifdef _DRAW_BOUNDING_BOXES
    m_cBoundingBox=aabbox3d<f32>(-m_fWidth/2,-m_fHeight/2,-m_fDepth/2,m_fWidth/2,m_fHeight/2,m_fDepth/2);
  #endif
}

ISceneNode *CIrrOdeGeomBox::clone(ISceneNode* newParent, ISceneManager* newManager) {
  if (newManager==NULL) newManager=m_pSceneManager;
  if (newParent==NULL) newParent=getParent();

  CIrrOdeGeomBox *pRet=new CIrrOdeGeomBox(newParent,newManager?newManager:m_pSceneManager);
  copyParams(pRet);
  return pRet;
}

void CIrrOdeGeomBox::copyParams(CIrrOdeSceneNode *pDest, bool bRecurse) {
  if (bRecurse) CIrrOdeGeom::copyParams(pDest);
  CIrrOdeGeomBox *pDst=(CIrrOdeGeomBox *)pDest;
  pDst->setWidth(m_fWidth);
  pDst->setHeight(m_fHeight);
  pDst->setDepht(m_fDepth);

  pDst->m_bUseAllMeshBuffers=m_bUseAllMeshBuffers;
  pDst->m_aUseMeshBuffer.clear();

  for (u32 i=0; i<m_aUseMeshBuffer.size(); i++)
    pDst->m_aUseMeshBuffer.push_back(m_aUseMeshBuffer[i]);
}

} //namespace ode
} //namespace irr
