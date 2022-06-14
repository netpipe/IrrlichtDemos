/*
===============================================================================
	File:	CGeomoddableSolid_v0.inl
	Desc:.
===============================================================================
*/

#define USE_PHYSICS
#define DRAW_AABB

//
//	CGeomoddableSolid_v0 - version 0.0.
//

class CGeomoddableSolid_v0 : public mxGeomoddableSolid {
public:
			CGeomoddableSolid_v0();
			~CGeomoddableSolid_v0();

	void	CreateFromMesh( const ::irr::scene::IMesh* mesh );

	// overrides ( mxRenderEntity )
	void	Render( const mxView& view );
	bool	IsVisible( const mxView& view );

	void	ApplyDecal( const Vec3D& position, const Vec3D& normal );

	//void	ApplyFracture( mxCrackPattern& fracture );
	//void	CSG_Subtract( mxGeoModMesh& mesh );
	//void	CSG_Union( mxGeoModMesh& mesh );

	void	SetMeshForSubtract( const ::irr::scene::IMesh* mesh );
	void	SetMeshForUnion( const ::irr::scene::IMesh* mesh );

	bool	Debug_TraceLine_Subtract( const Vec3D& start, const Vec3D& end );
	bool	Debug_TraceLine_Union( const Vec3D& start, const Vec3D& end );

private:
	// TODO: move this into the BSP tree.
	mxRenderMesh	m_renderMesh;	// For storing geometry (vertices & indices) and rendering.

	NodeyBSPTree	*m_tree;		// Spatial sorting, CSG, etc.

	mxRenderMesh	m_geoModMesh;	// Subtractive mesh.
	Vec3D			m_vLastGeoModMeshTranslation;

	mxRenderMesh	m_meshForUnion;
	Vec3D			m_vLastUnionMeshTranslation;

	mxAction_Bool	m_wireframeMode;

	//********************
	// btPhysics
	btMotionState *	m_pMotionState;
	btRigidBody *	m_pRigidBody;
};

/*================================
	class CGeomoddableSolid_v0
================================*/

CGeomoddableSolid_v0::CGeomoddableSolid_v0()
{
//	SetPosition( Vec3D( 0,0,0 ) );
//	SetRotation( Vec3D( 0,0,0 ) );
//	UpdateWorldTransform();

	//************
	m_pMotionState = NULL;
	m_pRigidBody = NULL;

//	::mix::GWorld->AddAction( m_wireframeMode, ::irr::EKEY_CODE::KEY_KEY_O );
	//m_wireframeMode.SetBool( false );
}

CGeomoddableSolid_v0::~CGeomoddableSolid_v0()
{
	delete m_pMotionState;

	if ( m_pRigidBody ) {
		::mix::GPhysicsWorld->m_pDynamicsWorld->removeRigidBody( m_pRigidBody );
		delete m_pRigidBody;
	}
}

void CGeomoddableSolid_v0::SetMeshForSubtract( const ::irr::scene::IMesh* mesh )
{
	m_geoModMesh.CopyFromMesh( mesh );

	// Center the geomod mesh.
	AABB  aabb;
	m_geoModMesh.ComputeBoundingBox( aabb );

	TMatrix4  matrix;
	matrix.setTranslation( -aabb.getCenter() );
	matrix.setRotationRadians( Vec3D( 0.1f, 0.3f, 0.5f ) );	// random rotation
	m_geoModMesh.TransformVertices( matrix );

	m_vLastGeoModMeshTranslation = Vec3D( 0,0,0 );
}

void CGeomoddableSolid_v0::SetMeshForUnion( const ::irr::scene::IMesh* mesh )
{
	m_meshForUnion.CopyFromMesh( mesh );

	// Center the geomod mesh.
	AABB  aabb;
	m_meshForUnion.ComputeBoundingBox( aabb );

	TMatrix4  matrix;
	matrix.setTranslation( -aabb.getCenter() );
	matrix.setRotationRadians( Vec3D( -0.1f, 0.1f, -0.5f ) );	// random rotation
	m_meshForUnion.TransformVertices( matrix );

	m_vLastUnionMeshTranslation = Vec3D( 0,0,0 );
}

void CGeomoddableSolid_v0::CreateFromMesh( const ::irr::scene::IMesh* mesh )
{
	m_renderMesh.CopyFromMesh( mesh );

	//m_renderMesh.WeldVertices();
	// make sure that the mesh is consistent ( manifold )
	//m_tree->MakeManifold( m_renderMesh );

	m_tree->Build( m_renderMesh );
	m_renderMesh.RemoveAll();
	m_tree->GenerateMesh( m_renderMesh );

#ifdef USE_PHYSICS
	//m_physics = ::mix::GPhysicsWorld->GetStaticTriangleMeshPhysicsEntity( m_renderMesh );
	//m_physics->SetEntityPtr( this );

	btCollisionShape * pShape =  m_tree;

	btTransform   staticMeshTransform;
	staticMeshTransform.setIdentity();

	btScalar   mass( 0. );
	btVector3  localInertia( 0,0,0 );

	m_pMotionState = new btDefaultMotionState( staticMeshTransform );
	btRigidBody::btRigidBodyConstructionInfo  rbInfo( mass, m_pMotionState, pShape, localInertia );

	m_pRigidBody = new btRigidBody( rbInfo );
	m_pRigidBody->setUserPointer( this );
	::mix::GPhysicsWorld->m_pDynamicsWorld->addRigidBody( m_pRigidBody );
#endif
}

void CGeomoddableSolid_v0::Render( const mxView& view )
{
	GRenderWorld->Driver->setTransform( ::irr::video::ETS_WORLD, m_worldTransform );
	GRenderWorld->Driver->setMaterial( mxRenderWorld::DefaultSolidMaterial );
	m_renderMesh.Render();

	if ( m_wireframeMode.GetBool() )
	{
		GRenderWorld->Driver->setMaterial( GRenderWorld->WireframeMaterial );
		m_renderMesh.Render();

		//GRenderWorld->Driver->setMaterial( GRenderWorld->NoLightMaterial );
		//m_renderMesh.DrawWireframe();

#ifdef DRAW_AABB
		GRenderWorld->Driver->setMaterial( GRenderWorld->AABBMaterial );
		GRenderWorld->Driver->draw3DBox( m_tree->GetBounds() );
#endif
	}
}

bool CGeomoddableSolid_v0::IsVisible( const mxView& view )
{
	return VolumeIntersectsFrustum( m_tree->GetBounds(), view.Frustum );
}

//
//void CGeomoddableSolid_v0::ApplyFracture( mxCrackPattern& fracture )
//{}

//void CGeomoddableSolid_v0::CSG_Subtract( mxGeoModMesh& mesh )
//{}
//
//void CGeomoddableSolid_v0::CSG_Union( mxGeoModMesh& mesh )
//{}

bool CGeomoddableSolid_v0::Debug_TraceLine_Subtract( const Vec3D& start, const Vec3D& end )
{
	Vec3D  intersection;
	if ( m_tree->TraceLine( start, end, intersection ) )
	{
		DEBUG_CODE( u32 beginTime = GSystem->GetRealTime() );

		TMatrix4  matrix;
		matrix.setTranslation( intersection - m_vLastGeoModMeshTranslation );
		m_geoModMesh.TransformVertices( matrix );
		m_vLastGeoModMeshTranslation = intersection;

		m_tree->Subtract( m_geoModMesh );
		m_tree->GenerateMesh( m_renderMesh );

		DEBUG_CODE( u32 endTime = GSystem->GetRealTime(); GCommon->DebugPrint( "\nCSG: %u ms\n", endTime-beginTime ); );
		return true;
	}
	return false;
}

bool CGeomoddableSolid_v0::Debug_TraceLine_Union( const Vec3D& start, const Vec3D& end )
{
	Vec3D  intersection;
	if ( m_tree->TraceLine( start, end, intersection ) )
	{
		DEBUG_CODE( u32 beginTime = GSystem->GetRealTime() );

		TMatrix4  matrix;
		matrix.setTranslation( intersection - m_vLastUnionMeshTranslation );
		m_meshForUnion.TransformVertices( matrix );
		m_vLastUnionMeshTranslation = intersection;

		m_tree->Union( m_meshForUnion );
		m_tree->GenerateMesh( m_renderMesh );

		DEBUG_CODE( u32 endTime = GSystem->GetRealTime(); GCommon->DebugPrint( "\nCSG: %u ms\n", endTime-beginTime ); );
		return true;
	}
	return false;
}

void CGeomoddableSolid_v0::ApplyDecal( const Vec3D& position, const Vec3D& normal )
{
//	::mix::GRenderWorld->CreatePermMark( position );
}

//--------------------------------------------------------------//
//				End Of File.									//
//--------------------------------------------------------------//
