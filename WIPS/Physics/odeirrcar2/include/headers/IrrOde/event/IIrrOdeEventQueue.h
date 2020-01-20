#ifndef _I_IRR_ODE_EVENT_QUEUE
  #define _I_IRR_ODE_EVENT_QUEUE

  #include <irrlicht.h>

namespace irr {
namespace ode {

class IIrrOdeEvent;
class IIrrOdeEventListener;
class CIrrOdeManager;

class IIrrOdeEventQueue {
  public:
    virtual ~IIrrOdeEventQueue() { }

    virtual void postEvent(IIrrOdeEvent *pEvent, bool bDelete=true)=0;

    virtual void addEventListener(IIrrOdeEventListener *p)=0;
    virtual void removeEventListener(IIrrOdeEventListener *p)=0;
};

}
}
#endif

