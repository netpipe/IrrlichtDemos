#ifndef _IRR_ODE_EVENT_NODE_CLONED
  #define _IRR_ODE_EVENT_NODE_CLONED

  #include <irrlicht.h>
  #include <event/IIrrOdeEvent.h>

namespace irr {
namespace ode {

/**
 * @class CIrrOdeEventNodeCloned
 * This event gets posted if a node is cloned
 */
class CIrrOdeEventNodeCloned : public IIrrOdeEvent {
  protected:
    s32 m_iSourceId,
        m_iNewId;

  public:
    CIrrOdeEventNodeCloned(s32 iSourceId, s32 iNewId);
    CIrrOdeEventNodeCloned(CSerializer *pData);
    CIrrOdeEventNodeCloned(IIrrOdeEvent *pEvent);

    virtual u16 getType();

    void setSourceId(s32 iId) { m_iSourceId=iId; }
    void setNewId(s32 iId) { m_iNewId=iId; }

    s32 getSourceId() { return m_iSourceId; }
    s32 getNewId() { return m_iNewId; }

    virtual CSerializer *serialize();
    virtual const c8 *toString();

    virtual IIrrOdeEvent *clone() { return new CIrrOdeEventNodeCloned(this); }

    virtual bool isObservable() { return true; }
};

} //namespace ode
} //namespace irr

#endif

