
#include "RotationAnimator.h"
#include "ITransformable.h"

RotationAnimator::RotationAnimator(core::vector3df speed)
{
	this->speedRadians = speed * core::DEGTORAD;
}

void RotationAnimator::Animate(ITransformable *target, f32 dt)
{
	core::quaternion rot( target->GetRotation() * core::DEGTORAD );
	core::quaternion delta( speedRadians * dt );
	
	rot = rot * delta;
	
	target->SetRotation( maths::quaternion_to_euler(rot) );
}
