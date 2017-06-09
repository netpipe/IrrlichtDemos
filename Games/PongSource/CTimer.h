#pragma once

#include "CIrrObject.h"

class CTimer : public CIrrObject
{
public:
	CTimer();
	~CTimer();

	f32 getDelta();
	bool onTimeOut();
	u32 getTimeOut();
	u32 getTime();

	void setTimeOut(s32 time);
	void updateDelta();

private:
	u32 lastDeltaTime;
	u32 lastTime;
	u32 timeOut;
};
