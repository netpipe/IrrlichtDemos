  #include <CStateSetup.h>

CStateSetup::CStateSetup(IrrlichtDevice *pDevice, CStateMachine *pStateMachine) : IState(pDevice,pStateMachine) {
}

CStateSetup::~CStateSetup() {
}

void CStateSetup::activate(IState *pPrevious) {
  m_bNextState=false;   //do not (yet) switch to the "demo" state
  m_pConfigControl->addGui(0,NULL,position2di(10,50));  //create the gui to configure the controls
  m_pGuienv->addButton(rect<s32>(10,400,150,420),NULL,23,L"Start Demo");  //add the "start demo" button
  m_pDevice->setEventReceiver(this);  //set this state as Irrlicht event receiver
}

void CStateSetup::deactivate(IState *pNext) {
  m_pGuienv->clear(); //clear the gui environment
}

u32 CStateSetup::update() {
  return m_bNextState?2:0;    //return 0 if the "start demo" button was not pressed, index of the demo state +1 otherwise
}

void CStateSetup::setConfigControl(CIrrCC *pCtrl) {
  m_pConfigControl=pCtrl;
}

bool CStateSetup::OnEvent(const SEvent &event) {
  bool bRet=m_pConfigControl->ConfigEvent(event);
  if (!bRet && event.EventType==EET_GUI_EVENT && event.GUIEvent.Caller->getID()==23 && event.GUIEvent.EventType==EGET_BUTTON_CLICKED) {
    m_bNextState=true;
  }
  return bRet;
}
