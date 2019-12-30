/*
===============================================================================
	File:	BSP.h
	Desc:	.
===============================================================================
*/

#ifndef __MX_DESTRUCTION_CSG_BSP_H__
#define __MX_DESTRUCTION_CSG_BSP_H__

#include "../../Include/IrrMix.h"

//#include "BSP.h"

namespace mix {

//
//	TPrimitives
//
class TPrimitives {
public:
	TFaceHandle		faces;	// linked list of faces
	//mxBounds	bounds;		// extent of the entire list
};

//
//	MeshGenerationHeuristics - used for generating a render mesh from a BSP tree.
//
class MeshGeneration_Heuristics {
public:
};

//
typedef int TNodeHandle;

//
//	BspNode
//
class BspNode {
public:

	// This enum describes the allowed BSP node types.
	enum E_Type
	{
		// For polygon-aligned nodes.
		Internal,	// An internal partitioning node.
		OutCell,	// An outcell leaf node ( representing empty space ).
		InCell,		// An incell leaf node ( representing solid space ).
		Undecided,	// Used by progressive BSP trees.

		// For axis-aligned splitting planes.
		Plane_XY,
		Plane_XZ,
		Plane_YZ,
	};

	E_Type		kind;	// Specifies the node's type.

	TPrimitives		primitives;	// specifies a list of objects ( polygons ) that fall into the node.

	// Hyperplane of the node
	//u16			plane;// index of the splitting plane (in the plane array)
	mxPlane			plane;

	// Extent of the node including all its children.
	// Used for frustum culling, coarse side tests, etc.
	mxBounds	bounds;

	// Two pointers to children nodes.
	TNodeHandle  front;
	TNodeHandle  back;

public:
	bool	IsLeaf() const;
	bool	IsInternal() const;

	bool	IsSolid() const;
	bool	IsSpace() const;
};

inline bool BspNode::IsLeaf() const
{ return kind != Internal; }

inline bool BspNode::IsInternal() const
{ return kind == Internal; }

inline bool BspNode::IsSolid() const
{ return kind == InCell; }

inline bool BspNode::IsSpace() const
{ return kind == OutCell; }

//
//	Partition_Heuristics
//
// used for BSP during the tree building process.
class Partition_Heuristics {
public:
	// Select a splitter from the given linked list of polygons
	// and remove it from the list.
	//LFace *	SelectSplitter( LFace** polygons );
};

//
//	BspStats
//
class BspStats {
public:
	u32		numPolygons;	// number of resulting polygons
	u32		numSplits;		// number of cuts caused by BSP

	u32		numInternalNodes;
	//u32		depth;
	u32		numSolidLeaves, numEmptyLeaves;

	u32		elapsedTime;	// time required to finish task
private:
	u32		m_beginTime;	// for measuring time with timer

public:
			BspStats();

	void	Reset();
	void	Stop();

	void	Print();
};

#ifdef MX_DEBUG
	#define STATS( code )	code
#else
	#define STATS( code )
#endif // !__IRRMIX_DEBUG__

//
//	CsgStats
//
class CsgStats {
public:
			CsgStats();

	void	Reset();
	void	Stop();

	void	Print();

	u32		GetElapsedTime() const;

private:
	u32		m_beginTime;	// for measuring time with timer
	u32		m_elapsedTime;	// time required to finish task
};

inline u32 CsgStats::GetElapsedTime() const {
	return m_elapsedTime;
}

//----------------------------------------------------------------------------------

//
//	BspSolid
//
class BspSolid

//#ifdef BULLET_PHYSICS
//	: public btConcaveShape
//#endif

{
public:
				BspSolid();
				~BspSolid();

	void		Build( mxRenderMesh& mesh );

	void		GenerateMesh( mxRenderMesh &OutMesh );

				// Builds a tree from the mesh and uses it to construct a valid (closed, 2-manifold) mesh.
	//void		MakeConsistent( mxRenderMesh & mesh );

				// Transforms the BSP tree by a 4x4 homogeneous matrix.
	void		Transform( const TMatrix4& matrix );

				// Returns the bounding volume of the entire tree.
	const mxBounds & GetBounds() const;

	void		Clear();

				// Returns 'true' if hit anything.
	bool		TraceLine( const Vec3D& start, const Vec3D& end, Vec3D &OutResult );

				// 'point' is assumed to lie on the face's plane.
				// NOTE: slow.
				// TODO: move this code into mxDynamicMesh.
	bool		IsPointInsideFace( const Vec3D& point, TFaceHandle  hFace, const f32 epsilon = 0.001f ) /*const*/;

	void		Merge_Subract( mxGeoModMesh& rGeoModMesh );

	//-------------------------------------------------------------------------------------------------------

#ifdef BULLET_PHYSICS
	// overrides ( btConcaveShape )
	//void processAllTriangles( btTriangleCallback* callback, const btVector3& aabbMin, const btVector3& aabbMax ) const;
#endif

private:
	TNodeHandle  BuildTree_R( TFaceHandle  faceHandle );

	TFaceHandle  SelectSplitter( TFaceHandle  faceHandle );

	void SplitFace( TFaceHandle  inFace, const mxPlane& splitPlane,
		TFaceHandle &OutFrontPolys, TFaceHandle &OutBackPolys, TFaceHandle &OutCoplanarPolys );

	TNodeHandle  GetEmptyLeaf();
	TNodeHandle  GetSolidLeaf();

	bool IsLeaf( TNodeHandle  hNode ) const;
	bool IsSolidLeaf( TNodeHandle  hNode ) const;
	bool IsEmptyLeaf( TNodeHandle  hNode ) const;
	bool IsInternal( TNodeHandle  hNode ) const;

	// OutFaceBounds += inFace.GetBounds()
	void UpdateBounds( TFaceHandle  inFace, mxBounds &OutFaceBounds );

private:
	bool	TraceLine_R( TNodeHandle  hNode, const Vec3D& start, const Vec3D& end, Vec3D &OutResult );

	// BSP tree merging
	void	MergeSubtract_R( TNodeHandle & node0, TNodeHandle  node1 );

private:
	void	Init();

	BspNode &	GetNode( TNodeHandle  index );

private:
	TNodeHandle			m_root;

	mxDynamicMesh		m_mesh;	// geometry storage + topology info

	// All BSP nodes are kept here.
	// The first and second elements are 'empty' and 'solid' leaves respectively.
	enum { EMPTY_NODE_INDEX = 0, SOLID_NODE_INDEX = 1 };
	TArray< BspNode >	m_nodes;

	// Temporary storage used by polygon clipping routines, line checking code, etc..
	enum { MAX_FACE_VERTS = 256 }; // max. number of vertices a face may have
	mutable TFixedArray< f32,			MAX_FACE_VERTS + 4 >	TmpDists;
	mutable TFixedArray< E_PlaneSide,	MAX_FACE_VERTS + 4 >	TmpSides;
	mutable TFixedArray< u32, 3 >								TmpCounts;
	mutable TFixedArray< Vec3D,	MAX_FACE_VERTS + 4 >			TmpVectors;
	//mutable TFixedArray< mxVertex,	MAX_FACE_VERTS + 4 >	TmpVertices;

	// For testing & debugging.
	TFaceHandle			TmpLastHitFace;	// is set by ray-tracing routines

	// For debugging.
	STATS( BspStats  m_stats; )
};

inline BspNode & BspSolid::GetNode( TNodeHandle  index ) {
	return m_nodes[ index ];
}

inline TNodeHandle  BspSolid::GetEmptyLeaf() {
	return EMPTY_NODE_INDEX;
}

inline TNodeHandle  BspSolid::GetSolidLeaf() {
	return SOLID_NODE_INDEX;
}

inline bool BspSolid::IsLeaf( TNodeHandle  hNode ) const
{
	return hNode == EMPTY_NODE_INDEX
		|| hNode == SOLID_NODE_INDEX;
}

inline bool BspSolid::IsSolidLeaf( TNodeHandle  hNode ) const {
	return hNode == SOLID_NODE_INDEX;
}

inline bool BspSolid::IsEmptyLeaf( TNodeHandle  hNode ) const {
	return hNode == EMPTY_NODE_INDEX;
}

inline bool BspSolid::IsInternal( TNodeHandle  hNode ) const {
	return hNode > SOLID_NODE_INDEX;
}

//
//	mxGeoModMesh
//
class mxGeoModMesh {
public:
			mxGeoModMesh();
			~mxGeoModMesh();

	void	Build( const mxRenderMesh& inMesh );

//private:
	const CsgInfo		m_info;	// is this mesh for CSG subtract or union ?

	mxDynamicMesh		m_mesh;

	TNodeHandle			m_root;
	TArray< BspNode >	m_nodes;
};

}//end of namespace mix

#endif /* ! __MX_DESTRUCTION_CSG_BSP_H__ */

//--------------------------------------------------------------//
//				End Of File.									//
//--------------------------------------------------------------//
