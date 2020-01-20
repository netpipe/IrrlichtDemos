  #include <event/CIrrOdeEventBodyRemoved.h>
  #include <CIrrOdeManager.h>
  #include <IIrrOdeDevice.h>

namespace irr {
namespace ode {

class CIrrOdeBody;

CIrrOdeEventBodyRemoved::CIrrOdeEventBodyRemoved(CIrrOdeBody *pBody) : IIrrOdeEvent() {
  m_pBody=pBody;
  m_iBodyId=pBody?pBody->getID():0;
}

CIrrOdeEventBodyRemoved::CIrrOdeEventBodyRemoved(CSerializer *pData, ISceneManager *pSmgr) : IIrrOdeEvent() {
  pData->resetBufferPos();
  if (pData->getU16()==eIrrOdeEventBodyRemoved) {
    m_iBodyId=pData->getU32();
    m_pBody=reinterpret_cast<CIrrOdeBody *>(pSmgr->getSceneNodeFromId(m_iBodyId));
    if (m_pBody) {
      CIrrOdeManager::getSharedInstance()->getOdeDevice()->bodyDestroy(m_pBody->getBodyId());
      if (m_pBody->getWorld()) m_pBody->getWorld()->removeBody(m_pBody);
    }
  }
}

CIrrOdeEventBodyRemoved::CIrrOdeEventBodyRemoved(IIrrOdeEvent *pIn) : IIrrOdeEvent() {
  if (pIn->getType()==eIrrOdeEventBodyRemoved) {
    CIrrOdeEventBodyRemoved *p=(CIrrOdeEventBodyRemoved *)pIn;
    m_pBody=p->getBody();
    m_iBodyId=p->getBodyId();
  }
}

u16 CIrrOdeEventBodyRemoved::getType() {
  return eIrrOdeEventBodyRemoved;
}

CIrrOdeBody *CIrrOdeEventBodyRemoved::getBody() {
  return m_pBody;
}

CSerializer *CIrrOdeEventBodyRemoved::serialize() {
  if (m_pSerializer==NULL) {
    m_pSerializer=new CSerializer();
    m_pSerializer->addU16(eIrrOdeEventBodyRemoved);
    m_pSerializer->addU32(m_iBodyId);
  }

  return m_pSerializer;
}

const c8 *CIrrOdeEventBodyRemoved::toString() {
  sprintf(m_sString,"CIrrOdeEventBodyRemoved: %i",m_iBodyId);
  return m_sString;
}

} //namespace ode
} //namespace irr
