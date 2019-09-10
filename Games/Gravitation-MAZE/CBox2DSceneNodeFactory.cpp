#include <irrlicht.h>
#include "CBox2DSceneNode.h"
#include "CBox2DImageSceneNode.h"
#include "CBox2DImageRootSceneNode.h"
#include "CBox2DSceneNodeFactory.h"

CBox2DSceneNodeFactory::CBox2DSceneNodeFactory(ISceneManager *pManager) {
  m_pManager=pManager;

  stringc sName1; sName1.append(BOX2D_SCENE_NODE_NAME); m_aSceneNodeNames.push_back(sName1);
  stringc sName2; sName2.append(BOX2D_IROOT_NODE_NAME); m_aSceneNodeNames.push_back(sName2);
  stringc sName3; sName3.append(BOX2D_IMAGE_NODE_NAME); m_aSceneNodeNames.push_back(sName3);
}

ISceneNode *CBox2DSceneNodeFactory::addSceneNode (const c8 *typeName, ISceneNode *parent) {
  ISceneNode *pResult=NULL;

  if (!parent) parent=m_pManager->getRootSceneNode();

  if (!strcmp(typeName,BOX2D_SCENE_NODE_NAME)) pResult=new CBox2DSceneNode         (parent,m_pManager,0);
  if (!strcmp(typeName,BOX2D_IROOT_NODE_NAME)) pResult=new CBox2DImageRootSceneNode(parent,m_pManager,0);
  if (!strcmp(typeName,BOX2D_IMAGE_NODE_NAME)) pResult=new CBox2DImageSceneNode    (parent,m_pManager,0);

  return pResult;
}

ISceneNode *CBox2DSceneNodeFactory::addSceneNode (ESCENE_NODE_TYPE type, ISceneNode *parent) {
  switch(type) {
    case BOX2D_SCENE_NODE: return new CBox2DSceneNode         (parent,m_pManager,0);
    case BOX2D_IMAGE_NODE: return new CBox2DImageRootSceneNode(parent,m_pManager,0);
    case BOX2D_IROOT_NODE: return new CBox2DImageSceneNode    (parent,m_pManager,0);
    default: return NULL;
  }
}

u32 CBox2DSceneNodeFactory::getCreatableSceneNodeTypeCount() const {
  return m_aSceneNodeNames.size();;
}

ESCENE_NODE_TYPE CBox2DSceneNodeFactory::getCreateableSceneNodeType (u32 idx) const {
  switch (idx) {
    case 0: return (ESCENE_NODE_TYPE)BOX2D_SCENE_NODE;
    case 1: return (ESCENE_NODE_TYPE)BOX2D_IMAGE_NODE;
    case 2: return (ESCENE_NODE_TYPE)BOX2D_IROOT_NODE;
  }
  return ESNT_UNKNOWN;
}

const c8 *CBox2DSceneNodeFactory::getCreateableSceneNodeTypeName(ESCENE_NODE_TYPE type) const {
  switch((int)type) {
    case BOX2D_SCENE_NODE: return m_aSceneNodeNames[0].c_str();
    case BOX2D_IMAGE_NODE: return m_aSceneNodeNames[1].c_str();
    case BOX2D_IROOT_NODE: return m_aSceneNodeNames[2].c_str();
    default: return NULL;
  }
}

const c8 *CBox2DSceneNodeFactory::getCreateableSceneNodeTypeName (u32 idx) const {
  printf("\t\tgetCreateableSceneNodeTypeName2: %i\n",idx);
  return idx<m_aSceneNodeNames.size()?m_aSceneNodeNames[idx].c_str():NULL;
}

CBox2DSceneNodeFactory::~CBox2DSceneNodeFactory () {
}

