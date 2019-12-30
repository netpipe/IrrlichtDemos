/*
===============================================================================
	File:	TRenderMesh.h
	Desc:
===============================================================================
*/

#ifndef __MX_MESH_H__
#define __MX_MESH_H__

namespace mix {

//
//	mxVertex
//
//	Used for drawing.
//
//	36 bytes
//
class mxVertex {
public:
	Vec3D		Pos;
	Vec3D		Normal;
	TColorARGB	Color;	// Four 8-bit color values for alpha, red, green, and blue.
	f32			U, V;	// Texture coordinates.

public:
	mxVertex()
		: Color(255,255,255,255)
	{
	}

	mxVertex( const Vec3D& position, const Vec3D& normal, const f32 u, const f32 v, TColorARGB color = TColorARGB(255,255,255,255) )
		: Pos( position )
		, Normal( normal )
		, U( u ), V( v )
		, Color( color )
	{
	}

	mxVertex( const TVertex& renderVertex )
	{
		Set( renderVertex );
	}

	void Set( const TVertex& renderVertex )
	{
		MemCopy( this, & renderVertex, sizeof( mxVertex ) );
	}

	void Interpolate( const mxVertex& v1, const mxVertex& v2, const f32 amount = 0.5f )
	{	// interpolate vertex attributes
		Lerp< Vec3D >( Pos, v1.Pos, v2.Pos, amount );
		Lerp< Vec3D >( Normal, v1.Normal, v2.Normal, amount );

		//::Interpolate< u32 >( Color, v1.Color, v2.Color, amount );

		Lerp< f32 >( U, v1.U, v2.U, amount );
		Lerp< f32 >( V, v1.V, v2.V, amount );
	}
};

STATIC_CHECK( sizeof(mxVertex) == sizeof(TVertex), size_of_bsp_vertex_must_equal_size_of_render_vertex );

//------------------------------------------------------------------------------------------------------

typedef u16  TIndex;	// only 16-bit index buffers are supported

typedef TTriangle< TIndex >  IndexTriple;	// Contains indices into an array of vertices.

//
//	Edge
//
struct Edge
{
	TIndex   iA, iB;	// startPoint and endPoint indices into vertex array

	Edge()
	{}

	Edge( TIndex A, TIndex B )
		: iA( A ), iB( B )
	{}
};

inline bool operator == ( const Edge& obj1, const Edge& obj2 )
{
	return (obj1.iA == obj2.iA) && (obj1.iB == obj2.iB);
}

//
//	EdgeHashProvider
//
class EdgeHashProvider {
public:
	static inline u32 GetHash( const Edge& edge )
	{
		return 19 * edge.iA + 7 * edge.iB;
	}
};

//
//	mxRenderMesh - a simple triangle based mesh for rendering.
//
class mxRenderMesh {
public:
			// Resizes arrays of vertices and triangles.
			mxRenderMesh( u32 numVerts = 0, u32 numTris = 0 );
			~mxRenderMesh();

			// Returns the number of triangles in the given mesh.
	u32		CopyFromMesh( const ::irr::scene::IMesh* mesh );
	void	Copy( const mxRenderMesh& other );

	void	Render() const;

	u32		GetTriangleCount() const
	{ return Triangles.Num(); }

	u32		GetVertexCount() const
	{ return Vertices.Num(); }

	const Vec3D & GetPos( u32 index ) const
	{ return Vertices[ index ].Pos; }

			// Delete all vertices and indices. Free alloced memory.
	void	Clear();

			// Remove all vertices and indices. Doesn't free allocated memory.
	void	RemoveAll();

			// Brute force method to calculate AABB.
	void	ComputeBoundingBox( AABB &bbox ) const;

	void	ScaleVertices( const f32 value );
	void	ScaleVertices( const Vec3D& value );
	void	TransformVertices( const TMatrix4& matrix );

	void	ScaleTextureCoordinates( const f32 factorU, const f32 factorV );


			// For debugging.

			// Extremely slow function!
	void	DrawWireframe() const;

			// Welds vertices that are close together.
	void	WeldVertices();

public:
	// Geometry for rendering.
	TArray< mxVertex >		Vertices;	// Array of vertices that the triangles can index into.
	TArray< IndexTriple >	Triangles;	// Array of triangles.  The triangles are triples of ints that are indices into the vertices array.
};

//======================================================================================

typedef short		TVertexHandle;
typedef short		TEdgeHandle;
typedef short		TFaceHandle;

//
//	LVertex
//
class LVertex {
public:
	mxVertex		vertex;			// actual vertex data
	TEdgeHandle		edge;			// one of the half edges emanating from this vertex

	LVertex()
	{}

	LVertex( const mxVertex& renderVertex )
		: vertex( renderVertex )
		, edge( INDEX_NONE )
	{}
};

//	A half-edge is a data structure that holds polygonal manifolds (a polygonal mesh without wedges or pinched vertices)
//	and allows efficient queries regarding the connectivity of the manifold.
//	For example, you can ask which edges are adjacent to a vertex, which faces are on either side of an edge,
//	you can traverse the edges around a face in counterclockwise order, etc.
//
class LHalfEdge {
public:
	TEdgeHandle		next;		// points to next edge (CCW order) which is adjacent to the current one
	TEdgeHandle		twin;		// opposite edge
	TFaceHandle		face;		// face on the left
	TVertexHandle	endVertex;	// end point for half edge

public:
	LHalfEdge()
	{
		MemSet( this, INDEX_NONE, sizeof( *this ) );
	}
};

//
//	LFace - a simple 3D convex face.
//
class LFace {
public:
	// Polygon flags.
	enum E_Flags {
		IsSelected		= BIT( 0 )	// This polygon has been selected as a splitter ( used when building a BSP tree ).
	};

	Vec3D			normal;

	TEdgeHandle		edge;	// One of the half edges bordering this face

	TFaceHandle		next;	// Next face in a linked list, used for creating lists

	u32		flags;

public:
	LFace()
	{
		MemSet( this, NULL, sizeof( *this ) );
		edge = INDEX_NONE;
		next = INDEX_NONE;
	}
};

//
//	mxDynamicMesh - mesh with topology (connectivity) information.
//
//	For GeoMod, input meshes should be closed, 2-manifold surfaces, i.e. they must
//	have the topology of a sphere.
//
class mxDynamicMesh {
public:
			mxDynamicMesh();
			~mxDynamicMesh();

	void	CreateFromTriangleMesh( const mxRenderMesh& mesh );

			// Traverse the mesh and determine if the mesh is closed.
	bool	IsClosed() const;

	u32		GetNumFaces() const;

	TArray< LVertex > &		GetVerts();
	TArray< LHalfEdge > &	GetEdges();
	TArray< LFace > &		GetFaces();

	LVertex &		GetVertex( const TVertexHandle hVertex );
	LHalfEdge &		GetEdge( const TEdgeHandle hEdge );
	LFace &			GetFace( const TFaceHandle hFace );

	void	CalculatePlane( TFaceHandle  inPoly, TPlane3D &OutPlane );

			// For debugging.
	void	CheckConsistency();

			// Check whether the face is valid or not.
	bool	CheckFace( TFaceHandle  hFace );

private:
	// Create a new half edge if necessary.
	TEdgeHandle  CreateEdge( const Edge& edge );

private:
	TArray< LVertex >		m_verts;
	TArray< LHalfEdge >		m_edges;
	TArray< LFace >			m_faces;

private:
	// TODO: move this into another place.
	// Used when converting triangle mesh into dynamic mesh.
	struct Pair {
		Edge			edge;
		TEdgeHandle		halfEdge;

		Pair()
		{}

		Pair( const Edge& inEdge, const TEdgeHandle inHalfEdge )
			: edge( inEdge ), halfEdge( inHalfEdge )
		{}
	};
	TArray< Pair >		m_pairs;
};

inline u32 mxDynamicMesh::GetNumFaces() const {
	return m_faces.Num();
}

inline TArray< LVertex > & mxDynamicMesh::GetVerts() {
	return m_verts;
}

inline TArray< LHalfEdge > & mxDynamicMesh::GetEdges() {
	return m_edges;
}

inline TArray< LFace > & mxDynamicMesh::GetFaces() {
	return m_faces;
}

inline LVertex & mxDynamicMesh::GetVertex( const TVertexHandle hVertex ) {
	return m_verts[ hVertex ];
}

inline LHalfEdge & mxDynamicMesh::GetEdge( const TEdgeHandle hEdge ) {
	return m_edges[ hEdge ];
}

inline LFace & mxDynamicMesh::GetFace( const TFaceHandle hFace ) {
	return m_faces[ hFace ];
}

//
//	mxMeshTools
//
class mxMeshTools {
public:
	static void		MakeConsistent( mxRenderMesh* triangleMesh );

	static void		LoadMesh( const char* filename, mxDynamicMesh *OutMesh );
	static void		SaveMesh( const char* filename, mxDynamicMesh* InMesh );
};

}//end of namespace mix

#endif // ! __MX_MESH_H__

//--------------------------------------------------------------//
//				End Of File.									//
//--------------------------------------------------------------//
