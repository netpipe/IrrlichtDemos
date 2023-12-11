// Copyright 2019 Nicolaus Anderson
// See license.txt for details.

#ifndef __IRR_AGG_COLOR_H__
#define __IRR_AGG_COLOR_H__

// Anti-Grain Geometry ------
#include <agg_color_rgba.h>
//#include <agg_color_gray.h> // Eventually

// Irrlicht -----------------
#include <SColor.h>


namespace irr {
namespace core {

	//! Convert (Irrlicht) SColor to (AGG) rgba
	inline ::agg::rgba
		convertSColorToAGGrgba(
			const ::irr::video::SColor&  color
			)
	{
		return ::agg::rgba(
			(double)color.getRed()/255,
			(double)color.getGreen()/255,
			(double)color.getBlue()/255,
			(double)color.getAlpha()/255
		);
	}

	//! Convert (AGG) rgba to (Irrlicht) SColor
	inline ::irr::video::SColor
		convertAGGrgbaToSColor(
			const ::agg::rgba&  color
		)
	{
		return ::irr::video::SColor(
			(irr::u32)(color.a * 255),
			(irr::u32)(color.r * 255),
			(irr::u32)(color.g * 255),
			(irr::u32)(color.b * 255)
		);
	}

	//! Convert (Irrlicht) SColor to (AGG) rgba
	inline ::agg::rgba
		convertSColorfToAGGrgba(
			const ::irr::video::SColorf&  color
			)
	{
		return ::agg::rgba(
			(double)color.r,
			(double)color.g,
			(double)color.b,
			(double)color.a
		);
	}

	//! Convert (AGG) rgba to (Irrlicht) SColorf
	inline ::irr::video::SColorf
		convertAGGrgbaToSColorf(
			const ::agg::rgba&  color
		)
	{
		return ::irr::video::SColorf(
			(irr::f32)color.r,
			(irr::f32)color.g,
			(irr::f32)color.b,
			(irr::f32)color.a
		);
	}

	//! Convert (Irrlicht) SColorHSL to (Irrlicht) SColor
	inline ::irr::video::SColor
		convertSColorHSLtoSColor(
			const ::irr::video::SColorHSL&  color
		)
	{
		::irr::video::SColorf  f_color;
		color.toRGB(f_color);
		return f_color.toSColor();
	}


	//! Convert (Irrlicht) SColor to (Irrlicht) SColorHSL
	inline ::irr::video::SColorHSL
		convertSColorToSColorHSL(
			const ::irr::video::SColor&  color
		)
	{
		::irr::video::SColorf  f_color(color);
		::irr::video::SColorHSL  hsl_color;
		hsl_color.fromRGB(f_color);
		return hsl_color;
	}


} // end namespace core
} // end namespace irr


#endif // __IRR_AGG_COLOR_H__
