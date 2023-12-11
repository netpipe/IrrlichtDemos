/*
===============================================================================
	File:	Common.h
	Desc:
===============================================================================
*/

#ifndef __MX_DESTRUCTION_COMMON_H__
#define __MX_DESTRUCTION_COMMON_H__

namespace mix {

//
// Bounding volume types.
//

typedef AABB	mxBounds;


bool VolumesEqual( const mxBounds& bv1, const mxBounds& bv2, const f32 epsilon = SMALL_EPSILON );
bool VolumesOverlap( const mxBounds& bv1, const mxBounds& bv2, const f32 epsilon = SMALL_EPSILON );
bool VolumeIntersectsFrustum( const mxBounds& bv, const TViewFrustum& frustum );

bool SphereContainsPoint( const Vec3D& center, const f32 radius, const Vec3D& point );

//----------------------------------------------------------------------------------------------------

INLINE bool VolumesOverlap( const mxBounds& bv1, const mxBounds& bv2, const f32 epsilon /* = SMALL_EPSILON */ )
{
	(void) epsilon;
	return bv1.intersectsWithBox( bv2 );
}

}//end of namespace mix

#endif /* ! __MX_DESTRUCTION_COMMON_H__ */

//--------------------------------------------------------------//
//				End Of File.									//
//--------------------------------------------------------------//


