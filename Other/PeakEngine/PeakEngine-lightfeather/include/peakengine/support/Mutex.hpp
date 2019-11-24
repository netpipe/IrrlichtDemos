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

#ifndef _PEAKENGINE_SUPPORT_MUTEX_HPP_
#define _PEAKENGINE_SUPPORT_MUTEX_HPP_

#include <pthread.h>

namespace peak
{
	/**
	 * Simple mutex for multithreading safety. The mutex is NOT recursive.
	 * \todo Currently only works with pthreads. We need Win32 threads as well.
	 */
	class Mutex
	{
		public:
			/**
			 * Constructor.
			 */
			Mutex();
			/**
			 * Destructor.
			 */
			~Mutex();

			/**
			 * Tries to lock the mutex, but returns immediately in any case.
			 * @return true, if the mutex was locked.
			 */
			bool tryLock() const;
			/**
			 * Locks the mutex.
			 */
			void lock() const;
			/**
			 * Unlocks the mutex.
			 */
			void unlock() const;
		private:
			mutable pthread_mutex_t mutex;
	};
}

#endif
