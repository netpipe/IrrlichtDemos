#ifndef _C_IRR_ODE_MESSAGE_FACTORY
  #define _C_IRR_ODE_MESSAGE_FACTORY

  #include <observer/IEventFactory.h>

namespace irr {
namespace ode {

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

/**
 * @class CIrrOdeDefaultEventFactory
 * @author Christian Keimel / bulletbyte.de
 * This is the standard event factory. It is created by default and creates all IrrOde internal
 * events.
 * @see IEventFactory
 */
class CIrrOdeDefaultEventFactory : public IIrrOdeEventFactory {
  public:
    CIrrOdeDefaultEventFactory();
    virtual IIrrOdeEvent *createMessage(CSerializer *pData, ISceneManager *pSmgr);
};

/**
 * @class CIrrOdeEventFactory
 * @author Christian Keimel / bulletbyte.de
 * This singleton class is the event factory that is used to deserialize events that come
 * from a recording or from the network. Due to performance reasons it's not a good idea
 * to use this class for normal event generation.
 * @see CIrrOdeDefaultEventFactory
 */
class CIrrOdeEventFactory {
  protected:
    ISceneManager *m_pSmgr;                       /**<! the scene manager to use */
    CIrrOdeDefaultEventFactory *m_pDefaultFact;   /**< the default event factory */
    list<IIrrOdeEventFactory *> m_lFactories;     /**< a list of all registered event factories */
    CIrrOdeEventFactory();                        /**< protected constructor */
    ~CIrrOdeEventFactory();                       /**< protected destructor */

  public:
    /**
     * Get the shared instance
     * @return the shared instance
     */
    static CIrrOdeEventFactory *getSharedEventFactory();

    /**
     * Create a message from serialized data
     * @param pData the serialized data
     * @param pSmgr the scene manager to use
     */
    IIrrOdeEvent *createMessage(CSerializer *pData, ISceneManager *pSmgr);

    /**
     * Register an event factory. The event factory will be deleted on removal.
     * @param p the event factory to register
     * @see IEventFactory
     */
    void registerEventFactory(IIrrOdeEventFactory *p);

    /**
     * Remove a registered event factory. The event factory is deleted by this method.
     * @param p the event factory to remove
     */
    void removeEventFactory(IIrrOdeEventFactory *p);

    /**
     * Set the scene manager to use
     * @param pSmgr the scene manager to use
     */
    void setSceneManager(ISceneManager *pSmgr);

    /**
     * Get the registered scene manager
     * @return the registered scene manager
     */
    ISceneManager *getSceneManager() { return m_pSmgr; }
};

}
}

#endif
