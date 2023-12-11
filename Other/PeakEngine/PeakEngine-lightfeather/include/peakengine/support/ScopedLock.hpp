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

#ifndef _PEAKENGINE_SUPPORT_SCOPEDLOCK_HPP_
#define _PEAKENGINE_SUPPORT_SCOPEDLOCK_HPP_

#include "Mutex.hpp"

namespace peak
{
	/**
	 * Auxilliary class for automatically unlocking a mutex when the current
	 * variable scope is left. Example:
	 * \code
	 * void someFunction()
	 * {
	 *     Mutex m;
	 *     // The mutex is not locked here
	 *     for (int i = 0; i < 10; i++)
	 *     {
	 *         ScopedLock lock(m);
	 *         // The mutex is locked here
	 *     }
	 *     // The mutex is not locked here
	 * }
	 * \endcode
	 * This is useful to restrict a mutex lock to one function.
	 */
	class ScopedLock
	{
		public:
			/**
			 * Constructor. Locks the mutex.
			 */
			ScopedLock(const Mutex &mutex);
			/**
			 * Destructor. Unlocks the mutex.
			 */
			~ScopedLock();
		private:
			const Mutex &mutex;
	};
}

#endif
