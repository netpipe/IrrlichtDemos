
#ifndef ROTATE_TO_ANIMATOR_H
#define ROTATE_TO_ANIMATOR_H

#include "Litha.h"

class RotateToAnimator : public IMotionAnimator
{
	f32 finalY;
	
public:
	RotateToAnimator(f32 finalY)
		: finalY(finalY)
	{
	}
	
	void Init(ITransformable *target)
	{
	}
	
	void SetFinalY(f32 finalY)
	{
		this->finalY = finalY;
	}
	
	void Animate(ITransformable *target, f32 dt)
	{
		f32 currentRotY = maths::extract_y_rotation(target->GetRotation());
		
		core::vector3df newRot = core::vector3df(0,
				maths::interpolate_angle( currentRotY, finalY, 10.f*dt ),
				0);
		
		target->SetRotation(newRot);
		
		if ( fabs(newRot.Y - finalY) < 0.01 )
		{
			// Remove!?
		}
	}
};

#endif
