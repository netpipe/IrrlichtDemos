#ifdef SYNCSYS
#pragma once

#if (defined DO_OPTIMIZE && !defined _DEBUG)
#if (defined _SECURE_SCL && _SECURE_SCL == 1)
#define __STR2__(x) #x
#define __STR1__(x) __STR2__(x)
#define __LOC__ __FILE__ "("__STR1__(__LINE__)") : Warning: "
#pragma message(__LOC__"_SECURE_SCL is already defined and set to 1. Using _SECURE_SCL 0 will most likely case a crash!")
#endif
//this define disable some security checks and give a performanceboost.
//If you dont want do disable compile the lib with DONT_OPTIMIZE in the projectsetting definied.
//If you want to use it in your hole project add _SECURE_SCL=0; to all projects in the projectsettings.
#define _SECURE_SCL 0
#endif

#ifndef DONT_INCLUDE_HEADERS
#ifdef _WIN32
#include <windows.h>
#include <mmsystem.h>
#include <intrin.h>
#else
#include <pthread.h>
#include <sys/time.h>
#include <unistd.h>
#include <cstring>
#include <cassert>
#include <cstdlib>
#include <wchar.h>
#include <netinet/in.h>
#endif
#endif

namespace helpers
{
//!portable version of a critical section/mutex
class MY_CRITICALSECTION;

//!atomic or
template<typename T1, typename T2> inline T1 Atomic_Or(volatile T1& Target, T2 Value);
//!atomic and
template<typename T1, typename T2> inline T1 Atomic_And(volatile T1& Target, T2 Value);
//!atomic dec
template<typename T1, typename T2> inline T1 Atomic_Dec(volatile T1& Target, T2 Value);
//!atomic inc
template<typename T1, typename T2> inline void Atomic_Inc(volatile T1& Target, T2 Value);
//!atomic set
template<typename T1, typename T2> inline void Atomic_Set(volatile T1& Target, T2 Value);
//!atomic add
template<typename T1, typename T2> inline void Atomic_Add(volatile T1& Target, T2 Value);
//!portable version of sleep
inline void PortableSleep(unsigned int _t);
//!portable version to get the time in MS
inline unsigned int PortableGetTime(void);

#ifdef _WIN32

typedef HANDLE MY_THREADHANDLE;
typedef DWORD THREAD_RETURN;

class MY_CRITICALSECTION
{
	mutable CRITICAL_SECTION critcal;

	void operator = (const MY_CRITICALSECTION&) {}
public:
	MY_CRITICALSECTION(const MY_CRITICALSECTION&)	{ InitializeCriticalSection(&critcal); }
	MY_CRITICALSECTION(void)						{ InitializeCriticalSection(&critcal); }
	~MY_CRITICALSECTION(void)						{ DeleteCriticalSection(&critcal); }

	void Lock(void) const		{ EnterCriticalSection(&critcal); }
	void Unlock(void) const		{ LeaveCriticalSection(&critcal); }
};

inline MY_THREADHANDLE PortableThreadCreate(THREAD_RETURN __stdcall function(void*), void* arguments = 0, size_t stacksize = 0)
{
	return CreateThread(NULL, stacksize, function, arguments, 0, 0);
}
inline int  Atomic_Dec(unsigned volatile& v)						{ return InterlockedDecrement(reinterpret_cast<volatile long*>(&v)); }
inline void Atomic_Inc(unsigned volatile& v)						{ InterlockedIncrement(reinterpret_cast<volatile long*>(&v)); }
inline void Atomic_Set(unsigned volatile& v, unsigned value)		{ InterlockedExchange(reinterpret_cast<long volatile*>(&v), value); }
inline void Atomic_Add(unsigned volatile& v, unsigned value)		{ InterlockedExchangeAdd(reinterpret_cast<long volatile*>(&v), value); }

inline int  Atomic_Dec(int volatile& v)								{ return InterlockedDecrement(reinterpret_cast<volatile long*>(&v)); }
inline void Atomic_Inc(int volatile& v)								{ InterlockedIncrement(reinterpret_cast<volatile long*>(&v)); }
inline void Atomic_Set(int volatile& v, int value)					{ InterlockedExchange(reinterpret_cast<volatile long*>(&v), value); }
inline void Atomic_Add(int volatile& v, int value)					{ InterlockedExchangeAdd(reinterpret_cast<volatile long*>(&v), value); }


template<typename T2> inline unsigned char Atomic_Or(unsigned char volatile& v, T2 value)	{ return _InterlockedOr8((volatile char*)&v, value); }
template<typename T2> inline unsigned char Atomic_And(unsigned char volatile& v, T2 value)	{ return _InterlockedAnd8((volatile char*)&v, value); }

inline void PortableSleep(unsigned int _t)			{ Sleep( _t ); }
inline unsigned int PortableGetTime(void)			{ return timeGetTime(); }

inline size_t wcs2mbslen(const wchar_t* wcstring)
{
	size_t neededbytes;
	wcstombs_s(&neededbytes, NULL, 0, wcstring, _TRUNCATE);
	return neededbytes;
}

inline void wcs2mbs_n(char* mbstring, const wchar_t* wcstring, size_t number)
{
	size_t neededbytes;
	wcstombs_s(&neededbytes, mbstring, number, wcstring, _TRUNCATE);
}

inline void mbs2wcs_n(wchar_t* wcstring, const char* mbstring, size_t number)
{
	mbstowcs_s(&number, wcstring, number, mbstring, _TRUNCATE);
}

#else

#define __stdcall
#define SOCKET      int
#define _ASSERT	    assert
typedef pthread_t MY_THREADHANDLE;
typedef void* THREAD_RETURN;


class MY_CRITICALSECTION
{
	mutable pthread_mutex_t critcal;

	void operator = (const MY_CRITICALSECTION&) {}

	void create(void)
	{
		pthread_mutexattr_t attr;

		pthread_mutexattr_init(&attr);
		pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);

		pthread_mutex_init(&critcal, &attr);
	}

public:
	MY_CRITICALSECTION(const MY_CRITICALSECTION&)	{ create(); }
	MY_CRITICALSECTION(void)						{ create(); }

	~MY_CRITICALSECTION(void)	{ pthread_mutex_destroy(&critcal); }
	void Lock(void) const		{ pthread_mutex_lock(&critcal); }
	void Unlock(void) const		{ pthread_mutex_unlock(&critcal); }
};

inline MY_THREADHANDLE PortableThreadCreate(THREAD_RETURN __stdcall function(void*), void* arguments = 0, size_t stacksize = 0)
{
	MY_THREADHANDLE thread;
	if(stacksize)
	{
		pthread_attr_t attr;
		pthread_attr_init(&attr);
		pthread_attr_setstacksize(&attr, stacksize);
		pthread_create(&thread, &attr, function, arguments);
		pthread_attr_destroy(&attr);
	}
	else
	{
		pthread_create(&thread, 0, function, arguments);
	}
	return thread;
}

template<typename T> inline void Atomic_Inc(volatile T &v)
{
	__sync_fetch_and_add(&v, 1);
}

template<typename T> inline T Atomic_Dec(volatile T &v)
{
	return __sync_fetch_and_sub(&v, 1);
}

template<typename T1, typename T2> inline void Atomic_Set(volatile T1& Target, T2 Value)
{
	__sync_fetch_and_and(&Target, 0);
	__sync_fetch_and_or(&Target, Value);
}

template<typename T1, typename T2> inline void Atomic_Add(volatile T1& Target, T2 Value)
{
	__sync_fetch_and_add(&Target, Value);
}

template<typename T1, typename T2> inline T1 Atomic_Or(volatile T1& Target, T2 Value)
{
	return __sync_fetch_and_or(&Target, Value);
}

template<typename T1, typename T2> inline T1 Atomic_And(volatile T1& Target, T2 Value)
{
	return  __sync_fetch_and_and(&Target, Value);
}

inline int _wtoi(const wchar_t *str)
{
	return static_cast<int>(wcstol(str, NULL, 10));
}

inline double _wtof(const wchar_t *str)
{
	return wcstod(str, NULL);
}

inline void PortableSleep(unsigned int _t)
{
	usleep( 1000 * _t + 1 );//a _t=0 would have no effect but means "just sleep a short time" so +1
}

inline unsigned int PortableGetTime(void)
{
	timeval tv;
	gettimeofday(&tv, 0);
	return tv.tv_sec * 1000 + tv.tv_usec / 1000;//EDIT: dirty workaround MUST BE FIXED!
}

inline size_t wcs2mbslen(const wchar_t* wcstring)
{
	return (wcstombs(NULL, wcstring, 0) + 1);
}

inline void wcs2mbs_n(char* mbstring, const wchar_t* wcstring, size_t number)
{
	wcstombs(mbstring, wcstring, number);
}

inline void mbs2wcs_n(wchar_t* wcstring, const char* mbstring, size_t number)
{
	mbstowcs(wcstring, mbstring, number);
}

#endif
};
#endif
