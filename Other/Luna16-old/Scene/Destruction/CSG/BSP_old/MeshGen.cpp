/*
===============================================================================
	File:	MeshGen.cpp
	Desc:	Mesh generation from a node-storing BSP tree.
===============================================================================
*/

//#include "stdafx.h"
#pragma hdrstop
#include "../../Include/IrrMix.h"

#include "BSP.h"

namespace mix {

//------------------------------------------------------------------------
//	NodeyBSPTree::GenerateMesh
//------------------------------------------------------------------------
//
void NodeyBSPTree::GenerateMesh( mxRenderMesh &OutMesh )
{
	OutMesh.Vertices.SetNum( 0, false /* don't resize */ );
	OutMesh.Vertices.SetNum( 0, false /* don't resize */ );

	EmitNode_R( m_pRoot, OutMesh );

	//****************
	m_pRenderMesh = & OutMesh;
}

//
//	CreateRenderVertex
//
TIndex CreateRenderVertex( const BspVertex& v, mxRenderMesh & mesh )
{
	mxVertex & rNewVertex = mesh.Vertices.Alloc();

	rNewVertex.Pos    = v.Pos;
	rNewVertex.Normal = v.Normal;
	rNewVertex.U = v.U;
	rNewVertex.V = v.V;

	return mesh.Vertices.Num() - 1;
}

//------------------------------------------------------------------------
//	NodeyBSPTree::EmitNode_R
//------------------------------------------------------------------------
//
void NodeyBSPTree::EmitNode_R( Node* pNode, mxRenderMesh &OutMesh ) const
{
	if ( pNode->IsInternal() )
	{
		// TODO: don't emit this node if the data in this node hasn't been changed.

		EmitNode_R( pNode->GetBack(), OutMesh );
		EmitNode_R( pNode->GetFront(), OutMesh );

		//----------------
		pNode->Data.FirstTriangle = OutMesh.GetTriangleCount();
		if ( pNode->Data.FirstTriangle > 0 ) {
			pNode->Data.FirstTriangle--;
		}

		pNode->Data.NumTriangles = 0;


		// Update bounding box of this node.
		pNode->Bounds.reset( 0,0,0 );
		pNode->Bounds.addInternalBox( pNode->GetFront()->GetBounds() );
		pNode->Bounds.addInternalBox( pNode->GetBack()->GetBounds() );


		// Loop through all faces of this node.
		const Polygon * polygon = pNode->Faces;
		while ( polygon != NULL )
		{
			Assert( polygon->NumVertices() >= 3 );

			const u32 numTriangles = polygon->NumVertices() - 2;

			const BspVertex & basePoint = polygon->Vertices[ 0 ];
			for ( u32 i = 1; i < numTriangles + 1; i++ )
			{
				IndexTriple & rNewTriangle = OutMesh.Triangles.Alloc();

				rNewTriangle.iA = CreateRenderVertex( basePoint,				OutMesh );
				rNewTriangle.iB = CreateRenderVertex( polygon->Vertices[ i ],	OutMesh );
				rNewTriangle.iC = CreateRenderVertex( polygon->Vertices[ i+1 ],	OutMesh );

				// Update bounding box of this node.
				pNode->Bounds.addInternalPoint( basePoint.Pos );
				pNode->Bounds.addInternalPoint( polygon->Vertices[ i ].Pos );
				pNode->Bounds.addInternalPoint( polygon->Vertices[ i+1 ].Pos );

				pNode->Data.NumTriangles++;
			}
			polygon = polygon->GetNext();
		}
	}
}

}//end of namespace mix

//--------------------------------------------------------------//
//				End Of File.									//
//--------------------------------------------------------------//
