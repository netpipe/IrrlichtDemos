#ifndef _IRR_ODE_EVENT_BODY_REMOVED
  #define _IRR_ODE_EVENT_BODY_REMOVED

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
class CIrrOdeEventBodyRemoved : public IIrrOdeEvent {
  protected:
    CIrrOdeBody *m_pBody; /*!< the removed body */
    u32 m_iBodyId;        /*!< id of the removed body */

  public:
    CIrrOdeEventBodyRemoved(CIrrOdeBody *pBody, s32 ID=-1);
    CIrrOdeEventBodyRemoved(CSerializer *pData, ISceneManager *pSmgr, s32 ID=-1);
    CIrrOdeEventBodyRemoved(IIrrOdeEvent *pIn);

    virtual s32 getID();
    virtual u16 getType();

    CIrrOdeBody *getBody();
    u32 getBodyId() { return m_iBodyId; }

    virtual CSerializer *serialize();
    virtual const c8 *toString();
};

} //namespace ode
} //namespace irr

#endif
