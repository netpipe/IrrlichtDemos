  #include <event/CIrrOdeEventProgress.h>

namespace irr {
namespace ode {

CIrrOdeEventProgress::CIrrOdeEventProgress(u32 iCurrent, u32 iCount) : IIrrOdeEvent() {
  m_iCurrent=iCurrent;
  m_iCount=iCount;
}

CIrrOdeEventProgress::CIrrOdeEventProgress(IIrrOdeEvent *pIn) {
  if (pIn->getType()==eIrrOdeEventProgress) {
    CIrrOdeEventProgress *pEvt=reinterpret_cast<CIrrOdeEventProgress *>(pIn);
    m_iCount=pEvt->getCount();
    m_iCurrent=pEvt->getCurrent();
  }
}

CIrrOdeEventProgress::CIrrOdeEventProgress(CSerializer *pData) : IIrrOdeEvent() {
  pData->resetBufferPos();
  u16 iCode=pData->getU16();
  if (iCode==eIrrOdeEventProgress) {
    m_iCount=pData->getU32();
    m_iCurrent=pData->getU32();
  }
}

u16 CIrrOdeEventProgress::getType() {
  return eIrrOdeEventProgress;
}

CSerializer *CIrrOdeEventProgress::serialize() {
  return NULL;
}

const c8 *CIrrOdeEventProgress::toString() {
  sprintf(m_sString,"CIrrOdeEventProgress: count=%i, current=%i\n",m_iCount,m_iCurrent);
  return m_sString;
}

}
}
