  #include <observer/CIrrOdeEventFactory.h>

  #include <event/CIrrOdeEventInit.h>
  #include <event/CIrrOdeEventClose.h>
  #include <event/CIrrOdeEventBeforeStep.h>
  #include <event/CIrrOdeEventBodyCreated.h>
  #include <event/CIrrOdeEventBodyRemoved.h>
  #include <event/CIrrOdeEventBodyMoved.h>
  #include <event/CIrrOdeEventLoadScene.h>
  #include <event/CIrrOdeEventActivationChanged.h>
  #include <event/CIrrOdeEventNodeCloned.h>
  #include <event/CIrrOdeEventRayHit.h>
  #include <event/CIrrOdeEventStep.h>
  #include <event/CIrrOdeEventIdentify.h>
  #include <event/CIrrOdeEventNodeRemoved.h>

namespace irr {
namespace ode {

CIrrOdeDefaultEventFactory::CIrrOdeDefaultEventFactory() {
}

IIrrOdeEvent *CIrrOdeDefaultEventFactory::createMessage(CSerializer *pData, ISceneManager *pSmgr) {
  pData->resetBufferPos();
  u16 iCode=pData->getU16();
  switch (iCode) {
    case eIrrOdeEventInit             : return new CIrrOdeEventInit             (pData);
    case eIrrOdeEventClose            : return new CIrrOdeEventClose            (pData);
    case eIrrOdeEventBeforeStep       : return new CIrrOdeEventBeforeStep       (pData);
    case eIrrOdeEventStep             : return new CIrrOdeEventStep             (pData);
    case eIrrOdeEventBodyCreated      : return new CIrrOdeEventBodyCreated      (pData,pSmgr);
    case eIrrOdeEventBodyRemoved      : return new CIrrOdeEventBodyRemoved      (pData,pSmgr);
    case eIrrOdeEventBodyMoved        : return new CIrrOdeEventBodyMoved        (pData,pSmgr);
    case eIrrOdeEventActivationChanged: return new CIrrOdeEventActivationChanged(pData,pSmgr);
    case eIrrOdeEventRayHit           : return new CIrrOdeEventRayHit           (pData,pSmgr);
    case eIrrOdeEventNodeCloned       : return new CIrrOdeEventNodeCloned       (pData);
    case eIrrOdeEventLoadScene        : return new CIrrOdeEventLoadScene        (pData);
    case eIrrOdeEventIdentify         : return new CIrrOdeEventIdentify         (pData);
    case eIrrOdeEventNodeRemoved      : return new CIrrOdeEventNodeRemoved      (pData);
    default: return NULL;
  }
}

CIrrOdeEventFactory::CIrrOdeEventFactory() {
  m_pDefaultFact=new CIrrOdeDefaultEventFactory();
  m_lFactories.push_back(m_pDefaultFact);
}

CIrrOdeEventFactory::~CIrrOdeEventFactory() {
  while (m_lFactories.getSize()>0) {
    list<IIrrOdeEventFactory *>::Iterator it=m_lFactories.begin();
    IIrrOdeEventFactory *p=*it;
    m_lFactories.erase(it);
    delete p;
  }
}

CIrrOdeEventFactory *CIrrOdeEventFactory::getSharedEventFactory() {
  static CIrrOdeEventFactory cFactory;
  return &cFactory;
}

IIrrOdeEvent *CIrrOdeEventFactory::createMessage(CSerializer *pData, ISceneManager *pSmgr) {
  list<IIrrOdeEventFactory *>::Iterator it;
  pData->resetBufferPos();
  for (it=m_lFactories.begin(); it!=m_lFactories.end(); it++) {
    IIrrOdeEvent *evt=(*it)->createMessage(pData,pSmgr);
    if (evt!=NULL) return evt;
  }
  return NULL;
}

void CIrrOdeEventFactory::registerEventFactory(IIrrOdeEventFactory *p) {
  list<IIrrOdeEventFactory *>::Iterator it;
  for (it=m_lFactories.begin(); it!=m_lFactories.end(); it++) if (*it==p) return;
  m_lFactories.push_back(p);
}

void CIrrOdeEventFactory::removeEventFactory(IIrrOdeEventFactory *p) {
  list<IIrrOdeEventFactory *>::Iterator it;
  for (it=m_lFactories.begin(); it!=m_lFactories.end(); it++)
    if (*it==p) {
      IIrrOdeEventFactory *pFactory=*it;
      m_lFactories.erase(it);
      delete pFactory;
      return;
    }
}

void CIrrOdeEventFactory::setSceneManager(ISceneManager *pSmgr) {
  m_pSmgr=pSmgr;
}

}
}
