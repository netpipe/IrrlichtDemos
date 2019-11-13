  #include <CPlay.h>

  #define _MAX_CAM_HI 60
  #define _MAX_CAM_DIST 20
  #define _MIN_CAM_DIST  7

/**
 * The contructor
 * We do some very basic initialization here. Most initialization is done in the "activate" method
 */
CPlay::CPlay(IrrlichtDevice *pDevice) : IState(pDevice) {
  CIrrOdeManager::getSharedInstance()->addEventListener(this);
  CIrrOdeManager::getSharedInstance()->setTimer(m_pTimer);
  m_pFont=m_pGuiEnv->getFont("../../data/fontcourier.bmp");
  m_pGuiEnv->getSkin()->setFont(m_pFont);
  m_pOdeManager=CIrrOdeManager::getSharedInstance();
}

/**
 * The desctructor. Nothing to do here
 */
CPlay::~CPlay() {
}

/**
 * In this method we do all the game initialization the is needed
 * @see IState::activate
 */
void CPlay::activate() {
  //get the game's start time, initialize the ODE, load the level and init the level's physics
  m_iStartTime=m_pTimer->getTime();
  m_pOdeManager->initODE();
  m_pSceneManager->loadScene("../../data/irrOdeMarble_level.xml");

  //now we get the marble's CIrrOdeBody scene node, add the camera, hide the mouse pointer and set the Irrlicht event receiver
  m_pMarble=reinterpret_cast<CIrrOdeBody *>(m_pSceneManager->getSceneNodeFromName("marble"));
  m_pCam=m_pSceneManager->addCameraSceneNode();
  m_pCam->setPosition(m_pMarble->getPosition()+vector3df(7,1,0));
  m_pCam->setTarget(m_pMarble->getPosition());
  m_pDevice->getCursorControl()->setVisible(false);
  m_pDevice->setEventReceiver(this);
  m_pCtrl=m_pDevice->getCursorControl();

  //variable initialization
  m_bEndGame=false;
  m_bRunning=true;
  m_bMouseLeft=false;
  m_bMouseRght=false;

  m_fCamRot=0.0f;
  m_fCamHi=25.0f;
  m_fCamDist=12.5f;

  m_iCheckPoint=0;
  m_pDevice->getCursorControl()->setPosition(position2di(320,200));
  m_pSceneManager->setShadowColor(irr::video::SColor(128,0,0,0));

  //Now we fill an array of the checkpoints the player has to reach
  CIrrOdeGeom *cp=NULL;
  int i=0;
  do {
    char s[0xFF];
    sprintf(s,"checkpoint_%i",i);
    cp=reinterpret_cast<CIrrOdeGeom *>(m_pSceneManager->getSceneNodeFromName(s));
    if (cp) {
      m_lCheckpoints.push_back(cp);
      wchar_t ws[0xFF];
      swprintf(ws,0xFE,L"Checkpoint %i",i);
      IGUIStaticText *t=m_pGuiEnv->addStaticText(ws,rect<s32>(5,20*i+10,205,20*i+25),0,true);
      t->setDrawBackground(true);
      t->setBackgroundColor(SColor(0x80,0x80,0x80,0x80));
      m_aCheckText.push_back(t);
    }
    i++;
  }
  while (cp!=NULL);

  //if checkpoints were found we initialize the pointers to the first and to the next checkpoint
  if (i>1) {
    m_pThisCheckpoint=m_lCheckpoints.begin();
    m_pLastCheckpoint=m_pThisCheckpoint;
  }

  //set the game state
  m_iGameState=_STATE_PLAYING;

  //Now we add a shadow to some scene nodes. The node must be an IrrOdeBody, and the IrrOde "ident" attribute must be "1"
  list<CIrrOdeSceneNode *> lNodes=CIrrOdeManager::getSharedInstance()->getIrrOdeNodes();
  list<CIrrOdeSceneNode *>::Iterator it;

  for (it=lNodes.begin(); it!=lNodes.end(); it++) {
    CIrrOdeSceneNode *node=*it;
    if (node->getType()==IRR_ODE_BODY_ID && ((CIrrOdeBody *)node)->getIdent()==1) {
      list<ISceneNode *>children=node->getChildren();
      list<ISceneNode *>::Iterator ic;
      for (ic=children.begin(); ic!=children.end(); ic++)
        if ((*ic)->getType()==ESNT_ANIMATED_MESH) {
          IAnimatedMeshSceneNode *iam=reinterpret_cast<IAnimatedMeshSceneNode *>(*ic);
          iam->addShadowVolumeSceneNode();
        }
    }
  }

  CIrrOdeWorld *pWorld=reinterpret_cast<CIrrOdeWorld *>(m_pSceneManager->getSceneNodeFromName("world"));
  vector3df e=pWorld->getBoundingBox().getExtent();

  //create the HighScore object for later use
  m_pHiScore=new HiScore(m_pDevice);
  m_pOdeManager->initPhysics();
}

/**
 * This method does all deinitialization, mainly clears the scene and the gui environment
 * @see IState::deactivate
 */
void CPlay::deactivate() {
  m_pSceneManager->clear();
  m_pGuiEnv->clear();
  CIrrOdeManager::getSharedInstance()->closeODE();
  m_bRunning=false;
  m_lCheckpoints.clear();
  m_aCheckText.clear();
  delete m_pHiScore;
}

/**
 * This is the "heart" of the game, the update method
 * @see IState::update
 */
int CPlay::update() {
  int iRet=0;
  u32 iTime=m_pTimer->getTime()-m_iStartTime;
  position2di pos;

  s32 ix=0,iy=0;
  switch (m_iGameState) {
    //game in progress?
    case _STATE_PLAYING:
      //first we get the movemeent of the mouse
      pos=m_pDevice->getCursorControl()->getPosition();
      ix=pos.X-320,iy=pos.Y-200;
      m_pDevice->getCursorControl()->setPosition(position2di(320,200));
      //now we update the text field that shows the time that has passed since game start
      if (m_iCheckPoint>=0 && (u32)m_iCheckPoint<m_aCheckText.size()) {
        IGUIStaticText *t=m_aCheckText[m_iCheckPoint];
        wchar_t ws[0xFF];
        swprintf(ws,0xFE,L"checkpoint %i: %.3f",m_iCheckPoint,((f32)iTime)/1000);
        t->setText(ws);
      }

      //If the marble's Y-position is below -70 we respawn it
      if (m_pMarble->getPosition().Y<-70.0f) {
        ISceneNode *cp=(*m_pLastCheckpoint)->getParent();
        m_pMarble->setPosition(cp->getPosition()-vector3df(2.5f,-5.0f,2.5f));
        m_pMarble->setLinearVelocity(vector3df(0.0f,0.0f,0.0f));
        m_pMarble->setAngularVelocity(vector3df(0.0f,0.0f,0.0f));
      }

      //step the simulation
      CIrrOdeManager::getSharedInstance()->step();

      //if the right mouse button is pressed we focus the camera on the next checkpoint
      if (m_bMouseRght) {
        if (m_pThisCheckpoint!=m_lCheckpoints.end()) {
          CIrrOdeGeom *ncp=*m_pThisCheckpoint;
          vector3df pCamTarget=ncp->getAbsolutePosition();

          m_pCam->setPosition(m_pMarble->getPosition()-8*(pCamTarget-m_pMarble->getPosition()).normalize()+vector3df(0,4,0));
          m_pCam->setTarget(pCamTarget);
        }
      }
      else {
        if (m_bMouseLeft) {
          //If the left mouse button is pressed the user wants to control the camera with mouse movement
          m_fCamRot-=ix;
          m_fCamHi-=iy;

          if (m_fCamHi> _MAX_CAM_HI) m_fCamHi= _MAX_CAM_HI;
          if (m_fCamHi<-_MAX_CAM_HI) m_fCamHi=-_MAX_CAM_HI;
        }
        else {
          //if no mouse button is pressed the mouse movement is converted into an impulse that is added to the marble
          f32 fMoveX=15*(f32)ix,fMoveY=15*(f32)iy;
          vector3df t=vector3df(fMoveX*sin(m_fCamRot*GRAD_PI2)+fMoveY*cos(m_fCamRot*GRAD_PI2),0,
                                fMoveX*cos(m_fCamRot*GRAD_PI2)-fMoveY*sin(m_fCamRot*GRAD_PI2));
          m_pMarble->addTorque(t);
        }

        //now we calculate the camera's position and update the camera
        f32 fCamX=(f32)(m_fCamDist*sin(m_fCamRot*GRAD_PI2)*cos(m_fCamHi*GRAD_PI2)),
            fCamY=(f32)(m_fCamDist*sin(m_fCamHi*GRAD_PI2)),
            fCamZ=(f32)(m_fCamDist*cos(m_fCamRot*GRAD_PI2)*cos(m_fCamHi*GRAD_PI2));

        m_pCam->setPosition(m_pMarble->getPosition()+vector3df(fCamX,fCamY,fCamZ));
        m_pCam->setTarget(m_pMarble->getPosition());
      }
      break;

    //if the game is over we show the "game over" image and wait for 2 seconds until we proceed to the highscore
    case _STATE_GAMEOVER:
      if (m_pTimer->getTime()-m_iGameOverTime>2000) m_iGameState=_STATE_HIGHSCORE;
      break;

    //this state is only used to initialize the HiScore object
    case _STATE_HIGHSCORE:
      m_pHiScore->show(m_iGameOverTime-m_iStartTime);
      m_iGameState=_STATE_ENTER_NAME;
      break;

    //the name of this state is a little bit misleading. In reality it shows the highscore table, and maybe
    //the user can enter his name
    case _STATE_ENTER_NAME:
      iRet=m_pHiScore->update();
      break;

    //if we are done we do nothing
    case _STATE_DONE:
      break;
  }

  if (m_bEndGame) iRet=1;

  return iRet;
}

/**
 * This callback is called by the Irrlicht event pipeline. We resond to mouse movement, mouse button
 * clicks and the "ESCAPE" key to quit playing
 */
bool CPlay::OnEvent(const SEvent &event) {
  if (event.EventType==EET_MOUSE_INPUT_EVENT) {
    if (event.MouseInput.Event==EMIE_LMOUSE_PRESSED_DOWN) m_bMouseLeft=true ;
    if (event.MouseInput.Event==EMIE_LMOUSE_LEFT_UP     ) m_bMouseLeft=false;
    if (event.MouseInput.Event==EMIE_RMOUSE_PRESSED_DOWN) m_bMouseRght=true ;
    if (event.MouseInput.Event==EMIE_RMOUSE_LEFT_UP     ) m_bMouseRght=false;

    if (event.MouseInput.Event==EMIE_MOUSE_WHEEL) {
      m_fCamDist+=event.MouseInput.Wheel/3;
      if (m_fCamDist>_MAX_CAM_DIST) m_fCamDist=_MAX_CAM_DIST;
      if (m_fCamDist<_MIN_CAM_DIST) m_fCamDist=_MIN_CAM_DIST;
    }
  }

  if (event.EventType==EET_KEY_INPUT_EVENT) {
    if (event.KeyInput.PressedDown) {
      switch (event.KeyInput.Key) {
        case KEY_ESCAPE:
          m_bEndGame=true;
          break;

        default:
          break;
      }
    }
  }
  return false;
}

/**
 * This method is called by the IrrODE event pipeline. We do only handle collision events with checkpoints
 */
bool CPlay::onEvent(IIrrOdeEvent *pEvent) {
  if (pEvent->getType()==irr::ode::eIrrOdeEventBodyMoved) {
    CIrrOdeEventBodyMoved *c=(CIrrOdeEventBodyMoved *)pEvent;
    CIrrOdeBody *b=c->getBody();
    CIrrOdeGeom *g=c->getTouched();

    if (g!=NULL && g->getOdeClassname()==stringc("checkpoint") && !strcmp(b->getName(),"marble")) {
      if (m_pThisCheckpoint!=m_lCheckpoints.end()) {
        if (g==*m_pThisCheckpoint) {
          m_iCheckPoint++;
          m_pLastCheckpoint=m_pThisCheckpoint;
          m_pThisCheckpoint++;
          list<ISceneNode *> children=g->getParent()->getChildren();
          list<ISceneNode *>::Iterator i;

          for (i=children.begin(); i!=children.end(); i++)
            if ((*i)->getType()==ESNT_LIGHT)
              (*i)->setVisible(false);
        }
      }
      else {
        m_iGameState=_STATE_GAMEOVER;
        m_iGameOverTime=m_pTimer->getTime();
        ITexture *txt=m_pDriver->getTexture("../../data/gameover.png");
        dimension2du screenSize=m_pDriver->getScreenSize();
        position2di pos((screenSize.Width/2)-249,25);
        m_pGuiEnv->addImage(txt,pos,true);
      }
    }
  }

  return false;
}

/**
 * When this method is called we can tell the IrrODE event pipeline that we only handle
 * collision events
 */
bool CPlay::handlesEvent(IIrrOdeEvent *pEvent) {
  return pEvent->getType()==irr::ode::eIrrOdeEventBodyMoved;
}
