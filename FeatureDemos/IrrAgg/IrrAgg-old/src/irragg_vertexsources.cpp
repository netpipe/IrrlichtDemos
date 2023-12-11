// Copyright 2017 Nicolaus Anderson
// See license.txt for details.

#include "../include/irragg_vertexsources.h"

namespace irr {
namespace vecg {

	//! cstor
	VertexSourceBase::VertexSourceBase()
		: _step(0)
		, _origin_x(0)
		, _origin_y(0)
	{}

	//! cstor
	VertexSourceBase::VertexSourceBase( double  origin_x, double  origin_y )
		: _step(0)
		, _origin_x(origin_x)
		, _origin_y(origin_y)
	{}

	//! Set starting coordinates for the vertex.
	void
	VertexSourceBase::setOrigin( double  x, double  y )
	{
		_origin_x = x;
		_origin_y = y;
	}

	//! Get the initial position
	Point2D
	VertexSourceBase::getOrigin()
	{
		return Point2D(_origin_x, _origin_y);
	}


	//! cstor
	VertexSourceFuncPump::VertexSourceFuncPump(unsigned  num_points, VertexSourceFunction  function)
		: VertexSourceBase()
		, _num_points(num_points)
		, _function(function)
	{}

	//! AGG template requirement: reset
	void
	VertexSourceFuncPump::rewind( unsigned )
	{
		_step = 0;
	}

	//! AGG template requirement: vertex return
	unsigned
	VertexSourceFuncPump::vertex( double*  x, double*  y )
	{
		if ( _step > _num_points )
			return ::agg::path_cmd_stop;

		Point2D  p = _function( _step );
		*x = p.x + _origin_x;
		*y = p.y + _origin_y;
		_step++;

		if ( _step == _num_points )
			return ::agg::path_cmd_end_poly;

		if ( _step == 0 )
			return ::agg::path_cmd_move_to;

		return ::agg::path_cmd_line_to;
	}

} // end sub namespace agg
} // end namespace irr
