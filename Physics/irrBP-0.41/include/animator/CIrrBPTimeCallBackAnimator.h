#ifndef CIrrBP_TCBL_ANIM_H_
#define CIrrBP_TCBL_ANIM_H_
#include "animator/CIrrBPAnimator.h"

class CIrrBPCollisionObject;
class CIrrBPWorld;

class EndTimeCallback
{
public:
	EndTimeCallback()	{	}
	virtual ~EndTimeCallback()	{	}
	virtual void addSingleResult()=0;
};

class CIrrBPTimeCallbackAnimator  : public CIrrBPAnimator
{
public:
	CIrrBPTimeCallbackAnimator(irr::ITimer* timer, irr::s32 ms, EndTimeCallback * callback);
	void setBody(CIrrBPCollisionObject* body) ;
	void animate();
	
	void drop() {delete this;}
private:
	irr::ITimer* irrTimer;
	irr::u32 activationTime;
	irr::u32 endTime;
	irr::u32 timeMs;

	CIrrBPWorld * rWorld;

	EndTimeCallback * cbk;
};
#endif