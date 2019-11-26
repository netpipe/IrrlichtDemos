#ifndef _TIMER_HPP_
#define _TIMER_HPP_

#include "Globals.hpp"

class LocalTimer
{
  private:

    f32
      m_prevTime,
      m_time,
      //m_deltaTime,
      m_timeFactor,
      m_step,
      m_accumulator,
      m_delta,
      m_tick,
      m_tickFast,
      m_defaultTickRate;

    u32
      i,
      milliseconds,
      seconds,
      minutes;

		double
			t1,
			t2,
			m_deltaTime;

    IrrlichtDevice *irrDevice;

  public:

    static LocalTimer *Instance();

    void SetDefaultValues(void);
    void Reset();
    void Update();
    f32 GetTimeFactor();
    f32 GetDeltaTime();
    u32 GetTime(void);
    void SetTime(u32 amount);
    f32 GetTick(void);
    bool Tick();
    bool TickFast();

    __inline void setDevice(IrrlichtDevice *device)
    {
      irrDevice = device;
    }

  protected:

    static LocalTimer *m_localtimer;
    static LocalTimer m_default_localtimer_buffer;
};

#endif
