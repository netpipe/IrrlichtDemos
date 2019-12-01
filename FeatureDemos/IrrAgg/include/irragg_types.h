// (C) 2019 Nicolaus Anderson
// See license.txt for details.

#ifndef IRRAGG_TYPES_H
#define IRRAGG_TYPES_H

#include <agg_basics.h>
#include <vector2d.h>

namespace irr {
namespace vecg {

//! Compiler option
//#define IRR_AGG_COMPILE_AS_BIG_ENDIAN

//! Internally used point
typedef  core::vector2d<double>  point_t;

//! Convert vector2df to point_t
inline  point_t
toPoint_t( const core::vector2df  v ) {
	return  point_t( (double)v.X, (double)v.Y );
}

//! Path Motion
/*
	Indicators for how to create a path.
*/
struct PathMotion
{
	static const unsigned  LINE_TO = ::agg::path_cmd_line_to;
	static const unsigned  MOVE_TO = ::agg::path_cmd_move_to;
	static const unsigned  END_POLY = ::agg::path_cmd_end_poly;
	static const unsigned  STOP = ::agg::path_cmd_stop;

	unsigned Motion;
	double X;
	double Y;

	PathMotion()
		: Motion(LINE_TO)
		, X(0)
		, Y(0)
	{}

	PathMotion(double  x, double  y, unsigned  motion = LINE_TO)
		: Motion(motion)
		, X(x)
		, Y(y)
	{}
};

} // end namespace vecg
} // end namespace irr

#endif
