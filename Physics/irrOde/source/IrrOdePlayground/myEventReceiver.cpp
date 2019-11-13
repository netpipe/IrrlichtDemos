#include <IrrODE.h>
#include "myEventReceiver.h"

myEventReceiver::myEventReceiver() {
  m_bNewCylinder=false;
  m_bNewCapsule=false;
  m_bNewTrimesh=false;
  m_bNewSphere=false;
  m_bNewBridge=false;
  m_bNewStack=false;
  m_bNewBox=false;

  m_bNewSliderJoint=false;
  m_bNewPistonJoint=false;
  m_bNewHinge2Joint=false;
  m_bNewHingeJoint=false;
  m_bNewFixedJoint=false;
  m_bNewBallJoint=false;

  m_bDrawTerrainGrid=false;

  m_bLeftBtn=false; m_bLeftBtnLast=false;
  m_bRghtBtn=false; m_bRghtBtnLast=false;
  m_bMidlBtn=false; m_bMidlBtnLast=false;

  m_bIncSpeed=false;
  m_bDecSpeed=false;
  m_bPause=false;

  m_bShift=false;
  m_bCtrl=false;
  m_bXKey=false;
  m_bHelp=false;

  m_iWheel=0;
}

myEventReceiver::~myEventReceiver() { }

bool myEventReceiver::OnEvent(const SEvent &event) {
  if (event.EventType==EET_KEY_INPUT_EVENT) {
    m_bShift=event.KeyInput.Shift;
    m_bCtrl =event.KeyInput.Control;

    if (!event.KeyInput.PressedDown) {
      switch (event.KeyInput.Key) {
        case KEY_KEY_S   : m_bNewSphere      =true; break;
        case KEY_KEY_B   : m_bNewBox         =true; break;
        case KEY_KEY_Y   : m_bNewCylinder    =true; break;
        case KEY_KEY_C   : m_bNewCapsule     =true; break;
        case KEY_KEY_T   : m_bNewTrimesh     =true; break;
        case KEY_KEY_1   : m_bNewBridge      =true; break;
        case KEY_KEY_2   : m_bNewStack       =true; break;
        case KEY_F1      : m_bHelp=!m_bHelp;       break;
        case KEY_F5      : m_bNewFixedJoint  =true; break;
        case KEY_F6      : m_bNewBallJoint   =true; break;
        case KEY_F7      : m_bNewHingeJoint  =true; break;
        case KEY_F8      : m_bNewSliderJoint =true; break;
        case KEY_F9      : m_bNewPistonJoint =true; break;
        case KEY_F4      : m_bNewHinge2Joint =true; break;
        case KEY_KEY_P   : m_bPause          =true; break;
        case KEY_ADD     : m_bIncSpeed       =true; break;
        case KEY_SUBTRACT: m_bDecSpeed       =true; break;
        case KEY_KEY_X   : m_bXKey           =true; break;
        case KEY_DELETE  : m_bMidlBtn=1; m_bMidlBtnLast=0; break;
        case KEY_KEY_G   : m_bDrawTerrainGrid=!m_bDrawTerrainGrid; break;
        default: return false; break;
      }

      return true;
    }
  }

  if (event.EventType==EET_MOUSE_INPUT_EVENT) {
    if (event.MouseInput.Event==EMIE_LMOUSE_PRESSED_DOWN) m_bLeftBtn=true;
    if (event.MouseInput.Event==EMIE_LMOUSE_LEFT_UP     ) m_bLeftBtn=false;
    if (event.MouseInput.Event==EMIE_RMOUSE_PRESSED_DOWN) m_bRghtBtn=true;
    if (event.MouseInput.Event==EMIE_RMOUSE_LEFT_UP     ) m_bRghtBtn=false;
    if (event.MouseInput.Event==EMIE_MMOUSE_PRESSED_DOWN) m_bMidlBtn=true;
    if (event.MouseInput.Event==EMIE_MMOUSE_LEFT_UP     ) m_bMidlBtn=false;

    if (event.MouseInput.Event==EMIE_MOUSE_WHEEL) m_iWheel=(s32)event.MouseInput.Wheel;
  }

  return false;
}

bool myEventReceiver::newSphere  () { bool bRet=m_bNewSphere  ; m_bNewSphere  =false; return bRet; }
bool myEventReceiver::newBox     () { bool bRet=m_bNewBox     ; m_bNewBox     =false; return bRet; }
bool myEventReceiver::newCylinder() { bool bRet=m_bNewCylinder; m_bNewCylinder=false; return bRet; }
bool myEventReceiver::newStack   () { bool bRet=m_bNewStack   ; m_bNewStack   =false; return bRet; }
bool myEventReceiver::newTrimesh () { bool bRet=m_bNewTrimesh ; m_bNewTrimesh =false; return bRet; }
bool myEventReceiver::newCapsule () { bool bRet=m_bNewCapsule ; m_bNewCapsule =false; return bRet; }
bool myEventReceiver::newBridge  () { bool bRet=m_bNewBridge  ; m_bNewBridge  =false; return bRet; }

bool myEventReceiver::newFixedJoint () { bool bRet=m_bNewFixedJoint ; m_bNewFixedJoint =false; return bRet; }
bool myEventReceiver::newBallJoint  () { bool bRet=m_bNewBallJoint  ; m_bNewBallJoint  =false; return bRet; }
bool myEventReceiver::newHingeJoint () { bool bRet=m_bNewHingeJoint ; m_bNewHingeJoint =false; return bRet; }
bool myEventReceiver::newSliderJoint() { bool bRet=m_bNewSliderJoint; m_bNewSliderJoint=false; return bRet; }
bool myEventReceiver::newPistonJoint() { bool bRet=m_bNewPistonJoint; m_bNewPistonJoint=false; return bRet; }
bool myEventReceiver::newHinge2Joint() { bool bRet=m_bNewHinge2Joint; m_bNewHinge2Joint=false; return bRet; }

bool myEventReceiver::pause   () { bool bRet=m_bPause   ; m_bPause   =false; return bRet; }
bool myEventReceiver::incSpeed() { bool bRet=m_bIncSpeed; m_bIncSpeed=false; return bRet; }
bool myEventReceiver::decSpeed() { bool bRet=m_bDecSpeed; m_bDecSpeed=false; return bRet; }

bool myEventReceiver::leftBtn() {
  bool bRet=m_bLeftBtn && !m_bLeftBtnLast;
  m_bLeftBtnLast=m_bLeftBtn;
  return bRet;
}

bool myEventReceiver::rghtBtn() {
  bool bRet=m_bRghtBtn && !m_bRghtBtnLast;
  m_bRghtBtnLast=m_bRghtBtn;
  return bRet;
}

bool myEventReceiver::midlBtn() {
  bool bRet=m_bMidlBtn && !m_bMidlBtnLast;
  m_bMidlBtnLast=m_bMidlBtn;
  return bRet;
}

bool myEventReceiver::ctrlDown () { return m_bCtrl ; }
bool myEventReceiver::shiftDown() { return m_bShift; }

bool myEventReceiver::help() { return m_bHelp; }

bool myEventReceiver::xKeyDown() { bool bRet=m_bXKey; m_bXKey=false; return bRet; }

bool myEventReceiver::drawTerrainGrid() { return m_bDrawTerrainGrid; }

s32 myEventReceiver::getWheelDelta() {
  s32 iRet=m_iWheel;
  m_iWheel=0;
  return iRet;
}
