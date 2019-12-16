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

#ifndef _egPrimitives_H_
#define _egPrimitives_H_

#include "egPrerequisites.h"
#include "utMath.h"


class BoundingBox
{
private:	
	
	Vec3f _minCoords, _maxCoords;

public:
	
	Vec3f &getMinCoords() { return _minCoords; }
	Vec3f &getMaxCoords() { return _maxCoords; }
	
	void clear()
	{
		_minCoords = Vec3f( 0, 0, 0 );
		_maxCoords = Vec3f( 0, 0, 0 );
	}

	Vec3f getCorner( uint32 index )
	{
		switch( index )
		{
		case 0:
			return Vec3f( _minCoords.x, _minCoords.y, _maxCoords.z );
		case 1:
			return Vec3f( _maxCoords.x, _minCoords.y, _maxCoords.z );
		case 2:
			return Vec3f( _maxCoords.x, _maxCoords.y, _maxCoords.z );
		case 3:
			return Vec3f( _minCoords.x, _maxCoords.y, _maxCoords.z );
		case 4:
			return Vec3f( _minCoords.x, _minCoords.y, _minCoords.z );
		case 5:
			return Vec3f( _maxCoords.x, _minCoords.y, _minCoords.z );
		case 6:
			return Vec3f( _maxCoords.x, _maxCoords.y, _minCoords.z );
		case 7:
			return Vec3f( _minCoords.x, _maxCoords.y, _minCoords.z );
		default:
			return Vec3f();
		}
	}


	void transform( const Matrix4f &m )
	{
		// Efficient algorithm for transforming an AABB, taken from Graphics Gems
		
		float minA[3] = { _minCoords.x, _minCoords.y, _minCoords.z }, minB[3];
		float maxA[3] = { _maxCoords.x, _maxCoords.y, _maxCoords.z }, maxB[3];

		for( uint32 i = 0; i < 3; ++i )
		{
			minB[i] = m.c[3][i];
			maxB[i] = m.c[3][i];
			
			for( uint32 j = 0; j < 3; ++j )
			{
				float x = minA[j] * m.c[j][i];
				float y = maxA[j] * m.c[j][i];
				minB[i] += minf( x, y );
				maxB[i] += maxf( x, y );
			}
		}

		_minCoords = Vec3f( minB[0], minB[1], minB[2] );
		_maxCoords = Vec3f( maxB[0], maxB[1], maxB[2] );
	}


	bool makeUnion( BoundingBox &b )
	{
		bool changed = false;

		// Ignore zero-size boxes
		if( _minCoords == _maxCoords )
		{
			changed = true;
			_minCoords = b._minCoords;
			_maxCoords = b._maxCoords;
		}
		else if( b._minCoords != b._maxCoords )
		{
			if( b._minCoords.x < _minCoords.x ) { changed = true; _minCoords.x = b._minCoords.x; }
			if( b._minCoords.y < _minCoords.y ) { changed = true; _minCoords.y = b._minCoords.y; }
			if( b._minCoords.z < _minCoords.z ) { changed = true; _minCoords.z = b._minCoords.z; }

			if( b._maxCoords.x > _maxCoords.x ) { changed = true; _maxCoords.x = b._maxCoords.x; }
			if( b._maxCoords.y > _maxCoords.y ) { changed = true; _maxCoords.y = b._maxCoords.y; }
			if( b._maxCoords.z > _maxCoords.z ) { changed = true; _maxCoords.z = b._maxCoords.z; }
		}

		return changed;
	}
};


class Frustum
{
private:
	
	Vec3f			_origin;
	Plane			_planes[6];		// Planes of frustum
	Vec3f			_corners[8];	// Corner points

public:

	const Vec3f &getOrigin() const { return _origin; }
	const Vec3f &getCorner( uint32 index ) const { return _corners[index]; }
	
	void buildViewFrustum( const Matrix4f &transMat, float fov, float aspect, float near, float far );
	void buildViewFrustum( const Matrix4f &transMat, float left, float right,
						   float bottom, float top, float near, float far );
	void buildViewFrustum( const Matrix4f &viewMat, const Matrix4f &projMat );
	void buildBoxFrustum( const Matrix4f &transMat, float left, float right,
						  float bottom, float top, float front, float back );
	bool cullSphere( Vec3f pos, float rad );
	bool cullBox( BoundingBox &b ) const;
	bool cullFrustum( const Frustum &frust );
};

#endif // _egPrimitives_H_
