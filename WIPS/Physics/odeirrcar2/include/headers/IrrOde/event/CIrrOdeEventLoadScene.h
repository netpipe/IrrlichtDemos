#ifndef _IRR_ODE_EVENT_LOAD_SCENE
  #define _IRR_ODE_EVENT_LOAD_SCENE

  #include <irrlicht.h>
  #include <event/IIrrOdeEvent.h>

namespace irr {
namespace ode {

/**
 * @class CIrrOdeEventLoadScene
 * This event is posted before each ODE step
 */
class CIrrOdeEventLoadScene : public IIrrOdeEvent {
  protected:
    stringc m_sScene;

  public:
    CIrrOdeEventLoadScene(const c8 *sScene);
    CIrrOdeEventLoadScene(IIrrOdeEvent *pIn);
    CIrrOdeEventLoadScene(CSerializer *pData);

    virtual u16 getType();

    virtual CSerializer *serialize();

    virtual const c8 *toString();

    const c8 *getScene();

    virtual IIrrOdeEvent *clone() { return new CIrrOdeEventLoadScene(this); }

    virtual bool isObservable() { return true; }
};

} //namespace ode
} //namespace irr

#endif

