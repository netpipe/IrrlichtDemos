#include <irrlicht.h>
#include "CWaterPostProc.h"

CWaterPostProc::CWaterPostProc( IPostProc* follow, DIMENSION2D sOut, irr::f32 depth ) : IPostProc( follow, sOut ) {
	irr::video::IVideoDriver* driver = smgr->getVideoDriver( );
	irr::video::IGPUProgrammingServices* gpu = driver->getGPUProgrammingServices( );
	
	ylevel = depth;
	
	if( supported ) {
		// ToDo: this line is a little risky - the root might not be a renderer if this has been extended.
		CRendererPostProc* r1 = (CRendererPostProc*) follow->getRoot( smgr );
		if( r1 == NULL )
			reflection = new CRendererPostProc( smgr, sOut, true, true );
		else
			reflection = new CRendererPostProc( smgr, sOut, true, true, r1->getColor( ) );
		irr::scene::ICameraSceneNode* fcam = smgr->getActiveCamera( );
		refCam = smgr->addCameraSceneNode( smgr->getRootSceneNode( ) );
		refCam->setInputReceiverEnabled( false );
		if( fcam != NULL )
			smgr->setActiveCamera( fcam );
		
		scWater = CWaterShaderCallback( );
		
		irr::s32 matid, matid2;
		switch( driver->getDriverType( ) ) {
			case irr::video::EDT_OPENGL:
				matid = gpu->addHighLevelShaderMaterial( GL_V_FULLSCREEN, "main", irr::video::EVST_VS_1_1, GL_WATER, "main", irr::video::EPST_PS_1_1, &scWater, irr::video::EMT_SOLID, 2 );
				matid2 = gpu->addHighLevelShaderMaterial( GL_V_FULLSCREEN, "main", irr::video::EVST_VS_1_1, GL_UNDERWATER, "main", irr::video::EPST_PS_1_1, &scWater, irr::video::EMT_SOLID, 1 );
				break;
			case irr::video::EDT_DIRECT3D9:
				matid = gpu->addHighLevelShaderMaterial( DX_V_FULLSCREEN, "main", irr::video::EVST_VS_1_1, DX_WATER, "main", irr::video::EPST_PS_1_1, &scWater, irr::video::EMT_SOLID, 0 );
				matid2 = gpu->addHighLevelShaderMaterial( DX_V_FULLSCREEN, "main", irr::video::EVST_VS_1_1, DX_UNDERWATER, "main", irr::video::EPST_PS_1_1, &scWater, irr::video::EMT_SOLID, 0 );
				break;
		}
		
		mat = irr::video::SMaterial( );
		mat.MaterialType = (irr::video::E_MATERIAL_TYPE) matid;
		mat.Wireframe = false;
		mat.Lighting = false;
		mat.FogEnable = false;
		mat.ZBuffer = 0;
		mat.setTexture( 1u, reflection->getOutputTexture( ) );
		mat.TextureLayer[ 0u ].TextureWrapU = irr::video::ETC_CLAMP_TO_EDGE;
		mat.TextureLayer[ 0u ].TextureWrapV = irr::video::ETC_CLAMP_TO_EDGE;
		mat.TextureLayer[ 1u ].TextureWrapU = irr::video::ETC_CLAMP_TO_EDGE;
		mat.TextureLayer[ 1u ].TextureWrapV = irr::video::ETC_CLAMP_TO_EDGE;
		mat.TextureLayer[ 0u ].BilinearFilter = false;
		mat.TextureLayer[ 1u ].BilinearFilter = false;
		
		matUnder = irr::video::SMaterial( );
		matUnder.MaterialType = (irr::video::E_MATERIAL_TYPE) matid2;
		matUnder.Wireframe = false;
		matUnder.Lighting = false;
		matUnder.FogEnable = false;
		matUnder.ZBuffer = 0;
		matUnder.TextureLayer[ 0u ].TextureWrapU = irr::video::ETC_CLAMP_TO_EDGE;
		matUnder.TextureLayer[ 0u ].TextureWrapV = irr::video::ETC_CLAMP_TO_EDGE;
	}
	setInput( 0u, follow );
}
CWaterPostProc::~CWaterPostProc( ) {
	if( supported ) {
		delete reflection;
		refCam->remove( );
	}
}
void CWaterPostProc::configureInput( irr::u8 n ) {
	if( !supported )
		return;
	if( n == 0u ) {
		mat.setTexture( 0u, getInputTexture( 0u ) );
		matUnder.setTexture( 0u, getInputTexture( 0u ) );
	}
}
void CWaterPostProc::renderEffect( irr::video::ITexture* target ) {
	irr::video::IVideoDriver* driver = smgr->getVideoDriver( );
	
	if( !hasInput( 0u ) )
		return;
	
	if( !supported || !enabled ) {
		requestInput( 0u, target );
		return;
	}
	
	irr::scene::ICameraSceneNode* fcam = smgr->getActiveCamera( );
	if( fcam == NULL ) {
		requestInput( 0u, target );
		return;
	}
	if( timer != NULL )
		scWater.time = timer->getTime( ) * 0.001f;
	scWater.waveHeight = 0.05f;
	scWater.waveSize = 2.0f;
	scWater.refraction = 4.0f / 3.0f;
	scWater.ylevel = ylevel;
	scWater.visibility = 20.0f;
	scWater.lightPen = 6.0f;
	scWater.smooth = 2.0f;
	scWater.sharp = 0.2f;
	scWater.colSurf = irr::video::SColorf( 0.0078f, 0.5176f, 0.7f, 1.0f );
	scWater.colDeep = irr::video::SColorf( 0.0039f, 0.00196f, 0.145f, 1.0f );
	fcam->updateAbsolutePosition( );
	scWater.pCamera = fcam->getAbsolutePosition( );
	if( fcam->getAbsolutePosition( ).Y > ylevel ) {
		refCam->setPosition( irr::core::vector3d<irr::f32>( fcam->getAbsolutePosition( ).X, ylevel * 2.0f - fcam->getAbsolutePosition( ).Y, fcam->getAbsolutePosition( ).Z ) );
		refCam->setTarget( irr::core::vector3d<irr::f32>( fcam->getTarget( ).X, ylevel * 2.0f - fcam->getTarget( ).Y, fcam->getTarget( ).Z ) );
		refCam->setUpVector( fcam->getUpVector( ) );
		// ToDo: can these 4 be replaced with setting projection matrix? this would be more flexible (allowing orthographic cameras, for example)
		refCam->setFOV( fcam->getFOV( ) );
		refCam->setNearValue( fcam->getNearValue( ) );
		refCam->setFarValue( fcam->getFarValue( ) );
		refCam->setAspectRatio( fcam->getAspectRatio( ) );
		
		smgr->setActiveCamera( refCam );
		driver->setClipPlane( 0u, irr::core::plane3d<irr::f32>( irr::core::vector3d<irr::f32>( 0.0f, ylevel, 0.0f ), irr::core::vector3d<irr::f32>( 0.0f, 1.0f, 0.0f ) ), true );
		reflection->preRender( );
		smgr->setActiveCamera( fcam );
		// simply disabling the clip plane doesn't work, so set it to something non-sensical:
		driver->setClipPlane( 0u, irr::core::plane3d<irr::f32>( irr::core::vector3d<irr::f32>( 0.0f, 0.0f, 0.0f ), irr::core::vector3d<irr::f32>( 0.0f, 0.0f, 0.0f ) ), true );
		requestInput( 0u );
		
		driver->setRenderTarget( target, false, false );
		driver->setMaterial( mat );
		irr::core::CMatrix4<irr::f32> v = fcam->getViewMatrix( );
		v.setInverseTranslation( irr::core::vector3d<irr::f32>( 0.0f, 0.0f, 0.0f ) ); // We handle translation manually. It seems translation is part of this matrix, but doesn't work in a way I can understand / use, so just blank it.
		driver->setTransform( irr::video::ETS_VIEW, v );
		driver->setTransform( irr::video::ETS_WORLD, irr::core::CMatrix4<irr::f32>( ) );
		driver->setTransform( irr::video::ETS_PROJECTION, fcam->getProjectionMatrix( ) );
		driver->drawMeshBuffer( Mesh );
	} else {
		requestInput( 0u );
		driver->setRenderTarget( target, false, false );
		driver->setMaterial( matUnder );
		irr::core::CMatrix4<irr::f32> v = fcam->getViewMatrix( );
		v.setInverseTranslation( irr::core::vector3d<irr::f32>( 0.0f, 0.0f, 0.0f ) ); // We handle translation manually. It seems translation is part of this matrix, but doesn't work in a way I can understand / use, so just blank it.
		driver->setTransform( irr::video::ETS_VIEW, v );
		driver->setTransform( irr::video::ETS_WORLD, irr::core::CMatrix4<irr::f32>( ) );
		driver->setTransform( irr::video::ETS_PROJECTION, fcam->getProjectionMatrix( ) );
		driver->drawMeshBuffer( Mesh );
	}
}
