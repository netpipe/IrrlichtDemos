// Copyright (C) 2008-2012 Benjamin Hampe
// This file uses parts of the "Irrlicht Engine"
// For conditions of distribution and use, see copyright notice in irrlicht.h

#ifndef __IRR_CREATE_RAINBOW_MESH_H__
#define __IRR_CREATE_RAINBOW_MESH_H__

#include <irrlicht.h>

namespace irr
{
	namespace video
	{
		//! calculates SColor from a wavelength in nanometer
		//! only valid in range λ ∈ [380, 780] nm
		//! which are the visible colors for human eyes (blue to red)
		//! uses algorithm from Bruton which is a linear approximation
		//! of the CIE color system, that defines 3 (more complex) functions for the base-colors RGB
		SColor colorFromWavelength(	f64 lambda = 380.0f /*blue end*/, f64 gamma = 1.0);

		//! uses colorFromWaveLength and calculates an amount "count"
		//! of rainbowcolors from λ ∈ [380, 780] nm --> interpolates
		SColor createRainbowColor( u32 i = 0, u32 count = 64, f64 gamma = 1.0);

		//! does not need the driver, but the path to class CImage!!!
		//! the height actually contains all different values, w cols are just copies
		//! but driver may only support spare textures
		IImage* createRainbowImage( u32 w = 4, u32 h = 64, u8 transparency = 127, f64 gamma = 1.0);

		//! if driver 0, it uses the null device
		ITexture* createRainbowTexture( IVideoDriver* driver = 0, u32 w = 64, u32 h = 64, u8 transparency = 127, f64 gamma = 1.0);

	} // END NAMSPACE VIDEO

	namespace scene
	{
		//! arc has by default 180 degrees -> so rainbow shape
		//! origin will be center of arc, meaning the center of a full circle
		SMesh* createArcMesh( // XY-plane, normale is -Z
				f32 outerRadius = 100.0f,
				f32 innerRadius = 50.0f,
				f32 zAngleStart = 0.0f,
				f32 zAngleEnd = 180.0f,
				u32 segments = 13,
				const core::vector3df& pos = core::vector3df(0,0,0));

	} // END NAMSPACE SCENE
} // END NAMSPACE IRR
#endif




