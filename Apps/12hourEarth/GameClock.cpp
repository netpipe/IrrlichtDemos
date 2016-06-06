
#include "GameClock.h"

GameClock::GameClock() : ClockOffset(0), ClockSecs(0), ClockMins(0), ClockHours(0), ClockDays(0), DayNight(0),
                        TCycleCounter(0), NewHours(0), OldHours(0), ElapsedHours(0)
{}

GameClock::~GameClock(){}

void GameClock::update()
{
    MyTimer::update();

    // Adjust for twelve hour timecycle
    NewHours = MyTimer::TotalHours;
    ElapsedHours = (NewHours - OldHours);
    OldHours = NewHours;

    TCycleCounter += ElapsedHours;
    if(TCycleCounter >= 24)
    {
        TCycleCounter -= 24;
        ++ClockDays;
    }

    if(TCycleCounter < 11 || TCycleCounter > 22)
    { DayNight = "AM"; }
    else
    { DayNight = "PM"; }

    ClockSecs = (MyTimer::TotalSeconds%60);
    ClockMins = (MyTimer::TotalMinutes%60);
    ClockHours = ((TCycleCounter%12)+1);
}

irr::core::stringw GameClock::getClockDisplay()
{
    irr::core::stringw str("  ");
    str.append(irr::core::stringw(ClockHours));
    str.append(":");
    if(ClockMins < 10)
    str.append("0");
    str.append(irr::core::stringw(ClockMins));
    str.append(":");
    if(ClockSecs < 10)
    str.append("0");
    str.append(irr::core::stringw(ClockSecs));
    str.append(" ");
    str.append(DayNight);

    return str;
}

void GameClock::setClock(irr::u32 hours, irr::u32 minutes, irr::u32 seconds, irr::core::stringw timecycle)
{
    //don't accept invalid times
    if(!hours || minutes >= 60 || seconds >= 60)
    {
        // report error invalid clock offset
        // clock currently starts no earlier then 1:00:00AM
    }
    else
    {
        // Convert everything to seconds and advance the timer
        ClockOffset = ((hours*60)*60) - 3600 + (minutes*60) + seconds;
        MyTimer::TotalSeconds += (ClockOffset);
        if(timecycle == "PM")
        {
            MyTimer::TotalSeconds += 43200; //aka 12 hours in seconds
        }
    }
}

void GameClock::setClockSpeed(irr::f32 factor)
{
    // max factor can not be more then float values without changing irrlicht
    MyTimer::timer->setSpeed(factor);
}

irr::core::stringw GameClock::getTimeCycle()
{
    return DayNight;
}

irr::u32 GameClock::getClockSeconds()
{
    return ClockSecs;
}

irr::u32 GameClock::getClockMinutes()
{
    return ClockMins;
}

irr::u32 GameClock::getClockHours()
{
    return ClockHours;
}

irr::u32 GameClock::getClockDays()
{
    return ClockDays;
} 
