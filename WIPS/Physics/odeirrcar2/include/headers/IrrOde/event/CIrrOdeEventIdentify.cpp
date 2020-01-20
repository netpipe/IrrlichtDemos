  #include <event/CIrrOdeEventIdentify.h>

namespace irr {
namespace ode {

CIrrOdeEventIdentify::CIrrOdeEventIdentify(const c8 *sAppName) : IIrrOdeEvent() {
  strcpy(m_sIdentify,_IRR_ODE_IDENTIFY_STRING);
  strcpy(m_sAppName,sAppName);
}

CIrrOdeEventIdentify::CIrrOdeEventIdentify(CSerializer *pData) {
  pData->resetBufferPos();
  u16 iCode=pData->getU16();
  if (iCode==eIrrOdeEventIdentify) {
    pData->getString(m_sIdentify);
    pData->getString(m_sAppName);
  }
}

CIrrOdeEventIdentify::CIrrOdeEventIdentify(irr::ode::IIrrOdeEvent *pEvent) {
  if (pEvent->getType()==eIrrOdeEventIdentify) {
    CIrrOdeEventIdentify *p=(CIrrOdeEventIdentify *)pEvent;
    strcpy(m_sIdentify,p->getIdentify());
    strcpy(m_sAppName ,p->getAppName ());
  }
}

const c8 *CIrrOdeEventIdentify::getIdentify() {
  return m_sIdentify;
}

const c8 *CIrrOdeEventIdentify::getAppName() {
  return m_sAppName;
}

CSerializer *CIrrOdeEventIdentify::serialize() {
  if (m_pSerializer==NULL) {
    m_pSerializer=new CSerializer();
    m_pSerializer->addU16(eIrrOdeEventIdentify);
    m_pSerializer->addString(m_sIdentify);
    m_pSerializer->addString(m_sAppName);
  }

  return m_pSerializer;
}

const c8 *CIrrOdeEventIdentify::toString() {
  sprintf(m_sString,"CIrrOdeEventIdentify: sIdentify=\"%s\", sAppName=\"%s\"",m_sIdentify,m_sAppName);
  return m_sString;
}

}
}

