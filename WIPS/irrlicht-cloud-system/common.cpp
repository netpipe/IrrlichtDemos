#include <time.h>
#include <stdlib.h>


/* time sleeping */
#ifdef WIN32
	#include <windows.h>
#elif _POSIX_C_SOURCE >= 199309L
	#include <time.h>   // for nanosleep
	#include <sys/time.h>
#else
	#include <unistd.h> // for usleep
#endif

void sleep_ms(unsigned int milliseconds) // cross-platform sleep function
{
	#ifdef WIN32
		Sleep(milliseconds);
	#elif _POSIX_C_SOURCE >= 199309L
		static struct timespec ts;
		ts.tv_sec = 0;
		ts.tv_nsec = milliseconds * 1000000;
		nanosleep(&ts, NULL);
	#else
		usleep(milliseconds * 1000);
	#endif
}

void sleep_us(unsigned int microseconds) // cross-platform sleep function
{
	#ifdef WIN32
		Sleep(microseconds*1000);
	#elif _POSIX_C_SOURCE >= 199309L
		static struct timespec ts;
		ts.tv_sec = 0;
		ts.tv_nsec = microseconds * 1000;
		nanosleep(&ts, NULL);
	#else
		usleep(microseconds);
	#endif
}

unsigned int time_ms()
{
	#ifdef WIN32
		return GetTickCount();
	#else
		static struct timeval tv;
		gettimeofday(&tv, 0);
		return tv.tv_sec * 1000 + tv.tv_usec/1000;
	#endif
}