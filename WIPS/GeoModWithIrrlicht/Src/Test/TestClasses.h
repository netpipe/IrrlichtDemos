/*
===============================================================================
	File:	TestClasses.h
	Desc:	Experimental code for testing ideas.
===============================================================================
*/

#ifndef __MX_TEST_CLASSES_H__
#define __MX_TEST_CLASSES_H__

namespace mix {

// Forward declarations.
class mxEntity;
class CsgInfo;

//
// TestLevel
//
class TestLevel : public mxEnvironment {
public:
			TestLevel();
			~TestLevel();

	// overrides ( mxEnvironment )
	virtual void	Spawn();
	virtual void	Shutdown();

	//--- Physics --------------------------------------------------------------------------


	//--- Destruction ----------------------------------------------------------------------------

	// Testing & Debugging.

	// overrides ( mxEnvironment )
	bool Debug_TraceLine( const Vec3D& start, const Vec3D& end, Vec3D &OutResult, void* pUserPtr );

protected:
	// Called upon adding the given entity to the environment.
	virtual void	PreLinkEntity( mxEntity* entity ) {}

	// Called when removing the given entity from the environment.
	virtual void	UnLinkEntity( mxEntity* entity ) {}

private:
	::irr::scene::ISceneNode *	m_pSceneNode;	// For testing & debugging.
	mxGeomoddableSolid *		m_pSolid;
};

//
//	mxBoxEntity
//
class mxBoxEntity : public mxRenderEntity {
public:
	mxBoxEntity( const Vec3D& dimensions, const Vec3D& position );
	~mxBoxEntity();

	// overrides ( mxRenderEntity )
	void	Render( const mxView& view );
	void	ApplyDecal( const Vec3D& position, const Vec3D& normal );

	const Vec3D &	GetDimensions() const;

	// overrides ( mxEntity )
	void	RunPhysics();

private:
	TRenderMaterial		m_material;
	mxRenderMesh		m_renderMesh;

	// Decals. ---------------------------------------------
	//
	//	Decal_s - the simplest decal possible.
	//	Should be as small as possible.
	//
	class Decal {
	public:
		TVertex		quad[ 4 ];	// Data for rendering.

		void	Init( const Vec3D& position, const Vec3D& normal, const f32 size );
	};
	TArray< Decal >		m_arrDecals;
	TRenderMaterial		m_decalMaterial;
};

//
//	mxStaticPlaneEntity
//
class mxStaticPlaneEntity : public mxRenderEntity {
public:
	mxStaticPlaneEntity( const TPlane3D& plane = TPlane3D( Vec3D( 0,1,0 ), 0 ) );
	~mxStaticPlaneEntity();

	// overrides ( mxRenderEntity )
	void	Render( const mxView& view );
	void	ApplyDecal( const Vec3D& position, const Vec3D& normal );

private:
	TPlane3D				m_plane;

	// Data for rendering.
	mxVertex			m_vertices[ 4 ];
	TRenderMaterial		m_material;

	// Decals. ---------------------------------------------
	//
	//	Decal_s - the simplest decal possible.
	//	Should be as small as possible.
	//
	struct Decal_s {
		TVertex		quad[ 4 ];	// Data for rendering.
	};
	TArray< Decal_s >	m_arrDecals;
	TRenderMaterial		m_decalMaterial;
};

}//end of namespace mix

#endif /* ! __MX_TEST_CLASSES_H__ */

//--------------------------------------------------------------//
//				End Of File.									//
//--------------------------------------------------------------//

