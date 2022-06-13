/*! \file osClock.h
    ----------------------------------------------------------------------------<br>
    Irrlicht implementation by A.Buschhüter<br>
    http://abusoft.g0dsoft.com<br>
    abusoft@g0dsoft.com<br>
    ----------------------------------------------------------------------------<br>
    OpenSteer -- Steering Behaviors for Autonomous Characters<br>
    <br>
    Copyright (c) 2002-2005, Sony Computer Entertainment America<br>
    Original author: Craig Reynolds <craig_reynolds@playstation.sony.com><br>
    <br>
    Permission is hereby granted, free of charge, to any person obtaining a<br>
    copy of this software and associated documentation files (the "Software"),<br>
    to deal in the Software without restriction, including without limitation<br>
    the rights to use, copy, modify, merge, publish, distribute, sublicense,<br>
    and/or sell copies of the Software, and to permit persons to whom the<br>
    Software is furnished to do so, subject to the following conditions:<br>
    <br>
    The above copyright notice and this permission notice shall be included in<br>
    all copies or substantial portions of the Software.<br>
    <br>
    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR<br>
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,<br>
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL<br>
    THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER<br>
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING<br>
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER<br>
    DEALINGS IN THE SOFTWARE.<br>
    ----------------------------------------------------------------------------<br>
*/
#ifdef OPENSTEER
#ifndef OPENSTEER_CLOCK_H
#define OPENSTEER_CLOCK_H

#include "osUtilities.h"
#include <irrlicht.h>
//#include <windows.h>

//! namespace OpenSteer
namespace OpenSteer{

  class Clock{
    private:
      irr::ITimer* irrTimer;
      // main clock modes: variable or fixed frame rate, real-time or animation
      // mode, running or paused.
      // run as fast as possible, simulation time is based on real time
      bool variableFrameRateMode;
      // fixed frame rate (ignored when in variable frame rate mode) in
      // real-time mode this is a "target", in animation mode it is absolute
      int fixedFrameRate;
      // used for offline, non-real-time applications
      bool animationMode;
      // is simulation running or paused?
      bool paused;

      // clock keeps track of "smoothed" running average of recent frame rates.
      // When a fixed frame rate is used, a running average of "CPU load" is
      // kept (aka "non-wait time", the percentage of each frame time (time
      // step) that the CPU is busy).
      float smoothedFPS;
      float smoothedUsage;
      void updateSmoothedRegisters(void){
        const float rate = getSmoothingRate();
        if(elapsedRealTime > 0)
            blendIntoAccumulator(rate, 1 / elapsedRealTime, smoothedFPS);
        if(! getVariableFrameRateMode())
            blendIntoAccumulator(rate, getUsage(), smoothedUsage);
      }
      // clock state member variables and public accessors for them
      // real "wall clock" time since launch
      float totalRealTime;
      // total time simulation has run
      float totalSimulationTime;
      // total time spent paused
      float totalPausedTime;
      // sum of (non-realtime driven) advances to simulation time
      float totalAdvanceTime;
      // interval since last simulation time
      float elapsedSimulationTime;
      // interval since last clock update time
      float elapsedRealTime;
      // interval since last clock update, exclusive of time spent waiting for frame boundary when targetFPS>0
      float elapsedNonWaitRealTime;

      //! "manually" advance clock by this amount on next update
      float newAdvanceTime;
      //! "Calendar time" when this clock was first updated from QueryPerformanceCounter on Windows
      long basePerformanceCounter;

    public:
      Clock();
      //! update this clock, called exactly once per simulation step ("frame")
      void update(void);
      //! returns the number of seconds of real time (represented as a float) since the clock was first updated.
      float realTimeSinceFirstClockUpdate(void);
      //! force simulation time ahead, ignoring passage of real time.
      float advanceSimulationTimeOneFrame(void);
      //! force simulation time ahead, ignoring passage of real time.
      void advanceSimulationTime(const float seconds);
      //! "wait" until next frame time
      void frameRateSync(void);

      //! get fixed frame rate
      int getFixedFrameRate(void){ return fixedFrameRate; }
      //! set fixed frame rate
      int setFixedFrameRate(int ffr){ return fixedFrameRate = ffr; }
      //! get animation mode
      bool getAnimationMode(void){ return animationMode; }
      //! set animation mode
      bool setAnimationMode(bool am){ return animationMode = am; }
      //! get animation mode
      bool getVariableFrameRateMode(void){ return variableFrameRateMode; }
      //! set animation mode
      bool setVariableFrameRateMode(bool vfrm){
        return variableFrameRateMode = vfrm;
      }
      //! toggle paused state
      bool togglePausedState(void){ return (paused = !paused); };
      //! get paused state
      bool getPausedState(void){ return paused; };
      //! set paused state
      bool setPausedState(bool newPS){ return paused = newPS; };

      //! set an Irrlicht timer for use (reccomended)
      void setTimer(irr::ITimer* timer);

      //! get smoothed FPS
      float getSmoothedFPS(void) const { return smoothedFPS; }
      //! get smoothed usage
      float getSmoothedUsage(void) const { return smoothedUsage; }
      //! get smoothing rate
      float getSmoothingRate(void) const{
        if(smoothedFPS == 0) return 1; else return elapsedRealTime * 1.5f;
      }
      //! percental system usage
      float getUsage(void){
        // run time per frame over target frame time (as a percentage)
        return ((100 * elapsedNonWaitRealTime) / (1.0f / fixedFrameRate));
      }

      //! get total real time
      float getTotalRealTime(void){ return totalRealTime; }
      //! get total simulation time
      float getTotalSimulationTime(void){ return totalSimulationTime; }
      //! get total paused time
      float getTotalPausedTime(void){ return totalPausedTime; }
      //! get total advance time
      float getTotalAdvanceTime(void){ return totalAdvanceTime; }
      //! get elapsed simulation time
      float getElapsedSimulationTime(void){ return elapsedSimulationTime; }
      //! get elapsed real time
      float getElapsedRealTime(void){ return elapsedRealTime; }
      //! get elapsed non wait real time
      float getElapsedNonWaitRealTime(void){ return elapsedNonWaitRealTime; }

  };

} // namespace OpenSteer

#endif // OPENSTEER_CLOCK_H
#endif
