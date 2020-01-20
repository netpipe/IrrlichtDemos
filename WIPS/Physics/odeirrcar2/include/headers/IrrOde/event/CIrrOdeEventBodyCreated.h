#ifndef _IRR_ODE_EVENT_BODY_CREATED
  #define _IRR_ODE_EVENT_BODY_CREATED

  #include <irrlicht.h>
  #include <event/IIrrOdeEvent.h>
  #include <CIrrOdeBody.h>
  #include <geom/CIrrOdeGeom.h>

namespace irr {
namespace ode {

/**
 * @class CIrrOdeEventBodyCreated
 * This event is posted whenever a new body is created
 */
class CIrrOdeEventBodyCreated : public IIrrOdeEvent {
  protected:
    CIrrOdeBody *m_pBody; /*!< the newly created body */

  public:
    CIrrOdeEventBodyCreated(CIrrOdeBody *pBody);
    CIrrOdeEventBodyCreated(CSerializer *pData, ISceneManager *pSmgr);
    CIrrOdeEventBodyCreated(IIrrOdeEvent *pIn);

    virtual u16 getType();

    CIrrOdeBody *getBody();

    virtual CSerializer *serialize();
    virtual const c8 *toString();

    virtual IIrrOdeEvent *clone() { return new CIrrOdeEventBodyCreated(this); }
};

} //namespace ode
} //namespace irr

#endif
