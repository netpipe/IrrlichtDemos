// Copyright 2017 Nicolaus Anderson
// see license.txt for details

#include "../include/irragg_matrix.h"

namespace irr {
namespace core {

	// Irrlicht uses a 16-slot array and treats each successive set of 4 slots
	// as a column.
	// AGG uses named doubles that correspond to certain Irrlicht matrix slots
	// that perform the same purpose.
	// Below is a diagram showing the slots in Irrlicht with their corresponding
	// AGG trans affine matrix representations in a similarly-arranged matrix.
	//
	// [0 ] [4 ] [8 ] [12] -> [sx ] [shx] [   ] [tx ]
	// [1 ] [5 ] [9 ] [13] -> [shy] [sy ] [   ] [ty ]
	// [2 ] [6 ] [10] [14] -> [   ] [   ] [   ] [   ]
	// [3 ] [7 ] [11] [15] -> [   ] [   ] [   ] [   ]
	//

	//!
	::agg::trans_affine
		convertCMatrix4ToAGGaffine(
			const core::CMatrix4<double>&  m
		)
	{
		::agg::trans_affine  out;

		out.sx = m[0];
		out.shy = m[1];
		out.shx = m[4];
		out.sy = m[5];
		out.tx = m[12];
		out.ty = m[13];

		return out;
	}

	//!
	core::CMatrix4<double>
		convertAGGaffineToCMatrix4(
			const ::agg::trans_affine&  m
		)
	{
		core::CMatrix4<double>  out;

		out[0] = m.sx;
		out[1] = m.shy;
		out[4] = m.shx;
		out[5] = m.sy;
		out[12] = m.tx;
		out[13] = m.ty;

		return out;
	}

} // end namespace core
} // end namespace irr
