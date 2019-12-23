#ifndef CCLOCKGUIELEMENT_H
#define CCLOCKGUIELEMENT_H

#include <irrlicht.h>
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

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

  public:
    IrrlichtDevice *device;
    CGUIClock(core::rect<s32> rect, IrrlichtDevice *dev, IGUIEnvironment* env, IGUIElement* parent=0, video::E_MATERIAL_TYPE materialtype = video::EMT_TRANSPARENT_ALPHA_CHANNEL);
    void setBodyTexture(video::ITexture* texture);
    void setNeedleTexture(video::ITexture* txtH, video::ITexture* txtM, video::ITexture* txtS);
    void setSize(core::dimension2d<s32> size);
    void setPosition(core::position2d<s32> pos);
    void setRect(core::rect<s32> rect);
    virtual void draw();

    void setTime(u32 h, u32 m, u32 s);
    void setShowSeconds(bool showS);
    bool getShowSeconds();
    void setUseRealTime(bool useRT);
    bool getUseRealTime();
    void setAutoUpdate(bool doAU);
    bool getAutoUpdate();
    void setSecondsForDay(u32 secs);
    u32 getSecondsForDay();
    void setUpdatesPerSecond(u32 upd);
    u32 getUpdatesPerSecond();

    void setSyncClock(CGUIClock* clock);
    void setSyncClock(scene::CClockSceneNode* clock);
    CGUIClock* getSyncGUIClock();
    scene::CClockSceneNode* getSyncSceneClock();

    u32 getGameTimeHours();
    u32 getGameTimeMinutes();
    u32 getGameTimeSeconds();
    core::stringc getGameTimeString();
    core::stringw getGameTimeStringW();

};
}} // end namespaces
#endif // CCLOCKGUIELEMENT_H
