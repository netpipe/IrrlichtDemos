/*

Copyright (c) 2010, Jonathan Wayne Parrott

Please see the license.txt file included with this source
distribution for more information.

*/

#ifndef __PHOENIXTIME_H__
#define __PHOENIXTIME_H__

#include "config.h"
#include "WindowManager.h"

namespace phoenix
{

    //! Timer class
    /*!
        A class that provides a simple interface to a real time clock. It is useful for tracking time
		between frames, profiling, frames per second calcuation and many other things.
    */
    class Timer
    {

    public:

		//! Constructor.
        Timer()	
			: startTicks(0), pausedTicks(0), paused(false), started(false)
		{
		}

        //! Start timer.
        inline void start()
		{
			//Start the timer
			started = true;
			//Unpause the timer
			paused = false;
			//Get the current clock time
			startTicks = WindowManager::Instance()->getTime();
		}

        //! Stops timing.
        void stop()
		{
			//Stop the timer
			started = false;
			//Unpause the timer
			paused = false;
		}

		//! Calls stop() then start()
		inline void reset() { stop(); start(); }

        //! Pause timer in place.
        void pause()
		{
			//If the timer is running and isn't already paused
			if( ( started == true ) && ( paused == false ) )
			{
				//Pause the timer
				paused = true;
				//Calculate the paused ticks
				pausedTicks = WindowManager::Instance()->getTime() - startTicks;
			}
		}

        //! Resume timer from from pause.
        void resume()
		{
			//If the timer is paused
			if( paused == true )
			{
				//Unpause the timer
				paused = false;
				//Reset the starting ticks
				startTicks = WindowManager::Instance()->getTime() - pausedTicks;
				//Reset the paused ticks
				pausedTicks = 0;
			}
		}

        //! Returns how many seconds the timer has gone through.
        const double getTime() const
		{
		    //If the timer is running
			if( started == true )
			{
				//If the timer is paused
				if( paused == true )
				{
					//Return the number of ticks when the the timer was paused
					return pausedTicks;
				}
				else
				{
					//Return the current time minus the start time
					return WindowManager::Instance()->getTime() - startTicks;
				}
			}
			//If the timer isn't running
			return 0;
		}

		//! Adjusts the timer the given about of seconds.
		void adjust( const double _d )
		{
			if( started ){
				if( paused ){
					pausedTicks += _d;
				}else{
					startTicks += _d;
				}
			}
		}

        //! Returns true if the timer has started counting.
        inline bool isStarted() const { return started; }

        //! Returns true is the timer is paused.
        inline bool isPaused() const { return paused; }

	private:

        double startTicks, pausedTicks;
        bool paused, started;


    };

} //namespace phoenix

#endif //__PHOENIXTIME_H__
