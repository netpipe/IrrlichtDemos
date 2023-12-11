// Copyright 2017 Nicolaus Anderson
// see license.txt for details
/*
	This file contains a class/classes for quickly rendering onto a buffer.
	Some draw functions are faster than using the direct rendering functions in irragg_render.h
		and also allow for matrices.
*/

//*** Standard Library ***
#include <math.h>

//*** IrrAgg ***
#include "../include/irragg_engine.h"
#include "../include/irragg_color.h"

namespace irr {
namespace vecg {
namespace engine {

	//! cstor
	RendererARGB32::RendererARGB32()
		: _point_source()
		, _target(IRR_AGG_NULL)
		, _drop_with_destruct(false)
		, _draw_color()
		, _stroke_width(1)
		, _stroke_interpolation_step(0.05)
		, _use_drawing_matrix(false)
		, _drawing_matrix()
		, _buffer()
		, _format()
		, _base()
		, _circle_vertex_source()
		, _is_circle_ready(false)
	{
	}

	//! cstor
	RendererARGB32::RendererARGB32(
		video::IImage*  target,
		bool  drop_with_destruct
	)
		: _point_source()
		, _target(IRR_AGG_NULL)
		, _drop_with_destruct(false)
		, _draw_color()
		, _stroke_width(1)
		, _stroke_interpolation_step(0.05)
		, _use_drawing_matrix(false)
		, _drawing_matrix()
		, _buffer()
		, _format()
		, _base()
		, _is_circle_ready(false)
	{
		setRenderTarget(target, drop_with_destruct);
	}

	//! dstor
	RendererARGB32::~RendererARGB32()
	{
		if ( _drop_with_destruct && _target )
			_target->drop();
	}

	//! Set back buffer
	bool
	RendererARGB32::setRenderTarget(
		video::IImage*  target,
		bool  drop_with_destruct
	) {
		if ( target == IRR_AGG_NULL || target->getColorFormat() != ::irr::video::ECF_A8R8G8B8 )
		{
			// Reset the target here
			_target = IRR_AGG_NULL;
			_drop_with_destruct = false;
			return false;
		}

		_target = target;
		_drop_with_destruct = drop_with_destruct;

		irr::u8*  image_data = (irr::u8*)target->getData();
		irr::core::dimension2du  image_size = target->getDimension();

		_buffer.attach( image_data, image_size.Width, image_size.Height, _target->getPitch() );
		_format.attach(_buffer);
		_base.attach(_format);

		return true;
	}

	//! Set rendering color
	void
	RendererARGB32::setDrawingColor(
		video::SColor  color
	) {
		_draw_color = core::convertSColorToAGGrgba(color);
	}

	//! Set rendering color
	void
	RendererARGB32::setDrawingColor(
		::agg::rgba  color
	) {
		_draw_color = color;
	}

	//! Set stroke width
	void
	RendererARGB32::setStrokeWidth(
		double  stroke_width
	) {
		_stroke_width = stroke_width < 0 ? -stroke_width : stroke_width;
	}

	//! Set spline resolution
	void
	RendererARGB32::setBSplineResolution(
		irr::u32  resolution
	) {
		//_stroke_resolution = resolution;
		_stroke_interpolation_step = 1.0 / (double) resolution;
	}

	//! Enable using the drawing matrix
	void
	RendererARGB32::setDrawingMatrixEnabled(
		bool  yes
	) {
		_use_drawing_matrix = yes;
	}

	//! Set drawing matrix
	void
	RendererARGB32::setDrawingMatrixFull(
		const core::CMatrix4<double>&  drawing_matrix
	) {
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
		_drawing_matrix.sx = drawing_matrix[0];
		_drawing_matrix.shy = drawing_matrix[1];
		_drawing_matrix.shx = drawing_matrix[4];
		_drawing_matrix.sy = drawing_matrix[5];
		_drawing_matrix.tx = drawing_matrix[12];
		_drawing_matrix.ty = drawing_matrix[13];
	}

	//! Set drawing matrix
	void
	RendererARGB32::setDrawingMatrixFull(
		const agg::trans_affine&  drawing_matrix
	) {
		_drawing_matrix = drawing_matrix;
	}

	//! Set drawing matrix rotation, scaling, and shear
	void
	RendererARGB32::setDrawingMatrix(
			double  s00,
			double  s10,
			double  s01,
			double  s11
	) {
		_drawing_matrix.sx = s00;
		_drawing_matrix.shx = s10;
		_drawing_matrix.shy = s01;
		_drawing_matrix.sy = s11;
	}

	//! Set drawing matrix translation
	void
	RendererARGB32::setDrawingMatrixTranslation(
		double  x,
		double  y
	) {
		_drawing_matrix.tx = x;
		_drawing_matrix.ty = y;
	}

	//! Set the complete drawing matrix
	void
	RendererARGB32::setDrawingMatrixFull(
			double  s00,
			double  s10,
			double  s01,
			double  s11,
			double  tx,
			double  ty
	) {
		_drawing_matrix.sx = s00;
		_drawing_matrix.shx = s10;
		_drawing_matrix.shy = s01;
		_drawing_matrix.sy = s11;
		_drawing_matrix.tx = tx;
		_drawing_matrix.ty = ty;
	}

	//! Buffer points to draw
	bool
	RendererARGB32::bufferPoints(
		PointSource*  source
	) {
		if (!source)
			return false;

		_point_source.clear();
		size_t  point_count = source->getPointCount();
		size_t  p = 0;
		_point_source.prepare( point_count );

		for (; p < point_count; ++p )
		{
			_point_source.add(source->getPoint(p));
		}
		return true;
	}

	//! Buffer points to draw
	bool
	RendererARGB32::bufferPoints(
		ArrayVertexSource&  source
	) {
		_point_source.clear();
		size_t  point_count = source.getPointCount();
		size_t  p = 0;
		_point_source.prepare( point_count );

		for (; p < point_count; ++p )
		{
			_point_source.add(source.getPoint(p));
		}
		return true;
	}

	//! Draw the last-buffered path
	bool
	RendererARGB32::draw(
			EPathStroke::Value  path_stroke_type
		)
	{
		switch( path_stroke_type )
		{
		case EPathStroke::NORMAL:
			return drawNormal();

		case EPathStroke::BRUSH:
			return drawStroked();

		case EPathStroke::BSPLINE:
			return drawBSpline();

		default:
			return false;
		}
	}

	//! Draw a simple path
	bool
	RendererARGB32::drawNormal()
	{
		if ( _target == IRR_AGG_NULL )
			return false;

		::agg::scanline_p8  scanline;
		::agg::rasterizer_scanline_aa<>  rasterizer;

		rasterizer.add_path(_point_source);
		::agg::render_scanlines_aa_solid(
			rasterizer, scanline, _base, _draw_color
			);
		return true;
	}

	//! Draw a stroked path
	bool
	RendererARGB32::drawStroked()
	{
		if ( _target == IRR_AGG_NULL )
			return false;

		::agg::scanline_p8  scanline;
		::agg::rasterizer_scanline_aa<>  rasterizer;

		::agg::conv_stroke< ArrayVertexSource >  conv_stroke_vs(_point_source);
		conv_stroke_vs.width( _stroke_width );

		rasterizer.add_path( conv_stroke_vs );
		::agg::render_scanlines_aa_solid(
			rasterizer, scanline, _base, _draw_color
			);
		return true;
	}

	//! Draw a B-Spline path
	bool
	RendererARGB32::drawBSpline()
	{
		if ( _target == IRR_AGG_NULL )
			return false;

		::agg::scanline_p8  scanline;
		::agg::rasterizer_scanline_aa<>  rasterizer;

		bspline_type  vs_bspline(_point_source);
		vs_bspline.interpolation_step(_stroke_interpolation_step);

		::agg::conv_stroke< bspline_type >  vs_stroke(vs_bspline);
		vs_stroke.width( _stroke_width );

		rasterizer.add_path(vs_stroke);
		::agg::render_scanlines_aa_solid(
			rasterizer, scanline, _base, _draw_color
			);
		return true;
	}

	//! Draw a point source
	bool
	RendererARGB32::drawPointSource(
			PointSource*  source,
			EPathStroke::Value  path_stroke_type
		)
	{
		if ( bufferPoints(source) )
			return draw( path_stroke_type );

		return false;
	}

	//! Draw an array of points
	bool
	RendererARGB32::drawArrayVertexSource(
			ArrayVertexSource&  source,
			EPathStroke::Value  path_stroke_type
		)
	{
		if ( bufferPoints(source) )
			return draw( path_stroke_type );

		return false;
	}

	//! Draw a triangle
	bool
	RendererARGB32::drawTriangle(
		const core::vector2d<double>&  point_a,
		const core::vector2d<double>&  point_b,
		const core::vector2d<double>&  point_c,
		EPathStroke::Value  path_stroke_type
		)
	{
		_point_source.clear();
		_point_source.prepare(3);
		// Should modify with matrix here
		_point_source.add( applyDrawingMatrix(Point2D(point_a)) );
		_point_source.add( applyDrawingMatrix(Point2D(point_b)) );
		_point_source.add( applyDrawingMatrix(Point2D(point_c)) );
		return draw(path_stroke_type);
	}

	//! Draw a rectangle
	bool
	RendererARGB32::drawRectangle(
		const core::vector2d<double>&  upper_left_corner,
		const core::vector2d<double>&  lower_right_corner,
		EPathStroke::Value  path_stroke_type
	) {
		// Create the rectangle
		_point_source.clear();
		_point_source.prepare(5);
		_point_source.add( applyDrawingMatrix(Point2D( upper_left_corner )) );
		_point_source.add( applyDrawingMatrix(Point2D( lower_right_corner.X, upper_left_corner.Y )) );
		_point_source.add( applyDrawingMatrix(Point2D( lower_right_corner )) );
		_point_source.add( applyDrawingMatrix(Point2D( upper_left_corner.X, lower_right_corner.Y )) );
		_point_source.add( applyDrawingMatrix(Point2D( upper_left_corner )) );
		return draw(path_stroke_type);
	}

	//! Draw circle
	bool
	RendererARGB32::drawCircle(
		double  radius,
		const core::vector2d<double>&  center,
		EPathStroke::Value  path_stroke_type
	) {
		// Create the circle if necessary
		if ( !_is_circle_ready )
			prepareCircle();

		const size_t  num_points = _circle_vertex_source.getPointCount();
		size_t  step = 0;

		_point_source.clear();
		_point_source.prepare( num_points );

		for (; step < num_points; ++step)
		{
			const Point2D  p = _circle_vertex_source.getPoint(step);
			_point_source.add(
				applyDrawingMatrix(
					Point2D(p.x * radius + center.X, p.y * radius + center.Y)
				)
			);
		}
		// Closing point
		const Point2D  p = _circle_vertex_source.getPoint(0);
		_point_source.add(
			applyDrawingMatrix(
				Point2D(p.x * radius + center.X, p.y * radius + center.Y)
			)
		);
		return draw( path_stroke_type );
	}


	//! Apply the drawing matrix to the given point
	Point2D
	RendererARGB32::applyDrawingMatrix( const Point2D&  point )
	{
		Point2D out = point;
		if ( _use_drawing_matrix )
			_drawing_matrix.transform(&out.x, &out.y);
		return out;
	}


	//! Pre-create circle
	void
	RendererARGB32::prepareCircle()
	{
		const size_t  size = _circle_vertex_source.getPointCount();
		// The circle is divided into the number of segments given by the size,
		// however, it can be rendered at a smoother resolution by rendering it as a bezier curve.
		const double  segment_rads = 3.1415926535897932384626433832795028841971693993751 * 2/ size;
		size_t  step = 0;

		for (; step < size; ++step)
		{
			const double  x = cos(segment_rads * step);
			const double  y = sin(segment_rads * step);
			_circle_vertex_source.set(step, x, y);
		}
		_is_circle_ready = true;
	}



} // end sub namespace engine
} // end sub namespace vecg
} // end namespace irr
