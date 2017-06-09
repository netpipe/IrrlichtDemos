// FunCollision -- Collision detection and response system -- Created by Peter Szollosi, 2009-2010.
// For conditions of distribution and use, see copyright notice in license.txt or FunCollision.h

#ifndef CCOLLCOMMON_H_INCLUDED
#define CCOLLCOMMON_H_INCLUDED

#include <irrlicht.h>
using namespace irr;


namespace COLL
{

//! consts
/*  Constant for small movements to be cropped (1.0f).
    If this value is too high, it can lead to squared movement, or no movement at all.
    The right value depends on the sizes of your scene and other settings. */
const float COLL_MIN_LENGTH_SQ = 1.0f;

/*  Default value for keeping the entities "outside" (0.001f).
    The smaller it is, the more stickier the movement can be, or if it's too high,
    it can cause constant jumping. */
const float COLL_DEF_PUSH_OUT = 0.001f;

/*  Maximum number of loops per object (5).
    Lowering this can lead to skipping collisions. */
const u32 COLL_MAX_ITERATIONS = 5;

/*  Number of milliseconds a period lasts (10).
    Changing this value will affect physical computations. */
const u32 COLL_DEF_TIME_PERIOD = 10;

/*  Limit for automatic awakening (100).
    This value controls how often the entities should be awaken automatically. */
const u32 COLL_DEF_AUTO_AWAKE = 100;

/*  Changing the constants below might lead to unexpected behaviour. */
const float COLL_MAX_DISTANCE = FLT_MAX;
const float COLL_SQRT_2 = 1.4142135f;


//! Math -----------------------------------------------------------------------------------------------------------------------------
//! pointToLine
core::vector3df pointToLine(const core::vector3df& Point, const core::line3df& Line);

//! pointOnSegment
bool pointOnSegment(const core::vector3df& Point, const core::line3df& Segment, float ToleranceSQ);

//! lineToLine
bool lineToLine(const core::vector3df& p1, const core::vector3df& p2,
                const core::vector3df& p3, const core::vector3df& p4,
                core::vector3df& pa, core::vector3df& pb);

//! lineToPlane
bool lineToPlane(const core::vector3df& LineStart, const core::vector3df& LineVector,
                 const core::plane3df& Plane, core::vector3df& Isect);

//! This function was originally written by: Dan Sunday
//! In the article: "Intersections of Lines, Segments and Planes (2D and 3D)"
//! http://www.softsurfer.com/Archive/algorithm_0104/algorithm_0104B.htm
//! isectPlanes
bool isectPlanes(const core::plane3df& PlaneA, const core::plane3df& PlaneB, core::line3df& Isect);

//! isectBoxes
bool isectBoxes(const core::aabbox3df& BBoxA, const core::aabbox3df& BBoxB);

//! samePoint
bool samePoint(const core::vector3df& PointA, const core::vector3df& PointB, float ToleranceSQ);

//! frontFacingDir
bool frontFacingDir(const core::vector3df& DirA, const core::vector3df& DirB);

//! rotateAroundAxis
void rotateAroundAxis(core::vector3df& RotationDegrees, const core::vector3df& Axis, float Degrees);


//! CCollTriangloid ----------------------------------------------------------------------------------------------------------------------
//! CCollTriangloid class
class CCollTriangloid
{
    public:
        //! empty constructor
        CCollTriangloid(u32 ID = 0) { dist = 0; id = ID; }

        //! constructor
        CCollTriangloid(const core::triangle3df& Tri, u32 ID = 0) { dist = 0; setTri(Tri); id = ID; }

        //! constructor
        CCollTriangloid(const core::vector3df& PointA, const core::vector3df& PointB, const core::vector3df& PointC, u32 ID = 0)
        { dist = 0; setTri(PointA, PointB, PointC); id = ID; }

        //! copy constructor
        CCollTriangloid(const CCollTriangloid& Other) { setFrom(Other); }

        //! get functions
        const u32 getID() const { return id; }
        const core::triangle3df& getTri() const { return tri; }
        const core::line3df getEdgeAB() const { return core::line3df(tri.pointA, tri.pointB); }
        const core::line3df getEdgeBC() const { return core::line3df(tri.pointB, tri.pointC); }
        const core::line3df getEdgeCA() const { return core::line3df(tri.pointC, tri.pointA); }
        const core::vector3df& getPointA() const { return tri.pointA; }
        const core::vector3df& getPointB() const { return tri.pointB; }
        const core::vector3df& getPointC() const { return tri.pointC; }
        const core::aabbox3df& getBBox() const { return bbox; }
        const core::vector3df& getNorm() const { return norm; }

        //! set
        inline void setID(u32 ID) { id = ID; }
        inline void setTri(const core::triangle3df& Tri) { tri = Tri; }
        inline void setTri(const core::vector3df& PointA, const core::vector3df& PointB,
                           const core::vector3df& PointC) { tri.set(PointA, PointB, PointC); }
        void setFrom(const CCollTriangloid& Other);

        //! Recalcuclates the bbox
        void recalcBBox();

        //! Recalcuclates the plane
        void recalcPlane();

        //! Recalculates the bbox and plane
        void recalcAll();

        //! pointInside
        inline bool pointInside(const core::vector3df& Point) const { return tri.isPointInsideFast(Point); }

        //! Projects a point onto the tri's plane
        void projectToPlane(const core::vector3df& Point, core::vector3df& Proj) const;

        //! Projects a point onto the tri
        bool project(const core::vector3df& Point, core::vector3df& Proj) const;

        //! Intersects the triangle plane with a line
        bool intersectPlane(const core::vector3df& Point, const core::vector3df& Vect, core::vector3df& Isect) const;

        //! Intersects the triangle with a line
        bool intersect(const core::vector3df& Point, const core::vector3df& Vect, core::vector3df& Isect) const;

        //! Transforms the tri with a matrix
        void transform(const core::matrix4& Matrix);

        //! Checks if the tri intersects with the given bounding box
        inline bool isectsBBox(const core::aabbox3df& BBox) const { return isectBoxes(bbox, BBox); }

        //! Checks if the tri is inside of the given bounding box
        inline bool insideBBox(const core::aabbox3df& BBox) const { return bbox.isFullInside(BBox); }

        //! Calculates the distance from the tri's plane to the given point
        inline float getDistance(const core::vector3df& Point) const { return norm.dotProduct(Point) + dist; }

        //! Checks if the given point faces the tri
        inline bool frontFacingPoint(const core::vector3df& Point) const { return getDistance(Point) > core::ROUNDING_ERROR_f32; }

        //! Checks if the given direction faces the tri
        inline bool frontFacingDir(const core::vector3df& Dir) const { return norm.dotProduct(Dir) > core::ROUNDING_ERROR_f32; }

        //! Decides if the triangle is degenerated or not
        bool degenerated(float Limit = core::ROUNDING_ERROR_f32);

    private:
        //! vars
        u32 id;
        core::triangle3df tri;
        core::aabbox3df bbox;
        core::vector3df norm;
        float dist;

}; // end class CCollTriangloid


//! SCollContact ----------------------------------------------------------------------------------------------------------------------
//! forward decl.
class ICollEntity;
class ICollObject;

//! SCollContact struct
struct SCollContact
{
    //! empty constructor
    SCollContact()
    {
        distSQ = COLL_MAX_DISTANCE;
        entity = NULL;
        object = NULL;
    }

    //! copy constructor
    SCollContact(const SCollContact& Other)
    {
        distSQ = Other.distSQ;
        impact = Other.impact;
        normal = Other.normal;
        closest = Other.closest;
        tri = Other.tri;
        entity = Other.entity;
        object = Other.object;
    }

    //! vars
    float distSQ;               // (for inner use only)
    core::vector3df impact;     // first encounter
    core::vector3df normal;     // face out normal
    core::vector3df closest;    // closest point
    CCollTriangloid tri;        // collision triangloid
    ICollEntity* entity;        // collision entity
    ICollObject* object;        // collision object

}; // end struct SCollContact


//! Misc ---------------------------------------------------------------------------------------------------------------------------------
//! ECollEntityState
enum ECollEntityState
{
    ECES_FALLING = 0,
    ECES_SLIDING,
    ECES_STANDING
};

} // end namespace COLL

#endif // CCOLLCOMMON_H_INCLUDED
