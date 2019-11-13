  #include <event/CIrrOdeEventBodyCreated.h>

namespace irr {
namespace ode {

class CIrrOdeBody;

CIrrOdeEventBodyCreated::CIrrOdeEventBodyCreated(CIrrOdeBody *pBody, s32 ID) : IIrrOdeEvent() {
  m_pBody=pBody;
  this->ID=ID;
}

CIrrOdeEventBodyCreated::CIrrOdeEventBodyCreated(CSerializer *pData, ISceneManager *pSmgr, s32 ID) : IIrrOdeEvent() {
  this->ID=ID;
  pData->resetBufferPos();
  if (pData->getU16()==eIrrOdeEventBodyCreated) {
    u32 id=pData->getU32();
    m_pBody=reinterpret_cast<CIrrOdeBody *>(pSmgr->getSceneNodeFromId(id));
  }
}

CIrrOdeEventBodyCreated::CIrrOdeEventBodyCreated(IIrrOdeEvent *pIn) : IIrrOdeEvent() {
  this->ID=pIn->getID();
  CIrrOdeEventBodyCreated *p=(CIrrOdeEventBodyCreated *)pIn;
  m_pBody=p->getBody();
}

u16 CIrrOdeEventBodyCreated::getType() {
  return eIrrOdeEventBodyCreated;
}

s32 CIrrOdeEventBodyCreated::getID() {
  return ID;
}

CIrrOdeBody *CIrrOdeEventBodyCreated::getBody() {
  return m_pBody;
}

CSerializer *CIrrOdeEventBodyCreated::serialize() {
  if (m_pSerializer==NULL) {
    m_pSerializer=new CSerializer();
    m_pSerializer->addU16(eIrrOdeEventBodyCreated);
    m_pSerializer->addU32(m_pBody?m_pBody->getID():0);
  }

  return m_pSerializer;
}

const c8 *CIrrOdeEventBodyCreated::toString() {
  sprintf(m_sString,"CIrrOdeEventBodyCreated: body=%i\n",m_pBody?m_pBody->getID():0);
  return m_sString;
}

} //namespace ode
} //namespace irr
