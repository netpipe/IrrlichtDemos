#include "animator/CIrrBPDeleteAnimator.h"

#include "CIrrBPCollisionObject.h"


CIrrBPDeleteAnimator::CIrrBPDeleteAnimator(irr::ITimer* timer, irr::u32 end)
{
	isEnded = false;
	rBody = NULL;
	irrTimer = timer;
	timeMs = end;
}

void CIrrBPDeleteAnimator::setBody(CIrrBPCollisionObject *body)
{
	activationTime = irrTimer->getRealTime();
	
	endTime = timeMs+activationTime;
	rBody = body;
}
void CIrrBPDeleteAnimator::animate()
{
	if(rBody == NULL)
		return;

	if(endTime <= irrTimer->getRealTime())
	{
		isEnded = true;
		rBody->setValidStatus(false);
	}
}