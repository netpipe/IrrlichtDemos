// *************************************************************************************************
//
// Horde3D
//   Next-Generation Graphics Engine
// --------------------------------------
// Copyright (C) 2006-2008 Nicolas Schulz
//
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
//
// *************************************************************************************************

#include "egPrimitives.h"

#include "utDebug.h"


// *************************************************************************************************
// Frustum
// *************************************************************************************************

void Frustum::buildViewFrustum( const Matrix4f &transMat, float fov, float aspect, float near, float far )
{
	float ymax = near * tanf( degToRad( fov / 2 ) );
	float xmax = ymax * aspect;
	
	buildViewFrustum( transMat, -xmax, xmax, -ymax, ymax, near, far );
}


void Frustum::buildViewFrustum( const Matrix4f &transMat, float left, float right,
							    float bottom, float top, float near, float far ) 
{
	// Use intercept theorem to get params for far plane
	float left_f = left * far / near;
	float right_f = right * far / near;
	float bottom_f = bottom * far / near;
	float top_f = top * far / near;

	// Get points on near plane
	_corners[0] = Vec3f( left, bottom, -near );
	_corners[1] = Vec3f( right, bottom, -near );
	_corners[2] = Vec3f( right, top, -near );
	_corners[3] = Vec3f( left, top, -near );

	// Get points on far plane
	_corners[4] = Vec3f( left_f, bottom_f, -far );
	_corners[5] = Vec3f( right_f, bottom_f, -far );
	_corners[6] = Vec3f( right_f, top_f, -far );
	_corners[7] = Vec3f( left_f, top_f, -far );

	// Transform points to fit camera position and rotation
	_origin = transMat * Vec3f( 0, 0, 0 );
	for( uint32 i = 0; i < 8; ++i )
		_corners[i] = transMat * _corners[i];

	// Build planes
	_planes[0] = Plane( _origin, _corners[3], _corners[0] );		// Left
	_planes[1] = Plane( _origin, _corners[1], _corners[2] );		// Right
	_planes[2] = Plane( _origin, _corners[0], _corners[1] );		// Bottom
	_planes[3] = Plane( _origin, _corners[2], _corners[3] );		// Top
	_planes[4] = Plane( _corners[0], _corners[1], _corners[2] );	// Near
	_planes[5] = Plane( _corners[5], _corners[4], _corners[7] );	// Far
}


void Frustum::buildViewFrustum( const Matrix4f &viewMat, const Matrix4f &projMat )
{
	// This routine works with the OpenGL projection matrix
	// The view matrix is the inverse camera transformation matrix
	// Note: Frustum corners are not updated!
	
	Matrix4f m = projMat * viewMat;
	
	_planes[0] = Plane( -(m.c[0][3] + m.c[0][0]), -(m.c[1][3] + m.c[1][0]),
						-(m.c[2][3] + m.c[2][0]), -(m.c[3][3] + m.c[3][0]) );	// Left
	_planes[1] = Plane( -(m.c[0][3] - m.c[0][0]), -(m.c[1][3] - m.c[1][0]),
						-(m.c[2][3] - m.c[2][0]), -(m.c[3][3] - m.c[3][0]) );	// Right
	_planes[2] = Plane( -(m.c[0][3] + m.c[0][1]), -(m.c[1][3] + m.c[1][1]),
						-(m.c[2][3] + m.c[2][1]), -(m.c[3][3] + m.c[3][1]) );	// Bottom
	_planes[3] = Plane( -(m.c[0][3] - m.c[0][1]), -(m.c[1][3] - m.c[1][1]),
						-(m.c[2][3] - m.c[2][1]), -(m.c[3][3] - m.c[3][1]) );	// Top
	_planes[4] = Plane( -(m.c[0][3] + m.c[0][2]), -(m.c[1][3] + m.c[1][2]),
						-(m.c[2][3] + m.c[2][2]), -(m.c[3][3] + m.c[3][2]) );	// Near
	_planes[5] = Plane( -(m.c[0][3] - m.c[0][2]), -(m.c[1][3] - m.c[1][2]),
						-(m.c[2][3] - m.c[2][2]), -(m.c[3][3] - m.c[3][2]) );	// Far

	_origin = viewMat.inverted() * Vec3f( 0, 0, 0 );

	// Calculate corners (this is not tested)
	/*Matrix4f mm = m.inverted();
	_corners[0] = mm * Vec3f( -1, -1,  1 );
	_corners[1] = mm * Vec3f(  1, -1,  1 );
	_corners[2] = mm * Vec3f(  1,  1,  1 );
	_corners[3] = mm * Vec3f( -1,  1,  1 );
	_corners[4] = mm * Vec3f( -1, -1,  1 );
	_corners[5] = mm * Vec3f(  1, -1,  1 );
	_corners[6] = mm * Vec3f(  1,  1,  1 );
	_corners[7] = mm * Vec3f( -1,  1,  1 );*/
}


void Frustum::buildBoxFrustum( const Matrix4f &transMat, float left, float right,
							   float bottom, float top, float front, float back ) 
{
	// Get points on front plane
	_corners[0] = Vec3f( left, bottom, front );
	_corners[1] = Vec3f( right, bottom, front );
	_corners[2] = Vec3f( right, top, front );
	_corners[3] = Vec3f( left, top, front );

	// Get points on far plane
	_corners[4] = Vec3f( left, bottom, back );
	_corners[5] = Vec3f( right, bottom, back );
	_corners[6] = Vec3f( right, top, back );
	_corners[7] = Vec3f( left, top, back );

	// Transform points to fit camera position and rotation
	_origin = transMat * Vec3f( 0, 0, 0 );
	for( uint32 i = 0; i < 8; ++i )
		_corners[i] = transMat * _corners[i];

	// Build planes
	_planes[0] = Plane( _corners[0], _corners[3], _corners[7] );	// Left
	_planes[1] = Plane( _corners[2], _corners[1], _corners[6] );	// Right
	_planes[2] = Plane( _corners[0], _corners[4], _corners[5] );	// Bottom
	_planes[3] = Plane( _corners[3], _corners[2], _corners[6] );	// Top
	_planes[4] = Plane( _corners[0], _corners[1], _corners[2] );	// Front
	_planes[5] = Plane( _corners[4], _corners[7], _corners[6] );	// Back
}


bool Frustum::cullSphere( Vec3f pos, float rad )
{
	// Check the distance of the center to the planes
	for( uint32 i = 0; i < 6; ++i )
	{
		if( _planes[i].distToPoint( pos ) > rad ) return true;
	}

	return false;
}


bool Frustum::cullBox( BoundingBox &b ) const
{
	// Idea for optimized AABB testing from www.lighthouse3d.com
	for( uint32 i = 0; i < 6; ++i )
	{
		const Vec3f &n = _planes[i].normal;
		
		Vec3f positive = b.getMinCoords();
		if( n.x <= 0 ) positive.x = b.getMaxCoords().x;
		if( n.y <= 0 ) positive.y = b.getMaxCoords().y;
		if( n.z <= 0 ) positive.z = b.getMaxCoords().z;

		if( _planes[i].distToPoint( positive ) > 0 ) return true;
	}
	
	return false;
}


bool Frustum::cullFrustum( const Frustum &frust )
{
	for( uint32 i = 0; i < 6; ++i )
	{
		bool allOut = true;
		
		for( uint32 j = 0; j < 8; ++j )
		{
			if( _planes[i].distToPoint( frust._corners[j] ) < 0 )
			{
				allOut = false;
				break;
			}
		}

		if( allOut ) return true;
	}

	return false;
}
