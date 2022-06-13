#include "../../../../config.h"
#ifdef OPENSTEER
/*! Irrlicht implementation by A.Buschhüter<br> (http://abusoft.g0dsoft.com<br>)
----------------------------------------------------------------------------<br>
//
//
// OpenSteer -- Steering Behaviors for Autonomous Characters
//
// Copyright (c) 2002-2005, Sony Computer Entertainment America
// Original author: Craig Reynolds <craig_reynolds@playstation.sony.com>
//
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//
//
// ----------------------------------------------------------------------------<br>
//
// Utilities to work with Vec3.
//
// 05-12-05 bk:  Created based on code of PolylinePathway.
//
// ----------------------------------------------------------------------------<br>
*/

#ifndef OPENSTEER_VEC3UTILITIES_H
#define OPENSTEER_VEC3UTILITIES_H

#include "osVec3.h"
#include "osStandardTypes.h"
#include "osUtilities.h"

namespace OpenSteer {

/**
 * Returns the nearest point on the segment @a segmentPoint0 to
 * @a segmentPoint1 from @a point.
 */
OpenSteer::Vec3  nearestPointOnSegment( const Vec3& point,
                                        const Vec3& segmentPoint0,
                                        const Vec3& segmentPoint1 );
//  OpenSteer::Vec3  nearestPointOnSegment( const irr::core::vector3df& point,
//                                          const irr::core::vector3df& segmentPoint0,
//                                          const irr::core::vector3df& segmentPoint1 ){
//    Vec3 a = point, b = segmentPoint0, c = segmentPoint1;
//    return nearestPointOnSegment(a,b,c);
//  }

/**
 * Computes minimum distance from @a point to the line segment defined by
 * @a segmentPoint0 and @a segmentPoint1.
 */
float pointToSegmentDistance( const Vec3& point,
                              const Vec3& segmentPoint0,
                              const Vec3& segmentPoint1);
//  float pointToSegmentDistance( const irr::core::vector3df& point,
//                                const irr::core::vector3df& segmentPoint0,
//                                const irr::core::vector3df& segmentPoint1){
//    Vec3 a = point, b = segmentPoint0, c = segmentPoint1;
//    return pointToSegmentDistance(a,b,c);
//  }

/**
 * Retuns distance between @a a and @a b.
 */
inline float distance (const Vec3& a, const Vec3& b) {
  return (a-b).length();
}
inline float distance (const irr::core::vector3df& a, const irr::core::vector3df& b) {
  return a.getDistanceFrom(b);
}

/**
 * Elementwise relative tolerance comparison of @a lhs and @a rhs taking
 * the range of the elements into account.
 *
 * See Christer Ericson, Real-Time Collision Detection, Morgan Kaufmann,
 * 2005, pp. 441--443.
 *
 * @todo Rewrite using the stl or providing an own range based function.
 */
inline bool equalsRelative( Vec3 const& lhs, Vec3 const& rhs, float const& tolerance = std::numeric_limits< float >::epsilon() ) {
  return equalsRelative( lhs.x, rhs.x, tolerance ) && equalsRelative( lhs.y, rhs.y ) && equalsRelative( lhs.z, rhs.z );
}
inline bool equalsRelative( irr::core::vector3df const& lhs, irr::core::vector3df const& rhs, float const& tolerance = std::numeric_limits< float >::epsilon() ) {
  return equalsRelative( lhs.X, rhs.X, tolerance ) && equalsRelative( lhs.Y, rhs.Y ) && equalsRelative( lhs.Z, rhs.Z );
}

} // namespace OpenSteer

#endif // OPENSTEER_VEC3UTILITIES_H
#endif
