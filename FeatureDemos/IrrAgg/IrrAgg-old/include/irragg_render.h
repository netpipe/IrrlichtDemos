// Copyright 2017 Nicolaus Anderson
// see license.txt for details
/*
	This file contains templated functions for rendering raw vertex sources.
*/

#ifndef __IRR_AGG_RENDER_H__
#define __IRR_AGG_RENDER_H__

//*** Anti-Grain Geometry ***
#include <agg_basics.h>
#include <agg_rendering_buffer.h>
#include <agg_rasterizer_scanline_aa.h>
#include <agg_scanline_p.h>
#include <agg_renderer_scanline.h>
// For curves
// Specifically for the curve
#include <agg_conv_stroke.h>
#include <agg_conv_bspline.h>
// For color
#include <agg_pixfmt_rgba.h>

//#include "irragg_includes_color32.h"

//*** Irrlicht ***
#include <IImage.h>

//*** IrrAgg ***
#include "irragg_config.h"
#include "irragg_format_support.h"
#include "irragg_color.h"
#include "irragg_path_stroke.h"


namespace irr {
namespace vecg {

	//! Simple rasterization
	template< class VertexSource, class RendererBase >
	bool rasterizeVectorPathSimple(
		VertexSource&  vertex_source,
		RendererBase&  base,
		irr::video::SColor  color // Could also be abstracted, and use templates to pull out colors
	) {
		::agg::scanline_p8  scanline;
		::agg::rasterizer_scanline_aa<>  rasterizer;

		rasterizer.add_path( vertex_source );
		::agg::render_scanlines_aa_solid(
			rasterizer, scanline, base,
			core::convertSColorToAGGrgba(color)
			);
		return true;
	}


	//! Stroke rasterization
	template< class VertexSource, class RendererBase >
	bool rasterizeVectorPathStroke(
		VertexSource&  vertex_source,
		RendererBase&  base,
		irr::video::SColor  color, // Could also be abstracted, and use templates to pull out colors
		irr::f32  stroke_width
	) {
		::agg::scanline_p8  scanline;
		::agg::rasterizer_scanline_aa<>  rasterizer;

		::agg::conv_stroke< VertexSource >  conv_stroke_vs(vertex_source);
		conv_stroke_vs.width( stroke_width );

		rasterizer.add_path( conv_stroke_vs );
		::agg::render_scanlines_aa_solid(
			rasterizer, scanline, base,
			core::convertSColorToAGGrgba(color)
			);
		return true;
	}


	//! BSpline rasterization
	template< class VertexSource, class RendererBase >
	bool rasterizeVectorPathBSpline(
		VertexSource&  vertex_source,
		RendererBase&  base,
		irr::video::SColor  color, // Could also be abstracted, and use templates to pull out colors
		irr::u32  stroke_width,
		irr::u32  resolution
	) {
		::agg::scanline_p8  scanline;
		::agg::rasterizer_scanline_aa<>  rasterizer;

		typedef ::agg::conv_bspline< VertexSource >  bspline_type;
		bspline_type  vs_bspline(vertex_source);
		vs_bspline.interpolation_step(1.0 / resolution);

		::agg::conv_stroke< bspline_type >  vs_stroke(vs_bspline);
		vs_stroke.width( stroke_width );

		rasterizer.add_path(vs_stroke);
		::agg::render_scanlines_aa_solid(
			rasterizer, scanline, base,
			core::convertSColorToAGGrgba(color)
			);
		return true;
	}


	//! Render a vector path onto a 32-bit color image
	/*
		\param resolution - Sets how smooth the path will be drawn if it is a curve.
	*/
	template< class VertexSource >
	bool renderVectorPathARGB32(
		VertexSource&  vertex_source,
		irr::video::IImage*  target,
		irr::video::SColor  color,
		EPathStroke::Value  path_stroke_type = EPathStroke::NORMAL,
		irr::u32  stroke_width = 1,
		irr::u32  resolution = 20
	) {
		if ( ! isImageARGB32(target) )
			return false;

		irr::u8*  image_data = (irr::u8*)target->getData();
		irr::core::dimension2du  image_size = target->getDimension();

		::agg::rendering_buffer  buffer( image_data, image_size.Width, image_size.Height, target->getPitch() );
		// Note: The choice of ::agg::pixfmt_bgra32 may have to do with endianness of the machine.
#ifdef IRR_AGG_COMPILE_AS_BIG_ENDIAN
		::agg::pixfmt_argb32  format(buffer);
		::agg::renderer_base< ::agg::pixfmt_argb32 >  base(format);
#else
		::agg::pixfmt_bgra32  format(buffer);
		::agg::renderer_base< ::agg::pixfmt_bgra32 >  base(format);
#endif

		switch( path_stroke_type )
		{
		case EPathStroke::NORMAL:
			return rasterizeVectorPathSimple( vertex_source, base, color );

		case EPathStroke::BRUSH:
			return rasterizeVectorPathStroke( vertex_source, base, color, stroke_width );

		case EPathStroke::BSPLINE:
			return rasterizeVectorPathBSpline( vertex_source, base, color, stroke_width, resolution );

		default:
			return false;
		}
	}


	//! Render a vector path
	/*
		\param resolution - Sets how smooth the path will be drawn if it is a curve.
	*/
	template< class VertexSource >
	bool renderVectorPath(
		VertexSource&  vertex_source,
		irr::video::IImage*  target,
		irr::video::SColor  color,
		EPathStroke::Value  path_stroke_type = EPathStroke::NORMAL,
		irr::u32  stroke_width = 1,
		irr::u32  resolution = 20
	) {
		if ( target == IRR_AGG_NULL )
			return false;

		switch( target->getColorFormat() )
		{
		case video::ECF_A8R8G8B8:
			return renderVectorPathARGB32( vertex_source, target, color, path_stroke_type, stroke_width, resolution );

		default:
			return false;
		}
	}


} // end sub namespace vecg
} // end namespace irr

#endif // __IRR_AGG_RENDER_H__
