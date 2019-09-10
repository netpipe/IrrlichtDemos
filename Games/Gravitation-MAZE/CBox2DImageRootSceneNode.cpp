#include <irrlicht.h>
#include "CBox2DImageSceneNode.h"
#include "CBox2DImageRootSceneNode.h"
#include "CBox2DSceneNodeFactory.h"

CBox2DImageRootSceneNode::CBox2DImageRootSceneNode(ISceneNode *pParent, ISceneManager *pManager, u32 id) : CBox2DImageSceneNode(pParent,pManager,id) {
  strcpy((char *)c_sTypeName,"CBox2DImageRootSceneNode");
}

void CBox2DImageRootSceneNode::setMesh(IAnimatedMesh *pMesh) {
  m_pMesh=pMesh;

  m_pNodes.push_back(this);

  for (unsigned i=0; i<pMesh->getFrameCount(); i++) {
    CBox2DImageSceneNode *b2dnode=reinterpret_cast<CBox2DImageSceneNode *>(m_pManager->addSceneNode("CBox2DImageSceneNode",this));
    if (b2dnode) {
      b2dnode->setMeshBuffer(pMesh->getMesh(i)->getMeshBuffer(0));
      if (pMesh->getMesh(i)->getMeshBufferCount()>=2) b2dnode->setBox2DBuffer(pMesh->getMesh(i)->getMeshBuffer(1));
      if (pMesh->getMesh(i)->getMeshBufferCount()>=3) b2dnode->setHorizontalMeshBuffer(pMesh->getMesh(i)->getMeshBuffer(2));
      b2dnode->setAutomaticCulling(EAC_FRUSTUM_BOX);
      b2dnode->setMaterialFlag(EMF_LIGHTING,false);

      SMaterial &aMaterial1=b2dnode->getMaterial(0);
      aMaterial1.setTexture(0,m_pManager->getVideoDriver()->getTexture("levels/texture_1.jpg"));
      SMaterial &aMaterial2=b2dnode->getMaterial(1);
      aMaterial2.setTexture(0,m_pManager->getVideoDriver()->getTexture("levels/texture_2.jpg"));

      m_pNodes.push_back(b2dnode);
      b2dnode->drop();
    }
  }
}

void CBox2DImageRootSceneNode::setDrawBox2D(bool b) {
  for (unsigned i=0; i<m_pNodes.size(); i++) m_pNodes[i]->setDrawBox2D(b);
}

void CBox2DImageRootSceneNode::setDrawBoundingBox(bool b) {
  for (unsigned i=0; i<m_pNodes.size(); i++) m_pNodes[i]->setDrawBoundingBox(b);
}

ESCENE_NODE_TYPE CBox2DImageRootSceneNode::getType() const {
  return (ESCENE_NODE_TYPE)BOX2D_IROOT_NODE;
}

const char *CBox2DImageRootSceneNode::getTypeName() const {
  return BOX2D_IROOT_NODE_NAME;
}

void CBox2DImageRootSceneNode::loadImageMesh(const c8 *sImageName) {
  IAnimatedMesh *pData=m_pManager->getMesh(sImageName);
  printf("--> CBox2DImageRootSceneNode::LoadImageMesh: %i Frames\n",pData->getFrameCount());
  setMesh(pData);
}

void CBox2DImageRootSceneNode::clear() {
  while (m_pNodes.size()>1) {
    m_pManager->addToDeletionQueue(m_pNodes[1]);
    m_pNodes.erase(1);
  }

  m_pManager->getMeshCache()->removeMesh(m_pMesh);
  m_pMesh=NULL;

  printf("nodes.size=%i\n",m_pNodes.size());
}
