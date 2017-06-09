// FunCollision -- Collision detection and response system -- Created by Peter Szollosi, 2009-2010.
// For conditions of distribution and use, see copyright notice in license.txt or FunCollision.h

#include <irrlicht.h>
using namespace irr;

#include "CCollCommon.h"


namespace COLL
{


//! Math --------------------------------------------------------------------------------------------------------------------------------
//! pointToLine
core::vector3df pointToLine(const core::vector3df& Point, const core::line3df& Line)
{
    core::vector3df c = Point - Line.start;
    core::vector3df v = Line.end - Line.start;
    float len = v.X * v.X + v.Y * v.Y + v.Z * v.Z;

    if (len <= core::ROUNDING_ERROR_f32) return Point;
    len = sqrtf(len);

    v /= len;
    float t = v.dotProduct(c);

    v *= t;
    return Line.start + v;
}

//! pointOnSegment
bool pointOnSegment(const core::vector3df& Point, const core::line3df& Segment, float ToleranceSQ)
{
    return Point.getDistanceFromSQ(Segment.start) + Point.getDistanceFromSQ(Segment.end)
           <= Segment.getLengthSQ() + ToleranceSQ;
}

//! lineToLine
bool lineToLine(const core::vector3df& p1, const core::vector3df& p2,
                const core::vector3df& p3, const core::vector3df& p4,
                core::vector3df& pa, core::vector3df& pb)
{
    core::vector3df p13, p43, p21;
    float d1343, d4321, d1321, d4343, d2121;
    float numer, denom;
    float mua, mub;

    p13 = p1 - p3;
    p43 = p4 - p3;
    if (p43.getLengthSQ() <= 0) return false; // R.I.P. stupid sinking bug

    p21 = p2 - p1;
    if (p21.getLengthSQ() <= 0) return false; // R.I.P. nasty sinking bug

    d1343 = p13.X * p43.X + p13.Y * p43.Y + p13.Z * p43.Z;
    d4321 = p43.X * p21.X + p43.Y * p21.Y + p43.Z * p21.Z;
    d1321 = p13.X * p21.X + p13.Y * p21.Y + p13.Z * p21.Z;
    d4343 = p43.X * p43.X + p43.Y * p43.Y + p43.Z * p43.Z;
    d2121 = p21.X * p21.X + p21.Y * p21.Y + p21.Z * p21.Z;

    denom = d2121 * d4343 - d4321 * d4321;

    if (core::abs_(denom) <= 0) return false; // R.I.P. sneaky sinking bug

    numer = d1343 * d4321 - d1321 * d4343;

    mua = numer / denom;
    mub = (d1343 + d4321 * mua) / d4343;

    pa = p1 + core::vector3df(mua) * p21;
    pb = p3 + core::vector3df(mub) * p43;

    return true;
}

//! lineToPlane
bool lineToPlane(const core::vector3df& LineStart, const core::vector3df& LineVector,
                 const core::plane3df& Plane, core::vector3df& Isect)
{
    float t2 = Plane.Normal.dotProduct(LineVector);

    if (core::iszero(t2)) return false;

    float t = -(Plane.Normal.dotProduct(LineStart) + Plane.D) / t2;
    Isect = LineStart + (LineVector * t);

    return true;
}

//! This function was originally written by: Dan Sunday
//! In the article: "Intersections of Lines, Segments and Planes (2D and 3D)"
//! http://www.softsurfer.com/Archive/algorithm_0104/algorithm_0104B.htm
//! isectPlanes
bool isectPlanes(const core::plane3df& PlaneA, const core::plane3df& PlaneB, core::line3df& Isect)
{
    // cross product
    core::vector3df u = PlaneA.Normal.crossProduct(PlaneB.Normal);
    float ax = (u.X >= 0) ? u.X : -u.X;
    float ay = (u.Y >= 0) ? u.Y : -u.Y;
    float az = (u.Z >= 0) ? u.Z : -u.Z;

    // test if the two planes are parallel
    if ((ax + ay + az) <= core::ROUNDING_ERROR_f32) return false;

    // PointA and PointB intersect in a line
    // first determine max abs coordinate of cross product
    // next, to get a point on the intersect line
    // zero the max coord, and solve for the other two
    core::vector3df ip;

    // select max coordinate
    switch (ax > ay ? (ax > az ? 1 : 3) : (ay > az ? 2 : 3))
    {
        case 1: // intersect with X
        {
            ip.X = 0;
            ip.Y = (PlaneB.D * PlaneA.Normal.Z - PlaneA.D * PlaneB.Normal.Z) / u.X;
            ip.Z = (PlaneA.D * PlaneB.Normal.Y - PlaneB.D * PlaneA.Normal.Y) / u.X;
        }
        break;
        case 2: // intersect with Y
        {
            ip.X = (PlaneA.D * PlaneB.Normal.Z - PlaneB.D * PlaneA.Normal.Z) / u.Y;
            ip.Y = 0;
            ip.Z = (PlaneB.D * PlaneA.Normal.X - PlaneA.D * PlaneB.Normal.X) / u.Y;
        }
        break;
        case 3: // intersect with Z
        {
            ip.X = (PlaneB.D * PlaneA.Normal.Y - PlaneA.D * PlaneB.Normal.Y) / u.Z;
            ip.Y = (PlaneA.D * PlaneB.Normal.X - PlaneB.D * PlaneA.Normal.X) / u.Z;
            ip.Z = 0;
        }
        break;
    }

    // save return values
    Isect.start = ip;
    Isect.end = ip + u;

    return true;
}

//! isectBoxes
bool isectBoxes(const core::aabbox3df& BBoxA, const core::aabbox3df& BBoxB)
{
    return !(BBoxA.MinEdge.X > BBoxB.MaxEdge.X || BBoxA.MaxEdge.X < BBoxB.MinEdge.X ||
             BBoxA.MinEdge.Y > BBoxB.MaxEdge.Y || BBoxA.MaxEdge.Y < BBoxB.MinEdge.Y ||
             BBoxA.MinEdge.Z > BBoxB.MaxEdge.Z || BBoxA.MaxEdge.Z < BBoxB.MinEdge.Z);
}

//! samePoint
bool samePoint(const core::vector3df& PointA, const core::vector3df& PointB, float ToleranceSQ)
{ return PointA.getDistanceFromSQ(PointB) <= ToleranceSQ; }

//! frontFacingDir
bool frontFacingDir(const core::vector3df& DirA, const core::vector3df& DirB)
{ return DirA.dotProduct(DirB) > core::ROUNDING_ERROR_f32; }

//! rotateAroundAxis
void rotateAroundAxis(core::vector3df& RotationDegrees, const core::vector3df& Axis, float Degrees)
{
    core::vector3df euler;
    core::quaternion current, rota;

    // Get the balls current rotation and convert it to a quaternion
    euler.set(RotationDegrees * core::DEGTORAD);
    current.set(euler);

    // Create the rotation to be applied from the angle/axis
    // IMPORTANT: Axis has to be normalised.
    rota.fromAngleAxis(Degrees * core::PI, core::vector3df(Axis).normalize());

    // Apply the new ortation to the current orientation
    // Have to be rotation = current * rotation; !!!!!!!
    // Quaternion mulitplication is not commutative
    rota = current * rota;

    // convert to euler angles and apply result to node
    rota.toEuler(euler);

    RotationDegrees = euler * core::RADTODEG;
}


//! CCollTriangloid ----------------------------------------------------------------------------------------------------------------------
//! set
void CCollTriangloid::setFrom(const CCollTriangloid& Other)
{
    id = Other.id;
    tri = Other.tri;
    bbox = Other.bbox;
    norm = Other.norm;
    dist = Other.dist;
}

//! recalcBBox
void CCollTriangloid::recalcBBox()
{
    bbox.reset(tri.pointA);
    bbox.addInternalPoint(tri.pointB);
    bbox.addInternalPoint(tri.pointC);
}

//! recalcPlane
void CCollTriangloid::recalcPlane()
{
    norm = tri.getNormal().normalize();
    dist = -tri.pointA.dotProduct(norm);
}

//! recalcAll
void CCollTriangloid::recalcAll()
{
    recalcBBox();
    recalcPlane();
}

//! projectToPlane
void CCollTriangloid::projectToPlane(const core::vector3df& Point, core::vector3df& Proj) const
{
    float d = norm.dotProduct(Point) + dist;
    float t = d / norm.getLengthSQ();
    Proj = Point - (norm * t);
}

//! project
bool CCollTriangloid::project(const core::vector3df& Point, core::vector3df& Proj) const
{
    projectToPlane(Point, Proj);
    return tri.isPointInsideFast(Proj);
}

//! intersectPlane
bool CCollTriangloid::intersectPlane(const core::vector3df& Point, const core::vector3df& Vect, core::vector3df& Isect) const
{
    float t2 = norm.dotProduct(Vect);
    if (core::iszero(t2)) return false;
    float t = (norm.dotProduct(Point) + dist) / t2;
    Isect = Point - (Vect * t);
    return true;
}

//! intersect
bool CCollTriangloid::intersect(const core::vector3df& Point, const core::vector3df& Vect, core::vector3df& Isect) const
{
    if (!intersectPlane(Point, Vect, Isect)) return false;
    return tri.isPointInsideFast(Isect);
}

//! transform
void CCollTriangloid::transform(const core::matrix4& Matrix)
{
    Matrix.transformVect(tri.pointA);
    Matrix.transformVect(tri.pointB);
    Matrix.transformVect(tri.pointC);
}

//! degenerated
bool CCollTriangloid::degenerated(float Limit)
{
    float limitSQ = Limit * Limit;

    float dAB = tri.pointA.getDistanceFromSQ(tri.pointB);
    if (dAB <= limitSQ) return true;

    float dBC = tri.pointB.getDistanceFromSQ(tri.pointC);
    if (dBC <= limitSQ) return true;

    float dCA = tri.pointC.getDistanceFromSQ(tri.pointA);
    if (dCA <= limitSQ) return true;

    dAB = sqrt(dAB);
    dBC = sqrt(dBC);
    dCA = sqrt(dCA);
    float dmax = dAB;
    dmax = (dBC > dmax) ? dBC : dmax;
    dmax = (dCA > dmax) ? dCA : dmax;
    float rem = dAB + dBC + dCA - (dmax * 2);
    if (rem <= Limit) return true;

    return false;
}


} // end namespace COLL
