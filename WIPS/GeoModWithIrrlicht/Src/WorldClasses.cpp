/*
===============================================================================
	File:	WorldClasses.cpp
	Desc:
===============================================================================
*/

#include "stdafx.h"
#pragma hdrstop
#include <Include/IrrMix.h>

namespace mix {

/*================================
	class mxClass
================================*/

mxClass::~mxClass()
{}

void* mxClass::operator new( size_t  num )
{
	return ::mix::GSystem->Allocate( num );
}

void mxClass::operator delete( void* mem )
{
	return ::mix::GSystem->Free( mem );
}

/*================================
	class mxMaterialSystem
================================*/

mxMaterialSystem::mxMaterialSystem()
{

}

mxMaterialSystem::~mxMaterialSystem()
{}



/*================================
	class mxEntity
================================*/

mxEntity::mxEntity()
	: m_physics( NULL )
	, m_material( NULL )
{}

mxEntity::~mxEntity()
{
	if ( m_physics ) {
		GPhysicsWorld->DeletePhysicsEntity( m_physics );
	}
}

void mxEntity::SetOrigin( const Vec3D& newOrigin )
{
	m_physics->SetOrigin( newOrigin );
}

/*================================
	class mxRenderEntity
================================*/

mxRenderEntity::mxRenderEntity()
	: ListNode( this )
{
	GRenderWorld->AddEntity( this );
}

mxRenderEntity::~mxRenderEntity()
{
	GRenderWorld->RemoveEntity( this );
}

void mxRenderEntity::ApplyDecal( const Vec3D& position, const Vec3D& normal )
{
	(void) position;
	(void) normal;
//	GRenderWorld->CreatePermMark( position, E_Colour::Color_White );
}

void mxRenderEntity::SetPosition( const Vec3D& newPosition ) {
	m_vTranslation = newPosition;
}

void mxRenderEntity::SetRotation( const Vec3D& rotationInDegress ) {
	m_vRotation = rotationInDegress;
}

const Vec3D & mxRenderEntity::GetPosition() const {
	return m_vTranslation;
}

const Vec3D & mxRenderEntity::GetRotation() const {
	return m_vRotation;
}

void mxRenderEntity::UpdateWorldTransform()
{
	m_worldTransform.setTranslation( m_vTranslation );
	m_worldTransform.setRotationDegrees( m_vRotation );
}

const TMatrix4 & mxRenderEntity::GetWorldTransform() const {
	return m_worldTransform;
}

/*================================
	class mxSpectator
================================*/

mxSpectator::mxSpectator()
{
	using namespace ::irr;
	using namespace ::irr::scene;
	using namespace ::irr::video;

	m_pCamera = GRenderWorld->SceneManager->addCameraSceneNodeFPS(
		NULL,
		300, // rotate speed
		0.04f // move speed
	);
	m_pCamera->setNearValue( 0.05f );
	m_pCamera->setFarValue( 100000.0f );


#if 0 // Create light ( for debugging ).
	ILightSceneNode * pLight = GRenderWorld->SceneManager->addLightSceneNode( m_pCamera );
	pLight->setLightType(
		//E_LIGHT_TYPE::ELT_POINT
		E_LIGHT_TYPE::ELT_SPOT
		//E_LIGHT_TYPE::ELT_DIRECTIONAL
	);
	pLight->setRadius( 800.0f );
	//pLight->getLightData().OuterCone /= 1.5;

	// TODO: make "Debug_PrintVector( const Vec3D& vec );
	//GCommon->DebugPrint( "attenuation: %f, %f, %f", pLight->getLightData().Attenuation.X, pLight->getLightData().Attenuation.Y, pLight->getLightData().Attenuation.Z );
	//pLight->getLightData().Attenuation = Vec3D( 0.000001f, 0.000001f, 0.000001f );

	//pLight->enableCastShadow();
#endif
}

mxSpectator::~mxSpectator()
{
}

void mxSpectator::Spawn()
{
}

/*================================
	class CSceneNodeEntity
================================*/

CSceneNodeEntity::CSceneNodeEntity()
	: m_pSceneNode( NULL )
{}

CSceneNodeEntity::~CSceneNodeEntity()
{
}

void CSceneNodeEntity::RunPhysics()
{
	if ( m_physics )
	{
		m_pSceneNode->setPosition( m_physics->GetOrigin() );
		m_pSceneNode->setRotation( m_physics->GetRotationInDegrees() );
	}
}

void CSceneNodeEntity::SetOrigin( const Vec3D& newOrigin )
{
	m_pSceneNode->setPosition( newOrigin );
	m_physics->SetOrigin( newOrigin );
}

/*================================
	class CBoxEntity
================================*/

CBoxEntity::CBoxEntity( const Vec3D& dimensions, const Vec3D& position )
{
	// Graphics.
	m_pSceneNode = new ::irr::scene::CBox_SceneNode( dimensions.X, dimensions.Y, dimensions.Z );
	m_pSceneNode->drop();
	//m_pSceneNode->setMaterialFlag( video::EMF_LIGHTING, false );
	m_pSceneNode->setMaterialTexture( 0, GRenderWorld->Driver->getTexture( PATH_TO_MEDIA "Models/Test/PANS.jpg" ) );

	// Physics.
	const f32   mass( 100 );
	m_physics = GPhysicsWorld->GetBoxPhysicsEntity( dimensions, mass, position );
	m_physics->SetEntityPtr( this );
}

CBoxEntity::~CBoxEntity()
{}

/*================================
	class Test_Sphere
================================*/

Test_Sphere::Test_Sphere( f32 radius, const Vec3D& position )
{
	// Graphics.
	m_pSceneNode = GRenderWorld->SceneManager->addSphereSceneNode( radius, 16, NULL, -1, position );
	m_pSceneNode->setMaterialFlag( ::irr::video::EMF_LIGHTING, false );
	m_pSceneNode->setMaterialTexture( 0, GRenderWorld->Driver->getTexture( PATH_TO_MEDIA "Models/Test/PANS.jpg" ) );

	// Physics.
	const f32   mass( 100 );
	m_physics = GPhysicsWorld->GetSpherePhysicsEntity( radius, mass, position );
	m_physics->SetEntityPtr( this );
}

Test_Sphere::~Test_Sphere()
{}

/*================================
	class CLargeStaticPlaneEntity
================================*/

CLargeStaticPlaneEntity::CLargeStaticPlaneEntity( const TPlane3D& plane /* =TPlane3D( Vec3D( 0,1,0 ), 0 ) */ )
{
#if 1 // Create Irrlicht terrain scene node.
	const f32  fExtent( 10.0f );
	const Vec3D  vScale( fExtent, fExtent, fExtent );
	const Vec3D  vPosition( 0, 0, 0 );
	const Vec3D  vRotation( 0, 0, 0 );

	// Graphics.
	::irr::scene::ITerrainSceneNode * m_pSceneNode = GRenderWorld->SceneManager->addTerrainSceneNode(
		PATH_TO_MEDIA "Images/Textures/Terrain/plane-terrain-heightmap.bmp",
		NULL,											// parent node
		-1,												// node id
		vPosition,										// position
		vRotation,										// rotation
		vScale,											// scale
		::irr::video::SColor ( 255, 255, 255, 255 ),	// vertexColor,
		2,												// maxLOD
		::irr::scene::ETPS_9,							// patchSize
		0												// smoothFactor
	);
	Assert2( m_pSceneNode, "Failed to load terrain heightmap!" );

	const ::irr::core::aabbox3df & rAABB = m_pSceneNode->getBoundingBox();
	m_pSceneNode->setPosition( Vec3D(
		(-rAABB.MaxEdge.X + rAABB.MinEdge.X)/2, 0, (-rAABB.MaxEdge.Z + rAABB.MinEdge.Z)/2
		) );

	//m_pSceneNode->setMaterialFlag( ::irr::video::EMF_LIGHTING, false );
	m_pSceneNode->setMaterialType( ::irr::video::EMT_DETAIL_MAP );

	m_pSceneNode->setMaterialTexture( 0, GRenderWorld->Driver->getTexture( PATH_TO_MEDIA "Images/Textures/Terrain/blue-terrain-texture.jpg" ) );
	m_pSceneNode->setMaterialTexture( 1, GRenderWorld->Driver->getTexture( PATH_TO_MEDIA "Images/Textures/Terrain/terrain-detailmap.jpg" ) );

	m_pSceneNode->scaleTexture( 90.0f, 90.0f );
#else


#endif
	// Physics.
	m_physics = GPhysicsWorld->GetStaticPlanePhysicsEntity( plane );
	m_physics->SetEntityPtr( this );
}

CLargeStaticPlaneEntity::~CLargeStaticPlaneEntity()
{}

}//end of namespace mix

//--------------------------------------------------------------//
//				End Of File.									//
//--------------------------------------------------------------//
