#ifndef __C_CLOCK_SCENE_NODE_H_INCLUDED__
#define __C_CLOCK_SCENE_NODE_H_INCLUDED__

/* NOTE (Acki#1#): I know this should be an animator and not a node,
   but I'm to lazy to learn how to use animators !!! ;)) */

#include <irrlicht.h>
/// #include <windows.h> // NOTE (Linux#9#): use Irrlicht timer
#include "CGUIClock.h"

namespace irr{
namespace gui{
  class CGUIClock;
}
namespace scene{
class CClockSceneNode : public ISceneNode{
  private:
    ISceneNode* nodeH;
    ISceneNode* nodeM;
    ISceneNode* nodeS;
    core::vector3df rotAxis;

    bool useRealTime;
    bool autoUpdate;
    f32 curTimeH;
    f32 curTimeM;
    f32 curTimeS;
    u32 secsForDay;
    u32 updPerSecond;
    CClockSceneNode* syncSceneClock;
    gui::CGUIClock* syncGUIClock;

    void rotateNodes();

  public:
    IrrlichtDevice *device;
    CClockSceneNode(IrrlichtDevice *dev, scene::ISceneManager* mgr, ISceneNode* nodeH = 0, ISceneNode* nodeM = 0, ISceneNode* nodeS = 0);
    virtual void OnRegisterSceneNode();
    virtual void render();
    virtual const core::aabbox3d<f32>& getBoundingBox() const;

    void setVisibility(bool visible);
    void setNodes(ISceneNode* nH, ISceneNode* nM, ISceneNode* nS);
    void setNodeH(ISceneNode* nH);
    void setNodeM(ISceneNode* nM);
    void setNodeS(ISceneNode* nS);
    void setRotationAxis(core::vector3df ra);
    ISceneNode* getNodeH();
    ISceneNode* getNodeM();
    ISceneNode* getNodeS();
    core::vector3df getRotationAxis();

    void setTime(u32 h, u32 m, u32 s);
    void setUseRealTime(bool useRT);
    bool getUseRealTime();
    void setAutoUpdate(bool doAU);
    bool getAutoUpdate();
    void setSecondsForDay(u32 secs);
    u32 getSecondsForDay();
    void setUpdatesPerSecond(u32 upd);
    u32 getUpdatesPerSecond();

    void setSyncClock(CClockSceneNode* clock);
    void setSyncClock(gui::CGUIClock* clock);
    CClockSceneNode* getSyncSceneClock();
    gui::CGUIClock* getSyncGUIClock();

    u32 getGameTimeHours();
    u32 getGameTimeMinutes();
    u32 getGameTimeSeconds();
    core::stringc getGameTimeString();
    core::stringw getGameTimeStringW();
};

}} // end namespaces
#endif // __C_CLOCK_SCENE_NODE_H_INCLUDED__
