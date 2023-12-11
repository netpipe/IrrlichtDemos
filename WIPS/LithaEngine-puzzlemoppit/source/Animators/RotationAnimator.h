
#include "IMotionAnimator.h"

class RotationAnimator : public IMotionAnimator
{
	core::vector3df speedRadians;
	
public:
	// Degrees / second
	RotationAnimator(core::vector3df speed);
	
	void Init(ITransformable *target){}
	void Animate(ITransformable *target, f32 dt);
};


