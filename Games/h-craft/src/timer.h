// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#ifndef TIMER_H
#define TIMER_H

#include <irrlicht.h>

class Timer
{
public:
	Timer(irr::ITimer * irrlichtTimer_, bool startRunning_=false);	// startRunning_ decides if timer is running or stopped after creation
	~Timer();

    // in milliseconds
	irr::u32 GetTime() const;
	void SetTime(irr::u32 time_);

	irr::u32 GetLastTickInMs() const;
	irr::f32 GetLastTickInSeconds() const;

    // unlike irr::ITimer there's no reference counting for start/stop!
	void Stop();
	void Start();
	bool IsStopped() const;

	irr::u32 GetLastStartTime() const;

	void SetSpeed(irr::f32 speed_ = 1.0f);
	irr::f32 GetSpeed() const;

	// Must be called regularly.
	void Tick();

private:
    irr::ITimer  *  mIrrlichtTimer;

    irr::u32    mTime;
    irr::f32    mTimeRest;
    irr::u32    mLastRealTime;
    irr::u32    mLastStartTime;
    irr::f32    mSpeed;
    irr::u32    mLastTick;
    int         mIsRunning;
};

#endif // TIMER_H
