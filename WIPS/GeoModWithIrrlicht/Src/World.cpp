/*
===============================================================================
	File:	mxWorld.cpp
	Desc:	Contains all global variables and the core stuff.
===============================================================================
*/

#include "stdafx.h"
#pragma hdrstop
#include <Include/IrrMix.h>
//#include "World.h"

namespace mix {

//
//	Global variables. These pointers are initialized by mxWorld in Spawn().
//

ICommon *			GCommon			= NULL;
mxSystemWorld *		GSystem			= NULL;
mxRenderWorld *		GRenderWorld	= NULL;

mxPhysicsWorld *	GPhysicsWorld	= NULL;
mxWorld *			GWorld			= NULL;

/*================================
	class mxSystemWorld
================================*/

mxSystemWorld::mxSystemWorld()
: Logger( NULL ), FileSystem( NULL ), CursorControl( NULL ), m_timer( NULL )
{}

void mxSystemWorld::Spawn()
{
#ifdef MX_DEBUG
	// Enable run-time memory check for debug builds.
	int flag = _CrtSetDbgFlag( _CRTDBG_REPORT_FLAG ); // Get current flag

	// Turn on leak-checking bits.
	flag |= _CRTDBG_LEAK_CHECK_DF;
	flag |= _CRTDBG_ALLOC_MEM_DF;

	_CrtSetDbgFlag( flag ); // Set flag to the new value

	// That will report any leaks to the output window when upon exiting the application.
#endif
}

void mxSystemWorld::Shutdown()
{}

void* mxSystemWorld::Allocate( size_t  Num )
{
	return ::operator new ( Num );
}

void* mxSystemWorld::Reallocate( void* old, size_t  Num )
{
	Unimplemented();
	return NULL;
}

void mxSystemWorld::Free( void* mem )
{
	return ::operator delete ( mem );
}

/*================================
	class mxConsole
================================*/

mxConsole::mxConsole()
{}

mxConsole::~mxConsole()
{}

void mxConsole::Show()
{}

void mxConsole::Hide()
{}

mxConsole & mxConsole::Print( const char* text, ... )
{
	GCommon->Print( "%s", text );
	return *this;
}

mxConsole & mxConsole::operator << ( const char* string )
{
	GCommon->Print( "%s", string );
	return *this;
}

mxConsole & mxConsole::operator << ( const Vec3D& vector )
{
	GCommon->Print( "(%f,%f,%f)", vector.X, vector.Y, vector.Z );
	return *this;
}

/*================================
	class mxRenderWorld
================================*/

void mxOnScreenText::ChangeColor( E_Colour newColor )
{
	m_pStaticText->setOverrideColor( GRenderWorld->GetColor( newColor ) );
}

void mxOnScreenText::SetText( const char* format, ... )
{
	va_list	 argptr;

//	va_start( argptr, format );
	vsprintf( m_buffer, format, argptr );
//	va_end( argptr );

	BREAK_IF( strlen( m_buffer ) >= m_Wstring.Num() );

	size_t  result = ::mbstowcs( m_Wstring.Ptr(), m_buffer, m_Wstring.Num() );
	m_pStaticText->setText( m_Wstring.Ptr() );
}

/*================================
	class mxRenderWorld
================================*/

//
//	Static members.
//
const AABB	mxRenderWorld::UniverseExtent( Vec3D( -WORLD_EXTENT, -WORLD_EXTENT, -WORLD_EXTENT ), Vec3D( +WORLD_EXTENT, +WORLD_EXTENT, +WORLD_EXTENT ) );

const TRenderMaterial	mxRenderWorld::NoLightMaterial;
const TRenderMaterial	mxRenderWorld::WireframeMaterial;
const TRenderMaterial	mxRenderWorld::AABBMaterial;

TRenderMaterial			mxRenderWorld::DefaultSolidMaterial;
TRenderMaterial			mxRenderWorld::DecalMaterial;

//----------------------------------------------------------------------------------

mxRenderWorld::mxRenderWorld()
	: Driver( NULL ), SceneManager( NULL ), RootSceneNode( NULL ), GUI( NULL )
	, DesiredFrameRate( 60 ), MaxFrameRate( 150 )
{

#ifdef USE_D3DX9

//	pD3D		= NULL;
//	pD3DDevice  = NULL;

#endif // USE_D3DX9

}

// Initialize the render world for the first time.
//
void mxRenderWorld::Spawn()
{
	// Add Irrlicht logo.
	//GUI->addImage( GRenderWorld->Driver->getTexture( PATH_TO_MEDIA "IrrlichtLogo/irrlichtlogo.png" ),
	//	core::position2d< s32 >( 10,10 ) );


#ifdef USE_D3DX9

//	pD3D		= Driver->getExposedVideoData().D3D9.D3D9;
//	pD3DDevice	= Driver->getExposedVideoData().D3D9.D3DDev9;

#endif // USE_D3DX9



	// Create font.
	::irr::gui::IGUIFont * font = GUI->getFont( PATH_TO_MEDIA "Images/Fonts/fontlucida.png" );
	Assert( font );

	::irr::gui::IGUISkin * currentSkin = GUI->getSkin();
	currentSkin->setFont( font );

	// Change GUI transparency.
	for ( u32 c = 0; c < irr::gui::EGDC_COUNT; ++c )
	{
		::irr::video::SColor col = currentSkin->getColor( irr::gui::EGUI_DEFAULT_COLOR( c ) );
		col.setAlpha( 0 );
		currentSkin->setColor( ::irr::gui::EGUI_DEFAULT_COLOR( c ), col );
	}

	//------------------------------------------------------------------------------

	// TODO: remove this kludge, initialize the material through its ctor.
	// NoLightMaterial
	const_cast< TRenderMaterial& >( NoLightMaterial ).Lighting = false;

	// WireframeMaterial
	const_cast< TRenderMaterial& >( WireframeMaterial ).Wireframe = true;
	const_cast< TRenderMaterial& >( WireframeMaterial ).Lighting = false;

	// AABBMaterial
	//const_cast< TRenderMaterial& >( AABBMaterial ).DiffuseColor = ::irr::video::SColor( 255, 120, 255, 220 );
	const_cast< TRenderMaterial& >( AABBMaterial ).Lighting = false;

	//------------------------------------------------------------------------------

	// DefaultSolidMaterial
	//
//	DefaultSolidMaterial.setFlag( ::irr::video::E_MATERIAL_FLAG::EMF_LIGHTING, false );
	//DefaultSolidMaterial.setFlag( ::irr::video::E_MATERIAL_FLAG::EMF_GOURAUD_SHADING, true );
//	DefaultSolidMaterial.MaterialType = ::irr::video::E_MATERIAL_TYPE::EMT_SOLID;
	DefaultSolidMaterial.setTexture( 0, GRenderWorld->Driver->getTexture( PATH_TO_MEDIA "Images/Textures/Test/Masonry.Unit Masonry.Brick.Modular.Running.Grey.jpg" ) );

	// DecalMaterial
	//
//	DecalMaterial.setFlag( ::irr::video::E_MATERIAL_FLAG::EMF_LIGHTING, true );
	DecalMaterial.MaterialType = irr::video::EMT_TRANSPARENT_ADD_COLOR;

	DecalMaterial.setFlag( irr::video::EMF_LIGHTING, false );
	DecalMaterial.setFlag( irr::video::EMF_ZBUFFER, false );

	DecalMaterial.TextureLayer[0].Texture = GRenderWorld->Driver->getTexture( PATH_TO_MEDIA "Images/Textures/Decals/hole_lg_mrk.jpg" );
}

void mxRenderWorld::RenderFrame()
{
	Driver->beginScene( true, true, ::irr::video::SColor(0,0,200,200) );

	UpdateView();

	// Draw everything.
	DrawEntities( m_view );
	//DrawPortals( m_view );

	// Draw GUI elements.
	GUI->drawAll();

	Driver->endScene();
}

//void mxRenderWorld::DrawPortals( const mxView& view )
//{
//	for( mxRenderEntity * pEntity = m_portals.Next();
//		pEntity != NULL;
//		pEntity = pEntity->ListNode.Next() )
//	{
//		pEntity->RegisterForRendering();
//		if ( pEntity->IsVisible( view ) )
//		{
//			pEntity->UpdateWorldTransform();
//			pEntity->Render( view );
//		}
//	}
//}

void mxRenderWorld::DrawEntities( const mxView& view )
{
	// Draw Irrlicht scene nodes.
	SceneManager->drawAll();

#ifdef MX_DEBUG
	GPhysicsWorld->DebugDrawWorld();
#endif

	for( mxRenderEntity * pEntity = m_renderEntities.Next();
		pEntity != NULL;
		pEntity = pEntity->ListNode.Next() )
	{
		pEntity->RegisterForRendering();
		if ( pEntity->IsVisible( view ) )
		{
			pEntity->UpdateWorldTransform();
			pEntity->Render( view );
		}
	}
}

void mxRenderWorld::UpdateView()
{
//	SceneManager->setActiveCamera( g_pPlayerCamera );
//	MemCopy( &(m_view.Frustum), g_pPlayerCamera->getViewFrustum(), sizeof( TViewFrustum ) );
}

void mxRenderWorld::Shutdown()
{
	SceneManager->setActiveCamera( NULL );

	// Delete all scene nodes.
	SceneManager->clear();

	//---------------------------------------------
	for ( u32 i=0; i < m_staticTexts.Num(); i++ ) {
		delete m_staticTexts[ i ];
	}
	m_staticTexts.Clear();
}

void mxRenderWorld::AddEntity( mxRenderEntity* entity )
{
	entity->ListNode.AddToEnd( m_renderEntities );
}

void mxRenderWorld::RemoveEntity( mxRenderEntity* entity )
{
	entity->ListNode.Remove();
}

//void mxRenderWorld::AddPortal( mxPortal* portal )
//{
//	RemoveEntity( portal );
//	portal->PortalListNode.AddToEnd( m_portals );
//}
//
//void mxRenderWorld::RemovePortal( mxPortal* portal )
//{
//	portal->PortalListNode.Remove();
//}

//mxOnScreenText * mxRenderWorld::CreateText( f32 x, f32 y, E_Colour color /* =E_Colour::Color_White */, E_Font font /* =E_Font::Font_Medium */ )
/*
{
	const s32  iX = s32(x * GetScreenWidth());
	const s32  iY = s32(y * GetScreenHeight());

	const s32  width = GetScreenWidth() - iX;
	const s32  height = 30;	//<= TODO: select height based on the font.

	::irr::core::rect< s32 >   rect( iX, iY, iX+width, iY+height );
	::irr::gui::IGUIStaticText * pStaticText = GUI->addStaticText(
		L"",
		rect,
		false,	// border
		false,	// word wrap
		NULL,	// parent
		-1,		// id
		true	// fill background
	);
	pStaticText->setTextAlignment( ::irr::gui::EGUIA_UPPERLEFT, irr::gui::EGUIA_CENTER );
	pStaticText->setOverrideColor( GRenderWorld->GetColor( color ) );

	mxOnScreenText * newText = new mxOnScreenText();
	m_staticTexts.Append( newText );

	newText->m_pStaticText = pStaticText;

	return newText;
}
*/
//mxOnScreenText * mxRenderWorld::CreateText( u16 x, u16 y, u16 width, u16 height, E_Colour color , E_Font font /* =E_Font::Font_Medium */ )
/*
{
	::irr::core::rect< s32 >   rect( x, y, x+width, y+height );
	::irr::gui::IGUIStaticText * pStaticText = GUI->addStaticText(
		L"",
		rect,
		false,	// border
		true,	// word wrap
		NULL,	// parent
		-1,		// id
		true	// fill background
	);
	pStaticText->setTextAlignment( ::irr::gui::EGUIA_UPPERLEFT, irr::gui::EGUIA_CENTER );
	pStaticText->setOverrideColor( GRenderWorld->GetColor( color ) );

	mxOnScreenText * newText = new mxOnScreenText();
	m_staticTexts.Append( newText );

	newText->m_pStaticText = pStaticText;

	return newText;
}

void mxRenderWorld::RemoveText( mxOnScreenText* pText )
{
	if ( ! pText ) {
		return;
	}
	pText->m_pStaticText->setVisible( false );
	pText->m_Wstring.Clear();
	// TODO: free memory allocated for pText
}
*/
::irr::video::SColor mxRenderWorld::GetColor( E_Colour color )
{
	switch ( color ) {
		case Color_White :		return ::irr::video::SColor( 255, 255, 255, 255 );
		case Color_Grey :		return ::irr::video::SColor( 255, 255, 255, 255 );
		case Color_Black :		return ::irr::video::SColor( 255, 0, 0, 0 );
		case Color_Blue :		return ::irr::video::SColor( 255, 0, 0, 255 );
		case Color_Green :		return ::irr::video::SColor( 255, 0, 255, 0 );
		case Color_Yellow :		return ::irr::video::SColor( 255, 250, 255, 155 );
		case Color_Red :		return ::irr::video::SColor( 255, 255, 0, 0 );
		default: Unreachable();
			return ::irr::video::SColor( 255, 255, 255, 255 );
	}
}
/*
//TODO: pre-cache all materials (textures, etc.)
void mxRenderWorld::CreatePermMark( const Vec3D& position, E_Colour color  )
{
	::irr::scene::ISceneNode * mark = SceneManager->addSphereSceneNode( 0.1f, 8, RootSceneNode, -1, position );
	mark->setMaterialFlag( ::irr::video::E_MATERIAL_FLAG::EMF_LIGHTING, false );// = E_Colour::Color_Black

	{
		const char * textureName = NULL;
		switch ( color )
		{
		case Color_White :
			textureName = PATH_TO_MEDIA "Images/Textures/Color/White.jpg";
			break;

		case Color_Grey :
			textureName = PATH_TO_MEDIA "Images/Textures/Color/Grey.jpg";
			break;

		case Color_Black :
			textureName = PATH_TO_MEDIA "Images/Textures/Color/Black.jpg";
			break;

		case Color_Blue :
			textureName = PATH_TO_MEDIA "Images/Textures/Color/Blue.jpg";
			break;

		case Color_Green :
			textureName = PATH_TO_MEDIA "Images/Textures/Color/Green.jpg";
			break;

		case Color_Yellow :
			textureName = PATH_TO_MEDIA "Images/Textures/Color/Yellow.jpg";
			break;

		case Color_Red :
			textureName = PATH_TO_MEDIA "Images/Textures/Color/Red.jpg";
			break;

		default:
			Unreachable();
		}

		mark->setMaterialTexture( 0, GRenderWorld->Driver->getTexture( textureName ) );
	}

	//mark->setMaterialFlag( ::irr::video::EMF_FOG_ENABLE, true );
}

void mxRenderWorld::CreateTempMark( const Vec3D& position, u32 timeMs ) //timeMs=10000
{
	Unimplemented();
}
*/
//================================
//	class mxSoundWorld
//================================


/*================================
	class mxPhysicsWorld
================================*/

mxPhysicsWorld::mxPhysicsWorld()

	: m_pDynamicsWorld			( NULL )
	, m_pCollisionConfiguration	( NULL )
	, m_pCollisionDispatcher	( NULL )
	, m_pConstraintSolver		( NULL )
	, m_pDebugDrawer			( NULL )

	, m_pMaterialSystem			( NULL )
{
	m_fInternalTimeStep = 1 / REAL( 3 * 60 ); //1 / REAL( 60 );
}

mxPhysicsWorld::~mxPhysicsWorld()
{}

//===================================================================================================

// Iterate over all manifolds and generate audio feedback for colliding objects.
//
static void mxInternalTickCallback( btDynamicsWorld *world, btScalar timeStep )
{
	btPersistentManifold ** ppManifold = world->getDispatcher()->getInternalManifoldPointer();
	for ( int i = 0; i < world->getDispatcher()->getNumManifolds(); i++ )
	{
		btPersistentManifold * pManifold = world->getDispatcher()->getManifoldByIndexInternal( i );

		const void* pBody0 = pManifold->getBody0();
		const void* pBody1 = pManifold->getBody1();

		const btCollisionObject * pObj0 = (btCollisionObject*) pBody0;
		const btCollisionObject * pObj1 = (btCollisionObject*) pBody1;

		void * pUserPtr0 = pObj0->getUserPointer();
		void * pUserPtr1 = pObj1->getUserPointer();

		mxEntity * pEntity0 = (mxEntity*) pUserPtr0;
		mxEntity * pEntity1 = (mxEntity*) pUserPtr1;

		//TODO: avoid extra checks for NULL pointers:
		if ( (pEntity0 && pEntity0->IsGamePlayer()) || (pEntity1 && pEntity1->IsGamePlayer()) ) {
			continue;
		}

		for ( int iPointIdx = 0; iPointIdx < pManifold->getNumContacts(); iPointIdx++ )
		{
//			btScalar  impulse = pManifold->getContactPoint( iPointIdx ).getAppliedImpulse();
			int  lifeTime = pManifold->getContactPoint( iPointIdx ).getLifeTime();

			const btVector3  position( pManifold->getContactPoint( iPointIdx ).getPositionWorldOnB() );
/*
			// Generate impact sound.
			if ( lifeTime < 2 && impulse > 50 )
			{
				ContactInfo_s   contactInfo;
				contactInfo.impulse = impulse;

				mxSoundSource * pSndSource = GPhysicsWorld->GetMaterialSystem().GetSoundForMaterials( NULL,NULL, contactInfo );//pEntity0->GetMaterial(), pEntity1->GetMaterial() );
				if ( pSndSource ) {
					Vec3D  pos;
					Assign( pos, position );

					GSoundWorld->Play3D( pSndSource, pos );
				}
			}
			*/
		}
	}
}

//===================================================================================================

void mxPhysicsWorld::Spawn()
{
	m_pCollisionConfiguration = new btDefaultCollisionConfiguration();
	m_pCollisionDispatcher = new btCollisionDispatcher( m_pCollisionConfiguration );
//	m_pCollisionDispatcher->setNearCallback( MyNearCallback );

	btVector3  worldAabbMin( -HALF_WORLD_EXTENT, -HALF_WORLD_EXTENT, -HALF_WORLD_EXTENT );
	btVector3  worldAabbMax( HALF_WORLD_EXTENT, HALF_WORLD_EXTENT, HALF_WORLD_EXTENT );
	int	 maxProxies = 1024;

	m_pBroadphaseInterface =
		new btAxisSweep3( worldAabbMin, worldAabbMax, maxProxies );
		//new bt32BitAxisSweep3( worldAabbMin, worldAabbMax, maxProxies );
		//new btDbvtBroadphase();

	m_pConstraintSolver = new btSequentialImpulseConstraintSolver();

	m_pDynamicsWorld =
	//		new btSimpleDynamicsWorld( m_pCollisionDispatcher, m_pBroadphaseInterface, m_pConstraintSolver, m_pCollisionConfiguration );
			new btDiscreteDynamicsWorld( m_pCollisionDispatcher, m_pBroadphaseInterface, m_pConstraintSolver, m_pCollisionConfiguration );

	m_pDynamicsWorld->setGravity( btVector3( 0, -9.80665f, 0 ) );

	m_pDynamicsWorld->setInternalTickCallback( mxInternalTickCallback );

	//-------------------------------------------------------------------
//	m_pDebugDrawer = new mxPhysicsDebugDrawer();
//	m_pDebugDrawer->setDebugMode( btIDebugDraw::DBG_DrawAabb | btIDebugDraw::DBG_DrawContactPoints );
//	m_pDynamicsWorld->setDebugDrawer( m_pDebugDrawer );


	//-------------------------------------------------------------------
	m_pMaterialSystem = new mxMaterialSystem();
}

void mxPhysicsWorld::Shutdown()
{
	delete m_pDynamicsWorld;
	delete m_pConstraintSolver;
	delete m_pCollisionConfiguration;
	delete m_pBroadphaseInterface;
	delete m_pCollisionDispatcher;
	delete m_pDebugDrawer;

	m_pDynamicsWorld			= NULL;
	m_pCollisionConfiguration	= NULL;
	m_pCollisionDispatcher		= NULL;
	m_pConstraintSolver			= NULL;
	m_pBroadphaseInterface		= NULL;
	m_pDebugDrawer				= NULL;

	// Collect garbage.
	m_collisionShapes.DeleteContents( false );
	m_motionStates.DeleteContents( false );

	for ( u32 i = 0; i < m_physicsEntities.Num(); i++ )	{
		DeletePhysicsEntity( m_physicsEntities[i] );
	}
	m_physicsEntities.Clear();

	m_triangleMeshes.DeleteContents( false );

	//-------------------------------------------------------
	delete m_pMaterialSystem;
	m_pMaterialSystem = NULL;
}

void mxPhysicsWorld::RunFrame( const REAL timeDelta )
{	STATIC_ASSERT( sizeof( timeDelta ) == sizeof( btScalar ) );
	m_pDynamicsWorld->stepSimulation( timeDelta, 3, m_fInternalTimeStep );
}

mxPhysicsEntity * mxPhysicsWorld::GetSpherePhysicsEntity( f32 inRadius, f32 inMass, const Vec3D& position )
{
	// TODO: if the same shape exists then return that shape.
	btCollisionShape * shape = new btSphereShape( inRadius );
	m_collisionShapes.Append( shape );

	btVector3  localInertia( 0,0,0 );
	shape->calculateLocalInertia( inMass, localInertia );

	btTransform   transform;
	transform.setIdentity();
	transform.setOrigin( btVector3( position.X, position.Y, position.Z ) );
	btDefaultMotionState * pMotionState = new btDefaultMotionState( transform );
	m_motionStates.Append( pMotionState );
	btRigidBody::btRigidBodyConstructionInfo   cInfo( inMass, pMotionState, shape, localInertia );

	CBulletPhysicsEntity * newPhysEntity = new CBulletPhysicsEntity();

	newPhysEntity->m_pRigidBody = new btRigidBody( cInfo );
	newPhysEntity->m_pRigidBody->setDamping( 0.01f, 0.01f );
	newPhysEntity->m_pRigidBody->setFriction( 1.0f );

	m_pDynamicsWorld->addRigidBody( newPhysEntity->m_pRigidBody );

	return newPhysEntity;
}

mxPhysicsEntity * mxPhysicsWorld::GetBoxPhysicsEntity( const Vec3D& dimensions, f32 inMass, const Vec3D& position )
{
	// TODO: if the same shape exists then return that shape.
	const btVector3  halfExtents( btScalar(dimensions.X/2), btScalar(dimensions.Y/2), btScalar(dimensions.Z/2) );

	btCollisionShape * shape = new btBoxShape( halfExtents );
	m_collisionShapes.Append( shape );

	btVector3  localInertia( 0,0,0 );
	shape->calculateLocalInertia( inMass, localInertia );

	btTransform   transform;
	transform.setIdentity();
	transform.setOrigin( btVector3( position.X, position.Y, position.Z ) );
	btDefaultMotionState * pMotionState = new btDefaultMotionState( transform );
	m_motionStates.Append( pMotionState );
	btRigidBody::btRigidBodyConstructionInfo   cInfo( inMass, pMotionState, shape, localInertia );

	CBulletPhysicsEntity * newPhysEntity = new CBulletPhysicsEntity();

	newPhysEntity->m_pRigidBody = new btRigidBody( cInfo );
	newPhysEntity->m_pRigidBody->setDamping( 0.1f, 0.1f );
	newPhysEntity->m_pRigidBody->setFriction( 1.0f );

	m_pDynamicsWorld->addRigidBody( newPhysEntity->m_pRigidBody );

	return newPhysEntity;
}

mxPhysicsEntity * mxPhysicsWorld::GetStaticMeshPhysicsEntity( const ::irr::scene::IMesh* pMesh, const Vec3D& position /* = Vec3D(0,0,0) */ )
{
	btTriangleMesh * pPhysTriMesh = new btTriangleMesh();
	ConvertIrrlichtMeshIntoBulletMesh( pMesh, pPhysTriMesh );

	btCollisionShape * pMeshShape = new btBvhTriangleMeshShape( pPhysTriMesh, true /*useQuantizedAabbCompression*/, true /*buildTree*/ );
	m_collisionShapes.Append( pMeshShape );

	btTransform   staticMeshTransform;
	staticMeshTransform.setIdentity();
	staticMeshTransform.setOrigin( btVector3( position.X, position.Y, position.Z ) );

	btScalar   mass( 0. );
	btVector3  localInertia( 0,0,0 );

	btDefaultMotionState * motionState = new btDefaultMotionState( staticMeshTransform );
	m_motionStates.Append( motionState );
	btRigidBody::btRigidBodyConstructionInfo  rbInfo( mass, motionState, pMeshShape, localInertia );

	CBulletPhysicsEntity * newPhysEntity = new CBulletPhysicsEntity();
	newPhysEntity->m_pRigidBody = new btRigidBody( rbInfo );
	m_pDynamicsWorld->addRigidBody( newPhysEntity->m_pRigidBody );

	return newPhysEntity;
}

// For testing & debugging.
mxPhysicsEntity * mxPhysicsWorld::GetStaticTriangleMeshPhysicsEntity( const mxRenderMesh& rMesh )
{
#if 0
	btTriangleMesh * pPhysTriMesh = new btTriangleMesh();
	m_triangleMeshes.Append( pPhysTriMesh );

	ConvertMeshIntoPhysicsShape( rMesh, pPhysTriMesh );

	//btCollisionShape * pMeshShape = new mxTriangleMeshShape( pPhysTriMesh );
	btCollisionShape * pMeshShape = new mxStaticTriangleMeshCollisionShape( rMesh );
#else
	btTriangleMesh * pPhysTriMesh = new btTriangleMesh();
	m_triangleMeshes.Append( pPhysTriMesh );

	ConvertMeshIntoPhysicsShape( rMesh, pPhysTriMesh );

	btCollisionShape * pMeshShape = new btBvhTriangleMeshShape( pPhysTriMesh, true /*useQuantizedAabbCompression*/, true /*buildTree*/ );
#endif
	m_collisionShapes.Append( pMeshShape );

	btTransform   staticMeshTransform;
	staticMeshTransform.setIdentity();

	btScalar   mass( 0. );
	btVector3  localInertia( 0,0,0 );

	btDefaultMotionState * motionState = new btDefaultMotionState( staticMeshTransform );
	m_motionStates.Append( motionState );
	btRigidBody::btRigidBodyConstructionInfo  rbInfo( mass, motionState, pMeshShape, localInertia );

	CBulletPhysicsEntity * newPhysEntity = new CBulletPhysicsEntity();
	newPhysEntity->m_pRigidBody = new btRigidBody( rbInfo );
	m_pDynamicsWorld->addRigidBody( newPhysEntity->m_pRigidBody );

	return newPhysEntity;
}

mxPhysicsEntity * mxPhysicsWorld::GetStaticPlanePhysicsEntity( const TPlane3D& plane )
{
	// Static plane
	btCollisionShape * pShape = new btStaticPlaneShape( btVector3(plane.Normal.X,plane.Normal.Y,plane.Normal.Z), btScalar(plane.D) );
	m_collisionShapes.Append( pShape );

	btTransform   groundTransform;
	groundTransform.setIdentity();
	groundTransform.setOrigin( btVector3(0,0,0) );

	btScalar   mass(0.);
	btVector3  localInertia(0,0,0);

	btDefaultMotionState * pMotionState = new btDefaultMotionState( groundTransform );
	m_motionStates.Append( pMotionState );
	btRigidBody::btRigidBodyConstructionInfo  rbInfo( mass, pMotionState, pShape, localInertia );

	CBulletPhysicsEntity * newPhysEntity = new CBulletPhysicsEntity();
	newPhysEntity->m_pRigidBody = new btRigidBody( rbInfo );

	newPhysEntity->m_pRigidBody->setFriction( 1.0f );

	m_pDynamicsWorld->addRigidBody( newPhysEntity->m_pRigidBody );

	return newPhysEntity;
}

mxPhysicsEntity * mxPhysicsWorld::CreatePlayerPhysics( const Vec3D& initialPosition, f32 mass /* = 90.0f */ )
{
	// Create a capsule representing the character standing.
	btCollisionShape * shape = new btCapsuleShape( 50.f /* radius */, 100.f /* height */ );
	m_collisionShapes.Append( shape );	//TODO: <= reuse collision shapes

	btVector3  localInertia( 0,0,0 );
	//shape->calculateLocalInertia( mass, localInertia );

	btTransform   transform;
	transform.setIdentity();
	transform.setOrigin( btVector3( initialPosition.X, initialPosition.Y, initialPosition.Z ) );

	btDefaultMotionState * motionState = new btDefaultMotionState( transform );
	m_motionStates.Append( motionState );
	btRigidBody::btRigidBodyConstructionInfo   cInfo( mass, motionState, shape, localInertia );

	CPlayerPhysicsEntity * playerPhysEntity = new CPlayerPhysicsEntity();
	playerPhysEntity->m_pRigidBody = new btRigidBody( cInfo );
	playerPhysEntity->m_pRigidBody->setDamping( 0.0f, 1e10 );

	playerPhysEntity->m_pRigidBody->setSleepingThresholds( 0, 0 );
	playerPhysEntity->m_pRigidBody->setAngularFactor( 0.0f );	// player capsule should remain standing on the up axis

	playerPhysEntity->m_pRigidBody->setRestitution( 0.f );
	playerPhysEntity->m_pRigidBody->setFriction( 0.f );

	m_pDynamicsWorld->addRigidBody( playerPhysEntity->m_pRigidBody );

	return playerPhysEntity;
}

//UNDONE:
void mxPhysicsWorld::TracePhysicsEntity( const Vec3D& vFrom, const Vec3D& vTo, const mxPhysicsEntity* pPhysEntity, Vec3D &OutNewPos )
{
	const CBulletPhysicsEntity * pEntity = static_cast< const CBulletPhysicsEntity* >( pPhysEntity );
	ClosestNotMeRayResultCallback   cb( pEntity->m_pRigidBody );
	Assign( cb.m_rayFromWorld, vFrom );
	Assign( cb.m_rayToWorld, vTo );

	m_pDynamicsWorld->rayTest( btVector3( vFrom.X, vFrom.Y, vFrom.Z ), btVector3( vTo.X, vTo.Y, vTo.Z ), cb );

	if ( cb.hasHit() ) {
		Assign( OutNewPos, cb.m_hitPointWorld );
		//printf( "\a" );
	}
}

//void mxPhysicsWorld::Acquire( mxPhysicsEntity* physEnt )
//{
//	m_physicsEntities.Append( physEnt );
//}

void mxPhysicsWorld::DeletePhysicsEntity( mxPhysicsEntity* physEnt )
{
	CBulletPhysicsEntity * pPhysicsEntity = static_cast< CBulletPhysicsEntity* >( physEnt );
	m_pDynamicsWorld->removeCollisionObject( pPhysicsEntity->m_pRigidBody );
	delete pPhysicsEntity->m_pRigidBody;
	delete physEnt;
}

void mxPhysicsWorld::DebugDrawWorld()
{
}

/*================================
	class mxWorld
================================*/

mxWorld::mxWorld()
{
	PreInit();
}

mxWorld::~mxWorld()
{}

void mxWorld::PreInit()
{
	m_bInitialized = false;
	m_pIrrlichtDevice = NULL;
	Environment = NULL;

	m_uFrameNumber = 0;
	m_uLastFrameTime = m_uCurrentTime = 0;
	m_bPaused = false;
}

void mxWorld::Spawn()
{
	if ( m_bInitialized ) {
		return;
	}

	//---- Initialize common stuff -----------------------
	#include "CCommon.inl"

	GCommon = new CCommon();

	//----------------------------------------------------
	// Init. settings.
#if 1
	m_renderWorld.m_eDriverType = ::irr::video::EDT_DIRECT3D9,
	m_renderWorld.m_uScreenWidth = 640;
	m_renderWorld.m_uScreenHeight = 480;
	m_renderWorld.m_bFullScreen = false;
#else
	m_renderWorld.m_eDriverType = ::irr::video::EDT_DIRECT3D9,
	m_renderWorld.m_uScreenWidth = 1024;
	m_renderWorld.m_uScreenHeight = 768;
	m_renderWorld.m_bFullScreen = true;
#endif

	//----------------------------------------------------
	// Create Irrlicht device.
	//
	m_pIrrlichtDevice = createDevice(
		irr::video::EDT_OPENGL,
		::irr::core::dimension2d< s32 >( m_renderWorld.GetScreenWidth(), m_renderWorld.GetScreenHeight() ),
		32,								// bits
		m_renderWorld.IsFullScreen(),	// fullscreen ?

		true,	// stencil buffer
		false,	// vsync
		this	// Irrlicht event receiver
	);
	if ( ! m_pIrrlichtDevice ) {
		GCommon->FatalError( "failed to create Irrlicht device" );
		return;
	}

	//----------------------------------------------------
	// Initialize system world.
	GSystem = & m_systemWorld;

	GSystem->Logger			= m_pIrrlichtDevice->getLogger();
	GSystem->FileSystem		= m_pIrrlichtDevice->getFileSystem();
	GSystem->CursorControl	= m_pIrrlichtDevice->getCursorControl();
	GSystem->m_timer		= m_pIrrlichtDevice->getTimer();

	GSystem->Spawn();

	// Hide mouse cursor.
	GSystem->SetMouseCursorVisible( false );

	//----------------------------------------------------
	// Create render world.
	//
	GRenderWorld = & m_renderWorld;

	m_renderWorld.Driver		= m_pIrrlichtDevice->getVideoDriver();
	m_renderWorld.SceneManager	= m_pIrrlichtDevice->getSceneManager();
	m_renderWorld.RootSceneNode	= m_pIrrlichtDevice->getSceneManager()->getRootSceneNode();
	m_renderWorld.GUI			= m_pIrrlichtDevice->getGUIEnvironment();

	m_renderWorld.Spawn();

	//----------------------------------------------------
	// Create sound world.
	//


	//----------------------------------------------------
	// Create physical world.
	//
	GPhysicsWorld = & m_physicsWorld;
	m_physicsWorld.Spawn();

	//----------------------------------------------------
	// Init debug info.
	//
#pragma region Debug Info

#ifdef MX_DEBUG

	m_debugData.statsText		= GRenderWorld->CreateText( 0.1f, 0.1f, E_Colour::Color_White );
	m_debugData.playerPosText	= GRenderWorld->CreateText( 0.3f, 0.7f, E_Colour::Color_Yellow );
	m_debugData.numObjectsText	= GRenderWorld->CreateText( 0.7f, 0.1f, E_Colour::Color_Yellow );

	// Add 'crosshair'.
	mxOnScreenText * pCrossHair = GRenderWorld->CreateText(
		GRenderWorld->GetScreenWidth() / 2, GRenderWorld->GetScreenHeight() / 2, 50, 30,
		E_Colour::Color_Green, E_Font::Font_Large );
	pCrossHair->SetText( "X" );



        m_renderWorld.SceneManager->addTextSceneNode(
            m_renderWorld.GUI->getBuiltInFont(),
            L"X = 0, Y = 0, Z = 0",
			::irr::video::SColor(255,255,255,255),
            0,
            Vec3D( 0, 0, 0 ) );

        m_renderWorld.SceneManager->addTextSceneNode(
            m_renderWorld.GUI->getBuiltInFont(),
            L"X = 1000, Y = 0, Z = 0",
            ::irr::video::SColor(255,255,255,255),
            0,
            Vec3D( 1000, 0, 0 ) );

        m_renderWorld.SceneManager->addTextSceneNode(
            m_renderWorld.GUI->getBuiltInFont(),
            L"X = 0, Y = 100, Z = 0",
            ::irr::video::SColor(255,255,255,255),
            0,
            Vec3D( 0, 100, 0 ) );

		m_renderWorld.SceneManager->addTextSceneNode(
            m_renderWorld.GUI->getBuiltInFont(),
            L"X = 0, Y = 0, Z = 1000",
            ::irr::video::SColor(255,255,255,255),
            0,
            Vec3D( 0, 0, 1000 ) );


		//------------------------------------------------------------------------------
#if 0	// Testing and debugging.
		#define PATH_TO_IRR_MEDIA	"./Media"

		m_pIrrlichtDevice->getFileSystem()->addZipFileArchive( PATH_TO_IRR_MEDIA "map-20kdm2.pk3" );

		::irr::scene::IAnimatedMesh* mesh = GRenderWorld->SceneManager->getMesh( PATH_TO_IRR_MEDIA "20kdm2.bsp" );
		if ( mesh )
		{
			::irr::scene::ISceneNode* node = GRenderWorld->SceneManager->addOctTreeSceneNode( mesh->getMesh(0) );
			if ( node )
			{
			//	node->setPosition( ::irr::core::vector3df(-1300,-144,-1249));
			//	node->setScale( ::irr::core::vector3df(2,2,2));
				node->setPosition( ::irr::core::vector3df(-200,-20,-200));
				node->setScale( ::irr::core::vector3df( 0.2f, 0.2f, 0.2f ) );
			}
		}

		#undef PATH_TO_IRR_MEDIA
#endif

#endif // MX_DEBUG

#pragma endregion

	//----------------------------------------------------

	LoadDemoLevel();
}

//---------------------------------------------------------------------------------------------------------------------------

// Main game loop.
//
void mxWorld::Run()
{
	if ( ! Environment ) {
		GCommon->DebugPrint( "No level loaded!\n" );
		return;
	}
	if ( ! GetLocalPlayer() ) {
		GetConsole().Print( "No player!\n" );
		return;
	}

	// Time accumulator to ensure constant stepping at different frame times.
	REAL  fTimeAcculmulator = 0;

	const REAL MAX_TIMESTEP = 0.2f;	 // Maximum time interval between physics updates.

	// Set this to zero to run at maximum frame rate.
	const REAL MIN_STEP_SIZE = 1 / REAL(60); // Minimum time before an update loop.

	// Win32 game loop with fixed frame rate.

	while ( m_pIrrlichtDevice->run() )
	{
        if ( m_pIrrlichtDevice->isWindowActive() )
		{
			m_uCurrentTime = GSystem->GetRealTime();
			const u32  timeDiff = m_uCurrentTime - m_uLastFrameTime;
			m_uLastFrameTime = m_uCurrentTime;

			const REAL  fDelta = timeDiff / 1000.f;

			fTimeAcculmulator += fDelta;
			if ( fTimeAcculmulator > MAX_TIMESTEP ) {
				fTimeAcculmulator = MAX_TIMESTEP;
			}

			// TODO: re-arrange scene graph on the fly for better performance,
			// monitor frame rate and statistics, evaluate 'stress' factor .
			// Use that info to weight the level of detail in the scene eliminating detail to maintain the target frame rate.
			if ( fTimeAcculmulator >= MIN_STEP_SIZE )
			{
				RunFrame( fTimeAcculmulator );
				fTimeAcculmulator = 0;
			}

			m_pIrrlichtDevice->yield();

		}// if render window is active

		//***********
		DEBUG_CODE( Sleep( 10 ); );
	}
}

void mxWorld::Shutdown()
{
#ifdef MX_DEBUG
	// Destroy debug data.
	GRenderWorld->RemoveText( m_debugData.statsText );
#endif

	//---------------------------------------------
	// Destroy the game level.
	if ( Environment ) {
		Environment->Shutdown();
		delete Environment;
		Environment = NULL;
	}

	//---------------------------------------------
	// Destroy physics world.
	GPhysicsWorld->Shutdown();
	GPhysicsWorld = NULL;

	//---------------------------------------------
	// Destroy render world.
	GRenderWorld->Shutdown();
	GRenderWorld = NULL;


	GRenderWorld = NULL;

	if ( m_pIrrlichtDevice ) {
		m_pIrrlichtDevice->drop();
		m_pIrrlichtDevice = NULL;
	}

	//---------------------------------------------
	// Destroy system world.
	GSystem->Shutdown();
	GSystem = NULL;

	//---------------------------------------------
	delete GCommon;
	GCommon = NULL;

	//---------------------------------------------
	PreInit();
}

bool mxWorld::OnEvent( const ::irr::SEvent & event )
{
	using namespace ::irr;

	//
	//	Keyboard input event
	//
	if ( EET_KEY_INPUT_EVENT == event.EventType
		&& ! event.KeyInput.PressedDown )
	{
		switch ( event.KeyInput.Key )
		{
		// ESC key was pressed - user wants to quit.
		case KEY_ESCAPE :
		case KEY_KEY_Q :
			{
				m_pIrrlichtDevice->closeDevice();
				return true;
			}

		// Pause physics simulation.
		case KEY_PAUSE :
			m_bPaused ^= 1;
			//SetPaused( m_bPaused );
            break;

			// TODO:

	//	case KEY_KEY_O :
//			for ( u32 i = 0; i < m_keyActions[ KEY_KEY_O ].Num(); i++ )
		//	{
		//		m_keyActions[ KEY_KEY_O ][i]->Execute();
		//	}
		//	break;

		}//switch
	}//end case of a keyboard event

	return false;
}

void mxWorld::RunFrame( const REAL fDeltaTime )
{
	// Let the world think and simulate.
	Environment->Tick( fDeltaTime );

	// Run physics.
	if ( ! m_bPaused )
	{
		GPhysicsWorld->RunFrame( fDeltaTime );
	}

	// Draw the world.
	GRenderWorld->RenderFrame();

	RunDebugInfo();
}

void mxWorld::LoadDemoLevel()
{
	// Create our environment.
	Environment = new TestLevel();
	Environment->Spawn();
}

void mxWorld::RunDebugInfo()
{
#ifdef MX_DEBUG
	m_debugData.statsText->SetText( "FPS: %u, triangles: %u",
		GRenderWorld->GetFPS(), GRenderWorld->Driver->getPrimitiveCountDrawn() );

	{
		const Vec3D & pos = GetLocalPlayer()->GetOrigin();
		m_debugData.playerPosText->SetText( "Player pos: ( %f, %f, %f )", pos.X, pos.Y, pos.Z );
	}

	m_debugData.numObjectsText->SetText( "Num. Entities: %i", GPhysicsWorld->m_pDynamicsWorld->getNumCollisionObjects() );
#endif // MX_DEBUG
}

mxGamePlayer* mxWorld::SpawnHumanPlayer( const Vec3D& initialPosition )
{
	if ( m_localPlayer ) {
		GCommon->Warning( "Player has already been spawned." );
		return NULL;
	}

	// Create the player.
	mxGamePlayer * player = new mxGamePlayer();
	player->Spawn();

	player->SetOrigin( initialPosition );

	m_localPlayer = player;

	return player;
}

}//end of namespace mix

//--------------------------------------------------------------//
//				End Of File.									//
//--------------------------------------------------------------//
