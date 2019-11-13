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
    CIrrOdeEventStep(u32 iStepNo, s32 ID=-1);
    CIrrOdeEventStep(CSerializer *pData, s32 ID=-1);

    virtual s32 getID();
    virtual u16 getType();

    void setFrameNo(u32 i) { m_iFrameNo=i; }
    u32 getFrameNo() { return m_iFrameNo; }

    u32 getStepNo() { return m_iStepNo; }

    virtual CSerializer *serialize();
    virtual const c8 *toString();
};

} //namespace ode
} //namespace irr

#endif
