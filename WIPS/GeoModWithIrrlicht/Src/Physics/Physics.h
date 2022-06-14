/*
===============================================================================
	File:	Physics.h
	Desc:
===============================================================================
*/

#ifndef __MX_PHYSICS_H__
#define __MX_PHYSICS_H__

namespace mix {

// Forward declarations.
class mxEntity;
class mxGeomoddableSolid;
class mxDestructionManager;

//
//	CBulletPhysicsEntity
//
// TODO: rename into CPhysicsEntity
//
class CBulletPhysicsEntity : public mxPhysicsEntity {
public:
			CBulletPhysicsEntity();
			~CBulletPhysicsEntity();

	// overrides ( mxPhysicsEntity )
	void	SetOrigin( const Vec3D& newOrigin );

	void	SetLinearVelocity( const Vec3D& velocity );
	void	SetAngularVelocity( const Vec3D& newAngularVelocity );

	const Vec3D		GetOrigin() const;
	const Vec3D		GetRotationInDegrees() const;

	void	SetEntityPtr( mxEntity* entity );
	btRigidBody *		m_pRigidBody;
protected:
//	friend mxPhysicsWorld;


};

//
//	CNullPhysicsEntity
//
class CNullPhysicsEntity : public mxPhysicsEntity {
public:
	CNullPhysicsEntity() {}
	~CNullPhysicsEntity() {}

	// overrides ( mxPhysicsEntity )
	void	SetOrigin( const Vec3D& newOrigin )	{ m_position = newOrigin; }
	const Vec3D GetOrigin() const { return m_position; }

	void	SetLinearVelocity( const Vec3D& velocity ) {}
	void	SetAngularVelocity( const Vec3D& newAngularVelocity ) {}

	const Vec3D		GetRotationInDegrees() const { return m_rotation; }

private:
	Vec3D	m_position;
	Vec3D	m_rotation;	// in degrees
};

//===========================================================================================================

inline void Assign( Vec3D& vec, const btVector3& other )
{
	vec.X = other.getX();
	vec.Y = other.getY();
	vec.Z = other.getZ();
}

inline void Assign( btVector3& vec, const Vec3D& other )
{
	vec.setX( other.X );
	vec.setY( other.Y );
	vec.setZ( other.Z );
}

inline Vec3D Convert( const btVector3& vector ) {
	return Vec3D( vector.getX(), vector.getY(), vector.getZ() );
}

inline btVector3 Convert( const Vec3D& vector ) {
	return btVector3( vector.X, vector.Y, vector.Z );
}

//===========================================================================================================

void ConvertIrrlichtMeshIntoBulletMesh( const ::irr::scene::IMesh* pMesh, btTriangleMesh *OutPhysTriMesh );
void ConvertMeshIntoPhysicsShape( const mxRenderMesh& rMesh, btTriangleMesh *OutPhysTriMesh );

//===========================================================================================================

//
//	CPlayerPhysicsEntity
//
//	Simulates the motion of a player through the environment. Input from the
//	player is used to allow a certain degree of control over the motion.
//
class CPlayerPhysicsEntity : public CBulletPhysicsEntity {
public:
			CPlayerPhysicsEntity();
			~CPlayerPhysicsEntity();
};

//
//	ClosestNotMeRayResultCallback
//
class ClosestNotMeRayResultCallback : public btCollisionWorld::ClosestRayResultCallback
{
public:
	ClosestNotMeRayResultCallback( btCollisionObject* me )
		: btCollisionWorld::ClosestRayResultCallback( btVector3(0.0, 0.0, 0.0), btVector3(0.0, 0.0, 0.0) )
	{
		m_me = me;
	}

	virtual btScalar addSingleResult( btCollisionWorld::LocalRayResult& rayResult, bool normalInWorldSpace )
	{
		if ( rayResult.m_collisionObject == m_me ) {
			return 1.0;
		}
		return ClosestRayResultCallback::addSingleResult( rayResult, normalInWorldSpace );
	}

protected:
	btCollisionObject * m_me;
};

}//end of namespace mix

#endif /* ! __MX_PHYSICS_H__ */

//--------------------------------------------------------------//
//				End Of File.									//
//--------------------------------------------------------------//
