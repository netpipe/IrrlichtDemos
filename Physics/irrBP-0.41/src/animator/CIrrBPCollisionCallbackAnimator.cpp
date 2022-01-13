#include "animator/CIrrBPCollisionCallbackAnimator.h"
#include "CIrrBPCollisionObject.h"
#include "CIrrBPWorld.h"

CIrrBPCollisionCallbackAnimator::CIrrBPCollisionCallbackAnimator(CIB_DFLAG collFlag,CIrrBPWorld * world,CollisionResultCallback * resultCallback)
{
	rWorld = world;
	rBody= NULL;
	internalStatus = 0;
	cbk = resultCallback;
	cFlag = collFlag;

}
void CIrrBPCollisionCallbackAnimator::setBody(CIrrBPCollisionObject *body)
{
	rBody = body;
}
void CIrrBPCollisionCallbackAnimator::animate()
{
	if(rBody == NULL || rWorld == NULL || cFlag <= 0)
		return;

	switch(cFlag)
	{
		case ON_COLLIDE:
			if(rWorld->getBodyCollidingPoint(rBody,&contact))
			{
				this->cbk->addSingleResult(contact.point);
				isEnded=true;
			}
		break;
		case ON_COLLISION_RELEASE:
			if(internalStatus == 0)
			{
				if(rWorld->getBodyCollidingPoint(rBody,&contact))
					internalStatus =1;
			}
			else if(internalStatus == 1)
			{
				if(contact.contact ? !rWorld->isBodyColliding(rBody) : !rWorld->getBodyCollidingPoint(rBody,&contact))
					internalStatus =2;
			}
			else if(internalStatus == 2)
			{
				this->cbk->addSingleResult(contact.point);
				isEnded = true;
			}

		break;

	}

}
