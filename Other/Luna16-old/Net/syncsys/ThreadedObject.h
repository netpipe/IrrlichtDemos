#ifdef SYNCSYS
#pragma once

#ifndef DONT_INCLUDE_HEADERS
#include "Portable.h"
#endif

namespace helpers
{
//class to create an object which is updated by an own thread in an easy way
class ThreadedObject
{
	bool abort;
	volatile unsigned LastUpdateTimeInMs;
	volatile unsigned NeededUpdateTimeInMs;
	volatile MY_THREADHANDLE Threadhandle;

	static THREAD_RETURN __stdcall Thread(void*);

protected:
	///If the thread needs less time it will sleep for WishedUpdateIntervalInMs - neededtime. A WishedUpdateIntervalInMs of -1 signals, that the thread do a 0 sleep every loop.
	unsigned WishedUpdateIntervalInMs;

	///returns true if the thread should terminate its exection. Useful if you dont want to run it in a real thread.
	bool			CheckForTermination(void) const { return abort || ShouldStop(); }
	///is called frequently by the thread to check if it should stop
	virtual bool	ShouldStop(void) const = 0;
	///is called frequently by the thread
	virtual void	ThreadUpdate(unsigned time) = 0;
	///is called if the thread is going to stop execution and before reseting the variables.
	virtual void	PerformLastWishes(void) {}

public:

	///
	ThreadedObject(void);
	///\note: the dtor will block until the thread finished.
	virtual ~ThreadedObject();

	///starts the thread
	virtual void StartThread(void);
	///stops the thread.
	/**If called from the thread it has to be nonblocking!
	Thread can be started again after successful termination if wished.*/
	void StopThread(bool blocking = true);

	///returns if the thread is still running
	bool			IsRunning(void) const				{ return Threadhandle != 0; }
	///returns the timestamp of the last update
	unsigned		GetLastUpdateTimeInMs(void) const	{ return LastUpdateTimeInMs; }
	///returns the time how long the last ThreadUpdate-call needed
	unsigned		GetNeededUpdateTimeInMs(void) const	{ return NeededUpdateTimeInMs; }
};
}
#endif
