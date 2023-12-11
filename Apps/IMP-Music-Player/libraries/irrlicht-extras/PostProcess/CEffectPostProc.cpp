#include <irrlicht.h>
#include "CEffectPostProc.h"

CEffectPostProc::CEffectPostProc( IPostProc* follow1, DIMENSION2D sOut, POSTPROC_EFFECTID effectID, irr::f32 para1, irr::f32 para2, irr::f32 para3, irr::f32 para4, irr::f32 para5, irr::f32 para6, irr::f32 para7, irr::f32 para8 ) : IPostProc( follow1, sOut ) {
	irr::video::IVideoDriver* driver = smgr->getVideoDriver( );

	// A rendered-to Texture0 is very fast, but a rendered-to Texture1 is slow. So if we have 2 textures, we have the option to render them in 2 materials
	// Set useTextureOptimize to EMT_TRANSPARENT_ADD_COLOR, EMT_TRANSPARENT_ALPHA_CHANNEL, EMT_TRANSPARENT_ALPHA_REF, etc. to render the second texture over
	// the first using the chosen method (shader is applied to the second texture)
	useTextureOptimize = irr::video::EMT_SOLID;
	IPostProc* follow2 = NULL;

	if( effectID >= PP_COUNT || effectID == PP_CUSTOM )
		effectID = PP_DIRECT;

	eff = effectID;
	if( supported ) {
		mat = irr::video::SMaterial( );
		mat.Wireframe = false;
		mat.Lighting = false;
		mat.FogEnable = false;
		mat.ZBuffer = 0;
		mat.TextureLayer[ 0u ].TextureWrapU = irr::video::ETC_CLAMP_TO_EDGE;
		mat.TextureLayer[ 0u ].TextureWrapV = irr::video::ETC_CLAMP_TO_EDGE;
		mat.TextureLayer[ 1u ].TextureWrapU = irr::video::ETC_CLAMP_TO_EDGE;
		mat.TextureLayer[ 1u ].TextureWrapV = irr::video::ETC_CLAMP_TO_EDGE;
		mat2 = irr::video::SMaterial( mat );
		mat3 = irr::video::SMaterial( mat );
		mat4 = irr::video::SMaterial( mat );
		scEffect = CShaderCallback( );

		CEffectPostProc* t1 = NULL;
		CEffectPostProc* t2 = NULL;
		CEffectPostProc* t3 = NULL;
		CEffectPostProc* t4 = NULL;
		CEffectPostProc* t5 = NULL;
		CEffectPostProc* t6 = NULL;
		switch( effectID ) {
			case PP_BLUR:
				if( para1 == FLOAT_DEFAULTVALUE )	para1 = 0.01f;
				t1 = new CEffectPostProc( follow1, sOut, PP_HBLUR );
				follow1 = (IPostProc*) t1;
				bound.reallocate( 1u );
				bound.push_back( t1 );
				effectID = PP_VBLUR;
				break;
			case PP_SHARPEN:
				if( para1 == FLOAT_DEFAULTVALUE )	para1 = 0.01f;
				if( para2 == FLOAT_DEFAULTVALUE )	para2 = 0.1f;
				t1 = new CEffectPostProc( follow1, sOut, PP_HSHARPEN );
				follow1 = (IPostProc*) t1;
				bound.reallocate( 1u );
				bound.push_back( t1 );
				effectID = PP_VSHARPEN;
				break;
			case PP_BLURDOFFAR:
				if( para1 == FLOAT_DEFAULTVALUE )	para1 = 0.0f;
				if( para2 == FLOAT_DEFAULTVALUE )	para2 = 20.0f;
				if( para3 == FLOAT_DEFAULTVALUE )	para3 = 0.01f;
				t1 = new CEffectPostProc( follow1, sOut, PP_HBLURDOFFAR );
				follow1 = (IPostProc*) t1;
				bound.reallocate( 1u );
				bound.push_back( t1 );
				effectID = PP_VBLURDOFFAR;
				break;
			case PP_BLURDOFNEAR:
				if( para1 == FLOAT_DEFAULTVALUE )	para1 = 0.0f;
				if( para2 == FLOAT_DEFAULTVALUE )	para2 = 20.0f;
				if( para3 == FLOAT_DEFAULTVALUE )	para3 = 0.01f;
				t1 = new CEffectPostProc( follow1, sOut, PP_HBLURDOFNEAR );
				follow1 = (IPostProc*) t1;
				bound.reallocate( 1u );
				bound.push_back( t1 );
				effectID = PP_VBLURDOFNEAR;
				break;
			case PP_BLURDOF:
				if( para1 == FLOAT_DEFAULTVALUE )	para1 = 0.0f;
				if( para4 == FLOAT_DEFAULTVALUE )	para4 = 20.0f;
				if( para2 == FLOAT_DEFAULTVALUE ) {
					if( para3 == FLOAT_DEFAULTVALUE )
						para2 = (para1 + para4) / 3.0f;
					else
						para2 = (para1 + para3) * 0.5f;
				}
				if( para3 == FLOAT_DEFAULTVALUE )	para3 = (para2 + para4) * 0.5f;
				if( para5 == FLOAT_DEFAULTVALUE )	para5 = 0.01f;
				t1 = new CEffectPostProc( follow1, sOut, PP_BLURDOFFAR );
				t2 = new CEffectPostProc( t1, sOut, PP_HBLURDOFNEAR );
				follow1 = (IPostProc*) t2;
				bound.reallocate( 2u );
				bound.push_back( t1 );
				bound.push_back( t2 );
				effectID = PP_VBLURDOFNEAR;
				break;
			case PP_BLOOM:
				if( para1 == FLOAT_DEFAULTVALUE )	para1 = 4.0f;
				if( para2 == FLOAT_DEFAULTVALUE )	para2 = 0.005f;
				if( para3 == FLOAT_DEFAULTVALUE )	para3 = 2.0f;
				bound.reallocate( 2u );
				t1 = new CEffectPostProc( follow1, sOut, PP_LIGHTEN );
				t2 = new CEffectPostProc( t1, sOut / 2, PP_BLUR );
				follow2 = (IPostProc*) t2;
				bound.push_back( t1 );
				bound.push_back( t2 );
				effectID = PP_OVERLAY;
				break;
			case PP_GLOOM:
				if( para1 == FLOAT_DEFAULTVALUE )	para1 = 4.0f;
				if( para2 == FLOAT_DEFAULTVALUE )	para2 = 0.005f;
				if( para3 == FLOAT_DEFAULTVALUE )	para3 = 2.0f;
				bound.reallocate( 2u );
				t1 = new CEffectPostProc( follow1, sOut, PP_DARKEN );
				t2 = new CEffectPostProc( t1, sOut / 2, PP_BLUR );
				follow2 = (IPostProc*) t2;
				bound.push_back( t1 );
				bound.push_back( t2 );
				effectID = PP_OVERLAYNEG;
				break;
			case PP_NIGHTVISION:
				if( para1 == FLOAT_DEFAULTVALUE )	para1 = 0.5f;
				if( para2 == FLOAT_DEFAULTVALUE )	para2 = 0.005f;
				if( para3 == FLOAT_DEFAULTVALUE )	para3 = 0.3f;
				bound.reallocate( 3u );
				t1 = new CEffectPostProc( follow1, sOut, PP_RANGE );
				t2 = new CEffectPostProc( t1, sOut, PP_BLUR );
				t3 = new CEffectPostProc( t2, sOut, PP_NOISE );
				follow1 = (IPostProc*) t3;
				bound.push_back( t1 );
				bound.push_back( t2 );
				bound.push_back( t3 );
				effectID = PP_TINT;
				break;
			case PP_MONITOR:
				if( para1 == FLOAT_DEFAULTVALUE )	para1 = 0.5f;
				if( para2 == FLOAT_DEFAULTVALUE )	para2 = 0.1f;
				if( para3 == FLOAT_DEFAULTVALUE )	para3 = 0.2f;
				if( para4 == FLOAT_DEFAULTVALUE )	para4 = 0.3f;
				if( para5 == FLOAT_DEFAULTVALUE )	para5 = 0.005f;
				if( para6 == FLOAT_DEFAULTVALUE )	para6 = 0.95f;
				bound.reallocate( 4u );
				t1 = new CEffectPostProc( follow1, sOut, PP_TINT );
				t2 = new CEffectPostProc( t1, sOut, PP_NOISE );
				t3 = new CEffectPostProc( t2, sOut, PP_PIXELATEBANDS );
				t4 = new CEffectPostProc( t3, sOut, PP_PUNCH );
				follow1 = (IPostProc*) t4;
				bound.push_back( t1 );
				bound.push_back( t2 );
				bound.push_back( t3 );
				bound.push_back( t4 );
				effectID = PP_VIGNETTE;
				break;
			case PP_WATERCOLOR:
				if( para1 == FLOAT_DEFAULTVALUE )	para1 = 1.5f;
				if( para2 == FLOAT_DEFAULTVALUE )	para2 = 0.005f;
				if( para3 == FLOAT_DEFAULTVALUE )	para3 = 10.0f;
				if( para4 == FLOAT_DEFAULTVALUE )	para4 = 0.01f;
				if( para5 == FLOAT_DEFAULTVALUE )	para5 = 0.2f;
				if( para6 == FLOAT_DEFAULTVALUE )	para6 = 0.15f;
				bound.reallocate( 6u );
				t1 = new CEffectPostProc( follow1, DIMENSION2D( 64, 64 ), PP_PURENOISE );
				t2 = new CEffectPostProc( follow1, sOut, PP_CURVES );
				t3 = new CEffectPostProc( t2, sOut, PP_OVERLAY );
				t3->setInput( 1u, t1 );
				t4 = new CEffectPostProc( t3, sOut, PP_BIBLUR );
				t5 = new CEffectPostProc( t4, sOut, PP_POSTERIZE );
				t6 = new CEffectPostProc( t5, sOut, PP_HSHARPEN );
				follow1 = (IPostProc*) t6;
				bound.push_back( t1 );
				bound.push_back( t2 );
				bound.push_back( t3 );
				bound.push_back( t4 );
				bound.push_back( t5 );
				bound.push_back( t6 );
				t1->setFixed( true );
				effectID = PP_VSHARPEN;
				break;
		}
		switch( effectID ) {
			case PP_DIRECT:
				setShader( NULL, NULL, irr::video::EPST_PS_1_1, irr::video::EPST_PS_1_1, &mat );
				break;
			case PP_PUNCH:
				if( para1 == FLOAT_DEFAULTVALUE )	para1 = 0.2f;
				if( para2 == FLOAT_DEFAULTVALUE )	para2 = para1;
				if( para3 == FLOAT_DEFAULTVALUE )	para3 = 0.5f;
				if( para4 == FLOAT_DEFAULTVALUE )	para4 = 0.5f;
				scEffect.usesnum = 4u;
				setShader( GL_PUNCH, DX_PUNCH, irr::video::EPST_PS_1_1, irr::video::EPST_PS_2_0, &mat );
				break;
			case PP_PIXELATE:
				if( para1 == FLOAT_DEFAULTVALUE )	para1 = 0.01f;
				if( para2 == FLOAT_DEFAULTVALUE )	para2 = para1;
				scEffect.usesnum = 2u;
				setShader( GL_PIXELATE, DX_PIXELATE, irr::video::EPST_PS_1_1, irr::video::EPST_PS_2_0, &mat );
				break;
			case PP_POSTERIZE:
				if( para1 == FLOAT_DEFAULTVALUE )	para1 = 2.0f;
				scEffect.usesnum = 1u;
				setShader( GL_POSTERIZE, DX_POSTERIZE, irr::video::EPST_PS_1_1, irr::video::EPST_PS_2_0, &mat );
				break;
			case PP_PIXELATEBANDS:
				if( para1 == FLOAT_DEFAULTVALUE )	para1 = 0.01f;
				if( para2 == FLOAT_DEFAULTVALUE )	para2 = para1;
				if( para3 == FLOAT_DEFAULTVALUE )	para3 = 0.8f;
				scEffect.usesnum = 3u;
				setShader( GL_PIXELATEBANDS, DX_PIXELATEBANDS, irr::video::EPST_PS_1_1, irr::video::EPST_PS_2_0, &mat );
				break;
			case PP_DEPTH:
				if( para1 == FLOAT_DEFAULTVALUE )	para1 = 0.0f;
				if( para2 == FLOAT_DEFAULTVALUE )	para2 = 200.0f;
				scEffect.usesnum = 2u;
				setShader( GL_DEPTH, DX_DEPTH, irr::video::EPST_PS_1_1, irr::video::EPST_PS_2_0, &mat );
				break;
			case PP_OCCLUSION:
				if( para1 == FLOAT_DEFAULTVALUE )	para1 = 1.0f;
				scEffect.usesnum = 3u;
				setShader( GL_OCCLUSION, DX_OCCLUSION, irr::video::EPST_PS_1_1, irr::video::EPST_PS_2_0, &mat );
				setShader( GL_OCCLUSIONFAST, DX_OCCLUSIONFAST, irr::video::EPST_PS_1_1, irr::video::EPST_PS_2_0, &mat2 );
				break;
			case PP_DARKEN:
				if( para1 == FLOAT_DEFAULTVALUE )	para1 = 1.0f;
				scEffect.usesnum = 1u;
				setShader( GL_DARKEN, DX_DARKEN, irr::video::EPST_PS_1_1, irr::video::EPST_PS_2_0, &mat );
				break;
			case PP_LIGHTEN:
				if( para1 == FLOAT_DEFAULTVALUE )	para1 = 1.0f;
				scEffect.usesnum = 1u;
				setShader( GL_LIGHTEN, DX_LIGHTEN, irr::video::EPST_PS_1_1, irr::video::EPST_PS_2_0, &mat );
				break;
			case PP_RANGE:
				if( para1 == FLOAT_DEFAULTVALUE )	para1 = 0.0f;
				if( para2 == FLOAT_DEFAULTVALUE )	para2 = 1.0f;
				scEffect.usesnum = 2u;
				setShader( GL_RANGE, DX_RANGE, irr::video::EPST_PS_1_1, irr::video::EPST_PS_2_0, &mat );
				break;
			case PP_INVERT:
				setShader( GL_INVERT, DX_INVERT, irr::video::EPST_PS_1_1, irr::video::EPST_PS_1_1, &mat );
				break;
			case PP_CURVES:
				if( para2 == FLOAT_DEFAULTVALUE && para3 == FLOAT_DEFAULTVALUE ) {
					para2 = para1;
					para3 = para1;
				}
				if( para1 == FLOAT_DEFAULTVALUE )	para1 = 1.0f;
				if( para2 == FLOAT_DEFAULTVALUE )	para2 = 1.0f;
				if( para3 == FLOAT_DEFAULTVALUE )	para3 = 1.0f;
				scEffect.usesnum = 3u;
				setShader( GL_CURVES, DX_CURVES, irr::video::EPST_PS_1_1, irr::video::EPST_PS_2_0, &mat );
				break;
			case PP_TINT:
				if( para2 == FLOAT_DEFAULTVALUE && para3 == FLOAT_DEFAULTVALUE ) {
					para2 = para1;
					para3 = para1;
				}
				if( para1 == FLOAT_DEFAULTVALUE )	para1 = 1.0f;
				if( para2 == FLOAT_DEFAULTVALUE )	para2 = 1.0f;
				if( para3 == FLOAT_DEFAULTVALUE )	para3 = 1.0f;
				if( para3 == FLOAT_DEFAULTVALUE )	para4 = 1.0f;
				scEffect.usesnum = 4u;
				setShader( GL_TINT, DX_TINT, irr::video::EPST_PS_1_1, irr::video::EPST_PS_2_0, &mat );
				break;
			case PP_GREYSCALE:
				if( para1 == FLOAT_DEFAULTVALUE )	para1 = 1.0f;
				scEffect.usesnum = 4u;
				setShader( GL_TINT, DX_TINT, irr::video::EPST_PS_1_1, irr::video::EPST_PS_2_0, &mat );
				break;
			case PP_SEPIA:
				para1 = 2.0f;
				para2 = 1.0f;
				para3 = 0.7f;
				scEffect.usesnum = 4u;
				setShader( GL_TINT, DX_TINT, irr::video::EPST_PS_1_1, irr::video::EPST_PS_2_0, &mat );
				break;
			case PP_SATURATE:
				if( para1 == FLOAT_DEFAULTVALUE )	para1 = 1.5f;
				scEffect.usesnum = 4u;
				setShader( GL_TINT, DX_TINT, irr::video::EPST_PS_1_1, irr::video::EPST_PS_2_0, &mat );
				break;
			case PP_VIGNETTE:
				if( para1 == FLOAT_DEFAULTVALUE )	para1 = 2.0f;
				if( para2 == FLOAT_DEFAULTVALUE )	para2 = 0.0f;
				if( para3 == FLOAT_DEFAULTVALUE )	para3 = 1.0f;
				scEffect.usesnum = 3u;
				setShader( GL_VIGNETTE, DX_VIGNETTE, irr::video::EPST_PS_1_1, irr::video::EPST_PS_2_0, &mat );
				break;
			case PP_NOISE:
				if( para1 == FLOAT_DEFAULTVALUE )	para1 = 0.1f;
				scEffect.usesnum = 1u;
				scEffect.usesrand = true;
				setShader( GL_NOISE, DX_NOISE, irr::video::EPST_PS_1_1, irr::video::EPST_PS_2_0, &mat );
				break;
			case PP_PURENOISE:
				if( para1 == FLOAT_DEFAULTVALUE )	para1 = 0.5f;
				scEffect.usesnum = 1u;
				scEffect.usesrand = true;
				setShader( GL_PURENOISE, DX_PURENOISE, irr::video::EPST_PS_1_1, irr::video::EPST_PS_2_0, &mat );
				break;
			case PP_COLORNOISE:
				if( para1 == FLOAT_DEFAULTVALUE )	para1 = 0.1f;
				scEffect.usesnum = 1u;
				scEffect.usesrand = true;
				setShader( GL_COLORNOISE, DX_COLORNOISE, irr::video::EPST_PS_1_1, irr::video::EPST_PS_2_0, &mat );
				break;
			case PP_HBLUR:
				if( para1 == FLOAT_DEFAULTVALUE )	para1 = 0.01f;
				scEffect.usesnum = 1u;
				setShader( GL_HBLUR_5, DX_HBLUR_5, irr::video::EPST_PS_1_2, irr::video::EPST_PS_2_0, &mat );
				setShader( GL_HBLUR_3, DX_HBLUR_3, irr::video::EPST_PS_1_2, irr::video::EPST_PS_2_0, &mat2 );
				setShader( GL_HBLUR_11, DX_HBLUR_11, irr::video::EPST_PS_1_2, irr::video::EPST_PS_2_0, &mat3 );
				break;
			case PP_VBLUR:
				if( para1 == FLOAT_DEFAULTVALUE )	para1 = 0.01f;
				scEffect.usesnum = 1u;
				setShader( GL_VBLUR_5, DX_VBLUR_5, irr::video::EPST_PS_1_2, irr::video::EPST_PS_2_0, &mat );
				setShader( GL_VBLUR_3, DX_VBLUR_3, irr::video::EPST_PS_1_2, irr::video::EPST_PS_2_0, &mat2 );
				setShader( GL_VBLUR_11, DX_VBLUR_11, irr::video::EPST_PS_1_2, irr::video::EPST_PS_2_0, &mat3 );
				break;
			case PP_HSHARPEN:
				if( para1 == FLOAT_DEFAULTVALUE )	para1 = 0.01f;
				if( para2 == FLOAT_DEFAULTVALUE )	para2 = 0.1f;
				scEffect.usesnum = 2u;
				setShader( GL_HSHARPEN_5, DX_HSHARPEN_5, irr::video::EPST_PS_1_2, irr::video::EPST_PS_2_0, &mat );
				setShader( GL_HSHARPEN_3, DX_HSHARPEN_3, irr::video::EPST_PS_1_2, irr::video::EPST_PS_2_0, &mat2 );
				setShader( GL_HSHARPEN_11, DX_HSHARPEN_11, irr::video::EPST_PS_1_2, irr::video::EPST_PS_2_0, &mat3 );
				break;
			case PP_VSHARPEN:
				if( para1 == FLOAT_DEFAULTVALUE )	para1 = 0.01f;
				if( para2 == FLOAT_DEFAULTVALUE )	para2 = 0.1f;
				scEffect.usesnum = 2u;
				setShader( GL_VSHARPEN_5, DX_VSHARPEN_5, irr::video::EPST_PS_1_2, irr::video::EPST_PS_2_0, &mat );
				setShader( GL_VSHARPEN_3, DX_VSHARPEN_3, irr::video::EPST_PS_1_2, irr::video::EPST_PS_2_0, &mat2 );
				setShader( GL_VSHARPEN_11, DX_VSHARPEN_11, irr::video::EPST_PS_1_2, irr::video::EPST_PS_2_0, &mat3 );
				break;
			case PP_BIBLUR:
				if( para1 == FLOAT_DEFAULTVALUE )	para1 = 0.01f;
				if( para2 == FLOAT_DEFAULTVALUE )	para2 = para1;
				scEffect.usesnum = 2u;
				setShader( GL_BIBLUR_9, DX_BIBLUR_9, irr::video::EPST_PS_1_2, irr::video::EPST_PS_2_0, &mat );
				setShader( GL_BIBLUR_5, DX_BIBLUR_5, irr::video::EPST_PS_1_2, irr::video::EPST_PS_2_0, &mat2 );
				setShader( GL_BIBLUR_13, DX_BIBLUR_13, irr::video::EPST_PS_1_2, irr::video::EPST_PS_2_0, &mat3 );
				break;
			case PP_HBLURDOFFAR:
				if( para1 == FLOAT_DEFAULTVALUE )	para1 = 0.0f;
				if( para2 == FLOAT_DEFAULTVALUE )	para2 = 1.0f;
				if( para3 == FLOAT_DEFAULTVALUE )	para3 = 0.01f;
				scEffect.usesnum = 3u;
				setShader( GL_HBLURDOFFAR_5, DX_HBLURDOFFAR_5, irr::video::EPST_PS_1_2, irr::video::EPST_PS_2_0, &mat );
				setShader( GL_HBLURDOFFAR_3, DX_HBLURDOFFAR_3, irr::video::EPST_PS_1_2, irr::video::EPST_PS_2_0, &mat2 );
				break;
			case PP_VBLURDOFFAR:
				if( para1 == FLOAT_DEFAULTVALUE )	para1 = 0.0f;
				if( para2 == FLOAT_DEFAULTVALUE )	para2 = 1.0f;
				if( para3 == FLOAT_DEFAULTVALUE )	para3 = 0.01f;
				scEffect.usesnum = 3u;
				setShader( GL_VBLURDOFFAR_5, DX_VBLURDOFFAR_5, irr::video::EPST_PS_1_2, irr::video::EPST_PS_2_0, &mat );
				setShader( GL_VBLURDOFFAR_3, DX_VBLURDOFFAR_3, irr::video::EPST_PS_1_2, irr::video::EPST_PS_2_0, &mat2 );
				break;
			case PP_HBLURDOFNEAR:
				if( para1 == FLOAT_DEFAULTVALUE )	para1 = 0.0f;
				if( para2 == FLOAT_DEFAULTVALUE )	para2 = 1.0f;
				if( para3 == FLOAT_DEFAULTVALUE )	para3 = 0.01f;
				scEffect.usesnum = 3u;
				setShader( GL_HBLURDOFNEAR_5, DX_HBLURDOFNEAR_5, irr::video::EPST_PS_1_2, irr::video::EPST_PS_2_0, &mat );
				setShader( GL_HBLURDOFNEAR_3, DX_HBLURDOFNEAR_3, irr::video::EPST_PS_1_2, irr::video::EPST_PS_2_0, &mat2 );
				break;
			case PP_VBLURDOFNEAR:
				if( para1 == FLOAT_DEFAULTVALUE )	para1 = 0.0f;
				if( para2 == FLOAT_DEFAULTVALUE )	para2 = 1.0f;
				if( para3 == FLOAT_DEFAULTVALUE )	para3 = 0.01f;
				scEffect.usesnum = 3u;
				setShader( GL_VBLURDOFNEAR_5, DX_VBLURDOFNEAR_5, irr::video::EPST_PS_1_2, irr::video::EPST_PS_2_0, &mat );
				setShader( GL_VBLURDOFNEAR_3, DX_VBLURDOFNEAR_3, irr::video::EPST_PS_1_2, irr::video::EPST_PS_2_0, &mat2 );
				break;
			case PP_LINEARBLUR:
				if( para2 == FLOAT_DEFAULTVALUE ) para2 = 0.0f;
				if( para1 == FLOAT_DEFAULTVALUE ) {
					para1 = 0.01f;
					para2 = 0.0f;
				}
				scEffect.usesnum = 2u;
				setShader( GL_LINEARBLUR_5, DX_LINEARBLUR_5, irr::video::EPST_PS_1_2, irr::video::EPST_PS_2_0, &mat );
				setShader( GL_LINEARBLUR_3, DX_LINEARBLUR_3, irr::video::EPST_PS_1_2, irr::video::EPST_PS_2_0, &mat2 );
				setShader( GL_LINEARBLUR_11, DX_LINEARBLUR_11, irr::video::EPST_PS_1_2, irr::video::EPST_PS_2_0, &mat3 );
				break;
			case PP_RADIALBLUR:
				if( para1 == FLOAT_DEFAULTVALUE ) para1 = 0.5f;
				if( para2 == FLOAT_DEFAULTVALUE ) para2 = 0.5f;
				if( para3 == FLOAT_DEFAULTVALUE ) para3 = 0.01f;
				if( para4 == FLOAT_DEFAULTVALUE ) para4 = para3;
				scEffect.usesnum = 4u;
				setShader( GL_RADIALBLUR_5, DX_RADIALBLUR_5, irr::video::EPST_PS_1_2, irr::video::EPST_PS_2_0, &mat );
				setShader( GL_RADIALBLUR_3, DX_RADIALBLUR_3, irr::video::EPST_PS_1_2, irr::video::EPST_PS_2_0, &mat2 );
				setShader( GL_RADIALBLUR_11, DX_RADIALBLUR_11, irr::video::EPST_PS_1_2, irr::video::EPST_PS_2_0, &mat3 );
				break;
			case PP_RADIALBEAM:
				if( para1 == FLOAT_DEFAULTVALUE ) para1 = 0.5f;
				if( para2 == FLOAT_DEFAULTVALUE ) para2 = 0.5f;
				if( para3 == FLOAT_DEFAULTVALUE ) para3 = 0.01f;
				if( para4 == FLOAT_DEFAULTVALUE ) para4 = para3;
				scEffect.usesnum = 4u;
				setShader( GL_RADIALBEAM_5, DX_RADIALBEAM_5, irr::video::EPST_PS_1_2, irr::video::EPST_PS_2_0, &mat );
				setShader( GL_RADIALBEAM_3, DX_RADIALBEAM_3, irr::video::EPST_PS_1_2, irr::video::EPST_PS_2_0, &mat2 );
				setShader( GL_RADIALBEAM_11, DX_RADIALBEAM_11, irr::video::EPST_PS_1_2, irr::video::EPST_PS_2_0, &mat3 );
				break;
			case PP_ROTATIONALBLUR:
				if( para1 == FLOAT_DEFAULTVALUE ) para1 = 0.5f;
				if( para2 == FLOAT_DEFAULTVALUE ) para2 = 0.5f;
				if( para3 == FLOAT_DEFAULTVALUE ) para3 = 0.01f;
				if( para4 == FLOAT_DEFAULTVALUE ) para4 = para3;
				scEffect.usesnum = 4u;
				setShader( GL_ROTATIONALBLUR_5, DX_ROTATIONALBLUR_5, irr::video::EPST_PS_1_2, irr::video::EPST_PS_2_0, &mat );
				setShader( GL_ROTATIONALBLUR_3, DX_ROTATIONALBLUR_3, irr::video::EPST_PS_1_2, irr::video::EPST_PS_2_0, &mat2 );
				setShader( GL_ROTATIONALBLUR_11, DX_ROTATIONALBLUR_11, irr::video::EPST_PS_1_2, irr::video::EPST_PS_2_0, &mat3 );
				break;
			case PP_MOTIONBLUR:
				if( para1 == FLOAT_DEFAULTVALUE )	para1 = 0.05f;
				scEffect.usesnum = 1u;
				follow2 = follow1;
				follow1 = this;
				useTextureOptimize = irr::video::EMT_TRANSPARENT_ALPHA_CHANNEL;
				setShader( GL_MOTIONBLUR2, DX_MOTIONBLUR2, irr::video::EPST_PS_1_1, irr::video::EPST_PS_1_1, &mat );
				break;
			case PP_HAZE:
				if( para1 == FLOAT_DEFAULTVALUE ) para1 = 0.005f;
				if( para2 == FLOAT_DEFAULTVALUE ) para2 = 0.5f;
				if( para3 == FLOAT_DEFAULTVALUE ) para3 = 1.0f;
				if( para4 == FLOAT_DEFAULTVALUE ) para4 = 1.0f;
				scEffect.usesnum = 4u;
				scEffect.usestime = true;
				setShader( GL_HAZE, DX_HAZE, irr::video::EPST_PS_1_2, irr::video::EPST_PS_2_0, &mat );
				break;
			case PP_HAZEDEPTH:
				if( para1 == FLOAT_DEFAULTVALUE ) para1 = 0.005f;
				if( para2 == FLOAT_DEFAULTVALUE ) para2 = 0.5f;
				if( para3 == FLOAT_DEFAULTVALUE ) para3 = 1.0f;
				if( para4 == FLOAT_DEFAULTVALUE ) para4 = 1.0f;
				scEffect.usesnum = 4u;
				scEffect.usestime = true;
				setShader( GL_HAZEDEPTH, DX_HAZEDEPTH, irr::video::EPST_PS_1_2, irr::video::EPST_PS_2_0, &mat );
				mat.TextureLayer[ 1u ].BilinearFilter = false;
				mat2.TextureLayer[ 1u ].BilinearFilter = false;
				mat3.TextureLayer[ 1u ].BilinearFilter = false;
				mat4.TextureLayer[ 1u ].BilinearFilter = false;
				break;
			case PP_OVERLAY:
				if( para1 == FLOAT_DEFAULTVALUE )	para1 = 1.0f;
				scEffect.usesnum = 1u;
				useTextureOptimize = irr::video::EMT_TRANSPARENT_ADD_COLOR;
				setShader( GL_OVERLAY2, DX_OVERLAY2, irr::video::EPST_PS_1_1, irr::video::EPST_PS_2_0, &mat );
				break;
			case PP_OVERLAYNEG:
				if( para1 == FLOAT_DEFAULTVALUE )	para1 = 1.0f;
				scEffect.usesnum = 1u;
				setShader( GL_OVERLAYNEG, DX_OVERLAYNEG, irr::video::EPST_PS_1_1, irr::video::EPST_PS_2_0, &mat );
				break;
		}

		if( para1 == FLOAT_DEFAULTVALUE ) para1 = 0.0f;
		if( para2 == FLOAT_DEFAULTVALUE ) para2 = 0.0f;
		if( para3 == FLOAT_DEFAULTVALUE ) para3 = 0.0f;
		if( para4 == FLOAT_DEFAULTVALUE ) para4 = 0.0f;
		if( para5 == FLOAT_DEFAULTVALUE ) para5 = 0.0f;
		if( para6 == FLOAT_DEFAULTVALUE ) para6 = 0.0f;
		if( para7 == FLOAT_DEFAULTVALUE ) para7 = 0.0f;
		if( para8 == FLOAT_DEFAULTVALUE ) para8 = 0.0f;

		setParameters( para1, para2, para3, para4, para5, para6, para7, para8 );
	}
	setInput( 0u, follow1 );
	setInput( 1u, follow2 );
}
CEffectPostProc::CEffectPostProc( IPostProc* follow1, DIMENSION2D sOut, const irr::c8* prog_gl, const irr::c8* prog_dx, irr::video::E_PIXEL_SHADER_TYPE gl_ps, irr::video::E_PIXEL_SHADER_TYPE dx_ps, irr::video::E_MATERIAL_TYPE optimize, POSTPROC_FLAGS flags, irr::f32 para1, irr::f32 para2, irr::f32 para3, irr::f32 para4, irr::f32 para5, irr::f32 para6, irr::f32 para7, irr::f32 para8 ) : IPostProc( follow1, sOut ) {
	irr::video::IVideoDriver* driver = smgr->getVideoDriver( );
	useTextureOptimize = optimize;
	eff = PP_CUSTOM;

	if( supported ) {
		mat.Wireframe = false;
		mat.Lighting = false;
		mat.FogEnable = false;
		mat.ZBuffer = 0;
		mat.TextureLayer[ 0u ].TextureWrapU = irr::video::ETC_CLAMP_TO_EDGE;
		mat.TextureLayer[ 0u ].TextureWrapV = irr::video::ETC_CLAMP_TO_EDGE;
		mat.TextureLayer[ 1u ].TextureWrapU = irr::video::ETC_CLAMP_TO_EDGE;
		mat.TextureLayer[ 1u ].TextureWrapV = irr::video::ETC_CLAMP_TO_EDGE;
		mat2 = mat;
		mat3 = mat;
		mat4 = mat;
		if( flags & PPF_FROMFILE )
			setShaderFromFiles( prog_gl, prog_dx, gl_ps, dx_ps );
		else
			setShader( prog_gl, prog_dx, gl_ps, dx_ps, &mat );
		setParameters( para1, para2, para3, para4, para5, para6, para7, para8 );
	}
	setInput( 0u, follow1 );
}
CEffectPostProc::CEffectPostProc( IPostProc* follow1, DIMENSION2D sOut, irr::io::IReadFile* file_gl, irr::io::IReadFile* file_dx, irr::video::E_PIXEL_SHADER_TYPE gl_ps, irr::video::E_PIXEL_SHADER_TYPE dx_ps, irr::video::E_MATERIAL_TYPE optimize, irr::f32 para1, irr::f32 para2, irr::f32 para3, irr::f32 para4, irr::f32 para5, irr::f32 para6, irr::f32 para7, irr::f32 para8 ) : IPostProc( follow1, sOut ) {
	irr::video::IVideoDriver* driver = smgr->getVideoDriver( );
	useTextureOptimize = optimize;
	eff = PP_CUSTOM;

	if( supported ) {
		mat.Wireframe = false;
		mat.Lighting = false;
		mat.FogEnable = false;
		mat.ZBuffer = 0;
		mat.TextureLayer[ 0u ].TextureWrapU = irr::video::ETC_CLAMP_TO_EDGE;
		mat.TextureLayer[ 0u ].TextureWrapV = irr::video::ETC_CLAMP_TO_EDGE;
		mat.TextureLayer[ 1u ].TextureWrapU = irr::video::ETC_CLAMP_TO_EDGE;
		mat.TextureLayer[ 1u ].TextureWrapV = irr::video::ETC_CLAMP_TO_EDGE;
		mat2 = mat;
		mat3 = mat;
		mat4 = mat;
		setShaderFromFiles( file_gl, file_dx, gl_ps, dx_ps );
		setParameters( para1, para2, para3, para4, para5, para6, para7, para8 );
	}
	setInput( 0u, follow1 );
}
CEffectPostProc::~CEffectPostProc( ) {
	for( irr::u8 i = 0u; i != bound.size( ); ++ i )
		delete bound[ i ];
}
void CEffectPostProc::enable( bool yes ) {
	if( yes ) {
		if( !enabled ) {
			if( hasInput( 0u ) )
				if( useTextureOptimize == irr::video::EMT_SOLID || !hasInput( 1u ) ) {
					mat.setTexture( 0u, getInputTexture( 0u ) );
					mat2.setTexture( 0u, getInputTexture( 0u ) );
					mat3.setTexture( 0u, getInputTexture( 0u ) );
					mat4.setTexture( 0u, getInputTexture( 0u ) );
				}
			if( hasInput( 1u ) ) {
				if( useTextureOptimize != irr::video::EMT_SOLID ) {
					mat.setTexture( 0u, getInputTexture( 1u ) );
					mat2.setTexture( 0u, getInputTexture( 1u ) );
					mat3.setTexture( 0u, getInputTexture( 1u ) );
					mat4.setTexture( 0u, getInputTexture( 1u ) );
				} else {
					mat.setTexture( 1u, getInputTexture( 1u ) );
					mat2.setTexture( 1u, getInputTexture( 1u ) );
					mat3.setTexture( 1u, getInputTexture( 1u ) );
					mat4.setTexture( 1u, getInputTexture( 1u ) );
				}
			}
		}
	} else if( enabled ) {
		mat.setTexture( 0u, NULL );
		mat.setTexture( 1u, NULL );
		mat2.setTexture( 0u, NULL );
		mat2.setTexture( 1u, NULL );
		mat3.setTexture( 0u, NULL );
		mat3.setTexture( 1u, NULL );
		mat4.setTexture( 0u, NULL );
		mat4.setTexture( 1u, NULL );
	}
	IPostProc::enable( yes );
	for( irr::u8 i = 0u; i != bound.size( ); ++ i )
		if( bound[ i ] != NULL )
			bound[ i ]->enable( yes );
}
void CEffectPostProc::setShader( const irr::c8* prog_gl, const irr::c8* prog_dx, irr::video::E_PIXEL_SHADER_TYPE gl_ps, irr::video::E_PIXEL_SHADER_TYPE dx_ps, irr::video::SMaterial* tgt ) {
	if( !supported )
		return;

	irr::video::IVideoDriver* driver = smgr->getVideoDriver( );
	irr::video::IGPUProgrammingServices* gpu = driver->getGPUProgrammingServices( );

	irr::u32 matid;
	switch( driver->getDriverType( ) ) {
		case irr::video::EDT_OPENGL:
			matid = gpu->addHighLevelShaderMaterial( GL_V_FULLSCREEN, "main", irr::video::EVST_VS_1_1, prog_gl, "main", gl_ps, &scEffect, useTextureOptimize, useTextureOptimize == irr::video::EMT_SOLID ? 2 : 1 );
			break;
		case irr::video::EDT_DIRECT3D9:
			matid = gpu->addHighLevelShaderMaterial( DX_V_FULLSCREEN, "main", irr::video::EVST_VS_1_1, prog_dx, "main", dx_ps, &scEffect, useTextureOptimize, 0 );
			break;
	}
	tgt->MaterialType = (irr::video::E_MATERIAL_TYPE) matid;
}
void CEffectPostProc::setShaderFromFiles( const irr::c8* file_gl, const irr::c8* file_dx, irr::video::E_PIXEL_SHADER_TYPE gl_ps, irr::video::E_PIXEL_SHADER_TYPE dx_ps ) {
	if( !supported )
		return;

	irr::video::IVideoDriver* driver = smgr->getVideoDriver( );
	irr::video::IGPUProgrammingServices* gpu = driver->getGPUProgrammingServices( );

	irr::io::IReadFile* psfile = smgr->getGUIEnvironment( )->getFileSystem( )->createAndOpenFile( (driver->getDriverType( ) == irr::video::EDT_OPENGL) ? file_gl : file_dx );
	if( !psfile ) {
		setShader( NULL, NULL, irr::video::EPST_PS_1_1, irr::video::EPST_PS_1_1, &mat );
		eff = PP_DIRECT;
		disable( );
		return;
	}

	const long size = psfile->getSize( );
	if( size == 0l ) {
		setShader( NULL, NULL, irr::video::EPST_PS_1_1, irr::video::EPST_PS_1_1, &mat );
		eff = PP_DIRECT;
		disable( );
		return;
	}
	irr::c8* ps = new irr::c8[ size + 1 ];
	psfile->read( ps, size );
	ps[ size ] = '\0';

	setShader( ps, ps, gl_ps, dx_ps, &mat );
	delete [] ps;
	psfile->drop( );
}
void CEffectPostProc::setShaderFromFiles( irr::io::IReadFile* file_gl, irr::io::IReadFile* file_dx, irr::video::E_PIXEL_SHADER_TYPE gl_ps, irr::video::E_PIXEL_SHADER_TYPE dx_ps ) {
	if( !supported )
		return;

	irr::video::IVideoDriver* driver = smgr->getVideoDriver( );
	irr::video::IGPUProgrammingServices* gpu = driver->getGPUProgrammingServices( );

	irr::io::IReadFile* psfile = (driver->getDriverType( ) == irr::video::EDT_OPENGL) ? file_gl : file_dx;
	if( !psfile ) {
		setShader( NULL, NULL, irr::video::EPST_PS_1_1, irr::video::EPST_PS_1_1, &mat );
		eff = PP_DIRECT;
		disable( );
		return;
	}

	const long size = psfile->getSize( );
	if( size == 0 ) {
		setShader( NULL, NULL, irr::video::EPST_PS_1_1, irr::video::EPST_PS_1_1, &mat );
		eff = PP_DIRECT;
		disable( );
		return;
	}
	irr::c8* ps = new irr::c8[ size + 1 ];
	psfile->read( ps, size );
	ps[ size ] = '\0';

	setShader( ps, ps, gl_ps, dx_ps, &mat );
	delete [] ps;
}
void CEffectPostProc::configureInput( irr::u8 i ) {
	if( !supported )
		return;

	if( !enabled )
		return;

	switch( i ) {
		case 0u:
			if( useTextureOptimize == irr::video::EMT_SOLID || !hasInput( 1u ) ) {
				mat.setTexture( 0u, getInputTexture( 0u ) );
				mat2.setTexture( 0u, getInputTexture( 0u ) );
				mat3.setTexture( 0u, getInputTexture( 0u ) );
				mat4.setTexture( 0u, getInputTexture( 0u ) );
			}
			break;
		case 1u:
			if( useTextureOptimize != irr::video::EMT_SOLID ) {
				mat.setTexture( 0u, getInputTexture( 1u ) );
				mat2.setTexture( 0u, getInputTexture( 1u ) );
				mat3.setTexture( 0u, getInputTexture( 1u ) );
				mat4.setTexture( 0u, getInputTexture( 1u ) );
			} else {
				mat.setTexture( 1u, getInputTexture( 1u ) );
				mat2.setTexture( 1u, getInputTexture( 1u ) );
				mat3.setTexture( 1u, getInputTexture( 1u ) );
				mat4.setTexture( 1u, getInputTexture( 1u ) );
			}
			break;
	}
}
IPostProc* CEffectPostProc::getInput( irr::u8 n ) {
	switch( eff ) {
		case PP_BLUR:
		case PP_BLURDOFFAR:
		case PP_BLURDOFNEAR:
		case PP_BLURDOF:
		case PP_BLOOM:
		case PP_GLOOM:
		case PP_NIGHTVISION:
		case PP_MONITOR:
			return bound[ 0u ]->getInput( n );
			break;
	}
	return IPostProc::getInput( n );
}
void CEffectPostProc::setQuality( POSTPROC_QUALITY q ) {
	IPostProc::setQuality( q );
	for( irr::u8 i = 0u; i != bound.size( ); ++ i )
		if( bound[ i ] != NULL )
			bound[ i ]->setQuality( q );
}
void CEffectPostProc::setParameters( irr::f32 para1, irr::f32 para2, irr::f32 para3, irr::f32 para4, irr::f32 para5, irr::f32 para6, irr::f32 para7, irr::f32 para8 ) {
	if( !supported )
		return;

	switch( eff ) {
		case PP_BLUR:
			bound[ 0u ]->setParameters( para1 );
			break;
		case PP_SHARPEN:
			bound[ 0u ]->setParameters( para1, para2 );
			break;
		case PP_BLURDOFFAR:
		case PP_BLURDOFNEAR:
			bound[ 0u ]->setParameters( para1, para2, para3 );
			break;
		case PP_BLURDOF:
			bound[ 0u ]->setParameters( para3, para4, para5 );
			bound[ 1u ]->setParameters( para1, para2, para5 );
			para3 = para5;
			break;
		case PP_BLOOM:
			bound[ 0u ]->setParameters( para1 );
			bound[ 1u ]->setParameters( para2 );
			para1 = para3;
			break;
		case PP_GLOOM:
			bound[ 0u ]->setParameters( para1 );
			bound[ 1u ]->setParameters( para2 );
			para1 = para3;
			break;
		case PP_OCCLUSION:
			para3 = para1;
			para4 = para2;
			para1 = 1.0f / output->getSize( ).Width;
			para2 = 1.0f / output->getSize( ).Height;
			break;
		case PP_NIGHTVISION:
			bound[ 0u ]->setParameters( -0.5f, para1 );
			bound[ 1u ]->setParameters( para2 );
			bound[ 2u ]->setParameters( para3 );
			para1 = 0.2f;
			para2 = 2.0f;
			para3 = 0.5f;
			break;
		case PP_MONITOR:
			bound[ 0u ]->setParameters( 1.0f, 0.9f, 0.8f, 1.0f - para4 );
			bound[ 1u ]->setParameters( para3 );
			bound[ 2u ]->setParameters( para5, para5, para6 );
			bound[ 3u ]->setParameters( para2, para2, 0.5f, 0.5f );
			if( para1 == 0.0f )
				para3 = 0.0f;
			else
				para3 = 1.0f / para1;
			para1 = 4.0f;
			para2 = 0.0f;
			break;
		case PP_GREYSCALE:
			para2 = para1;
			para3 = para1;
			para4 = 1.0f;
			break;
		case PP_SEPIA:
			para4 = 1.0f;
			break;
		case PP_SATURATE:
			para4 = 1.0f - para1;
			para1 = 1.0f;
			para2 = 1.0f;
			para3 = 1.0f;
			break;
		case PP_WATERCOLOR:
			bound[ 0u ]->setParameters( 0.5f );
			bound[ 0u ]->requireFirstRender( );
			bound[ 1u ]->setParameters( para1, para1, para1 );
			bound[ 2u ]->setParameters( para6 );
			bound[ 3u ]->setParameters( para2, para2 );
			bound[ 4u ]->setParameters( para3 );
			bound[ 5u ]->setParameters( para4, para5 );
			para1 = para4;
			para2 = para5;
			break;
	}
	irr::f32 t;
	switch( eff ) {
		case PP_HBLURDOFFAR:
		case PP_VBLURDOFFAR:
		case PP_BLURDOFFAR:
		case PP_HBLURDOFNEAR:
		case PP_VBLURDOFNEAR:
		case PP_BLURDOFNEAR:
		case PP_BLURDOF:
			if( para1 > para2 ) {
				t = para1;
				para1 = para2;
				para2 = t;
			}
			break;
	}
	scEffect.setParams( para1, para2, para3, para4, para5, para6, para7, para8 );
}
void CEffectPostProc::renderEffect( irr::video::ITexture* target ) {
	if( !hasInput( 0u ) )
		return;

	irr::video::IVideoDriver* driver = smgr->getVideoDriver( );

	bool noNeed = false;
	irr::video::SMaterial* usemat = &mat;

	switch( eff ) {
		case PP_DIRECT:
			if( quality <= PPQ_CRUDE )
				noNeed = true;
			else if( target == NULL )
				noNeed = getInputTexture( 0u )->getSize( ) == driver->getScreenSize( );
			else
				noNeed = getInputTexture( 0u )->getSize( ) == target->getSize( );
			break;
		case PP_PUNCH:
			noNeed = scEffect.getParam( 2u ) == 0.0f && scEffect.getParam( 3u ) == 0.0f;
			break;
		case PP_PIXELATE:
			if( quality <= PPQ_CRUDE )
				noNeed = true;
			else
				noNeed = scEffect.getParam( 0u ) == 0.0f || scEffect.getParam( 1u ) == 0.0f;
			break;
		case PP_POSTERIZE:
			noNeed = scEffect.getParam( 0u ) <= 1.0f || scEffect.getParam( 0u ) >= 255.0f;
			break;
		case PP_DARKEN:
		case PP_LIGHTEN:
			noNeed = scEffect.getParam( 0u ) == 1.0f;
			break;
		case PP_RANGE:
			noNeed = scEffect.getParam( 0u ) == 0.0f && scEffect.getParam( 1u ) == 1.0f;
			break;
		case PP_CURVES:
			noNeed = scEffect.getParam( 0u ) == 1.0f && scEffect.getParam( 1u ) == 1.0f && scEffect.getParam( 2u ) == 1.0f;
			break;
		case PP_MONITOR:
		case PP_VIGNETTE:
			noNeed = scEffect.getParam( 0u ) <= 0.0f || (scEffect.getParam( 1u ) >= 1.0f && scEffect.getParam( 2u ) >= 1.0f) || (scEffect.getParam( 1u ) <= 0.0f && scEffect.getParam( 2u ) <= 0.0f);
			break;
		case PP_NOISE:
		case PP_PURENOISE:
		case PP_COLORNOISE:
			noNeed = scEffect.getParam( 0u ) == 0.0f;
			break;
		case PP_TINT:
			noNeed = scEffect.getParam( 3u ) == 0.0f;
			break;
		case PP_BLUR:
		case PP_HBLUR:
		case PP_VBLUR:
			noNeed = scEffect.getParam( 0u ) == 0.0f;
			if( quality <= PPQ_FAST )
				usemat = &mat2;
			else if( quality >= PPQ_GOOD )
				usemat = &mat3;
			break;
		case PP_WATERCOLOR:
		case PP_SHARPEN:
		case PP_HSHARPEN:
		case PP_VSHARPEN:
			noNeed = scEffect.getParam( 0u ) == 0.0f || scEffect.getParam( 1u ) == 0.0f;
			if( quality <= PPQ_FAST )
				usemat = &mat2;
			else if( quality >= PPQ_GOOD )
				usemat = &mat3;
			break;
		case PP_BIBLUR:
			noNeed = scEffect.getParam( 0u ) == 0.0f && scEffect.getParam( 1u ) == 0.0f;
			if( quality <= PPQ_CRUDE )
				usemat = &mat2;
			else if( quality >= PPQ_DEFAULT )
				usemat = &mat3;
			break;
		case PP_BLURDOFFAR:
		case PP_HBLURDOFFAR:
		case PP_VBLURDOFFAR:
			noNeed = scEffect.getParam( 0u ) == scEffect.getParam( 1u ) || scEffect.getParam( 2u ) == 0.0f;
			if( quality <= PPQ_FAST )
				usemat = &mat2;
			break;
		case PP_BLURDOFNEAR:
		case PP_HBLURDOFNEAR:
		case PP_VBLURDOFNEAR:
		case PP_BLURDOF:
			noNeed = scEffect.getParam( 1u ) <= 0.0f || scEffect.getParam( 0u ) == scEffect.getParam( 1u ) || scEffect.getParam( 2u ) == 0.0f;
			if( quality <= PPQ_FAST )
				usemat = &mat2;
			break;
		case PP_LINEARBLUR:
			noNeed = scEffect.getParam( 0u ) == 0.0f && scEffect.getParam( 1u ) == 0.0f;
			if( quality <= PPQ_FAST )
				usemat = &mat2;
			else if( quality >= PPQ_GOOD )
				usemat = &mat3;
			break;
		case PP_RADIALBLUR:
		case PP_RADIALBEAM:
		case PP_ROTATIONALBLUR:
			noNeed = scEffect.getParam( 2u ) == 0.0f && scEffect.getParam( 3u ) == 0.0f;
			if( quality <= PPQ_FAST )
				usemat = &mat2;
			else if( quality >= PPQ_GOOD )
				usemat = &mat3;
			break;
		case PP_HAZE:
		case PP_HAZEDEPTH:
			noNeed = scEffect.getParam( 0u ) == 0.0f || scEffect.getParam( 1u ) == 0.0f || !hasInput( 1u );
			break;
		case PP_OVERLAY:
		case PP_OVERLAYNEG:
			noNeed = scEffect.getParam( 0u ) == 0.0f || !hasInput( 0u );
			break;
		case PP_BLOOM:
		case PP_GLOOM:
			noNeed = scEffect.getParam( 0u ) == 0.0f;
			break;
		case PP_OCCLUSION:
			noNeed = scEffect.getParam( 0u ) == 0.0f;
			if( quality <= PPQ_FAST )
				usemat = &mat2;
			break;
		case PP_MOTIONBLUR:
			noNeed = scEffect.getParam( 0u ) == 1.0f;
			break;
	}

	if( !supported || !enabled || noNeed ) {
		requestInput( 0u, target );
		return;
	}

	if( eff == PP_MOTIONBLUR && firstRender ) {
		requestInput( 1u, target );
		return;
	}

	if( eff != PP_PURENOISE ) {
		if( useTextureOptimize != irr::video::EMT_SOLID )
			requestInput( 0u, target );
		else
			requestInput( 0u );
	}

	requestInput( 1u );

	if( timer )
		scEffect.setTime( timer->getTime( ) * 0.001f );
	driver->setRenderTarget( target, false, false );
	driver->setMaterial( *usemat );
	driver->drawMeshBuffer( Mesh );
}
