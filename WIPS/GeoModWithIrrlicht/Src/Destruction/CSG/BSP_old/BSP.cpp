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
		Polygon
================================*/

void Polygon::AddToBounds( mxBounds &OutBounds ) const
{
	for ( u32 i = 0; i < NumVertices(); i++ )
	{
		OutBounds.addInternalPoint( GetPos( i ) );
	}
}

bool Polygon::PointInside( const Vec3D& point, const f32 epsilon /* = 0.001f */ ) const
{
	// Get the first normal to test.
	Vec3D   toPt = point - GetPos( 0 );
	Vec3D   edge = GetPos( 1 ) - GetPos( 0 );
	Vec3D   testCross = edge.crossProduct( toPt );
	//testCross.normalize();

	for ( u32 i = 0; i < NumVertices(); i++ )
	{
		toPt = point - GetPos( i );
		edge = GetPos( (i+1) % NumVertices() ) - GetPos( i );
		Vec3D   cross = edge.crossProduct( toPt );
		//cross.normalize();

		if ( cross.dotProduct( testCross ) < epsilon ) {
			return false;
		}
	}
	return true;
}

/*================================
			Node
================================*/

Node * Node::GetContainingNode( const Vec3D& point )
{
	E_PlaneSide  eRelation = Plane.ClassifyPointRelation( point );
/*
	if ( Type == ENodeType::Internal )
	{
		if ( eRelation == SIDE_BACK ) {
			return BackChild->GetContainingNode( point );
		}
		if ( eRelation == SIDE_FRONT ) {
			return FrontChild->GetContainingNode( point );
		}
	}
	*/
    return this;
}

/*================================
		DBspStats
================================*/

DBspStats::DBspStats()
{ Reset(); }

void DBspStats::Reset()
{
	MemSet( this, 0, sizeof( DBspStats ) );

	m_beginTime = GSystem->GetTime();
}

void DBspStats::Stop()
{
	elapsedTime = GSystem->GetTime() - m_beginTime;
}

void DBspStats::Print()
{
	GCommon->DebugPrint( "\n=== BSP statistics ========\n"			 );
	GCommon->DebugPrint( "Num. Polys(Begin): %d\n", numOrigPolygons	 );
	GCommon->DebugPrint( "Num. Polys(End):   %d\n", numPolygons		 );
	GCommon->DebugPrint( "Num. Splits:	     %d\n", numSplits		 );
	GCommon->DebugPrint( "Num. Inner Nodes:  %d\n", numInternalNodes );
	GCommon->DebugPrint( "Num. Solid Leaves: %d\n", numSolidLeaves	 );
	GCommon->DebugPrint( "Num. Empty Leaves: %d\n", numEmptyLeaves	 );
	//GCommon->DebugPrint( "Tree Depth:        %d\n", depth			 );
	GCommon->DebugPrint( "Time elapsed:      %d\n", elapsedTime		 );
	GCommon->DebugPrint( "==== End ====================\n"			 );
}

/*================================
		DCsgStats
================================*/

DCsgStats::DCsgStats()
{ Reset(); }

void DCsgStats::Reset()
{
	MemSet( this, 0, sizeof( DBspStats ) );

	m_beginTime = GSystem->GetTime();
}

void DCsgStats::Stop()
{
	m_elapsedTime = GSystem->GetTime() - m_beginTime;
}

void DCsgStats::Print()
{
	GCommon->DebugPrint( "\n=== CSG statistics ========\n"			 );
	GCommon->DebugPrint( "Num. Calls:	     %d\n", numRecursiveCalls);
	GCommon->DebugPrint( "Time elapsed:      %d\n", m_elapsedTime	 );
	GCommon->DebugPrint( "==== End ====================\n"			 );
}

/*================================
		NodeyBSPTree
================================*/

NodeyBSPTree::NodeyBSPTree()
{
	Init();
}

NodeyBSPTree::~NodeyBSPTree()
{
	Clear();
}

void NodeyBSPTree::Init()
{
	Clear();
	m_bCanDoCSG = true;
	m_pRenderMesh = NULL;

	//************
//	m_shapeType = CUSTOM_CONCAVE_SHAPE_TYPE;
}

void NodeyBSPTree::Clear()
{
	m_pRoot = NULL;
	m_nodes.Clear();
	m_faces.Clear();
}

void NodeyBSPTree::Build( const mxRenderMesh& mesh )
{
	STATS( m_stats.Reset() );

	Clear();

	u32  numPolys = 0;
	Polygon * faces = BuildPolygonList( mesh, numPolys );

	STATS( m_stats.numOrigPolygons = numPolys );
	STATS( m_stats.numPolygons = numPolys );

	//m_nodes.Resize( m_mesh.GetNumFaces() );
	m_pRoot = BuildTree_R( faces );

	STATS( m_stats.Stop(); m_stats.Print() );
}

Polygon * NodeyBSPTree::BuildPolygonList( const mxRenderMesh& InMesh, u32 &OutNumPolys )
{
	Polygon * prevFace = NULL;

	for ( u32 i = 0; i < InMesh.GetTriangleCount(); i++ )
	{
		Polygon * pNewFace = new (m_faces) Polygon();

		++OutNumPolys;

		const IndexTriple & tri = InMesh.Triangles[ i ];

		pNewFace->AddVertex( InMesh.Vertices[ tri.Points[0] ] );
		pNewFace->AddVertex( InMesh.Vertices[ tri.Points[1] ] );
		pNewFace->AddVertex( InMesh.Vertices[ tri.Points[2] ] );

		// Construct polygon plane and normal.
		pNewFace->Plane.Set( TPlane3D( InMesh.GetPos( tri.Points[0] ),
			InMesh.GetPos( tri.Points[1] ), InMesh.GetPos( tri.Points[2] ) )
		);

		pNewFace->SetNext( prevFace );
		prevFace = pNewFace;
	}
	return prevFace;
}

Node * NodeyBSPTree::BuildTree_R( Polygon* pFaces )
{
	Polygon * splitter = SelectSplitter( pFaces );

	Node * pNewNode = AllocNode( splitter );

	Polygon * pFrontPolys = NULL;
	Polygon * pBackPolys = NULL;

	Polygon * pCurrPoly = pFaces;

	while ( pCurrPoly != NULL )
	{
		Polygon * pNextPoly = pCurrPoly->GetNext();

		SplitFace( pCurrPoly, pNewNode->Plane,
			pFrontPolys, pBackPolys, pNewNode->Faces );

		// Update the node's bounding volume.
		pNewNode->Faces->AddToBounds( pNewNode->Bounds );

		pCurrPoly = pNextPoly;
	}

	if ( pFrontPolys ) {
		pNewNode->FrontChild = BuildTree_R( pFrontPolys );
	} else {
		pNewNode->FrontChild = CreateEmptyLeaf();
	}

	if ( pBackPolys ) {
		pNewNode->BackChild = BuildTree_R( pBackPolys );
	} else {
		pNewNode->BackChild = CreateSolidLeaf();
	}

	if ( pNewNode->FrontChild->IsInternal() ) {
		pNewNode->Bounds.addInternalBox( pNewNode->FrontChild->Bounds );
	}
	if ( pNewNode->BackChild->IsInternal() ) {
		pNewNode->Bounds.addInternalBox( pNewNode->BackChild->Bounds );
	}
	return pNewNode;
}

//
// Removes the selected splitter from the given list of polygons !
//
Polygon * NodeyBSPTree::SelectSplitter( Polygon *& polygons )
{	BREAK_IF( ! polygons );

	//Polygon * currPoly = polygons;
	//Polygon * bestSplitter = NULL;
	//while ( currPoly != NULL )
	//{
	//	TODO: ...
	//}

	// Debugging.
	// Take the first polygon from the list as a splitter.
	Polygon * splitter = polygons;

	// Remove splitter from the given list of polygons.
	polygons = splitter->GetNext();

	splitter->SetNext( NULL );
	return splitter;
}

//
//	If the specified faces straddles the splitting plane,
//	then two new faces are created and the original face is deleted.
//
void NodeyBSPTree::SplitFace( Polygon* inFace, const mxPlane& splitPlane,
			Polygon *& pOutFrontPolys, Polygon *& pOutBackPolys, Polygon *& pOutCoplanars )
{
	f32  dists[ Polygon::MAX_VERTICES + 4 ];
	int  sides[ Polygon::MAX_VERTICES + 4 ];
	int  counts[ 3 ] = { 0 };

	// Classify individual vertices with respect to the splitPlane.
	{
		u32  vertexCount = 0;
		for ( vertexCount = 0; vertexCount < inFace->NumVertices(); vertexCount++ )
		{
			f32  fDistance = splitPlane.getDistanceTo( inFace->GetPos(vertexCount) );
			dists[ vertexCount ] = fDistance;

			const f32 fEpsilon = 0.001f;	// planeside epsilon

			if ( fDistance > +fEpsilon )
			{
				sides[ vertexCount ] = SIDE_FRONT;
			}
			else if ( fDistance < -fEpsilon )
			{
				sides[ vertexCount ] = SIDE_BACK;
			}
			else {
				sides[ vertexCount ] = SIDE_ON;
			}

			++counts[ sides[ vertexCount ] ];
		}

		sides[ vertexCount ] = sides[ 0 ];
		dists[ vertexCount ] = dists[ 0 ];
	}

	if ( counts[ SIDE_ON ] == inFace->NumVertices() ) {
		PrependItem< Polygon >( pOutCoplanars, inFace );	// if the polygon is aligned with the splitPlane.
		return;// SIDE_ON;
	}
	if ( ! counts[ SIDE_FRONT ] ) {
		PrependItem< Polygon >( pOutBackPolys, inFace );	// if the polygon is completely behind the splitPlane.
		return;// SIDE_BACK;
	}
	if ( ! counts[ SIDE_BACK ] ) {
		PrependItem< Polygon >( pOutFrontPolys, inFace );	// if the polygon is completely in front of the splitPlane.
		return;// SIDE_FRONT;
	}

	// Straddles the splitPlane - we must clip.

	STATS( m_stats.numSplits++ );

	const u32  maxPoints = inFace->NumVertices() + 4; // Estimated number of points.

	if ( inFace->NumVertices() >= Polygon::SPLIT_THRESHOLD ) {
		DEBUG_BREAK;
		Unimplemented2( "polygons with more than 16 vertices" );
	}

	Polygon * frontPoly = new (m_faces) Polygon();
	Polygon * backPoly = new (m_faces) Polygon();

	STATS( m_stats.numPolygons++ ); // We delete the old face and create two new faces.

	//frontPoly->m_Flags = backPoly->m_Flags = inFace->m_Flags;

	for ( u32 i = 0; i < inFace->NumVertices(); i++ )
	{
		const BspVertex & v1 = inFace->GetVertex( i );

		if ( sides[i] == SIDE_ON ) {
			frontPoly->AddVertex( v1 );
			backPoly->AddVertex( v1 );
			continue;
		}
		if ( sides[i] == SIDE_FRONT ) {
			frontPoly->AddVertex( v1 );
		}
		if ( sides[i] == SIDE_BACK ) {
			backPoly->AddVertex( v1 );
		}

		if ( (sides[i+1] == SIDE_ON) || (sides[i+1] == sides[i]) ) {
			continue;
		}

		// Generate a split point.
		BspVertex  mid;

		const BspVertex & v2 = inFace->GetVertex( (i+1) % inFace->NumVertices() );

#if 0 // More precise (and slower).

		// Always calculate the split going from the same side
		// or minor epsilon issues can happen.
		if ( sides[i] == SIDE_FRONT )
		{
			const f32  fDot = dists[i] / ( dists[i] - dists[i+1] );
			for ( u32 j = 0; j < 3; j++ )
			{
				// Avoid round off error when possible.
				if ( Get( splitPlane.Normal, j ) == 1.0f ) {
					Set( mid.Pos, j, -splitPlane.D );
				} else if ( Get( splitPlane.Normal, j ) == -1.0f ) {
					Set( mid.Pos, j, splitPlane.D );
				} else {
					Set( mid.Pos, j,
						Get( v1.Pos, j ) + fDot * (Get( v2.Pos, j ) - Get( v1.Pos, j ))
					);
				}
			}

			mid.Interpolate( v1, v2, fDot );
		}
		else
		{
			const f32  fDot = dists[i+1] / ( dists[i+1] - dists[i] );
			for ( u32 j = 0; j < 3; j++ )
			{
				// Avoid round off error when possible.
				if ( Get( splitPlane.Normal, j ) == 1.0f ) {
					Set( mid.Pos, j, -splitPlane.D );
				} else if ( Get( splitPlane.Normal, j ) == -1.0f ) {
					Set( mid.Pos, j, splitPlane.D );
				} else {
					Set( mid.Pos, j,
						Get( v2.Pos, j ) + fDot * (Get( v1.Pos, j ) - Get( v2.Pos, j ))
					);
				}
			}

			mid.Interpolate( v2, v1, fDot );
		}
#else
		const f32 fraction = splitPlane.getKnownIntersectionWithLine( v1.Pos, v2.Pos );
		mid.Interpolate( v1, v2, fraction );
#endif
		frontPoly->AddVertex( mid );
		backPoly->AddVertex( mid );
	}

	if ( frontPoly->NumVertices() > maxPoints
		|| backPoly->NumVertices() > maxPoints )
	{
		GCommon->Error( "ClipFace: number of points exceeded estimate" );
	}

	PrependItem< Polygon >( pOutFrontPolys, frontPoly );
	PrependItem< Polygon >( pOutBackPolys, backPoly );

	// Delete the original face.
//	m_faces.Delete( inFace );

	//return SIDE_CROSS;
}

Node * NodeyBSPTree::AllocNode( Polygon* splitter )
{
	Node * newNode = new (m_nodes) Node();
	MemSet( newNode, 0, sizeof( Node ) );

	// This is already set by MemZero, because
	// enum ENodeType::Internal == 0
	//newNode->m_eType = ENodeType::Internal;

	newNode->Faces = splitter;

	// Initialize splitting plane.
	{
		newNode->Plane.Set(
			splitter->GetPos( 0 ),
			splitter->GetPos( 1 ),
			splitter->GetPos( 2 ) );
	}

	// Update node bounds.
	newNode->Bounds.reset( 0,0,0 );
	splitter->AddToBounds( newNode->Bounds );

	STATS( m_stats.numInternalNodes++ );

	return newNode;
}

Node* NodeyBSPTree::CreateEmptyLeaf()
{
    /*
	static Node  emptyLeaf =
	{
		Node::ENodeType::OutCell,
		mxPlane(),	// plane
		NULL, NULL,	// children
		mxBounds( Vec3D(0,0,0) ),
		NULL,	// faces
		0		// flags
	};

	STATS( m_stats.numEmptyLeaves++ );

	return & emptyLeaf;
	*/
}

Node* NodeyBSPTree::CreateSolidLeaf()
{
/*
	static Node  solidLeaf =
	{
		Node::ENodeType::InCell,
		mxPlane(),	// plane
		NULL, NULL,	// children
		mxBounds( Vec3D(0,0,0) ),
		NULL,	// faces
		0		// flags
	};

	STATS( m_stats.numSolidLeaves++ );

	return & solidLeaf;
	*/
}

bool NodeyBSPTree::TraceLine( const Vec3D& start, const Vec3D& end, Vec3D &OutResult )
{
	return TraceLine_R( m_pRoot, start, end, OutResult );
}

bool NodeyBSPTree::TraceLine_R( Node* pNode, const Vec3D& start, const Vec3D& end, Vec3D &OutResult )
{	BREAK_IF( ! pNode );

	if ( pNode->IsLeaf() ) {
		return false;
	}

	const f32  dStart = pNode->Plane.DistanceTo( start );
	const f32  dEnd   = pNode->Plane.DistanceTo( end );

	if ( dStart > 0 && dEnd > 0 ) {
		return TraceLine_R( pNode->FrontChild, start, end, OutResult );
	}
	else if ( dStart < 0 && dEnd < 0 ) {
		return TraceLine_R( pNode->BackChild, start, end, OutResult );
	}

	// The line spans the splitting plane.

	// Variables to hold which side to traverse first and which second.
	Node *  pFirstSide = NULL;
	Node *  pSecondSide = NULL;

	// If we know that one is positive and the other is negative, and dStart < dEnd, so
	// dStart is negative and dEnd is positive.
	if ( dStart < dEnd )
	{
		pFirstSide = pNode->BackChild;	// The back side contains the start point so the back will be the first.
		pSecondSide = pNode->FrontChild;
	}
	// If dEnd is negative and dStart is positive.
	else if ( dEnd < dStart )
	{
		pFirstSide = pNode->FrontChild;	// The front side contains the start point so the front will be the first.
		pSecondSide = pNode->BackChild;
	}
	else {
		pFirstSide = pNode->FrontChild;
		pSecondSide = pNode->BackChild;
	}

	Vec3D  midPoint;	// intersection
	pNode->Plane.getIntersectionWithLimitedLine( start, end, midPoint );

	// Check the first side.
	if ( TraceLine_R( pFirstSide, start, midPoint, OutResult ) ) {
		return true;
	}

	{
		const Polygon * pCurrPoly = pNode->Faces;
		while ( pCurrPoly != NULL )
		{
			if ( pCurrPoly->PointInside( midPoint ) )
			{
				OutResult = midPoint;
				return true;
			}
			pCurrPoly = pCurrPoly->GetNext();
		}
	}

	// Check the second side.
	return TraceLine_R( pSecondSide, midPoint, end, OutResult );
}

const mxBounds & NodeyBSPTree::GetBounds() const {
	return m_pRoot->GetBounds();
}

}//end of namespace mix

//--------------------------------------------------------------//
//				End Of File.									//
//--------------------------------------------------------------//
