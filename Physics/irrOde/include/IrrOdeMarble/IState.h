#ifndef _I_STATE
  #define _I_STATE

  #include <irrlicht.h>

using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

/**
 * @interface IState
 * This interface is the base class for all states of the finite state machine included in this program
 */
class IState {
  protected:
    IrrlichtDevice *m_pDevice;        /**< the Irrlicht device */

    ISceneManager *m_pSceneManager;   /**< the Irrlicht scene manager */
    IVideoDriver *m_pDriver;          /**< the Irrlicht video driver */
    IGUIEnvironment *m_pGuiEnv;       /**< the Irrlicht GUI environment */
    ITimer *m_pTimer;                 /**< the timer */

  public:
    /**
     * The constructor
     * @param pDevice the Irrlicht Device of the application
     */
    IState(IrrlichtDevice *pDevice) {
      m_pDevice=pDevice;
      m_pSceneManager=m_pDevice->getSceneManager();
      m_pGuiEnv=m_pDevice->getGUIEnvironment();
      m_pDriver=m_pDevice->getVideoDriver();
      m_pTimer=m_pDevice->getTimer();
    }

    /**
     * Virtual and empty destructor
     */
    virtual ~IState() { }

    virtual void activate()=0;    /**< the "activate" method needs to be implemented by subclass */
    virtual void deactivate()=0;  /**< the "deactivate" method needs to be implemented by subclass */

    /**
     * the "update" method needs to be implemented by subclass
     * @return the index of the state to switch to +1, 0 if no state switch is to be done
     */
    virtual int update()=0;
};

#endif
