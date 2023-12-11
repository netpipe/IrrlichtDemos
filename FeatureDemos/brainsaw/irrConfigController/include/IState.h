#ifndef _I_STATE
  #define _I_STATE

  #include <irrlicht.h>

using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

class CStateMachine;

/**
 * @class IState
 * This is the base class for all states the state machine handles
 */
class IState {
  protected:
    IrrlichtDevice  *m_pDevice;     /**< the irrlicht device */
    IVideoDriver    *m_pDriver;     /**< the irrlicht video driver */
    ISceneManager   *m_pSmgr;       /**< the irrlicht scene manager */
    IGUIEnvironment *m_pGuienv;     /**< the irrlicht gui environment */

    CStateMachine *m_pStateMachine; /**< the state machine this state belongs to */

  public:
    /**
     * The constructor
     * @param pDevice the irrlicht device
     * @param pStateMachine the state machine the state belongs to
     */
    IState(IrrlichtDevice *pDevice, CStateMachine *pStateMachine) {
      m_pDevice=pDevice;
      m_pDriver=m_pDevice->getVideoDriver();
      m_pSmgr  =m_pDevice->getSceneManager();
      m_pGuienv=m_pDevice->getGUIEnvironment();

      m_pStateMachine=pStateMachine;
    }

    /**
     * The destructor
     */
    virtual ~IState() { }

    /**
     * This method is called when a state is activated. You can use this method e.g. to load a scene
     * @param pPrevious the state that is stopped
     */
    virtual void activate(IState *pPrevious)=0;

    /**
     * This method is called when a state is stopped. You can use this method e.g. to clear your scene
     * @param pNext the next state that runs
     */
    virtual void deactivate(IState *pNext)=0;

    /**
     * This method is called once per frame and can be used to do something
     * @return "0" if no state change is needed, The index of the new state +1 otherwise
     */
    virtual u32 update()=0;
};
#endif
