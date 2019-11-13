  #include "CCar.h"
  #include <math.h>

f32 frontWheelContact::getSlip2() {
  f32 fRet=slip2,f2=f>0?f:-f;

  if (f2<3.0f)
    fRet=0.001;
  else
    if (f2>=3.0f && f2<=400.0f)
      fRet=0.001+((slip2-0.001)*(f2-3.0f)/397);

  return fRet;
}

s32 frontWheelContact::getMode() {
  vector3df rot=m_pCar->getAbsoluteTransformation().getRotationDegrees().invert(),v=m_pCar->getLinearVelocity();
  f=((rot.rotationToDirection(v).getHorizontalAngle().Y-270.0f)+(m_pJoint->getParameter(0,CIrrOdeJoint::eLoStop)*180/M_PI))*v.getLength()/2;

  return f>=-0.5f && f<=0.5 && !m_bSteering?mode&(!slip1 & !slip2):mode;
}

f32 rearWheelContact::getSlip2() {
  f32 fRet=slip2,f2=f>0?f:-f;

  if (f2<3.0f)
    fRet=0.001;
  else
    if (f2>=3.0f && f2<=200.0f)
      fRet=0.001+((slip2-0.001)*(f2-3.0f)/197);

  return fRet;
}

s32 rearWheelContact::getMode() {
  vector3df rot=m_pCar->getAbsoluteTransformation().getRotationDegrees().invert(),v=m_pCar->getLinearVelocity();
  f=(rot.rotationToDirection(v).getHorizontalAngle().Y-270.0f)*v.getLength();

  return f>=-0.5f && f<=0.5 && !m_bSteering?mode&(!slip1 & !slip2):mode;
}

CCar::CCar(IrrlichtDevice *pDevice) : CVehicle(pDevice,L"Car","../../data/irrOdeCarHelp.txt") {
  m_pCarBody=reinterpret_cast<CIrrOdeBody *>(m_pSmgr->getSceneNodeFromName("theCar"));

  m_pMotor[0]=reinterpret_cast<CIrrOdeMotor *>(m_pSmgr->getSceneNodeFromName("motor1"));
  m_pMotor[1]=reinterpret_cast<CIrrOdeMotor *>(m_pSmgr->getSceneNodeFromName("motor2"));

  m_pServo[0]=reinterpret_cast<CIrrOdeServo *>(m_pSmgr->getSceneNodeFromName("steer1"));
  m_pServo[1]=reinterpret_cast<CIrrOdeServo *>(m_pSmgr->getSceneNodeFromName("steer2"));

  m_pWheelGeoms[0]=reinterpret_cast<CIrrOdeGeomSphere *>(m_pSmgr->getSceneNodeFromName("w1"));
  m_pWheelGeoms[1]=reinterpret_cast<CIrrOdeGeomSphere *>(m_pSmgr->getSceneNodeFromName("w2"));
  m_pWheelGeoms[2]=reinterpret_cast<CIrrOdeGeomSphere *>(m_pSmgr->getSceneNodeFromName("w3"));
  m_pWheelGeoms[3]=reinterpret_cast<CIrrOdeGeomSphere *>(m_pSmgr->getSceneNodeFromName("w4"));


  m_iSteer=0;
  m_fSteer=45.0f;
  m_iAcc=0;
  m_bHelp=false;
  m_bFlip=false;
  m_bBrake=false;
  m_bBoost=false;
  m_bAdaptSteer=true;

  m_pCam=m_pSmgr->addCameraSceneNode();

  m_pInfo=m_pGuiEnv->addStaticText(L"Hello World!",rect<s32>(5,5,150,55),true);
  m_pInfo->setDrawBackground(true);
  m_pInfo->setBackgroundColor(SColor(0x80,0xFF,0xFF,0xFF));
  m_pInfo->setVisible(false);
}

CCar::~CCar() {
}

void CCar::activate() {
  m_pSmgr->setActiveCamera(m_pCam);
  m_pInfo->setVisible(true);
  m_pDevice->setEventReceiver(this);
  m_pDevice->getCursorControl()->setVisible(false);
  m_bSwitchToMenu=false;
}

void CCar::deactivate() {
  m_pInfo->setVisible(false);
}

u32 CCar::update() {
  u32 iRet=CVehicle::update();
  vector3df pos=m_pCarBody->getRotation().rotationToDirection(vector3df(5,1.5,0)),
            up =m_pCarBody->getRotation().rotationToDirection(vector3df(0,0.1,0)),
            tgt=m_pCarBody->getRotation().rotationToDirection(vector3df(0,1  ,0));

  m_pCam->setPosition(m_pCarBody->getPosition()+pos);
  m_pCam->setUpVector(up);
  m_pCam->setTarget(m_pCarBody->getPosition()+tgt);

  f32 v=m_pCarBody->getLinearVelocity().getLength();

  if (!adaptSteer())
    m_fActSteer=m_fSteer;
  else
    if (v<10.0f)
      m_fActSteer=m_fSteer;
    else
      if (v>45.0f)
        m_fActSteer=25.0f;
      else {
        m_fActSteer=m_fSteer-(m_fSteer-25.0f)*(v-10.0f)/35.0f;
      }

  switch (getSteer()) {
    case 1:
      m_pCarBody->updateAbsolutePosition();
      for (int i=0; i<2; i++) m_pServo[i]->setServoPos(m_fActSteer);
      break;

    case -1:
      for (int i=0; i<2; i++) m_pServo[i]->setServoPos(-m_fActSteer);
      break;

    default:
      for (int i=0; i<2; i++) m_pServo[i]->setServoPos(0);
      break;
  }

  switch (getAcc()) {
    case 1:
      for (int i=0; i<2; i++) {
        m_pMotor[i]->setVelocity(100.0f);
        m_pMotor[i]->setForce(getBoost()?15:5);
      }
      break;

    case -1:
      for (int i=0; i<2; i++) {
        m_pMotor[i]->setVelocity(-150.0f);
        m_pMotor[i]->setForce(getBoost()?15:5);
      }
      break;

    default:
      for (int i=0; i<2; i++) {
        m_pMotor[i]->setVelocity(0.0f);
        m_pMotor[i]->setForce(2);
      }
      break;
  }

  if (getBrake()) {
    for (int i=0; i<2; i++) {
        m_pMotor[i]->setVelocity(0.0f);
        m_pMotor[i]->setForce(75);
    }
  }

  if (getFlip()) {
    vector3df t=m_pCarBody->getAbsoluteTransformation().getRotationDegrees().rotationToDirection(vector3df(2000,0,0));
    m_pCarBody->setTorque(t);
  }

  wchar_t dummy[0xFF];
  pos=m_pCarBody->getAbsolutePosition();
  swprintf(dummy,0xFE,L"vel: %.2f\npos: (%.0f, %.0f, %.0f)\nsteer: %.2f",v,pos.X,pos.Y,pos.Z,m_fActSteer);
  if (getBoost()) swprintf(dummy,0xFF,L"%s\nBOOST!",dummy);
  m_pInfo->setText(dummy);

  return iRet;
}

bool CCar::OnEvent(const SEvent &event) {
  if (CVehicle::OnEvent(event)) return true;

  if (event.EventType==EET_KEY_INPUT_EVENT) {
    if (event.KeyInput.PressedDown) {
      switch (event.KeyInput.Key) {
        case KEY_UP   : m_iAcc  =-1; return true; break;
        case KEY_DOWN : m_iAcc  = 1; return true; break;
        case KEY_LEFT : m_iSteer= 1; return true; break;
        case KEY_RIGHT: m_iSteer=-1; return true; break;

        case KEY_SPACE : m_bBrake=true; return true; break;
        case KEY_KEY_B : m_bBoost=true; return true; break;
        case KEY_RETURN: m_bFlip =true; return true; break;

        case KEY_KEY_X: m_bAdaptSteer=!m_bAdaptSteer; break;
        case KEY_F1   : m_bHelp      =!m_bHelp; break;
        default:
          break;
      }
    }
    else {
      switch (event.KeyInput.Key) {
        case KEY_UP  :
        case KEY_DOWN: m_iAcc=0; return true; break;

        case KEY_LEFT :
        case KEY_RIGHT: m_iSteer=0; return true; break;

        case KEY_SPACE: m_bBrake=false; return true; break;
        case KEY_KEY_B: m_bBoost=false; return true; break;

        default:
          break;
      }
    }
  }
  return false;
}

int CCar::getSteer() { return m_iSteer; }
int CCar::getAcc  () { return m_iAcc  ; }

bool CCar::getBrake() { return m_bBrake; }
bool CCar::getBoost() { return m_bBoost; }

bool CCar::getFlip() { bool bRet=m_bFlip; m_bFlip=false; return bRet; }

bool CCar::adaptSteer () { return m_bAdaptSteer; }

bool CCar::getHelp() { return m_bHelp; }

