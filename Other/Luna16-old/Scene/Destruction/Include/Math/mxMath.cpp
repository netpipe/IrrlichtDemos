/*
===============================================================================
	File:	mxMath.cpp
	Desc:.
===============================================================================
*/

#include "../../stdafx.h"
#pragma hdrstop
#include "../../Include/IrrMix.h"

namespace mix {

/*================================
	mxPlane
================================*/

void mxPlane::Set( const TPlane3D& plane )
{
	TPlane3D::operator = ( plane );
}

void mxPlane::Set( const Vec3D& pointA, const Vec3D& pointB, const Vec3D& pointC )
{
	TPlane3D::setPlane( pointA, pointB, pointC );
}

E_PlaneSide mxPlane::ClassifyPointRelation_epsilon( const Vec3D& point, const f32 epsilon /* =PLANESIDE_EPSILON */ ) const
{
	const f32  fDist = DistanceTo( point );
	if ( fDist < -epsilon ) {
		return SIDE_BACK;
	}
	else if ( fDist > +epsilon ) {
		return SIDE_FRONT;
	}
	return SIDE_ON;
}

E_PlaneSide mxPlane::ClassifyPointRelation( const Vec3D& point ) const
{
	const f32  fDist = DistanceTo( point );
	if ( fDist < -PLANESIDE_EPSILON ) {
		return SIDE_BACK;
	}
	else if ( fDist > +PLANESIDE_EPSILON ) {
		return SIDE_FRONT;
	}
	return SIDE_ON;
}

E_PlaneSide mxPlane::ClassifyVolumeRelation( const AABB& bv, const f32 epsilon /* =PLANESIDE_EPSILON */ ) const
{
	// if mxBounds is an axis aligned bounding box
	Vec3D  vCenter = (bv.MinEdge + bv.MaxEdge) * 0.5f;

	f32  dist1 = DistanceTo( vCenter );

	const f32 * center	= reinterpret_cast< const f32* >( & vCenter );
	const f32 * b1		= reinterpret_cast< const f32* >( & bv.MaxEdge );
	const f32 * normal	= reinterpret_cast< const f32* >( & Normal );

	f32  dist2 = idMath::Fabs( ( b1[0] - center[0] ) * normal[0] ) +
					idMath::Fabs( ( b1[1] - center[1] ) * normal[1] ) +
						idMath::Fabs( ( b1[2] - center[2] ) * normal[2] );

	if ( dist1 - dist2 > epsilon ) {
		return SIDE_FRONT;
	}
	if ( dist1 + dist2 < -epsilon ) {
		return SIDE_BACK;
	}
	return SIDE_CROSS;
}

f32 mxPlane::DistanceTo( const Vec3D& point ) const
{
	return TPlane3D::getDistanceTo( point );
}

//------------------------------------------------------------------------------------------------------

/*
// Swiped from the good old flipcode.
// tests if a sphere is within the frustrum
int Frustrum::ContainsSphere(const Sphere& refSphere) const
{
	// various distances
	float fDistance;

	// calculate our distances to each of the planes
	for(int i = 0; i < 6; ++i) {

		// find the distance to this plane
		fDistance = m_plane[i].Normal().dotProduct(refSphere.Center())+m_plane[i].Distance();

		// if this distance is < -sphere.radius, we are outside
		if(fDistance < -refSphere.Radius())
			return(OUT);

		// else if the distance is between +- radius, then we intersect
		if((float)fabs(fDistance) < refSphere.Radius())
			return(INTERSECT);
	}

	// otherwise we are fully in view
	return(IN);
}
*/

bool SphereIntersectsSphere( const Vec3D& center0, f32 radius0, const Vec3D& center1, f32 radius1 )
{
	// Get the separating axis.
	Vec3D  vSeparatingAxis = center0 - center1;

	// Get the sum of the radii.
	f32  fRadiiSum = radius0 + radius1;

	// If the distance between the centers is less than the sum of the radii, then we have an intersection.
	if( vSeparatingAxis.getLengthSQ() < fRadiiSum * fRadiiSum ) {
		return true;
	}
	// Otherwise they are separated.
	return false;
}

bool SphereIntersectsFrustum( const Vec3D& center, f32 radius, const TViewFrustum& frustum )
{
	for ( u32 i = 0; i != ::irr::scene::SViewFrustum::VF_PLANE_COUNT; ++i )
	{
		const f32  fDistance = frustum.planes[i].getDistanceTo( center );
		if ( fDistance < -radius ) {
			return false;
		}
		if ( idMath::Fabs( fDistance ) < radius ) {
			return true;
		}
	}
	return true;
}

}//end of namespace mix

//--------------------------------------------------------------//
//				End Of File.									//
//--------------------------------------------------------------//
