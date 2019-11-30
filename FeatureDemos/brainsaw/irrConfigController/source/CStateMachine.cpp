  #include <CStateMachine.h>
  #include <CStateSetup.h>
  #include <CStateDemo.h>

CStateMachine::CStateMachine() {
  m_pActive=NULL;
}


CStateMachine::~CStateMachine() {
}

void CStateMachine::initStates(IrrlichtDevice *pDevice) {
  //load another (bigger) font
  pDevice->getGUIEnvironment()->getSkin()->setFont(m_pGuienv->getFont("data/fontcourier.bmp"));

  //create both steps of this demo
  CStateSetup *pSetup=new CStateSetup(pDevice,this); m_aStates.push_back(pSetup);
  CStateDemo  *pDemo =new CStateDemo (pDevice,this); m_aStates.push_back(pDemo );

  //create an instance of CIrrCC and ...
  m_pConfigControl=new CIrrCC(pDevice);
  //... add some controller items
  m_pConfigControl->addItem(0,stringw("move foreward"),KEY_KEY_W,CIrrCC::eCtrlAxis);
  m_pConfigControl->addItem(0,stringw("move backward"),KEY_KEY_S,CIrrCC::eCtrlAxis);
  m_pConfigControl->addItem(0,stringw("move left"    ),KEY_KEY_A,CIrrCC::eCtrlAxis);
  m_pConfigControl->addItem(0,stringw("move right"   ),KEY_KEY_D,CIrrCC::eCtrlAxis);
  m_pConfigControl->addItem(0,stringw("turn left"    ),KEY_LEFT ,CIrrCC::eCtrlAxis);
  m_pConfigControl->addItem(0,stringw("turn right"   ),KEY_RIGHT,CIrrCC::eCtrlAxis);
  m_pConfigControl->addItem(0,stringw("move up"      ),KEY_UP   ,CIrrCC::eCtrlAxis);
  m_pConfigControl->addItem(0,stringw("move down"    ),KEY_DOWN ,CIrrCC::eCtrlAxis);

  m_pConfigControl->createAxis(0,1);
  m_pConfigControl->createAxis(2,3);
  m_pConfigControl->createAxis(4,5);
  m_pConfigControl->createAxis(6,7);

  //submit the instance to both states
  pSetup->setConfigControl(m_pConfigControl);
  pDemo ->setConfigControl(m_pConfigControl);

  //load the controller settings
  m_pConfigControl->load("data/control.xml");

  //active the first state
  m_pActive=pSetup;
}

void CStateMachine::clearStates() {
  for (u32 i=0; i<m_aStates.size(); i++) delete m_aStates[i];
  m_aStates.clear();
}

void CStateMachine::run(IrrlichtDevice *pDevice) {
  bool bQuit=false;

  //set the irrlicht objects
  m_pDevice=pDevice;

  m_pDriver=m_pDevice->getVideoDriver();
  m_pSmgr  =m_pDevice->getSceneManager();
  m_pGuienv=m_pDevice->getGUIEnvironment();

  //init the states
  initStates(m_pDevice);

  //activate the first state
  if (m_pActive) m_pActive->activate(NULL);

  //run the program
  while(m_pDevice->run() && !bQuit) {
    m_pDriver->beginScene(true,true,SColor(0,200,200,200));

    m_pSmgr->drawAll();
    m_pGuienv->drawAll();

    m_pDriver->endScene();

    u32 iRet=m_pActive?m_pActive->update():0;

    //if the value returned by the active state is bigger than "0", ...
    if (iRet) {
      //... decrease this value by 1 to get the next state's index and ...
      iRet--;
      if (iRet<m_aStates.size()) {
        //... get the new state, activate it and deactivate the old state or ...
        IState *pNext=m_aStates[iRet];
        m_pActive->deactivate(pNext);
        pNext->activate(m_pActive);
        m_pActive=pNext;
      }
      else bQuit=true;  //... quit the program if the index of the new state is too big
    }
  }

  //deactivate the active state
  if (m_pActive) m_pActive->deactivate(NULL);
  m_pDevice->closeDevice();
}
