
#include "BobAnimator.h"
#include "ITransformable.h"
#include "IWorld.h"
#include "IEngine.h"

BobAnimator::BobAnimator(f32 amplitude, f32 speed)
{
	world = GetEngine()->GetWorld();
	
	this->amplitude = amplitude;
	this->speed = speed;
	
	SetFixed(true);
}

void BobAnimator::Init(ITransformable *target)
{
	startTime = world->GetVirtualTime();
	startPos = target->GetPosition();
}

void BobAnimator::Animate(ITransformable *target, f32 dt)
{
	f32 currentTime = world->GetVirtualTime() - startTime;
	
	if (isFixed)
	{
		target->SetPosition( startPos
				+ core::vector3df(0, 0.5*amplitude*sin(currentTime*speed), 0)
				);
	}
	else
	{
		// Don't use a fixed start position, instead just add the delta.
		// Inaccuracies may cause the position of the object to move over time.
		core::vector3df newPos = core::vector3df(0, 0.5*amplitude*sin(currentTime*speed), 0);
		target->SetPosition( target->GetPosition() + (newPos-lastPos) );
		lastPos = newPos;
	}
}
