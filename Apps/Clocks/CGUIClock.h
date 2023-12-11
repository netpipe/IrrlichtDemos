#ifndef CCLOCKGUIELEMENT_H
#define CCLOCKGUIELEMENT_H

#include <irrlicht.h>
#ifdef win32
#include <windows.h> // NOTE (Linux#9#): use Irrlicht timer
#endif
#include "CClockSceneNode.h"

namespace irr{
namespace scene{
  class CClockSceneNode;
}
namespace gui{
class CGUIClock : public IGUIElement{
  private:
    void SetupQuadMesh(scene::SMeshBuffer &mesh, f32 f32Width, video::E_MATERIAL_TYPE materialtype);
    scene::SMeshBuffer BodyMesh;
    scene::SMeshBuffer NeedleMeshH;
    scene::SMeshBuffer NeedleMeshM;
    scene::SMeshBuffer NeedleMeshS;
    core::rect<s32> drawRect;
    core::matrix4 MatrixH;
    core::matrix4 MatrixM;
    core::matrix4 MatrixS;
    bool showSeconds;
    bool useRealTime;
    bool autoUpdate;
    f32 curTimeH;
    f32 curTimeM;
    f32 curTimeS;
    u32 secsForDay;
    u32 updPerSecond;
    CGUIClock* syncGUIClock;
    scene::CClockSceneNode* syncSceneClock;

    IGUIEnvironment* env;
    irr::IrrlichtDevice* mgr2;

  public:
    CGUIClock(core::rect<s32> rect, irr::IrrlichtDevice* device, IGUIElement* parent=0, video::E_MATERIAL_TYPE materialtype = video::EMT_TRANSPARENT_ALPHA_CHANNEL);
    virtual void setBodyTexture(video::ITexture* texture);
    virtual void setNeedleTexture(video::ITexture* txtH, video::ITexture* txtM, video::ITexture* txtS);
    virtual void setSize(core::dimension2d<s32> size);
    virtual void setPosition(core::position2d<s32> pos);
    virtual void setRect(core::rect<s32> rect);
    virtual void draw();

    virtual void setTime(u32 h, u32 m, u32 s);
    virtual void setShowSeconds(bool showS);
    virtual bool getShowSeconds();
    virtual void setUseRealTime(bool useRT);
    virtual bool getUseRealTime();
    virtual void setAutoUpdate(bool doAU);
    virtual bool getAutoUpdate();
    virtual void setSecondsForDay(u32 secs);
    virtual u32 getSecondsForDay();
    virtual void setUpdatesPerSecond(u32 upd);
    virtual u32 getUpdatesPerSecond();

    virtual void setSyncClock(CGUIClock* clock);
    virtual void setSyncClock(scene::CClockSceneNode* clock);
    virtual CGUIClock* getSyncGUIClock();
    virtual scene::CClockSceneNode* getSyncSceneClock();

    virtual u32 getGameTimeHours();
    virtual u32 getGameTimeMinutes();
    virtual u32 getGameTimeSeconds();
    virtual core::stringc getGameTimeString();
    virtual core::stringw getGameTimeStringW();

};
}} // end namespaces
#endif // CCLOCKGUIELEMENT_H
