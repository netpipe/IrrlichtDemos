#ifndef _C_IRR_ODE_WORLD_OBSERVER
  #define _C_IRR_ODE_WORLD_OBSERVER

  #include <event/CIrrOdeEventQueue.h>
  #include <event/IIrrOdeEventListener.h>

namespace irr {
namespace ode {

class IIrrOdeEvent;

/**
 * @class CIrrOdeWorldObserver
 * This class is used for nice features like the IrrOdeRecorder and IrrOdeNet. The purpose of
 * this class is to keep track of all changes that happen in the simulated world. Whenever a
 * new listener registeres the necessary events to reflect this state will be sent
 * @author Christian Keimel / dustbin::games
 */
class CIrrOdeWorldObserver : public CIrrOdeEventQueue, public IIrrOdeEventListener {
  protected:
    /** the events that reflect the current state of the world */
    list<IIrrOdeEvent *> m_lEvents;

    /** protected constructor */
    CIrrOdeWorldObserver();

    /** protected destructor */
    virtual ~CIrrOdeWorldObserver();

  public:
    /**
     * get the shared instance of the observer
     * @return the shared instance of the observer
     */
    static CIrrOdeWorldObserver *getSharedInstance();

    /** install the observer. Registeres the shared instance as event listener */
    void install();
    /** destall the observer. Removes the shared instancfe as event listener */
    void destall();

    //implementation of CIrrOdeEventQueue methods
    virtual void postEvent(IIrrOdeEvent *pEvent, bool bDelete=true);

    virtual void addEventListener(IIrrOdeEventListener *pListener);
    virtual void removeEventListener(IIrrOdeEventListener *pListener);

    //implementation of IIrrOdeEventListener methods
    virtual bool onEvent(IIrrOdeEvent *pEvent);
    virtual bool handlesEvent(IIrrOdeEvent *pEvent);

    u32 getEventCount() { return m_lEvents.getSize(); }
};

}
}
#endif
