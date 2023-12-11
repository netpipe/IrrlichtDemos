/*
===============================================================================
	File:	TRenderMesh.cpp
	Desc:
===============================================================================
*/

//#include "stdafx.h"
#pragma hdrstop
#include "../Include/IrrMix.h"

namespace mix {

/*==============================
	class mxRenderMesh
==============================*/

mxRenderMesh::mxRenderMesh( u32 numVerts /* =0 */, u32 numTris /* =0 */ )
{
	if ( numVerts ) {
		Vertices.Resize( numVerts );
	}
	if ( numTris ) {
		Triangles.Resize( numTris );
	}
}

mxRenderMesh::~mxRenderMesh()
{
	Clear();
}

u32 mxRenderMesh::CopyFromMesh( const ::irr::scene::IMesh* mesh )
{
	if ( mesh->getMeshBufferCount() > 0 )
	{
		if ( mesh->getMeshBufferCount() > 1 ) {
			Unimplemented2( "multiple mesh buffers" );
		}

		::irr::scene::IMeshBuffer * pBuf = mesh->getMeshBuffer( 0 );
		Assert( pBuf );

		Assert2( pBuf->getIndexType() == ::irr::video::E_INDEX_TYPE::EIT_16BIT, "only 16-bit indices are supported !" );
		Assert2( pBuf->getVertexType() == ::irr::video::E_VERTEX_TYPE::EVT_STANDARD, "only standard vertex type is supported !" );

		const TVertex *	pVertices = ( TVertex* ) pBuf->getVertices();
		const u32  vertexCount = pBuf->getVertexCount();

		Vertices.SetNum( vertexCount );
		MemCopy( Vertices.Ptr(), pVertices, vertexCount * sizeof( TVertex ) );

		const TIndex * pIndices = pBuf->getIndices();
		const u32  triangleCount = pBuf->getIndexCount() / 3;

		Triangles.SetNum( triangleCount );
		MemCopy( Triangles.Ptr(), pIndices, pBuf->getIndexCount() * sizeof( TIndex ) );

	//	GCommon->DebugPrint( "* Loaded mesh: %u verts, %u tris\n", vertexCount, triangleCount );

		return triangleCount;
	}
	return 0;
}

void mxRenderMesh::Copy( const mxRenderMesh& other )
{
	Vertices = other.Vertices;
	Triangles = other.Triangles;
}

void mxRenderMesh::Render() const
{
///	GRenderWorld->Driver->drawIndexedTriangleList(
///		( const TVertex* ) Vertices.Ptr(),
///		Vertices.Num(),
///		( const u16* ) Triangles.Ptr(),
///		Triangles.Num()
///	);
}

void mxRenderMesh::ComputeBoundingBox( AABB &bbox ) const
{
	bbox.reset( 0.0f, 0.0f, 0.0f );

	for ( u32 i = 0; i < GetVertexCount(); i++ )
	{
		bbox.addInternalPoint( GetPos( i ) );
	}
}

void mxRenderMesh::Clear()
{
	Vertices.Clear();
	Triangles.Clear();
}

void mxRenderMesh::RemoveAll()
{
	Vertices.SetNum( 0, false /* don't resize */ );
	Triangles.SetNum( 0, false /* don't resize */ );
}

void mxRenderMesh::ScaleVertices( const f32 value )
{
	for ( u32 i = 0; i < GetVertexCount(); ++i )
	{
		Vertices[ i ].Pos *= value;
	}
}

void mxRenderMesh::ScaleVertices( const Vec3D& value )
{
	for ( u32 i = 0; i < GetVertexCount(); ++i )
	{
		Vertices[ i ].Pos *= value;
	}
}

void mxRenderMesh::TransformVertices( const TMatrix4& matrix )
{
	for ( u32 i = 0; i < GetVertexCount(); ++i )
	{
		matrix.transformVect( Vertices[ i ].Pos );
	}
}

void mxRenderMesh::ScaleTextureCoordinates( const f32 factorU, const f32 factorV )
{
	for ( u32 i = 0; i < GetVertexCount(); ++i )
	{
		Vertices[ i ].U *= factorU;
		Vertices[ i ].V *= factorV;
	}
}

// Very slow! Should only be used for debugging.
//
void mxRenderMesh::DrawWireframe() const
{
	for ( u32 i = 0; i < Triangles.Num(); i++ )
	{
		const IndexTriple & rTri = Triangles[ i ];

///		GRenderWorld->Driver->draw3DTriangle(
///			::irr::core::triangle3df( GetPos( rTri.iA ), GetPos( rTri.iB ), GetPos( rTri.iC ) )
///		);
	}
}

// TODO: exploit a better algorithm, use vertex hash table/map.
//
void mxRenderMesh::WeldVertices()
{
	TArray< mxVertex >	newVerts;
	newVerts.Resize( GetVertexCount() );

	for ( u32 triIdx = 0; triIdx < GetTriangleCount(); triIdx++ )
	{
		IndexTriple & rTri = Triangles[ triIdx ];

		for ( TIndex i = 0; i < 3; i++ )
		{
			const mxVertex & rCurrVertex = Vertices[ rTri[i] ];

			s32  iExistingVertex = -1;
			for ( TIndex iVtx = 0; iVtx < newVerts.Num(); iVtx++ )
			{
				if ( VectorsEqual( newVerts[ iVtx ].Pos, rCurrVertex.Pos ) )
				{
					iExistingVertex = iVtx;
					break;
				}
			}

			if ( iExistingVertex != -1 ) {
				rTri[ i ] = iExistingVertex;
			} else {
				rTri[ i ] = newVerts.Append( mxVertex( rCurrVertex ) );
			}
		}
	}

	Vertices = newVerts;
}

/*================================
	class mxDynamicMesh
================================*/

mxDynamicMesh::mxDynamicMesh()
{
}

mxDynamicMesh::~mxDynamicMesh()
{
}

// Build a half-edge mesh from the given triangle mesh.
// Also builds a linked list of faces. The first face is the head of the list.
//
void mxDynamicMesh::CreateFromTriangleMesh( const mxRenderMesh& mesh )
{
	// stats
	DEBUG_CODE(	u32  beginTime = GSystem->GetTime(); )

	m_verts.Resize( mesh.GetVertexCount() );

	for ( u32 i = 0; i < mesh.GetVertexCount(); i++ )
	{
		new (m_verts) LVertex( mesh.Vertices[i] );
	}

	m_pairs.Clear();

	m_edges.Resize( mesh.GetTriangleCount() * 3 );

	m_faces.Resize( mesh.GetTriangleCount() );
	for ( u32 triIdx = 0; triIdx < mesh.GetTriangleCount(); triIdx++ )
	{
		const IndexTriple &  rTri = mesh.Triangles[ triIdx ];

		Edge  edge1( rTri.iA, rTri.iB );
		Edge  edge2( rTri.iB, rTri.iC );
		Edge  edge3( rTri.iC, rTri.iA );

		TEdgeHandle  pHalfEdge1 = CreateEdge( edge1 );
		TEdgeHandle  pHalfEdge2 = CreateEdge( edge2 );
		TEdgeHandle  pHalfEdge3 = CreateEdge( edge3 );

		m_edges[ pHalfEdge1 ].next = pHalfEdge3;
		m_edges[ pHalfEdge3 ].next = pHalfEdge2;
		m_edges[ pHalfEdge2 ].next = pHalfEdge1;

		TFaceHandle  newFace = m_faces.Append( LFace() );

		m_edges[ pHalfEdge1 ].face = newFace;
		m_edges[ pHalfEdge2 ].face = newFace;
		m_edges[ pHalfEdge3 ].face = newFace;

		LFace & rNewFace = m_faces[ newFace ];
		rNewFace.edge = pHalfEdge1;

		rNewFace.normal = TPlane3D(
				GetVertex( rTri.iA ).vertex.Pos,
				GetVertex( rTri.iB ).vertex.Pos,
				GetVertex( rTri.iC ).vertex.Pos
			).Normal;
	}

	// stats
	//DEBUG_CODE(
	//	u32  endTime = GSystem->GetTime();
	//	GCommon->DebugPrint( "* TriMesh->DynamicMesh: %u verts, %u edges, %u faces in %u ms\n",
	//		m_verts.Num(), m_edges.Num(), m_faces.Num(), endTime - beginTime );
	//	GCommon->DebugPrint( "* Memory footprint: %u KBytes\n", (m_verts.Allocated() + m_edges.Allocated() + m_faces.Allocated()) / 1000 );
	//)//Debug

	// Build a linked list of faces.
	// ( Needed for BSP tree construction. )
	for ( u32 i = 0; i < m_faces.Num(); i++ ) {
		m_faces[i].next = i + 1;
	}
	m_faces.GetLast().next = INDEX_NONE;
}

TEdgeHandle  mxDynamicMesh::CreateEdge( const Edge& edge )
{
	// Check if a half-edge corresponding to the edge already exists.
	for ( u32 i = 0; i < m_pairs.Num(); i++ )
	{
		if ( m_pairs[i].edge == edge ) {
			return m_pairs[i].halfEdge;
		}
	}
	// Create a new half-edge.
	TEdgeHandle  newHalfEdge = m_edges.Append( LHalfEdge() );
	m_pairs.Append( Pair( edge, newHalfEdge ) );

	m_verts[ edge.iA ].edge = newHalfEdge;
	m_edges[ newHalfEdge ].endVertex = edge.iB;

	// If there's an opposite edge then hook both half-edges together.
	Edge  oppositeEdge( edge.iB, edge.iA );
	for ( u32 i = 0; i < m_pairs.Num(); i++ )
	{
		if ( m_pairs[i].edge == oppositeEdge ) {
			m_edges[newHalfEdge].twin = i;
			m_edges[i].twin = newHalfEdge;
		}
	}
	return newHalfEdge;
}

bool mxDynamicMesh::IsClosed() const
{
	for ( u32 iEdgeIndex = 0; iEdgeIndex < m_edges.Num(); iEdgeIndex++ )
	{
		if ( m_edges[ iEdgeIndex ].twin == INDEX_NONE ) {
			return false;
		}
	}
	return true;
}

void mxDynamicMesh::CalculatePlane( TFaceHandle  inPoly, TPlane3D &OutPlane )
{
	LFace & rFace = m_faces[ inPoly ];

#ifdef FAST_CSG
	// Consider only the first three vertices of the face.
	const u32 maxNumVerts = 3;

	TEdgeHandle  hEdge = rFace.edge;
	const LHalfEdge & rEdge0 = m_edges[ hEdge ];
	TVertexHandle  hVertex0 = rEdge0.endVertex;

	const LHalfEdge & rEdge1 = m_edges[ rEdge0.next ];
	TVertexHandle  hVertex1 = rEdge1.endVertex;

	const LHalfEdge & rEdge2 = m_edges[ rEdge1.next ];
	TVertexHandle  hVertex2 = rEdge2.endVertex;

	OutPlane.setPlane(
		m_verts[ hVertex0 ].vertex.Pos,
		m_verts[ hVertex1 ].vertex.Pos,
		m_verts[ hVertex2 ].vertex.Pos );

#else // PRECISE_SLOW

	// Loop through all vertices of the face and compute the plane.
	Unimplemented();
	TEdgeHandle  currEdge = rFace.edge;
	do {
		LHalfEdge & rEdge = m_edges[ currEdge ];

		= m_verts[ rEdge.endVertex ].vertex;

		currEdge = rEdge.next;
	} while ( currEdge != rFace.edge );
#endif
}

void mxDynamicMesh::CheckConsistency()
{
	// Each face has >= 3 vertices.
	for ( u32 iFaceIndex = 0; iFaceIndex < GetNumFaces(); iFaceIndex++ )
	{
		LFace & rFace = m_faces[ iFaceIndex ];
		TEdgeHandle  hEdge = rFace.edge;

		u32 numVerts = 0;
		do {
			LHalfEdge & rEdge = m_edges[ hEdge ];
			++numVerts;
			hEdge = rEdge.next;
		} while ( hEdge != rFace.edge );

		if ( numVerts < 3 ) {
		//	GCommon->DebugPrint( "* !Mesh is invalid!\n" );
		}
	}
}

}//end of namespace mix

//--------------------------------------------------------------//
//				End Of File.									//
//--------------------------------------------------------------//
