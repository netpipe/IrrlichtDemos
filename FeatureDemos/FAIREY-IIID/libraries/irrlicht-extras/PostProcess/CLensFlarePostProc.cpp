#include <irrlicht.h>
#include "CLensFlarePostProc.h"

CLensFlarePostProc::CLensFlarePostProc( IPostProc* follow, DIMENSION2D sOut, irr::video::ITexture* m, irr::core::vector3d<irr::f32> p, irr::f32 r ) : IPostProc( follow, sOut ) {
	setPosition( p, r );
	init( follow, sOut, m );
}
CLensFlarePostProc::CLensFlarePostProc( IPostProc* follow, DIMENSION2D sOut, irr::video::ITexture* m, irr::scene::ISceneNode* p, irr::f32 r ) : IPostProc( follow, sOut ) {
	setPosition( p, r );
	init( follow, sOut, m );
}
CLensFlarePostProc::~CLensFlarePostProc( ) {
	if( supported )
		delete mf;
}
void CLensFlarePostProc::init( IPostProc* follow, DIMENSION2D sOut, irr::video::ITexture* m ) {
	irr::video::IVideoDriver* driver = smgr->getVideoDriver( );
	irr::video::IGPUProgrammingServices* gpu = driver->getGPUProgrammingServices( );
	
	mCol = irr::video::SColorf( 1.0f, 1.0f, 1.0f, 1.0f );
	
	flP.reallocate( 5u );
	
	flP.push_back( 0.0f );
	flP.push_back( 0.0f );
	flP.push_back( 0.55f );
	flP.push_back( 0.53f );
	flP.push_back( 0.7f );
//	flP.push_back( 1.4f );
	
	flR.reallocate( flP.size( ) );
	
	flR.push_back( 0.3f );
	flR.push_back( 0.4f );
	flR.push_back( 0.15f );
	flR.push_back( 0.08f );
	flR.push_back( 0.4f );
//	flR.push_back( 1.4f );
	
	flT.reallocate( flP.size( ) );
	
	flT.push_back( irr::core::rect<irr::f32>( 0.0f, 0.5f, 1.0f, 1.0f ) );
	flT.push_back( irr::core::rect<irr::f32>( 0.0f, 0.0f, 0.5f, 0.5f ) );
	flT.push_back( irr::core::rect<irr::f32>( 0.75f, 0.25f, 1.0f, 0.5f ) );
	flT.push_back( irr::core::rect<irr::f32>( 0.5f, 0.25f, 0.75f, 0.5f ) );
	flT.push_back( irr::core::rect<irr::f32>( 0.5f, 0.0f, 0.75f, 0.25f ) );
//	flT.push_back( irr::core::rect<irr::f32>( 0.75f, 0.0f, 1.0f, 0.25f ) );
	
	if( supported ) {
		mf = new irr::scene::CMeshBuffer<irr::video::S3DVertex>( );
		mf->Vertices.reallocate( 4u * flP.size( ) );
		mf->Indices.reallocate( 6u * flP.size( ) );
		
		for( irr::u8 i = 0u; i != flP.size( ); ++ i ) {
			mf->Vertices.push_back( irr::video::S3DVertex( -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, irr::video::SColor( 255u, 255u, 255u, 255u ), flT[ i ].UpperLeftCorner.X, flT[ i ].UpperLeftCorner.Y ) );
			mf->Vertices.push_back( irr::video::S3DVertex(  1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, irr::video::SColor( 255u, 255u, 255u, 255u ), flT[ i ].LowerRightCorner.X, flT[ i ].UpperLeftCorner.Y ) );
			mf->Vertices.push_back( irr::video::S3DVertex( -1.0f,  1.0f, 0.0f, 0.0f, 0.0f, 0.0f, irr::video::SColor( 255u, 255u, 255u, 255u ), flT[ i ].UpperLeftCorner.X, flT[ i ].LowerRightCorner.Y ) );
			mf->Vertices.push_back( irr::video::S3DVertex(  1.0f,  1.0f, 0.0f, 0.0f, 0.0f, 0.0f, irr::video::SColor( 255u, 255u, 255u, 255u ), flT[ i ].LowerRightCorner.X, flT[ i ].LowerRightCorner.Y ) );
			
			mf->Indices.push_back( 0u + i * 4u );
			mf->Indices.push_back( 2u + i * 4u );
			mf->Indices.push_back( 1u + i * 4u );
			mf->Indices.push_back( 3u + i * 4u );
			mf->Indices.push_back( 1u + i * 4u );
			mf->Indices.push_back( 2u + i * 4u );
		}
		
		mf->setHardwareMappingHint( irr::scene::EHM_STREAM, irr::scene::EBT_VERTEX );
		mf->setHardwareMappingHint( irr::scene::EHM_STATIC, irr::scene::EBT_INDEX );
		
		irr::s32 matid;
		switch( driver->getDriverType( ) ) {
			case irr::video::EDT_OPENGL:
				matid = gpu->addHighLevelShaderMaterial( GL_V_FULLSCREENWITHVERTCOL, "main", irr::video::EVST_VS_1_1, NULL, NULL, irr::video::EPST_PS_1_1, &scTextures, irr::video::EMT_TRANSPARENT_ADD_COLOR, 1 );
				break;
			case irr::video::EDT_DIRECT3D9:
				matid = gpu->addHighLevelShaderMaterial( DX_V_FULLSCREENWITHVERTCOL, "main", irr::video::EVST_VS_1_1, NULL, NULL, irr::video::EPST_PS_1_1, NULL, irr::video::EMT_TRANSPARENT_ADD_COLOR, 0 );
				break;
		}
		
		mf->Material.MaterialType = (irr::video::E_MATERIAL_TYPE) matid;
		mf->Material.Wireframe = false;
		mf->Material.Lighting = false;
		mf->Material.FogEnable = false;
		mf->Material.ZBuffer = 0;
		mf->Material.setTexture( 0u, m );
		mf->Material.TextureLayer[ 0u ].TextureWrapU = irr::video::ETC_CLAMP_TO_EDGE;
		mf->Material.TextureLayer[ 0u ].TextureWrapV = irr::video::ETC_CLAMP_TO_EDGE;
	}
	setInput( 0u, follow );
}
void CLensFlarePostProc::renderEffect( irr::video::ITexture* target ) {
	irr::video::IVideoDriver* driver = smgr->getVideoDriver( );
	
	if( !hasInput( 0u ) )
		return;
	
	if( !supported || !enabled ) {
		requestInput( 0u, target );
		return;
	}
	
	irr::video::ITexture* tex = target;
	if( target == NULL ) {
		requestInput( 0u );
		tex = getInputTexture( 0u );
	}
	requestInput( 0u, target );
	
	if( track != NULL ) {
		track->updateAbsolutePosition( ); // Prevent object lag
		pos = track->getAbsolutePosition( );
	}
	
	irr::scene::ISceneCollisionManager* scm = smgr->getSceneCollisionManager( );
	
	smgr->getActiveCamera( )->updateAbsolutePosition( ); // Prevent camera lag
	irr::core::position2d<irr::s32> pS = scm->getScreenCoordinatesFrom3DPosition( pos );
	DIMENSION2D dsc = driver->getScreenSize( );
	if( pS.X < 0 || pS.Y < 0 || pS.X >= dsc.Width || pS.Y >= dsc.Height )
		return;
	
	DIMENSION2D dim = tex->getSize( );
	irr::core::position2d<irr::f32> pp = irr::core::position2d<irr::f32>( pS.X / (irr::f32) dsc.Width, pS.Y / (irr::f32) dsc.Height );
	
	irr::f32 mr = mCol.getRed( ), mg = mCol.getGreen( ), mb = mCol.getBlue( );
	
	irr::u32 p, px, py;
	// ToDo: set realRad to the radius (in pixels) of the sun after projection
//	irr::core::vector3d<irr::f32> tmp = pos;
//	(smgr->getActiveCamera( )->getViewMatrix( ) * smgr->getActiveCamera( )->getProjectionMatrix( )).transformVect( tmp );
	irr::f32 realRad = rad * 0.4f; //((irr::f32) dsc.Width * rad) / tmp.Z;
	if( quality <= PPQ_CRUDE ) {
		if( track != NULL ) {
			// We do a simple test to see if the centre of the sun is visible. This only checks bounding boxes, so is often fairly random
			if( scm->getSceneNodeFromScreenCoordinatesBB( irr::core::position2d<irr::s32>( (irr::s32) (pp.X * dsc.Width), (irr::s32) (pp.Y * dsc.Height) ), 0, true ) == track )
				mr = mg = mb = 1.0f;
			else
				mr = mg = mb = 0.0f;
		}
		// If we aren't following an object, just assume the sun is 100% visible. Bad assumption, but all methods are too slow for the CRUDE setting.
	} else if( quality <= PPQ_FAST || realRad <= 0.5f ) {
		// We do a simple pixel test at the center of the sun - if it is < 0.9 on average, assume the sun is covered.
		if( dim.Width != 0 && dim.Height != 0 && pp.X >= 0.0f && pp.Y >= 0.0f && pp.X < 1.0f && pp.Y < 1.0f ) {
			irr::u8* tmp2 = (irr::u8*) tex->lock();
			if( tmp2 ) {
				py = ((irr::u32) (pp.Y * dim.Height)) * tex->getPitch( );
				px = (irr::u32) (pp.X * dim.Width);
				switch( tex->getColorFormat( ) ) {
					case irr::video::ECF_A8R8G8B8:
						p = *((irr::u32*) (tmp2 + py + px * 4u));
						mr = ((p >> 16) & 0xFF) / 255.0f;
						mg = ((p >> 8) & 0xFF) / 255.0f;
						mb = (p & 0xFF) / 255.0f;
						break;
					case irr::video::ECF_R8G8B8:
						p = *((irr::u32*) (tmp2 + py + px * 3u));
						mr = ((p >> 16) & 0xFF) / 255.0f;
						mg = ((p >> 8) & 0xFF) / 255.0f;
						mb = (p & 0xFF) / 255.0f;
						break;
					case irr::video::ECF_A1R5G5B5:
						p = *((irr::u32*) (tmp2 + py + px * 2u));
						mr = (((p >> 10) & 0x1F) << 3) / 255.0f;
						mg = (((p >> 5) & 0x1F) << 3) / 255.0f;
						mb = ((p & 0x1F) << 3) / 255.0f;
						break;
					case irr::video::ECF_R5G6B5:
						p = *((irr::u32*) (tmp2 + py + px * 2u));
						mr = (((p >> 11) & 0x1F) << 3) / 255.0f;
						mg = (((p >> 5) & 0x3F) << 2) / 255.0f;
						mb = ((p & 0x1F) << 3) / 255.0f;
						break;
				}
				mr = mg = mb = (mr + mg + mb) > (0.9f * 3.0f) ? 1.0f : 0.0f;
				tex->unlock( );
			}
		}
	} else {
		// We test pixels within the radius of the sun to guess how occluded it is
		if( dim.Width != 0 && dim.Height != 0 && pp.X >= 0.0f && pp.Y >= 0.0f && pp.X < 1.0f && pp.Y < 1.0f ) {
			irr::u8* tmp2 = (irr::u8*) tex->lock();
			if( tmp2 ) {
				py = ((irr::u32) (pp.Y * dim.Height)) * tex->getPitch( );
				px = (irr::u32) (pp.X * dim.Width);
				irr::s32 ppx, ppy;
				irr::f32 xx, yy;
				for( xx = -1.0f; xx <= 1.0f; xx += 0.1f )
					for( yy = -1.0f; yy <= 1.0f; yy += 0.1f )
						if( xx * xx + yy * yy <= 1.0f ) {
							mg += 1.0f;
							ppy = py + ((irr::s32) (yy * realRad)) * tex->getPitch( );
							ppx = px + (irr::s32) (xx * realRad);
							if( ppx >= 0 && ppx < dim.Width && ppy >= 0 && ppy < dim.Height * (irr::s32) (tex->getPitch( )) - 1 ) {
								switch( tex->getColorFormat( ) ) {
									case irr::video::ECF_A8R8G8B8:
										p = *((irr::u32*) (tmp2 + ppy + ppx * 4u));
										mr += (((p >> 16) & 0xFF) + ((p >> 8) & 0xFF) + (p & 0xFF)) > (0.9f * 765.0f) ? 1.0f : 0.0f;
										break;
									case irr::video::ECF_R8G8B8:
										p = *((irr::u32*) (tmp2 + ppy + ppx * 3u));
										mr += (((p >> 16) & 0xFF) + ((p >> 8) & 0xFF) + (p & 0xFF)) > (0.9f * 765.0f) ? 1.0f : 0.0f;
										break;
									case irr::video::ECF_A1R5G5B5:
										p = *((irr::u32*) (tmp2 + ppy + ppx * 2u));
										mr += ((((p >> 10) & 0x1F) << 3) + (((p >> 5) & 0x1F) << 3) + ((p & 0x1F) << 3)) > (0.9f * 765.0f) ? 1.0f : 0.0f;
										break;
									case irr::video::ECF_R5G6B5:
										p = *((irr::u32*) (tmp2 + ppy + ppx * 2u));
										mr += ((((p >> 11) & 0x1F) << 3) + (((p >> 5) & 0x3F) << 2) + ((p & 0x1F) << 3)) > (0.9f * 765.0f) ? 1.0f : 0.0f;
										break;
								}
							}
						}
				mr /= mg;
				mg = mb = mr;
				tex->unlock( );
			}
		}
	}
	
	pp = irr::core::position2d<irr::f32>( pp.X * 2.0f - 1.0f, pp.Y * 2.0f - 1.0f );
	irr::f32 sideFade = irr::core::max_( 1.0f - irr::core::max_( pp.X * pp.X * pp.X * pp.X + pp.Y * pp.Y * pp.Y * pp.Y - 0.5f, 0.0f ) * 2.0f, 0.0f );
	irr::f32 lr = sqrtf( pp.X * pp.X + pp.Y * pp.Y );
	irr::video::SColor lum = irr::video::SColor( 255u, (irr::u32) (mr * sideFade * 255.0f), (irr::u32) (mg * sideFade * 255.0f), (irr::u32) (mb * sideFade * 255.0f) );
	irr::f32 rot = atan2( pp.Y * dsc.Height, pp.X * dsc.Width );
	irr::f32 ss = sinf( rot );
	irr::f32 cc = cosf( rot );
	irr::core::vector2d<irr::f32> c;
	mf->Vertices[ 0u ].Pos = irr::core::vector3d<irr::f32>( (ss * flR[ 0u ] + cc * lr) * dsc.Height / (irr::f32) dsc.Width, cc * flR[ 0u ] - ss * lr, 0.0f );
	mf->Vertices[ 1u ].Pos = irr::core::vector3d<irr::f32>( (ss * flR[ 0u ] - cc * lr) * dsc.Height / (irr::f32) dsc.Width, ss * lr + cc * flR[ 0u ], 0.0f );
	mf->Vertices[ 2u ].Pos = irr::core::vector3d<irr::f32>( (cc * lr - ss * flR[ 0u ]) * dsc.Height / (irr::f32) dsc.Width,-cc * flR[ 0u ] - ss * lr, 0.0f );
	mf->Vertices[ 3u ].Pos = irr::core::vector3d<irr::f32>(-(cc * lr + ss * flR[ 0u ]) * dsc.Height / (irr::f32) dsc.Width, ss * lr - cc * flR[ 0u ], 0.0f );
	irr::f32 m = irr::core::max_( irr::core::min_( lr * 4.0f - 1.0f, 1.0f ), 0.0f );
	irr::video::SColor lum2 = irr::video::SColor( 255u, (irr::u32) (lum.getRed( ) * m), (irr::u32) (lum.getGreen( ) * m), (irr::u32) (lum.getBlue( ) * m) );
	mf->Vertices[ 0u ].Color = lum2;
	mf->Vertices[ 1u ].Color = lum2;
	mf->Vertices[ 2u ].Color = lum2;
	mf->Vertices[ 3u ].Color = lum2;
	for( irr::u8 i = 1u; i != flP.size( ); ++ i ) {
		c = irr::core::vector2d<irr::f32>( pp.X * (1.0f - flP[ i ] * 2.0f), -pp.Y * (1.0f - flP[ i ] * 2.0f) );
		mf->Vertices[ i * 4u + 0u ].Pos = irr::core::vector3d<irr::f32>( c.X + (ss + cc) * flR[ i ] * dsc.Height / (irr::f32) dsc.Width, c.Y - (ss - cc) * flR[ i ], 0.0f );
		mf->Vertices[ i * 4u + 1u ].Pos = irr::core::vector3d<irr::f32>( c.X - (cc - ss) * flR[ i ] * dsc.Height / (irr::f32) dsc.Width, c.Y + (ss + cc) * flR[ i ], 0.0f );
		mf->Vertices[ i * 4u + 2u ].Pos = irr::core::vector3d<irr::f32>( c.X - (ss - cc) * flR[ i ] * dsc.Height / (irr::f32) dsc.Width, c.Y - (cc + ss) * flR[ i ], 0.0f );
		mf->Vertices[ i * 4u + 3u ].Pos = irr::core::vector3d<irr::f32>( c.X - (cc + ss) * flR[ i ] * dsc.Height / (irr::f32) dsc.Width, c.Y - (cc - ss) * flR[ i ], 0.0f );
		mf->Vertices[ i * 4u + 0u ].Color = lum;
		mf->Vertices[ i * 4u + 1u ].Color = lum;
		mf->Vertices[ i * 4u + 2u ].Color = lum;
		mf->Vertices[ i * 4u + 3u ].Color = lum;
	}
	
	driver->setRenderTarget( target, false, false );
	driver->setMaterial( mf->Material );
	driver->drawMeshBuffer( mf );
}
