/*

Copyright (c) 2010, Jonathan Wayne Parrott

Please see the license.txt file included with this source
distribution for more information.

*/


#ifndef __PH_AB_GC_H__
#define __PH_AB_GC_H__

#include "config.h"
#include <boost/thread.hpp>
#include <boost/function.hpp>
#include <boost/noncopyable.hpp>

namespace phoenix
{

//! Abstract Garbage Collector.
/*!
	The class eases the chore of creating threaded incremental garbage collectors. It provides a (relatively)
	simple means to peridocially run a function to clean up resource lists. The class is used by ResourceManager
	and BatchRenderer. The basic concept behind threaded garbage collection in phoenix is to allow iteration
	over containers while allowing safe removal of the objects contained. Threaded, incremental garbage collection
	achieves this by adding dropped() objects to a list of items to be removed (called a recycle list). The threaded
	garbage collector should periodically come by and lock the list of objects and remove some (or all) of the objects
	that are in the recycle list. This class provides the utility of periodically calling the function to delete objects
	in the recycle list. It does not provide the recycle list, or does it provide the function that actually removes
	the objects- this is up to the object list manager.
*/
class AbstractGarbageCollector
    : boost::noncopyable
{

public:

	/*!
		Creates an abstract garbage collector. Does not start the thread automatically, you must call
        start() to begin collecting.
		\sa setGarbageCollectionFunction()
	*/
	AbstractGarbageCollector( boost::function< void() > _f = boost::function< void() >() )
		: gc_thread(), gc_mutex(), gc_param_mutex(), gc_sleep_time( 50 ), gc_collect_rate( 10 )
	{
	}

	/*!
		Stops the thread.
        \note If the garbage collection thread accesses derived members, you should reset the function first!
	*/
	virtual ~AbstractGarbageCollector(){
		stop();
	}

    //! Start Garbage Collecting
    inline virtual void start() { 
        gc_thread = boost::thread( boost::bind( &AbstractGarbageCollector::gcThreadMain, this ) ); 
    }

    //! Stop Garbage Collecting
    inline void stop() { 
		gc_thread.interrupt(); 

		if (gc_thread.joinable())
			gc_thread.join();
		
		gc_thread = boost::thread(); 
	}

	//! Get a pointer to the thread's handle.
	inline boost::thread& getThreadHandle() { return gc_thread; }

	//! Get a pointer to the resource list's mutex.
	inline boost::recursive_mutex& getMutex() { return gc_mutex; }

	//! Lock resource list mutex.
	/*!
		This should be used (along with scoped_locks) in the resource manager that
		derives from this class in order to secure exclusive access to its
		resource list. You must call unlock() for every call to lock(), failing
		to do so will result in deadlock.
		\sa unlock()
	*/
	inline void lock(){ gc_mutex.lock(); }

	//! Unlock resource list mutex.
	/*!
		This should be used (along with scoped_locks) in the resource manager that
		derives from this class in order to release exclusive access to the resource
		list. 
		\sa lock()
	*/
	inline void unlock() { gc_mutex.unlock(); }

    //! Clean function.
    /*!
        This function is called by the thread or may be called manually. Derived classes
        are expected to overload this to define their own functions. Derived classes
        are responsible for locking their own mutexes!
    */
    virtual void clean() = 0;

	//! Get Sleep Time.
	unsigned int getSleepTime(){
		boost::mutex::scoped_lock( gc_param_mutex );
		return gc_sleep_time;
	}

	//! Set Sleep Time.
	void setSleepTime( unsigned int _s ){
		boost::mutex::scoped_lock( gc_param_mutex );
		gc_sleep_time = _s;
	}

	//! Get Collection Rate
	unsigned int getCollectionRate(){
		boost::mutex::scoped_lock( gc_param_mutex );
		return gc_collect_rate;
	}

	//! Set Collection Rate
	void setCollectionRate( unsigned int _r ){
		boost::mutex::scoped_lock( gc_param_mutex );
		gc_collect_rate = _r;
	}


private:

	//! Thread Handle.
	boost::thread gc_thread;
	
	//! Resource list Mutex.
	boost::recursive_mutex gc_mutex;

	//! Parameter mutex
	boost::mutex gc_param_mutex;

	//! Sleep time.
	/*!
		How long (in milliseconds) the thread sleeps before calling
		the Garbage Collection function. default is 50ms.
	*/
	unsigned int gc_sleep_time;

	//! Collection rate.
	/*
		The number of resources this will collect every visit. This is optional
		and should be used in the garbage collection function if desired. default
		is 10 resources.
	*/
	unsigned int gc_collect_rate;

	//! Garbage Collection Thread.
	/*!
		This is fairly simple, but fairly robust thread. It sleeps for gc_sleep_time, then
		calls clean(). It always waits for interruption from the desctructor
		in order to terminate.
		\sa setGarbageCollectionFunction()
	*/
	void gcThreadMain()
	{
		try
		{
			while(1)
			{

				//sleep
				boost::this_thread::sleep( boost::posix_time::milliseconds( getSleepTime() ) );

			    //clean
				clean();
			}
		}
		catch( boost::thread_interrupted )
		{
			return;
		}
	}


}; // AbstractGarbageCollector

}; //namespace phoenix

#endif //__PH_AB_GC_H__