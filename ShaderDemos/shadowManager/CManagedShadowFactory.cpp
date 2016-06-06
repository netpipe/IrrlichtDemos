  #include <CManagedShadowFactory.h>
  #include <CManagedShadow.h>

CManagedShadowFactory::CManagedShadowFactory(ISceneManager *pManager) : ISceneNodeFactory() {
  m_pManager=pManager;
}

ISceneNode *CManagedShadowFactory::addSceneNode(ESCENE_NODE_TYPE type, ISceneNode *parent) {
  ISceneNode *pRet=NULL;
  if (type==MANAGED_SHADOW_ID) {
    if (!parent) parent=m_pManager->getRootSceneNode();
    pRet=new CManagedShadow(parent,m_pManager);
  }
  return pRet;
}

ISceneNode *CManagedShadowFactory::addSceneNode (const c8 *typeName, ISceneNode *parent) {
  if (!strcmp(typeName,"CManagedShadow")) {
		if (!parent) parent=m_pManager->getRootSceneNode();
    return new CManagedShadow(parent,m_pManager);
  }
  return NULL;
}

u32 CManagedShadowFactory::getCreatableSceneNodeTypeCount() const {
  return 1;
}

ESCENE_NODE_TYPE CManagedShadowFactory::getCreateableSceneNodeType(u32 idx) const {
	return idx==0?(ESCENE_NODE_TYPE)MANAGED_SHADOW_ID:ESNT_UNKNOWN;
}

const c8 *CManagedShadowFactory::getCreateableSceneNodeTypeName(ESCENE_NODE_TYPE type) const {
	return (ESCENE_NODE_TYPE)type==MANAGED_SHADOW_ID?MANAGED_SHADOW_NAME:NULL;
}

const c8 *CManagedShadowFactory::getCreateableSceneNodeTypeName(u32 idx) const {
	return idx==0?MANAGED_SHADOW_NAME:NULL;
}

CManagedShadowFactory::~CManagedShadowFactory() {
}
