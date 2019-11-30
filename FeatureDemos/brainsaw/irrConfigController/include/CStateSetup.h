#ifndef _C_STATE_SETUP
  #define _C_STATE_SETUP

  #include <IState.h>
  #include <irrCC.h>
  #include <irrlicht.h>

class CStateSetup : public IState, public IEventReceiver {
  private:
    CIrrCC *m_pConfigControl;   /**< the config control object */
    bool m_bNextState;          /**< should the state machine switch to the next state? */

  public:
    CStateSetup(IrrlichtDevice *pDevice, CStateMachine *pStateMachine);
    virtual ~CStateSetup();
    virtual void activate(IState *pPrevious);
    virtual void deactivate(IState *pNext);
    virtual u32 update();

    void setConfigControl(CIrrCC *pCtrl);

    virtual bool OnEvent(const SEvent &event);
};

#endif
