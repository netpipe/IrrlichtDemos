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
    CIrrOdeEventBodyCreated(CIrrOdeBody *pBody, s32 ID=-1);
    CIrrOdeEventBodyCreated(CSerializer *pData, ISceneManager *pSmgr, s32 ID=-1);
    CIrrOdeEventBodyCreated(IIrrOdeEvent *pIn);

    virtual s32 getID();
    virtual u16 getType();

    CIrrOdeBody *getBody();

    virtual CSerializer *serialize();
    virtual const c8 *toString();
};

} //namespace ode
} //namespace irr

#endif
