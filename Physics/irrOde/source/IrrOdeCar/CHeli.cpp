  #include "CHeli.h"
  #include <CProjectile.h>

CHeli::CHeli(IrrlichtDevice *pDevice) : CVehicle(pDevice,L"Helicopter","../../data/irrOdeHeliHelp.txt") {
  m_pWorld=reinterpret_cast<CIrrOdeWorld *>(m_pSmgr->getSceneNodeFromName("worldNode"));
  m_pHeliBody=reinterpret_cast<CIrrOdeBody *>(m_pSmgr->getSceneNodeFromName("theHeli"));
  m_pTerrain=reinterpret_cast<ITerrainSceneNode *>(m_pSmgr->getSceneNodeFromName("terrain"));

  m_pCam=m_pSmgr->addCameraSceneNode();
  m_bLeft=true;
  m_bMissileCam=false;

  CIrrOdeManager::getSharedInstance()->addEventListener(this);

  m_pInfo=m_pGuiEnv->addStaticText(L"Hello World!",rect<s32>(5,5,150,65),true);
  m_pInfo->setDrawBackground(true);
  m_pInfo->setBackgroundColor(SColor(0x80,0xFF,0xFF,0xFF));
  m_pInfo->setVisible(false);

  m_fThrust=0;
  m_iThrust=0;
  m_iPitch=0;
  m_iRoll=0;
  m_iYaw=0;
}

CHeli::~CHeli() {
}

void CHeli::activate() {
  m_pSmgr->setActiveCamera(m_pCam);
  m_pDevice->setEventReceiver(this);
  m_pDevice->getCursorControl()->setVisible(false);
  m_pInfo->setVisible(true);
  m_bSwitchToMenu=false;
}

void CHeli::deactivate() {
  m_pInfo->setVisible(false);
}

u32 CHeli::update() {
  u32 iRet=CVehicle::update();
  static u32 iLastTime=0,iStep=0;

  if (!iLastTime)
    iLastTime=m_pDevice->getTimer()->getTime();
  else {
    u32 iTime=m_pDevice->getTimer()->getTime();
    iStep+=iTime-iLastTime;
    iLastTime=iTime;
  }

  vector3df rot=m_pHeliBody->getRotation();

  vector3df pos=rot.rotationToDirection(vector3df(15,5,0)),
            up =m_pHeliBody->getRotation().rotationToDirection(vector3df(0,0.1,0)),
            tgt=m_pHeliBody->getRotation().rotationToDirection(vector3df(0,1  ,0));

  CProjectileManager *ppm=CProjectileManager::getSharedInstance();

  if (m_bMissileCam && ppm->getLast()!=NULL) {
    pos=ppm->getLast()->getBody()->getRotation().rotationToDirection(vector3df(10,2,0));
    m_pCam->setPosition(ppm->getLast()->getBody()->getPosition()+pos);
    m_pCam->setUpVector(vector3df(0,1,0));
    m_pCam->setTarget(ppm->getLast()->getBody()->getPosition());
  }
  else {
    m_pCam->setPosition(m_pHeliBody->getPosition()+pos);
    m_pCam->setUpVector(up);
    m_pCam->setTarget(m_pHeliBody->getPosition()+tgt);
  }

  wchar_t dummy[0xFF];
  pos=m_pHeliBody->getAbsolutePosition();
  rot=m_pHeliBody->getRotation();
  vector3df v=m_pHeliBody->getLinearVelocity();
  swprintf(dummy,0xFE,L"pos: (%.0f, %.0f, %.0f)\nvelocity: (%.0f, %.0f, %.0f)=%.2f",pos.X,pos.Y,pos.Z,v.X,v.Y,v.Z,v.getLength());
  swprintf(dummy,0xFE,L"%s\nrotation: (%.0f, %.0f, %.0f)\ncontrols: ",dummy,rot.X,rot.Y,rot.Z);

  swprintf(dummy,0xFE,L"%s (T=%.0f%%",dummy,m_fThrust*100);

  switch (m_iYaw) {
    case  0: swprintf(dummy,0xFE,L"%s, Y=0",dummy); break;
    case -5: swprintf(dummy,0xFE,L"%s, Y=-1",dummy); break;
    case  5: swprintf(dummy,0xFE,L"%s, Y=1",dummy); break;
  }

  switch (m_iPitch) {
    case  0: swprintf(dummy,0xFE,L"%s, P=0",dummy); break;
    case -5: swprintf(dummy,0xFE,L"%s, P=-1",dummy); break;
    case  5: swprintf(dummy,0xFE,L"%s, P=1",dummy); break;
  }

  switch (m_iRoll) {
    case  0: swprintf(dummy,0xFE,L"%s, R=0",dummy); break;
    case -5: swprintf(dummy,0xFE,L"%s, R=-1",dummy); break;
    case  5: swprintf(dummy,0xFE,L"%s, R=1",dummy); break;
  }

  swprintf(dummy,0xFE,L"%s)",dummy);
  f32 fHeight=pos.Y-m_pTerrain->getHeight(pos.X,pos.Z);
  swprintf(dummy,0xFE,L"%s\nAltitude: %.0f",dummy,fHeight);
  if (m_bMissileCam) swprintf(dummy,0xFE,L"%s\nMissile Cam active",dummy);

  m_pInfo->setText(dummy);

  return iRet;
}

bool CHeli::OnEvent(const SEvent &event) {
  if (CVehicle::OnEvent(event)) return true;
  if (event.EventType==EET_KEY_INPUT_EVENT) {
    if (event.KeyInput.PressedDown) {
      switch (event.KeyInput.Key) {
        case KEY_KEY_S: m_iThrust=-5; break;
        case KEY_KEY_W: m_iThrust= 5; break;
        case KEY_KEY_A: m_iYaw=-5; break;
        case KEY_KEY_D: m_iYaw= 5; break;
        case KEY_UP: m_iPitch=5; break;
        case KEY_DOWN: m_iPitch=-5; break;
        case KEY_LEFT: m_iRoll=-5; break;
        case KEY_RIGHT: m_iRoll=5; break;
        case KEY_KEY_C: m_bMissileCam=!m_bMissileCam; break;
        case KEY_SPACE: {
          //We add a new bomb...
          CIrrOdeBody *pMissile=reinterpret_cast<CIrrOdeBody *>(m_pSmgr->addSceneNode(CIrrOdeSceneNode::nodeNameToC8(IRR_ODE_BODY_NAME),m_pWorld));
          pMissile->setOdeClassname(stringc("missile"));
          pMissile->setName("Missile");
          pMissile->setIsFastMoving(true);

          IAnimatedMesh *pMesh=m_pSmgr->getMesh("../../data/missile.3ds");
          IAnimatedMeshSceneNode *pNode=m_pSmgr->addAnimatedMeshSceneNode(pMesh,pMissile);
          pNode->setScale(vector3df(0.8f,0.8f,0.8f));
          CIrrOdeGeomBox *pBox=reinterpret_cast<CIrrOdeGeomBox *>(m_pSmgr->addSceneNode(CIrrOdeSceneNode::nodeNameToC8(IRR_ODE_GEOM_BOX_NAME),pNode));

          //...set it's mass and position and init physics.
          pBox->setMassTotal(10.0f);

          vector3df pos=m_pHeliBody->getAbsolutePosition()+m_pHeliBody->getRotation().rotationToDirection(vector3df(0.0f,0.6f,m_bLeft?-2.0f:2.0f));
          pMissile->setPosition(pos);
          pMissile->setRotation(m_pHeliBody->getRotation());
          pMissile->initPhysics();
          pMissile->setLinearVelocity(m_pHeliBody->getLinearVelocity());

          new CProjectile(pMissile,600,vector3df(-500.0f,0.0f,0.0f));
          m_bLeft=!m_bLeft;
        }
        default:
          break;
      }
    }

    if (!event.KeyInput.PressedDown) {
      switch (event.KeyInput.Key) {
        case KEY_KEY_W:
        case KEY_KEY_S: m_iThrust=0; break;

        case KEY_KEY_A:
        case KEY_KEY_D: m_iYaw=0; break;

        case KEY_UP:
        case KEY_DOWN: m_iPitch=0; break;

        case KEY_LEFT:
        case KEY_RIGHT: m_iRoll=0; break;

        default:
          break;
      }
    }
  }
  return false;
}

bool CHeli::onEvent(IIrrOdeEvent *pEvent) {
  if (pEvent->getType()==irr::ode::eIrrOdeEventStep) {
    vector3df rot=m_pHeliBody->getRotation();
    f32 alt=m_pHeliBody->getAbsolutePosition().Y,f=alt<=0?1:(2000-alt)/2000;

    m_fThrust+=((f32)m_iThrust)/1000;

    if (m_fThrust>1.0f) m_fThrust=1.0f;
    if (m_fThrust<0.0f) m_fThrust=0.0f;

    m_pHeliBody->addForce(rot.rotationToDirection(vector3df(0,20*m_fThrust*f,0)));
    m_pHeliBody->addTorque(rot.rotationToDirection(vector3df(0,m_iYaw,0)));
    m_pHeliBody->addTorque(rot.rotationToDirection(vector3df(0,0,m_iPitch)));
    m_pHeliBody->addTorque(rot.rotationToDirection(vector3df(m_iRoll,0,0)));

    vector3df cRot=m_pHeliBody->getRotation();
    vector3df vel=cRot.rotationToDirection(vector3df(0,0,1));
    f32 fVel=m_pHeliBody->getLinearVelocity().getLength();
    f32 dot=0.01*fVel*fVel*vel.dotProduct(m_pHeliBody->getLinearVelocity().normalize());

    if (dot>0.01f || dot<-0.01f) {
      vector3df vForce=cRot.rotationToDirection(vector3df(0.0f,0.0f,vel.X+0.1f));
      m_pHeliBody->addForce(-10.0f*dot*vel);
    }
  }

  /*if (pEvent->getType()==irr::ode::eIrrOdeEventBodyMoved) {
    irr::ode::CIrrOdeEventBodyMoved *pEvt=(irr::ode::CIrrOdeEventBodyMoved *)pEvent;
    if (m_pMissile!=NULL && pEvt->getBody()==m_pMissile->getBody() && pEvt->getTouched()!=NULL) {
      m_pMissile->collide();
      m_pMissile=NULL;
    }
  }*/

  return false;
}

bool CHeli::handlesEvent(IIrrOdeEvent *pEvent) {
  return pEvent->getType()==irr::ode::eIrrOdeEventStep || pEvent->getType()==irr::ode::eIrrOdeEventBodyMoved;
}

