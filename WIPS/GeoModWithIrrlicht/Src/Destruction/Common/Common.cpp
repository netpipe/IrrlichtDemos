/*
===============================================================================
	File:	Common.cpp
	Desc:	
===============================================================================
*/

#include "stdafx.h"
#pragma hdrstop
#include <Include/IrrMix.h>

namespace mix {

//------------------------------------
//		VolumeIntersectsFrustum
//	TODO: more efficient algorithm ( radar approach ).
//------------------------------------
//
bool VolumeIntersectsFrustum( const mxBounds& bv, const TViewFrustum& frustum )
{
	Vec3D edges[8];
	bv.getEdges( edges );

	for ( u32 i = 0; i != ::irr::scene::SViewFrustum::VF_PLANE_COUNT; ++i )
	{
		bool boxInFrustum=false;

		for (u32 j=0; j!=8; ++j)
		{
			if ( frustum.planes[i].classifyPointRelation(edges[j]) != ::irr::core::ISREL3D_FRONT)
			{
				return true;
			}
		}
	}
	return false;
}

}//end of namespace mix

//--------------------------------------------------------------//
//				End Of File.									//
//--------------------------------------------------------------//
