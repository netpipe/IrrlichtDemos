/*
===============================================================================
	File:	WorldClasses.h
	Desc:
===============================================================================
*/

#ifndef __MX_WORLD_CLASSES_H__
#define __MX_WORLD_CLASSES_H__

#include "Include/Containers/Templates.h"
// Forward declarations.
namespace irr {
namespace scene {
	class ISceneNode;
		class ICameraSceneNode;
}//end of namespace scene
}//end of namespace irr


namespace mix {

//
//	mxClass - base class for most game classes.
//
class mxClass {
public:
	virtual		~mxClass();

	void *		operator new( size_t  num );
	void		operator delete( void* mem );
};

//===============================================================================================

//
//	mxThinkingEntity
//
class mxThinkingEntity {
public:
	virtual ~mxThinkingEntity() {}

	virtual void	Think( const REAL fElapsedTime ) = 0;
};

//
//	mxTask
//
class mxTask {
public:
	virtual ~mxTask() {}

	virtual void	Start() = 0;
	virtual void	Execute( const REAL fElapsedTime ) = 0;
	virtual void	Terminate() = 0;
};

//===============================================================================================

// Forward declarations.
class mxMaterial;
class mxVolumeContents;

//
//	mxMaterial
//
class mxMaterial {
public:
private:
	friend class mxMaterialSystem;

	mxMaterial();
	~mxMaterial();

	const u32	m_iMaterialID;	// Unique material ID assigned to this material.
};

//
//	ContactInfo_s - for generating impact sounds.
//
struct ContactInfo_s {
	f32		impulse;
};

//
//	mxMaterialSystem
//
class mxMaterialSystem {
public:
			mxMaterialSystem();
			~mxMaterialSystem();


	// For playing impact sounds.
//	mxSoundSource *		GetSoundForMaterials( const mxMaterial* material0, const mxMaterial* material1, const ContactInfo_s& contactInfo );

private:
//	mxSoundSource *		m_tempSndSrc;
};

//
//	DamageInfo_s
//
struct DamageInfo_s {
	mxMaterial *	SurfaceMaterial;
};

// Forward declarations.
class mxDecal;

//
//	mxDecalManager
//
//	For creating bullet holes, impact splashes, scorch marks, etc.
//
class mxDecalManager {
public:
	mxDecal *	GetDecal( const DamageInfo_s& damageInfo );
};

//===============================================================================================

// Forward declarations.
class mxEntity;
	class mxRenderEntity;
	class mxPhysicsEntity;

//
//	mxEntity - base class for most game entity classes.
//
class mxEntity : public mxClass, public mxThinkingEntity {
public:
				mxEntity();
	virtual		~mxEntity();

	//--- Thinking -------------------------------------------------------------------------

	// overrides ( mxThinkingEntity )
	virtual void	Think( const REAL fElapsedTime ) {}

	//--- Physics -------------------------------------------------------------------------

						// Set a new physics object to be used by this entity.
	void				SetPhysics( mxPhysicsEntity* newPhysics );

						// Get the physics object used by this entity.
	mxPhysicsEntity *	GetPhysics() const;

						// (Called once per frame.)
	virtual void		RunPhysics() {}

						// Set the origin of the physics object.
	virtual void		SetOrigin( const Vec3D& newOrigin );

	const mxMaterial *	GetMaterial() const;

	//--- Miscellaneous --------------------------------------------------------------------

//	virtual mxGamePlayer *	IsGamePlayer() { return NULL; }

	// This function is called from low-level collision-detection code.
	// Portals will override this to teleport the entity.
	virtual void	CollideWith( mxEntity *other ) {}

	// Returns Irrlicht scene node which represents the visuals of this entity.
	//virtual ::irr::scene::ISceneNode *	HasAttachedSceneNode() { return NULL; }

	virtual mxRenderEntity *	IsRenderEntity() { return NULL; }

protected:
	mxPhysicsEntity *		m_physics;	// physics used for this entity

	mxMaterial *			m_material;	// for generating sounds on impact, etc.
};

inline mxPhysicsEntity * mxEntity::GetPhysics() const {
	return m_physics;
}

inline const mxMaterial * mxEntity::GetMaterial() const {
	return m_material;
}

//===============================================================================================

//
//	mxRenderEntity
//
//	Render entities form a dynamic scene graph.
//
//	NOTE (written just before uploading): that was a major mistake. I should have used Irrlicht scene nodes for rendering.
//	(i was dissapointed by Irrlicht's primitive render system.)
//
class mxRenderEntity : public mxEntity
{
	friend class mxRenderWorld;

	//TCircularList< mxRenderEntity >		ListNode;	// For being linked into the render list.

protected:
	mxRenderEntity();	// Inserts itself into the render list.
	~mxRenderEntity();	// Removes itself from the render list.

public:
	virtual mxRenderEntity *	IsRenderEntity() { return this; }

//	virtual bool	IsVisible( const mxView& view ) { return true; }

					// Called just before rendering this entity.
//	virtual void	RegisterForRendering() {}

//	virtual void	Render( const mxView& view ) = 0;

	// TODO: rename to 'ApplyDamage' ? pass damage info and generate decals, marks, sounds, etc.
//	virtual void	ApplyDecal( const Vec3D& position, const Vec3D& normal );


	void	SetPosition( const Vec3D& newPosition );
	void	SetRotation( const Vec3D& rotationInDegress );

	const Vec3D &	GetPosition() const;
	const Vec3D &	GetRotation() const;

			// NOTE: Expensive! Call this only if the entity is visible.
			// TODO: make this virtual? this only makes sense for moving, non-static objects.
	void	UpdateWorldTransform();

	const TMatrix4 &	GetWorldTransform() const;

protected:
	Vec3D			m_vTranslation;
	Vec3D			m_vRotation;	// In degrees.
	TMatrix4		m_worldTransform;		// Absolute transformation (for rendering).
};

//===============================================================================================

// TODO: remove this class:
//
//	mxPhysicsEntity - a physics object in a simulation.
//
class mxPhysicsEntity {
protected:
	friend class mxPhysicsWorld;

	virtual		~mxPhysicsEntity() {}

public:
	virtual void	SetOrigin( const Vec3D& newOrigin ) = 0;
	virtual void	SetLinearVelocity( const Vec3D& velocity ) = 0;
	virtual void	SetAngularVelocity( const Vec3D& newAngularVelocity ) = 0;

	virtual const Vec3D		GetOrigin() const = 0;
	virtual const Vec3D		GetRotationInDegrees() const = 0;

	virtual void	SetEntityPtr( mxEntity* entity ) = 0;
};

//===============================================================================================

/*
======================================
			World Trace
======================================
*/

//
//	mxContactInfo
//
class mxContactInfo {
public:
	mxMaterial *		Material;	// Surface material.
	mxVolumeContents *	Contents;	// Contents at other side of surface.
	Vec3D				Position;	// Point of contact.
	Vec3D				Normal;		// Contact plane normal.
};

//
//	mxTrace
//
//	A trace is returned as a result of spatial queries against entities.
//
class mxTrace {
public:
	mxEntity *		HitEntity;

	mxContactInfo	ContactInfo;	// Contact information, only valid if (HitEntity != NULL).
};

//
//	mxScratchpad
//
//	A piece of memory used for temporary calculations.
//
class mxScratchpad {
public:
};

//
//	mxEntityCache
//
class mxEntityCache {
public:
	TArray< mxEntity* >		Entities;
};

//
//	ISpatialSorting - interface for sorting a scene.
//
template< typename T >
class ISpatialSorting {
public:
	virtual			~ISpatialSorting() {}

	virtual void	InsertObject( T* pObject ) = 0;
	virtual void	DeleteObject( T* pObject ) = 0;

	virtual void	TraceLine( mxEntity* pSourceEntity, const Vec3D& start, const Vec3D& end, mxTrace &OutTraceResult ) = 0;
	virtual void	TraceRay( mxEntity* pSourceEntity, const Vec3D& start, const Vec3D& direction, mxTrace &OutTraceResult ) = 0;

	//--- Get all entities in a particular volume. ---------------------------------------------------------------------------
	virtual void	EnumerateEntitiesInPoint( const Vec3D& point, mxEntityCache &OutEntities ) = 0;
	virtual void	EnumerateEntitiesAlongLine( const Vec3D& start, const Vec3D& end, mxEntityCache &OutEntities ) = 0;
	virtual void	EnumerateEntitiesAlongRay( const Vec3D& start, const Vec3D& direction, mxEntityCache &OutEntities ) = 0;
	virtual void	EnumerateEntitiesInBox( const Vec3D& min, const Vec3D& max, mxEntityCache &OutEntities ) = 0;
	virtual void	EnumerateEntitiesInSphere( const Vec3D& origin, const f32& radius, mxEntityCache &OutEntities ) = 0;

	virtual void	Tick( f32 elapsedTime ) = 0;
};

//===============================================================================================

//
//	mxActor - entity in a certain environment.
//
class mxActor : public mxEntity {
public:
//	mxActor()
//		: Environment( GWorld->Environment )
//	{}

//	~mxActor()
//	{}

public:
//	mxEnvironment *		Environment;	// Environment this entity is in.
};

//-------------------------------------------------------------------

//
//	mxSpectator
//
class mxSpectator : public mxActor {
public:
				mxSpectator();
				~mxSpectator();

	void	Spawn();

	const Vec3D &	GetCameraTarget() const	{ return m_pCamera->getTarget(); }

	::irr::scene::ICameraSceneNode *	GetCamera() { return m_pCamera; }

	// overrides ( mxEntity )
	void SetOrigin( const Vec3D& newOrigin )
	{
		m_pCamera->setPosition( newOrigin );
	}

	const Vec3D & GetOrigin() const
	{
		return m_pCamera->getPosition();
	}

protected:
	::irr::scene::ICameraSceneNode *	m_pCamera;
};

//===============================================================================================

//
// CSceneNodeEntity - game entity with an Irrlicht scene node attached.
//
class CSceneNodeEntity : public mxEntity {
public:
			CSceneNodeEntity();
			~CSceneNodeEntity();

	// override ( CEntity )
	void	RunPhysics();

	void	SetOrigin( const Vec3D& newOrigin );

	//::irr::scene::ISceneNode *	HasAttachedSceneNode() { return m_pSceneNode; }

protected:
	::irr::scene::ISceneNode *	m_pSceneNode;
};

//
//	CBoxEntity
//
class CBoxEntity : public CSceneNodeEntity {
public:
	CBoxEntity( const Vec3D& dimensions, const Vec3D& position );
	~CBoxEntity();
};

//
// Test_Sphere
//
class Test_Sphere : public CSceneNodeEntity {
public:
	Test_Sphere( f32 radius, const Vec3D& position );
	~Test_Sphere();
};

//
//	CLargeStaticPlaneEntity
//
class CLargeStaticPlaneEntity : public mxEntity {
public:
	CLargeStaticPlaneEntity( const TPlane3D& plane = TPlane3D( Vec3D( 0,1,0 ), 0 ) );
	~CLargeStaticPlaneEntity();
};

}//end of namespace mix

#endif // ! __MX_WORLD_CLASSES_H__

//--------------------------------------------------------------//
//				End Of File.									//
//--------------------------------------------------------------//
