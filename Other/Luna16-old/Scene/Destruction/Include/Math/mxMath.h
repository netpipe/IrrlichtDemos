/*
===============================================================================
	File:	mxMath.h
	Desc:
===============================================================================
*/

#ifndef __MX_MATH_H__
#define __MX_MATH_H__

namespace mix {

//==================================================================================

//
// Numerical precision epsilons
//
#define SMALL_EPSILON			0.000001f
#define VECTOR_EPSILON			0.001f
#define VECTOR_EPSILON_SQUARED	1e-06f
#define PLANESIDE_EPSILON		0.1f	// "on-plane" epsilon

//==================================================================================

// This enum describes spatial relation of an object to a plane.
// The order is important! Don't change!
enum E_PlaneSide
{
	SIDE_FRONT,
	SIDE_BACK,
	SIDE_ON,
	SIDE_CROSS,
};

//
//	E_PlaneType
//
// Plane types are used to speed some tests.
// 0-2 are axial planes.
// 3-5 are non-axial planes snapped to the nearest.
//
enum E_PlaneType
{
	PLANE_NON_AXIAL	= 0,
	PLANE_X			= BIT( 0 ),
	PLANE_Y			= BIT( 1 ),
	PLANE_Z			= BIT( 2 ),
	PLANE_XY		= ( PLANE_X | PLANE_Y ),
	PLANE_XZ		= ( PLANE_X | PLANE_Z ),
	PLANE_YZ		= ( PLANE_Y | PLANE_Z ),
	PLANE_XYZ		= ( PLANE_X | PLANE_Y | PLANE_Z ),
};

inline E_PlaneType   PlaneTypeForNormal( const Vec3D& normal )
{
	const f32 * p = reinterpret_cast< const f32* >( &normal );
	if ( p[0] == 1.0 || p[0] == -1.0 ) {
		return PLANE_X;
	}
	if ( p[1] == 1.0 || p[1] == -1.0 ) {
		return PLANE_Y;
	}
	if ( p[2] == 1.0 || p[2] == -1.0 ) {
		return PLANE_Z;
	}
	return PLANE_NON_AXIAL;
}

//
//	E_AxisType
//
enum E_AxisType
{
	NonAxial,
	NegativeX,	PositiveX,
	NegativeY,	PositiveY,
	NegativeZ,	PositiveZ,
};

// Returns the major axis of a vector.
//E.g., if the vector is pointing mostly in the direction of positive y, this will return PositiveY. And so on.
E_AxisType  MajorAxisForNormal( const Vec3D& normal );

//==================================================================================

//
//	mxPlane
//
class mxPlane : public TPlane3D {
public:
	void	Set( const TPlane3D& plane );
	void	Set( const Vec3D& pointA, const Vec3D& pointB, const Vec3D& pointC );

	E_PlaneSide  ClassifyPointRelation_epsilon	( const Vec3D& point, const f32 epsilon = PLANESIDE_EPSILON ) const;
	E_PlaneSide  ClassifyPointRelation			( const Vec3D& point ) const;

	// Classify the box with respect to this plane. Returns SIDE_BACK or SIDE_FRONT or SIDE_CROSS.
	E_PlaneSide  ClassifyVolumeRelation( const AABB& bv, const f32 epsilon = PLANESIDE_EPSILON ) const;

	f32		DistanceTo( const Vec3D& point ) const;
	bool	LineIntersectsPlane( const Vec3D& pointA, const Vec3D& pointB, float &fraction ) const;

	// Are these optimizations really 'optimizations' ?
	//byte			flags;	// signx + (signy<<1) + (signz<<2), used as lookup during collision
	//E_PlaneType	type;	// for fast side tests
};

//==================================================================================

inline bool IsPowerOfTwo( int x ) {
	return ( (x & (x - 1)) == 0 ) && (x > 0);
}

//------------------------------------------------------------------------------------------------------

inline bool VectorsEqual( const Vec3D& a, const Vec3D& b, const f32 epsilon = VECTOR_EPSILON )
{
	if ( idMath::Fabs( a.X - b.X ) > epsilon ) {
		return false;
	}
	if ( idMath::Fabs( a.Y - b.Y ) > epsilon ) {
		return false;
	}
	if ( idMath::Fabs( a.Z - b.Z ) > epsilon ) {
		return false;
	}
	return true;
}

//------------------------------------------------------------------------------------------------------

// Takes a floating point value as input and converts it into an integer by trimming the decimal point.
//
inline int FtoI( float flt )
{
    int i;
  //  _asm
  //  {
      asm(" mov  EAX, flt;"); //loaded mem to acc

        asm("rcl  EAX, 1;");   //left shift acc to remove the sign

        asm("mov  EBX, EAX;"); //save the acc

        asm("mov  EDX, 4278190080;"); //clear reg edx;

        asm("and  EAX, EDX;"); //and acc to retrieve the exponent

        asm("shr  EAX, 24;");
        asm("sub  EAX, 7fh;"); //subtract 7fh(127) to get the actual power

        asm("mov  EDX, EAX;"); //save acc val power

        asm("mov  EAX, EBX; ");//retrieve from ebx

        asm("rcl  EAX, 8;");     //trim the left 8 bits that contain the power

        asm("mov  EBX, EAX;"); //store

       asm("mov  ECX, 1fh;"); //subtract 17 h

        asm("sub  ECX, EDX; ");
        asm("mov  EDX, 00000000h;");
        asm("cmp  ECX, 0;");

        asm("je   loop2;");

        asm("shr  EAX, 1;");
        asm("or   EAX, 80000000h;");
asm("loop1:");
        asm("shr  EAX, 1;"); //shift (total bits - power bits);

        asm("sub  ECX, 1;");
        asm("add  EDX, 1;");
        asm("cmp  ECX, 0;");
        asm("ja   loop1;");
asm("loop2:");
        asm("mov  i, EAX;");

//check sign +/- :
        asm("mov  EAX, flt;");
        asm("and  EAX, 80000000h;");
        asm("cmp  EAX, 80000000h;");
        asm("je     putsign;");
 //   }

    return i;

putsign:
    return -i;
}

//------------------------------------------------------------------------------------------------------

bool SphereIntersectsSphere( const Vec3D& center0, f32 radius0, const Vec3D& center1, f32 radius1 );
bool SphereIntersectsFrustum( const Vec3D& center, f32 radius, const TViewFrustum& frustum );

//------------------------------------------------------------------------------------------------------

//
//	PluekerCoords
//
class PluekerCoords {
public:
private:
	Vec3D	A, B;
};

}//end of namespace mix

#endif /* ! __MX_MATH_H__ */

