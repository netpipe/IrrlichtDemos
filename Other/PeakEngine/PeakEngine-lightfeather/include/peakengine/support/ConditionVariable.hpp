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

#ifndef _PEAKENGINE_SUPPORT_CONDITIONVARIABLE_HPP_
#define _PEAKENGINE_SUPPORT_CONDITIONVARIABLE_HPP_

#include <pthread.h>

namespace peak
{
	/**
	 * Condition variable class for safe variable locking. One thread locks the
	 * condition variable and then waits for it, the other (signalling) thread
	 * locks it and then signals the condition variable which makes the other
	 * thread resume. Note that wait() has to be called BEFORE the signal()
	 * occurs or a deadlock will take place! Example:
	 * \code
	 * ConditionVariable done;
	 * void test()
	 * {
	 *     // Lock the variable first!
	 *     done.lock();
	 *     // Start the thread
	 *     startThread(thread);
	 *     // Wait for the thread
	 *     done.wait();
	 *     done.unlock();
	 * }
	 * void thread()
	 * {
	 *     // Do something here
	 *     // Signal the first thread that we are done
	 *     done.lock();
	 *     done.signal();
	 *     done.unlock();
	 * }
	 * \endcode
	 * Currently only one waiting thread at once is supported.
	 * \todo Allow more waiting threads.
	 */
	class ConditionVariable
	{
		public:
			/**
			 * Constructor.
			 */
			ConditionVariable();
			/**
			 * Destructor.
			 */
			~ConditionVariable();

			/**
			 * Locks the condition variable. Has to be called before signal() or
			 * wait().
			 */
			void lock();
			/**
			 * Unlocks the condition variable. Has to be called after signal()
			 * or wait().
			 */
			void unlock();

			/**
			 * Causes the waiting thread to wake up.
			 */
			void signal();
			/**
			 * Waits for a signal() to happen.
			 */
			void wait();
		private:
			pthread_mutex_t mutex;
			pthread_cond_t cond;
	};
}

#endif
