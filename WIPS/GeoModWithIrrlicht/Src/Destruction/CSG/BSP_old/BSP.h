/*
===============================================================================
	File:	BSP.h
	Desc:	.
===============================================================================
*/

#ifndef __MX_DESTRUCTION_CSG_OLD_BSP_H__
#define __MX_DESTRUCTION_CSG_OLD_BSP_H__

namespace mix {

// TVertex used for rendering and CSG operations.
typedef mxVertex	BspVertex;

//
//	Polygon - a simple (convex) face.
//
//	NOTE: a fixed array of vertices; wastes a lot of memory.
//	TODO: use adjacency data structure to avoid cracks and holes resulting from polygon splitting,
//	for vertex normals recalculation, for determination of isolated pieces, floating chunks of solid material,
//	for saving memory and avoiding duplication, etc.
//
 struct Polygon
{
	Polygon()
		: m_NumVertices( NULL )
		, m_pNext( NULL )
		, Flags( 0 )
	{}

	enum { MAX_VERTICES = 16 };					// Maximal number of vertices a polygon may have.
	enum { SPLIT_THRESHOLD = MAX_VERTICES-2 };	// Threshold for splitting into two polys when the number of vertices is too big.

	BspVertex	Vertices[ MAX_VERTICES ];	// actual vertices

	mxPlane		Plane;

	UBYTE		Flags;

private:
	u32			m_NumVertices;

	Polygon *	m_pNext;	// Used for organizing linked lists of polygons.

public:
	u32		NumVertices() const;

	void	AddVertex( const BspVertex& v );

	void	SetVertex( u32 index, const mxVertex& renderVertex );

	const BspVertex &	GetVertex( u32 index ) const;
	const Vec3D &		GetPos( u32 index ) const;

	Polygon *		GetNext();
	const Polygon *	GetNext() const;
	void			SetNext( Polygon* face );

	E_PlaneSide		GetRelationToPlane( const mxPlane& plane, const f32 epsilon = PLANESIDE_EPSILON ) const;

			// Add the points of this polygon to the specified bounding box.
	void	AddToBounds( mxBounds &OutBounds ) const;

//	void			RecalculateBounds();	// Recalculate the bounding box of this polygon.
//	const TBounds &	GetBounds() const	{ return m_bounds; }

	const mxPlane &	GetPlane() const;

	bool	PointInside( const Vec3D& point, const f32 epsilon = 0.001f ) const;

	bool	CheckValidity();	// Check if this polygon is valid.
};

#include "Polygon.inl"

//==========================================================================

//
//	E_TraversalType
//
enum E_TraversalType
{
	Preorder,	// Visit the root. Traverse the left subtree. Traverse the right subtree.
	Postorder,	// Traverse the left subtree. Traverse the right subtree. Visit the root.
	Inorder,	// Traverse the left subtree. Visit the root. Traverse the right subtree.
};

//
//	E_NodeState
//
enum E_NodeState
{
	Unchanged,	// Node has not been modified.
	LeftChild,	// The left child has not been modified.
	RightChild,	// The right child has not been modified.
};

//
//	NodeData
//
struct NodeData
{
	// For speeding-up collision detection.
	TIndex	FirstTriangle;
	TIndex	NumTriangles;

//	E_TraversalType	eTraversalStrategy;
};

//
// Node - represents a node of a node-storing solid BSP tree.
//
 struct Node
{
	// This enum describes the allowed BSP node types.
	// NOTE: the order is important.
	enum ENodeType
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

	ENodeType	Type;	// Specifies the node's type.

	mxPlane		Plane;		// Hyperplane of the node.

	// Two pointers to children nodes.
	Node *		FrontChild;
	Node *		BackChild;

	// Extent of the node including all its children.
	// Used for frustum culling, coarse side tests, etc.
	mxBounds	Bounds;

	// List of polygons lying on this node's plane.
	Polygon *	Faces;

	UBYTE		Flags;

	NodeData	Data;
	//void *	UserPointer;

public:
	bool	IsLeaf() const;
	bool	IsInternal() const;

	bool	IsSolid() const;
	bool	IsSpace() const;

	void	SetFront( Node* child );
	void	SetBack( Node* child );

	Node * GetFront();
	Node * GetBack();

	const Node * GetFront() const;
	const Node * GetBack() const;

	Node *	GetContainingNode( const Vec3D& point );

	const mxPlane &  GetPlane() const;
	const mxBounds & GetBounds() const;
};

#include "BspNode.inl"

//========================================================================

//
//	DBspStats - for testing & debugging.
//
class DBspStats {
public:
	u32		numOrigPolygons;
	u32		numPolygons;	// number of resulting polygons
	u32		numSplits;		// number of cuts caused by BSP

	u32		numInternalNodes;
	//u32		depth;
	u32		numSolidLeaves, numEmptyLeaves;

	u32		elapsedTime;	// time required to finish task
private:
	u32		m_beginTime;	// for measuring time with timer

public:
			DBspStats();

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
//	DCsgStats - for testing & debugging.
//
class DCsgStats {
public:
			DCsgStats();

	void	Reset();
	void	Stop();

	void	Print();

	u32		GetElapsedTime() const;

public:
	u32		numRecursiveCalls;

private:
	u32		m_beginTime;	// for measuring time with timer
	u32		m_elapsedTime;	// time required to finish task
};

inline u32 DCsgStats::GetElapsedTime() const {
	return m_elapsedTime;
}

//========================================================================

//
//	NodeyBSPTree - node-storing solid BSP tree.
//
class NodeyBSPTree : public btConcaveShape {
public:
			NodeyBSPTree();
			~NodeyBSPTree();

	void		Build( const mxRenderMesh& mesh );

	void		GenerateMesh( mxRenderMesh &OutMesh );

					// Returns the bounding volume of the entire tree.
	const mxBounds & GetBounds() const;

	void		Subtract( const mxRenderMesh& geomodMesh );
	void		Union( const mxRenderMesh& geomodMesh );

	bool		TraceLine( const Vec3D& start, const Vec3D& end, Vec3D &OutResult );

	//-----------------------------------------------------------------------------------

	// overrides ( btConcaveShape )
	void processAllTriangles( btTriangleCallback* callback, const btVector3& aabbMin, const btVector3& aabbMax ) const;

	// overrides ( btCollisionShape )
	void getAabb( const btTransform& t, btVector3& aabbMin, btVector3& aabbMax ) const;
	void calculateLocalInertia( btScalar mass, btVector3& inertia ) const;

	void setLocalScaling( const btVector3& scaling );
	const btVector3 & getLocalScaling() const;

	// For debugging.
	const char * getName() const { return "BSP_Static_CollisionShape"; }
private:
	void ProcessAllTriangles_R( const Node* pNode, const AABB& aabb, btTriangleCallback* callback ) const;

private:
				// Constructs a linked list of polygons and returns the head of the list.
	Polygon *	BuildPolygonList( const mxRenderMesh& InMesh, u32 &OutNumPolys );

				// Builds a BSP tree when fed a linked list of faces.
	Node *		BuildTree_R( Polygon* pFaces );

				// Selects the best splitter polygon from the given linked list of polygons and removes the splitter from the list.
	Polygon *	SelectSplitter( Polygon *& polygons );

	void SplitFace( Polygon* inFace, const mxPlane& splitPlane,
		Polygon *& pOutFrontPolys, Polygon *& pOutBackPolys, Polygon *& pOutCoplanars );

				// Creates a new (internal) BSP node and assigns the given poly.
	Node *		AllocNode( Polygon* splitter );

				// Creates a leaf representing empty space.
	Node *		CreateEmptyLeaf();

				// Creates a leaf representing solid space.
	Node *		CreateSolidLeaf();


	bool TraceLine_R( Node* pNode, const Vec3D& start, const Vec3D& end, Vec3D &OutResult );

	void	MergeSubtract_R( Node*& pMyNode, Node* pOtherNode );
	void	MergeUnion_R( Node*& pMyNode, Node* pOtherNode );

	void PartitionNodeWithPlane( const mxPlane& splitPlane, Node* node,
		Node*& OutFront, Node*& OutBack );

	void	RemoveFacesOutsideNode( Polygon *& faces, const Node* pNode );
	void	RemoveFacesOutsideNode_R( Polygon* inFaces, const Node* pNode, Polygon *& OutFaces );

	void	RemoveFacesInsideNode( Polygon *& faces, const Node* pNode );
	void	RemoveFacesInsideNode_R( Polygon* inFaces, const Node* pNode, Polygon *& OutFaces );

	void	EmitNode_R( Node* pNode, mxRenderMesh &OutMesh ) const;

private:
	void Init();
	void Clear();

private:
	Node *		m_pRoot;	// Root node of the entire BSP tree.

	mxMemoryPool< Node >		m_nodes;
	mxMemoryPool< Polygon >		m_faces;

	bool		m_bCanDoCSG;	// True if CSG operations can be applied.

	// HACK: move render mesh into here.
	mxRenderMesh *	m_pRenderMesh;	// For speeding-up collision detection.

	// For testing & debugging.
	STATS( DBspStats  m_stats; );
	STATS( DCsgStats  m_csgStats; );
};

}//end of namespace mix

#endif /* ! __MX_DESTRUCTION_CSG_OLD_BSP_H__ */

//--------------------------------------------------------------//
//				End Of File.									//
//--------------------------------------------------------------//
