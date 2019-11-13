#ifndef _C_ABOUT
  #define _C_ABOUT

  #include <CHiScore.h>
  #include "IState.h"

/**
 * @class CAbout
 * This is the "about" screen and the highscore table
 * @see IState
 */
class CAbout : public IState, public IEventReceiver {
  private:
    bool m_bClose,        /**< should the state be closed? */
         m_bRunning;      /**< is the state up and running? */
    ITexture *m_pImage;   /**< the info image-texture */
    IGUIImage *m_pImg;    /**< the info image */
    u32 m_iState;         /**< the state of this state (info, highscore */
    HiScore *m_pHiScore;  /**< the Hiscore object */

  public:
    /**
     * The constructor
     * @param pDevice the Irrlicht device to be used
     */
    CAbout(IrrlichtDevice *pDevice);

    /**
     * the destructor
     */
    virtual ~CAbout();

    /**
     * This is an implementation of IState::activate. This time not much has to be done, the image is loaded,
     * the event receiver is set and the highscore is loaded
     * @see IState::activate
     */
    virtual void activate();

    /**
     * Deactivate the state. Here we clear the scene, the GUI and destroy the hiscore object
     */
    virtual void deactivate();

    /**
     * Just check if the state should be ended
     */
    virtual int update();

    /**
     * The event receiver for the Irrlicht event pipeline. The two states of this class (about, hiscore)
     * are switched with any key. If a key is pressed when the highscore is up the state is ended,
     * and the first key release is ignored (this release would lead back to the main menu way to early.
     * @param evt the event to be handled
     */
    virtual bool OnEvent(const SEvent &evt);
};

#endif

