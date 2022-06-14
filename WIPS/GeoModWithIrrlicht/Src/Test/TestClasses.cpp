/*
===============================================================================
	File:	TestClasses.cpp
	Desc:	Experimental code for testing ideas.
===============================================================================
*/

#include "stdafx.h"
#pragma hdrstop
#include <Include/IrrMix.h>

namespace mix {

/*================================
	class TestLevel
================================*/

TestLevel::TestLevel()
	: m_pSceneNode( NULL )
{
}

TestLevel::~TestLevel()
{
}

void TestLevel::Spawn()
{
	mxEnvironment::Spawn();

	GWorld->Environment->AddEntity( new mxStaticPlaneEntity() );

	mxGamePlayer * player = GWorld->SpawnHumanPlayer( Vec3D( 10.0f, 2.0f, 10.0f ) );
	AddEntity( player );

	player->GiveWeapon( CreateWeapon_GeoModder() );
	player->GiveWeapon( CreateWeapon_SMG() );
	player->GiveWeapon( CreateWeapon_TestObjectSpawner() );

	//-----------------------------------------------------------
	// Create a static geomoddable solid.
#if 1
	const char* szInputMesh = PATH_TO_MEDIA "/Models/Test/"
	//	"cube.3DS"
		"test_0.3DS"
	//	"test_2_cyl_tube.3DS"
	//	"model-noname.3DS"
	;

	const char* szGeoModMesh = PATH_TO_MEDIA "/Models/Test/"
		"cube.3DS"
	//	"test_0.3DS"
	//	"pyramid.3DS"
	;


	::irr::scene::IMesh * pMesh = GRenderWorld->SceneManager->getMesh( szInputMesh );
	//GRenderWorld->SceneManager->getMeshManipulator()->flipSurfaces( pMesh );
	const Vec3D  vScale1( 1.0f, 1.f, 1.0f );
	GRenderWorld->SceneManager->getMeshManipulator()->scale( pMesh, vScale1/10 );



	::irr::scene::IMesh * pGeomodMesh = GRenderWorld->SceneManager->getMesh( szGeoModMesh );
	GRenderWorld->SceneManager->getMeshManipulator()->flipSurfaces( pGeomodMesh );
	const Vec3D  vScale2( 0.1f, 0.1f, 0.1f );
	GRenderWorld->SceneManager->getMeshManipulator()->scale( pGeomodMesh, vScale2/2 );




	m_pSolid = GetDestructionManager().CreateSolid();
	m_pSolid->CreateFromMesh( pMesh );
	m_pSolid->SetMeshForSubtract( pGeomodMesh );

	GRenderWorld->SceneManager->getMeshManipulator()->flipSurfaces( pGeomodMesh );
	GRenderWorld->SceneManager->getMeshManipulator()->scale( pGeomodMesh, Vec3D( 1.3f, 1.3f, 1.3f ) );
	m_pSolid->SetMeshForUnion( pGeomodMesh );

	AddEntity( m_pSolid );
#endif
	//-----------------------------------------------------------

#if 0
	// Create portals.
	mxPortal * portal0 = new mxRectangularPortal_RTT(
		Vec3D( 1, 2, 1 ),	// dimensions
		Vec3D( 20, 1, 20 ) );
	AddEntity( portal0 );

	mxPortal * portal1 = new mxPortalExitPoint( Vec3D( -20, 1, -20 ) );
	AddEntity( portal1 );

	portal0->Link( portal1 );
	portal1->Link( portal0 );

#endif

	//-----------------------------------------------------------

#if 0 // create a light
	const f32  fPos = 90000.f;
	::irr::scene::ILightSceneNode * pLight = GRenderWorld->SceneManager->addLightSceneNode();
	pLight->setLightType(
		::irr::video::E_LIGHT_TYPE::ELT_POINT
		//::irr::video::E_LIGHT_TYPE::ELT_SPOT
		//::irr::video::E_LIGHT_TYPE::ELT_DIRECTIONAL
	);
	pLight->setPosition( Vec3D( -fPos, fPos/5, -fPos ) );
	pLight->setRadius( fPos * 2 );

	{
		::irr::video::SColorf & rDiffuseColor = pLight->getLightData().DiffuseColor;
		rDiffuseColor.r = 0.98f;
		rDiffuseColor.b = 0.5f;
		rDiffuseColor.g = 0.5f;
	}
#endif



#if 1	//--- create skybox -------------------------------

	GRenderWorld->Driver->setTextureCreationFlag( ::irr::video::ETCF_CREATE_MIP_MAPS, false );
	::irr::scene::ISceneNode* skybox = GRenderWorld->SceneManager->addSkyBoxSceneNode(
/*
			GRenderWorld->Driver->getTexture( PATH_TO_MEDIA "Images/Skybox/skyboxsun5deg2/skyrender0003.bmp" ),	// up
			GRenderWorld->Driver->getTexture( PATH_TO_MEDIA "Images/Skybox/skyboxsun5deg2/skyrender0003.bmp" ),	// down
			GRenderWorld->Driver->getTexture( PATH_TO_MEDIA "Images/Skybox/skyboxsun5deg2/skyrender0005.bmp" ),	// left
			GRenderWorld->Driver->getTexture( PATH_TO_MEDIA "Images/Skybox/skyboxsun5deg2/skyrender0002.bmp" ),	// right
			GRenderWorld->Driver->getTexture( PATH_TO_MEDIA "Images/Skybox/skyboxsun5deg2/skyrender0001.bmp" ),	// front
			GRenderWorld->Driver->getTexture( PATH_TO_MEDIA "Images/Skybox/skyboxsun5deg2/skyrender0004.bmp" ));// back
*/
			GRenderWorld->Driver->getTexture( PATH_TO_MEDIA "From Irrlicht SDK/irrlicht2_up.jpg" ),	// up
			GRenderWorld->Driver->getTexture( PATH_TO_MEDIA "From Irrlicht SDK/irrlicht2_dn.jpg" ),	// down
			GRenderWorld->Driver->getTexture( PATH_TO_MEDIA "From Irrlicht SDK/irrlicht2_lf.jpg" ),	// left
			GRenderWorld->Driver->getTexture( PATH_TO_MEDIA "From Irrlicht SDK/irrlicht2_rt.jpg" ),	// right
			GRenderWorld->Driver->getTexture( PATH_TO_MEDIA "From Irrlicht SDK/irrlicht2_ft.jpg" ),	// front
			GRenderWorld->Driver->getTexture( PATH_TO_MEDIA "From Irrlicht SDK/irrlicht2_bk.jpg" )); // back
	GRenderWorld->Driver->setTextureCreationFlag( ::irr::video::ETCF_CREATE_MIP_MAPS, true );
	//::irr::scene::ISceneNodeAnimator * const skyAnim = GRenderWorld->SceneManager->createRotationAnimator( Vec3D( 0.0f, 0.025f, 0.0f ) );
	//skybox->addAnimator( skyAnim );
	//skyAnim->drop();
#endif

}

void TestLevel::Shutdown()
{
	// Delete scene nodes.
	if ( m_pSceneNode ) {
		m_pSceneNode->remove();
	}

	// Shutdown environment.
	mxEnvironment::Shutdown();
}

bool TestLevel::Debug_TraceLine( const Vec3D& start, const Vec3D& end, Vec3D &OutResult, void* pUserPtr )
{
	(void) OutResult;

	if ( pUserPtr ) {
		return m_pSolid->Debug_TraceLine_Subtract( start, end );
	}
	return m_pSolid->Debug_TraceLine_Union( start, end );
}

/*================================
	class mxBoxEntity
================================*/

mxBoxEntity::mxBoxEntity( const Vec3D& dimensions, const Vec3D& position )
{
	m_worldTransform.setTranslation( position );

	//-----------------------------------
	// visuals

	const TIndex  indices[36] = {   0,2,1,   0,3,2,   1,5,4,   1,2,5,   4,6,7,   4,5,6,
        7,3,0,   7,6,3,   9,5,2,   9,8,5,   0,11,10,   0,10,7};

	m_renderMesh.Triangles.SetNum( 12 );
	MemCopy( m_renderMesh.Triangles.Ptr(), indices, 36 * sizeof( TIndex ) );

	::irr::video::SColor	color( 255, 255, 255, 255 );

	m_renderMesh.Vertices.SetNum( 12 );

	const f32  dimX = dimensions.X;
	const f32  dimY = dimensions.Y;
	const f32  dimZ = dimensions.Z;

	// UNDONE: recalculation of normals
	//
	m_renderMesh.Vertices[0]  = mxVertex( Vec3D( 0,		0,		0	),	Vec3D( -1,-1,-1 ),	0, 1,	color	);
	m_renderMesh.Vertices[1]  = mxVertex( Vec3D( dimX,	0,		0	),	Vec3D( 1,-1,-1 ),	1, 1,	color	);
	m_renderMesh.Vertices[2]  = mxVertex( Vec3D( dimX,	dimY,	0	),	Vec3D( 1, 1,-1 ),	1, 0,	color	);
	m_renderMesh.Vertices[3]  = mxVertex( Vec3D( 0,		dimY,	0	),	Vec3D( -1, 1,-1 ),	0, 0,	color	);
	m_renderMesh.Vertices[4]  = mxVertex( Vec3D( dimX,	0,		dimZ),	Vec3D( 1,-1, 1 ),	0, 1,	color	);
	m_renderMesh.Vertices[5]  = mxVertex( Vec3D( dimX,	dimY,	dimZ),	Vec3D( 1, 1, 1 ),	0, 0,	color	);
	m_renderMesh.Vertices[6]  = mxVertex( Vec3D( 0,		dimY,	dimZ),	Vec3D( -1, 1, 1 ),	1, 0,	color	);
	m_renderMesh.Vertices[7]  = mxVertex( Vec3D( 0,		0,		dimZ),	Vec3D( -1,-1, 1 ),	1, 1,	color	);
	m_renderMesh.Vertices[8]  = mxVertex( Vec3D( 0,		dimY,	dimZ),	Vec3D( -1, 1, 1 ),	0, 1,	color	);
	m_renderMesh.Vertices[9]  = mxVertex( Vec3D( 0,		dimY,	0	),	Vec3D( -1, 1,-1 ),	1, 1,	color	);
	m_renderMesh.Vertices[10] = mxVertex( Vec3D( dimX,	0,		dimZ),	Vec3D( 1,-1, 1 ),	1, 0,	color	);
	m_renderMesh.Vertices[11] = mxVertex( Vec3D( dimX,	0,		0	),	Vec3D( 1,-1,-1 ),	0, 0,	color	);

	for ( u32 i = 0; i < 12; ++i )
	{
		m_renderMesh.Vertices[i].Pos -= Vec3D( dimX/2, dimY/2, dimZ/2 );
		//m_renderMesh.Vertices[i].Pos *= m_scale;
	}

	m_material.setFlag( irr::video::EMF_LIGHTING, false );
	m_material.TextureLayer[0].Texture = GRenderWorld->Driver->getTexture( PATH_TO_MEDIA "Images/Textures/Test/LIMESTO3.jpg" );

	//-----------------------------------
	// physics
	const f32   mass( 100 );
	m_physics = GPhysicsWorld->GetBoxPhysicsEntity( dimensions, mass, position );
	m_physics->SetEntityPtr( this );

	//-----------------------------------
	// Decals.
	m_arrDecals.Resize( 256 /* newSize */, 64 /* newGranularity */ );

//	m_decalMaterial.setFlag( ::irr::video::E_MATERIAL_FLAG::EMF_LIGHTING, true );
	m_decalMaterial.MaterialType = irr::video::EMT_TRANSPARENT_ADD_COLOR;

	m_decalMaterial.setFlag( irr::video::EMF_LIGHTING, false );
	m_decalMaterial.setFlag( irr::video::EMF_ZBUFFER, false );

	m_decalMaterial.TextureLayer[0].Texture = GRenderWorld->Driver->getTexture( PATH_TO_MEDIA "Images/Textures/Decals/hole_lg_mrk.jpg" );
}

mxBoxEntity::~mxBoxEntity()
{
}

void mxBoxEntity::Render( const mxView& view )
{
	GRenderWorld->Driver->setMaterial( m_material );
	GRenderWorld->Driver->setTransform( ::irr::video::ETS_WORLD, m_worldTransform );
	m_renderMesh.Render();

	// Render decals.
//	GRenderWorld->Driver->setMaterial( m_decalMaterial );
//	const static u16 indices[] = { 0,1,2, 2,3,0 };
//	for ( u32 i=0; i < m_arrDecals.Num(); i++ ) {
//		GRenderWorld->Driver->drawIndexedTriangleList( (const TVertex*) m_arrDecals[i].quad, 4, indices, 2 );
//	}
}

void mxBoxEntity::ApplyDecal( const Vec3D& position, const Vec3D& normal )
{
//	Decal * newDecal = new (m_arrDecals) Decal();
//	newDecal->Init( position, normal, 5 );
}

void mxBoxEntity::RunPhysics()
{
	SetPosition( m_physics->GetOrigin() );
	SetRotation( m_physics->GetRotationInDegrees() );
}

/*================================
	class mxBoxEntity::Decal
================================*/

// Creates a quad representing decal.
//
void mxBoxEntity::Decal::Init( const Vec3D& position, const Vec3D& normal, const f32 size )
{
	// Get the plane defined by the normal and the point on that plane.
	TPlane3D    decalPlane( position, normal );

	// Make a quad that lies on the plane.

	// First create a cirle lying on the plane.
	const f32  fRadius = (size / 2) * idMath::SQRT_TWO;


DEBUG_BREAK;
	// Pick a random point on that plane.
	Vec3D   point( 66, 66, 0 );

	const Vec3D  vUp	( 0, 0, 1 );
	const Vec3D  vRight	( 1, 0, 0 );

	// If viewed from outside:

	// lower left corner
	quad[ 0 ] = TVertex(
		Vec3D( 0,0,0 ),	// position
		normal,			// normal
		TColorARGB( 255,255,255,255 ),
		Vec2D( 0, 1 )	// u, v
	);

	// upper left corner
	quad[ 1 ] = TVertex(
		Vec3D( 0,0,1 ),	// position
		normal,			// normal
		TColorARGB( 255,255,255,255 ),
		Vec2D( 0, 0 )	// u, v
	);

	// upper right corner
	quad[ 2 ] = TVertex(
		Vec3D( 1,0,1 ),	// position
		normal,			// normal
		TColorARGB( 255,255,255,255 ),
		Vec2D( 1, 0 )	// u, v
	);

	// lower right corner
	quad[ 3 ] = TVertex(
		Vec3D( 1,0,0 ),	// position
		normal,			// normal
		TColorARGB( 255,255,255,255 ),
		Vec2D( 1, 1 )	// u, v
	);

	const f32  fOffset = 0.001f;

	const f32  fScale = 0.1f;

	for ( u32 i=0; i < 4; i++ ) {
		quad[ i ].Pos *= fScale;
		quad[ i ].Pos += position;
		quad[ i ].Pos.Y += fOffset;
		quad[ i ].Color = ::irr::video::SColor( 255,255,255,255 );
	}
}

/*================================
	class mxStaticPlaneEntity
================================*/

mxStaticPlaneEntity::mxStaticPlaneEntity( const TPlane3D& plane /* =TPlane3D( Vec3D( 0,1,0 ), 0 ) */ )
	: m_plane( plane )
{
	if ( ! VectorsEqual( m_plane.Normal, Vec3D(0,1,0) ) ) {
		Unimplemented2( "slanted plane" );
	}

	// If viewed from above then

	// lower left corner
	mxVertex  v0(
		Vec3D( 0,0,0 ),	// position
		Vec3D( 0,1,0 ),	// normal
		0, 1			// u, v
	);

	// upper left corner
	mxVertex  v1(
		Vec3D( 0,0,1 ),	// position
		Vec3D( 0,1,0 ),	// normal
		0, 0			// u, v
	);

	// upper right corner
	mxVertex  v2(
		Vec3D( 1,0,1 ),	// position
		Vec3D( 0,1,0 ),	// normal
		1, 0			// u, v
	);

	// lower right corner
	mxVertex  v3(
		Vec3D( 1,0,0 ),	// position
		Vec3D( 0,1,0 ),	// normal
		1, 1			// u, v
	);

	//--------------------------
	const f32  fScale = 100.0f;

	const Vec3D  vTranslation( -fScale/2, 0, -fScale/2 );

	m_vertices[0] = v0;
	m_vertices[1] = v1;
	m_vertices[2] = v2;
	m_vertices[3] = v3;

	for ( u32 i=0; i < 4; i++ ) {
		m_vertices[ i ].Pos *= fScale;

		m_vertices[ i ].Pos += vTranslation;

		m_vertices[ i ].U *= 10;
		m_vertices[ i ].V *= 10;
	}

	//--------------------------
//	const f32  fExtent = 9000.0f;
//	const AABB  bounds( Vec3D( -fExtent, 0, -fExtent ), Vec3D( +fExtent, 0, +fExtent ) );

//	m_worldTransform.setTranslation( Vec3D(
//		(-bounds.MaxEdge.X + bounds.MinEdge.X)/2, 0, (-bounds.MaxEdge.Z + bounds.MinEdge.Z)/2
//	) );

	//--------------------------
//	m_material.setFlag( ::irr::video::E_MATERIAL_FLAG::EMF_LIGHTING, true );

	//m_material.MaterialType = ::irr::video::E_MATERIAL_TYPE::EMT_DETAIL_MAP;
//	m_material.MaterialType = ::irr::video::E_MATERIAL_TYPE::EMT_SOLID_2_LAYER;

	m_material.TextureLayer[0].Texture = GRenderWorld->Driver->getTexture( PATH_TO_MEDIA "Images/Textures/Terrain/blue-terrain-texture.jpg" );
	m_material.TextureLayer[1].Texture = GRenderWorld->Driver->getTexture( PATH_TO_MEDIA "Images/Textures/Terrain/terrain-detailmap.jpg" );

	//--------------------------
	// Physics.
	m_physics = GPhysicsWorld->GetStaticPlanePhysicsEntity( plane );
	m_physics->SetEntityPtr( this );

	//--------------------------
	// Decals.
	m_arrDecals.Resize( 256 /* newSize */, 64 /* newGranularity */ );

//	m_decalMaterial.setFlag( ::irr::video::E_MATERIAL_FLAG::EMF_LIGHTING, true );
	m_decalMaterial.MaterialType = irr::video::EMT_TRANSPARENT_ADD_COLOR;

	m_decalMaterial.setFlag( irr::video::EMF_LIGHTING, false );
	m_decalMaterial.setFlag( irr::video::EMF_ZBUFFER, false );

	m_decalMaterial.TextureLayer[0].Texture = GRenderWorld->Driver->getTexture( PATH_TO_MEDIA "Images/Textures/Decals/hole_lg_mrk.jpg" );
}

mxStaticPlaneEntity::~mxStaticPlaneEntity()
{}

void mxStaticPlaneEntity::Render( const mxView& view )
{
	GRenderWorld->Driver->setMaterial( m_material );
	GRenderWorld->Driver->setTransform( ::irr::video::ETS_WORLD, m_worldTransform );

	const static u16 indices[] = { 0,1,2, 2,3,0 };
	GRenderWorld->Driver->drawIndexedTriangleList( (const TVertex*) m_vertices, 4, indices, 2 );

	// Render decals.
	// TODO: cluster decals (if too many of them), organise them in some data structure, cull clusters which are not visible.
	GRenderWorld->Driver->setMaterial( m_decalMaterial );
	for ( u32 i=0; i < m_arrDecals.Num(); i++ ) {
		GRenderWorld->Driver->drawIndexedTriangleList( (const TVertex*) m_arrDecals[i].quad, 4, indices, 2 );
	}
}

// Spawn decal particle for the static surface.
//
void mxStaticPlaneEntity::ApplyDecal( const Vec3D& position, const Vec3D& normal )
{
	(void) normal;

	Decal_s * newDecal = new (m_arrDecals) Decal_s();

	const Vec3D  vNormal( 0, 1, 0 );//m_plane.Normal );
	const Vec3D  vUp	( 0, 0, 1 );
	const Vec3D  vRight	( 1, 0, 0 );

	const f32  fSize = 10.0f;

	// If viewed from air then

	// lower left corner
	newDecal->quad[ 0 ] = TVertex(
		Vec3D( 0,0,0 ),	// position
		Vec3D( 0,1,0 ),	// normal
		TColorARGB( 255,255,255,255 ),
		Vec2D( 0, 1 )	// u, v
	);

	// upper left corner
	newDecal->quad[ 1 ] = TVertex(
		Vec3D( 0,0,1 ),	// position
		Vec3D( 0,1,0 ),	// normal
		TColorARGB( 255,255,255,255 ),
		Vec2D( 0, 0 )	// u, v
	);

	// upper right corner
	newDecal->quad[ 2 ] = TVertex(
		Vec3D( 1,0,1 ),	// position
		Vec3D( 0,1,0 ),	// normal
		TColorARGB( 255,255,255,255 ),
		Vec2D( 1, 0 )	// u, v
	);

	// lower right corner
	newDecal->quad[ 3 ] = TVertex(
		Vec3D( 1,0,0 ),	// position
		Vec3D( 0,1,0 ),	// normal
		TColorARGB( 255,255,255,255 ),
		Vec2D( 1, 1 )	// u, v
	);

	const f32  fOffset = 0.001f;

	const f32  fScale = 0.2f;

	for ( u32 i=0; i < 4; i++ ) {
		newDecal->quad[ i ].Pos *= fScale;
		newDecal->quad[ i ].Pos += position;
		newDecal->quad[ i ].Pos.Y += fOffset;
		//newDecal->quad[ i ].Normal = vNormal;
		newDecal->quad[ i ].Color = ::irr::video::SColor( 255,255,255,255 );
	}

//	GRenderWorld->CreatePermMark( position );
}

}//end of namespace mix

//--------------------------------------------------------------//
//				End Of File.									//
//--------------------------------------------------------------//

