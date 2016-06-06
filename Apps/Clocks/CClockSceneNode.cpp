#include "CClockSceneNode.h"
#include <irrlicht.h>

using namespace irr;
using namespace scene;
using namespace core;
using namespace io;

namespace irr{
namespace scene{

CClockSceneNode::CClockSceneNode(irr::IrrlichtDevice* device, ISceneNode* nH, ISceneNode* nM, ISceneNode* nS):ISceneNode(device->getSceneManager()->getRootSceneNode(), device->getSceneManager()){
  #ifdef _DEBUG
  setDebugName("CClockSceneNode");
  #endif
  // Defaults
  useRealTime = true;
  autoUpdate = true;
  curTimeH = 0;
  curTimeM = 0;
  curTimeS = 0;
  secsForDay = 3200; // = 1h
  updPerSecond = 100; // = 10 Updates
  syncGUIClock = 0;
  syncSceneClock = 0;
  nodeH = nH;
  nodeM = nM;
  nodeS = nS;
  rotAxis = core::vector3df(0,0,-1);
    mgr= device->getSceneManager();
  mgr2 = device;
}
void CClockSceneNode::render(){
  //  this is not a node forself !!! ;)
}
const core::aabbox3d<f32>& CClockSceneNode::getBoundingBox() const{
  //  this is not a node forself !!! ;)
}

void CClockSceneNode::rotateNodes(){
  if(nodeS){ // seconds
    core::vector3df rotNode = nodeS->getRotation();
    f32 cr = curTimeS * 6.0;
    core::vector3df rotTime = core::vector3df(cr, cr, cr) * rotAxis;
    if(rotAxis.X){
      rotNode.X = rotTime.X;
    }else if(rotAxis.Y){
      rotNode.Y = rotTime.Y;
    }else{
      rotNode.Z = rotTime.Z;
    }
    nodeS->setRotation(rotNode);
  }
  if(nodeM){ // minutes
    core::vector3df rotNode = nodeM->getRotation();
    f32 cr = curTimeM * 6.0;
    core::vector3df rotTime = core::vector3df(cr, cr, cr) * rotAxis;
    if(rotAxis.X){
      rotNode.X = rotTime.X;
    }else if(rotAxis.Y){
      rotNode.Y = rotTime.Y;
    }else{
      rotNode.Z = rotTime.Z;
    }
    nodeM->setRotation(rotNode);
  }
  if(nodeH){ // hours (incl. minutes)
    core::vector3df rotNode = nodeH->getRotation();
    f32 cr = ((f32)((u32)curTimeH % 12) + curTimeM / 60.0) * 30.0;
    core::vector3df rotTime = core::vector3df(cr, cr, cr) * rotAxis;
    if(rotAxis.X){
      rotNode.X = rotTime.X;
    }else if(rotAxis.Y){
      rotNode.Y = rotTime.Y;
    }else{
      rotNode.Z = rotTime.Z;
    }
    nodeH->setRotation(rotNode);
  }
}
void CClockSceneNode::OnRegisterSceneNode(){
  // Update clock
  static u32 lastTick = mgr2->getTimer()->getTime();  // NOTE (Linux#9#): use Irrlicht timer
  if(syncSceneClock){
    curTimeH = syncSceneClock->getGameTimeHours();
    curTimeM = syncSceneClock->getGameTimeMinutes();
    curTimeS = syncSceneClock->getGameTimeSeconds();
    rotateNodes();
  }else if(syncGUIClock){
    curTimeH = syncGUIClock->getGameTimeHours();
    curTimeM = syncGUIClock->getGameTimeMinutes();
    curTimeS = syncGUIClock->getGameTimeSeconds();
    rotateNodes();
  }else if(autoUpdate){
    if(mgr2->getTimer()->getTime() - lastTick > updPerSecond){ // NOTE (Linux#9#): use Irrlicht timer
      if(useRealTime){
        // NOTE (Linux#9#): use Irrlicht timer
//        SYSTEMTIME tm;
//        GetSystemTime(&tm);

        u32 tm = mgr2->getTimer()->getRealTime();

        printf ("RealTime : %u" , tm);

        curTimeH = mgr2->getTimer()->getRealTimeAndDate().Hour;// tm.tm_hour;// tm.wHour + 2;
        curTimeM = mgr2->getTimer()->getRealTimeAndDate().Minute;//tm.wMinute;
        curTimeS = mgr2->getTimer()->getRealTimeAndDate().Second;//tm.wSecond;
      }else{
        f32 realSecsPassed = (f32)(mgr2->getTimer()->getTime() - lastTick) / 1000.0; // NOTE (Linux#9#): use Irrlicht timer
        f32 gameSecsPassed = realSecsPassed * 86400.0 / (f32)secsForDay;
        curTimeS  += (u32)gameSecsPassed;
        for(; curTimeS > 59; ++curTimeM) curTimeS -= 60;
        for(; curTimeM > 59; ++curTimeH) curTimeM -= 60;
        curTimeH = (u32)curTimeH % 24;
      }
      lastTick = mgr2->getTimer()->getTime(); // NOTE (Linux#9#): use Irrlicht timer
      rotateNodes();
    }
  }else{
    lastTick = mgr2->getTimer()->getTime(); // NOTE (Linux#9#): use Irrlicht timer
  }
}

void CClockSceneNode::setTime(u32 h, u32 m, u32 s){
  // seconds
  for(; s > 59; ++m) s -= 60;
  curTimeS = s;
  // minutes
  for(; m > 59; ++h) m -= 60;
  curTimeM = m;
  // hours
  curTimeH = h % 24;
}
void CClockSceneNode::setUseRealTime(bool useRT){
  useRealTime = useRT;
}
bool CClockSceneNode::getUseRealTime(){
  return useRealTime;
}
void CClockSceneNode::setAutoUpdate(bool doAU){
  autoUpdate = doAU;
}
bool CClockSceneNode::getAutoUpdate(){
  return autoUpdate;
}
void CClockSceneNode::setSecondsForDay(u32 secs){
  secsForDay = secs;
}
u32 CClockSceneNode::getSecondsForDay(){
  return secsForDay;
}
void CClockSceneNode::setUpdatesPerSecond(u32 upd){
  if(upd > 100) upd = 100;
  if(upd < 1) upd = 1;
  updPerSecond = 1000 / upd;
}
u32 CClockSceneNode::getUpdatesPerSecond(){
  return updPerSecond * 1000;
}
void CClockSceneNode::setSyncClock(CClockSceneNode* clock){
  syncSceneClock = clock;
  syncGUIClock = 0; // only one clock allowed
}
void CClockSceneNode::setSyncClock(gui::CGUIClock* clock){
  syncGUIClock = clock;
  syncSceneClock = 0; // only one clock allowed
}
CClockSceneNode* CClockSceneNode::getSyncSceneClock(){
  return syncSceneClock;
}
gui::CGUIClock* CClockSceneNode::getSyncGUIClock(){
  return syncGUIClock;
}
u32 CClockSceneNode::getGameTimeHours(){
  return (u32)curTimeH;
}
u32 CClockSceneNode::getGameTimeMinutes(){
  return (u32)curTimeM;
}
u32 CClockSceneNode::getGameTimeSeconds(){
  return (u32)curTimeS;
}
core::stringc CClockSceneNode::getGameTimeString(){
  core::stringc ret;
  {core::stringc tmp((u32)curTimeH);
    if(tmp.size() == 1) ret += "0";
    ret += tmp;
  }
  {core::stringc tmp((u32)curTimeM);
    ret += ":";
    if(tmp.size() == 1) ret += "0";
    ret += tmp;
  }

//  if(showSeconds)
  {
    {core::stringc tmp((u32)curTimeS);
      ret += ":";
      if(tmp.size() == 1) ret += "0";
      ret += tmp;
    }
  }
  return ret;
}
core::stringw CClockSceneNode::getGameTimeStringW(){
  core::stringw ret;
  {core::stringc tmp((u32)curTimeH);
    if(tmp.size() == 1) ret += L"0";
    ret += tmp;
  }
  {core::stringc tmp((u32)curTimeM);
    ret += L":";
    if(tmp.size() == 1) ret += L"0";
    ret += tmp;
  }
//  if(showSeconds)
  {
    {core::stringc tmp((u32)curTimeS);
      ret += L":";
      if(tmp.size() == 1) ret += L"0";
      ret += tmp;
    }
  }
  return ret;
}

void CClockSceneNode::setNodes(ISceneNode* nH, ISceneNode* nM, ISceneNode* nS){
  nodeH = nH;
  nodeM = nM;
  nodeS = nS;
}
void CClockSceneNode::setNodeH(ISceneNode* nH){
  nodeH = nH;
}
void CClockSceneNode::setNodeM(ISceneNode* nM){
  nodeM = nM;
}
void CClockSceneNode::setNodeS(ISceneNode* nS){
  nodeS = nS;
}
void CClockSceneNode::setRotationAxis(core::vector3df ra){
  rotAxis = ra;
}
ISceneNode* CClockSceneNode::getNodeH(){
  return nodeH;
}
ISceneNode* CClockSceneNode::getNodeM(){
  return nodeM;
}
ISceneNode* CClockSceneNode::getNodeS(){
  return nodeS;
}
core::vector3df CClockSceneNode::getRotationAxis(){
  return rotAxis;
}

}} // end namespaces
