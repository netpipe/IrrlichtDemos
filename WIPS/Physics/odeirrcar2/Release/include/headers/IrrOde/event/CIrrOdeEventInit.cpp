  #include <event/CIrrOdeEventInit.h>

namespace irr {
namespace ode {

CIrrOdeEventInit::CIrrOdeEventInit() : IIrrOdeEvent() {
}

CIrrOdeEventInit::CIrrOdeEventInit(CSerializer *pData) {
}

u16 CIrrOdeEventInit::getType() {
  return eIrrOdeEventInit;
}

CSerializer *CIrrOdeEventInit::serialize() {
  if (m_pSerializer==NULL) {
    m_pSerializer=new CSerializer();
    m_pSerializer->addU16(eIrrOdeEventInit);
  }

  return m_pSerializer;
}

const c8 *CIrrOdeEventInit::toString() {
  sprintf(m_sString,"CIrrOdeEventInit");
  return m_sString;
}

} //namespace ode
} //namespace irr

