// (c) 2019 Nicolaus Anderson
// See license.txt for details.

#ifndef IRRAGG_ARRAY_H
#define IRRAGG_ARRAY_H

#include <irrArray.h>
#include "../irragg_shape.h"

namespace irr {
namespace vecg {

//! Temporary Array Wrapper
/*
	A shape meant for wrapping an array and being passed directly to a Painter.
	NOTE: It does not copy the array it is given because it is meant for transient usage.
	Example Usage: (C++11)
	[code]
		point_t  points[] = { point_t(0,0), point_t(1,3), point_t(2,0) };
		painter.drawNormal( GhostArray{ points, 3 } );
	[/code]
*/
struct GhostArray
	: public IShape
{
	//! Constructor
	GhostArray( const point_t*  points, size_t  size )
		: Points(points)
		, Size(size)
	{}

	//! Constructor
	GhostArray( core::array<point_t>&  points )
		: Points( points.pointer() )
		, Size( points.size() )
	{}

	//! Return how the polygon must be drawn (indices start at zero)
	virtual PathMotion  getPathMotion( size_t  index ) const {
		if ( index >= Size )
			return PathMotion();

		point_t  p = Points[index];
		return PathMotion(p.X, p.Y);
	}

	//! Return the number of points in the polygon
	virtual size_t  getPathStepCount() const {
		return Size;
	};

protected:

	const point_t*	Points;
	size_t			Size;
};

} // end namespace vecg
} // end namespace irr

#endif
