/*

Copyright (c) 2010, Jonathan Wayne Parrott

Please see the license.txt file included with this source
distribution for more information.

*/

#ifndef __PHDROPPABLE_H__
#define __PHDROPPABLE_H__

#include <boost/intrusive_ptr.hpp>
#include "config.h"

// Intrusive_ptr stuff foward decl
namespace phoenix{ class Droppable; }

namespace boost{
	//! Droppable intrusive_ptr reference increment function.
    void intrusive_ptr_add_ref( phoenix::Droppable*);
	//! Droppable intrusive_ptr reference decrement function.
    void intrusive_ptr_release( phoenix::Droppable*);
}


namespace phoenix
{

//! Droppable object.
/*!
	Provides a simple abstract class for 'droppable' objects. Essentially
	this eases the creating of garbage collected objects that are to be
	iterated over. Derived classes should add themselves to their managers
	recycle list to be garbage collected. Dropped objects are considered
	deleted and should be skipped during iteration. It also provides
    the facilities for intrusive_ptr to work on droppable objects.
	\sa AbstractGarbageCollector
*/
class Droppable
{

public:

	Droppable()
		: _refcount(0), _dropped( false )
	{}

	virtual ~Droppable()
	{
	}

	//! Drop
	/*!
		Sets this object to 'dropped', and does nothing else.
		It is encourage that derived classes overload this,
		and simply call it with Droppable::drop().
	*/
	inline virtual void drop()
	{
		_dropped = true;
	}

	//! Dropped
	/*!
		Returns if this object has been 'dropped', this
		can be used by iterative lists that want to 
		skip objects that have been listed to be deleted.
	*/
	inline bool dropped()
	{
		return _dropped;
	}

    //! Get reference count
    inline unsigned int getReferenceCount(){ return _refcount; }


private:

    friend void boost::intrusive_ptr_add_ref( Droppable* );
    friend void boost::intrusive_ptr_release( Droppable* );

    unsigned int _refcount;
	bool _dropped;

}; //class droppable

//! Friendly name for ptrs to droppables.
typedef boost::intrusive_ptr< Droppable > DroppablePtr;

} //namespace phoenix

// Intrusive_ptr stuff 
namespace boost{
    inline void intrusive_ptr_add_ref( phoenix::Droppable* ptr ){ ++(ptr->_refcount); }
    inline void intrusive_ptr_release( phoenix::Droppable* ptr ){
		if( ptr->_refcount == 0 ) throw std::runtime_error("Invalid itrusive_ptr release on phoenix::Droppable");
        if( (--ptr->_refcount) == 0){
            delete ptr;
        }
    }
}

#endif //__PHDROPPABLE_H__