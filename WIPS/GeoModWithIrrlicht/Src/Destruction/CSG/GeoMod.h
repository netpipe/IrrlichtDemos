/*
===============================================================================
	File:	GeoMod.h
	Desc:.
===============================================================================
*/

#ifndef __MX_DESTRUCTION__GEOMODDABLE_SOLID_H__
#define __MX_DESTRUCTION__GEOMODDABLE_SOLID_H__

namespace mix {

//
// Defines.
//

#define FAST_CSG	// Fast/low accuracy CSG operations.

//--------------------------------------------------------------------------------------

// This enum describes the allowed set operation types.
enum ESetOperation
{
	CSG_Subtract,				// Difference
	CSG_Union,					// The union set operation.

	NumOfSetOps		// Marker
};

//
//	CsgInfo
//
class CsgInfo {
public:
	ESetOperation	setOperation;

	CsgInfo()
		: setOperation( CSG_Subtract )
	{}
};

//======================================================================

// Forward declarations.
class mxGeomoddableSolid;
class mxCrackPattern;
class mxGeoModMesh;

//
//	mxGeomoddableSolid
//
//	Static object on which CSG operations can be performed.
//
class mxGeomoddableSolid : public mxRenderEntity {
public:
	virtual			~mxGeomoddableSolid() {}

	virtual	void	CreateFromMesh( const ::irr::scene::IMesh* mesh ) {}

//	virtual	void	ApplyFracture( mxCrackPattern& fracture ) {}
//	virtual	void	CSG_Subtract( mxGeoModMesh& mesh ) {}
//	virtual	void	CSG_Union( mxGeoModMesh& mesh ) {}

//	virtual void	Melt( const Vec3D& hitSource, const f32 factor ) {}
//	virtual void	TraceLine_GeoMod( const Vec3D& start, const Vec3D& end, const CsgInfo& params ) = 0;


					// Set the mesh for CSG subtraction.
	virtual void	SetMeshForSubtract( const ::irr::scene::IMesh* mesh ) {}

					// Set the mesh for CSG union.
	virtual void	SetMeshForUnion( const ::irr::scene::IMesh* mesh ) {}

					// Should return 'true' if hit anything.
	virtual	bool	Debug_TraceLine_Subtract( const Vec3D& start, const Vec3D& end ) { return false; }
	virtual	bool	Debug_TraceLine_Union( const Vec3D& start, const Vec3D& end ) { return false; }
};

}//end of namespace mix

#endif /* ! __MX_DESTRUCTION__GEOMODDABLE_SOLID_H__ */

//--------------------------------------------------------------//
//				End Of File.									//
//--------------------------------------------------------------//
