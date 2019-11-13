  #include "CTank.h"
  #include <CProjectile.h>

CTank::CTank(IrrlichtDevice *pDevice) : CVehicle(pDevice,L"Tank","../../data/irrOdeTankHelp.txt") {
  m_pWorld=reinterpret_cast<CIrrOdeWorld *>(m_pSmgr->getSceneNodeFromName("worldNode"));
  m_pTankBody=reinterpret_cast<CIrrOdeBody *>(m_pSmgr->getSceneNodeFromName("theTank"));
  m_pTurret=reinterpret_cast<CIrrOdeBody *>(m_pSmgr->getSceneNodeFromName("turret"));

  m_pMotor[0]=reinterpret_cast<CIrrOdeMotor *>(m_pSmgr->getSceneNodeFromName("motorRR"));
  m_pMotor[1]=reinterpret_cast<CIrrOdeMotor *>(m_pSmgr->getSceneNodeFromName("motorFR"));
  m_pMotor[2]=reinterpret_cast<CIrrOdeMotor *>(m_pSmgr->getSceneNodeFromName("motorRL"));
  m_pMotor[3]=reinterpret_cast<CIrrOdeMotor *>(m_pSmgr->getSceneNodeFromName("motorFL"));

  m_pTurretServo=reinterpret_cast<CIrrOdeServo *>(m_pSmgr->getSceneNodeFromName("turretServo"));
  m_pCannonServo=reinterpret_cast<CIrrOdeServo *>(m_pSmgr->getSceneNodeFromName("cannonServo"));

  m_pCam=m_pSmgr->addCameraSceneNode();

  m_iAcc=0;
  m_iSteer=0;
  m_iTurretTurn=0;
  m_iCannon=0;
  m_bFire=false;
  m_bFollowTurret=false;
  m_bFollowBullet=false;
  m_bFastCollision=true;

  m_pCannon=reinterpret_cast<CIrrOdeBody *>(m_pSmgr->getSceneNodeFromName("cannon"));

  CIrrOdeManager::getSharedInstance()->addEventListener(this);

  m_pTextures[0]=m_pDevice->getVideoDriver()->getTexture("../../data/target.jpg");
  m_pTextures[1]=m_pDevice->getVideoDriver()->getTexture("../../data/target1.jpg");

  m_pInfo=m_pGuiEnv->addStaticText(L"Hello World!",rect<s32>(5,5,150,65),true);
  m_pInfo->setDrawBackground(true);
  m_pInfo->setBackgroundColor(SColor(0x80,0xFF,0xFF,0xFF));
  m_pInfo->setVisible(false);
}

CTank::~CTank() {
}

void CTank::activate() {
  m_pSmgr->setActiveCamera(m_pCam);
  m_pDevice->setEventReceiver(this);
  m_pDevice->getCursorControl()->setVisible(false);
  m_pInfo->setVisible(true);
  m_bSwitchToMenu=false;
}

void CTank::deactivate() {
  m_pInfo->setVisible(false);
}

u32 CTank::update() {
  u32 iRet=CVehicle::update();
  static u32 iLastTime=0,iStep=0;

  if (!iLastTime)
    iLastTime=m_pDevice->getTimer()->getTime();
  else {
    u32 iTime=m_pDevice->getTimer()->getTime();
    iStep+=iTime-iLastTime;
    iLastTime=iTime;
  }

  if (m_bFollowBullet && CProjectileManager::getSharedInstance()->getLast()!=NULL) {
    CProjectile *p=CProjectileManager::getSharedInstance()->getLast();
    vector3df pos=p->getBody()->getPosition()-3*p->getBody()->getLinearVelocity().normalize();
    m_pCam->setPosition(pos);
    m_pCam->setUpVector(vector3df(0,1,0));
    m_pCam->setTarget(p->getBody()->getPosition());
  }
  else {
    vector3df rot=!m_bFollowTurret?m_pTankBody->getRotation():m_pTurret->getRotation();

    vector3df pos=rot.rotationToDirection(vector3df(7.5,2.5,0)),
              up =m_pTankBody->getRotation().rotationToDirection(vector3df(0,0.1,0)),
              tgt=m_pTankBody->getRotation().rotationToDirection(vector3df(0,1  ,0));

    m_pCam->setPosition(m_pTankBody->getPosition()+pos);
    m_pCam->setUpVector(up);
    m_pCam->setTarget(m_pTankBody->getPosition()+tgt);
  }

  if (m_bFire) {
    //We add a new bullet...
    CIrrOdeBody *pBullet=reinterpret_cast<CIrrOdeBody *>(m_pSmgr->addSceneNode(CIrrOdeSceneNode::nodeNameToC8(IRR_ODE_BODY_NAME),m_pWorld));
    pBullet->setOdeClassname(stringc("bullet"));
    pBullet->setName("Bullet");
    //...mark it as "fast moving"...
    pBullet->setIsFastMoving(m_bFastCollision);
    IAnimatedMesh *pMesh=m_pSmgr->getMesh("../../data/sphere.3ds");
    IAnimatedMeshSceneNode *pNode=m_pSmgr->addAnimatedMeshSceneNode(pMesh,pBullet);
    pNode->setScale(vector3df(0.1f,0.1f,0.1f));
    CIrrOdeGeomSphere *pSphere=reinterpret_cast<CIrrOdeGeomSphere *>(m_pSmgr->addSceneNode(CIrrOdeSceneNode::nodeNameToC8(IRR_ODE_GEOM_SPHERE_NAME),pNode));

    //...set it's mass and position and init physics.
    pSphere->setMassTotal(10.0f);

    vector3df pos=m_pCannon->getPosition()+m_pCannon->getRotation().rotationToDirection(vector3df(-3.0f,0,0));
    pBullet->setPosition(pos);
    pBullet->initPhysics();
    pBullet->setLinearVelocity(m_pTankBody->getLinearVelocity()+m_pCannon->getRotation().rotationToDirection(vector3df(-500.0f,0,0)));

    new CProjectile(pBullet,1200,vector3df(0,0,0));
    m_bFire=false;
  }

  f32 fVel[4]={ 0.0f, 0.0f, 0.0f, 0.0f };

  switch (m_iAcc) {
    case 1:
      for (int i=0; i<4; i++) fVel[i]=25.0f;
      break;

    case -1:
      for (int i=0; i<4; i++) fVel[i]=-25.0f;
      break;

    default:
      for (int i=0; i<4; i++) fVel[i]=0.0f;
      break;
  }

  switch (m_iSteer) {
    case -1:
      fVel[0]+=25.0f;
      fVel[1]+=25.0f;
      fVel[2]-=25.0f;
      fVel[3]-=25.0f;
      break;

    case 1:
      fVel[0]-=25.0f;
      fVel[1]-=25.0f;
      fVel[2]+=25.0f;
      fVel[3]+=25.0f;
      break;
  }

  f32 tPos=0.0f;

  u32 iDummy=iStep;

  switch (m_iTurretTurn) {
    case 1:
      tPos=m_pTurretServo->getServoPos();
      while (iDummy>5) { tPos+=0.15f; iDummy-=5; }
      if (tPos>175) tPos=175;
      m_pTurretServo->setServoPos(tPos);
      break;

    case -1:
      tPos=m_pTurretServo->getServoPos();
      while (iDummy>5) { tPos-=0.15; iDummy-=5; }
      if (tPos<-175) tPos=-175;
      m_pTurretServo->setServoPos(tPos);
      break;
  }

  iDummy=iStep;

  switch (m_iCannon) {
    case 1:
      tPos=m_pCannonServo->getServoPos();
      while (iDummy>5) { tPos+=0.2; iDummy-=5; }
      if (tPos>45) tPos=45;
      m_pCannonServo->setServoPos(tPos);
      break;

    case -1:
      tPos=m_pCannonServo->getServoPos();
      while (iDummy>5) { tPos-=0.2; iDummy-=5; }
      if (tPos<-5) tPos=-5;
      m_pCannonServo->setServoPos(tPos);
      break;
  }

  for (int i=0; i<4; i++) {
    m_pMotor[i]->setVelocity(fVel[i]);
    m_pMotor[i]->setForce(20);
  }

  while (iStep>5) iStep-=5;

  if (m_bFlip) {
    vector3df t=m_pTankBody->getAbsoluteTransformation().getRotationDegrees().rotationToDirection(vector3df(3000,0,0));
    m_pTankBody->setTorque(t);
    m_bFlip=false;
  }

  wchar_t dummy[0xFF];
  vector3df pos=m_pTankBody->getAbsolutePosition();
  CProjectileManager *ppm=CProjectileManager::getSharedInstance();
  u32 iHits=ppm->getHits(),iShots=ppm->getShots();
  swprintf(dummy,0xFE,L"pos: (%.0f, %.0f, %.0f)\nshots fired: %i\nhits: %i",pos.X,pos.Y,pos.Z,iShots,iHits);
  if (m_bFollowTurret) swprintf(dummy,0xFF,L"%s\ncamera follows turret",dummy);
  if (m_bFollowBullet) swprintf(dummy,0xFF,L"%s\ncamera follows bullet",dummy);
  if (m_bFastCollision) swprintf(dummy,0xFF,L"%s\nfast collision detection on",dummy);
  m_pInfo->setText(dummy);

  return iRet;
}

bool CTank::OnEvent(const SEvent &event) {
  if (CVehicle::OnEvent(event)) return true;
  if (event.EventType==EET_KEY_INPUT_EVENT) {
    if (event.KeyInput.PressedDown) {
      switch (event.KeyInput.Key) {
        case KEY_UP   : m_iAcc  =-1; return true; break;
        case KEY_DOWN : m_iAcc  = 1; return true; break;
        case KEY_LEFT : m_iSteer= 1; return true; break;
        case KEY_RIGHT: m_iSteer=-1; return true; break;

        case KEY_KEY_A: m_iTurretTurn= 1; return true; break;
        case KEY_KEY_D: m_iTurretTurn=-1; return true; break;
        case KEY_KEY_W: m_iCannon= 1; return true; break;
        case KEY_KEY_S: m_iCannon=-1; return true; break;
        case KEY_KEY_B: m_bFollowBullet=!m_bFollowBullet; return true; break;
        case KEY_KEY_F: m_bFastCollision=!m_bFastCollision; return true; break;

        case KEY_SPACE: m_bFire=true; return true; break;

        case KEY_KEY_T: m_bFollowTurret=!m_bFollowTurret; return true; break;

        case KEY_RETURN: m_bFlip =true; return true; break;

        case KEY_F1   : m_bHelp      =!m_bHelp; break;
        default:
          break;
      }
    }

    if (!event.KeyInput.PressedDown) {
      switch (event.KeyInput.Key) {
        case KEY_UP  :
        case KEY_DOWN: m_iAcc=0; return true; break;

        case KEY_LEFT :
        case KEY_RIGHT: m_iSteer=0; return true; break;

        case KEY_KEY_A:
        case KEY_KEY_D: m_iTurretTurn=0; break;

        case KEY_KEY_W:
        case KEY_KEY_S: m_iCannon=0; break;

        case KEY_SPACE: m_bBrake=false; return true; break;

        default:
          break;
      }
    }
  }
  return false;
}

bool CTank::onEvent(IIrrOdeEvent *pEvent) {
  return false;
}

bool CTank::handlesEvent(IIrrOdeEvent *pEvent) {
  return false;
}
