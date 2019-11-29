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

#include "core/Timer.h"
#include "core/Script.h"

namespace peak
{
	Timer::Timer()
	{
		running = false;
		time = 0;
	}
	Timer::~Timer()
	{
		for (unsigned int i = 0; i < callbacks.size(); i++)
		{
			if (callbacks[i].timer == this)
			{
				callbacks.erase(callbacks.begin() + i);
				i--;
			}
		}
	}
	
	void Timer::start(void)
	{
		running = true;
		gettimeofday(&starttime, 0);
	}
	void Timer::stop(void)
	{
		// Get current time and store difference
		timeval currtime;
		gettimeofday(&currtime, 0);
		float difference = currtime.tv_usec - starttime.tv_usec;
		difference /= 1000000;
		difference += currtime.tv_sec - starttime.tv_sec;
		time += difference;
		running = false;
	}
	void Timer::reset(void)
	{
		time = 0;
		gettimeofday(&starttime, 0);
	}
	
	void Timer::setTime(float time)
	{
		this->time = time;
		gettimeofday(&starttime, 0);
	}
	float Timer::getTime(void)
	{
		if (running)
		{
			// Get current time and add difference to returned time
			timeval currtime;
			gettimeofday(&currtime, 0);
			float difference = currtime.tv_usec - starttime.tv_usec;
			difference /= 1000000;
			difference += currtime.tv_sec - starttime.tv_sec;
			//printf("Getting time: %f + %f.\n", time, difference);
			return time + difference;
		}
		else
		{
			return time;
		}
	}
	
	void Timer::addCallback(float time, Script *script, std::string function)
	{
		TimerCallback cb;
		cb.timer = this;
		cb.script = script;
		cb.function = function;
		cb.time = time;
		callbacks.push_back(cb);
	}
	
	void Timer::doAllWork(void)
	{
		for (unsigned int i = 0; i < callbacks.size(); i++)
		{
			if (callbacks[i].timer->getTime() >= callbacks[i].time)
			{
				callbacks[i].script->callFunction(callbacks[i].function, UserClass("peak::TimerCallback", &callbacks[i]));
				callbacks.erase(callbacks.begin() + i);
				i--;
			}
		}
	}
	
	std::vector<TimerCallback> Timer::callbacks;
}

