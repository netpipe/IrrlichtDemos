#ifdef SYNCSYS
#include <memory>
#include "ThreadedObject.h"

#if defined _WIN32 && defined _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif


using namespace helpers;

ThreadedObject::ThreadedObject(void)
	: abort(false), LastUpdateTimeInMs(-1), NeededUpdateTimeInMs(-1), Threadhandle(0), WishedUpdateIntervalInMs(0)
{

}

ThreadedObject::~ThreadedObject(void)
{
	abort = true;

	while(Threadhandle)
		PortableSleep(100);
}

void ThreadedObject::StartThread(void)
{
	_ASSERT(!Threadhandle);

	Threadhandle = PortableThreadCreate(Thread, this);
}

void ThreadedObject::StopThread(bool blocking)
{
	abort = true;

	while(blocking && IsRunning())
		PortableSleep(100);

	abort = false;
}

THREAD_RETURN ThreadedObject::Thread(void* ptr)
{
	ThreadedObject* obj = static_cast<ThreadedObject*>(ptr);
	_ASSERT(obj);

	unsigned time1;
	unsigned time2;
	while(!obj->CheckForTermination())
	{
		time1 = helpers::PortableGetTime();
		obj->ThreadUpdate(time1);
		time2 = helpers::PortableGetTime();

		obj->NeededUpdateTimeInMs	= time2 - time1;
		obj->LastUpdateTimeInMs		= time2;

		if(obj->WishedUpdateIntervalInMs == (unsigned)-1)
			PortableSleep(0);
		else if(obj->NeededUpdateTimeInMs < obj->WishedUpdateIntervalInMs)
			PortableSleep(obj->WishedUpdateIntervalInMs - obj->NeededUpdateTimeInMs);
	}

	obj->PerformLastWishes();

	obj->LastUpdateTimeInMs		= -1;
	obj->NeededUpdateTimeInMs	= -1;
	obj->Threadhandle			= 0;
	return 0;
}
#endif
