
#ifndef __ODESphereAnimator_h
#define __ODESphereAnimator_h

#include "ODEAnimator.h"

class ODESphereAnimator : public ODEAnimator
{
		virtual ISceneNodeAnimator* createClone(scene::ISceneNode* node,scene::ISceneManager* newManager=0) {};
//		virtual ISceneNodeAnimator* createClone(ISceneNode* node,
//				ISceneManager* newManager=0) =0;

public:
    ODESphereAnimator(scene::ISceneNode *, ODEPhysics *, u32, f32);
};

#endif
