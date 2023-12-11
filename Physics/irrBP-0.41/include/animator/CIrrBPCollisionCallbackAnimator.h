#ifndef CIrrBP_CCBL_ANIM_H_
#define CIrrBP_CCBL_ANIM_H_
#include "animator/CIrrBPAnimator.h"

class CIrrBPCollisionObject;
class CIrrBPWorld;

class CollisionResultCallback
{
public:
	CollisionResultCallback()	{	}
	virtual ~CollisionResultCallback()	{	}
	virtual void addSingleResult(const irr::core::vector3df & point)=0;
};

//!Please note that collision callback against soft body is not yet implemented in Bullet Physics.<br> So The Collision Delete Animator DOESN'T works with soft bodies.
class CIrrBPCollisionCallbackAnimator  : public CIrrBPAnimator
{
public:
	CIrrBPCollisionCallbackAnimator(CIB_DFLAG collFlag,CIrrBPWorld * world,CollisionResultCallback * resultCallback);
	void setBody(CIrrBPCollisionObject* body) ;
	void animate();
	
	void drop() {delete this;}
private:
	CIB_DFLAG cFlag;
	irr::s16 internalStatus;
	CIrrBPWorld * rWorld;
	contactPoint contact;
	CollisionResultCallback * cbk;
};

#endif