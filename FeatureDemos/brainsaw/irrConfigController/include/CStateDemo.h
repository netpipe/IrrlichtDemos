#ifndef _C_STATE_DEMO
  #define _C_STATE_DEMO

  #include <IState.h>
  #include <irrCC.h>
  #include <irrlicht.h>

class CStateDemo : public IState, public IEventReceiver {
  private:
    CIrrCC *m_pConfigControl;   /**< the config control object */
    bool m_bNextState;          /**< should the state machine switch to the next state (i.e. quit) */

    ICameraSceneNode *m_pCam;
    ITimer *m_pTimer;
    f32 m_fRot;
    u32 m_iLastTime;

  public:
    CStateDemo(IrrlichtDevice *pDevice, CStateMachine *pStateMachine);
    virtual ~CStateDemo();
    virtual void activate(IState *pPrevious);
    virtual void deactivate(IState *pNext);
    virtual u32 update();

    void setConfigControl(CIrrCC *pCtrl);

    virtual bool OnEvent(const SEvent &event);
};

#endif

