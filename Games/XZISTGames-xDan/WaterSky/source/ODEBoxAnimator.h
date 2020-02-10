
#ifndef __ODEBoxAnimator_h
#define __ODEBoxAnimator_h

#include "ODEAnimator.h"

class ODEBoxAnimator : public ODEAnimator
{
public:
    ODEBoxAnimator(scene::ISceneNode *, ODEPhysics *, u32, core::vector3df);
};

#endif
