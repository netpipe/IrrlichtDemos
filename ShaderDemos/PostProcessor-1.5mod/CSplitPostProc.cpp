#include "CSplitPostProc.h"

CSplitPostProc::CSplitPostProc( IPostProc* follow1, dimension2di sOut, rect<float> zone1 ) : IPostProc( follow1, sOut ) {
	init( follow1, NULL, NULL, NULL, zone1, rect<float>( 0.0f, 0.0f, 0.0f, 0.0f ), rect<float>( 0.0f, 0.0f, 0.0f, 0.0f ), rect<float>( 0.0f, 0.0f, 0.0f, 0.0f ) );
}
CSplitPostProc::CSplitPostProc( IPostProc* follow1, IPostProc* follow2, dimension2di sOut, rect<float> zone1, rect<float> zone2 ) : IPostProc( follow1, sOut ) {
	init( follow1, follow2, NULL, NULL, zone1, zone2, rect<float>( 0.0f, 0.0f, 0.0f, 0.0f ), rect<float>( 0.0f, 0.0f, 0.0f, 0.0f ) );
}
CSplitPostProc::CSplitPostProc( IPostProc* follow1, IPostProc* follow2, IPostProc* follow3, dimension2di sOut, rect<float> zone1, rect<float> zone2, rect<float> zone3 ) : IPostProc( follow1, sOut ) {
	init( follow1, follow2, follow3, NULL, zone1, zone2, zone3, rect<float>( 0.0f, 0.0f, 0.0f, 0.0f ) );
}
CSplitPostProc::CSplitPostProc( IPostProc* follow1, IPostProc* follow2, IPostProc* follow3, IPostProc* follow4, dimension2di sOut, rect<float> zone1, rect<float> zone2, rect<float> zone3, rect<float> zone4 ) : IPostProc( follow1, sOut ) {
	init( follow1, follow2, follow3, follow4, zone1, zone2, zone3, zone4 );
}
void CSplitPostProc::init( IPostProc* follow1, IPostProc* follow2, IPostProc* follow3, IPostProc* follow4, rect<float> zone1, rect<float> zone2, rect<float> zone3, rect<float> zone4 ) {
	z[0] = zone1;
	z[1] = zone2;
	z[2] = zone3;
	z[3] = zone4;
	vspace = SColor( 255, 0, 0, 0 );
	bvspace = true;

	for( u8 i = 0u; i != 4; ++ i ) {
		m[i] = new CMeshBuffer<S3DVertex>( );
		m[i]->Vertices.reallocate( 4 );
		m[i]->Indices.reallocate( 6 );

		m[i]->Vertices.push_back( S3DVertex( -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, SColor( 255, 255, 255, 255 ), 0.0f, 0.0f ) );
		m[i]->Vertices.push_back( S3DVertex(  1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, SColor( 255, 255, 255, 255 ), 1.0f, 0.0f ) );
		m[i]->Vertices.push_back( S3DVertex( -1.0f,  1.0f, 0.0f, 0.0f, 0.0f, 0.0f, SColor( 255, 255, 255, 255 ), 0.0f, 1.0f ) );
		m[i]->Vertices.push_back( S3DVertex(  1.0f,  1.0f, 0.0f, 0.0f, 0.0f, 0.0f, SColor( 255, 255, 255, 255 ), 1.0f, 1.0f ) );

		m[i]->Indices.push_back( 0u );
		m[i]->Indices.push_back( 2u );
		m[i]->Indices.push_back( 1u );
		m[i]->Indices.push_back( 3u );
		m[i]->Indices.push_back( 1u );
		m[i]->Indices.push_back( 2u );
		m[i]->Material = Mesh->Material;

		m[i]->setHardwareMappingHint( EHM_STATIC, EBT_VERTEX_AND_INDEX );
	}

	setInput( 0, follow1 );
	setInput( 1, follow2 );
	setInput( 2, follow3 );
	setInput( 3, follow4 );
	setZone( 0, zone1 );
	setZone( 1, zone2 );
	setZone( 2, zone3 );
	setZone( 3, zone4 );
}
void CSplitPostProc::configureInput( u8 n ) {
	if( !supported )
		return;
	if( follows[n] != NULL )
		m[n]->Material.setTexture( 0, follows[n]->getOutputTexture( ) );
}
void CSplitPostProc::setZone( u8 n, rect<float> to ) {
	if( n >= 4u )
		return;
	z[n] = to;
	m[n]->Vertices[0].Pos = vector3df( to.UpperLeftCorner.X, -to.LowerRightCorner.Y, 0.0f );
	m[n]->Vertices[1].Pos = vector3df( to.LowerRightCorner.X, -to.LowerRightCorner.Y, 0.0f );
	m[n]->Vertices[2].Pos = vector3df( to.UpperLeftCorner.X, -to.UpperLeftCorner.Y, 0.0f );
	m[n]->Vertices[3].Pos = vector3df( to.LowerRightCorner.X, -to.UpperLeftCorner.Y, 0.0f );
}
void CSplitPostProc::renderEffect( ITexture *target ) {
	IVideoDriver *driver = smgr->getVideoDriver( );

	if( !enabled ) {
		if( follows[0] != NULL )
			requestInput( 0, target );
		else if( follows[1] != NULL )
			requestInput( 1, target );
		else if( follows[2] != NULL )
			requestInput( 2, target );
		else
			requestInput( 3, target );
		return;
	}
	if( !supported ) {
		if( bvspace )
			driver->draw2DRectangle( vspace, rect<int>( position2di( 0, 0 ), driver->getCurrentRenderTargetSize( ) ) );
		rect<int> vv = driver->getViewPort( );
		for( u8 i = 0u; i != 4u; ++ i )
			if( follows[i] != NULL && z[i].getWidth( ) != 0.0f && z[i].getHeight( ) != 0.0f ) {
				driver->setViewPort( rect<int>(vv.UpperLeftCorner.X + (z[i].UpperLeftCorner.X * 0.5f + 0.5f) * vv.getWidth( ),
										   vv.UpperLeftCorner.Y + (z[i].UpperLeftCorner.Y * 0.5f + 0.5f) * vv.getHeight( ),
										   vv.UpperLeftCorner.X + (z[i].LowerRightCorner.X * 0.5f + 0.5f) * vv.getWidth( ),
										   vv.UpperLeftCorner.Y + (z[i].LowerRightCorner.Y * 0.5f + 0.5f) * vv.getHeight( )
										   ) );
				requestInput( i, target );
			}
		driver->setViewPort( vv );
		return;
	}

	for( u8 i = 0u; i != 4u; ++ i )
		if( follows[i] != NULL && z[i].getWidth( ) != 0.0f && z[i].getHeight( ) != 0.0f )
			requestInput( i );

	driver->setRenderTarget( target, bvspace, false, vspace );

	for( u8 i = 0u; i != 4u; ++ i )
		if( follows[i] != NULL && z[i].getWidth( ) != 0.0f && z[i].getHeight( ) != 0.0f ) {
			driver->setMaterial( m[i]->Material );
			driver->drawMeshBuffer( m[i] );
		}
}
