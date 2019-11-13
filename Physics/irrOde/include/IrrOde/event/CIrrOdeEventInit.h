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
    CIrrOdeEventInit(s32 ID=-1);
    CIrrOdeEventInit(CSerializer *pData, s32 ID=-1);

    virtual s32 getID();
    virtual u16 getType();

    virtual CSerializer *serialize();
    virtual const c8 *toString();
};

} //namespace ode
} //namespace irr

#endif

