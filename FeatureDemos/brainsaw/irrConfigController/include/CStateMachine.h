#ifndef _C_STATE_MACHINE
  #define _C_STATE_MACHINE

  #include <irrlicht.h>
  #include <IState.h>
  #include <irrCC.h>

using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

/**
 * @class CStateMachine
 * @brief my finite state machine
 * This class handles various game states
 */
class CStateMachine {
  protected:
    array<IState *> m_aStates;    /**< the states of this machine */

    IState *m_pActive;            /**< the currently active state */

    IrrlichtDevice  *m_pDevice;   /**< the irrlicht device */
    IVideoDriver    *m_pDriver;   /**< the irrlicht video driver */
    ISceneManager   *m_pSmgr;     /**< the irrlicht scene manager */
    IGUIEnvironment *m_pGuienv;   /**< the irrlicht gui environment */

    CIrrCC *m_pConfigControl;   /**< the CIrrCC instance */

  public:
    CStateMachine();            /**< the constructor */
    virtual ~CStateMachine();   /**< the destructor */

    /**
     * In this method all states of the machine have to be initialized. This is called by the state machine
     * before it actually starts running
     * @param pDevice the irrlicht device
     */
    virtual void initStates(IrrlichtDevice *pDevice);

    /**
     * Destroy all states and clear the states array. This method is called when the machine stop running
     */
    virtual void clearStates();

    /**
     * The actual run loop of the machine
     * @param pDevice the irrlicht device
     */
    void run(IrrlichtDevice *pDevice);
};
#endif
