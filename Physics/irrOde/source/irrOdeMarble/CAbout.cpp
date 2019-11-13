  #include <CAbout.h>
  #include <CHiScore.h>

CAbout::CAbout(IrrlichtDevice *pDevice) : IState(pDevice) {
  m_pImage=pDevice->getVideoDriver()->getTexture("../../data/about.png");
}

CAbout::~CAbout() {
}

/**
 * This is an implementation of IState::activate. This time not much has to be done, the image is loaded,
 * the event receiver is set and the highscore is loaded
 * @see IState::activate
 */
void CAbout::activate() {
  m_iState=0;
  m_bClose=false;
  m_bRunning=false;
  m_pDevice->setEventReceiver(this);
  dimension2du res=m_pDevice->getVideoDriver()->getCurrentRenderTargetSize();
  m_pHiScore=new HiScore(m_pDevice);

  s32 x=(res.Width/2)-320,y=(res.Height/2)-240;

  m_pImg=m_pGuiEnv->addImage(rect<s32>(x,y,x+640,x+480));
  m_pImg->setImage(m_pImage);
  m_pSceneManager->loadScene("../../data/irrOdeMarble_about.xml");
  m_pSceneManager->addCameraSceneNode(NULL,vector3df(1,0,0),vector3df(0,0,0));
}

/**
 * Deactivate the state. Here we clear the scene, the GUI and destroy the hiscore object
 */
void CAbout::deactivate() {
  m_pGuiEnv->clear();
  m_pSceneManager->clear();
  delete m_pHiScore;
}

/**
 * Just check if the state should be ended
 */
int CAbout::update() {
  return m_bClose?1:0;
}

/**
 * The event receiver for the Irrlicht event pipeline. The two states of this class (about, hiscore)
 * are switched with any key. If a key is pressed when the highscore is up the state is ended,
 * and the first key release is ignored (this release would lead back to the main menu way to early.
 */
bool CAbout::OnEvent(const SEvent &event) {
  if (event.EventType==EET_KEY_INPUT_EVENT) {
    if (m_bRunning) {
      switch (m_iState) {
        case 0:
          m_iState=1;
          m_pHiScore->show(0);
          m_bRunning=false;
          m_pImg->remove();
          break;

        case 1:
          m_bClose=true;
          break;
      }
    }
    if (!event.KeyInput.PressedDown) m_bRunning=true;
  }

  return m_bClose;
}
