  #include <event/CIrrOdeEventNodeCloned.h>

namespace irr {
namespace ode {

CIrrOdeEventNodeCloned::CIrrOdeEventNodeCloned(s32 iSourceId, s32 iNewId) : IIrrOdeEvent() {
  m_iSourceId=iSourceId;
  m_iNewId=iNewId;
}

CIrrOdeEventNodeCloned::CIrrOdeEventNodeCloned(CSerializer *pData) {
  pData->resetBufferPos();
  u16 iCode=pData->getU16();
  if (iCode==eIrrOdeEventNodeCloned) {
    m_iSourceId=pData->getS32();
    m_iNewId=pData->getS32();
  }
}

CIrrOdeEventNodeCloned::CIrrOdeEventNodeCloned(IIrrOdeEvent *pEvent) {
  if (pEvent->getType()==eIrrOdeEventNodeCloned) {
    m_iSourceId=((CIrrOdeEventNodeCloned *)pEvent)->getSourceId();
    m_iNewId=((CIrrOdeEventNodeCloned *)pEvent)->getNewId();
  }
}

u16 CIrrOdeEventNodeCloned::getType() {
  return eIrrOdeEventNodeCloned;
}

CSerializer *CIrrOdeEventNodeCloned::serialize() {
  if (m_pSerializer==NULL) {
    m_pSerializer=new CSerializer();
    m_pSerializer->addU16(eIrrOdeEventNodeCloned);
    m_pSerializer->addS32(m_iSourceId);
    m_pSerializer->addS32(m_iNewId);
  }

  return m_pSerializer;
}

const c8 *CIrrOdeEventNodeCloned::toString() {
  sprintf(m_sString,"CIrrOdeEventNodeCloned: source id=%i, new id=%i",m_iSourceId,m_iNewId);
  return m_sString;
}

} //namespace ode
} //namespace irr

