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

#include "peakengine/support/Thread.hpp"

namespace peak
{
	static void *threadProxy(void *param)
	{
		Functor *functor = (Functor*)param;
		functor->call();
		delete functor;
		return 0;
	}
	Thread::Thread()
	{
	}
	Thread::~Thread()
	{
	}

	bool Thread::create(Functor *code)
	{
		return pthread_create(&thread, NULL, threadProxy, code) == 0;
	}
	void Thread::wait()
	{
		pthread_join(thread, 0);
	}
}
