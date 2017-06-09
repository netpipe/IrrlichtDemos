#include "CTimer.h"

#include <iostream>

using namespace std;

CTimer::CTimer()
{
	lastTime = 0;
}

CTimer::~CTimer()
{
}

f32 CTimer::getDelta()
{
	u32 now = device->getTimer()->getTime();
	f32 delta = (f32)(now - lastDeltaTime);
	lastDeltaTime = now;

	return delta;
}

u32 CTimer::getTime()
{
	return device->getTimer()->getTime();
}

u32 CTimer::getTimeOut()
{
	return timeOut;
}

bool CTimer::onTimeOut()
{
	if (device->getTimer()->getTime() >= lastTime + timeOut)
	{
		lastTime = device->getTimer()->getTime();
		return true;
	}

	return false;
}

void CTimer::setTimeOut(s32 time)
{
	timeOut = time;
}

void CTimer::updateDelta()
{
	lastDeltaTime = device->getTimer()->getTime();
}