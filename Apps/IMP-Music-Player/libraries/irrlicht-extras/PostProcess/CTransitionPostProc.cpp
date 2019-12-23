#include <irrlicht.h>
#include "CTransitionPostProc.h"

CTransitionPostProc::CTransitionPostProc( IPostProc* follow1, IPostProc* follow2, DIMENSION2D sOut, irr::video::ITexture* m ) : IPostProc( follow1, sOut ) {
	irr::video::IVideoDriver* driver = smgr->getVideoDriver( );
	irr::video::IGPUProgrammingServices* gpu = driver->getGPUProgrammingServices( );
	
	tmap = m;
	ps = 0.0f;
	
	if( supported ) {
		scTrans = CShaderCallback( );
		scTrans.usesnum = 1u;

		irr::s32 matid, matidFast;
		switch( driver->getDriverType( ) ) {
			case irr::video::EDT_OPENGL:
				matid = gpu->addHighLevelShaderMaterial( GL_V_FULLSCREEN, "main", irr::video::EVST_VS_1_1, GL_TRANSMAP, "main", irr::video::EPST_PS_1_1, &scTrans, irr::video::EMT_TRANSPARENT_ALPHA_CHANNEL, 2 );
				matidFast = gpu->addHighLevelShaderMaterial( GL_V_FULLSCREEN, "main", irr::video::EVST_VS_1_1, GL_TRANSMAPFAST, "main", irr::video::EPST_PS_1_1, &scTrans, irr::video::EMT_SOLID, 2 );
				break;
			case irr::video::EDT_DIRECT3D9:
				matid = gpu->addHighLevelShaderMaterial( DX_V_FULLSCREEN, "main", irr::video::EVST_VS_1_1, DX_TRANSMAP, "main", irr::video::EPST_PS_2_0, &scTrans, irr::video::EMT_TRANSPARENT_ALPHA_CHANNEL, 0 );
				matidFast = gpu->addHighLevelShaderMaterial( DX_V_FULLSCREEN, "main", irr::video::EVST_VS_1_1, DX_TRANSMAPFAST, "main", irr::video::EPST_PS_1_4, &scTrans, irr::video::EMT_SOLID, 0 );
				break;
		}
		
		mat = irr::video::SMaterial( );
		mat.MaterialType = (irr::video::E_MATERIAL_TYPE) matid;
		mat.Wireframe = false;
		mat.Lighting = false;
		mat.FogEnable = false;
		mat.ZBuffer = 0;
		mat.setTexture( 1u, tmap );
		mat.TextureLayer[ 0u ].TextureWrapU = irr::video::ETC_CLAMP_TO_EDGE;
		mat.TextureLayer[ 0u ].TextureWrapV = irr::video::ETC_CLAMP_TO_EDGE;
		mat.TextureLayer[ 1u ].TextureWrapU = irr::video::ETC_CLAMP_TO_EDGE;
		mat.TextureLayer[ 1u ].TextureWrapV = irr::video::ETC_CLAMP_TO_EDGE;
		mat.TextureLayer[ 0u ].BilinearFilter = false;
		
		mat = irr::video::SMaterial( mat );
		matFast.MaterialType = (irr::video::E_MATERIAL_TYPE) matidFast;
		matFast.TextureLayer[ 1u ].BilinearFilter = false;
	}
	setInput( 0u, follow1 );
	setInput( 1u, follow2 );
	setBlend( 0.0f );
}
void CTransitionPostProc::configureInput( irr::u8 n ) {
	if( !supported )
		return;
	if( n == 1u ) {
		mat.setTexture( 0u, getInputTexture( 1u ) );
		matFast.setTexture( 0u, getInputTexture( 1u ) );
	}
}
void CTransitionPostProc::setBlend( irr::f32 p ) {
	ps = irr::core::max_( 0.0f, irr::core::min_( 1.0f, p ) );
	if( !supported )
		return;
}
void CTransitionPostProc::renderEffect( irr::video::ITexture* target ) {
	irr::video::IVideoDriver* driver = smgr->getVideoDriver( );
	
	if( !supported || !enabled ) {
		if( ps > 0.5f )
			requestInput( 1u, target );
		else
			requestInput( 0u, target );
		return;
	}
	
	if( ps == 0.0f ) {
		requestInput( 0u, target );
		return;
	}
	if( ps == 1.0f ) {
		requestInput( 1u, target );
		return;
	}
	requestInput( 0u, target );
	requestInput( 1u );
	
	if( quality <= PPQ_FAST )
		scTrans.setParam( 0u, ps );
	else
		scTrans.setParam( 0u, ps * 1.2f - 0.1f );
	
	driver->setRenderTarget( target, false, false );
	driver->setMaterial( quality <= PPQ_FAST ? matFast : mat );
	driver->drawMeshBuffer( Mesh );
}
