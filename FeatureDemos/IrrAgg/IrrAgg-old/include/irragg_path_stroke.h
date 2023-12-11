// Copyright 2017 Nicolaus Anderson
// See license.txt for details.
/*
	Path stroke type
*/

#ifndef __IRR_AGG_PATH_STROKE_H__
#define __IRR_AGG_PATH_STROKE_H__

namespace irr {
namespace vecg {

	//! Path Stroke
	/*
		Indicates how the stroke should be drawn.
	*/
	struct EPathStroke {
	enum Value {
		NORMAL,
		BRUSH,
		BSPLINE
	};
	};

} // close namespace agg
} // close namespace irr

#endif // __IRR_AGG_PATH_STROKE_H__
