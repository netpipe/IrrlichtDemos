  #include <irrlicht.h>

  #include <CManagedShadow.h>
  #include <CShadowManager.h>

CManagedShadow::CManagedShadow(ISceneNode *pParent, ISceneManager *pMgr, s32 iId, const vector3df pos, const vector3df rot, const vector3df scale) : ISceneNode(pParent,pMgr,iId,pos,rot,scale) {
  m_pShadow=NULL;
  m_iLevel=3;

	m_pSceneManager=pMgr;
  CShadowManager::getSharedInstance()->addShadow(this);
}

void CManagedShadow::setLevel(u32 iLevel) {
  m_iLevel=iLevel;
}

u32 CManagedShadow::getLevel() {
  return m_iLevel;
}

void CManagedShadow::update(u32 theLevel) {
  if (m_iLevel<=theLevel) {
    if (m_pShadow==NULL) {
      IAnimatedMeshSceneNode *pParent=reinterpret_cast<IAnimatedMeshSceneNode *>(getParent());
      m_pShadow=pParent->addShadowVolumeSceneNode();
    }
  }
  else {
    if (m_pShadow!=NULL) {
      m_pShadow->setShadowMesh(NULL);
      m_pShadow->updateShadowVolumes();
      m_pShadow->remove();
      m_pShadow=NULL;
    }
  }
}

ESCENE_NODE_TYPE CManagedShadow::getType() const {
  return (ESCENE_NODE_TYPE)MANAGED_SHADOW_ID;
}

void CManagedShadow::serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options) const {
  ISceneNode::serializeAttributes(out,options);
  out->addInt("ShadowLevel",m_iLevel);
}

void CManagedShadow::deserializeAttributes(IAttributes* in, SAttributeReadWriteOptions* options) {
  ISceneNode::deserializeAttributes(in,options);
  m_iLevel=in->getAttributeAsInt("ShadowLevel");
}

ISceneNode *CManagedShadow::clone(ISceneNode* newParent, ISceneManager* newManager) {
	CManagedShadow *pRet=new CManagedShadow(newParent,newManager?newManager:m_pSceneManager);
  pRet->setLevel(m_iLevel);
  return pRet;
}
