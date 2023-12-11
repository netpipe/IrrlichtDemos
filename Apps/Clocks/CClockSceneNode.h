#ifndef __C_CLOCK_SCENE_NODE_H_INCLUDED__
#define __C_CLOCK_SCENE_NODE_H_INCLUDED__

/* NOTE (Acki#1#): I know this should be an animator and not a node,
   but I'm to lazy to learn how to use animators !!! ;)) */

#include <irrlicht.h>
#ifdef win32
#include <windows.h> // NOTE (Linux#9#): use Irrlicht timer
#endif
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

    virtual void rotateNodes();

  public:
    CClockSceneNode(irr::IrrlichtDevice* device, ISceneNode* nodeH = 0, ISceneNode* nodeM = 0, ISceneNode* nodeS = 0);
scene::ISceneManager* mgr;
    irr::IrrlichtDevice*  mgr2;
    virtual void OnRegisterSceneNode();
    virtual void render();
    virtual const core::aabbox3d<f32>& getBoundingBox() const;

    virtual void setNodes(ISceneNode* nH, ISceneNode* nM, ISceneNode* nS);
    virtual void setNodeH(ISceneNode* nH);
    virtual void setNodeM(ISceneNode* nM);
    virtual void setNodeS(ISceneNode* nS);
    virtual void setRotationAxis(core::vector3df ra);
    ISceneNode* getNodeH();
    ISceneNode* getNodeM();
    ISceneNode* getNodeS();
    core::vector3df getRotationAxis();

    virtual void setTime(u32 h, u32 m, u32 s);
    virtual void setUseRealTime(bool useRT);
    virtual bool getUseRealTime();
    virtual void setAutoUpdate(bool doAU);
    virtual bool getAutoUpdate();
    virtual void setSecondsForDay(u32 secs);
    virtual u32 getSecondsForDay();
    virtual void setUpdatesPerSecond(u32 upd);
    virtual u32 getUpdatesPerSecond();

    virtual void setSyncClock(CClockSceneNode* clock);
    virtual void setSyncClock(gui::CGUIClock* clock);
    virtual CClockSceneNode* getSyncSceneClock();
    virtual gui::CGUIClock* getSyncGUIClock();

    virtual u32 getGameTimeHours();
    virtual u32 getGameTimeMinutes();
    virtual u32 getGameTimeSeconds();
    virtual core::stringc getGameTimeString();
    virtual core::stringw getGameTimeStringW();
};

}} // end namespaces
#endif // __C_CLOCK_SCENE_NODE_H_INCLUDED__
