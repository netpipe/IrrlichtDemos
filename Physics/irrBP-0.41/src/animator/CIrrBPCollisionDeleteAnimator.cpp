#include "animator/CIrrBPCollisionDeleteAnimator.h"
#include "CIrrBPCollisionObject.h"
#include "CIrrBPWorld.h"


CIrrBPCollisionDeleteAnimator::CIrrBPCollisionDeleteAnimator(CIB_DFLAG deleteFlag, CIrrBPWorld * world)
{
	dFlag = deleteFlag;
	rWorld = world;
	rBody= NULL;
	internalStatus = 0;
}
void CIrrBPCollisionDeleteAnimator::setBody(CIrrBPCollisionObject *body)
{
	rBody = body;
}
void CIrrBPCollisionDeleteAnimator::animate()
{
	if(rBody == NULL || rWorld == NULL || dFlag <=0)
		return;

	switch(dFlag)
	{
		case ON_COLLIDE:
			if(rWorld->isBodyColliding(rBody))
			{
				isEnded = true;
				rBody->setValidStatus(false);
			}
		break;
		case ON_COLLISION_RELEASE:
			if(internalStatus == 0)
			{
				if(rWorld->isBodyColliding(rBody))
					internalStatus =1;
			}
			else if(internalStatus == 1)
			{
				if(!rWorld->isBodyColliding(rBody))
					internalStatus =2;
			}
			else if(internalStatus == 2)
			{
				isEnded = true;
				rBody->setValidStatus(false);
			}

		break;

	}
}