// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#include "timer.h"

using namespace irr;

Timer::Timer(irr::ITimer * irrlichtTimer_, bool startRunning_)
    : mIrrlichtTimer(irrlichtTimer_)
    , mTime(0)
    , mTimeRest(0.f)
    , mLastRealTime(0)
    , mLastStartTime(0)
    , mSpeed(1.f)
    , mLastTick(0)
    , mIsRunning(startRunning_)
{
}

Timer::~Timer()
{
}

u32 Timer::GetTime() const
{
    return mTime;
}

void Timer::SetTime(u32 time_)
{
    mTime = time_;
    mTimeRest = 0.f;
}

u32 Timer::GetLastTickInMs() const
{
    return mLastTick;
}

f32 Timer::GetLastTickInSeconds() const
{
    return static_cast<f32>(mLastTick) / 1000.f;
}


void Timer::Stop()
{
    mIsRunning = 0;
    mLastTick = 0;
}

void Timer::Start()
{
    mIsRunning = 1;
    if ( mIrrlichtTimer )
    {
        mLastRealTime = mIrrlichtTimer->getRealTime();
    }
    mLastStartTime = mTime;
}

bool Timer::IsStopped() const
{
    return mIsRunning ? false : true;
}

u32 Timer::GetLastStartTime() const
{
    return mLastStartTime;
}

void Timer::SetSpeed(f32 speed_)
{
    mSpeed = speed_;
}

f32 Timer::GetSpeed() const
{
    return mSpeed;
}

void Timer::Tick()
{
    if ( mLastRealTime == 0 )
    {
        if ( mIrrlichtTimer )
        {
            mLastRealTime = mIrrlichtTimer->getRealTime();
        }
    }

    if ( mIsRunning )
    {
        u32 timeNow = 0;
        if ( mIrrlichtTimer )
        {
            timeNow = mIrrlichtTimer->getRealTime();
            f32 timeAdd = static_cast<float>(timeNow-mLastRealTime) * mSpeed + mTimeRest;
            mLastTick = static_cast<int>(timeAdd);
            mTime += mLastTick;
            mTimeRest = timeAdd - mLastTick;
        }
    }
    if ( mIrrlichtTimer )
    {
        mLastRealTime = mIrrlichtTimer->getRealTime();
    }
}

