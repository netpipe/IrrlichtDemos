
#ifndef I_MOTION_ANIMATOR_H
#define I_MOTION_ANIMATOR_H

#include "litha_internal.h"

class ITransformable;

class IMotionAnimator : public virtual IReferenceCounted
{
public:
	virtual ~IMotionAnimator(){}
	
	// This is called once, when the animator has been set on a transformable.
	// (One instance of an animator should only be used on a single transformable.)
	virtual void Init(ITransformable *target) = 0;
	
	// Called every logic step.
	// The target is the same as set with Init.
	virtual void Animate(ITransformable *target, f32 dt) = 0;
};

#endif

