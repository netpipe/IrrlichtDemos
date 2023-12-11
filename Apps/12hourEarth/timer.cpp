
#include "timer.h"

MyTimer::MyTimer() : TotalSeconds(0), TotalMinutes(0), TotalHours(0), TotalDays(0), Delta(0),
                     NewTime(0), OldTime(0), Elapsed(0), OldRealTime(0), NewRealTime(0), SecondCounter(0)
{
}

MyTimer::~MyTimer()
{
}

void MyTimer::init(irr::ITimer* t)
{
   timer = t;
   OldTime = timer->getTime();
   OldRealTime = timer->getRealTime();
}

void MyTimer::update()
{
    NewTime = timer->getTime();
    Elapsed = (NewTime - OldTime);
    OldTime = NewTime;

    // this seperates delta time from the timer incase we changed the speed
    NewRealTime = timer->getRealTime();
    if(NewRealTime - OldRealTime)
    Delta = (NewRealTime - OldRealTime)/1000;
    OldRealTime = NewRealTime;

    SecondCounter += Elapsed;
    if(SecondCounter >= 1000)
    {
        SecondCounter -= 1000;
        ++TotalSeconds;
    }

    if(TotalSeconds != 0)
    {
        TotalMinutes = TotalSeconds/60;
        TotalDays = ((TotalSeconds/60)/60)/24;
        TotalHours = (TotalSeconds/60)/60;
    }
}

irr::u32 MyTimer::getSecondsElapsed()
{
    return TotalSeconds;
}

irr::f32 MyTimer::getMinutesElapsed()
{
    return TotalMinutes;
}

irr::f32 MyTimer::getHoursElapsed()
{
    return TotalHours;
}

irr::f32 MyTimer::getDaysElapsed()
{
    return TotalDays;
}

irr::f32 MyTimer::getDeltaTime()
{
    return Delta;
}
