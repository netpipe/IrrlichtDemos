#ifndef _PLAYERANIMATOR_H_
#define _PLAYERANIMATOR_H_
#include "ISceneNodeAnimator.h"
#include "ISceneNode.h"
#include "irrMap.h"
namespace irr{
namespace game{
class CPlayerAnimator: public scene::ISceneNodeAnimator{
    public:
        enum Action {LEFT,RIGHT,UP,DOWN,COUNT};
        CPlayerAnimator();
        virtual bool OnEvent(const SEvent& event);
        virtual void animateNode(scene::ISceneNode *node, u32 timeMs);
        virtual void allKeyUp();
        core::map<s32,Action> KeyToAction;
        core::map<Action,bool> ActionDown;
        f32 MaxX;
        f32 MinX;
        f32 MaxY;
        f32 MinY;
        bool ControlEnabled;
    protected:
        u32 LastTime;
    private:
        CPlayerAnimator(const CPlayerAnimator& other);
        CPlayerAnimator& operator=(const CPlayerAnimator& other);
        virtual scene::ISceneNodeAnimator* createClone(scene::ISceneNode* node,scene::ISceneManager* newManager=0);
};
}}//end irr::game
#endif
