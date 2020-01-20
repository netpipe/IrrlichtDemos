  #include <event/CIrrOdeEventNodeRemoved.h>

namespace irr { namespace ode {

CIrrOdeEventNodeRemoved::CIrrOdeEventNodeRemoved(scene::ISceneNode *pNode) {
  m_pNode=pNode;
  m_iNodeId=pNode->getID();
}

CIrrOdeEventNodeRemoved::CIrrOdeEventNodeRemoved(s32 iNodeId) {
  m_pNode=NULL;
  m_iNodeId=iNodeId;
}

CIrrOdeEventNodeRemoved::CIrrOdeEventNodeRemoved(CSerializer *pData) {
  m_pNode=NULL;
  m_iNodeId=-1;

  pData->resetBufferPos();
  u16 iCode=pData->getU16();
  if (iCode==eIrrOdeEventNodeRemoved) {
    m_iNodeId=pData->getS32();
  }
}

CIrrOdeEventNodeRemoved::CIrrOdeEventNodeRemoved(IIrrOdeEvent *pEvt) {
  if (pEvt->getType()==eIrrOdeEventNodeRemoved) {
    CIrrOdeEventNodeRemoved *p=(CIrrOdeEventNodeRemoved *)pEvt;
    m_iNodeId=p->getRemovedNodeId();
    m_pNode=p->getRemovedNode();
  }
}

u16 CIrrOdeEventNodeRemoved::getType() {
  return eIrrOdeEventNodeRemoved;
}

scene::ISceneNode *CIrrOdeEventNodeRemoved::getRemovedNode() {
  return m_pNode;
}

s32 CIrrOdeEventNodeRemoved::getRemovedNodeId() {
  return m_iNodeId;
}

CSerializer *CIrrOdeEventNodeRemoved::serialize() {
  if (m_pSerializer==NULL) {
    m_pSerializer=new CSerializer();
    m_pSerializer->addU16(eIrrOdeEventNodeRemoved);
    m_pSerializer->addS32(m_iNodeId);
  }

  return m_pSerializer;
}

const c8 *CIrrOdeEventNodeRemoved::toString() {
  sprintf(m_sString,"CIrrOdeEventNodeRemoved: m_iNodeId=%i\n",m_iNodeId);
  return m_sString;
}

} }
