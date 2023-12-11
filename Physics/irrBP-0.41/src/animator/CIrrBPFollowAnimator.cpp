#include "animator/CIrrBPFollowAnimator.h"
#include "body/CIrrBPRigidBody.h"

CIrrBPFollowAnimator::CIrrBPFollowAnimator(irr::scene::ISceneNode * node,irr::core::vector3df offset)
{
	toFollow = node;
	this->offset = offset;
}

void CIrrBPFollowAnimator::animate()
{
	rigidbody->setPosition(toFollow->getPosition() + offset);
}