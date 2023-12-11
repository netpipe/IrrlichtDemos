/*
Range class
(c) Nicolaus Anderson
Created Jan 10, 2013

License: zlib
*/

#ifndef __RANGE_CLASS__
#define __RANGE_CLASS__

template<class T>
class Range
{
public:
	T start;
	T end;

	Range<T>()
	{
		start = 0;
		end = 0;
	}

	Range<T>( T new_start, T new_end )
	{
		start = new_start;
		end = new_end;
	}

	inline T Length()
	{
		return end - start;
	}

	operator T ()
	{
		return Length();
	}

	//------- division

	// Overridden by operator T() and operator/(T& value)
	/*
	T operator/ ( Range<T>& other )
	{
		if ( other.Length() != 0 )
			return ( end - start ) / ( other.end - other.start );

		return T(0);
	}
	*/

	T operator/ ( T& value )
	{
		if ( value != 0 )
			return Length()/value;

		return 0;
	}

	Range<T>& operator/= ( Range<T>& other )
	{
		end = start + Length()/other.Length();
		return *this;
	}

	Range<T>& operator/= ( T& value )
	{
		end = start + Length()/value;
		return *this;
	}

	//------- multiplication

	T operator* ( Range<T>& other )
	{
		return Length()*other.Length();
	}

	T operator* ( T& value )
	{
		return Length()*value;
	}

	Range<T>& operator*= ( Range<T>& other )
	{
		end = start + Length()*other.Length();
		return *this;
	}

	Range<T>& operator*= ( T& value )
	{
		end = start + Length()*value;
		return *this;
	}

	//------- addition

		//! Add ranges
	/* Returns the union of two ranges */
	Range<T> operator+ ( Range<T>& other )
	{
		return Range<T>(
			(start < other.start)? start : other.start,
			(end > other.end)? end : other.end
			);
	}

		//! Add value
	/* Returns the length of the range plus the value */
	T operator+ ( T& value )
	{
		return Length() + value;
	}

	//------- other

		//! Extend range
	/* Extends the range by the given amount while
	returning the range for inline usage. */
	Range<T>& extend( T& value )
	{
		end += value;
		return *this;
	}

		//! In range
	/* Indicates if a given value is in the range.
	\param value - The number in question.
	\param bound_inclusive - Whether the bounds should be
	included in the range. */
	bool inRange( T& value, bool bound_inclusive=true )
	{
		if ( (bound_inclusive? this->min<=value : this->min<value) )
			if ( (bound_inclusive? value<=this->max : value<this->max) )
				return true;

		return false;
	}


	// ***************** With other types *******************

	template<class T2>
	inline T2 Length()
	{
		return (T2)(end - start);
	}

	template<class T2>
	operator T2 ()
	{
		return (T2)Length();
	}

	//-------- division

	// Overridden by operator T() and operator/(T& value)
	/*
	template<class T2>
	T operator/ ( Range<T2>& other )
	{
		if ( other.Length() != 0 )
			return ( end - start ) / (T)( other.end - other.start );

		return T(0);
	}
	*/

	template<class T2>
	T operator/ ( T2& value )
	{
		if ( value != 0 )
			return Length()/(T)value;

		return 0;
	}

	template<class T2>
	Range<T>& operator/= ( Range<T2>& other )
	{
		end = start + Length()/(T)other.Length();
		return *this;
	}

	template<class T2>
	Range<T>& operator/= ( T2& value )
	{
		end = start + Length()/(T)value;
		return *this;
	}

	//------- multiplication

	template<class T2>
	T operator* ( Range<T2>& other )
	{
		return Length()*(T)other.Length();
	}

	template<class T2>
	T operator* ( T2& value )
	{
		return Length()*(T)value;
	}

	template<class T2>
	Range<T>& operator*= ( Range<T2>& other )
	{
		end = start + Length()*other.Length();
		return *this;
	}

	template<class T2>
	Range<T>& operator*= ( T2& value )
	{
		end = start + Length()*(T)value;
		return *this;
	}

	//------- addition

		//! Add ranges
	/* Returns the union of two ranges */
	template<class T2>
	Range<T> operator+ ( Range<T2>& other )
	{
		return Range<T>(
			(start < other.start)? start : (T)other.start,
			(end > other.end)? end : (T)other.end
			);
	}

		//! Add value
	/* Returns the length of the range plus the value */
	template<class T2>
	T operator+ ( T2& value )
	{
		return Length() + (T)value;
	}

	//-------

		//! Extend range
	/* Extends the range by the given amount while
	returning the range for inline usage. */
	template<class T2>
	Range<T>& extend( T2& value )
	{
		end += (T)value;
		return *this;
	}

		//! In range
	/* Indicates if a given value is in the range.
	\param value - The number in question.
	\param bound_inclusive - Whether the bounds should be
	included in the range. */
	template<class T2>
	bool inRange( T2& value, bool bound_inclusive=true )
	{
		if ( (bound_inclusive? this->min<=value : this->min<value) )
			if ( (bound_inclusive? value<=this->max : value<this->max) )
				return true;

		return false;
	}

};

#endif // define __RANGE_CLASS__
