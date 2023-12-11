// Copyright 2017 Nicolaus Anderson
// See license.txt for details.

#ifndef __IRR_AGG_SHAPES_H__
#define __IRR_AGG_SHAPES_H__

//*** Anti-Grain Geometry ***

//*** Irrlicht ***
#include <vector2d.h>

//*** IrrAgg ***
#include "irragg_path_stroke.h"
#include "irragg_vertexsources.h"


namespace irr {

namespace video {

	class IImage;

}

namespace vecg {

	//! Basic triangle path
	struct TrianglePath
		: public VertexSourceBase
	{
		irr::core::vector2d<double>  point_a;
		irr::core::vector2d<double>  point_b;
		irr::core::vector2d<double>  point_c;


		//! cstor
		TrianglePath();

		//! cstor
		TrianglePath(
			irr::core::vector2d<double>  a,
			irr::core::vector2d<double>  b,
			irr::core::vector2d<double>  c
		);
		
		//! AGG template requirement: reset
		void
			rewind( unsigned );

		//! AGG template requirement: vertex return
		unsigned
			vertex( double*  x, double*  y );
	};


	//! Multi-sided Polygon
	// Use structures in irragg_array_path2d.h


	//**** Useful Functions ****

	//! Render a triangle
	/*
		Creates and renders to the target a triangle defined by the given x and y values.
	*/
	bool  renderTriangle(
			irr::video::IImage*  target,
			irr::f64  x1, // point 1
			irr::f64  y1,
			irr::f64  x2, // point 2
			irr::f64  y2,
			irr::f64  x3, // point 3
			irr::f64  y3,
			irr::video::SColor  color,
			EPathStroke::Value  path_stroke_type = EPathStroke::NORMAL,
			irr::u32  stroke_width = 1,
			irr::u32  resolution = 20
			);

	//! Render an ellipse
	/*
		Creates and renders to the target an ellipse with the given parameters for origin and size.
		See documentation: "Creating Shapes" for more information.
		For rendering a persistent ellipse, use agg::ellipse and renderVectorPath.
	*/
	bool  renderEllipse(
			irr::video::IImage*  target,
			double  x,
			double  y,
			double  altitude,
			double  radius,
			irr::video::SColor  color,
			EPathStroke::Value  path_stroke_type = EPathStroke::NORMAL,
			irr::u32  stroke_width = 1,
			irr::u32  resolution = 20
			);

	//! Render a round-cornered rectangle
	/*
		Creates and renders to the target a rectangle with corners rounded off with the given radius.
	*/
	bool  renderRoundedRectangle(
			irr::video::IImage*  target,
			double  x1,
			double  y1,
			double  x2,
			double  y2,
			double  radius,
			irr::video::SColor  color,
			EPathStroke::Value  path_stroke_type = EPathStroke::NORMAL,
			irr::u32  stroke_width = 1,
			irr::u32  resolution = 20
			);

	//! Render a round-cornered rectangle
	/*
		Creates and renders to the target a rectangle with corners rounded off with given radii.
	*/
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
			EPathStroke::Value  path_stroke_type = EPathStroke::NORMAL,
			irr::u32  stroke_width = 1,
			irr::u32  resolution = 20
			);

	//! Render a round-cornered rectangle
	/*
		Creates and renders to the target a rectangle with corners rounded off with the given radius.
	*/
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
			EPathStroke::Value  path_stroke_type = EPathStroke::NORMAL,
			irr::u32  stroke_width = 1,
			irr::u32  resolution = 20
			);


} // end sub namespace agg
} // end namespace irr

#endif // __IRR_AGG_SHAPES_H__
