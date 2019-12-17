// PostProcessing framework - Effect
// Created by David Evans, 2009

#ifndef __C_EFFECT_POST_PROC_H_INCLUDED__
#define __C_EFFECT_POST_PROC_H_INCLUDED__

#include "IPostProc.h"

// A random number used to check if no value was given. This should probably be replaced with some special float value.
#define FLOAT_DEFAULTVALUE 7285624.0f

enum POSTPROC_EFFECTID {
	// Effect ID	Parameters		Description
	PP_BLACK = 0u,	// ()			A testing shader - not recommended for use (intentionally not optimized)
	PP_DIRECT,		// ()			Does nothing to the input - useful for anti-aliasing
	PP_DARKEN,		// (mult)		Multiplies rgb by mult and maintains black (0) = black (0)
	PP_LIGHTEN,		// (mult)		Multiplies rgb by mult and maintains white (1) = white (1)
	PP_RANGE,		// (low,high)	Changes contrast so that low -> 0, high -> 1
	PP_INVERT,		// ()			Inverts the rgba channels
	PP_TINT,		// (r,g,b)		Converts the pixels to monochrome using a simple weighting then applies a tint. maintains black = black, white = white
	PP_CURVES,		// (r,g,b)		Applies a colour curve to the rgb channels, maintaining black = black and white = white. Values of 1.0 are no change, > 1.0 raises colour presence
	PP_GREYSCALE,	// (power)		= PP_TINT( power, power, power )
#define PP_GRAYSCALE PP_GREYSCALE
	PP_SEPIA,		// ()			= PP_TINT( 2.0, 1.0, 0.7 )
	PP_NOISE,		// (amount)		Adds psudo-random monochromatic noise to each pixel. Each frame uses different random numbers. Random function is crude.
	PP_COLORNOISE,	// (amount)		As above, but r,g,b channels are seperate
	PP_HBLUR,		// (distance)	Applies a simple horizontal linear blur filter using 5 samples
	PP_VBLUR,		// (distance)	As PP_HBLUR but vertical
	PP_LINEARBLUR,	// (dx,dy)		As PP_HBLUR, but applies along the line dx,dy
	PP_RADIALBLUR,	// (cx,cy,dx,dy)Applies a radial blur from (cx,cy) with a size of dx at (cx+1,cy) and dy at (cx,cy+1)
	PP_HBLURFINE,	// (distance)	Applies a simple horizontal linear blur filter using 11 samples
	PP_VBLURFINE,	// (distance)	As PP_HBLURFINE but vertical
	PP_LINEARBLURFINE,	// (dx,dy)			As PP_LINEARBLUR but with more samples
	PP_RADIALBLURFINE,	// (cx,cy,dx,dy)	As PP_RADIALBLUR but with more samples
	PP_OVERLAY,		// (mult)		Output = Texture1 + Texture2 * mult
	PP_OVERLAYNEG,	// (mult)		Output = Texture1 - (1 - Texture2) * mult  !WARNING: Due to no EMT_TRANSPARENT_SUBTRACT_COLOR option, this uses a SLOW method. Will cause a performance hit if Texture2 is non-static.
	PP_MOTIONBLUR,	// (sharp)		Retains a memory of past renders, low sharp = long trails. Recommended sharp ~= 0.1
	PP_DEPTH,		// ()			Renders depth (alpha) as greyscale, lighter = further away
	PP_OCCLUSION,	// (mult)		Taken from the Irrlicht forums and heavily mutilated, uses alpha channel to judge depth
	// Note that other shaders are careless with the alpha channel so this should be the first effect.
	// You will also need to use special renderers on your scene objects which set the alpha channel to the z position
	
	// Composite effects
	
	PP_BLUR,		// (distance)			= HBLUR(distance) + VBLUR(distance)
	PP_BLURFINE,	// (distance)			= HBLURFINE(distance) + VBLURFINE(distance)
	PP_BLOOM,		// (cut,distance,light)	= LIGHTEN(cut) + BLUR(distance) + OVERLAY(light)
	PP_GLOOM,		// (cut,distance,dark)	= DARKEN(cut) + BLUR(distance) + OVERLAYNEG(dark)  !WARNING: uses OVERLAYNEG which is slow!
	PP_NIGHTVISION,	// (max,distance,noise)	= RANGE(-0.5,max) + BLUR(distance) + NOISE(noise) + TINT(0.2,2.0,0.5)
	
	PP_COUNT // Not an effect, used to count number of effects.
};

class CEffectPostProc : public IPostProc {
private:
	POSTPROC_EFFECTID eff;
	CShaderCallback shadercallback;
	f32 shaderparameters[8];
	E_MATERIAL_TYPE useTextureOptimize;
	SMaterial mat;
	array<CEffectPostProc*> bound;
	void setShader( const c8* prog_gl, const c8* prog_dx, E_PIXEL_SHADER_TYPE type_ps );
	
public:
	CEffectPostProc( IPostProc* follow1, dimension2di sOut, POSTPROC_EFFECTID effectID, f32 para1 = FLOAT_DEFAULTVALUE, f32 para2 = FLOAT_DEFAULTVALUE, f32 para3 = FLOAT_DEFAULTVALUE, f32 para4 = FLOAT_DEFAULTVALUE );
	CEffectPostProc( IPostProc* follow1, dimension2di sOut, const c8* prog_gl, const c8* prog_dx, E_PIXEL_SHADER_TYPE type_ps = EPST_PS_1_1, E_MATERIAL_TYPE optimize = EMT_SOLID, f32 para1 = 0.0f, f32 para2 = 0.0f, f32 para3 = 0.0f, f32 para4 = 0.0f );
	virtual ~CEffectPostProc( );
	virtual void configureInput( u8 n );
	virtual IPostProc* getInput( u8 n = 0 );
	virtual void renderEffect( ITexture* target );
	virtual void enable( bool yes = true );
	void setParameters( f32 para1 = 0.0f, f32 para2 = 0.0f, f32 para3 = 0.0f, f32 para4 = 0.0f, f32 para5 = 0.0f, f32 para6 = 0.0f, f32 para7 = 0.0f, f32 para8 = 0.0f );
};

#endif