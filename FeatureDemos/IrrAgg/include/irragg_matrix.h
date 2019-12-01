// Copyright 2019 Nicolaus Anderson
// See license.txt for details.
/*
	Functions for converting to and from CMatrix4 from Irrlicht and trans_affine from AGG.
*/

#ifndef __IRR_AGG_MATRIX_H__
#define __IRR_AGG_MATRIX_H__

//*** Anti-Grain Geometry ***
#include <agg_trans_affine.h>

//*** Irrlicht ***
#include <matrix4.h>


namespace irr {
namespace core {

	//!
	inline
	::agg::trans_affine
		convertCMatrix4ToAGGaffine(
			const core::CMatrix4<double>&  m
		);

	//!
	inline
	core::CMatrix4<double>
		convertAGGaffineToCMatrix4(
			const ::agg::trans_affine&  m
		);

} // end namespace core
} // end namespace irr

#endif // __IRR_AGG_MATRIX_H__
