#include <irrlicht.h>
#include "CSplitPostProc.h"

CSplitPostProc::CSplitPostProc( IPostProc* follow1, DIMENSION2D sOut, irr::core::rect<irr::f32> zone1 ) : IPostProc( follow1, sOut ) {
	init( follow1, NULL, NULL, NULL, zone1, irr::core::rect<irr::f32>( 0.0f, 0.0f, 0.0f, 0.0f ), irr::core::rect<irr::f32>( 0.0f, 0.0f, 0.0f, 0.0f ), irr::core::rect<irr::f32>( 0.0f, 0.0f, 0.0f, 0.0f ) );
}
CSplitPostProc::CSplitPostProc( IPostProc* follow1, IPostProc* follow2, DIMENSION2D sOut, irr::core::rect<irr::f32> zone1, irr::core::rect<irr::f32> zone2 ) : IPostProc( follow1, sOut ) {
	init( follow1, follow2, NULL, NULL, zone1, zone2, irr::core::rect<irr::f32>( 0.0f, 0.0f, 0.0f, 0.0f ), irr::core::rect<irr::f32>( 0.0f, 0.0f, 0.0f, 0.0f ) );
}
CSplitPostProc::CSplitPostProc( IPostProc* follow1, IPostProc* follow2, IPostProc* follow3, DIMENSION2D sOut, irr::core::rect<irr::f32> zone1, irr::core::rect<irr::f32> zone2, irr::core::rect<irr::f32> zone3 ) : IPostProc( follow1, sOut ) {
	init( follow1, follow2, follow3, NULL, zone1, zone2, zone3, irr::core::rect<irr::f32>( 0.0f, 0.0f, 0.0f, 0.0f ) );
}
CSplitPostProc::CSplitPostProc( IPostProc* follow1, IPostProc* follow2, IPostProc* follow3, IPostProc* follow4, DIMENSION2D sOut, irr::core::rect<irr::f32> zone1, irr::core::rect<irr::f32> zone2, irr::core::rect<irr::f32> zone3, irr::core::rect<irr::f32> zone4 ) : IPostProc( follow1, sOut ) {
	init( follow1, follow2, follow3, follow4, zone1, zone2, zone3, zone4 );
}
CSplitPostProc::~CSplitPostProc( ) {
	for( irr::u8 i = 0u; i != 4u; ++ i )
		delete m[ i ];
}
void CSplitPostProc::init( IPostProc* follow1, IPostProc* follow2, IPostProc* follow3, IPostProc* follow4, irr::core::rect<irr::f32> zone1, irr::core::rect<irr::f32> zone2, irr::core::rect<irr::f32> zone3, irr::core::rect<irr::f32> zone4 ) {
	z[ 0u ] = zone1;
	z[ 1u ] = zone2;
	z[ 2u ] = zone3;
	z[ 3u ] = zone4;
	vspace = irr::video::SColor( 255u, 0u, 0u, 0u );
	bvspace = true;
	
	for( irr::u8 i = 0u; i != 4u; ++ i ) {
		m[ i ] = new irr::scene::CMeshBuffer<irr::video::S3DVertex>( );
		m[ i ]->Vertices.reallocate( 4u );
		m[ i ]->Indices.reallocate( 6u );
		
		m[ i ]->Vertices.push_back( irr::video::S3DVertex( -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, irr::video::SColor( 255u, 255u, 255u, 255u ), 0.0f, 0.0f ) );
		m[ i ]->Vertices.push_back( irr::video::S3DVertex(  1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, irr::video::SColor( 255u, 255u, 255u, 255u ), 1.0f, 0.0f ) );
		m[ i ]->Vertices.push_back( irr::video::S3DVertex( -1.0f,  1.0f, 0.0f, 0.0f, 0.0f, 0.0f, irr::video::SColor( 255u, 255u, 255u, 255u ), 0.0f, 1.0f ) );
		m[ i ]->Vertices.push_back( irr::video::S3DVertex(  1.0f,  1.0f, 0.0f, 0.0f, 0.0f, 0.0f, irr::video::SColor( 255u, 255u, 255u, 255u ), 1.0f, 1.0f ) );
		
		m[ i ]->Indices.push_back( 0u );
		m[ i ]->Indices.push_back( 2u );
		m[ i ]->Indices.push_back( 1u );
		m[ i ]->Indices.push_back( 3u );
		m[ i ]->Indices.push_back( 1u );
		m[ i ]->Indices.push_back( 2u );
		m[ i ]->Material = Mesh->Material;
		
		m[ i ]->setHardwareMappingHint( irr::scene::EHM_STATIC, irr::scene::EBT_VERTEX_AND_INDEX );
	}
	
	setInput( 0u, follow1 );
	setInput( 1u, follow2 );
	setInput( 2u, follow3 );
	setInput( 3u, follow4 );
	setZone( 0u, zone1 );
	setZone( 1u, zone2 );
	setZone( 2u, zone3 );
	setZone( 3u, zone4 );
}
void CSplitPostProc::configureInput( irr::u8 n ) {
	if( !supported )
		return;
	m[n]->Material.setTexture( 0u, getInputTexture( n ) );
}
void CSplitPostProc::setZone( irr::u8 n, irr::core::rect<irr::f32> to ) {
	if( n >= 4u )
		return;
	z[n] = to;
	m[n]->Vertices[ 0u ].Pos = irr::core::vector3d<irr::f32>( to.UpperLeftCorner.X, -to.LowerRightCorner.Y, 0.0f );
	m[n]->Vertices[ 1u ].Pos = irr::core::vector3d<irr::f32>( to.LowerRightCorner.X, -to.LowerRightCorner.Y, 0.0f );
	m[n]->Vertices[ 2u ].Pos = irr::core::vector3d<irr::f32>( to.UpperLeftCorner.X, -to.UpperLeftCorner.Y, 0.0f );
	m[n]->Vertices[ 3u ].Pos = irr::core::vector3d<irr::f32>( to.LowerRightCorner.X, -to.UpperLeftCorner.Y, 0.0f );
}
void CSplitPostProc::renderEffect( irr::video::ITexture* target ) {
	irr::video::IVideoDriver* driver = smgr->getVideoDriver( );
	
	if( !enabled ) {
		if( hasInput( 0u ) )
			requestInput( 0u, target );
		else if( hasInput( 1u ) )
			requestInput( 1u, target );
		else if( hasInput( 2u ) )
			requestInput( 2u, target );
		else
			requestInput( 3u, target );
		return;
	}
	if( !supported ) {
		if( bvspace )
			driver->draw2DRectangle( vspace, irr::core::rect<irr::s32>( irr::core::position2d<irr::s32>( 0, 0 ), (irr::core::dimension2d<irr::s32>) driver->getCurrentRenderTargetSize( ) ) );
		irr::core::rect<irr::s32> vv = driver->getViewPort( );
		for( irr::u8 i = 0u; i != 4u; ++ i )
			if( hasInput( i ) && z[ i ].getWidth( ) != 0.0f && z[ i ].getHeight( ) != 0.0f ) {
				driver->setViewPort( irr::core::rect<irr::s32>( vv.UpperLeftCorner.X + (irr::s32) ((z[ i ].UpperLeftCorner.X * 0.5f + 0.5f) * vv.getWidth( )),
															    vv.UpperLeftCorner.Y + (irr::s32) ((z[ i ].UpperLeftCorner.Y * 0.5f + 0.5f) * vv.getHeight( )),
															    vv.UpperLeftCorner.X + (irr::s32) ((z[ i ].LowerRightCorner.X * 0.5f + 0.5f) * vv.getWidth( )),
															    vv.UpperLeftCorner.Y + (irr::s32) ((z[ i ].LowerRightCorner.Y * 0.5f + 0.5f) * vv.getHeight( ))
															   ) );
				requestInput( i, target );
			}
		driver->setViewPort( vv );
		return;
	}
	
	for( irr::u8 i = 0u; i != 4u; ++ i )
		if( hasInput( i ) && z[ i ].getWidth( ) != 0.0f && z[ i ].getHeight( ) != 0.0f )
			requestInput( i );
	
	driver->setRenderTarget( target, bvspace, false, vspace );
	
	for( irr::u8 i = 0u; i != 4u; ++ i )
		if( hasInput( i ) && z[ i ].getWidth( ) != 0.0f && z[ i ].getHeight( ) != 0.0f ) {
			driver->setMaterial( m[ i ]->Material );
			driver->drawMeshBuffer( m[ i ] );
		}
}
