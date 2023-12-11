// (C) 2019 Nicolaus Anderson
// See license.txt for details.

#ifndef IRRAGG_RECTANGLE_H
#define IRRAGG_RECTANGLE_H

#include "../irragg_shape.h"

namespace irr {
namespace vecg {

//! Rectangle Path
/*
	A basic rectangle meant to be used with Painter.
	Possible Usage:
	[code]
		Rectangle  rectangle( point_t(0,0), point_t(1,1) );
		Painter.drawNormal(rectangle);
	[/code]
*/
struct Rectangle
	: public IShape
{
	//! Default constructor
	Rectangle()
		: UpperLeft(0)
		, LowerRight(0)
	{}

	//! Construct from two points
	Rectangle( point_t  upperLeft, point_t  lowerRight )
		: UpperLeft(upperLeft)
		, LowerRight(lowerRight)
	{
		repair();
	}

	//! Return how the polygon must be drawn (indices start at zero)
	virtual PathMotion  getPathMotion( size_t  index ) const {
		// Drawing motion is clockwise
		switch(index)
		{
		case 1: return PathMotion(LowerRight.X, UpperLeft.Y);
		case 2: return PathMotion(LowerRight.X, LowerRight.Y);
		case 3: return PathMotion(UpperLeft.X, LowerRight.Y);
		default: return PathMotion(UpperLeft.X, UpperLeft.Y);
		}
	}

	//! Return the number of points in the polygon
	virtual size_t  getPathStepCount() const {
		// Fifth point closes the rectangle
		return 5;
	};

	void repair() {
		point_t  xy0 = UpperLeft;
		if ( UpperLeft.X > LowerRight.X ) {
			UpperLeft.X = LowerRight.X;
			LowerRight.X = xy0.X;
		}
		if ( UpperLeft.Y > LowerRight.Y ) {
			UpperLeft.Y = LowerRight.Y;
			LowerRight.Y = xy0.Y;
		}
	}

	// Members -------------------------------
	point_t  UpperLeft;
	point_t  LowerRight;
};

} // end namespace vecg
} // end namespace irr

#endif
