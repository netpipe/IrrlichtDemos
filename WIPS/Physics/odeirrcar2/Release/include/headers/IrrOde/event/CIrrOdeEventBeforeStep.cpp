  #include <event/CIrrOdeEventBeforeStep.h>

namespace irr {
namespace ode {

CIrrOdeEventBeforeStep::CIrrOdeEventBeforeStep() : IIrrOdeEvent() {
}

CIrrOdeEventBeforeStep::CIrrOdeEventBeforeStep(IIrrOdeEvent *pIn) : IIrrOdeEvent() {
}

u16 CIrrOdeEventBeforeStep::getType() {
  return eIrrOdeEventBeforeStep;
}

CSerializer *CIrrOdeEventBeforeStep::serialize() {
  if (m_pSerializer==NULL) {
    m_pSerializer=new CSerializer();
    m_pSerializer->addU16(eIrrOdeEventBeforeStep);
  }
  return m_pSerializer;
}

const c8 *CIrrOdeEventBeforeStep::toString() {
  strcpy(m_sString,"CIrrOdeEventBeforeStep");
  return m_sString;
}

} //namespace ode
} //namespace irr
