  #include "CPlane.h"
  #include <math.h>
  #include <CProjectile.h>

CPlane::CPlane(IrrlichtDevice *pDevice) : CVehicle(pDevice,L"Helicopter","../../data/irrOdePlaneHelp.txt") {
  m_pWorld=reinterpret_cast<CIrrOdeWorld *>(m_pSmgr->getSceneNodeFromName("worldNode"));
  m_pPlaneBody=reinterpret_cast<CIrrOdeBody *>(m_pSmgr->getSceneNodeFromName("thePlane"));
  m_pTerrain=reinterpret_cast<ITerrainSceneNode *>(m_pSmgr->getSceneNodeFromName("terrain"));
  m_pSteer=reinterpret_cast<CIrrOdeServo *>(m_pSmgr->getSceneNodeFromName("plane_wheel_steer"));

  m_pRoll [0]=m_pSmgr->getSceneNodeFromName("roll1");
  m_pRoll [1]=m_pSmgr->getSceneNodeFromName("roll2");
  m_pPitch[0]=m_pSmgr->getSceneNodeFromName("pitch1");
  m_pPitch[1]=m_pSmgr->getSceneNodeFromName("pitch2");
  m_pYaw     =m_pSmgr->getSceneNodeFromName("yaw");

  m_bFollowBombCam=false;
  m_bBackView=false;

  m_pRearWheels[0]=reinterpret_cast<CIrrOdeBody *>(m_pSmgr->getSceneNodeFromName("plane_wheel_rl"));
  m_pRearWheels[1]=reinterpret_cast<CIrrOdeBody *>(m_pSmgr->getSceneNodeFromName("plane_wheel_rr"));

  m_pCam=m_pSmgr->addCameraSceneNode();

  CIrrOdeManager::getSharedInstance()->addEventListener(this);

  m_pInfo=m_pGuiEnv->addStaticText(L"Hello World!",rect<s32>(5,5,150,65),true);
  m_pInfo->setDrawBackground(true);
  m_pInfo->setBackgroundColor(SColor(0x80,0xFF,0xFF,0xFF));
  m_pInfo->setVisible(false);

  m_iThrustDir=0;
  m_iThrust=0;
  m_iPitch=0;
  m_iRoll=0;
  m_iYaw=0;
}

CPlane::~CPlane() {
}

void CPlane::activate() {
  m_pSmgr->setActiveCamera(m_pCam);
  m_pDevice->setEventReceiver(this);
  m_pDevice->getCursorControl()->setVisible(false);
  m_pInfo->setVisible(true);
  m_bSwitchToMenu=false;
}

void CPlane::deactivate() {
  m_pInfo->setVisible(false);
  m_iThrustDir=0;
  m_iPitch=0;
  m_iRoll=0;
  m_iYaw=0;
}

u32 CPlane::update() {
  u32 iRet=CVehicle::update();
  static u32 iLastTime=0,iStep=0;

  if (!iLastTime)
    iLastTime=m_pDevice->getTimer()->getTime();
  else {
    u32 iTime=m_pDevice->getTimer()->getTime();
    iStep+=iTime-iLastTime;
    iLastTime=iTime;
  }

  vector3df rot=m_pPlaneBody->getRotation();

  vector3df pos=rot.rotationToDirection(m_bBackView?vector3df(0,5,-15):vector3df(0,5,15)),
            up =m_pPlaneBody->getRotation().rotationToDirection(vector3df(0,0.1,0)),
            tgt=m_pPlaneBody->getRotation().rotationToDirection(vector3df(0,1  ,0));

  CProjectileManager *ppm=CProjectileManager::getSharedInstance();
  if (m_bFollowBombCam && ppm->getLast()!=NULL) {
    pos=ppm->getLast()->getBody()->getRotation().rotationToDirection(vector3df(0,5,10));
    m_pCam->setPosition(ppm->getLast()->getBody()->getPosition()+pos);
    m_pCam->setUpVector(vector3df(0,1,0));
    m_pCam->setTarget(ppm->getLast()->getBody()->getPosition());
  }
  else {
    m_pCam->setPosition(m_pPlaneBody->getPosition()+pos);
    m_pCam->setUpVector(up);
    m_pCam->setTarget(m_pPlaneBody->getPosition()+tgt);
  }

  wchar_t dummy[0xFF];
  pos=m_pPlaneBody->getAbsolutePosition();
  rot=m_pPlaneBody->getRotation();
  vector3df v=m_pPlaneBody->getLinearVelocity();
  swprintf(dummy,0xFE,L"pos: (%.0f, %.0f, %.0f)\nvelocity: (%.0f, %.0f, %.0f) %.2f",pos.X,pos.Y,pos.Z,v.X,v.Y,v.Z,m_fForewardVel);
  swprintf(dummy,0xFE,L"%s\nrotation: (%.0f, %.0f, %.0f)\ncontrols: ",dummy,rot.X,rot.Y,rot.Z);

  swprintf(dummy,0xFE,L"%s (T=%.0f%%",dummy,((f32)m_iThrust));

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

  if (m_bFollowBombCam) swprintf(dummy,0xFE,L"%s\nFollow Bomb Cam",dummy);

  m_pInfo->setText(dummy);

  return iRet;
}

bool CPlane::OnEvent(const SEvent &event) {
  if (CVehicle::OnEvent(event)) return true;
  if (event.EventType==EET_KEY_INPUT_EVENT) {
    if (event.KeyInput.PressedDown) {
      switch (event.KeyInput.Key) {
        case KEY_KEY_S: m_iThrustDir=-1; break;
        case KEY_KEY_W: m_iThrustDir= 1; break;
        case KEY_KEY_A: m_iYaw=-5; break;
        case KEY_KEY_D: m_iYaw= 5; break;
        case KEY_UP: m_iPitch=-5; break;
        case KEY_DOWN: m_iPitch=5; break;
        case KEY_LEFT: m_iRoll=-5; break;
        case KEY_RIGHT: m_iRoll=5; break;
        case KEY_KEY_C: m_bFollowBombCam=!m_bFollowBombCam; break;
        case KEY_KEY_R: m_bBackView=!m_bBackView; break;
        case KEY_KEY_B:
          m_pRearWheels[0]->setAngularDamping(0.8f);
          m_pRearWheels[1]->setAngularDamping(0.8f);
          break;

        case KEY_SPACE: {
          //We add a new bomb...
          CIrrOdeBody *pBomb=reinterpret_cast<CIrrOdeBody *>(m_pSmgr->addSceneNode(CIrrOdeSceneNode::nodeNameToC8(IRR_ODE_BODY_NAME),m_pWorld));
          pBomb->setOdeClassname(stringc("bomb"));
          pBomb->setName("Bomb");

          IAnimatedMesh *pMesh=m_pSmgr->getMesh("../../data/bomb.3ds");
          IAnimatedMeshSceneNode *pNode=m_pSmgr->addAnimatedMeshSceneNode(pMesh,pBomb);
          pNode->setScale(vector3df(1.2f,1.2f,1.2f));
          CIrrOdeGeomBox *pBox=reinterpret_cast<CIrrOdeGeomBox *>(m_pSmgr->addSceneNode(CIrrOdeSceneNode::nodeNameToC8(IRR_ODE_GEOM_BOX_NAME),pNode));

          //...set it's mass and position and init physics.
          pBox->setMassTotal(10.0f);

          vector3df pos=m_pPlaneBody->getAbsolutePosition()+m_pPlaneBody->getRotation().rotationToDirection(vector3df(0.0f,-4.0f,-2.0f));
          pBomb->setPosition(pos);
          pBomb->setRotation(m_pPlaneBody->getRotation());
          pBomb->initPhysics();
          pBomb->setLinearVelocity(m_pPlaneBody->getLinearVelocity());

          new CProjectile(pBomb,600,vector3df(0.0f,0.0f,0.0f));
        }
        default:
          break;
      }
    }

    if (!event.KeyInput.PressedDown) {
      switch (event.KeyInput.Key) {
        case KEY_KEY_W:
        case KEY_KEY_S: m_iThrustDir=0; break;

        case KEY_KEY_A:
        case KEY_KEY_D: m_iYaw=0; break;

        case KEY_UP:
        case KEY_DOWN: m_iPitch=0; break;

        case KEY_LEFT:
        case KEY_RIGHT: m_iRoll=0; break;

        case KEY_KEY_B:
          m_pRearWheels[0]->setAngularDamping(0.2f);
          m_pRearWheels[1]->setAngularDamping(0.2f);
          break;

        default:
          break;
      }
    }
  }
  return false;
}

bool CPlane::onEvent(IIrrOdeEvent *pEvent) {
  if (pEvent->getType()==irr::ode::eIrrOdeEventStep) {
    m_iThrust+=m_iThrustDir;

    if (m_iThrust>100) m_iThrust=100;
    if (m_iThrust<  0) m_iThrust=  0;

    f32 fMinHeight=300,fMaxHeight=1500;

    vector3df vRot=m_pPlaneBody->getRotation(),
              vVel=m_pPlaneBody->getLinearVelocity(),
              vPos=m_pPlaneBody->getAbsolutePosition(),
              vNormVel=vVel,
              vThrust=vRot.rotationToDirection(vector3df(0,0,-0.5f*((f32)m_iThrust))),
              vUp=vRot.rotationToDirection(vector3df(0.0f,1.0f,0.0f)),
              vForce=vector3df(0.0f,0.0f,0.0f);

    vNormVel.normalize();

    f32 fAltFact=vPos.Y<fMinHeight?1.0f:vPos.Y>fMaxHeight?0.0f:1-((vPos.Y-fMinHeight)/(fMaxHeight-fMinHeight));

    vector3df vForeward=vRot.rotationToDirection(vector3df(0,0,-1));
    f32 fForeward=vForeward.dotProduct(vNormVel.normalize());

    m_fForewardVel=fForeward*vVel.getLength();

    f32 fStallSpeed=50.0f,fMaxUpSpeep=80.0f;

    f32 fUp=fForeward;
    if (m_fForewardVel>=fStallSpeed)
      if (m_fForewardVel>=fMaxUpSpeep)
        vUp*=fMaxUpSpeep;
      else
        vUp*=m_fForewardVel-fStallSpeed;
    else
      vUp=vector3df(0.0f,0.0f,0.0f);

    vUp=0.15f*fUp*vUp;

    f32 fForewardDamp=m_fForewardVel*m_fForewardVel*0.005f;
    vector3df vForewardDamp=vForeward*fForewardDamp;

    vector3df vSideward=vRot.rotationToDirection(vector3df(1,0,0));
    f32 fSideward=vSideward.dotProduct(vNormVel.normalize());

    vector3df vSideDamp=vector3df(0.0f,0.0f,0.0f);

    if (fSideward>=0.01f || fSideward<=-0.01f) vSideDamp=vSideward*m_fForewardVel*fSideward;

    vector3df vUpward=vRot.rotationToDirection(vector3df(0.0f,1.0f,0.0f));
    f32 fUpward=vUpward.dotProduct(vNormVel.normalize());

    vector3df vUpDamp=vector3df(0.0f,0.0f,0.0f);

    if (fUpward>=0.1f || fUpward<=-0.1f) vUpDamp=vUpward*m_fForewardVel*fUpward;

    vForce+=vThrust+vUp-vForewardDamp-vSideDamp-vUpDamp;

    m_pPlaneBody->addForce(fAltFact*vForce);
    vector3df vTorque=vector3df(0.0f,0.0f,0.0f);

    if (m_fForewardVel>=2.0f) {
      vTorque=vRot.rotationToDirection(vector3df(m_iPitch,m_iYaw,m_iRoll));
      if (m_fForewardVel>=fMaxUpSpeep)
        vTorque*=(fMaxUpSpeep-2.0f)/40.0f;
      else
        vTorque*=(m_fForewardVel-2.0f)/40.0f;
    }
    m_pPlaneBody->addTorque(fAltFact*vTorque);

    m_pSteer->setServoPos(-5*m_iYaw);

    for(u32 i=0; i<2; i++) {
      m_pPitch[i]->setRotation(vector3df(-3*m_iPitch,i==0?-8:8,0));
      m_pRoll[i]->setRotation(vector3df(i==0?3*m_iRoll:-8*m_iRoll,i==0?4:-4,0));
    }
    m_pYaw->setRotation(vector3df(-3*m_iYaw,13,90));
  }

  return false;
}

bool CPlane::handlesEvent(IIrrOdeEvent *pEvent) {
  return pEvent->getType()==irr::ode::eIrrOdeEventStep;
}


