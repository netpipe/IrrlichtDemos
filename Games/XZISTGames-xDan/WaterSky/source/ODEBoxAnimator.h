
#ifndef __ODEBoxAnimator_h
#define __ODEBoxAnimator_h

#include "ODEAnimator.h"

class ODEBoxAnimator : public ODEAnimator
{
		virtual ISceneNodeAnimator* createClone(scene::ISceneNode* node,scene::ISceneManager* newManager=0) {};

public:
    ODEBoxAnimator(scene::ISceneNode *, ODEPhysics *, u32, core::vector3df);
};

#endif
