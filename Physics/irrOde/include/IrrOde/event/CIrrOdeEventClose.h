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
    CIrrOdeEventClose(s32 ID=-1);
    CIrrOdeEventClose(CSerializer *pData) : IIrrOdeEvent() { }

    virtual s32 getID();
    virtual u16 getType();

    virtual CSerializer *serialize();
    virtual const c8 *toString();
};

} //namespace ode
} //namespace irr

#endif


