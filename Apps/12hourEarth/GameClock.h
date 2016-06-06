#ifndef GAME_TIMER_H_INCLUDED_
#define GAME_TIMER_H_INCLUDED_

#include "timer.h"
#include "irrString.h"

//MyTimer timer;

class GameClock : public MyTimer
{
public:

   GameClock();
   ~GameClock();

    void update();

    void setClockSpeed(irr::f32 factor);
    void setClock(irr::u32 hours, irr::u32 minutes, irr::u32 seconds, irr::core::stringw timecycle);

    irr::core::stringw getClockDisplay();

    irr::core::stringw getTimeCycle();
    irr::u32 getClockSeconds();
    irr::u32 getClockMinutes();
    irr::u32 getClockHours();
    irr::u32 getClockDays();

private:

    irr::u32 ClockOffset;
    irr::u32 ClockSecs;
    irr::u32 ClockMins;
    irr::u32 ClockHours;
    irr::u32 ClockDays;

    irr::core::stringw DayNight;
    irr::u32 TCycleCounter;

    irr::u32 NewHours;
    irr::u32 OldHours;
    irr::u32 ElapsedHours;
};

#endif
