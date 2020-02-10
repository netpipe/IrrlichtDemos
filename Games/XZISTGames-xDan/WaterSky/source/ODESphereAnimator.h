
#ifndef __ODESphereAnimator_h
#define __ODESphereAnimator_h

#include "ODEAnimator.h"

class ODESphereAnimator : public ODEAnimator
{
public:
    ODESphereAnimator(scene::ISceneNode *, ODEPhysics *, u32, f32);
};

#endif
