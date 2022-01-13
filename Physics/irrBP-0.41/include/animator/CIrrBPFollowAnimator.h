#ifndef CIrrBP_FOLLOW_ANIM_H_
#define CIrrBP_FOLLOW_ANIM_H_

#include "animator/CIrrBPAnimator.h"

class CIrrBPRigidBody;
class CIrrBPFollowAnimator : public CIrrBPAnimator
{
public:
	CIrrBPFollowAnimator(irr::scene::ISceneNode * node,irr::core::vector3df offset = irr::core::vector3df(0,0,0));
	void setBody(CIrrBPRigidBody* body) {rigidbody = body;}
	void animate();
	void drop() {delete this;}
private:
	CIrrBPRigidBody * rigidbody;
	irr::scene::ISceneNode * toFollow;
	irr::core::vector3df offset;
};

#endif