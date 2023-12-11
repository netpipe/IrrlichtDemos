  #include <CStateDemo.h>

CStateDemo::CStateDemo(IrrlichtDevice *pDevice, CStateMachine *pStateMachine) : IState(pDevice,pStateMachine) {
  m_pTimer=pDevice->getTimer();
  while (m_pTimer->isStopped()) m_pTimer->start();
}

CStateDemo::~CStateDemo() {
}

void CStateDemo::activate(IState *pPrevious) {
  ITerrainSceneNode *terrain=m_pSmgr->addTerrainSceneNode("data/terrain-heightmap.bmp",
                                                          NULL,-1,
                                                          vector3df(0.0f,0.0f,0.0f),
                                                          vector3df(0.0f,0.0f,0.0f),
                                                          vector3df(40.0f,4.4f,40.0f),
                                                          SColor(255,255,255,255),
                                                          5,ETPS_17,4);
  if (terrain) {
    terrain->setMaterialFlag(EMF_LIGHTING,false);
    terrain->setMaterialTexture(0,m_pDriver->getTexture("data/terrain-texture.jpg"));
  }

  vector3df pos=terrain->getTerrainCenter();
  pos+=vector3df(0.0f,terrain->getHeight(pos.X,pos.Z)+50.0f,0.0f);;
  m_pCam=m_pSmgr->addCameraSceneNode(NULL,pos,pos+vector3df(1.0f,0.0f,0.0f));

  m_pDevice->setEventReceiver(this);
  m_fRot=0.0f;
  m_bNextState=false;
  m_pConfigControl->setMouseSensitivity(2.0f);
  m_iLastTime=m_pTimer->getTime();
  m_pDevice->getCursorControl()->setPosition(position2di(320,200));
}

void CStateDemo::deactivate(IState *pNext) {
  m_pConfigControl->save("data/control.xml");
}

u32 CStateDemo::update() {
  u32 iThisTime=m_pTimer->getTime();
  f32 fTimeMul=((f32)iThisTime-m_iLastTime)/2;
  m_iLastTime=iThisTime;

  vector3df pos=m_pCam->getPosition(),
            rot=vector3df(0.0f,m_fRot,0.0f),
            dif;

  f32 mx=m_pConfigControl->get(0)-m_pConfigControl->get(1)*fTimeMul,
      my=m_pConfigControl->get(6)-m_pConfigControl->get(7)*fTimeMul,
      mz=m_pConfigControl->get(2)-m_pConfigControl->get(3)*fTimeMul;

  dif=rot.rotationToDirection(vector3df(mx,my,mz));

  m_fRot+=(m_pConfigControl->get(5)-m_pConfigControl->get(4))*fTimeMul/8;
  m_pCam->setPosition(pos+dif);
  m_pCam->setTarget(m_pCam->getPosition()+rot.rotationToDirection(vector3df(1.0f,0.0f,0.0f)));
  return m_bNextState?3:0;
}

void CStateDemo::setConfigControl(CIrrCC *pCtrl) {
  m_pConfigControl=pCtrl;
}

bool CStateDemo::OnEvent(const SEvent &event) {
  bool bRet=m_pConfigControl->OnEvent(event);
  if (!bRet && event.EventType==EET_KEY_INPUT_EVENT && event.KeyInput.Key==KEY_ESCAPE) m_bNextState=true;
  return bRet;
}
