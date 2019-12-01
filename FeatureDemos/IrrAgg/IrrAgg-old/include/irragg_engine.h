// Copyright 2017 Nicolaus Anderson
// see license.txt for details
/*
	This file contains self-contained class/classes with all of the necessary components for rendering.
	The benefits are:
	- They may be slightly faster for drawing than the direct rendering functions in irragg_render.h.
	- They may be slightly more convenient.
	- They allow for matrix operations to be performed on the points prior to rendering.
*/

#ifndef __IRR_AGG_ENGINE_H__
#define __IRR_AGG_ENGINE_H__

//*** Anti-Grain Geometry ***
#include <agg_basics.h>
#include <agg_rendering_buffer.h>
#include <agg_rasterizer_scanline_aa.h>
#include <agg_scanline_p.h>
#include <agg_renderer_scanline.h>
// For matrices
#include <agg_trans_affine.h>
// For curves
// Specifically for the curve
#include <agg_conv_stroke.h>
#include <agg_conv_bspline.h>
// For color
#include <agg_pixfmt_rgba.h>

//#include "irragg_includes_color32.h"

//*** Irrlicht ***
#include <matrix4.h>
#include <IImage.h>

//*** IrrAgg ***
#include "irragg_config.h"
#include "irragg_format_support.h"
#include "irragg_color.h"
#include "irragg_path_stroke.h"
//#include "irragg_matrix_vertex_source.h"
#include "irragg_array_path2d.h"

namespace irr {
namespace vecg {
namespace engine {

	// USER NOTE:
	// This engine is not templated and does not allow using the standard AGG vertex rendering method.
	// Users who wish to have custom paths should inherit the class PointSource from irragg_point.h
	// and implement its virtual methods.


	//! Rendering Engine for A8R8G8B8 IImages.
	/*
		This class is used for rendering shapes to an IImage of format A8R8G8B8
		without the need to reconstruct all of the components for rendering every time.

		DEVELOPER NOTE:
		This class may be split into two classes:
			One with the basic methods and one with A8R8G8B8-specific elements and draw methods.
		That way, the order of alpha-red-green-blue can be changed.
		This may also result in the change in the name of the constant IRR_AGG_COMPILE_AS_BIG_ENDIAN.
	*/
	class RendererARGB32
	{
	protected:
		ArrayVertexSource  _point_source;
		video::IImage*  _target;
		bool  _drop_with_destruct;
		::agg::rgba  _draw_color;
		double  _stroke_width;
		//irr::u32  _stroke_resolution;
		double  _stroke_interpolation_step;

		// --- Drawing matrix ---
		bool  _use_drawing_matrix; // Indicates if the drawing matrix should be applied to the points prior to rendering.
		//MatrixVertexSource  _drawing_matrix;
		::agg::trans_affine  _drawing_matrix; // May be replaced

		// --- Anti-Grain rendering mechanisms ---
		::agg::rendering_buffer  _buffer;
		// Note: The choice of ::agg::pixfmt_bgra32 may have to do with endianness of the machine.
#ifdef IRR_AGG_COMPILE_AS_BIG_ENDIAN
		::agg::pixfmt_argb32  _format;
		::agg::renderer_base< ::agg::pixfmt_argb32 >  _base;
#else
		::agg::pixfmt_bgra32  _format;
		::agg::renderer_base< ::agg::pixfmt_bgra32 >  _base;
#endif
		typedef ::agg::conv_bspline< ArrayVertexSource >  bspline_type;

		// --- Precreated features ---
		StaticSizeVertexSource<24>  _circle_vertex_source; // Used for quickly rendering a circle
		bool  _is_circle_ready; // Indicates if the circle was made yet

	public:

		//! cstor
		RendererARGB32();

		//! cstor
		/*
			\param target - The new backbuffer. If it is not of type A8R8G8B8, this function may
				try to convert it. If conversion fails, no back buffer will be set.
			\param destroy_with_destruct - Indicates that the target image should be dropped (possibly
				triggering destruction) when this class undergoes destruction.
		*/
		RendererARGB32(
			video::IImage*  target,
			bool  drop_with_destruct
			);

		//! dstor
		~RendererARGB32();

		//! Set back buffer
		/*
			\param target - The new backbuffer. If it is not of type A8R8G8B8, this function may
				try to convert it. If conversion fails, no back buffer will be set.
			\param destroy_with_destruct - Indicates that the target image should be dropped (possibly
				triggering destruction) when this class undergoes destruction.
			\return true if the back buffer was set.
		*/
		bool
			setRenderTarget(
				video::IImage*  target,
				bool  drop_with_destruct
				);

		//! Set rendering color
		void
			setDrawingColor( video::SColor  color );

		//! Set rendering color
		void
			setDrawingColor( ::agg::rgba  color );

		//! Set stroke width
		/*
			Used for drawing curves and outlines.
		*/
		void
			setStrokeWidth( double  stroke_width );

		//! Set spline resolution
		/*
			Sets the smoothing for rendering b-splines.
		*/
		void
			setBSplineResolution( irr::u32  resolution );

		//! Enable using the drawing matrix
		void
			setDrawingMatrixEnabled( bool  yes );

		//! Set drawing matrix
		/*
			Set the matrix used for modifying points before drawing them.
			You should call this function prior to buffering points.
			This version of this method uses a native Irrlicht matrix.
			This sets both the rotation/scale/skew part and the translation.
		*/
		void
			setDrawingMatrixFull( const core::CMatrix4<double>&  drawing_matrix );

		//! Set drawing matrix
		/*
			Set the matrix used for modifying points before drawing them.
			You should call this function prior to buffering points.
			This version of this method uses a native AGG matrix.
			This sets both the rotation/scale/skew part and the translation.
		*/
		void
			setDrawingMatrixFull( const agg::trans_affine&  drawing_matrix );

		//! Set drawing matrix rotation, scaling, and shear
		/*
			Set the matrix used for modifying points before drawing them.
			You should call this function prior to buffering points.
			The parameters of this version represent the components of the
			rotation and scale of the matrix, and modify the matrix as though
			it were the following matrix being multiplied by the position vector
			of the point:

			[s00 | s10]   [x]   [s00*x + s10*y]
			[s01 | s11] * [y] = [s01*x + s11*y]
		*/
		void
			setDrawingMatrix(
				double  s00,
				double  s10,
				double  s01,
				double  s11
			);

		//! Set drawing matrix translation
		/*
			Set the translation that is applied to the points being buffered.
		*/
		void
			setDrawingMatrixTranslation( double  x, double  y );

		//! Set the complete drawing matrix
		/*
			Set the matrix used for modifying the points before drawing.
			This is a combination of setDrawingMatrix(double, double, double, double)
				and setDrawingMatrixTranslation(double, double). In other words,
				it sets both the rotation/scale/skew part and the translation.
			See documentation of setDrawingMatrix(double,double,double,double) for
				more information.
		*/
		void
			setDrawingMatrixFull(
				double  s00,
				double  s10,
				double  s01,
				double  s11,
				double  tx,
				double  ty
			);

		//! Buffer points to draw
		/*
			Populate the vertex buffer with points from the given source.
			The points will undergo matrix transformation here.
			You should use the bufferPoints methods if you do not intend to use
				the shape-named drawing methods (drawTriangle, drawCircle, etc.)
			\return - false if the source is an invalid pointer.
		*/
		bool
			bufferPoints( PointSource*  source );

		//! Buffer points to draw
		/*
			Populates the vertex buffer with points from the given source.
			The points will undergo matrix transformation here.
			You should use the bufferPoints methods if you do not intend to use
				the shape-named drawing methods (drawTriangle, drawCircle, etc.)
			\return - true.
				Required return type for method prototype matching with other versions.
		*/
		bool
			bufferPoints( ArrayVertexSource&  source );


		//----- Shape Drawing -----

		// DEVELOPER NOTE: When drawing, check for non-null target.

		//! Draw the last-buffered path
		bool
			draw(
				EPathStroke::Value  path_stroke_type = EPathStroke::NORMAL
			);

		//! Draw the last-buffered path as a simple path
		bool
			drawNormal();

		//! Draw the last-buffered path as a stroked path
		bool
			drawStroked();

		//! Draw the last-buffered path as a B-Spline path
		bool
			drawBSpline();

		//! Draw a source of points
		bool
			drawPointSource(
				PointSource*  source,
				EPathStroke::Value  path_stroke_type = EPathStroke::NORMAL
			);

		//! Draw an array of points
		bool
			drawArrayVertexSource(
				ArrayVertexSource&  source,
				EPathStroke::Value  path_stroke_type = EPathStroke::NORMAL
			);

		//! Draw a triangle
		bool
			drawTriangle(
				const core::vector2d<double>&  point_a,
				const core::vector2d<double>&  point_b,
				const core::vector2d<double>&  point_c,
				EPathStroke::Value  path_stroke_type = EPathStroke::NORMAL
			);

		//! Draw a rectangle
		bool
			drawRectangle(
				const core::vector2d<double>&  upper_left_corner,
				const core::vector2d<double>&  lower_right_corner,
				EPathStroke::Value  path_stroke_type = EPathStroke::NORMAL
			);

		//! Draw circle
		bool
			drawCircle(
				double  radius,
				const core::vector2d<double>&  center = core::vector2d<double>(0),
				EPathStroke::Value  path_stroke_type = EPathStroke::NORMAL
			);


	protected:

		//! Apply the drawing matrix to the given point
		Point2D
			applyDrawingMatrix( const Point2D&  point );

		//! Pre-create circle
		void
			prepareCircle();

	};


} // end sub namespace engine
} // end sub namespace vecg
} // end namespace irr

#endif // __IRR_AGG_ENGINE_H__
