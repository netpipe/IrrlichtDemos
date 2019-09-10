#include <irrlicht.h>
#include "CBox2DSceneNode.h"
#include "CBox2DSceneNodeFactory.h"

CBox2DSceneNode::CBox2DSceneNode(ISceneNode *pParent, ISceneManager *pManager, u32 id) : ISceneNode(pParent,pManager,id) {
  strcpy((char *)c_sTypeName,"CBox2DSceneNode");

  m_cPosition=vector3df(0,0,0);

  m_pDrv=pManager->getVideoDriver();
  m_bDrawBoundingBox=false;
  m_bDrawBox2D=false;
  m_pManager=pManager;

  m_pBuffer=NULL;
  m_pBox2DBuffer=NULL;
}

CBox2DSceneNode::~CBox2DSceneNode() {
  printf("CBox2DSceneNode destructor\n");
  if (m_pBuffer     ) m_pBuffer     ->drop();
  if (m_pBox2DBuffer) m_pBox2DBuffer->drop();
}

void CBox2DSceneNode::render() {
  if (m_pBuffer && !m_bDrawBox2D) {
    m_pDrv->setMaterial(m_cMaterial);
    m_pDrv->setTransform(video::ETS_WORLD,AbsoluteTransformation);
    m_pDrv->drawMeshBuffer(m_pBuffer);
  }

  if (m_pBox2DBuffer && m_bDrawBox2D) {
    SMaterial aMat;

    aMat.Wireframe=true;
    aMat.Lighting=false;
    m_pDrv->setMaterial(aMat);
    m_pDrv->drawMeshBuffer(m_pBox2DBuffer);
  }

  if (m_bDrawBoundingBox) {
    m_pDrv->draw3DBox(m_cBoundingBox,SColor(0xFF,0xFF,0,0));
  }
}

const core::aabbox3d<f32> &CBox2DSceneNode::getBoundingBox() const {
  return m_cBoundingBox;
}

void CBox2DSceneNode::setMeshBuffer(IMeshBuffer *pBuffer) {
  m_pBuffer=pBuffer;
  m_pBuffer->recalculateBoundingBox();
  m_cBoundingBox=m_pBuffer->getBoundingBox();
}

void CBox2DSceneNode::setBox2DBuffer(IMeshBuffer *pBuffer) {
  SColor theColors[]={ SColor(0xFF,0x80,0x80,0x80), SColor(0xFF,0x80,0x80,0), SColor(0xFF,0x80,0   ,0x80),
                       SColor(0xFF,0   ,0x80,0x80), SColor(0xFF,0   ,0x80,0), SColor(0xFF,0   ,0x80,0   ) };
  int iCol=0;

  S3DVertex *pVert=(S3DVertex *)pBuffer->getVertices();
  for (unsigned i=0; i<pBuffer->getVertexCount(); i++) {
    pVert[i].Pos.Z=0;
    pVert[i].Color=theColors[iCol++];
    if (iCol>5) iCol=0;
  }

  m_pBox2DBuffer=pBuffer;
  CBox2DManager::getSharedInstance()->addBox2DStaticSceneNode(this);
}

unsigned int CBox2DSceneNode::getVertexCount() {
  return m_pBuffer->getVertexCount();
}

void CBox2DSceneNode::OnRegisterSceneNode() {
  if (IsVisible) SceneManager->registerNodeForRendering(this);
  ISceneNode::OnRegisterSceneNode();
}

const s32 CBox2DSceneNode::getMaterialCount() {
  return 1;
}

video::SMaterial &CBox2DSceneNode::getMaterial(s32 i) {
  return m_cMaterial;
}

void CBox2DSceneNode::setMaterialTexture(u32 iTextureLayer, ITexture *pTexture) {
  m_cMaterial.TextureLayer[iTextureLayer].Texture=pTexture;
}

void CBox2DSceneNode::setMaterialFlag(E_MATERIAL_FLAG flag, bool bNewValue) {
  m_cMaterial.setFlag(flag,bNewValue);
}

void CBox2DSceneNode::setDrawBox2D(bool b) {
  m_bDrawBox2D=b;
}

void CBox2DSceneNode::setDrawBoundingBox(bool b) {
  m_bDrawBoundingBox=b;
}

ESCENE_NODE_TYPE CBox2DSceneNode::getType() const {
  return (ESCENE_NODE_TYPE)BOX2D_SCENE_NODE;
}

const char *CBox2DSceneNode::getTypeName() const {
  return BOX2D_SCENE_NODE_NAME;
}

void CBox2DSceneNode::setPosition(const vector3df &newPos) {
  RelativeTranslation=newPos;
}

void CBox2DSceneNode::setScale(const vector3df &scale) {
  RelativeScale=scale;
}

void CBox2DSceneNode::setRotation(const vector3df &rotation) {
  RelativeRotation=rotation;
}
