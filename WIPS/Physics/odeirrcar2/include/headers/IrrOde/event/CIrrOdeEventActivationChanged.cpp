  #include <event/CIrrOdeEventActivationChanged.h>
  #include <CSerializer.h>

namespace irr {
namespace ode {

class CIrrOdeBody;

CIrrOdeEventActivationChanged::CIrrOdeEventActivationChanged(CIrrOdeBody *pBody, bool bActive) : IIrrOdeEvent() {
  m_pBody=pBody;
  m_bActive=bActive;
}

CIrrOdeEventActivationChanged::CIrrOdeEventActivationChanged(CSerializer *pData, ISceneManager *pSmgr) : IIrrOdeEvent() {
  pData->resetBufferPos();
  if (pData->getU16()==eIrrOdeEventActivationChanged) {
    u32 bodyId=pData->getU32();
    m_pBody=reinterpret_cast<CIrrOdeBody *>(pSmgr->getSceneNodeFromId(bodyId));
    m_bActive=pData->getU8();
  }
}

CIrrOdeEventActivationChanged::CIrrOdeEventActivationChanged(IIrrOdeEvent *pIn) : IIrrOdeEvent() {
  if (pIn->getType()==eIrrOdeEventActivationChanged) {
    CIrrOdeEventActivationChanged *pEvt=reinterpret_cast<CIrrOdeEventActivationChanged *>(pIn);
    this->m_pBody=pEvt->getBody();
    this->m_bActive=pEvt->isActive();
  }
}

u16 CIrrOdeEventActivationChanged::getType() {
  return eIrrOdeEventActivationChanged;
}

CIrrOdeBody *CIrrOdeEventActivationChanged::getBody() {
  return m_pBody;
}

bool CIrrOdeEventActivationChanged::isActive() {
  return m_bActive;
}

CSerializer *CIrrOdeEventActivationChanged::serialize() {
  if (m_pSerializer==NULL) {
    m_pSerializer=new CSerializer();
    m_pSerializer->addU16(eIrrOdeEventActivationChanged);
    m_pSerializer->addU32(m_pBody->getID());
    m_pSerializer->addU8(m_bActive?1:0);
  }

  return m_pSerializer;
}

const c8 *CIrrOdeEventActivationChanged::toString() {
  sprintf(m_sString,"CIrrOdeEventActivationChanged: body=%i, active?%s",m_pBody->getID(),m_bActive?"YES":"NO");
  return m_sString;
}

} //namespace ode
} //namespace irr
