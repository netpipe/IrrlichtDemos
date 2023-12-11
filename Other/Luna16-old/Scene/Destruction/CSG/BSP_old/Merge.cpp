/*
===============================================================================
	File:	Merge.cpp
	Desc:	BSP tree merging.
===============================================================================
*/

//#include "stdafx.h"
#pragma hdrstop
#include "../../Include/IrrMix.h"

#include "BSP.h"


//#define SLOW_PRECISE


namespace mix {

// TODO: pre-complile geomod meshes.

void NodeyBSPTree::Subtract( const mxRenderMesh& geomodMesh )
{
	if ( ! m_bCanDoCSG ) {
		return;
	}
//	STATS( m_csgStats.Reset(); );

	u32  numPolys = 0;
	Polygon * pFaces = BuildPolygonList( geomodMesh, numPolys );

	Node * pNode = BuildTree_R( pFaces );

	MergeSubtract_R( m_pRoot, pNode );

	// Usually mesh doubles in size.
//	m_pRenderMesh->Triangles.SetNum( m_pRenderMesh->GetTriangleCount() * 2, true );
//	m_pRenderMesh->Vertices.SetNum( m_pRenderMesh->GetVertexCount() * 2, true );

//	STATS( m_csgStats.Stop() );
//	STATS( m_csgStats.Print() );
}

void NodeyBSPTree::Union( const mxRenderMesh& geomodMesh )
{
	if ( ! m_bCanDoCSG ) {
		return;
	}
	u32  numPolys = 0;
	Polygon * pFaces = BuildPolygonList( geomodMesh, numPolys );

	Node * pNode = BuildTree_R( pFaces );

	MergeUnion_R( m_pRoot, pNode );
}

//------------------------------------------------------------------------
//	NodeyBSPTree::MergeSubtract_R
//------------------------------------------------------------------------
//
void NodeyBSPTree::MergeSubtract_R( Node*& pMyNode, Node* pOtherNode )
{
	if ( pMyNode->IsSolid() ) {
		pMyNode = pOtherNode;
		return;
	}
	if ( pMyNode->IsSpace() ) {
		// TODO: Delete the second tree.
		//DeleteTree( pOtherNode );
		return;
	}

	const E_PlaneSide  eRelation = pMyNode->GetPlane().ClassifyVolumeRelation( pOtherNode->GetBounds() );

	if ( eRelation == SIDE_FRONT ) {
		MergeSubtract_R( pMyNode->FrontChild, pOtherNode );
	}
	else if ( eRelation == SIDE_BACK ) {
		MergeSubtract_R( pMyNode->BackChild, pOtherNode );
	}
	else // SIDE_CROSS
	{
		// Clip my polygons in the first node with the other tree.
		//TODO: move this into PartitionNodeWithPlane() when it's debugged.
		// Perform clipping only if polygons from both nodes intersect each other !

		RemoveFacesOutsideNode( pMyNode->Faces, pOtherNode );

		// Partition the other tree and merge with the corresponding pieces.
		Node * t2_partitioned_Front = NULL;
		Node * t2_partitioned_Back = NULL;

		PartitionNodeWithPlane( pMyNode->GetPlane(), pOtherNode,
				t2_partitioned_Front, t2_partitioned_Back );

		MergeSubtract_R( pMyNode->FrontChild, t2_partitioned_Front );
		MergeSubtract_R( pMyNode->BackChild, t2_partitioned_Back );
	}
}

//------------------------------------------------------------------------
//	NodeyBSPTree::MergeUnion_R
//------------------------------------------------------------------------
//
void NodeyBSPTree::MergeUnion_R( Node*& pMyNode, Node* pOtherNode )
{
	if ( pMyNode->IsSolid() ) {
		// TODO: Delete the second tree.
		//DeleteTree( pOtherNode );
		return;
	}
	if ( pMyNode->IsSpace() ) {
		pMyNode = pOtherNode;
		return;
	}

	const E_PlaneSide  eRelation = pMyNode->GetPlane().ClassifyVolumeRelation( pOtherNode->GetBounds() );

	if ( eRelation == SIDE_FRONT ) {
		MergeUnion_R( pMyNode->FrontChild, pOtherNode );
	}
	else if ( eRelation == SIDE_BACK ) {
		MergeUnion_R( pMyNode->BackChild, pOtherNode );
	}
	else // SIDE_CROSS
	{
		// Clip my polygons in the first node with the other tree.
		//TODO: move this into PartitionNodeWithPlane() when it's debugged.
		// Perform clipping only if polygons from both nodes intersect each other !

		RemoveFacesInsideNode( pMyNode->Faces, pOtherNode );

		// Partition the other tree and merge with the corresponding pieces.
		Node * t2_partitioned_Front = NULL;
		Node * t2_partitioned_Back = NULL;

		PartitionNodeWithPlane( pMyNode->GetPlane(), pOtherNode,
				t2_partitioned_Front, t2_partitioned_Back );

		MergeUnion_R( pMyNode->FrontChild, t2_partitioned_Front );
		MergeUnion_R( pMyNode->BackChild, t2_partitioned_Back );
	}
}

//------------------------------------------------------------------------
//	NodeyBSPTree::PartitionNodeWithPlane
//------------------------------------------------------------------------
//
void NodeyBSPTree::PartitionNodeWithPlane( const mxPlane& splitPlane, Node* node,
		Node*& OutFront, Node*& OutBack )
{
	STATS( m_csgStats.numRecursiveCalls++ );
	STATS( if ( m_csgStats.numRecursiveCalls > 20000 )  m_bCanDoCSG = false; );

	if ( node->IsLeaf() ) {
		OutFront = node;
		OutBack = node;
		return;
	}


#ifndef SLOW_PRECISE

	// Check for nodes with zero polygons.
	DEBUG_CODE(
	if ( ! node->Faces ) {
	//	GCommon->DebugPrint( "\nInternal node with no polygons! Shouldn't happen!" );
		DEBUG_BREAK;
	}
	);

#endif // !SLOW_PRECISE

	const mxPlane & plane2 = node->GetPlane();

	const f32  dot = splitPlane.Normal.dotProduct( node->GetPlane().Normal );

	// Variable to hold if the normal vectors are facing the same direction.
	bool  bNormalsInOneDir = ( dot > 0.0f ); //VECTOR_EPSILON );//0.0f );

	Polygon * frontPolys = NULL;
	Polygon * backPolys = NULL;
	Polygon * coplanars = NULL;

	Polygon * currFace = node->Faces;
	while ( currFace != NULL )
	{
		Polygon * next = currFace->GetNext();

		SplitFace( currFace, splitPlane,
			frontPolys, backPolys, coplanars );

		currFace = next;
	}


	if ( coplanars != NULL )
	{
	//	GCommon->DebugPrint( "\nWarning: cracks and holes appeared!" );

		//TODO: do something with coplanar polys!
		coplanars = NULL; // HACK: <= set to NULL to avoid further negative asserts
	}



#ifdef SLOW_PRECISE
	//HACK:
	goto L_Split_Both;

#endif // SLOW_PRECISE



	if ( frontPolys && backPolys ) {
		BREAK_IF( coplanars != NULL );
		goto L_Split_Both;
	}


	// If planes are coplanar.
	if ( !frontPolys && !backPolys )
	{
		// case "On"
		if ( bNormalsInOneDir )
		{
			// "parallel-on"
			OutFront = node->GetFront();
			OutBack = node->GetBack();
		}
		else
		{
			// "antiparallel-on"
			OutFront = node->GetBack();
			OutBack = node->GetFront();
		}
		return;
	}//End of case if ( !frontPolys && !backPolys )


	if ( ! frontPolys )
	{
		Assert2( backPolys, "All polygons of the node must be behind the splitting splitPlane" );
		Assert2( coplanars == NULL, "Coplanar polys are not supported!" );

		node->Faces = backPolys;

		if ( bNormalsInOneDir )
		{
			// Only the front child of the node needs to be partitioned.
			Node *  partitioned_front_F = NULL;
			Node *  partitioned_front_B = NULL;

			PartitionNodeWithPlane( splitPlane, node->GetFront(), partitioned_front_F, partitioned_front_B );

			OutFront = partitioned_front_F;
			OutFront->Bounds = node->GetBounds();// TODO: <= is this needed ?

			OutBack = node;
			OutBack->SetFront( partitioned_front_B );
			// node->backChild remains intact...
		}
		else
		{
			// Only the back child of the node is partitioned.
			Node *  partitioned_back_F = NULL;
			Node *  partitioned_back_B = NULL;

			PartitionNodeWithPlane( splitPlane, node->GetBack(), partitioned_back_F, partitioned_back_B );

			OutFront = partitioned_back_F;
			OutFront->Bounds = node->GetBounds(); // TODO: <= is this needed ?

			OutBack = node;
			OutBack->SetBack( partitioned_back_B );
			// node->frontChild remains intact...
		}
		return;
	}//End of case "No front polys"


	if ( ! backPolys )
	{
		Assert2( frontPolys, "All the polygons of the node must be in front of the splitting splitPlane" );
		Assert2( coplanars == NULL, "Coplanar polys are not supported!" );

		node->Faces = frontPolys;

		if ( bNormalsInOneDir )
		{
			// Only the back child of the node needs to be partitioned.
			Node *  partitioned_back_F = NULL;
			Node *  partitioned_back_B = NULL;

			PartitionNodeWithPlane( splitPlane, node->GetBack(), partitioned_back_F, partitioned_back_B );

			OutFront = node;
			OutFront->SetBack( partitioned_back_F );
			// node->frontChild remains intact...

			OutBack = partitioned_back_B;
			OutBack->Bounds = node->GetBounds();//TODO: <= is this needed
		}
		else
		{
			// Only the front child of the node has to be partitioned.
			Node *  partitioned_front_F = NULL;
			Node *  partitioned_front_B = NULL;

			PartitionNodeWithPlane( splitPlane, node->GetFront(), partitioned_front_F, partitioned_front_B );

			OutFront = node;
			OutFront->SetFront( partitioned_front_F );
			// node->backChild remains intact...

			OutBack = partitioned_front_B;
			OutBack->Bounds = node->GetBounds();// TODO: <= is this needed ?
		}
		return;
	}//End of case "No polys behind the splitPlane"


L_Split_Both:

	// Split both children of the node.

	OutFront = new (m_nodes) Node();
	MemSet( OutFront, 0, sizeof( Node ) );

	OutFront->Plane = node->GetPlane();
	OutFront->Faces = frontPolys;
	OutFront->Bounds = node->GetBounds();


	OutBack = new (m_nodes) Node();
	MemSet( OutBack, 0, sizeof( Node ) );

	OutBack->Plane = node->GetPlane();
	OutBack->Faces = backPolys;
	OutBack->Bounds = node->GetBounds();


	Node *  partitioned_front_F = NULL;
	Node *  partitioned_front_B = NULL;

	Node *  partitioned_back_F = NULL;
	Node *  partitioned_back_B = NULL;

	PartitionNodeWithPlane( splitPlane, node->GetFront(), partitioned_front_F, partitioned_front_B );
	PartitionNodeWithPlane( splitPlane, node->GetBack(), partitioned_back_F, partitioned_back_B );

	OutFront->SetFront( partitioned_front_F );
	OutFront->SetBack( partitioned_back_F );

	OutBack->SetFront( partitioned_front_B );
	OutBack->SetBack( partitioned_back_B );
}

//------------------------------------------------------------------------
//	NodeyBSPTree::RemoveFacesOutsideNode
//------------------------------------------------------------------------
//
void NodeyBSPTree::RemoveFacesOutsideNode( Polygon *& inFaces, const Node* pNode )
{
	Polygon * facesToSplit = NULL;

	// Construct bounding boxes for each face and check if splits are necessary.
	Polygon * face = inFaces;
	while ( face )
	{
		Polygon * next = face->GetNext();

		AABB  aabb;
		face->AddToBounds( aabb );
		if ( VolumesOverlap( aabb, pNode->GetBounds() ) )
		{
			PrependItem< Polygon >( facesToSplit, face );
		}

		face = next;
	}

	if ( facesToSplit )
	{
		Polygon * resultFaces = NULL;
		RemoveFacesOutsideNode_R( facesToSplit, pNode, resultFaces );
		inFaces = resultFaces;
	}
}

//------------------------------------------------------------------------
//	NodeyBSPTree::RemoveFacesOutsideNode_R
//------------------------------------------------------------------------
//
void NodeyBSPTree::RemoveFacesOutsideNode_R( Polygon* inFaces, const Node* pNode, Polygon *& OutFaces )
{
	if ( pNode->IsInternal() )
	{
		Polygon * frontPolys = NULL;
		Polygon * backPolys = NULL;
		Polygon * face = inFaces;
		while ( face )
		{
			Polygon * next = face->GetNext();
//TODO: ??? what to do with coplanars ???
			SplitFace( face, pNode->GetPlane(),
				frontPolys, backPolys, OutFaces );

			face = next;
		}

		if ( frontPolys ) {
			RemoveFacesOutsideNode_R( frontPolys, pNode->GetFront(), OutFaces );
		}
		if ( backPolys ) {
			RemoveFacesOutsideNode_R( backPolys, pNode->GetBack(), OutFaces );
		}
	}
	else if ( pNode->IsSolid() )
	{
		// Prepend inFaces to OutFaces.
		Polygon * item = inFaces;
		while ( item )
		{
			Polygon * next = item->GetNext();
			PrependItem< Polygon >( OutFaces, item );
			item = next;
		}
	}
	else {
		// The node represents the empty space.
		// TODO: delete inFaces
		// DeleteFaceList( inFaces );
	}
}

//------------------------------------------------------------------------
//	NodeyBSPTree::RemoveFacesInsideNode
//------------------------------------------------------------------------
//
void NodeyBSPTree::RemoveFacesInsideNode( Polygon *& inFaces, const Node* pNode )
{
	Polygon * facesToSplit = NULL;

	// Construct bounding boxes for each face and check if splits are necessary.
	Polygon * face = inFaces;
	while ( face )
	{
		Polygon * next = face->GetNext();

		AABB  aabb;
		face->AddToBounds( aabb );
		if ( VolumesOverlap( aabb, pNode->GetBounds() ) )
		{
			PrependItem< Polygon >( facesToSplit, face );
		}

		face = next;
	}

	if ( facesToSplit )
	{
		Polygon * resultFaces = NULL;
		RemoveFacesInsideNode_R( facesToSplit, pNode, resultFaces );
		inFaces = resultFaces;
	}
}

//------------------------------------------------------------------------
//	NodeyBSPTree::RemoveFacesInsideNode_R
//------------------------------------------------------------------------
//
void NodeyBSPTree::RemoveFacesInsideNode_R( Polygon* inFaces, const Node* pNode, Polygon *& OutFaces )
{
	if ( pNode->IsInternal() )
	{
		Polygon * frontPolys = NULL;
		Polygon * backPolys = NULL;
		Polygon * face = inFaces;
		while ( face )
		{
			Polygon * next = face->GetNext();
//TODO: ??? what to do with coplanars ???
			SplitFace( face, pNode->GetPlane(),
				frontPolys, backPolys, OutFaces );

			face = next;
		}

		if ( frontPolys ) {
			RemoveFacesInsideNode_R( frontPolys, pNode->GetFront(), OutFaces );
		}
		if ( backPolys ) {
			RemoveFacesInsideNode_R( backPolys, pNode->GetBack(), OutFaces );
		}
	}
	else if ( pNode->IsSolid() )
	{
		// The node represents the empty space.
		// TODO: delete inFaces
		// DeleteFaceList( inFaces );
	}
	else {
		// Prepend inFaces to OutFaces.
		Polygon * item = inFaces;
		while ( item )
		{
			Polygon * next = item->GetNext();
			PrependItem< Polygon >( OutFaces, item );
			item = next;
		}
	}
}

}//end of namespace mix

//--------------------------------------------------------------//
//				End Of File.									//
//--------------------------------------------------------------//
