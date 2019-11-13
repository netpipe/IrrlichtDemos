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
    CIrrOdeEventBeforeStep(s32 ID=-1);
    CIrrOdeEventBeforeStep(IIrrOdeEvent *pIn);
    CIrrOdeEventBeforeStep(CSerializer *pData) : IIrrOdeEvent() { }

    virtual s32 getID();
    virtual u16 getType();

    virtual CSerializer *serialize();

    virtual const c8 *toString();
};

} //namespace ode
} //namespace irr

#endif
