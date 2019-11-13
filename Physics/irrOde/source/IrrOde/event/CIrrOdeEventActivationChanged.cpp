  #include <event/CIrrOdeEventActivationChanged.h>
  #include <CSerializer.h>

namespace irr {
namespace ode {

class CIrrOdeBody;

CIrrOdeEventActivationChanged::CIrrOdeEventActivationChanged(CIrrOdeBody *pBody, bool bActive, s32 ID) : IIrrOdeEvent() {
  m_pBody=pBody;
  m_bActive=bActive;
  this->ID=ID;
}

CIrrOdeEventActivationChanged::CIrrOdeEventActivationChanged(CSerializer *pData, ISceneManager *pSmgr, s32 ID) : IIrrOdeEvent() {
  this->ID=ID;

  pData->resetBufferPos();
  if (pData->getU16()==eIrrOdeEventActivationChanged) {
    u32 bodyId=pData->getU32();
    m_pBody=reinterpret_cast<CIrrOdeBody *>(pSmgr->getSceneNodeFromId(bodyId));
    m_bActive=pData->getU8();
  }
}

CIrrOdeEventActivationChanged::CIrrOdeEventActivationChanged(IIrrOdeEvent *pIn) : IIrrOdeEvent() {
  this->ID=pIn->getID();
}

s32 CIrrOdeEventActivationChanged::getID() {
  return ID;
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
