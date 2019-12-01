// (C) 2019 Nicolaus Anderson
// See license.txt for details.

#ifndef IRRAGG_TRIANGLE_H
#define IRRAGG_TRIANGLE_H

#include "../irragg_shape.h"

namespace irr {
namespace vecg {

//! Triangle Path
/*
	A basic triangle meant to be used with Painter.
	Possible Usage:
	[code]
		Triangle  triangle( point_t(0,0), point_t(1,0), point_t(0,1) );
		Painter.drawNormal(triangle);
	[/code]
*/
struct Triangle
	: public IShape
{
	//! Default constructor
	Triangle()
		: A(0), B(0), C(0)
	{}

	//! Construct from three points
	Triangle( point_t  a, point_t  b, point_t  c )
		: A(a), B(b), C(c)
	{}

	//! Return how the polygon must be drawn (indices start at zero)
	virtual PathMotion  getPathMotion( size_t  index ) const {
		switch(index)
		{
		case 1: return PathMotion(B.X, B.Y);
		case 2: return PathMotion(C.X, C.Y);
		default: return PathMotion(A.X, A.Y);
		}
	}

	//! Return the number of points in the polygon
	virtual size_t  getPathStepCount() const {
		// Fourth point closes the triangle
		return 4;
	};

	// Members -------------------------------
	point_t  A;
	point_t  B;
	point_t  C;
};

} // end namespace vecg
} // end namespace irr

#endif
