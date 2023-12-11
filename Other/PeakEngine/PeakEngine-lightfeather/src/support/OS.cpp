/*
Copyright (c) 2009, Mathias Gottschlag

Permission to use, copy, modify, and/or distribute this software for any
purpose with or without fee is hereby granted, provided that the above
copyright notice and this permission notice appear in all copies.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

#include "peakengine/support/OS.hpp"

#if defined(_MSC_VER) || defined(_WINDOWS_) || defined(_WIN32)
#include <time.h>
#include <windows.h>
#define usleep(x) Sleep((x)/1000)
#else
#include <unistd.h>
#include <sys/time.h>
#endif

namespace peak
{
	OS &OS::get()
	{
		static OS os;
		return os;
	}
	OS::~OS()
	{
	}

	unsigned int OS::getTime()
	{
		return getSystemTime() - starttime;
	}
	uint64_t OS::getSystemTime()
	{
		#if defined(_MSC_VER) || defined(_WINDOWS_) || defined(_WIN32)
		LARGE_INTEGER timerfreq;
		QueryPerformanceFrequency(&timerfreq);
		LARGE_INTEGER counter;
		QueryPerformanceCounter(&counter);
		return counter.QuadPart * 1000000 / timerfreq.QuadPart;
		#else
		struct timeval tv;
		gettimeofday(&tv, 0);
		return (uint64_t)tv.tv_sec * 1000000 + tv.tv_usec;
		#endif
	}
	unsigned int OS::getMemory()
	{
		return 0;
	}
	unsigned int OS::getFreeMemory()
	{
		return 0;
	}
	unsigned int OS::getLogicalProcessors()
	{
		return 0;
	}
	unsigned int OS::getPhysicalProcessors()
	{
		return 0;
	}

	void OS::sleep(unsigned int usecs)
	{
		usleep(usecs);
	}

	OS::OS()
	{
		starttime = getSystemTime();
	}
}
