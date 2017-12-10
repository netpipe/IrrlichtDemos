/*
(c) 2012-2013 Nicolaus Anderson
Generated from Incrementer.h on: Jan 1, 2012

Incrementor - A simple incrementing class for keeping track of changes in a value
with fewer keystrokes.
*/

#ifndef __INCREMENTOR__
#define __INCREMENTOR__


enum ECycleType
{
	/* This incrementor is to act like a boolean value, switching back and forth
	between 0 and 1. */
	CYC_BOOLEAN = 0,
	
	/* This incrementor must be set to the min value after reaching the max value
	or must be set to the max value if decremented from the min value. */
	CYC_REPEAT,
	
	/* This incrementor counts up and down like an ordinary integer. */
	CYC_DEFAULT
};

template<class Num>
class Inc
{
	Num value;
	Num step;
	Num min;
	Num max;

	ECycleType cycle;

public:

	//! Constructor
	Inc(
		ECycleType type = CYC_DEFAULT,
		Num start_val=0, Num start_step=1, Num start_min=0, Num start_max=1
		)
		: value( start_val ), step( start_step ), min(start_min), max( start_max ), cycle( type )
	{}
	
	
	//! Deconstructor
	~Inc<Num>() {}
	
	
	//! Get type of cycle
	ECycleType getCycleType()
	{
		return cycle;
	}
	
	//! Set the cycle type directly
	void setCycleType( ECycleType type )
	{
		cycle = type;
	}
	
	//! Set the cycle type to the default
	void ctDef()
	{
		cycle = CYC_DEFAULT;
	}
	
	//! Set the cycle type to boolean
	void ctBool()
	{
		cycle = CYC_BOOLEAN;
	}
	
	//! Set the cycle type to repeat
	void ctRpt()
	{
		cycle = CYC_REPEAT;
	}

	//! Checks if out of bounds - fixes if necessary
	bool outOfBounds()
	{
		switch ( cycle )
		{
		case CYC_BOOLEAN:
			if ( value > 1 )
			{
				value = 0;
				return true;
			} else if ( value < 0 )
			{
				value = 1;
				return true;
			}
			break;

		case CYC_REPEAT:
			if ( value > max )
			{
				// Wrap around if greater than the max
				value = (value - max) + min;
				return true;
			} else if ( value < min )
			{
				// Wrap around if greater than the max
				value = (value - max) + min;
				return true;
			}

		default:
			if ( value < min || value > max )
				return true;
			break;
		}

		return false;
	}


//******* Getters and setters **********

	Num& Val()
	{
		return value;
	}

	Num& getStep()
	{
		return step;
	}
	
	Num& getMin()
	{
		return min;
	}
	
	Num& getMax()
	{
		return max;
	}

	void setVal( Num new_val )
	{
		value = new_val;
	}
	
	void setStep( Num new_step )
	{
		step = new_step;
	}
	
	void setMin( Num new_min )
	{
		min = new_min;
		clampVal();
	}
	
	void setMax( Num new_max )
	{
		max = new_max;
		clampVal();
	}

	void setMinShiftMax( Num new_min )
	{
		max += new_min - min;
		min = new_min;
		clampVal();
	}

	void setMaxShiftMin( Num new_max )
	{
		min += new_max - max;
		max = new_max;
		clampVal();
	}

	void setRange( Num new_min, Num new_max )
	{
		min = new_min;
		max = new_max;
		clampVal();
	}

	// Range modifiers
#ifdef __RANGE_CLASS__
	Range<Num> getRange()
	{
		return Range<Num>( min, max );
	}

	void setRange( Range<Num>& range )
	{
		min = range.start;
		max = range.end;
		clampVal();
	}

	Range<Num>& operator= ( Range<Num>& range )
	{
		min = range.start;
		max = range.end;
		clampVal();
		return *this;
	}
#endif

	void restart()
	{
		value = min;
	}

	void clampVal()
	{
		if ( value < min )
			value = min;

		else if ( max < value )
			value = max;
	}


// ********* Shortcut operators ***********
	
	bool operator++ ()
	{
		switch( cycle )
		{
		case CYC_BOOLEAN:
			value++;
			break;
		
		case CYC_REPEAT:
			if ( value == max )
			{
				value = min;
				return true;
			} else {
				value += step;
			}
			break;
		
		default:
			value += step;
		}

		return outOfBounds();
	}
	
	bool operator-- ()
	{
		switch( cycle )
		{
		case CYC_BOOLEAN:
			value--;
			break;
			
		case CYC_REPEAT:
			if ( value == min )
			{
				value = max;
				return true;
			} else {
				value -= step;
			}
			break;
			
		default:
			value -= step;
		}

		return outOfBounds();
	}
	
	bool operator==( Inc<Num>& other )
	{
		return value == other.value;
	}
	
	bool operator>( Inc<Num>& other )
	{
		return value > other.value;
	}
	
	bool operator>=( Inc<Num>& other )
	{
		return value >= other.value;
	}
	
	bool operator<( Inc<Num>& other )
	{
		return value < other.value;
	}
	
	bool operator<=( Inc<Num>& other )
	{
		return value <= other.value;
	}
	
	Inc<Num>& operator=( Inc<Num>& other )
	{		
		return copy(other);
	}
	
	Inc<Num>& copy( Inc<Num>& other )
	{
		value = other.value;
		step = other.step;
		min = other.min;
		max = other.max;
		cycle = other.cycle;
		
		return *this;
	}

	/* REMOVED
	Reason: Working with operator=( Num val ) it overrides
	the role of operator=( Inc<Num>& other )

	template<class T>
	operator T ()
	{
		return (T)value;
	}
	*/

//****** assignment operators *******

	Inc<Num>& operator+=( Inc<Num>& other )
	{
		value = other.value;

		return *this;
	}

	Inc<Num>& operator-=( Inc<Num>& other )
	{
		value -= other.value;

		return *this;
	}

	Inc<Num>& operator=( Num val )
	{
		value = val;

		return *this;
	}

	Inc<Num>& operator+=( Num val )
	{
		value += val;

		return *this;
	}
	Inc<Num>& operator-=( Num val )
	{
		value -= val;

		return *this;
	}

	/*
	Inc<Num>& operator=( stdu32 val )
	{
		value = (Num)val;

		return *this;
	}

	Inc<Num>& operator+=( stdu32 val )
	{
		value += (Num)val;

		return *this;
	}

	Inc<Num>& operator-=( stdu32 val )
	{
		value -= (Num)val;

		return *this;
	}
	*/
};

#endif // define __INCREMENTOR__