/*
===============================================================================
	File:	GameWeapons.cpp
	Desc:.
===============================================================================
*/

#include "stdafx.h"
#pragma hdrstop
#include <Include/IrrMix.h>

/*
=============================================================

		Built-in game weapons.

=============================================================
*/

namespace mix {

//
//	Weapon_GeoModder
//
//	Can perform CSG operations Subtract and Union on level's geometry.
//
class Weapon_GeoModder : public mxGameWeapon {
public:
				Weapon_GeoModder();
				~Weapon_GeoModder();

	// overrides ( mxGameWeapon )

	void	PrimaryFire();
	void	SecondaryFire();

private:
	CsgInfo		m_csgInfo;
};

/*================================
	class Weapon_GeoModder
================================*/

Weapon_GeoModder::Weapon_GeoModder()
{
	//m_name = "[GeoModder]";
	//m_sounds[ Empty ] = GSoundWorld->CacheSound( PATH_TO_MEDIA "Sounds/Weapons/dry_fire.ogg" );
	//m_sounds[ Single ] = GSoundWorld->CacheSound( PATH_TO_MEDIA "Sounds/Weapons/GeoModder_Single.ogg" );

	//m_sounds[ Single ]->setDefaultVolume( 0.2f );
}

Weapon_GeoModder::~Weapon_GeoModder()
{}

void Weapon_GeoModder::PrimaryFire()
{
	BeginAttack();

	if ( m_owner )
	{
		mxGamePlayer * player = static_cast< mxGamePlayer* >( m_owner );

		const Vec3D  vStart( player->GetOrigin() );

		Vec3D  vDirection( player->GetCameraTarget() - vStart );
		vDirection.normalize();

		const f32  fMaxTraceDist = WORLD_EXTENT;
		Vec3D  vEnd( vStart + vDirection * fMaxTraceDist );

		Vec3D  vIntersection;
		Environment->Debug_TraceLine( vStart, vEnd, vIntersection, (void*)1 );

	//	m_pCurrentSound = GSoundWorld->Play2D( m_sounds[ Single ] );
	}
}

void Weapon_GeoModder::SecondaryFire()
{
	BeginAttack();

	if ( m_owner )
	{
		mxGamePlayer * player = static_cast< mxGamePlayer* >( m_owner );

		const Vec3D  vStart( player->GetOrigin() );

		Vec3D  vDirection( player->GetCameraTarget() - vStart );
		vDirection.normalize();

		const f32  fMaxTraceDist = WORLD_EXTENT;
		Vec3D  vEnd( vStart + vDirection * fMaxTraceDist );

		Vec3D  vIntersection;
		Environment->Debug_TraceLine( vStart, vEnd, vIntersection, NULL );

	//	m_pCurrentSound = GSoundWorld->Play2D( m_sounds[ Single ] );
	}
}

//
//	Weapon_TestObjectSpawner
//
class Weapon_TestObjectSpawner : public mxGameWeapon {
public:
				Weapon_TestObjectSpawner();
				~Weapon_TestObjectSpawner();

	// overrides ( mxGameWeapon )

	void	PrimaryFire();
	void	SecondaryFire();
};

/*==============================================
	class CreateWeapon_TestObjectSpawner
==============================================*/

Weapon_TestObjectSpawner::Weapon_TestObjectSpawner()
{
	m_name = "[TestObjectSpawner]";
//	m_sounds[ Empty ] = GSoundWorld->CacheSound( PATH_TO_MEDIA "Sounds/Weapons/dry_fire.ogg" );
//	m_sounds[ Single ] = GSoundWorld->CacheSound( PATH_TO_MEDIA "Sounds/Weapons/GeoModder_Single.ogg" );
}

Weapon_TestObjectSpawner::~Weapon_TestObjectSpawner()
{}

void Weapon_TestObjectSpawner::PrimaryFire()
{
	BeginAttack();

	if ( m_owner )
	{
		mxGamePlayer * player = static_cast< mxGamePlayer* >( m_owner );

		const Vec3D  vStart( player->GetOrigin() );

		Vec3D  vDirection( player->GetCameraTarget() - vStart );
		vDirection.normalize();

		mxEntity * sphere = new Test_Sphere( 1.0f, // radius
			player->GetOrigin() + vDirection * (player->GetPlayerRadius() + 0.5f) );

		sphere->GetPhysics()->SetLinearVelocity( vDirection * 60 );
		sphere->GetPhysics()->SetAngularVelocity( Vec3D( 0.f, 0.1f, 0.5f ) );

		::mix::GWorld->Environment->AddEntity( sphere );
	}
}

void Weapon_TestObjectSpawner::SecondaryFire()
{
	BeginAttack();

	if ( m_owner )
	{
		mxGamePlayer * player = static_cast< mxGamePlayer* >( m_owner );

		const Vec3D  vStart( player->GetOrigin() );

		Vec3D  vDirection( player->GetCameraTarget() - vStart );
		vDirection.normalize();

		mxEntity * box = new mxBoxEntity( Vec3D(1,2,1),
			player->GetOrigin() + vDirection * (player->GetPlayerRadius() + 0.5f) );

		box->GetPhysics()->SetLinearVelocity( vDirection );
		box->GetPhysics()->SetAngularVelocity( Vec3D( 0.f, 0.1f, 0.5f ) );

		::mix::GWorld->Environment->AddEntity( box );
	}
}

//===================================================================

mxGameWeapon * CreateWeapon_GeoModder()
{
	mxGameWeapon * newWeapon = new Weapon_GeoModder();
	GWorld->Environment->AddEntity( newWeapon );
	return newWeapon;
}
//mxGameWeapon * CreateWeapon_SMG()
//{
//	return new Weapon_SMG();
//}
mxGameWeapon * CreateWeapon_TestObjectSpawner()
{
	mxGameWeapon * newWeapon = new Weapon_TestObjectSpawner();
	GWorld->Environment->AddEntity( newWeapon );
	return newWeapon;
}
//===================================================================

//
//	Weapon_SMG
//
class Weapon_SMG : public Weapon_Automatic {
public:
				Weapon_SMG();
				~Weapon_SMG();

	// overrides ( mxGameWeapon )

	void	PrimaryFire();
	void	SecondaryFire();
};

mxGameWeapon * CreateWeapon_SMG() {
	mxGameWeapon * newWeapon = new Weapon_SMG();
	GWorld->Environment->AddEntity( newWeapon );
	return newWeapon;
}

/*==============================================
	class CreateWeapon_TestObjectSpawner
==============================================*/

Weapon_SMG::Weapon_SMG()
{
	//m_name = "[Weapon_SMG]";
	//m_sounds[ Empty ] = GSoundWorld->CacheSound( PATH_TO_MEDIA "Sounds/Weapons/dry_fire.ogg" );
	//m_sounds[ Single ] = GSoundWorld->CacheSound( PATH_TO_MEDIA "Sounds/Weapons/SMG_Fire1.ogg" );

	//m_sounds[ Single ]->setDefaultVolume( 0.2f );
//
	//m_fireRate = 11;
}

Weapon_SMG::~Weapon_SMG()
{}

void Weapon_SMG::PrimaryFire()
{
	BeginAttack();

	if ( m_owner )
	{
		mxGamePlayer * player = static_cast< mxGamePlayer* >( m_owner );

		const Vec3D  vStart( player->GetOrigin() );

		Vec3D  vDirection( player->GetCameraTarget() - vStart );
		vDirection.normalize();

	//	GSoundWorld->Play2D( m_sounds[ Single ] );

		// Shoot a ray.
		//TODO: this should be performed by Environment.
		{
			Vec3D  ray( player->GetCameraTarget() - player->GetOrigin() );
			ray.normalize();
			ray *= HALF_WORLD_EXTENT + 1;

			// Apply an impulse.
			btVector3  cameraPos;
			btVector3  rayTo;

			Assign( cameraPos, player->GetOrigin() );
			Assign( rayTo, ray );

			btCollisionWorld::ClosestRayResultCallback  rayCallback( cameraPos, rayTo );

			GPhysicsWorld->m_pDynamicsWorld->rayTest( cameraPos, rayTo, rayCallback );

			if ( rayCallback.hasHit() )
			{
				if ( btRigidBody * body = btRigidBody::upcast( rayCallback.m_collisionObject ) )
				{
					// Spawn a decal.
					if ( void * pUserPtr = body->getUserPointer() )
					{
						mxEntity * pEntity = ( mxEntity* ) pUserPtr;

						const Vec3D  hitPoint( Convert( rayCallback.m_hitPointWorld ) );
						const Vec3D  hitNormal( Convert( rayCallback.m_hitNormalWorld ) );

						if ( mxRenderEntity * pRenderEntity = pEntity->IsRenderEntity() )
						{
							pRenderEntity->ApplyDecal( hitPoint, hitNormal );
						}
					}//End Of Decal code


					// Apply impulse to the hit body.
					if ( ! body->isStaticObject() )
					{
						body->setActivationState( ACTIVE_TAG );
						btVector3  impulse = rayTo;
						impulse.normalize();

						btScalar  impulseStrength = 200.f;
						impulse *= impulseStrength;

						btVector3  relPos = rayCallback.m_hitPointWorld - body->getCenterOfMassPosition();
						body->applyImpulse( impulse, relPos );
					}
				}
			}//End If Has Hit
		}//End Of raytracing
	}//if owner != NULL
}

void Weapon_SMG::SecondaryFire()
{
	//BeginAttack();

	if ( m_owner )
	{
		mxGamePlayer * player = static_cast< mxGamePlayer* >( m_owner );

		const Vec3D  vStart( player->GetOrigin() );

		Vec3D  vDirection( player->GetCameraTarget() - vStart );
		vDirection.normalize();

	//	GSoundWorld->Play2D( m_sounds[ Empty ] );

		//mxEntity * box = new CBoxEntity( Vec3D(100,200,10),
		//	player->GetOrigin() + vDirection * (player->GetPlayerRadius() + 10.f) );

		//box->GetPhysics()->SetLinearVelocity( vDirection * 300 );
		//box->GetPhysics()->SetAngularVelocity( Vec3D( 0.f, 0.1f, 0.5f ) );

		//::mix::GWorld->Environment->AddEntity( box );
	}
}

}//end of namespace mix

//--------------------------------------------------------------//
//				End Of File.									//
//--------------------------------------------------------------//
