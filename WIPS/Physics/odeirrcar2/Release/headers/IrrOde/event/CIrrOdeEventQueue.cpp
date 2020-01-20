  #include <event/CIrrOdeEventQueue.h>
  #include <event/IIrrOdeEventListener.h>

namespace irr {
namespace ode {

CIrrOdeEventQueue::CIrrOdeEventQueue() {
}

CIrrOdeEventQueue::~CIrrOdeEventQueue() {
  m_pListeners.clear();
}

void CIrrOdeEventQueue::postEvent(IIrrOdeEvent *pEvent, bool bDelete) {
  core::list<IIrrOdeEventListener *>::Iterator i;
  for (i=m_pListeners.begin(); i!=m_pListeners.end(); i++) {
    if ((*i)->handlesEvent(pEvent))
      (*i)->onEvent(pEvent);
  }
  if (bDelete) delete pEvent;
}

void CIrrOdeEventQueue::addEventListener(IIrrOdeEventListener *pListener) {
	core::list<IIrrOdeEventListener *>::Iterator it;
	for (it=m_pListeners.begin(); it!=m_pListeners.end(); it++)
	  if (*it==pListener)
			return;

  m_pListeners.push_back(pListener);
}

void CIrrOdeEventQueue::removeEventListener(IIrrOdeEventListener *pListener) {
	list<IIrrOdeEventListener *>::Iterator it;
	for (it=m_pListeners.begin(); it!=m_pListeners.end(); it++)
	  if (*it==pListener) {
			m_pListeners.erase(it);
			return;
	  }
}

}
}
