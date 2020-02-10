// Copyright (C) 2002-2006 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#ifndef __IRR_MATH_H_INCLUDED__
#define __IRR_MATH_H_INCLUDED__

#include "irrTypes.h"

namespace irr
{
namespace core
{
	//! Rounding error constant often used when comparing f32 values.
	const f32 ROUNDING_ERROR	= 0.0001f;

	//! Constant for PI.
	const f32 PI			= 3.14159f;

	//! Constant for 64bit PI.
	const f64 PI64			= 3.1415926535897932384626433832795028841971693993751;

	//! Constant for converting radians to degrees.
	const f64 GRAD_PI		= 180.0 / PI64;

	//! Constant for converting degrees to radians.
	const f64 GRAD_PI2		= PI64 / 180.0;

       //! 32bit Constant for converting from degrees to radians
       const f32 DEGTORAD              = PI / 180.0f;

	//! 32bit constant for converting from radians to degrees
	const f32 RADTODEG              = 180.0f / PI;

	//! 64bit constant for converting from degrees to radians
	const f64 DEGTORAD64    = PI64 / 180.0;

	//! 64bit constant for converting from radians to degrees
	const f64 RADTODEG64    = 180.0 / PI64;

	//! returns minimum of two values. Own implementation to get rid of the STL (VS6 problems)
	template<class T>
	inline const T min_(const T a, const T b)
	{
		return a < b ? a : b;
	}
	
	//! returns minimum of two values. Own implementation to get rid of the STL (VS6 problems)
	template<class T>
	inline T max_(const T a, const T b)
	{
		return a < b ? b : a;
	}

	//! returns abs of two values. Own implementation to get rid of STL (VS6 problems)
	template<class T>
	inline T abs_(const T a)
	{
		return a < 0 ? -a : a;
	}

	//! returns if a float equals the other one, taking floating 
	//! point rounding errors into account
	inline bool equals(f32 a, f32 b)
	{
		return (a + ROUNDING_ERROR > b) && (a - ROUNDING_ERROR < b);
	}

} // end namespace core
}// end namespace irr

#endif

