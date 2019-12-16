/*
Copyright (C) 2008  Mathias Gottschlag

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in the
Software without restriction, including without limitation the rights to use,
copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the
Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef _TIMER_H_
#define _TIMER_H_

#include "GameEngine.h"

#include <vector>

//tolua_begin
namespace peak
{
	class Timer;
	class Script;
	
	/**
	 * \brief Holds the data of a user defined timer callback
	 */
	struct TimerCallback
	{
		Timer *timer;
		float time;
		Script *script;
		std::string function;
	};
	
	/**
	 * \brief Class for measuring time
	 *
	 * When created, the timer is stopped and set to 0.
	 */
	class Timer
	{
		public:
			Timer();
			~Timer();
			
			/**
			 * \brief Starts the timer
			 */
			void start(void);
			/**
			 * \brief Stops the timer
			 */
			void stop(void);
			/**
			 * \brief Sets the time to 0.
			 *
			 * Does not stop the timer.
			 */
			void reset(void);
			
			/**
			 * \brief Sets the time to a new value
			 */
			void setTime(float time);
			/**
			 * \brief Returns the current time
			 */
			float getTime(void);
			
			/**
			 * \brief Adds a callback which is fired when the timer reaches a
			 * specified time.
			 */
			void addCallback(float time, Script *script, std::string function);
			
			/**
			 * \brief Checks all callbacks and triggers them if needed.
			 */
			static void doAllWork(void);
		private:
			//tolua_end
			float time;
			bool running;
			
			double starttime;
			
			static std::vector<TimerCallback> callbacks;
			//tolua_begin
	};
};
//tolua_end

#endif

