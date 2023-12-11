// Copyright 2017 Nicolaus Anderson
// see license.txt for details

//*** Anti-Grain Geometry ***
#include <agg_basics.h>
#include <agg_ellipse.h>
#include <agg_rounded_rect.h>

//*** Irrlicht ***
#include <vector2d.h>
#include <IImage.h>

//*** IrrAgg ***
#include "../include/irragg_shapes.h"
#include "../include/irragg_render.h"


namespace irr {
namespace vecg {


	//! cstor
	TrianglePath::TrianglePath()
		: point_a()
		, point_b()
		, point_c()
	{}

	//! cstor
	TrianglePath::TrianglePath(
		irr::core::vector2d<double>  a,
		irr::core::vector2d<double>  b,
		irr::core::vector2d<double>  c
	)
		: point_a(a)
		, point_b(b)
		, point_c(c)
	{}

	//! AGG template requirement: reset
	void
	TrianglePath::rewind( unsigned )
	{
		_step = 0;
	}

	//! AGG template requirement: vertex return
	unsigned
	TrianglePath::vertex( double*  x, double*  y )
	{
		switch( _step )
		{
		case 0:
			_step++;
			*x = point_a.X + _origin_x;
			*y = point_a.Y + _origin_y;
			return ::agg::path_cmd_move_to;

		case 1:
			_step++;
			*x = point_b.X + _origin_x;
			*y = point_b.Y + _origin_y;
			return ::agg::path_cmd_line_to;

		case 2:
			_step++;
			*x = point_c.X + _origin_x;
			*y = point_c.Y + _origin_y;
			return ::agg::path_cmd_line_to;

		case 3:
			_step++;
			*x = point_a.X + _origin_x;
			*y = point_a.Y + _origin_y;
			return ::agg::path_cmd_line_to;

		default: break;
		}
		return ::agg::path_cmd_stop;
	}


	//!
	bool  renderTriangle(
			irr::video::IImage*  target,
			irr::f64  x1, // point 1
			irr::f64  y1,
			irr::f64  x2, // point 2
			irr::f64  y2,
			irr::f64  x3, // point 3
			irr::f64  y3,
			irr::video::SColor  color,
			EPathStroke::Value  path_stroke_type,
			irr::u32  stroke_width,
			irr::u32  resolution
			)
	{
		TrianglePath  path(
			irr::core::vector2d<double>(x1,y1),
			irr::core::vector2d<double>(x2,y2),
			irr::core::vector2d<double>(x3,y3)
		);

		return renderVectorPath( path, target, color, path_stroke_type, stroke_width, resolution );
	}


	//!
	bool  renderEllipse(
			irr::video::IImage*  target,
			double  x,
			double  y,
			double  altitude,
			double  radius,
			irr::video::SColor  color,
			EPathStroke::Value  path_stroke_type,
			irr::u32  stroke_width,
			irr::u32  resolution
			)
	{
		::agg::ellipse  path;
		path.init(x, y, altitude, radius);

		return renderVectorPath( path, target, color, path_stroke_type, stroke_width, resolution );
	}


	//!
	bool  renderRoundedRectangle(
			irr::video::IImage*  target,
			double  x1,
			double  y1,
			double  x2,
			double  y2,
			double  radius,
			irr::video::SColor  color,
			EPathStroke::Value  path_stroke_type,
			irr::u32  stroke_width,
			irr::u32  resolution
			)
	{
		::agg::rounded_rect  path(x1,y1,x2,y2,radius);

		return renderVectorPath( path, target, color, path_stroke_type, stroke_width, resolution );
	}


	//!
	bool  renderRoundedRectangle(
			irr::video::IImage*  target,
			double  x1,
			double  y1,
			double  x2,
			double  y2,
			double  radius_x_bottom,
			double  radius_y_bottom,
			double  radius_x_top,
			double  radius_y_top,
			irr::video::SColor  color,
			EPathStroke::Value  path_stroke_type,
			irr::u32  stroke_width,
			irr::u32  resolution
			)
	{
		::agg::rounded_rect  path;
		path.rect(x1,y1,x2,y2);
		path.radius( radius_x_bottom, radius_y_bottom, radius_x_top, radius_y_top );

		return renderVectorPath( path, target, color, path_stroke_type, stroke_width, resolution );
	}


	//! Render a round-cornered rectangle
	bool  renderRoundedRectangle(
			irr::video::IImage*  target,
			double  x1,
			double  y1,
			double  x2,
			double  y2,
			double  radius_upper_left_x,
			double  radius_upper_left_y,
			double  radius_upper_right_x,
			double  radius_upper_right_y,
			double  radius_lower_left_x,
			double  radius_lower_left_y,
			double  radius_lower_right_x,
			double  radius_lower_right_y,
			irr::video::SColor  color,
			EPathStroke::Value  path_stroke_type,
			irr::u32  stroke_width,
			irr::u32  resolution
			)
	{
		::agg::rounded_rect  path;
		path.rect(x1,y1,x2,y2);
		path.radius(
			radius_upper_left_x,
			radius_upper_left_y,
			radius_upper_right_x,
			radius_upper_right_y,
			radius_lower_right_x,
			radius_lower_right_y,
			radius_lower_left_x,
			radius_lower_left_y
		);

		return renderVectorPath( path, target, color, path_stroke_type, stroke_width, resolution );
	}


} // end sub namespace agg
} // end namespace irr
