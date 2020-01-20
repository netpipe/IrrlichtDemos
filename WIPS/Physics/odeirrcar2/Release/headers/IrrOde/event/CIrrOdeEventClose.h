#ifndef _IRR_ODE_EVENT_CLOSE
  #define _IRR_ODE_EVENT_CLOSE

  #include <irrlicht.h>
  #include <event/IIrrOdeEvent.h>

namespace irr {
namespace ode {

/**
 * @class CIrrOdeEventClose
 * This event is posted after the ODE was closed
 */
class CIrrOdeEventClose : public IIrrOdeEvent {
  protected:

  public:
    CIrrOdeEventClose();
    CIrrOdeEventClose(CSerializer *pData) : IIrrOdeEvent() { }

    virtual u16 getType();

    virtual CSerializer *serialize();
    virtual const c8 *toString();

    virtual IIrrOdeEvent *clone() { return new CIrrOdeEventClose(); }
};

} //namespace ode
} //namespace irr

#endif


