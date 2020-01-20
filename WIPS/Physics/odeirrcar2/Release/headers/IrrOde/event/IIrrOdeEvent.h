#ifndef _IRR_ODE_EVENT
  #define _IRR_ODE_EVENT

  #include <irrlicht.h>
  #include <CSerializer.h>

namespace irr {
namespace ode {

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

/**
 * This enumeration holds the IDs of the IrrOde Events
 */
enum enumIrrOdeEvents {
  eIrrOdeEventNoEvent,            /*!<no event */
  eIrrOdeEventInit,               /*!<"ode initialized" event */
  eIrrOdeEventClose,              /*!<"ode closed" event */
  eIrrOdeEventBeforeStep,         /*!<"before step" event */
  eIrrOdeEventStep,               /*!<"after step" event */
  eIrrOdeEventBodyCreated,        /*!<"body created" event */
  eIrrOdeEventBodyRemoved,        /*!<"body removed" event */
  eIrrOdeEventBodyMoved,          /*!<"body moved" event */
  eIrrOdeEventActivationChanged,  /*!<"body activation changed" event */
  eIrrOdeEventRayHit,             /*!<"ray hit" event */
  eIrrOdeEventProgress,           /*!<"progress" event */
  eIrrOdeEventNodeCloned,         /*!<"node cloned" event */
  eIrrOdeEventLoadScene,          /*!<"load scene" event */
  eIrrOdeEventIdentify,           /*!<"identify" event for IrrOde recorder */
  eIrrOdeEventNodeRemoved,        /*!<"delete scene node" event */

  eIrrOdeEventUser=256,           /*!< user defined event IDs start at 256 */
  eIrrOdeEvent16Bit=0xFFFF        /*!< force the enum to be 16 bits */
};

class CSerializer;

/**
 * @interface IIrrOdeEvent
 * This is the base class for all IrrODE events
 */
class IIrrOdeEvent {
  protected:
    CSerializer *m_pSerializer;
    c8 m_sString[1000];

  public:
    IIrrOdeEvent() { m_pSerializer=NULL; }
    virtual ~IIrrOdeEvent() { if (m_pSerializer!=NULL) delete m_pSerializer; }

    virtual u16 getType()=0;

    void resetSerializer() { if (m_pSerializer) { delete m_pSerializer; m_pSerializer=NULL; } }
    virtual CSerializer *serialize()=0;

    virtual const c8 *toString()=0;

    virtual IIrrOdeEvent *clone()=0;

    virtual bool isObservable() { return false; }
};

} //namespace ode
} //namespace irr

#endif
