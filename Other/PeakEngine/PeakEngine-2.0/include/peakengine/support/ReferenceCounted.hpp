/*
Copyright (C) 2009  Mathias Gottschlag

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in the
Software without restriction, including without limitation the rights to use,
copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the
Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef _PEAKENGINE_SUPPORT_REFERENCECOUNTED_HPP_
#define _PEAKENGINE_SUPPORT_REFERENCECOUNTED_HPP_

namespace peak
{
	template<typename T> struct remove_const
	{
		typedef T type;
	};
	template<typename T> struct remove_const<const T>
	{
		typedef T type;
	};

	class ReferenceCounted;

	/**
	 * Class to derive other weak pointers from.
	 */
	class GenericWeakPointer
	{
		public:
			/**
			 * Constructor.
			 */
			GenericWeakPointer()
			{
				prev = 0;
				next = 0;
				target = 0;
			}
			/**
			 * Destructor.
			 */
			virtual ~GenericWeakPointer()
			{
				attach(0);
			}

			/**
			 * Invalidates the pointer. This usually happens when the object
			 * it is pointing to gets deleted.
			 */
			void invalidate()
			{
				attach(0);
			}

			/**
			 * Checks whether the pointer is invalid.
			 */
			bool isNull()
			{
				return target == 0;
			}

			operator bool() const
			{
				return target != 0;
			}
		protected:
			/**
			 * Attachs the pointer to an object and sets the value of the
			 * pointer. It will then automatically get invalidated if the
			 * object is deleted.
			 */
			inline void attach(ReferenceCounted *newtarget);

			ReferenceCounted *target;
		private:
			GenericWeakPointer *next;
			GenericWeakPointer *prev;
	};

	/**
	 * Base class for all classes which use reference counting. Reference
	 * counting makes sure that objects aren't deallocated as long as there are
	 * references to them (see: SharedPointer) by just counting the references.
	 * This is done via grab() (increments the reference count) and drop()
	 * (decrements the reference count). When the reference count reaches 0, the
	 * object is destroyed.
	 */
	class ReferenceCounted
	{
		public:
			ReferenceCounted()
			{
				refcount = 0;
				weakptr = 0;
			}
			virtual ~ReferenceCounted()
			{
				// Reset weak pointers
				while (weakptr)
					weakptr->invalidate();
			}

			/**
			 * Increments the reference count.
			 */
			void grab() const
			{
				refcount++;
			}
			/**
			 * Decrements the reference count.
			 */
			void drop() const
			{
				refcount--;
				if (refcount <= 0)
					delete this;
			}
		private:
			/**
			 * Number of references to the object.
			 */
			mutable int refcount;

			GenericWeakPointer *weakptr;

			friend class GenericWeakPointer;
	};

	/**
	 * Pointer class for simple access to ReferenceCounted. Does the grab()
	 * and drop() itself. The pointer still has to be initialized before any
	 * use, null pointer access is not caught properly and has to be checked
	 * via casting the pointer to bool or isNull().
	 */
	template<class T> class SharedPointer
	{
		public:
			/**
			 * Constructor.
			 * @param target Pointer to a reference counted object
			 */
			SharedPointer(T *target = 0) : target(target)
			{
				if (target)
					target->grab();
			}
			/**
			 * Copy constructor.
			 */
			SharedPointer(const SharedPointer<typename remove_const<T>::type> &ptr) : target(ptr.get())
			{
				if (target)
					target->grab();
			}
			/**
			 * Destructor.
			 */
			~SharedPointer()
			{
				if (target)
					target->drop();
			}

			/**
			 * Checks whether the pointer is invalid.
			 */
			bool isNull()
			{
				return target == 0;
			}

			/**
			 * Returns the raw pointer. This should usually not be used.
			 */
			T *get() const
			{
				return target;
			}

			SharedPointer &operator=(const SharedPointer &ptr)
			{
				if (target)
					target->drop();
				target = ptr.target;
				if (target)
					target->grab();
				return *this;
			}
			T *operator->() const
			{
				return target;
			}
			bool operator==(const SharedPointer &ptr) const
			{
				return target == ptr.target;
			}
			bool operator==(T *ptr) const
			{
				return target == ptr;
			}
			operator bool() const
			{
				return target != 0;
			}
		private:
			T *target;
	};

	void GenericWeakPointer::attach(ReferenceCounted *newtarget)
	{
		// Remove the pointer from the old target first
		if (target)
		{
			if (this == target->weakptr)
			{
				// We are in the beginning of the list
				target->weakptr = next;
				if (next)
				{
					next->prev = 0;
				}
			}
			else
			{
				// We are somewhere in the double linked list
				prev->next = next;
				if (next)
				{
					next->prev = prev;
				}
			}
		}
		// Attach the weak pointer to the new one
		if (newtarget)
		{
			// Set the new target
			target = newtarget;
			// Insert this pointer before the first pointer in the list
			if (target->weakptr)
				target->weakptr->prev = this;
			next = target->weakptr;
			prev = 0;
			target->weakptr = this;
		}
		else
		{
			// Just zero everything
			target = 0;
			next = 0;
			prev = 0;
		}
	}

	/**
	 * Weak pointer class for classes derived from ReferenceCounted.
	 */
	template<class T> class WeakPointer : public GenericWeakPointer
	{
		public:
			/**
			 * Constructor.
			 * @param target Pointer to a reference counted object
			 */
			WeakPointer(T *target = 0) : GenericWeakPointer()
			{
				attach(target);
			}
			/**
			 * Constructor.
			 * @param target Pointer to a reference counted object
			 */
			WeakPointer(SharedPointer<T> target) : GenericWeakPointer()
			{
				attach(target.get());
			}
			/**
			 * Destructor.
			 */
			~WeakPointer()
			{
			}

			/**
			 * Returns the raw pointer. This should usually not be used.
			 */
			T *get() const
			{
				return (T*)target;
			}

			WeakPointer &operator=(const WeakPointer &ptr)
			{
				attach(ptr.target);
				return *this;
			}
			WeakPointer &operator=(const SharedPointer<T> &ptr)
			{
				attach(ptr.get());
				return *this;
			}
			T *operator->() const
			{
				return (T*)target;
			}
			bool operator==(const WeakPointer &ptr) const
			{
				return target == ptr.target;
			}
			bool operator==(T *ptr) const
			{
				return target == ptr;
			}
		private:
	};
}

#endif
