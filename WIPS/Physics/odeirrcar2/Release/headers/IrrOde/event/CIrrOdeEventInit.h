#ifndef _IRR_ODE_EVENT_INIT
  #define _IRR_ODE_EVENT_INIT

  #include <irrlicht.h>
  #include <event/IIrrOdeEvent.h>

namespace irr {
namespace ode {

/**
 * @class CIrrOdeEventInit
 * This event is posted when initODE is called
 */
class CIrrOdeEventInit : public IIrrOdeEvent {
  protected:

  public:
    CIrrOdeEventInit();
    CIrrOdeEventInit(CSerializer *pData);

    virtual u16 getType();

    virtual CSerializer *serialize();
    virtual const c8 *toString();

    virtual IIrrOdeEvent *clone() { return new CIrrOdeEventInit(); }
};

} //namespace ode
} //namespace irr

#endif

