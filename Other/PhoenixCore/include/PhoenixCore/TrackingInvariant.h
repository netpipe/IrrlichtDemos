/*

Copyright (c) 2010, Jonathan Wayne Parrott

Please see the license.txt file included with this source
distribution for more information.

*/

#ifndef __PHTRACKINVARIANT__
#define __PHTRACKINVARIANT__

#include "config.h"

namespace phoenix
{

//! Key-Based Invariant
/*!
	An Invariant is a object whose value does not change; however, a tracking invariant can change but change implies
	that something must happen in order for the invariant to be valid again. This class provides that facility.
	It holds an object and keeps track of its current value and the previous valid value and provides means
	to check and update those.
*/
template< class T >
class TrackingInvariant
{

public:

	/*!
		Constructs a new valid invariant.
		\param _v Default valid value.
	*/
	TrackingInvariant( const T& _v = T() )
		: value(_v), previous(_v), invariant(true)
	{}

	virtual ~TrackingInvariant()
	{}

	/*!
		Sets the current value, and will invalidate the invariant if \f$ v != value \f$.
		This is automatically called by operator=().
		\sa operator=(), get(), check(), reset()
	*/
	inline virtual void set( const T& _v )
	{
		if( _v != value )
		{
			if( invariant )
			{
				previous = value;
			}
			value = _v;
			invariant = false;
		}
	}

	/*!
		Gets the current value. Does not affect invariant.
		You can also use implicit conversion to T.
		\sa set()
		\sa getPrevious()
	*/
	inline virtual const T& get( ) const
	{
		return value;
	}

	/*!
		Returns the previous valid value.
		\sa get()
	*/
	inline virtual const T& getPrevious() const
	{
		return previous;
	}

	//! Validity Check.
	/*!
		\returns True if value has been modified since the last reset().
	*/
	inline virtual bool check() const 
	{
		return invariant;
	}

	/*!
		This function sets the invariant to valid and the previous valid to the current value,
		thus making the change permanent.
		\sa check(), set()
	*/
	inline virtual void reset()
	{
		if( ! invariant )
		{
			previous = value;
			invariant = true;
		}
	}

	//! Implicit Conversion.
	/*!
		\returns The current value.
	*/
	inline operator const T& () const
	{
		return value;
	}

	//! Implicit assignment
	/*!
		Calls set(), can invalidate the invariant.
		\sa set()
	*/
	inline const TrackingInvariant& operator= ( const T& _rv )
	{
		this->set( _rv );
		return (*this );
	}

protected:
	T value; //!< Current value
	T previous; //!< Last valid value
	bool invariant; //!< Validity tracker

}; //end tracking invariant

} //namespace phoenix

#endif //__PHTRACKINVARIANT__