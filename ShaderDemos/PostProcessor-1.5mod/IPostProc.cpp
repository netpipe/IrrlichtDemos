#include "IPostProc.h"

IPostProc::IPostProc( ISceneManager* sm, dimension2di sOut ) {
	smgr = sm;
	init( sOut );
}
IPostProc::IPostProc( IPostProc* follow1, dimension2di sOut ) {
	smgr = follow1->getSceneManager( );
	init( sOut );
}
IPostProc::~IPostProc( ) {
	IVideoDriver *driver = smgr->getVideoDriver( );
	driver->removeTexture( output );
	driver->removeHardwareBuffer( Mesh );
	IPostProc* changeTo = NULL;
	for( u8 i = 0u; changeTo == NULL && i != nFollows; ++ i )
		changeTo = follows[ i ];
	removeOut( NULL, 0 ); // Clean outs array
	for( u8 i = 0; i != nFollows; ++ i )
		if( follows[ i ] != NULL )
			follows[ i ]->removeOut( this, i );
	while( outs.size( ) != 0u )
		outs[ 0 ]->setInput( outIs[ 0 ], changeTo );
}
void IPostProc::init( dimension2di sOut ) {

	IVideoDriver *driver = smgr->getVideoDriver( );
	if( sOut.Width == 0 || sOut.Height == 0 )
		output = NULL;
	else
		output = driver->addRenderTargetTexture( sOut );
	for( u8 i = 0u; i != 4; ++ i )
		follows[ i ] = NULL;
	outs.reallocate( 4 );
	outIs.reallocate( 4 );
	nFollows = 0u;
	lastRender = 0u;
	enabled = true;
	busy = false;

	supported = driver->getDriverType( ) != EDT_SOFTWARE && driver->getDriverType( ) != EDT_BURNINGSVIDEO && driver->getDriverType( ) != EDT_NULL;

	Mesh = new CMeshBuffer<S3DVertex>( );
	Mesh->Vertices.reallocate( 4 );
	Mesh->Indices.reallocate( 6 );

	Mesh->Vertices.push_back( S3DVertex( -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, SColor( 255, 255, 255, 255 ), 0.0f, 0.0f ) );
	Mesh->Vertices.push_back( S3DVertex(  1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, SColor( 255, 255, 255, 255 ), 1.0f, 0.0f ) );
	Mesh->Vertices.push_back( S3DVertex( -1.0f,  1.0f, 0.0f, 0.0f, 0.0f, 0.0f, SColor( 255, 255, 255, 255 ), 0.0f, 1.0f ) );
	Mesh->Vertices.push_back( S3DVertex(  1.0f,  1.0f, 0.0f, 0.0f, 0.0f, 0.0f, SColor( 255, 255, 255, 255 ), 1.0f, 1.0f ) );

	Mesh->Indices.push_back( 0u );
	Mesh->Indices.push_back( 2u );
	Mesh->Indices.push_back( 1u );
	Mesh->Indices.push_back( 3u );
	Mesh->Indices.push_back( 1u );
	Mesh->Indices.push_back( 2u );

	Mesh->setHardwareMappingHint( EHM_STATIC, EBT_VERTEX_AND_INDEX );

	if( supported ) {
		IGPUProgrammingServices* gpu = driver->getGPUProgrammingServices( );
		s32 matid;
		switch( driver->getDriverType( ) ) {
			case EDT_OPENGL:
				matid = gpu->addHighLevelShaderMaterial( GL_V_FULLSCREEN, "main", EVST_VS_1_1, GL_DIRECT, "main", EPST_PS_1_1, new CTexturesShaderCallback( ), EMT_SOLID, 1 );
				break;
			case EDT_DIRECT3D8:
			case EDT_DIRECT3D9:
			default:
				matid = gpu->addHighLevelShaderMaterial( DX_V_FULLSCREEN, "main", EVST_VS_1_1, DX_DIRECT, "main", EPST_PS_1_1, NULL, EMT_SOLID, 0 );
		}

		Mesh->Material.MaterialType = (E_MATERIAL_TYPE) matid;
		Mesh->Material.Wireframe = false;
		Mesh->Material.Lighting = false;
		Mesh->Material.FogEnable = false;
		Mesh->Material.ZBuffer = 0;
		Mesh->Material.TextureLayer[0].TextureWrap = ETC_CLAMP;
	}
}
void IPostProc::requestInput( u8 i ) {
	if( follows[ i ] == NULL )
		return;
	follows[ i ]->renderAtTime( lastRender );
}
void IPostProc::requestInput( u8 i, ITexture* target ) {
	if( follows[ i ] == NULL )
		return;
	follows[ i ]->render( target, lastRender );
}
void IPostProc::copyEffect( ITexture* target ) {
	IVideoDriver *driver = smgr->getVideoDriver( );
	driver->setRenderTarget( target, false, false );
	driver->setMaterial( Mesh->Material );
	driver->drawMeshBuffer( Mesh );
}
void IPostProc::render( ITexture* target, u32 rid ) {
	if( rid == 0u ) {
		rid = lastRender + 1u;
		if( rid == 0u )
			rid = 1u;
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
	lastRender = rid;
	if( outs.size( ) > 1u && supported && enabled ) {
		renderEffect( output );
		if( target != output )
			copyEffect( target );
	} else {
		renderEffect( target );
		if( target != output && outs.size( ) > 1u )
			lastRender = 0u;
	}
}
void IPostProc::setInput( u8 n, IPostProc* o ) {
	if( follows[ n ] != NULL )
		follows[ n ]->removeOut( this, n );
	follows[ n ] = o;
	if( o != NULL ) {
		if( n >= nFollows )
			nFollows = n + 1u;
		o->addOut( this, n );
	}
	u8 m = 0u;
	for( u8 i = 0u; i != nFollows; i ++ )
		if( follows[ i ] != NULL )
			m = i + 1u;
	nFollows = m;
	configureInput( n );
}
IPostProc* IPostProc::getInput( u8 n ) {
	if( n >= nFollows )
		return NULL;
	return follows[ n ];
}
IPostProc* IPostProc::getRoot( ISceneManager* sm ) {
	if( busy )
		return NULL;
	IPostProc* t = NULL;
	busy = true;
	for( u8 i = 0u; i != nFollows; ++ i )
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
void IPostProc::addOut( IPostProc* o, u8 n ) {
	for( u8 i = 0u; i != outs.size( ); ++ i )
		if( outs[ i ] == o && outIs[ i ] == n )
			return;
	outs.push_back( o );
	outIs.push_back( n );
	if( outs.size( ) > 1u )
		Mesh->Material.setTexture( 0, output );
}
void IPostProc::removeOut( IPostProc* o, u8 n ) {
	u8 del = 0u;
	for( u8 i = 0u; i != outs.size( ); ++ i ) {
		if( (outs[ i ] == o && outIs[ i ] == n) || outs[ i ] == NULL )
			del ++;
		else if( del != 0u ) {
			outs[ i - del ] = outs[ i ];
			outIs[ i - del ] = outIs[ i ];
		}
	}
	outs.set_used( outs.size( ) - del );
	if( outs.size( ) <= 1u )
		Mesh->Material.setTexture( 0, NULL );
}
