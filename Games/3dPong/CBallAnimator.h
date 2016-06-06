#ifndef _CBALLANIMATOR_H_
#define _CBALLANIMATOR_H_
#include "ISceneNodeAnimator.h"
#include "dimension2d.h"
#include "ITriangleSelector.h"
namespace irr{
namespace game{
class CBallAnimator:public scene::ISceneNodeAnimator{
    public:
        CBallAnimator();
        ~CBallAnimator();
        virtual void animateNode (scene::ISceneNode *node, u32 timeMs);
        virtual void setTriangleSelector(scene::ITriangleSelector* selector);
        static core::vector3df reflectVector(const core::vector3df & vector, const core::vector3df & axis);
        core::vector3df Velocity;
    protected:
        scene::ITriangleSelector* Selector;
        u32 LastTimeMs;
    private:
        CBallAnimator(const CBallAnimator& other);
        CBallAnimator& operator=(const CBallAnimator& other);
        scene::ISceneNodeAnimator* createClone(scene::ISceneNode* node,scene::ISceneManager* newManager=0);
};
}}//
#endif
