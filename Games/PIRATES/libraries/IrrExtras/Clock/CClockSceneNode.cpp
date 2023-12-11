#include "CClockSceneNode.h"



/*
	/// USAGE EXAMPLES:

	/// Create 2D GUI CLOCK:
	CGUIClock* guiClock = new CGUIClock(rect<s32>(1,1,100,100), device, guienv, guienv->getRootGUIElement());
	guiClock->drop();
	ITimer *real_time = device->getTimer();
	int curTimeH = real_time->getRealTimeAndDate().Hour;
	int curTimeM = real_time->getRealTimeAndDate().Minute;
	int curTimeS = real_time->getRealTimeAndDate().Second;
	guiClock->setTime(curTimeH, curTimeM, curTimeS);
	guiClock->setUpdatesPerSecond(20);
	guiClock->setAutoUpdate(true);
	guiClock->setShowSeconds(true);
	guiClock->setUseRealTime(true);
	guiClock->setSecondsForDay(3600); // 1 day per hour
	guiClock->setVisible(true);


    /// Create 3D GUI CLOCK:
	scene::ISceneNode* nBody = smgr->addMeshSceneNode(smgr->getMesh("clock_body.obj"));
	nBody->setPosition(vector3df(0, 193, -680));
	scene::ISceneNode* nHour = smgr->addMeshSceneNode(smgr->getMesh("clock_hours.obj"));
	nHour->setPosition(vector3df(0, 193, -680));
	scene::ISceneNode* nMinute = smgr->addMeshSceneNode(smgr->getMesh("clock_minutes.obj"));
	nMinute->setPosition(vector3df(0, 193, -680));
	scene::ISceneNode* nSecond = smgr->addMeshSceneNode(smgr->getMesh("clock_seconds.obj"));
	nSecond->setPosition(vector3df(0, 193, -680));


	CClockSceneNode* nodeClock = new CClockSceneNode(device, smgr, nHour, nMinute, nSecond);
	nodeClock->drop();
	ITimer *real_time = device->getTimer();
	int curTimeH = real_time->getRealTimeAndDate().Hour;
	int curTimeM = real_time->getRealTimeAndDate().Minute;
	int curTimeS = real_time->getRealTimeAndDate().Second;
	nodeClock->setTime(curTimeH, curTimeM, curTimeS);
	nodeClock->setUpdatesPerSecond(20);
	nodeClock->setAutoUpdate(true);
	// nodeClock->setShowSeconds(true);
	nodeClock->setUseRealTime(true);
	nodeClock->setVisible(false); /// Hide it, its only a dummy clock.
*/



namespace irr{
namespace scene{

CClockSceneNode::CClockSceneNode(IrrlichtDevice *dev, scene::ISceneManager* mgr, ISceneNode* nH, ISceneNode* nM, ISceneNode* nS):ISceneNode(mgr->getRootSceneNode(), mgr){
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
  /// rotAxis = core::vector3df(0,0,-1); /// <-------------------- The CORRECT clock axis...
  rotAxis = core::vector3df(0,0,1);      /// <-------------------- BUT!... Only in my case, I need this axis mirrored.....
  device = dev;
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


  /// IrrlichtDevice *device;

  // Update clock
  /// static u32 lastTick = GetTickCount(); // NOTE (Linux#9#): use Irrlicht timer
  static u32 lastTick = 0; // device->getTimer()->getTime();
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
    /// if(GetTickCount() - lastTick > updPerSecond){ // NOTE (Linux#9#): use Irrlicht timer
    if(device->getTimer()->getTime() - lastTick > updPerSecond){ // NOTE (Linux#9#): use Irrlicht timer
      if(useRealTime){
        // NOTE (Linux#9#): use Irrlicht timer

        ITimer *real_time = device->getTimer();
        curTimeH = real_time->getRealTimeAndDate().Hour;
        curTimeM = real_time->getRealTimeAndDate().Minute;
        curTimeS = real_time->getRealTimeAndDate().Second;

        /// SYSTEMTIME tm;
        /// GetSystemTime(&tm);
        /// curTimeH = tm.wHour + 2;
        /// curTimeM = tm.wMinute;
        /// curTimeS = tm.wSecond;
      }else{
        /// f32 realSecsPassed = (f32)(GetTickCount() - lastTick) / 1000.0; // NOTE (Linux#9#): use Irrlicht timer
        f32 realSecsPassed = (f32)(device->getTimer()->getTime() - lastTick) / 1000.0; // NOTE (Linux#9#): use Irrlicht timer
        f32 gameSecsPassed = realSecsPassed * 86400.0 / (f32)secsForDay;
        curTimeS  += (u32)gameSecsPassed;
        for(; curTimeS > 59; ++curTimeM) curTimeS -= 60;
        for(; curTimeM > 59; ++curTimeH) curTimeM -= 60;
        curTimeH = (u32)curTimeH % 24;
      }
      /// lastTick = GetTickCount(); // NOTE (Linux#9#): use Irrlicht timer
      lastTick = device->getTimer()->getTime(); // NOTE (Linux#9#): use Irrlicht timer
      rotateNodes();
    }
  }else{
    /// lastTick = GetTickCount(); // NOTE (Linux#9#): use Irrlicht timer
    lastTick = device->getTimer()->getTime(); // NOTE (Linux#9#): use Irrlicht timer
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

void CClockSceneNode::setVisibility(bool visible){
  nodeH->setVisible(visible);
  nodeM->setVisible(visible);
  nodeS->setVisible(visible);
};

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
