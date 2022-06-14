/*
===============================================================================
	File:	Physics.cpp
	Desc:
===============================================================================
*/

#include "stdafx.h"
#pragma hdrstop
#include <Include/IrrMix.h>

namespace mix {

/*================================
	CBulletPhysicsEntity
================================*/

CBulletPhysicsEntity::CBulletPhysicsEntity()
	: m_pRigidBody( NULL )
{
}

CBulletPhysicsEntity::~CBulletPhysicsEntity()
{}

void CBulletPhysicsEntity::SetOrigin( const Vec3D& newOrigin )
{
	btTransform  transform;
	m_pRigidBody->getMotionState()->getWorldTransform( transform );
	transform.setOrigin( btVector3( newOrigin.X, newOrigin.Y, newOrigin.Z ) );
	m_pRigidBody->setWorldTransform( transform );
}

void CBulletPhysicsEntity::SetLinearVelocity( const Vec3D& velocity )
{
	m_pRigidBody->setLinearVelocity( btVector3( velocity.X, velocity.Y, velocity.Z ) );
}

void CBulletPhysicsEntity::SetAngularVelocity( const Vec3D& newAngularVelocity )
{
	m_pRigidBody->setAngularVelocity( btVector3( newAngularVelocity.X, newAngularVelocity.Y, newAngularVelocity.Z ) );
}

const Vec3D CBulletPhysicsEntity::GetOrigin() const
{
	const btTransform  worldTrans( m_pRigidBody->getWorldTransform() );
	const btVector3 & orig = worldTrans.getOrigin();
	return Vec3D( orig.x(), orig.y(), orig.z() );
}

const Vec3D CBulletPhysicsEntity::GetRotationInDegrees() const
{
	const btMatrix3x3 & rotation( m_pRigidBody->getWorldTransform().getBasis() );

	btScalar  yaw, pitch, roll;
//	rotation.getEulerYPR( yaw, pitch, roll );
//	rotation.getEulerZYX( yaw, pitch, roll );
	rotation.getEulerYPR( yaw, pitch, roll );

	yaw = RAD2DEG( yaw );
	pitch = RAD2DEG( pitch );
	roll = RAD2DEG( roll );

	return Vec3D( roll, pitch, yaw );
}

void CBulletPhysicsEntity::SetEntityPtr( mxEntity* entity )
{
	m_pRigidBody->setUserPointer( entity );
}

//===========================================================================================================

void ConvertIrrlichtMeshIntoBulletMesh( const ::irr::scene::IMesh* pMesh, btTriangleMesh *OutPhysTriMesh )
{
	DEBUG_BREAK;
	//Unimplemented();
}

//TODO: make it more efficient.
void ConvertMeshIntoPhysicsShape( const mxRenderMesh& rMesh, btTriangleMesh *OutPhysTriMesh )
{	BREAK_IF( ! OutPhysTriMesh );

	OutPhysTriMesh->preallocateVertices( rMesh.GetVertexCount() );
	OutPhysTriMesh->preallocateIndices( rMesh.GetTriangleCount() * 3 );

	for ( u32 triIdx = 0; triIdx < rMesh.GetTriangleCount(); triIdx++ )
	{
		const IndexTriple &  rTri = rMesh.Triangles[ triIdx ];

		OutPhysTriMesh->addTriangle(
			Convert( rMesh.GetPos( rTri.iA ) ),
			Convert( rMesh.GetPos( rTri.iB ) ),
			Convert( rMesh.GetPos( rTri.iC ) )
		);
	}
}

//===========================================================================================================

/*================================
	CPlayerPhysicsEntity
================================*/

CPlayerPhysicsEntity::CPlayerPhysicsEntity()
{
}

CPlayerPhysicsEntity::~CPlayerPhysicsEntity()
{}

}//end of namespace mix

//--------------------------------------------------------------//
//				End Of File.									//
//--------------------------------------------------------------//
