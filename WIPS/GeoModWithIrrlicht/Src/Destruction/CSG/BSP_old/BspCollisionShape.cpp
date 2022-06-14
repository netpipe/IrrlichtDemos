/*
===============================================================================
	File:	BspCollisionShape.cpp
	Desc:	BSP collision shape for use with Bullet Physics engine.
===============================================================================
*/

#include "stdafx.h"
#pragma hdrstop
#include <Include/IrrMix.h>

#include "BSP.h"

namespace mix {

//------------------------------------------------------------------------
//	NodeyBSPTree::processAllTriangles
//------------------------------------------------------------------------
//
void NodeyBSPTree::processAllTriangles( btTriangleCallback* callback, const btVector3& aabbMin, const btVector3& aabbMax ) const
{
	//const AABB  aabb( Convert( aabbMin ), Convert( aabbMax ) );
	const AABB  aabb( Convert( aabbMin ), Convert( aabbMax ) );

	ProcessAllTriangles_R( m_pRoot, aabb, callback );
}

//------------------------------------------------------------------------
//	NodeyBSPTree::ProcessAllTriangles_R
//------------------------------------------------------------------------
//
void NodeyBSPTree::ProcessAllTriangles_R( const Node* pNode, const AABB& aabb, btTriangleCallback* callback ) const
{
	if ( pNode->IsInternal() )
	{
		const E_PlaneSide  eRelation = pNode->GetPlane().ClassifyVolumeRelation( aabb );

		if ( eRelation == SIDE_FRONT ) {
			ProcessAllTriangles_R( pNode->FrontChild, aabb, callback );
		}
		else if ( eRelation == SIDE_BACK ) {
			ProcessAllTriangles_R( pNode->BackChild, aabb, callback );
		}
		else // SIDE_CROSS
		{
#if 0
			// Process all triangles in this node.
			for ( TIndex  iTriIndex = 0; iTriIndex <= pNode->Data.NumTriangles; ++iTriIndex )
			{
				const IndexTriple & rTri = m_pRenderMesh->Triangles[ iTriIndex + pNode->Data.FirstTriangle ];

				btVector3  triangle[3];
				for ( u32 i = 0; i < 3; i++ ) {
					Assign( triangle[i], m_pRenderMesh->GetPos( rTri[i] ) );
				}

				if ( TestTriangleAgainstAabb2( triangle, Convert(aabb.MinEdge), Convert(aabb.MaxEdge) ) )
				{
					callback->processTriangle( triangle, 0, (int)iTriIndex );
				}
			}
#else
			const Polygon * polygon = pNode->Faces;
			while ( polygon != NULL )
			{
				const u32 numTriangles = polygon->NumVertices() - 2;

				const Vec3D & basePoint = polygon->Vertices[ 0 ].Pos;

				btVector3  triangle[3];
				Assign( triangle[0], basePoint );

				for ( u32 i = 1; i < numTriangles + 1; i++ )
				{
					Assign( triangle[ 1 ], polygon->Vertices[ i ].Pos );
					Assign( triangle[ 2 ], polygon->Vertices[ i+1 ].Pos );

					//if ( TestTriangleAgainstAabb2( triangle, Convert(aabb.MinEdge), Convert(aabb.MaxEdge) ) )
					{
						callback->processTriangle( triangle, 0, i );
					}
				}

				polygon = polygon->GetNext();
			}
#endif
			ProcessAllTriangles_R( pNode->FrontChild, aabb, callback );
			ProcessAllTriangles_R( pNode->BackChild, aabb, callback );
		}
	}//End If Node Is Internal
}

//------------------------------------------------------------------------
//	NodeyBSPTree::getAabb
//------------------------------------------------------------------------
//
void NodeyBSPTree::getAabb( const btTransform& t, btVector3& aabbMin, btVector3& aabbMax ) const
{
	(void) t;

	Assert( m_pRoot );

	const AABB & aabb( GetBounds() );
	Assign( aabbMin, aabb.MinEdge );
	Assign( aabbMax, aabb.MaxEdge );
}

//------------------------------------------------------------------------
//	NodeyBSPTree::calculateLocalInertia
//------------------------------------------------------------------------
//
void NodeyBSPTree::calculateLocalInertia( btScalar mass, btVector3& inertia ) const
{
	(void) mass;

	// Moving concave objects not supported.
	inertia.setValue( btScalar(0.),btScalar(0.),btScalar(0.) );
}

//------------------------------------------------------------------------
//	NodeyBSPTree::setLocalScaling
//------------------------------------------------------------------------
//
void NodeyBSPTree::setLocalScaling( const btVector3& scaling )
{
	(void) scaling;
}

//------------------------------------------------------------------------
//	NodeyBSPTree::getLocalScaling
//------------------------------------------------------------------------
//
const btVector3 & NodeyBSPTree::getLocalScaling() const
{
	return btVector3( 1, 1, 1 );
}

}//end of namespace mix

//--------------------------------------------------------------//
//				End Of File.									//
//--------------------------------------------------------------//
