#ifndef _IRR_ODE_EVENT_BEFORE_STEP
  #define _IRR_ODE_EVENT_BEFORE_STEP

  #include <irrlicht.h>
  #include <event/IIrrOdeEvent.h>

namespace irr {
namespace ode {

/**
 * @class CIrrOdeEventBeforeStep
 * This event is posted before each ODE step
 */
class CIrrOdeEventBeforeStep : public IIrrOdeEvent {
  protected:

  public:
    CIrrOdeEventBeforeStep();
    CIrrOdeEventBeforeStep(IIrrOdeEvent *pIn);
    CIrrOdeEventBeforeStep(CSerializer *pData) : IIrrOdeEvent() { }

    virtual u16 getType();

    virtual CSerializer *serialize();

    virtual const c8 *toString();

    virtual IIrrOdeEvent *clone() { return new CIrrOdeEventBeforeStep(); }
};

} //namespace ode
} //namespace irr

#endif
