#include "CGUIClock.h"

namespace irr{
namespace gui{
void CGUIClock::SetupQuadMesh(scene::SMeshBuffer &mesh, f32 f32Width, video::E_MATERIAL_TYPE materialtype){
  mesh.Vertices.set_used(4);
  mesh.Indices.set_used(6);
  video::SColor white(255, 255, 255, 255);
  mesh.Vertices[0] = video::S3DVertex(-f32Width, -f32Width, 0.f, 0.f, 0.f, 1.f, white, 0.f, 1.f);
  mesh.Vertices[1] = video::S3DVertex(-f32Width,  f32Width, 0.f, 0.f, 0.f, 1.f, white, 0.f, 0.f);
  mesh.Vertices[2] = video::S3DVertex( f32Width,  f32Width, 0.f, 0.f, 0.f, 1.f, white, 1.f, 0.f);
  mesh.Vertices[3] = video::S3DVertex( f32Width, -f32Width, 0.f, 0.f, 0.f, 1.f, white, 1.f, 1.f);
  mesh.Indices[0] = 0;
  mesh.Indices[1] = 1;
  mesh.Indices[2] = 2;
  mesh.Indices[3] = 2;
  mesh.Indices[4] = 3;
  mesh.Indices[5] = 0;
  mesh.getMaterial().Lighting = false;
  mesh.getMaterial().MaterialType = materialtype;
}
CGUIClock::CGUIClock(core::rect<s32> rect, IrrlichtDevice *dev, IGUIEnvironment* env, IGUIElement* parent, video::E_MATERIAL_TYPE materialtype): IGUIElement(/* IrrExt13_ClockGUI */ EGUIET_ELEMENT, env, parent, -1, rect){
  #ifdef _DEBUG
  setDebugName("CGUIClock");
  #endif
  drawRect = rect;
  SetupQuadMesh(BodyMesh, 1.0f, materialtype);
  SetupQuadMesh(NeedleMeshH, 1.0f, materialtype);
  SetupQuadMesh(NeedleMeshM, 1.0f, materialtype);
  SetupQuadMesh(NeedleMeshS, 1.0f, materialtype);
  // defaults
  showSeconds = true;
  useRealTime = true;
  autoUpdate = true;
  curTimeH = 0;
  curTimeM = 0;
  curTimeS = 0;
  secsForDay = 3200; // = 1h
  updPerSecond = 100; // = 10 Updates
  syncGUIClock = 0;
  syncSceneClock = 0;
  device = dev;

}
void CGUIClock::setBodyTexture(video::ITexture* texture){
  BodyMesh.getMaterial().setTexture(0, texture);
}
void CGUIClock::setNeedleTexture(video::ITexture* txtH, video::ITexture* txtM, video::ITexture* txtS){
  NeedleMeshH.getMaterial().setTexture(0, txtH);
  NeedleMeshM.getMaterial().setTexture(0, txtM);
  NeedleMeshS.getMaterial().setTexture(0, txtS);
}
void CGUIClock::draw(){

  /// IrrlichtDevice *device;

  /// static u32 lastTick = GetTickCount(); // NOTE (Linux#9#): use Irrlicht timer
  static u32 lastTick = device->getTimer()->getTime(); // NOTE (Linux#9#): use Irrlicht timer
  if(syncGUIClock){
    curTimeH = syncGUIClock->getGameTimeHours();
    curTimeM = syncGUIClock->getGameTimeMinutes();
    curTimeS = syncGUIClock->getGameTimeSeconds();
  }else if(syncSceneClock){
    curTimeH = syncSceneClock->getGameTimeHours();
    curTimeM = syncSceneClock->getGameTimeMinutes();
    curTimeS = syncSceneClock->getGameTimeSeconds();
  }else if(autoUpdate){

    if(device->getTimer()->getTime() - lastTick > updPerSecond){ // NOTE (Linux#9#): use Irrlicht timer
    /// if(GetTickCount() - lastTick > updPerSecond){ // NOTE (Linux#9#): use Irrlicht timer
      if(useRealTime){

        ITimer *real_time = device->getTimer();
        curTimeH = real_time->getRealTimeAndDate().Hour;
        curTimeM = real_time->getRealTimeAndDate().Minute;
        curTimeS = real_time->getRealTimeAndDate().Second;

        // NOTE (Linux#9#): use Irrlicht timer
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
      lastTick = device->getTimer()->getTime();
    }
  }else{
    /// lastTick = GetTickCount(); // NOTE (Linux#9#): use Irrlicht timer
    lastTick = device->getTimer()->getTime();
  }

  video::IVideoDriver* driver = Environment->getVideoDriver();
  if(!(driver && IsVisible)) return;
  core::rect<s32> oldViewPort = driver->getViewPort();

  // draw clock
  driver->setViewPort(drawRect);
  core::matrix4 oldProjMat = driver->getTransform(video::ETS_PROJECTION);

  // body
  driver->setTransform(video::ETS_PROJECTION, core::matrix4());
  core::matrix4 oldViewMat = driver->getTransform(video::ETS_VIEW);
  driver->setTransform(video::ETS_VIEW, core::matrix4());
  driver->setTransform(video::ETS_WORLD, core::matrix4());
  driver->setMaterial(BodyMesh.Material);
  driver->drawMeshBuffer(&BodyMesh);
  // seconds
  if(showSeconds){
    MatrixS.makeIdentity();
    /// MatrixS.setRotationDegrees(core::vector3df(0, 0, (curTimeS * -6.0)));
    MatrixS.setRotationDegrees(core::vector3df(0, 0, (curTimeS * -6.0)));
    driver->setTransform(video::ETS_WORLD, MatrixS);
    driver->setMaterial(NeedleMeshS.Material);
    driver->drawMeshBuffer(&NeedleMeshS);
  }
  // minutes
  MatrixM.makeIdentity();
  /// MatrixM.setRotationDegrees(core::vector3df(0, 0, (curTimeM * -6.0)));
  MatrixM.setRotationDegrees(core::vector3df(0, 0, (curTimeM * -6.0)));
  driver->setTransform(video::ETS_WORLD, MatrixM);
  driver->setMaterial(NeedleMeshM.Material);
  driver->drawMeshBuffer(&NeedleMeshM);
  // hours (incl. minutes)
  MatrixH.makeIdentity();
  /// MatrixH.setRotationDegrees(core::vector3df(0, 0, (((f32)((u32)curTimeH % 12) + curTimeM / 60.0) * -30.0)));
  MatrixH.setRotationDegrees(core::vector3df(0, 0, (((f32)((u32)curTimeH % 12) + curTimeM / 60.0) * -30.0)));
  driver->setTransform(video::ETS_WORLD, MatrixH);
  driver->setMaterial(NeedleMeshH.Material);
  driver->drawMeshBuffer(&NeedleMeshH);

  driver->setTransform(video::ETS_PROJECTION, oldProjMat);
  driver->setViewPort(oldViewPort);
}
void CGUIClock::setSize(core::dimension2d<s32> size){
  drawRect.LowerRightCorner.X = drawRect.UpperLeftCorner.X + size.Width;
  drawRect.LowerRightCorner.Y = drawRect.UpperLeftCorner.Y + size.Height;
}
void CGUIClock::setPosition(core::position2d<s32> pos){
  core::dimension2d<s32> size;
  size.Width = drawRect.getWidth();
  size.Height = drawRect.getHeight();
  drawRect.UpperLeftCorner.X = pos.X;
  drawRect.UpperLeftCorner.Y = pos.Y;
  drawRect.LowerRightCorner.X = drawRect.UpperLeftCorner.X + size.Width;
  drawRect.LowerRightCorner.Y = drawRect.UpperLeftCorner.Y + size.Height;
}
void CGUIClock::setRect(core::rect<s32> rect){
  drawRect = rect;
}

void CGUIClock::setTime(u32 h, u32 m, u32 s){
  // seconds
  for(; s > 59; ++m) s -= 60;
  curTimeS = s;
  // minutes
  for(; m > 59; ++h) m -= 60;
  curTimeM = m;
  // hours
  curTimeH = h % 24;
}
void CGUIClock::setUseRealTime(bool useRT){
  useRealTime = useRT;
}
bool CGUIClock::getUseRealTime(){
  return useRealTime;
}
void CGUIClock::setAutoUpdate(bool doAU){
  autoUpdate = doAU;
}
bool CGUIClock::getAutoUpdate(){
  return autoUpdate;
}
void CGUIClock::setSecondsForDay(u32 secs){
  secsForDay = secs;
}
u32 CGUIClock::getSecondsForDay(){
  return secsForDay;
}
void CGUIClock::setShowSeconds(bool showS){
  showSeconds = showS;
}
bool CGUIClock::getShowSeconds(){
  return showSeconds;
}
void CGUIClock::setUpdatesPerSecond(u32 upd){
  if(upd > 100) upd = 100;
  if(upd < 1) upd = 1;
  updPerSecond = 1000 / upd;
}
u32 CGUIClock::getUpdatesPerSecond(){
  return updPerSecond * 1000;
}
void CGUIClock::setSyncClock(CGUIClock* clock){
  syncGUIClock = clock;
  syncSceneClock = 0; // only one clock allowed
}
void CGUIClock::setSyncClock(scene::CClockSceneNode* clock){
  syncSceneClock = clock;
  syncGUIClock = 0; // only one clock allowed
}
CGUIClock* CGUIClock::getSyncGUIClock(){
  return syncGUIClock;
}
scene::CClockSceneNode* CGUIClock::getSyncSceneClock(){
  return syncSceneClock;
}

u32 CGUIClock::getGameTimeHours(){
  return (u32)curTimeH;
}
u32 CGUIClock::getGameTimeMinutes(){
  return (u32)curTimeM;
}
u32 CGUIClock::getGameTimeSeconds(){
  return (u32)curTimeS;
}
core::stringc CGUIClock::getGameTimeString(){
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
  if(showSeconds){
    {core::stringc tmp((u32)curTimeS);
      ret += ":";
      if(tmp.size() == 1) ret += "0";
      ret += tmp;
    }
  }
  return ret;
}
core::stringw CGUIClock::getGameTimeStringW(){
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
  if(showSeconds){
    {core::stringc tmp((u32)curTimeS);
      ret += L":";
      if(tmp.size() == 1) ret += L"0";
      ret += tmp;
    }
  }
  return ret;
}

}} // end namespaces
