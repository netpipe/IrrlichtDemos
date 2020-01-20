  #include <event/CIrrOdeEventClose.h>

namespace irr {
namespace ode {

CIrrOdeEventClose::CIrrOdeEventClose() : IIrrOdeEvent() {
}

u16 CIrrOdeEventClose::getType() {
  return eIrrOdeEventClose;
}

CSerializer *CIrrOdeEventClose::serialize() {
  if (m_pSerializer==NULL) {
    m_pSerializer=new CSerializer();
    m_pSerializer->addU16(eIrrOdeEventClose);
  }

  return m_pSerializer;
}

const c8 *CIrrOdeEventClose::toString() {
  sprintf(m_sString,"CIrrOdeEventClose");
  return m_sString;
}

} //namespace ode
} //namespace irr


