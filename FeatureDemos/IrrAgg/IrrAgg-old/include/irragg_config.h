// Copyright 2017 Nicolaus Anderson
// see license.txt for details

#ifndef __IRR_AGG_CONFIG_H__
#define __IRR_AGG_CONFIG_H__

/*
	Note:
	C++99 and earlier contains only NULL, an alias of zero, not a pointer type.
	C++11 and later have it, but in order to be flexible, we let the user decide
		which to use.
*/
#ifndef IRR_AGG_NULL
 #ifdef IRR_AGG_USE_NULLPTR
  #define IRR_AGG_NULL nullptr
 #else
  #define IRR_AGG_NULL 0
 #endif
#endif

// Uncomment to enable compiling as big-endian
//#define IRR_AGG_COMPILE_AS_BIG_ENDIAN

namespace irr {
namespace vecg {

	/*
		Note to future viewers:
		AGG defines its own primitive types but never uses them.
		Irrlicht defines its own primitive types and uses them everywhere,
			but a number of them are strictly tied to specific types
			(e.g. irr::f64 is guaranteed to be "double" on every platform).
		Rather that introduce conflicts by using Irrlicht types, it makes more
			sense to continue with AGG and use basic primitives, especially since
			AGG is doing the heavy lifting.
	*/

	//typedef unsigned  UInt32;
	//typedef float     Float32;
	//typedef double    Float64;

	// For user convenience. Uncomment if desired.
	//typedef ::irr::core::vector2d<double>  vector2dd;

} // end sub namespace vecg
} // end namespace irr

#endif
