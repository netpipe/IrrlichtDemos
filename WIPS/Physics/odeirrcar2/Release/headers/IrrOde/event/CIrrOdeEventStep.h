#ifndef _IRR_ODE_EVENT_STEP
  #define _IRR_ODE_EVENT_STEP

  #include <irrlicht.h>
  #include <event/IIrrOdeEvent.h>

namespace irr {
namespace ode {

/**
 * @class CIrrOdeEventStep
 * This event is posted after each ODE step
 */
class CIrrOdeEventStep : public IIrrOdeEvent {
  protected:
    u32 m_iStepNo,
        m_iFrameNo;

  public:
    CIrrOdeEventStep(u32 iStepNo);
    CIrrOdeEventStep(CSerializer *pData);
    CIrrOdeEventStep(IIrrOdeEvent *pEvt);

    virtual u16 getType();

    void setFrameNo(u32 i) { m_iFrameNo=i; }
    u32 getFrameNo() { return m_iFrameNo; }

    u32 getStepNo() { return m_iStepNo; }

    virtual CSerializer *serialize();
    virtual const c8 *toString();

    virtual IIrrOdeEvent *clone() { return new CIrrOdeEventStep(this); }
};

} //namespace ode
} //namespace irr

#endif
