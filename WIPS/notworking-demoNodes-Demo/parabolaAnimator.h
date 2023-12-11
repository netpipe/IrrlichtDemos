#ifndef __ParabolaAnimator_h
#define __ParabolaAnimator_h

#include "ISceneNode.h"

using namespace irr;
using namespace scene;

class ParabolaAnimator : public ISceneNodeAnimator
{
public:
    ParabolaAnimator(
        scene::ISceneManager *,
        u32,
        const core::vector3df &,
        const core::vector3df &,
        const core::vector3df &,
        const core::vector3df &,
        const core::plane3df &
        );
    virtual void animateNode(ISceneNode *, u32);
private:
    scene::ISceneManager *smgr;
    u32 lastTime;
    core::vector3df pos;
    core::vector3df vel;
    core::vector3df acc;
    core::vector3df rvel;
    core::plane3df ground;
};

#endif 
