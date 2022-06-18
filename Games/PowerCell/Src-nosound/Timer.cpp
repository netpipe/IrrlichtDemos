#include "Timer.hpp"

LocalTimer *LocalTimer::m_localtimer = NULL;
LocalTimer LocalTimer::m_default_localtimer_buffer;

void LocalTimer::SetDefaultValues()
{
  m_prevTime = 0;
  m_time = 0;
  m_deltaTime = 0;
  t1 = 0;
  t2 = 0;
  m_timeFactor = 0;
  m_step = 0;
  m_accumulator = 0;
  m_delta = 0;
  m_tick = 0;
  m_tickFast = 0;
  m_defaultTickRate = 0.02f;
  i = 0;
  milliseconds = 0;
  seconds = 0;
  minutes = 0;
}

LocalTimer *LocalTimer::Instance()
{
  if(!m_localtimer)
    m_localtimer = &m_default_localtimer_buffer;

  return m_localtimer;
}

void LocalTimer::Reset()
{
  if(!irrDevice) return;
  m_time = irrDevice->getTimer()->getRealTime();
}

void LocalTimer::Update()
{
  if(!irrDevice) return;

  t2 = irrDevice->getTimer()->getTime();
  m_deltaTime = (t2 - t1) / 1000.0f;
  t1 = irrDevice->getTimer()->getTime();

  m_time += m_deltaTime;
  m_timeFactor = m_deltaTime;
}

f32 LocalTimer::GetTimeFactor()
{
  return m_timeFactor;
}

f32 LocalTimer::GetDeltaTime(void)
{
  return m_deltaTime;
}

u32 LocalTimer::GetTime(void)
{
  if(m_prevTime > 0)
  {
    i = (m_time - m_prevTime);
    seconds += i;
//    Output::Instance()->w("seconds=%i i=%i\n", seconds, i);
  }

  //if(i<1000) m_defaultTickRate -= 0.001f;
  //else if(i>1000) m_defaultTickRate += 0.001f;

  m_prevTime = m_time;
  return seconds;
}

void LocalTimer::SetTime(u32 amount)
{
  seconds += amount;
}

f32 LocalTimer::GetTick(void)
{
  return m_tick * 1000.0f;
}

bool LocalTimer::Tick()
{
  if(m_tick >= 1.0f)
  {
    m_tick = 0;
    return true; // Time to update.
  }

  m_tick += m_defaultTickRate;
  return false;
}

bool LocalTimer::TickFast()
{
  if(m_tickFast >= 0.2f)
  {
    m_tickFast = 0;
    return true; // Time to update.
  }

  m_tickFast += 0.04f;
  return false;
}

