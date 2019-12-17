#include "CTransitionPostProc.h"

CTransitionPostProc::CTransitionPostProc( IPostProc* follow1, IPostProc* follow2, dimension2di sOut, ITexture* m ) : IPostProc( follow1, sOut ) {
	IVideoDriver *driver = smgr->getVideoDriver( );
	IGPUProgrammingServices* gpu = driver->getGPUProgrammingServices( );
	
	tmap = m;
	ps = 0.0f;
	
	if( supported ) {
		s32 matid;
		switch( driver->getDriverType( ) ) {
			case EDT_OPENGL:
				matid = gpu->addHighLevelShaderMaterial( GL_V_FULLSCREEN, "main", EVST_VS_1_1, GL_TRANSMAP, "main", EPST_PS_1_1, &shadercallback, EMT_TRANSPARENT_ALPHA_CHANNEL, 2 );
				break;
			case EDT_DIRECT3D8:
			case EDT_DIRECT3D9:
			default:
				matid = gpu->addHighLevelShaderMaterial( DX_V_FULLSCREEN, "main", EVST_VS_1_1, DX_TRANSMAP, "main", EPST_PS_1_1, &shadercallback, EMT_TRANSPARENT_ALPHA_CHANNEL, 0 );
		}
		
		mat.MaterialType = (E_MATERIAL_TYPE) matid;
		mat.Wireframe = false;
		mat.Lighting = false;
		mat.FogEnable = false;
		mat.ZBuffer = 0;
		mat.setTexture( 1, tmap );
		mat.TextureLayer[0].TextureWrap = ETC_CLAMP;
		mat.TextureLayer[1].TextureWrap = ETC_CLAMP;
	}
	setInput( 0, follow1 );
	setInput( 1, follow2 );
	setBlend( 0.0f );
}
void CTransitionPostProc::configureInput( u8 n ) {
	if( !supported )
		return;
	if( !enabled )
		return;
	if( n == 1u )
		if( ps != 0.0f && ps != 1.0f ) {
			if( follows[1] != NULL )
				mat.setTexture( 0, follows[1]->getOutputTexture( ) );
			else
				mat.setTexture( 0, NULL );
		}
}
void CTransitionPostProc::setBlend( f32 p ) {
	if( !supported ) {
		ps = fmaxf( 0.0f, fminf( 1.0f, p ) );
		return;
	}
	if( p <= 0.0f || p >= 1.0f ) {
		mat.setTexture( 0, NULL );
	} else if( ps == 0.0f || ps == 1.0f ) {
		if( follows[1] != NULL )
			mat.setTexture( 0, follows[1]->getOutputTexture( ) );
		else
			mat.setTexture( 0, NULL );
	}
	ps = fmaxf( 0.0f, fminf( 1.0f, p ) );
	shaderparameters[0] = ps * 1.2f - 0.1f;
	shadercallback.setShaderParameters( &shaderparameters[0] );
}
void CTransitionPostProc::enable( bool yes ) {
	if( yes ) {
		if( !enabled )
			if( follows[1] != NULL )
				if( ps != 0.0f && ps != 1.0f )
					mat.setTexture( 0, follows[1]->getOutputTexture( ) );
	} else if( enabled )
		mat.setTexture( 0, NULL );
	IPostProc::enable( yes );
}
void CTransitionPostProc::renderEffect( ITexture *target ) {
	IVideoDriver *driver = smgr->getVideoDriver( );
	
	if( !supported || !enabled ) {
		if( ps > 0.5f )
			requestInput( 1, target );
		else
			requestInput( 0, target );
		return;
	}
	
	if( ps == 0.0f ) {
		requestInput( 0, target );
		return;
	}
	if( ps == 1.0f ) {
		requestInput( 1, target );
		return;
	}
	requestInput( 0, target );
	requestInput( 1 );
	
	driver->setRenderTarget( target, false, false );
	driver->setMaterial( mat );
	driver->drawMeshBuffer( Mesh );
}
