#include "CEffectPostProc.h"

CEffectPostProc::CEffectPostProc( IPostProc* follow1, dimension2di sOut, POSTPROC_EFFECTID effectID, f32 para1, f32 para2, f32 para3, f32 para4 ) : IPostProc( follow1, sOut ) {
	IVideoDriver *driver = smgr->getVideoDriver( );
	
	// A rendered-to Texture0 is very fast, but a rendered-to Texture1 is slow. So if we have 2 textures, we have the option to render them in 2 materials
	// Set useTextureOptimize to EMT_TRANSPARENT_ADD_COLOR, EMT_TRANSPARENT_ALPHA_CHANNEL, EMT_TRANSPARENT_ALPHA_REF, etc. to render the second texture over
	// the first using the chosen method (shader is applied to the second texture)
	useTextureOptimize = EMT_SOLID;
	IPostProc* follow2 = NULL;
	
	if( effectID >= PP_COUNT )
		effectID = PP_DIRECT;
	
	eff = effectID;
	if( supported ) {
		CEffectPostProc* t1 = NULL;
		CEffectPostProc* t2 = NULL;
		CEffectPostProc* t3 = NULL;
		switch( effectID ) {
			case PP_BLUR:
				if( para1 == FLOAT_DEFAULTVALUE )	para1 = 0.01f;
				t1 = new CEffectPostProc( follow1, sOut, PP_HBLUR );
				follow1 = (IPostProc*) t1;
				bound.reallocate( 1 );
				bound.push_back( t1 );
				effectID = PP_VBLUR;
				break;
			case PP_BLURFINE:
				if( para1 == FLOAT_DEFAULTVALUE )	para1 = 0.01f;
				t1 = new CEffectPostProc( follow1, sOut, PP_HBLURFINE );
				follow1 = (IPostProc*) t1;
				bound.reallocate( 1 );
				bound.push_back( t1 );
				effectID = PP_VBLURFINE;
				break;
			case PP_BLOOM:
				if( para1 == FLOAT_DEFAULTVALUE )	para1 = 4.0f;
				if( para2 == FLOAT_DEFAULTVALUE )	para2 = 0.005f;
				if( para3 == FLOAT_DEFAULTVALUE )	para3 = 2.0f;
				bound.reallocate( 2 );
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
				bound.reallocate( 2 );
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
				bound.reallocate( 3 );
				t1 = new CEffectPostProc( follow1, sOut, PP_RANGE );
				t2 = new CEffectPostProc( t1, sOut, PP_BLUR );
				t3 = new CEffectPostProc( t2, sOut, PP_NOISE );
				follow1 = (IPostProc*) t3;
				bound.push_back( t1 );
				bound.push_back( t2 );
				bound.push_back( t3 );
				effectID = PP_TINT;
				break;
		}
		switch( effectID ) {
			case PP_BLACK:
				setShader( GL_BLACK, DX_BLACK, EPST_PS_1_1 );
				break;
			case PP_DIRECT:
				setShader( GL_DIRECT, DX_DIRECT, EPST_PS_1_1 );
				break;
			case PP_DEPTH:
				setShader( GL_DEPTH, DX_DEPTH, EPST_PS_1_1 );
				break;
			case PP_OCCLUSION:
				if( para1 == FLOAT_DEFAULTVALUE )	para1 = 10.0f;
				setShader( GL_OCCLUSION, DX_OCCLUSION, EPST_PS_1_1 );
				break;
			case PP_DARKEN:
				if( para1 == FLOAT_DEFAULTVALUE )	para1 = 1.0f;
				setShader( GL_DARKEN, DX_DARKEN, EPST_PS_1_1 );
				break;
			case PP_LIGHTEN:
				if( para1 == FLOAT_DEFAULTVALUE )	para1 = 1.0f;
				setShader( GL_LIGHTEN, DX_LIGHTEN, EPST_PS_1_1 );
				break;
			case PP_RANGE:
				if( para1 == FLOAT_DEFAULTVALUE )	para1 = 0.0f;
				if( para2 == FLOAT_DEFAULTVALUE )	para2 = 1.0f;
				setShader( GL_RANGE, DX_RANGE, EPST_PS_1_1 );
				break;
			case PP_INVERT:
				setShader( GL_INVERT, DX_INVERT, EPST_PS_1_1 );
				break;
			case PP_CURVES:
				if( para2 == FLOAT_DEFAULTVALUE && para3 == FLOAT_DEFAULTVALUE ) {
					para2 = para1;
					para3 = para1;
				}
				if( para1 == FLOAT_DEFAULTVALUE )	para1 = 1.0f;
				if( para2 == FLOAT_DEFAULTVALUE )	para2 = 1.0f;
				if( para3 == FLOAT_DEFAULTVALUE )	para3 = 1.0f;
				setShader( GL_CURVES, DX_CURVES, EPST_PS_1_1 );
				break;
			case PP_TINT:
				if( para2 == FLOAT_DEFAULTVALUE && para3 == FLOAT_DEFAULTVALUE ) {
					para2 = para1;
					para3 = para1;
				}
				if( para1 == FLOAT_DEFAULTVALUE )	para1 = 1.0f;
				if( para2 == FLOAT_DEFAULTVALUE )	para2 = 1.0f;
				if( para3 == FLOAT_DEFAULTVALUE )	para3 = 1.0f;
				setShader( GL_TINT, DX_TINT, EPST_PS_1_1 );
				break;
			case PP_GREYSCALE:
				if( para1 == FLOAT_DEFAULTVALUE )	para1 = 1.0f;
				para2 = para1;
				para3 = para2;
				setShader( GL_TINT, DX_TINT, EPST_PS_1_1 );
				break;
			case PP_SEPIA:
				para1 = 2.0f;
				para2 = 1.0f;
				para3 = 0.7f;
				setShader( GL_TINT, DX_TINT, EPST_PS_1_1 );
				break;
			case PP_NOISE:
				if( para1 == FLOAT_DEFAULTVALUE )	para1 = 0.1f;
				setShader( GL_NOISE, DX_NOISE, EPST_PS_1_1 );
				break;
			case PP_COLORNOISE:
				if( para1 == FLOAT_DEFAULTVALUE )	para1 = 0.1f;
				setShader( GL_COLORNOISE, DX_COLORNOISE, EPST_PS_1_1 );
				break;
			case PP_HBLUR:
				if( para1 == FLOAT_DEFAULTVALUE )	para1 = 0.01f;
				setShader( GL_HBLUR, DX_HBLUR, EPST_PS_2_0 );
				break;
			case PP_VBLUR:
				if( para1 == FLOAT_DEFAULTVALUE )	para1 = 0.01f;
				setShader( GL_VBLUR, DX_VBLUR, EPST_PS_2_0 );
				break;
			case PP_LINEARBLUR:
				if( para2 == FLOAT_DEFAULTVALUE ) para2 = 0.0f;
				if( para1 == FLOAT_DEFAULTVALUE ) {
					para1 = 0.01f;
					para2 = 0.0f;
				}
				setShader( GL_LINEARBLUR, DX_LINEARBLUR, EPST_PS_2_0 );
				break;
			case PP_RADIALBLUR:
				if( para1 == FLOAT_DEFAULTVALUE ) para1 = 0.5f;
				if( para2 == FLOAT_DEFAULTVALUE ) para2 = 0.5f;
				if( para3 == FLOAT_DEFAULTVALUE ) para3 = 0.01f;
				if( para4 == FLOAT_DEFAULTVALUE ) para4 = para3;
				setShader( GL_RADIALBLUR, DX_RADIALBLUR, EPST_PS_2_0 );
				break;
			case PP_HBLURFINE:
				if( para1 == FLOAT_DEFAULTVALUE )	para1 = 0.01f;
				setShader( GL_HBLURFINE, DX_HBLURFINE, EPST_PS_2_0 );
				break;
			case PP_VBLURFINE:
				if( para1 == FLOAT_DEFAULTVALUE )	para1 = 0.01f;
				setShader( GL_VBLURFINE, DX_VBLURFINE, EPST_PS_2_0 );
				break;
			case PP_LINEARBLURFINE:
				if( para2 == FLOAT_DEFAULTVALUE ) para2 = 0.0f;
				if( para1 == FLOAT_DEFAULTVALUE ) {
					para1 = 0.01f;
					para2 = 0.0f;
				}
				setShader( GL_LINEARBLURFINE, DX_LINEARBLURFINE, EPST_PS_2_0 );
				break;
			case PP_RADIALBLURFINE:
				if( para1 == FLOAT_DEFAULTVALUE ) para1 = 0.5f;
				if( para2 == FLOAT_DEFAULTVALUE ) para2 = 0.5f;
				if( para3 == FLOAT_DEFAULTVALUE ) para3 = 0.01f;
				if( para4 == FLOAT_DEFAULTVALUE ) para4 = para3;
				setShader( GL_RADIALBLURFINE, DX_RADIALBLURFINE, EPST_PS_2_0 );
				break;
			case PP_MOTIONBLUR:
				if( para1 == FLOAT_DEFAULTVALUE )	para1 = 0.05f;
				follow2 = follow1;
				follow1 = this;
				useTextureOptimize = EMT_TRANSPARENT_ALPHA_CHANNEL;
				setShader( GL_MOTIONBLUR2, DX_MOTIONBLUR2, EPST_PS_1_1 );
				break;
			case PP_OVERLAY:
				if( para1 == FLOAT_DEFAULTVALUE )	para1 = 1.0f;
				useTextureOptimize = EMT_TRANSPARENT_ADD_COLOR;
				setShader( GL_OVERLAY2, DX_OVERLAY2, EPST_PS_1_1 );
				break;
			case PP_OVERLAYNEG:
				if( para1 == FLOAT_DEFAULTVALUE )	para1 = 1.0f;
				setShader( GL_OVERLAYNEG, DX_OVERLAYNEG, EPST_PS_1_1 );
				break;
		}
		
		if( para1 == FLOAT_DEFAULTVALUE )
			para1 = 0.0f;
		if( para2 == FLOAT_DEFAULTVALUE )
			para2 = 0.0f;
		if( para3 == FLOAT_DEFAULTVALUE )
			para3 = 0.0f;
		if( para4 == FLOAT_DEFAULTVALUE )
			para4 = 0.0f;
		
		mat.Wireframe = false;
		mat.Lighting = false;
		mat.FogEnable = false;
		mat.ZBuffer = 0;
		mat.TextureLayer[0].TextureWrap = ETC_CLAMP;
		mat.TextureLayer[1].TextureWrap = ETC_CLAMP;
		
		setParameters( para1, para2, para3, para4 );
	}
	setInput( 0, follow1 );
	setInput( 1, follow2 );
}
CEffectPostProc::CEffectPostProc( IPostProc* follow1, dimension2di sOut, const c8* prog_gl, const c8* prog_dx, E_PIXEL_SHADER_TYPE type_ps, E_MATERIAL_TYPE optimize, f32 para1, f32 para2, f32 para3, f32 para4 ) : IPostProc( follow1, sOut ) {
	IVideoDriver *driver = smgr->getVideoDriver( );
	useTextureOptimize = optimize;
	eff = PP_DIRECT; // Pretend we are the direct renderer
	
	if( supported ) {
		setShader( prog_gl, prog_dx, type_ps );
		mat.Wireframe = false;
		mat.Lighting = false;
		mat.FogEnable = false;
		mat.ZBuffer = 0;
		mat.TextureLayer[0].TextureWrap = ETC_CLAMP;
		mat.TextureLayer[1].TextureWrap = ETC_CLAMP;
		setParameters( para1, para2, para3, para4 );
	}
	setInput( 0, follow1 );
}
CEffectPostProc::~CEffectPostProc( ) {
	for( u8 i = 0u; i != bound.size( ); ++ i )
		delete bound[ i ];
}
void CEffectPostProc::enable( bool yes ) {
	if( yes ) {
		if( !enabled ) {
			if( follows[0] != NULL )
				if( useTextureOptimize == EMT_SOLID || follows[1] == NULL )
					mat.setTexture( 0, follows[0]->getOutputTexture( ) );
			if( follows[1] != NULL ) {
				if( useTextureOptimize != EMT_SOLID )
					mat.setTexture( 0, follows[1]->getOutputTexture( ) );
				else
					mat.setTexture( 1, follows[1]->getOutputTexture( ) );
			}
		}
	} else if( enabled ) {
		mat.setTexture( 0, NULL );
		mat.setTexture( 1, NULL );
	}
	IPostProc::enable( yes );
	for( u8 i = 0u; i != bound.size( ); ++ i )
		if( bound[ i ] != NULL )
			bound[ i ]->enable( yes );
}
void CEffectPostProc::setShader( const c8* prog_gl, const c8* prog_dx, E_PIXEL_SHADER_TYPE type_ps ) {
	if( !supported )
		return;
	
	IVideoDriver *driver = smgr->getVideoDriver( );
	IGPUProgrammingServices* gpu = driver->getGPUProgrammingServices( );
	
	u32 matid;
	switch( driver->getDriverType( ) ) {
		case EDT_OPENGL:
			if( useTextureOptimize != EMT_SOLID )
				matid = gpu->addHighLevelShaderMaterial( GL_V_FULLSCREEN, "main", EVST_VS_1_1, prog_gl, "main", type_ps, &shadercallback, useTextureOptimize, 1 );
			else
				matid = gpu->addHighLevelShaderMaterial( GL_V_FULLSCREEN, "main", EVST_VS_1_1, prog_gl, "main", type_ps, &shadercallback, EMT_SOLID, 4 );
			break;
		case EDT_DIRECT3D8:
		case EDT_DIRECT3D9:
		default:
			if( useTextureOptimize != EMT_SOLID )
				matid = gpu->addHighLevelShaderMaterial( DX_V_FULLSCREEN, "main", EVST_VS_1_1, prog_dx, "main", type_ps, &shadercallback, useTextureOptimize, 0 );
			else
				matid = gpu->addHighLevelShaderMaterial( DX_V_FULLSCREEN, "main", EVST_VS_1_1, prog_dx, "main", type_ps, &shadercallback, EMT_SOLID, 0 );
	}
	mat.MaterialType = (E_MATERIAL_TYPE) matid;
}
void CEffectPostProc::configureInput( u8 i ) {
	if( !supported )
		return;
	
	if( !enabled )
		return;
	
	switch( i ) {
		case 0u:
			if( useTextureOptimize == EMT_SOLID || follows[1] == NULL ) {
				if( follows[0] == NULL )
					mat.setTexture( 0, NULL );
				else
					mat.setTexture( 0, follows[0]->getOutputTexture( ) );
			}
			break;
		case 1u:
			if( useTextureOptimize != EMT_SOLID ) {
				if( follows[1] == NULL )
					mat.setTexture( 0, NULL );
				else
					mat.setTexture( 0, follows[1]->getOutputTexture( ) );
			} else {
				if( follows[1] == NULL )
					mat.setTexture( 1, NULL );
				else
					mat.setTexture( 1, follows[1]->getOutputTexture( ) );
			}
			break;
	}
}
IPostProc* CEffectPostProc::getInput( u8 n ) {
	switch( eff ) {
		case PP_BLUR:
		case PP_BLURFINE:
		case PP_BLOOM:
		case PP_GLOOM:
		case PP_NIGHTVISION:
			return bound[0]->getInput( n );
			break;
	}
	return IPostProc::getInput( n );
}
void CEffectPostProc::setParameters( f32 para1, f32 para2, f32 para3, f32 para4, f32 para5, f32 para6, f32 para7, f32 para8 ) {
	if( !supported )
		return;
	
	switch( eff ) {
		case PP_BLUR:
			bound[0]->setParameters( para1 );
			break;
		case PP_BLURFINE:
			bound[0]->setParameters( para1 );
			break;
		case PP_BLOOM:
			bound[0]->setParameters( para1 );
			bound[1]->setParameters( para2 );
			para1 = para3;
			break;
		case PP_GLOOM:
			bound[0]->setParameters( para1 );
			bound[1]->setParameters( para2 );
			para1 = para3;
			break;
		case PP_OCCLUSION:
			para3 = para1;
			para4 = para2;
			para1 = 1.0f / output->getOriginalSize( ).Width;
			para2 = 1.0f / output->getOriginalSize( ).Height;
			break;
		case PP_NIGHTVISION:
			bound[0]->setParameters( -0.5f, para1 );
			bound[1]->setParameters( para2 );
			bound[2]->setParameters( para3 );
			para1 = 0.2f;
			para2 = 2.0f;
			para3 = 0.5f;
			break;
	}
	shaderparameters[0] = para1;
	shaderparameters[1] = para2;
	shaderparameters[2] = para3;
	shaderparameters[3] = para4;
	shaderparameters[4] = para5;
	shaderparameters[5] = para6;
	shaderparameters[6] = para7;
	shaderparameters[7] = para8;
	shadercallback.setShaderParameters( &shaderparameters[0] );
}
void CEffectPostProc::renderEffect( ITexture *target ) {
	IVideoDriver *driver = smgr->getVideoDriver( );
	
	bool noNeed = false;
	
	switch( eff ) {
		case PP_DARKEN:
		case PP_LIGHTEN:
			noNeed = shadercallback.getParam( 0u ) == 1.0f;
			break;
		case PP_RANGE:
			noNeed = shadercallback.getParam( 0u ) == 0.0f && shadercallback.getParam( 1u ) == 1.0f;
			break;
		case PP_CURVES:
			noNeed = shadercallback.getParam( 0u ) == 1.0f && shadercallback.getParam( 1u ) == 1.0f && shadercallback.getParam( 2u ) == 1.0f;
			break;
		case PP_NOISE:
		case PP_COLORNOISE:
			noNeed = shadercallback.getParam( 0u ) == 0.0f;
			break;
		case PP_BLUR:
		case PP_HBLUR:
		case PP_VBLUR:
		case PP_BLURFINE:
		case PP_HBLURFINE:
		case PP_VBLURFINE:
			noNeed = shadercallback.getParam( 0u ) == 0.0f;
			break;
		case PP_LINEARBLUR:
		case PP_LINEARBLURFINE:
			noNeed = shadercallback.getParam( 0u ) == 0.0f && shadercallback.getParam( 1u ) == 0.0f;
			break;
		case PP_RADIALBLUR:
		case PP_RADIALBLURFINE:
			noNeed = shadercallback.getParam( 2u ) == 0.0f && shadercallback.getParam( 3u ) == 0.0f;
			break;
		case PP_OVERLAY:
		case PP_OVERLAYNEG:
		case PP_BLOOM:
		case PP_GLOOM:
			noNeed = shadercallback.getParam( 0u ) == 0.0f;
			break;
		case PP_MOTIONBLUR:
			noNeed = shadercallback.getParam( 0u ) == 1.0f;
			break;
	}
	
	if( !supported || !enabled || noNeed ) {
		requestInput( 0, target );
		return;
	}
	
	if( useTextureOptimize != EMT_SOLID )
		requestInput( 0, target );
	else
		requestInput( 0 );
	requestInput( 1 );
	
	driver->setRenderTarget( target, false, false );
	driver->setMaterial( mat );
	driver->drawMeshBuffer( Mesh );
}
