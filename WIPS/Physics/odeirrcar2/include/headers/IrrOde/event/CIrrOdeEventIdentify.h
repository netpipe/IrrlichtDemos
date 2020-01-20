#ifndef _C_EVENT_IDENTIFY
  #define _C_EVENT_IDENTIFY

  #include <event/IIrrOdeEvent.h>

using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

namespace irr {
namespace ode {

  #define _IRR_ODE_IDENTIFY_STRING "IrrOdeReplayFile"

class CIrrOdeEventIdentify : public IIrrOdeEvent {
  protected:
    c8 m_sIdentify[0xFF],m_sAppName[0xFF];

  public:
    CIrrOdeEventIdentify(const c8 *sAppName);
    CIrrOdeEventIdentify(CSerializer *pData);
    CIrrOdeEventIdentify(irr::ode::IIrrOdeEvent *pEvent);

    const c8 *getIdentify();
    const c8 *getAppName();

    s32 getID() { return 0; }
    virtual u16 getType() { return eIrrOdeEventIdentify; }
    virtual CSerializer *serialize();

    virtual const c8 *toString();

    virtual IIrrOdeEvent *clone() { return new CIrrOdeEventIdentify(this); }
};

}
}

#endif
