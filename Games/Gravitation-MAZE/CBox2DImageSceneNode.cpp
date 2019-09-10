#include <irrlicht.h>
#include "CBox2DSceneNode.h"
#include "CBox2DImageSceneNode.h"
#include "CBox2DSceneNodeFactory.h"

CBox2DImageSceneNode::CBox2DImageSceneNode(ISceneNode *pParent, ISceneManager *pManager, u32 id) : CBox2DSceneNode(pParent,pManager,id) {
  strcpy((char *)c_sTypeName,"CBox2DImageSceneNode");
  m_pHorizontalBuffer=NULL;
}

CBox2DImageSceneNode::~CBox2DImageSceneNode() {
  printf("CBox2DImageSceneNode destructor\n");
  if (m_pHorizontalBuffer) m_pHorizontalBuffer->drop();
}

void CBox2DImageSceneNode::setHorizontalMeshBuffer(IMeshBuffer *pBuffer) {
  m_pHorizontalBuffer=pBuffer;
  m_pHorizontalBuffer->recalculateBoundingBox();
  m_cBoundingBox.addInternalBox(m_pHorizontalBuffer->getBoundingBox());
}

void CBox2DImageSceneNode::render() {
  CBox2DSceneNode::render();
  if (m_pHorizontalBuffer && !m_bDrawBox2D) {
    m_pDrv->setMaterial(m_cHorizontalMaterial);
    m_pDrv->setTransform(video::ETS_WORLD, AbsoluteTransformation);
    m_pDrv->drawMeshBuffer(m_pHorizontalBuffer);
  }
}

const s32 CBox2DImageSceneNode::getMaterialCount() {
  return 2;
}

video::SMaterial& CBox2DImageSceneNode::getMaterial(s32 i) {
  return i==0?CBox2DSceneNode::getMaterial(i):m_cHorizontalMaterial;
}

void CBox2DImageSceneNode::setMaterialTexture(u32 iTextureLayer, ITexture *pTexture) {
  CBox2DSceneNode::setMaterialTexture(iTextureLayer,pTexture);
  m_cHorizontalMaterial.TextureLayer[iTextureLayer].Texture=pTexture;
}

void CBox2DImageSceneNode::setMaterialFlag(E_MATERIAL_FLAG flag, bool bNewValue) {
  CBox2DSceneNode::setMaterialFlag(flag,bNewValue);
  m_cHorizontalMaterial.setFlag(flag,bNewValue);
}

ESCENE_NODE_TYPE CBox2DImageSceneNode::getType() const {
  return (ESCENE_NODE_TYPE)BOX2D_IMAGE_NODE;
}

const char *CBox2DImageSceneNode::getTypeName() const {
  return BOX2D_IMAGE_NODE_NAME;
}

