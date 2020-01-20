/*
CIrrOdeManager *CIrrOdeManager::getSharedInstance() {
  static CIrrOdeManager theManager;
  return &theManager;
}
*/
  #include <observer/CIrrOdeWorldObserver.h>
  #include <event/CIrrOdeEventBodyMoved.h>
  #include <event/CIrrOdeEventBodyRemoved.h>
  #include <event/CIrrOdeEventBodyCreated.h>
  #include <event/CIrrOdeEventNodeCloned.h>
  #include <CIrrOdeManager.h>

namespace irr {
namespace ode {

CIrrOdeWorldObserver::CIrrOdeWorldObserver() {
}

CIrrOdeWorldObserver::~CIrrOdeWorldObserver() {
  while (m_lEvents.getSize()>0) {
    list<IIrrOdeEvent *>::Iterator it=m_lEvents.begin();
    IIrrOdeEvent *p=*it;
    m_lEvents.erase(it);
    delete p;
  }
}

CIrrOdeWorldObserver *CIrrOdeWorldObserver::getSharedInstance() {
  static CIrrOdeWorldObserver theObserver;
  return &theObserver;
}

void CIrrOdeWorldObserver::install() {
  CIrrOdeManager::getSharedInstance()->getQueue()->addEventListener(this);
}

void CIrrOdeWorldObserver::destall() {
  CIrrOdeManager::getSharedInstance()->getQueue()->removeEventListener(this);
}

void CIrrOdeWorldObserver::postEvent(IIrrOdeEvent *pEvent, bool bDelete) {
  CIrrOdeEventQueue::postEvent(pEvent,bDelete);
}

void CIrrOdeWorldObserver::addEventListener(IIrrOdeEventListener *pListener) {
  list<IIrrOdeEvent *>::Iterator it;

  //when a new listener is added we need to send the current state of the simulation
  for (it=m_lEvents.begin(); it!=m_lEvents.end(); it++)
    if (pListener->handlesEvent(*it)) pListener->onEvent(*it);

  CIrrOdeEventQueue::addEventListener(pListener);
}

void CIrrOdeWorldObserver::removeEventListener(IIrrOdeEventListener *pListener) {
  CIrrOdeEventQueue::removeEventListener(pListener);
}

bool CIrrOdeWorldObserver::onEvent(IIrrOdeEvent *pEvent) {
  bool bAddToBuffer=true;

  if (pEvent->getType()==eIrrOdeEventBodyMoved) {
    //if we receieve a "body moved" event we will search the list and update
    //the data of the "body moved" event of the same body (if there is one)
    CIrrOdeEventBodyMoved *p=reinterpret_cast<CIrrOdeEventBodyMoved *>(pEvent);
    if (p) {
      list<IIrrOdeEvent *>::Iterator it;
      for (it=m_lEvents.begin(); it!=m_lEvents.end(); it++) {
        IIrrOdeEvent *pEvt=*it;
        if (pEvt->getType()==eIrrOdeEventBodyMoved) {
          CIrrOdeEventBodyMoved *pEvtMoved=(CIrrOdeEventBodyMoved *)pEvt;
          if (p->getBodyId()==pEvtMoved->getBodyId()) {
            pEvtMoved->merge(p);
            bAddToBuffer=false;
            break;
          }
        }
      }
    }
  }
  else
    //if we receive a "body removed" message we need to find out whether or not
    //the body was added during simulation time. If this is the case we need
    //to remove all hints of the body, otherwise we need to store the "body removed"
    //event
    if (pEvent->getType()==eIrrOdeEventBodyRemoved) {
      CIrrOdeEventBodyRemoved *pRemoved=(CIrrOdeEventBodyRemoved *)pEvent;
      if (pRemoved->getBodyId()!=-1) {
        bool bBodyCreated=false;
        list<IIrrOdeEvent *>::Iterator it;
        for (it=m_lEvents.begin(); it!=m_lEvents.end() && !bBodyCreated; it++) {
          if ((*it)->getType()==eIrrOdeEventNodeCloned) {
            CIrrOdeEventNodeCloned *p=(CIrrOdeEventNodeCloned *)(*it);
            bBodyCreated=p->getNewId()==pRemoved->getBodyId();
            if (bBodyCreated) {
              m_lEvents.erase(it);
              bAddToBuffer=false;
              break;
            }
          }
        }

        for (it=m_lEvents.begin(); it!=m_lEvents.end(); it++) {
          if ((*it)->getType()==eIrrOdeEventBodyMoved) {
            CIrrOdeEventBodyMoved *p=(CIrrOdeEventBodyMoved *)(*it);
            if (p->getBodyId()==pRemoved->getBodyId()) {
              m_lEvents.erase(it);
              break;
            }
          }
        }
      }
    }

  if (pEvent->isObservable() && bAddToBuffer) {
    IIrrOdeEvent *pNewEvent=pEvent->clone();
    if (pNewEvent!=NULL) {
      m_lEvents.push_back(pNewEvent);

    }
  }

  postEvent(pEvent,false);
  return true;
}

bool CIrrOdeWorldObserver::handlesEvent(IIrrOdeEvent *pEvent) {
  return true;
}

}
}
