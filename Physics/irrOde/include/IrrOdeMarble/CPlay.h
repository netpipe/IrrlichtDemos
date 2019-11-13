#ifndef _C_PLAY
  #define _C_PLAY

  #include <IState.h>
  #include <CHiScore.h>
  #include <irrlicht.h>

  #include <IrrOde.h>

using namespace ode;

//The states the game can have
enum _GAME_STATES {
  _STATE_PLAYING,     /**< game is currently running */
  _STATE_GAMEOVER,    /**< game was ended */
  _STATE_HIGHSCORE,   /**< highscore is shown */
  _STATE_ENTER_NAME,  /**< user is entering his/her name */
  _STATE_DONE         /**< it's finished */
};

/**
 * @class CPlay
 * This class is the core of this application, the actual game
 * @see IState
 */
class CPlay : public IState, public IEventReceiver, public IIrrOdeEventListener {
  private:
    ICameraSceneNode *m_pCam;   /**< the marble's cam */
    CIrrOdeBody *m_pMarble;     /**< the marble */
    bool m_bEndGame,            /**< was the game ended? */
         m_bRunning,            /**< is the game still running? */
         m_bMouseLeft,          /**< left mouse button pressed? */
         m_bMouseRght;          /**< right mouse button pressed? */
    ICursorControl *m_pCtrl;    /**< Irrlicht's cursor control */
    f32 m_fCamRot,              /**< horizontal angle of the camera */
        m_fCamHi,               /**< vertical angle of the camera */
        m_fCamDist;             /**< distance from the marble to the camera */
    _GAME_STATES m_iGameState;  /**< the current game state */
    u32 m_iStartTime,           /**< the start time of the current game */
        m_iGameOverTime;        /**< the end time of the current game */
    s32 m_iCheckPoint;          /**< the index of the next checkpoint to reach */

    list<CIrrOdeGeom *> m_lCheckpoints;               /**< list of all checkpoints */
    list<CIrrOdeGeom *>::Iterator m_pThisCheckpoint,  /**< the next checkpoint */
                                  m_pLastCheckpoint;  /**< the last checkpoint */
    array<IGUIStaticText *>m_aCheckText;              /**< array of text fields to show the checkpoint times */
    IGUIFont *m_pFont;                                /**< the font for the text fields */
    HiScore *m_pHiScore;                              /**< the hiscore object */

	CIrrOdeManager *m_pOdeManager;

  public:
    CPlay(IrrlichtDevice *pDevice);
    virtual ~CPlay();

    virtual void activate();    /**< implementation of IState::activate */
    virtual void deactivate();  /**< implementation of IState::deactivate */
    virtual int update();       /**< implementation of IState::update */

    /**
     * callback for the Irrlicht event pipeline
     * @param evt the event to be handled
     * @return "true" if the event was handled, "false" otherwise
     */
    virtual bool OnEvent(const SEvent &evt);

    /**
     * callback for IrrODE's event pipeline
     * @param pEvent the event to be handled
     * @return does not matter, ignored
     */
    virtual bool onEvent(IIrrOdeEvent *pEvent);

    /**
     * callback for IrrODE's event pipeline.
     * @param pEvent the event in question
     * @return "true" if this receiver handles this kind of events, "false" otherwise
     */
    virtual bool handlesEvent(IIrrOdeEvent *pEvent);
};

#endif
