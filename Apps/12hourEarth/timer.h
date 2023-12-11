#ifndef TIMER_H_INCLUDED_
#define TIMER_H_INCLUDED_

#include "ITimer.h"

class MyTimer
{
public:

    MyTimer();
    ~MyTimer();

    void init(irr::ITimer* t);
    void update();

    irr::u32 getSecondsElapsed();
    irr::f32 getMinutesElapsed();
    irr::f32 getHoursElapsed();
    irr::f32 getDaysElapsed();
    irr::f32 getDeltaTime();

protected:

    irr::u32 TotalSeconds;
    irr::u32 TotalMinutes;
    irr::u32 TotalHours;

    irr::ITimer* timer;

private:

    irr::u32 TotalDays;

    irr::f32 NewRealTime;
    irr::f32 OldRealTime;
    irr::f32 Delta;

    irr::f32 NewTime;
    irr::f32 OldTime;
    irr::f32 Elapsed;

    irr::s32 SecondCounter;

};

#endif 
