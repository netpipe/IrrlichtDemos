  #include <CMenu.h>


CMenu::CMenu(IrrlichtDevice *pDevice) : IState(pDevice) {
  m_iActiveItem=0;
}

CMenu::~CMenu() {
}

/**
 * The activate method. In this method we load the scene of the menu, add a camera, set the
 * state as Irrlicht Event Receiver and do some more initialization. This method is called
 * whenever the state of our finite state machine reaches the "menu" state
 * @see IState::activate
 */
void CMenu::activate() {
  m_iMenuMove=0;
  m_bSelect=false;
  m_fItemHeight=2.5f;
  m_iLastTime=m_pTimer->getTime();

  m_pDevice->getSceneManager()->loadScene("../../data/irrOdeMarble_menu.xml");
  m_pDevice->getSceneManager()->addCameraSceneNode(NULL,vector3df(0.0f,0.0f,-7.5f),vector3df(0.0f,0.0f,0.0f));

  m_pDevice->setEventReceiver(this);

  m_pMenu=reinterpret_cast<IAnimatedMeshSceneNode *>(m_pSceneManager->getSceneNodeFromName("theMenu"));

  m_pDevice->getCursorControl()->setVisible(false);
  m_pMenu->setPosition(vector3df(0.0f,-m_fItemHeight+(m_fItemHeight*m_iActiveItem),0.0f));
  m_iItemCount=m_pMenu->getID();
}

/**
 * This method is called when the finite state machine is switched to another state. Not much to
 * do, just clear the scene loaded in "activate"
 * @see IState::deactivate
 */
void CMenu::deactivate() {
  m_pSceneManager->clear();
}

/**
 * This method is called for each frame Irrlicht draws
 * @see IState::update
 */
int CMenu::update() {
  int iRet=0;
  u32 iNow=m_pTimer->getTime(),iDiff=iNow-m_iLastTime;  //let's get the time since the last call
  f32 yDst=0.0f;

  //store the current time
  m_iLastTime=iNow;

  //was the menu moved by the user?
  if (m_iMenuMove) {
    yDst=-m_fItemHeight+(m_fItemHeight*m_iActiveItem);
    vector3df pos=m_pMenu->getPosition();
    //the menu was moved downwards
    if (m_iMenuMove>0) {
      m_pMenu->setPosition(pos+vector3df(0.0f,0.005f*iDiff,0.0f));
      if (pos.Y>=yDst) {
        m_iMenuMove=0;
        m_pMenu->setPosition(vector3df(pos.X,yDst,pos.Z));
      }
    }
    else {  //the menu was moved upwards
      m_pMenu->setPosition(pos-vector3df(0.0f,0.005f*iDiff,0.0f));
      if (pos.Y<=yDst) {
        m_iMenuMove=0;
        m_pMenu->setPosition(vector3df(pos.X,yDst,pos.Z));
      }
    }
  }

  //an item was selected so let's tell our finite state machine which one it was
  if (m_bSelect) {
    iRet=m_iActiveItem+2;
    m_bSelect=false;
  }

  return iRet;
}

/**
 * This method is called by the Irrlicht event pipeline. We just handle key input, cursor up and cursor down
 * to change the menu selection and the "Enter" key to select the selected item
 */
bool CMenu::OnEvent(const SEvent &event) {
  if (event.EventType==EET_KEY_INPUT_EVENT) {
    if (event.KeyInput.PressedDown) {
      switch (event.KeyInput.Key) {
        case KEY_UP:
          if (m_iActiveItem>0) {
            m_iMenuMove=-1;
            m_iActiveItem--;
          }
          break;

        case KEY_DOWN:
          if (m_iActiveItem<m_iItemCount-1) {
            m_iMenuMove=1;
            m_iActiveItem++;
          }
          break;

        case KEY_RETURN:
          m_bSelect=true;
          break;

        default:
          break;
      }
    }
  }
  return false;
}
