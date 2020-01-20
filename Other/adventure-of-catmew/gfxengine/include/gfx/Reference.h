// Copyright (C) 2007 Kevin van Leeuwen
// This file is part of the "LeeuwenGfx" Library.
// For conditions of distribution and use, see copyright notice in LeeuwenGfx.h

#ifndef _ReferenceGfxHeader_
#define _ReferenceGfxHeader_

namespace lew
{
	namespace gfx
	{
		//! A reference counting class...
		/** This class provides reference counting and should be derrived from.

		\code
			class subclass : public gen::Reference
			{
				public:
					subclass() : Reference() {}
			};
			
			//...
			
			subclass* instance = new subclass();
			
			instance->lock();			
			instance->remove(); //won't work, instance is locked
			
			instance->unlock();
			instance->remove(); //works now			
		\endcode
		*/
		class Reference
		{
			public:
				//! constructor
				//! derrived classes need to call this constructor.
				Reference()
				{
					ReferenceCount = 0;
					grab();
				
					Lock = false;
				}

				virtual ~Reference() {}

				//! increase the reference count by one
				void grab()
				{
					++ReferenceCount;
				}

				//! decrease the reference count by one
				void ungrab()
				{
					if(ReferenceCount > 0)
						--ReferenceCount;
				}

				//! delete the class instance.
				void remove()
				{
					ungrab();
				
					if(!Lock)
					{
						if(ReferenceCount == 0)
							delete (this);
					}
				}
			
				//! lock the class instance. when an instance is locked, 
				//! you can't remove() it until you unlock() it again.
				void lock()
				{
					Lock = true;
				}
			
				//! unlock the class instance.
				void unlock()
				{
					Lock = false;
				}
			
				//! \return the reference count.
				int getReferenceCount() const
				{
					return ReferenceCount;
				}
			

			protected:
				int ReferenceCount;
				bool Lock;
		};
	};
};

#endif
