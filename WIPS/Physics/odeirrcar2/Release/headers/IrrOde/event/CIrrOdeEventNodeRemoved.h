#ifndef _IRR_ODE_EVENT_NODE_REMOVED
  #define _IRR_ODE_EVENT_NODE_REMOVED

  #include <irrlicht.h>
  #include <event/IIrrOdeEvent.h>

namespace irr {
namespace ode {

/**
 * @class CIrrOdeEventNodeRemoved
 * This event is when a scene node is removed using CIrrOdeManager::removeSceneNode
 * @see CIrrOdeManager::removeSceneNode
 */
class CIrrOdeEventNodeRemoved : public IIrrOdeEvent {
  protected:
    s32 m_iNodeId;              /**<! node id of the removed node */
    scene::ISceneNode *m_pNode; /**<! the removed node */

  public:
    CIrrOdeEventNodeRemoved(scene::ISceneNode *pNode);
    CIrrOdeEventNodeRemoved(s32 iNodeId);
    CIrrOdeEventNodeRemoved(CSerializer *pData);
    CIrrOdeEventNodeRemoved(IIrrOdeEvent *pEvt);

    virtual u16 getType();

    /**
     * Get the removed node
     * @return the removed node. If this method returns NULL you have to get the node from the id
     */
    scene::ISceneNode *getRemovedNode();

    /**
     * Get the removed node's ID
     * @return removed node's ID
     */
    s32 getRemovedNodeId();

    virtual CSerializer *serialize();
    virtual const c8 *toString();

    virtual IIrrOdeEvent *clone() { return new CIrrOdeEventNodeRemoved(this); }

    virtual bool isObservable() { return true; }
};

} //namespace ode
} //namespace irr

#endif

