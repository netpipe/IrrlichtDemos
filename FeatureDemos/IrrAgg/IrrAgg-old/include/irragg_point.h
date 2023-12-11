// Copyright 2017 Nicolaus Anderson
// See license.txt for details.

#ifndef _IRR_AGG_POINT_H_
#define _IRR_AGG_POINT_H_

//*** Irrlicht ***
#include <vector2d.h>

//*** IrrAgg ***
#include "irragg_config.h"

namespace irr {
namespace vecg {

	struct Point2D
	{
		double  x;
		double  y;

		//! cstor
		Point2D()
			: x(0)
			, y(0)
		{}

		//! cstor
		Point2D( const Point2D&  other )
			: x(other.x)
			, y(other.y)
		{}

		//! cstor
		Point2D( double  x, double  y )
			: x(x)
			, y(y)
		{}

		//! cstor
		Point2D( const ::irr::core::vector2d<double>&  other )
			: x(other.X)
			, y(other.Y)
		{}
	};


	//! Point source
	/*
		This is meant to be inherited by classes that are to be drawn using the rendering engines
		in irragg_engine.h.
		The user should implement both getPointCount and getPoint.
	*/
	struct PointSource
	{
		~PointSource() {}

		//! Get the number of points in this path
		virtual size_t getPointCount() const
		{
			return 0;
		}

		//! Get the point at the given step
		virtual Point2D getPoint( size_t  step )
		{
			return Point2D();
		}
	};


} // end namespace vecg
} // end namespace irr

#endif
