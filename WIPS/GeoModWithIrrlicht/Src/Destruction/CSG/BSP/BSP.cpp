/*
===============================================================================
	File:	BSP.cpp
	Desc:.
===============================================================================
*/

#include "stdafx.h"
#pragma hdrstop
#include <Include/IrrMix.h>

#include "BSP.h"

namespace mix {

/*================================
	BspStats
================================*/

BspStats::BspStats()
{ Reset(); }

void BspStats::Reset()
{
	MemSet( this, NULL, sizeof( BspStats ) );

	m_beginTime = GSystem->GetTime();
}

void BspStats::Stop()
{
	elapsedTime = GSystem->GetTime() - m_beginTime;
}

void BspStats::Print()
{
	GCommon->DebugPrint( "\n=== BSP statistics ========\n"			 );
	GCommon->DebugPrint( "Num. Polys:	    %d\n", numPolygons		 );
	GCommon->DebugPrint( "Num. Splits:	    %d\n", numSplits		 );
	GCommon->DebugPrint( "Num. Inner Nodes:  %d\n", numInternalNodes );
	GCommon->DebugPrint( "Num. Solid Leaves: %d\n", numSolidLeaves	 );
	GCommon->DebugPrint( "Num. Empty Leaves: %d\n", numEmptyLeaves	 );
	//GCommon->DebugPrint( "Tree Depth:        %d\n", depth			 );
	GCommon->DebugPrint( "Time elapsed:      %d\n", elapsedTime		 );
	GCommon->DebugPrint( "==== End ====================\n"			 );
}

/*================================
	BspSolid
================================*/

BspSolid::BspSolid()
{
	Clear();
}

BspSolid::~BspSolid()
{}

void BspSolid::Clear()
{
	m_root = INDEX_NONE;
	m_nodes.SetNum( 0, false /* don't resize */ );
}

void BspSolid::Build( mxRenderMesh& mesh )
{
	STATS( m_stats.Reset() );

	Clear();

	m_mesh.CreateFromTriangleMesh( mesh );
	if ( ! m_mesh.IsClosed() ) {
		GCommon->DebugPrint( "! Input mesh isn't closed.\n" );
	}

	m_mesh.CheckConsistency();

	m_nodes.Resize( m_mesh.GetNumFaces() );

	Init();
	m_root = BuildTree_R( 0 );

	STATS( m_stats.Stop(); m_stats.Print() );
}

TNodeHandle BspSolid::BuildTree_R( TFaceHandle  faceList )
{
	TFaceHandle  splitter = SelectSplitter( faceList );

	// Create a new node.
	TNodeHandle  hNewNode = m_nodes.Num();
	BspNode & rNewNode = m_nodes.Alloc();
	rNewNode.kind = BspNode::Internal;
	m_mesh.CalculatePlane( splitter, rNewNode.plane );

	rNewNode.primitives.faces = INDEX_NONE;

	rNewNode.bounds.reset( 0,0,0 );
	UpdateBounds( splitter, rNewNode.bounds );

	TFaceHandle  frontPolys = INDEX_NONE;
	TFaceHandle  backPolys = INDEX_NONE;

	TFaceHandle  currPoly = faceList;

	while ( currPoly != INDEX_NONE )
	{
		TFaceHandle  nextPoly = m_mesh.GetFace( currPoly ).next;

		SplitFace( currPoly, rNewNode.plane,
			frontPolys, backPolys, rNewNode.primitives.faces );

		UpdateBounds( rNewNode.primitives.faces, rNewNode.bounds );

		currPoly = nextPoly;
	}

	if ( frontPolys != INDEX_NONE ) {
		rNewNode.front = BuildTree_R( frontPolys );
	} else {
		rNewNode.front = GetEmptyLeaf();
	}

	if ( backPolys != INDEX_NONE ) {
		rNewNode.back = BuildTree_R( backPolys );
	} else {
		rNewNode.back = GetSolidLeaf();
	}

	// Update node bounds.
	if ( IsInternal( rNewNode.front ) ) {
		rNewNode.bounds.addInternalBox( m_nodes[ rNewNode.front ].bounds );
	}
	if ( IsInternal( rNewNode.back ) ) {
		rNewNode.bounds.addInternalBox( m_nodes[ rNewNode.back ].bounds );
	}
	return hNewNode;
}

TFaceHandle  BspSolid::SelectSplitter( TFaceHandle  faceHandle )
{
	//TODO:
	return faceHandle;
}

void BspSolid::SplitFace( TFaceHandle  inFace, const mxPlane& splitPlane,
		TFaceHandle &OutFrontPolys, TFaceHandle &OutBackPolys, TFaceHandle &OutCoplanarPolys )
{
	u32  TmpCounts[ 3 ] = { 0 };

	// Epsilon to use for polygon clipping.
	const f32  fEpsilon = PLANESIDE_EPSILON; //VECTOR_EPSILON;

	// Classify individual vertices with respect to the plane.
	LFace & rFace = m_mesh.GetFace( inFace );
	TEdgeHandle  currEdge = rFace.edge;

	u32  iVertexIndex = 0;
	do
	{
		LHalfEdge & rEdge = m_mesh.GetEdge( currEdge );

		const mxVertex & rVertex = m_mesh.GetVertex( rEdge.endVertex ).vertex;

		const f32  fDistance = splitPlane.DistanceTo( rVertex.Pos );
		TmpDists[ iVertexIndex ] = fDistance;

		if ( fDistance > +fEpsilon ) {
			TmpSides[ iVertexIndex ] = SIDE_FRONT;
		}
		else if ( fDistance < -fEpsilon ) {
			TmpSides[ iVertexIndex ] = SIDE_BACK;
		}
		else {
			TmpSides[ iVertexIndex ] = SIDE_ON;
		}
		++TmpCounts[ TmpSides[ iVertexIndex ] ];

		++iVertexIndex;

		currEdge = rEdge.next;
	}
	while ( currEdge != rFace.edge );

	// The number of vertices in the polygon.
	const u32 numVertices = iVertexIndex;
	BREAK_IF( numVertices < 3 );
	BREAK_IF( numVertices >= MAX_FACE_VERTS );

	// If the polygon is aligned with the plane.
	if ( TmpCounts[ SIDE_ON ] == numVertices ) {
		rFace.next = OutCoplanarPolys;
		OutCoplanarPolys = inFace;
		return; // SIDE_ON
	}

	// If the polygon is completely behind the plane.
	if ( TmpCounts[ SIDE_FRONT ] == 0 ) {
		rFace.next = OutBackPolys;
		OutBackPolys = inFace;
		return; // SIDE_BACK
	}

	// If the polygon is completely in front of the plane.
	if ( TmpCounts[ SIDE_BACK ] == 0 ) {
		rFace.next = OutFrontPolys;
		OutFrontPolys = inFace;
		return; // SIDE_FRONT
	}

	TmpSides[ numVertices ] = TmpSides[ 0 ];
	TmpDists[ numVertices ] = TmpDists[ 0 ];

	// Straddles the plane - we must clip.
	const u32  maxPoints = numVertices + 4; // Estimated number of points.

//????


/*
	TFaceHandle  frontPoly = m_mesh.GetFaces().Alloc();
	TFaceHandle  backPoly = m_mesh.GetFaces().Alloc();

	for ( u32 i = 0; i < numVertices; i++ )
	{
		TVertex v1 = verts[ i ];
		if ( TmpSides[i] == SIDE_ON )
		{
			AddVertex( frontPoly, v1 );
			AddVertex( backPoly, v1 );
			continue;
		}
		if ( TmpSides[i] == SIDE_FRONT ) {
			frontPoly->AddVertex( v1 );
		}
		if ( TmpSides[i] == SIDE_BACK ) {
			backPoly->AddVertex( v1 );
		}

		if ( (TmpSides[i+1] == SIDE_ON) || (TmpSides[i+1] == TmpSides[i]) ) {
			continue;
		}

		// Generate a split point.
		TVertex  mid;

		const BspVertex & v2 = face->GetVertex( (i+1) % face->NumVertices() );

#ifdef FAST_CSG

		const f32 fraction = plane.getKnownIntersectionWithLine( v1.Pos, v2.Pos );
		mid.Interpolate( v1, v2, fraction );

#else // SLOW_PRECISE
		Unimplemented();
#endif

		frontPoly->AddVertex( mid );
		backPoly->AddVertex( mid );
	}

#ifdef MX_DEBUG
	if ( frontPoly->NumVertices() > maxPoints
		|| backPoly->NumVertices() > maxPoints )
	{
		::mix::GCommon->Error( "ClipFace: number of points exceeded estimate" );
	}
#endif // MX_DEBUG

	frontPoly.next = OutFrontPolys;
	OutFrontPolys = frontPoly;

	backPoly.next = OutBackPolys;
	OutBackPolys = backPoly;
*/
	//return SIDE_CROSS;
}

void BspSolid::Init()
{	Assert( m_nodes.Num() == 0 );

	BspNode		emptyLeaf;
	emptyLeaf.kind = BspNode::OutCell;
	m_nodes.Append( emptyLeaf );

	BspNode		solidLeaf;
	solidLeaf.kind = BspNode::OutCell;
	m_nodes.Append( solidLeaf );
}

// OutFaceBounds += inFace.GetBounds()
//
void BspSolid::UpdateBounds( TFaceHandle  inFace, mxBounds &OutFaceBounds )
{
	BREAK_IF( inFace == INDEX_NONE );

	LFace & rFace = m_mesh.GetFace( inFace );
	TEdgeHandle  currEdge = rFace.edge;
	do {
		LHalfEdge & rEdge = m_mesh.GetEdge( currEdge );

		OutFaceBounds.addInternalBox( m_mesh.GetVertex( rEdge.endVertex ).vertex.Pos );

		currEdge = rEdge.next;
	} while ( currEdge != rFace.edge );
}

const mxBounds & BspSolid::GetBounds() const
{
	BREAK_IF( m_root == INDEX_NONE );
	return m_nodes[ m_root ].bounds;
}

bool BspSolid::TraceLine( const Vec3D& start, const Vec3D& end, Vec3D &OutResult )
{
	if ( TraceLine_R( m_root, start, end, OutResult ) )
	{
//		::mix::GRenderWorld->CreatePermMark( OutResult );

#if 1 // TEST CODE:
	// Loop around the hit face and mark all vertices around the face.
	{
		LFace & rFace = m_mesh.GetFace( TmpLastHitFace );
		TEdgeHandle  currEdge = rFace.edge;
		do {
			LHalfEdge & rEdge = m_mesh.GetEdge( currEdge );

//			::mix::GRenderWorld->CreatePermMark( m_mesh.GetVertex( rEdge.endVertex ).vertex.Pos, E_Colour::Color_Yellow );

			currEdge = rEdge.next;
		} while ( currEdge != rFace.edge );
	}
#endif

		return true;
	}
	return false;
}

bool BspSolid::TraceLine_R( TNodeHandle  hNode, const Vec3D& start, const Vec3D& end, Vec3D &OutResult )
{
	if ( IsLeaf( hNode ) ) {
		return false;
	}

	const BspNode & rNode = GetNode( hNode );
	const f32  dStart = rNode.plane.DistanceTo( start );
	const f32  dEnd = rNode.plane.DistanceTo( end );

	if ( dStart > 0 && dEnd > 0 ) {
		return TraceLine_R( rNode.front, start, end, OutResult );
	}
	else if ( dStart < 0 && dEnd < 0 ) {
		return TraceLine_R( rNode.back, start, end, OutResult );
	}

	// The line spans the splitting plane.

	// Variables to hold which side to traverse first and which second.
	TNodeHandle  firstSide = INDEX_NONE;
	TNodeHandle  secondSide = INDEX_NONE;

	// If we know that one is positive and the other is negative, and dStart < dEnd, so
	// dStart is negative and dEnd is positive.
	if ( dStart < dEnd )
	{
		firstSide = rNode.back;	// The back side contains the start point so the back will be the first.
		secondSide = rNode.front;
	}
	// If dEnd is negative and dStart is positive.
	else if ( dEnd < dStart )
	{
		firstSide = rNode.front;	// The front side contains the start point so the front will be the first.
		secondSide = rNode.back;
	}
	else {
		firstSide = rNode.front;
		secondSide = rNode.back;
	}

	Vec3D  midPoint;	// intersection
	rNode.plane.getIntersectionWithLimitedLine( start, end, midPoint );

	// Check the first side.
	if ( TraceLine_R( firstSide, start, midPoint, OutResult ) ) {
		return true;
	}

	{
		TFaceHandle  currPoly = rNode.primitives.faces;
		while ( currPoly != INDEX_NONE )
		{
			if ( IsPointInsideFace( midPoint, currPoly ) )
			{
				OutResult = midPoint;
#if 1	// TEST CODE:
	TmpLastHitFace = currPoly;
#endif
				return true;
			}
			currPoly = m_mesh.GetFace( currPoly ).next;
		}
	}

	// Check the second side.
	return TraceLine_R( secondSide, midPoint, end, OutResult );
}

bool BspSolid::IsPointInsideFace( const Vec3D& point, TFaceHandle  hFace, const f32 epsilon /* = 0.001f */ )
{
	const LFace & rFace = m_mesh.GetFace( hFace );
	TEdgeHandle  currEdge = rFace.edge;
	u32 iVertexIndex = 0;
	do {
		LHalfEdge & rEdge = m_mesh.GetEdge( currEdge );

		TmpVectors[ iVertexIndex++ ] = m_mesh.GetVertex( rEdge.endVertex ).vertex.Pos;

		currEdge = rEdge.next;
	} while ( currEdge != rFace.edge );

	const u32  numVerts = iVertexIndex;

	// Get the first normal to test.
	Vec3D   toPt = point - TmpVectors[ 0 ];
	Vec3D   edge = TmpVectors[1] - TmpVectors[0];
	Vec3D   testCross = edge.crossProduct( toPt );
	//testCross.normalize();

	for ( u32 i = 0; i < numVerts; i++ )
	{
		toPt = point - TmpVectors[ i ];
		edge = TmpVectors[ (i+1) % numVerts ] - TmpVectors[ i ];
		Vec3D   cross = edge.crossProduct( toPt );
		//cross.normalize();

		if ( cross.dotProduct( testCross ) < epsilon ) {
			return false;
		}
	}
	return true;
}

void BspSolid::Merge_Subract( mxGeoModMesh& rGeoModMesh )
{	Assert( m_root != INDEX_NONE );

	DEBUG_CODE( u32 beginTime = GSystem->GetTime(); )

//	MergeSubtract_R( m_root, rGeoModMesh.m_root );

	DEBUG_CODE( u32 endTime = GSystem->GetTime();
		GCommon->Print( "- CSG: %u polys + %u polys, %ums\n",
			m_mesh.GetNumFaces(), rGeoModMesh.m_mesh.GetNumFaces(), endTime-beginTime ); )
}

void BspSolid::GenerateMesh( mxRenderMesh &OutMesh )
{
}

void BspSolid::MergeSubtract_R( TNodeHandle & hNode0, TNodeHandle  hNode1 )
{
	BREAK_IF( hNode0 == INDEX_NONE || hNode1 == INDEX_NONE );
/*
	if ( IsSolidLeaf( hNode0 ) )
	{
		ReindexTree( hNode1 );
		hNode0 = hNode1;
	}
	else if ( IsEmptyLeaf( hNode0 ) )
	{
		Delete( hNode1 );
	}
	else // hNode0 is an internal partitioning node
	{
		BspNode & rNode = GetNode( hNode0 );
		const E_PlaneSide  eRelation = rNode.plane.ClassifyVolumeRelation( GetBounds( hNode1 ) );

		if ( eRelation == SIDE_FRONT ) {
			MergeSubtract_R( rNode.front, hNode1 );
		}
		else if ( eRelation == SIDE_BACK ) {
			MergeSubtract_R( rNode.back, hNode1 );
		}
		else // SIDE_CROSS
		{
			// Partition the other tree and merge with the corresponding pieces.
			TNodeHandle  hT2_partitioned_Front = INDEX_NONE;
			TNodeHandle  hT2_partitioned_Back = INDEX_NONE;

			SplitNode( rNode, hNode1, hT2_partitioned_Front, hT2_partitioned_Back );

			MergeSubtract_R( rNode.front, hT2_partitioned_Front );
			MergeSubtract_R( rNode.back, hT2_partitioned_Back );
		}
	}
*/
}

#ifdef BULLET_PHYSICS

//void BspSolid::processAllTriangles( btTriangleCallback* callback, const btVector3& aabbMin, const btVector3& aabbMax ) const
//{
//	Unimplemented();
//}

#endif

/*================================
	mxGeoModMesh
================================*/

mxGeoModMesh::mxGeoModMesh()
{}

mxGeoModMesh::~mxGeoModMesh()
{}

void mxGeoModMesh::Build( const mxRenderMesh& inMesh )
{
	m_mesh.CreateFromTriangleMesh( inMesh );
}

}//end of namespace mix

//--------------------------------------------------------------//
//				End Of File.									//
//--------------------------------------------------------------//
