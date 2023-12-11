// PostProcessing framework - Effect
// Created by David Evans, 2009

#ifndef __C_EFFECT_POST_PROC_H_INCLUDED__
#define __C_EFFECT_POST_PROC_H_INCLUDED__

#include "IPostProc.h"

// A random number used to check if no value was given. This should probably be replaced with some special float value.
#define FLOAT_DEFAULTVALUE 7285624.0f

enum POSTPROC_FLAGS {
	PPF_DEFAULT = 0u,
	
	PPF_FROMCODE = 0u,
	PPF_FROMFILE = 1u
};
enum POSTPROC_EFFECTID {
	// Effect ID	Parameters		Description
	PP_CUSTOM = 0u,	// ()			Do not use - used internally
	PP_DIRECT,		// ()			Does nothing to the input - useful for anti-aliasing
	PP_PUNCH,		// (dx,dy,cx,cy)Applies a punch effect to the input, centred at (cx,cy) with strength (dx,dy)
	PP_PIXELATE,	// (w,h)		Pixellates the input into w x h sized chunks (units in the range 0-1) Note: this does NOT use full antialiasing - only the centre pixel of each block is sampled.
#define PP_PIXELLATE PP_PIXELATE
	PP_PIXELATEBANDS,	// (w,h,m)	As PP_PIXELATE, but also darkens every other row (multiplies colour by m)
#define PP_PIXELLATEBANDS PP_PIXELATEBANDS
	PP_DARKEN,		// (mult)		Multiplies rgb by mult and maintains black (0) = black (0)
	PP_LIGHTEN,		// (mult)		Multiplies rgb by mult and maintains white (1) = white (1)
	PP_RANGE,		// (low,high)	Changes contrast so that low -> 0, high -> 1
	PP_POSTERIZE,	// (levels)		Reduces the colours by "rounding" them to the levels. i.e. levels = 2 means each channel is either 0.0 or 1.0. levels = 3 means 0.0, 0.5 or 1.0, etc.
	PP_INVERT,		// ()			Inverts the rgba channels
	PP_TINT,		// (r,g,b,m)	Converts the pixels to monochrome using a simple weighting then applies a tint. maintains black = black, white = white. Finally merges with original (m=0 -> no change, m=1 -> full tint)
	PP_CURVES,		// (r,g,b)		Applies a colour curve to the rgb channels, maintaining black = black and white = white. Values of 1.0 are no change, > 1.0 raises colour presence
	PP_GREYSCALE,	// (power)		= PP_TINT( power, power, power, 1.0 )
#define PP_GRAYSCALE PP_GREYSCALE
	PP_SEPIA,		// ()			= PP_TINT( 2.0, 1.0, 0.7, 1.0 )
	PP_SATURATE,	// (amount)		= PP_TINT( 1.0, 1.0, 1.0, 1.0-amount ) 1.0 = no change, > 1.0 = saturate, < 1.0 = desaturate. Negative values will invert the colours, but not the luminosiry, can make interesting effects
	PP_VIGNETTE,	// (power,start,end)Applies a black vignette around the input. Set power to 2 for a standard circle, or a higher value for a more rectangular shape. Lower values will make star-like patterns.
	PP_NOISE,		// (amount)		Adds psudo-random monochromatic noise to each pixel. Each frame uses different random numbers. Random function is crude.
	PP_COLORNOISE,	// (amount)		As above, but r,g,b channels are seperate
	PP_PURENOISE,	// (amount)		As PP_NOISE, but ignores input. Renders as if on a grey background. Useful as a generator
	PP_HBLUR,		// (distance)	Applies a simple horizontal linear blur filter using 5 samples
	PP_VBLUR,		// (distance)	As PP_HBLUR but vertical
	PP_HSHARPEN,	// (d,mult)		A horizontal sharpen; raises contrast around edges
	PP_VSHARPEN,	// (d,mult)		As PP_HSHARPEN but vertical
	PP_BIBLUR,		// (dx,dy)		A simultaneous horizontal & vertical blur. A better effect, but cannot take advantage of parallel processing, so usually slower than HBLUR+VBLUR.
	PP_HBLURDOFFAR,	// (near,far,d)	Applies a depth of field, blurier further away. Needs depth in alpha channel, like PP_DEPTH and PP_OCCLUSION
	PP_VBLURDOFFAR,	// (near,far,d)	As PP_HBLURDOFFAR but vertical
	PP_HBLURDOFNEAR,// (near,far,d)	Applies a depth of field, blurier close-up. Needs depth in alpha channel, like PP_DEPTH and PP_OCCLUSION
	PP_VBLURDOFNEAR,// (near,far,d)	As PP_HBLURDOFNEAR but vertical
	PP_LINEARBLUR,	// (dx,dy)		As PP_HBLUR, but applies along the line dx,dy
	PP_RADIALBLUR,	// (cx,cy,dx,dy)Applies a radial blur from (cx,cy) with a size of dx at (cx+1,cy) and dy at (cx,cy+1)
	PP_RADIALBEAM,	// (cx,cy,dx,dy)Applies a radial blur from (cx,cy) with a size of dx at (cx+1,cy) and dy at (cx,cy+1) with an additive effect to make beams
	PP_ROTATIONALBLUR,	// (cx,cy,dx,dy)Applies a rotational blur around (cx,cy)
	PP_OVERLAY,		// (mult)		Output = Texture1 + Texture2 * mult
	PP_OVERLAYNEG,	// (mult)		Output = Texture1 - (1 - Texture2) * mult  !WARNING: Due to no EMT_TRANSPARENT_SUBTRACT_COLOR option, this uses a SLOW method. Will cause a performance hit if Texture2 is non-static.
	PP_MOTIONBLUR,	// (sharp)		Retains a memory of past renders, low sharp = long trails. Recommended sharp ~= 0.1
	PP_HAZE,		// (dist,opac,speed,scale)	Adds a heat haze, using the red channel of texture2 as heat, with 0 = cold, 1 = hot
	PP_HAZEDEPTH,	// (dist,opac,speed,scale)	Adds a heat haze, using the red channel of texture2 as heat, with 0 = cold, 1 = hot and the green channel as the z-depth
	PP_DEPTH,		// ()			Renders depth (alpha) as greyscale, lighter = further away
	PP_OCCLUSION,	// (mult)		Taken from the Irrlicht forums and heavily mutilated, uses alpha channel to judge depth
	
	// Composite effects
	
	PP_BLUR,		// (distance)			= HBLUR(distance) + VBLUR(distance)
	PP_SHARPEN,		// (distance,mult)		= HSHARPEN(distance,mult) + VSHARPEN(distance,mult)
	PP_BLURDOFFAR,	// (near,far,distance)	= HBLURDOFFAR(near,far,distance) + VBLURDOFFAR(near,far,distance)
	PP_BLURDOFNEAR,	// (near,far,distance)	= HBLURDOFNEAR(near,far,distance) + VBLURDOFNEAR(near,far,distance)
	PP_BLURDOF,		// (b1,f1,f2,b2,dist)	= BLURDOFFAR(f2,b2,dist) + HBLURDOFNEAR(b1,f1,dist) + VBLURDOFNEAR(b1,f1,dist)
	PP_BLOOM,		// (cut,distance,light)	= LIGHTEN(cut) + BLUR(distance) + OVERLAY(light)
	PP_GLOOM,		// (cut,distance,dark)	= DARKEN(cut) + BLUR(distance) + OVERLAYNEG(dark)  !WARNING: uses OVERLAYNEG which is slow!
	PP_NIGHTVISION,	// (max,distance,noise)	= RANGE(-0.5,max) + BLUR(distance) + NOISE(noise) + TINT(0.2,2.0,0.5)
	PP_MONITOR,		// (vig,bulge,noise,sat,pixel,rowm)	= TINT(1.0,0.9,0.8,1.0-sat) + NOISE(noise) + PIXELATEBANDS(pixel,pixel,rowm) + PUNCH(bulge,bulge,0.5,0.5) + VIGNETTE(4.0,0.0,1.0/vig)
	PP_WATERCOLOR,	// (bright,blur,levels,sharp,mult,noise)	= NOISE( noise ) + CURVES( bright, bright, bright ) + BIBLUR( blur, blur ) + POSTERIZE( levels ) + SHARPEN( sharp, mult )
	
	PP_COUNT // Not an effect, used to count number of effects.
};

class CEffectPostProc : public IPostProc {
private:
	POSTPROC_EFFECTID eff;
	CShaderCallback scEffect;
	irr::video::E_MATERIAL_TYPE useTextureOptimize;
	irr::video::SMaterial mat, mat2, mat3, mat4;
	irr::core::array<CEffectPostProc*> bound;
	
	void setShaderFromFiles( const irr::c8* file_gl, const irr::c8* file_dx, irr::video::E_PIXEL_SHADER_TYPE gl_ps, irr::video::E_PIXEL_SHADER_TYPE dx_ps );
	void setShaderFromFiles( irr::io::IReadFile* file_gl, irr::io::IReadFile* file_dx, irr::video::E_PIXEL_SHADER_TYPE gl_ps, irr::video::E_PIXEL_SHADER_TYPE dx_ps );
	void setShader( const irr::c8* prog_gl, const irr::c8* prog_dx, irr::video::E_PIXEL_SHADER_TYPE gl_ps, irr::video::E_PIXEL_SHADER_TYPE dx_ps, irr::video::SMaterial* tgt );
	
protected:
	virtual void configureInput( irr::u8 n );

public:
	CEffectPostProc( IPostProc* follow1, DIMENSION2D sOut, POSTPROC_EFFECTID effectID, irr::f32 para1 = FLOAT_DEFAULTVALUE, irr::f32 para2 = FLOAT_DEFAULTVALUE, irr::f32 para3 = FLOAT_DEFAULTVALUE, irr::f32 para4 = FLOAT_DEFAULTVALUE, irr::f32 para5 = FLOAT_DEFAULTVALUE, irr::f32 para6 = FLOAT_DEFAULTVALUE, irr::f32 para7 = FLOAT_DEFAULTVALUE, irr::f32 para8 = FLOAT_DEFAULTVALUE );
	CEffectPostProc( IPostProc* follow1, DIMENSION2D sOut, const irr::c8* prog_gl, const irr::c8* prog_dx, irr::video::E_PIXEL_SHADER_TYPE gl_ps = irr::video::EPST_PS_1_1, irr::video::E_PIXEL_SHADER_TYPE dx_ps = irr::video::EPST_PS_1_1, irr::video::E_MATERIAL_TYPE optimize = irr::video::EMT_SOLID, POSTPROC_FLAGS flags = PPF_DEFAULT, irr::f32 para1 = 0.0f, irr::f32 para2 = 0.0f, irr::f32 para3 = 0.0f, irr::f32 para4 = 0.0f, irr::f32 para5 = 0.0f, irr::f32 para6 = 0.0f, irr::f32 para7 = 0.0f, irr::f32 para8 = 0.0f );
	CEffectPostProc( IPostProc* follow1, DIMENSION2D sOut, irr::io::IReadFile* file_gl, irr::io::IReadFile* file_dx, irr::video::E_PIXEL_SHADER_TYPE gl_ps = irr::video::EPST_PS_1_1, irr::video::E_PIXEL_SHADER_TYPE dx_ps = irr::video::EPST_PS_1_1, irr::video::E_MATERIAL_TYPE optimize = irr::video::EMT_SOLID, irr::f32 para1 = 0.0f, irr::f32 para2 = 0.0f, irr::f32 para3 = 0.0f, irr::f32 para4 = 0.0f, irr::f32 para5 = 0.0f, irr::f32 para6 = 0.0f, irr::f32 para7 = 0.0f, irr::f32 para8 = 0.0f );
	virtual ~CEffectPostProc( );
	virtual IPostProc* getInput( irr::u8 n = 0u );
	virtual void renderEffect( irr::video::ITexture* target );
	virtual void enable( bool yes = true );
	virtual void setQuality( POSTPROC_QUALITY q );
	void setRequiredVariables( irr::u8 parameterNumber, bool usesTime = false, bool usesRandNum = false ) { if( eff != PP_CUSTOM ) return; scEffect.usesnum = parameterNumber; scEffect.usestime = usesTime; scEffect.usesrand = usesRandNum; }
	void setParameters( irr::f32 para1 = 0.0f, irr::f32 para2 = 0.0f, irr::f32 para3 = 0.0f, irr::f32 para4 = 0.0f, irr::f32 para5 = 0.0f, irr::f32 para6 = 0.0f, irr::f32 para7 = 0.0f, irr::f32 para8 = 0.0f );
};

#endif