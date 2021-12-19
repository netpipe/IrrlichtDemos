#include <irrlicht.h>
#include "IPostProc.h"

IPostProc::IPostProc( irr::scene::ISceneManager* sm, DIMENSION2D sOut ) {
	smgr = sm;
	init( sOut );
}
IPostProc::IPostProc( IPostProc* follow1, DIMENSION2D sOut ) {
	smgr = follow1->getSceneManager( );
	init( sOut );
}
IPostProc::~IPostProc( ) {
	irr::video::IVideoDriver* driver = smgr->getVideoDriver( );
	driver->removeTexture( output );
	driver->removeHardwareBuffer( Mesh );
	delete Mesh;
	IPostProc* changeTo = NULL;
	for( irr::u8 i = 0u; changeTo == NULL && i != 4u; ++ i )
		changeTo = follows[ i ];
	for( irr::u8 i = 0u; i != 4u; ++ i )
		if( follows[ i ] != NULL )
			follows[ i ]->removeOut( this, i );
	removeOut( NULL, 0u ); // Clean outs array
	while( outs.size( ) != 0u )
		outs[ 0u ].obj->setInput( outs[ 0u ].num, changeTo );
}
void IPostProc::init( DIMENSION2D sOut ) {
	irr::video::IVideoDriver* driver = smgr->getVideoDriver( );
	if( sOut.Width == 0 || sOut.Height == 0 )
		output = driver->addRenderTargetTexture( DIMENSION2D( 1, 1 ) );
	else
		output = driver->addRenderTargetTexture( sOut );
	for( irr::u8 i = 0u; i != 4u; ++ i ) {
		follows[ i ] = NULL;
		Tfollows[ i ] = NULL;
	}
	outs.reallocate( 4u );
	lastRender = 0u;
	pre = 0u;
	enabled = true;
	busy = false;
	timer = NULL;
	quality = PPQ_DEFAULT;
	requireSelfRender = false;
	firstRender = true;
	fixed = false;
	scTextures = CTexturesShaderCallback( );
	
	supported = driver->getDriverType( ) == irr::video::EDT_OPENGL || driver->getDriverType( ) == irr::video::EDT_DIRECT3D9;
	
	Mesh = new irr::scene::CMeshBuffer<irr::video::S3DVertex>( );
	Mesh->Vertices.reallocate( 4 );
	Mesh->Indices.reallocate( 6 );
	
	Mesh->Vertices.push_back( irr::video::S3DVertex( -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, irr::video::SColor( 255u, 255u, 255u, 255u ), 0.0f, 0.0f ) );
	Mesh->Vertices.push_back( irr::video::S3DVertex(  1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, irr::video::SColor( 255u, 255u, 255u, 255u ), 1.0f, 0.0f ) );
	Mesh->Vertices.push_back( irr::video::S3DVertex( -1.0f,  1.0f, 0.0f, 0.0f, 0.0f, 0.0f, irr::video::SColor( 255u, 255u, 255u, 255u ), 0.0f, 1.0f ) );
	Mesh->Vertices.push_back( irr::video::S3DVertex(  1.0f,  1.0f, 0.0f, 0.0f, 0.0f, 0.0f, irr::video::SColor( 255u, 255u, 255u, 255u ), 1.0f, 1.0f ) );
	
	Mesh->Indices.push_back( 0u );
	Mesh->Indices.push_back( 2u );
	Mesh->Indices.push_back( 1u );
	Mesh->Indices.push_back( 3u );
	Mesh->Indices.push_back( 1u );
	Mesh->Indices.push_back( 2u );
	
	Mesh->setHardwareMappingHint( irr::scene::EHM_STATIC, irr::scene::EBT_VERTEX_AND_INDEX );
	
	if( supported ) {
		irr::video::IGPUProgrammingServices* gpu = driver->getGPUProgrammingServices( );
		irr::s32 matid;
		switch( driver->getDriverType( ) ) {
			case irr::video::EDT_OPENGL:
				matid = gpu->addHighLevelShaderMaterial( GL_V_FULLSCREEN, "main", irr::video::EVST_VS_1_1, NULL, NULL, irr::video::EPST_PS_1_1, &scTextures, irr::video::EMT_SOLID, 1 );
				break;
			case irr::video::EDT_DIRECT3D9:
				matid = gpu->addHighLevelShaderMaterial( DX_V_FULLSCREEN, "main", irr::video::EVST_VS_1_1, NULL, NULL, irr::video::EPST_PS_1_1, NULL, irr::video::EMT_SOLID, 0 );
				break;
		}
		
		Mesh->Material.MaterialType = (irr::video::E_MATERIAL_TYPE) matid;
		Mesh->Material.Wireframe = false;
		Mesh->Material.Lighting = false;
		Mesh->Material.FogEnable = false;
		Mesh->Material.ZBuffer = 0;
		Mesh->Material.setTexture( 0u, output );
		Mesh->Material.TextureLayer[ 0u ].TextureWrapU = irr::video::ETC_CLAMP_TO_EDGE;
		Mesh->Material.TextureLayer[ 0u ].TextureWrapV = irr::video::ETC_CLAMP_TO_EDGE;
	}
}
void IPostProc::setTimer( irr::ITimer* t ) {
	if( timer != NULL || t == NULL )
		return;
	timer = t;
	for( irr::u8 i = 0u; i != 4u; ++ i )
		if( follows[ i ] != NULL )
			follows[ i ]->setTimer( t );
	for( irr::u8 i = 0u; i != outs.size( ); ++ i )
		if( outs[ i ].obj != NULL )
			outs[ i ].obj->setTimer( t );
}
void IPostProc::requestInput( irr::u8 i ) {
	if( i >= 4u )
		return;
	if( follows[ i ] != NULL )
		follows[ i ]->renderAtTime( lastRender, pre );
}
void IPostProc::requestInput( irr::u8 i, irr::video::ITexture* target ) {
	if( i >= 4u )
		return;
	if( follows[ i ] != NULL )
		follows[ i ]->render( target, lastRender, pre );
	else if( Tfollows[ i ] != NULL ) {
		irr::video::IVideoDriver* driver = smgr->getVideoDriver( );
		driver->setRenderTarget( target, false, false );
		driver->setMaterial( Tmats[ i ] );
		driver->drawMeshBuffer( Mesh );
	}
}
void IPostProc::copyEffect( irr::video::ITexture* target ) {
	irr::video::IVideoDriver* driver = smgr->getVideoDriver( );
	irr::core::rect<irr::s32> curSz = driver->getViewPort( );
	if( supported && target != NULL )
		driver->setViewPort( irr::core::rect<irr::s32>( irr::core::position2d<irr::s32>( 0, 0 ), (irr::core::dimension2d<irr::s32>) driver->getScreenSize( ) ) );
	driver->setRenderTarget( target, false, false );
	driver->setMaterial( Mesh->Material );
	driver->drawMeshBuffer( Mesh );
	driver->setViewPort( curSz );
}
void IPostProc::render( irr::video::ITexture* target, irr::u32 rid, bool p ) {
	if( fixed && !firstRender ) {
		if( target != output )
			if( supported )
				copyEffect( target );
		return;
	}
	if( rid == 0u ) {
		rid = lastRender + 1u;
		if( rid == 0u )
			rid = 1u;
	}
	if( pre && !p ) {
		pre = false;
		if( target != output ) {
			if( supported )
				copyEffect( target );
			else
				render( target );
		}
		lastRender = rid;
		return;
	}
	if( lastRender == rid ) {
		if( target != output ) {
			if( supported && enabled )
				copyEffect( target );
			else
				render( target );
		}
		return;
	}
	if( p )
		pre = true;
	lastRender = rid;
	irr::video::IVideoDriver* driver = smgr->getVideoDriver( );
	if( (outs.size( ) > 1u || p || requireSelfRender || fixed) && supported && enabled ) {
		irr::core::rect<irr::s32> curSz = driver->getViewPort( );
		if( supported )
			driver->setViewPort( irr::core::rect<irr::s32>( irr::core::position2d<irr::s32>( 0, 0 ), (irr::core::dimension2d<irr::s32>) driver->getScreenSize( ) ) );
		renderEffect( output );
		firstRender = false;
		driver->setViewPort( curSz );
		if( target != output )
			copyEffect( target );
	} else {
		renderEffect( target );
		firstRender = false;
		if( target != output && outs.size( ) > 1u )
			lastRender = 0u;
	}
}
void IPostProc::preRender( ) {
	pre = true;
	irr::video::IVideoDriver* driver = smgr->getVideoDriver( );
	irr::core::rect<irr::s32> curSz = driver->getViewPort( );
	if( supported )
		driver->setViewPort( irr::core::rect<irr::s32>( irr::core::position2d<irr::s32>( 0, 0 ), (irr::core::dimension2d<irr::s32>) driver->getScreenSize( ) ) );
	renderEffect( output );
	firstRender = false;
	driver->setViewPort( curSz );
}
void IPostProc::setInput( irr::u8 n, IPostProc* o ) {
	if( n >= 4u )
		return;
	if( follows[ n ] == o )
		return;
	if( follows[ n ] != NULL )
		follows[ n ]->removeOut( this, n );
	if( o == this )
		requireSelfRender = true;
	follows[ n ] = o;
	Tfollows[ n ] = NULL;
	if( o != NULL ) {
		o->addOut( this, n );
		setTimer( o->getTimer( ) );
	}
	configureInput( n );
}
void IPostProc::setInput( irr::u8 n, irr::video::ITexture* o ) {
	if( n >= 4u )
		return;
	if( Tfollows[ n ] == o )
		return;
	if( follows[ n ] != NULL )
		follows[ n ]->removeOut( this, n );
	follows[ n ] = NULL;
	if( o == NULL ) {
		Tfollows[ n ] = NULL;
		return;
	}
	if( o == output )
		requireSelfRender = true;
	Tfollows[ n ] = o;
	Tmats[ n ].MaterialType = Mesh->Material.MaterialType;
	Tmats[ n ].Wireframe = false;
	Tmats[ n ].Lighting = false;
	Tmats[ n ].FogEnable = false;
	Tmats[ n ].ZBuffer = 0;
	Tmats[ n ].TextureLayer[ 0u ].TextureWrapU = irr::video::ETC_CLAMP_TO_EDGE;
	Tmats[ n ].TextureLayer[ 0u ].TextureWrapV = irr::video::ETC_CLAMP_TO_EDGE;
	Tmats[ n ].setTexture( 0u, o );
	configureInput( n );
}
bool IPostProc::hasInput( irr::u8 n ) {
	if( n >= 4u )
		return false;
	if( follows[ n ] != NULL )
		return true;
	return Tfollows[ n ] != NULL;
}
IPostProc* IPostProc::getInput( irr::u8 n ) {
	if( n >= 4u )
		return NULL;
	return follows[ n ];
}
irr::video::ITexture* IPostProc::getInputTexture( irr::u8 n ) {
	if( n >= 4u )
		return NULL;
	if( follows[ n ] != NULL )
		return follows[ n ]->getOutputTexture( );
	return Tfollows[ n ];
}
IPostProc* IPostProc::getRoot( irr::scene::ISceneManager* sm ) {
	if( busy )
		return NULL;
	IPostProc* t = NULL;
	busy = true;
	for( irr::u8 i = 0u; i != 4u; ++ i )
		if( follows[ i ] != NULL ) {
			t = follows[ i ]->getRoot( sm );
			if( t != NULL ) {
				busy = false;
				return t;
			}
		}
	busy = false;
	if( smgr == sm || sm == NULL )
		return this;
	return NULL;
}
void IPostProc::setQuality( POSTPROC_QUALITY q ) {
	if( quality == q )
		return;
	quality = q;
	Mesh->Material.TextureLayer[ 0u ].BilinearFilter = quality > PPQ_CRUDE;
}
void IPostProc::setOverallQuality( POSTPROC_QUALITY q ) {
	if( busy )
		return;
	setQuality( q );
	busy = true;
	for( irr::u8 i = 0u; i != 4u; ++ i )
		if( follows[ i ] != NULL )
			follows[ i ]->setOverallQuality( q );
	for( irr::u8 i = 0u; i != outs.size( ); ++ i )
		if( outs[ i ].obj != NULL )
			outs[ i ].obj->setOverallQuality( q );
	busy = false;
}
irr::u8 IPostProc::countInputs( ) {
	irr::u8 t = 0u;
	for( irr::u8 i = 0u; i != 4u; ++ i ) {
		if( follows[ i ] != NULL || Tfollows[ i ] != NULL )
			++ t;
	}
	return t;
}
void IPostProc::addOut( IPostProc* o, irr::u8 n ) {
	for( irr::u8 i = 0u; i != outs.size( ); ++ i )
		if( outs[ i ].obj == o && outs[ i ].num == n )
			return;
	outs.push_back( PSConnector( o, n ) );
	setTimer( o->getTimer( ) );
}
void IPostProc::removeOut( IPostProc* o, irr::u8 n ) {
	irr::u8 del = 0u;
	for( irr::u8 i = 0u; i != outs.size( ); ++ i ) {
		if( (outs[ i ].obj == o && outs[ i ].num == n) || outs[ i ].obj == NULL )
			++ del;
		else if( del != 0u )
			outs[ i - del ] = outs[ i ];
	}
	outs.set_used( outs.size( ) - del );
}
