#include "animator/CIrrBPTimeCallBackAnimator.h"

CIrrBPTimeCallbackAnimator::CIrrBPTimeCallbackAnimator(irr::ITimer* timer, irr::s32 ms,EndTimeCallback * callback)
{
	isEnded = false;
	rBody = NULL;
	irrTimer = timer;
	timeMs = ms;
	cbk=callback;
}
void CIrrBPTimeCallbackAnimator::setBody(CIrrBPCollisionObject *body)
{
	activationTime = irrTimer->getRealTime();
	
	endTime = timeMs+activationTime;
	rBody = body;
}
void CIrrBPTimeCallbackAnimator::animate()
{
	
	if(rBody == NULL)
		return;

	if(endTime <= irrTimer->getRealTime())
	{
		isEnded = true;
		cbk->addSingleResult();
	}
}