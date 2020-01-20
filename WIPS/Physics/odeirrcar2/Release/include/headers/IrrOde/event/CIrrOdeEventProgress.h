#ifndef _IRR_ODE_EVENT_PROGRESS
  #define _IRR_ODE_EVENT_PROGRESS

  #include <irrlicht.h>
  #include <event/IIrrOdeEvent.h>

namespace irr {
namespace ode {

/**
 * @class CIrrOdeEventProgress
 * This event is posted after every initialization step in the initial initPhysics
 */
class CIrrOdeEventProgress : public IIrrOdeEvent {
  protected:
    u32 m_iCount,   /*!< the total number of necessary initialization steps */
        m_iCurrent; /*!< the current initialization step */

  public:
    /**
     * Creates this event.
     * @param iCurrent the current progress number
     * @param iCount the total progress number
     */
    CIrrOdeEventProgress(u32 iCurrent, u32 iCount);
    CIrrOdeEventProgress(IIrrOdeEvent *pIn);
    CIrrOdeEventProgress(CSerializer *pData);

    virtual u16 getType();

    virtual CSerializer *serialize();

    virtual const c8 *toString();

    u32 getCurrent() { return m_iCurrent; }
    u32 getCount() { return m_iCount; }

    virtual IIrrOdeEvent *clone() { return new CIrrOdeEventProgress(this); }
};

} //namespace ode
} //namespace irr

#endif

