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
#include "../../../../config.h"
#ifdef OPENSTEER
#include "osClock.h"
//#include <windows.h>

// Constructor
OpenSteer::Clock::Clock(void){
  irrTimer = 0;
    // default is "real time, variable frame rate" and not paused
    setFixedFrameRate (0);
    setPausedState (false);
    setAnimationMode (false);
    setVariableFrameRateMode (true);

    // real "wall clock" time since launch
    totalRealTime = 0;

    // time simulation has run
    totalSimulationTime = 0;

    // time spent paused
    totalPausedTime = 0;

    // sum of (non-realtime driven) advances to simulation time
    totalAdvanceTime = 0;

    // interval since last simulation time
    elapsedSimulationTime = 0;

    // interval since last clock update time
    elapsedRealTime = 0;

    // interval since last clock update,
    // exclusive of time spent waiting for frame boundary when targetFPS>0
    elapsedNonWaitRealTime = 0;

    // "manually" advance clock by this amount on next update
    newAdvanceTime = 0;

    // "Calendar time" when this clock was first updated
    basePerformanceCounter = 0;  // from QueryPerformanceCounter on Windows

    // clock keeps track of "smoothed" running average of recent frame rates.
    // When a fixed frame rate is used, a running average of "CPU load" is
    // kept (aka "non-wait time", the percentage of each frame time (time
    // step) that the CPU is busy).
    smoothedFPS = 0;
    smoothedUsage = 0;
}

// update this clock, called once per simulation step ("frame") to:
void OpenSteer::Clock::setTimer(irr::ITimer* timer){
  irrTimer = timer;
}

// update this clock, called once per simulation step ("frame") to:
void OpenSteer::Clock::update(void){
    // keep track of average frame rate and average usage percentage
    updateSmoothedRegisters ();

    // wait for next frame time (when targetFPS>0)
    // XXX should this be at the end of the update function?
    frameRateSync ();

    // save previous real time to measure elapsed time
    const float previousRealTime = totalRealTime;

    // real "wall clock" time since this application was launched
    if(!irrTimer)
      totalRealTime = realTimeSinceFirstClockUpdate ();
    else
      totalRealTime = irrTimer->getTime() * 0.001;
    // time since last clock update
    elapsedRealTime = totalRealTime - previousRealTime;

    // accumulate paused time
    if (paused) totalPausedTime += elapsedRealTime;

    // save previous simulation time to measure elapsed time
    const float previousSimulationTime = totalSimulationTime;

    // update total simulation time
    if(getAnimationMode()){
        // for "animation mode" use fixed frame time, ignore real time
        const float frameDuration = 1.0f / getFixedFrameRate ();
        totalSimulationTime += paused ? newAdvanceTime : frameDuration;
        if (!paused) newAdvanceTime += frameDuration - elapsedRealTime;
    }else{
        // new simulation time is total run time minus time spent paused
        totalSimulationTime = (totalRealTime
                               + totalAdvanceTime
                               - totalPausedTime);
    }

    // update total "manual advance" time
    totalAdvanceTime += newAdvanceTime;

    // how much time has elapsed since the last simulation step?
    elapsedSimulationTime = (paused ?
                             newAdvanceTime :
                             (totalSimulationTime - previousSimulationTime));

    // reset advance amount
    newAdvanceTime = 0;
}

// "wait" until next frame time (actually spin around this tight loop)
void OpenSteer::Clock::frameRateSync(void){
    // when in real time fixed frame rate mode
    // (not animation mode and not variable frame rate mode)
    if((! getAnimationMode ()) && (! getVariableFrameRateMode ())){
        // find next (real time) frame start time
        const float targetStepSize = 1.0f / getFixedFrameRate ();
        const float now = realTimeSinceFirstClockUpdate ();
        const int lastFrameCount = (int) (now / targetStepSize);
        const float nextFrameTime = (lastFrameCount + 1) * targetStepSize;

        // record usage ("busy time", "non-wait time") for OpenSteerDemo app
        elapsedNonWaitRealTime = now - totalRealTime;

        // wait until next frame time
        do {} while (realTimeSinceFirstClockUpdate () < nextFrameTime);
    }
}

// force simulation time ahead, ignoring passage of real time.
float OpenSteer::Clock::advanceSimulationTimeOneFrame(void){
    // decide on what frame time is (use fixed rate, average for variable rate)
    const float fps = (getVariableFrameRateMode () ?
                       getSmoothedFPS () :
                       getFixedFrameRate ());
    const float frameTime = 1.0 / fps;
    // bump advance time
    advanceSimulationTime (frameTime);

    // return the time value used (for OpenSteerDemo)
    return frameTime;
}

void OpenSteer::Clock::advanceSimulationTime(const float seconds){
    if(seconds < 0){
        /// @todo - throw? how to handle error conditions? Not by crashing an app!
        std::cerr << "negative arg to advanceSimulationTime - results will not be valid";
    }else
        newAdvanceTime += seconds;
}


namespace {

// ----------------------------------------------------------------------------<br>
// Returns the number of seconds of real time (represented as a float) since
// the clock was first updated.
//
// XXX Need to revisit conditionalization on operating system.
    float clockErrorExit(void){
        /// @todo - throw? how to handle error conditions? Not by crashing an app!
        std::cerr << "Problem reading system clock - results will not be valid";
        return 0.0f;
    }

} // anonymous namespace

float OpenSteer::Clock::realTimeSinceFirstClockUpdate(void){
    // get time from Windows
 //   long counter, frequency;
//    bool clockOK = (QueryPerformanceCounter ((unsigned long *)&counter)  &&
  //                  QueryPerformanceFrequency ((LARGE_INTEGER *)&frequency));
  //  if (!clockOK) return clockErrorExit ();

    // ensure the base counter is recorded once after launch
   // if (basePerformanceCounter == 0) basePerformanceCounter = counter;

    // real "wall clock" time since launch
  //  const LONGLONG counterDifference = counter - basePerformanceCounter;
  //  return ((float) counterDifference) / ((float)frequency);
}


// ----------------------------------------------------------------------------<br>
#endif
