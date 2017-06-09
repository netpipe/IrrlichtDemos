#ifdef _WIN32
	#pragma once
#endif

//********************************************************************************************
// File:   VectorMath.h
// Purpose:
// Started: 21th May 2011
// Last ed: 24th October 2011
// History:
//********************************************************************************************

#ifndef __VECTORMATH_H__
#define __VECTORMATH_H__

#include <math.h>
#include <float.h>
#include <assert.h>
#include <irrlicht.h>
#include <cmath>
//#include <vector>

using namespace irr;
using namespace core;

#define VECTOR_CHECK	0
#define NULL_VECTOR vector3df(0.0f,0.0f,0.0f);

#if  VECTOR_CHECK
	#define VALID_VECTOR(vec)	assert(IsValidVector(vec))
#else
	#define VALID_VECTOR(vec)	0
#endif

#define VectorExpand(vec) (vec).X, (vec).Y, (vec).Z

//*****************************************************************************
// Vector related operations

inline bool IsNullVector(const vector3df& v);
inline bool IsValidVector(const vector3df& v);
inline void VectorClear(vector3df& a);
inline void VectorCopy(const vector3df& src, vector3df& dst);
inline void VectorAdd(const vector3df& a,const vector3df& b, vector3df& c);
inline void VectorSubtract(const vector3df& a,const vector3df& b, vector3df& c);
inline void VectorMultiply(const vector3df& a, float b, vector3df& result );
inline void VectorMultiply(const vector3df& a,const vector3df& b, vector3df& c);
inline void VectorDivide(const vector3df& a, float b, vector3df& result );
inline void VectorDivide(const vector3df& a,const vector3df& b, vector3df& c);
inline void VectorScale(const vector3df& in, float scale, vector3df& result );
inline bool VectorsAreEqual(const vector3df& src1, const vector3df& src2, float tolerance = 0.0f );
inline float VectorLength(const vector3df& v );
inline float DotProduct(vector3df const& a, vector3df const& b);
inline void CrossProduct(const vector3df& a,const  vector3df& b, vector3df &result);
inline void VectorMin(const vector3df& a,const  vector3df& b, vector3df &result);
inline void VectorMax(const vector3df& a,const  vector3df& b, vector3df &result);
inline void VectorLerp(vector3df const& src1, vector3df const& src2, float t, vector3df& dest );
inline void VectorAbs(const vector3df& src, vector3df& dst );
inline void VectorNormalizeFast(vector3df &vec);
inline float VectorNormalize(vector3df &vec);
inline void VectorMA(const vector3df& start, float scale, const vector3df& direction, vector3df& dest);
inline float Distance(const vector3df& from, const vector3df& to);

//*****************************************************************************

inline bool IsNullVector(const vector3df& v)
{
	return v.X == 0.0f && v.Y == 0.0f && v.Z == 0.0f;
}

//*****************************************************************************

inline bool IsValidVector(const vector3df& v)
{
	return std::isfinite(v.X) && std::isfinite(v.Y) && std::isfinite(v.Z);
}

//*****************************************************************************

inline void VectorCopy(const vector3df& src, vector3df& dst )
{
	VALID_VECTOR(src);
	dst.X = src.X;
	dst.Y = src.Y;
	dst.Z = src.Z;
}

//*****************************************************************************

inline void VectorClear(vector3df& a )
{
	a.X = a.Y = a.Z = 0.0f;
}

//*****************************************************************************

inline void VectorAdd(const vector3df& a,const vector3df& b, vector3df& c)
{
	VALID_VECTOR(a);
	VALID_VECTOR(b);
	c.X = a.X + b.X;
	c.Y = a.Y + b.Y;
	c.Z = a.Z + b.Z;
}

//*****************************************************************************

inline void VectorSubtract(const vector3df& a,const vector3df& b, vector3df& c)
{
	VALID_VECTOR(a);
	VALID_VECTOR(b);
	c.X = a.X - b.X;
	c.Y = a.Y - b.Y;
	c.Z = a.Z - b.Z;
}

//*****************************************************************************

inline void VectorMultiply(const vector3df& a, float b, vector3df& c )
{
	VALID_VECTOR(a);
	//assert( std::isfinite(b) );
	c.X = a.X * b;
	c.Y = a.Y * b;
	c.Z = a.Z * b;
}

//*****************************************************************************

inline void VectorMultiply(const vector3df& a,const vector3df& b, vector3df& c)
{
	VALID_VECTOR(a);
	VALID_VECTOR(b);
	c.X = a.X * b.X;
	c.Y = a.Y * b.Y;
	c.Z = a.Z * b.Z;
}

//*****************************************************************************

inline void VectorScale(const vector3df& in, float scale, vector3df& result )
{
	VectorMultiply(in, scale, result);
}

//*****************************************************************************

inline void VectorDivide(const vector3df& a, float b, vector3df& c )
{
	VALID_VECTOR(a);
	assert(b != 0.0f);
	float oob = 1.0f / b;
	c.X = a.X * oob;
	c.Y = a.Y * oob;
	c.Z = a.Z * oob;
}

//*****************************************************************************

inline void VectorDivide(const vector3df& a,const vector3df& b, vector3df& c)
{
	VALID_VECTOR(a);
	VALID_VECTOR(b);
	assert( (b.X != 0.0f) && (b.Y != 0.0f) && (b.Z != 0.0f) );
	c.X = a.X / b.X;
	c.Y = a.Y / b.Y;
	c.Z = a.Z / b.Z;
}

//*****************************************************************************

inline void VectorLerp(const vector3df& src1, const vector3df& src2, float t, vector3df& dest )
{
	VALID_VECTOR(src1);
	VALID_VECTOR(src2);
//	dest[0] = src1[0] + (src2[0] - src1[0]) * t;
//	dest[1] = src1[1] + (src2[1] - src1[1]) * t;
//	dest[2] = src1[2] + (src2[2] - src1[2]) * t;
    dest.X = src1.X + (src2.X - src1.X) * t;
	dest.Y = src1.Y + (src2.Y - src1.Y) * t;
	dest.Z = src1.Z + (src2.Z - src1.Z) * t;
}

//*****************************************************************************

inline float DotProduct(const vector3df& a, const vector3df& b)
{
	VALID_VECTOR(a);
	VALID_VECTOR(b);
	return( a.X*b.X + a.Y*b.Y + a.Z*b.Z );
}

//*****************************************************************************

inline void CrossProduct(const vector3df& a,const  vector3df& b, vector3df &result)
{
	VALID_VECTOR(a);
	VALID_VECTOR(b);
	result.X = a.Y*b.Z - a.Z*b.Y;
	result.Y = a.Z*b.X - a.X*b.Z;
	result.Z = a.X*b.Y - a.Y*b.X;
}

//*****************************************************************************

inline float VectorLength( const vector3df& v )
{
	VALID_VECTOR(v);
	return (float)squareroot(v.X * v.X + v.Y * v.Y + v.Z * v.Z + FLT_EPSILON);
}

//*****************************************************************************

inline bool VectorsAreEqual(const vector3df& src1, const vector3df& src2, float tolerance )
{
	if (std::abs(src1.X - src2.X) > tolerance)
		return false;
	if (std::abs(src1.Y - src2.Y) > tolerance)
		return false;
	return (std::abs(src1.Z - src2.Z) <= tolerance);
}

//*****************************************************************************

inline void VectorAbs(const vector3df& src, vector3df& dst )
{
	dst.X = std::abs(src.X);
	dst.Y = std::abs(src.Y);
	dst.Z = std::abs(src.Z);
}

//*****************************************************************************

inline void VectorMin(const vector3df& a,const  vector3df& b, vector3df &result)
{
	result.X = (a.X < b.X) ? a.X : b.X;
	result.Y = (a.Y < b.Y) ? a.Y : b.Y;
	result.Z = (a.Z < b.Z) ? a.Z : b.Z;
}

//*****************************************************************************

inline void VectorMax(const vector3df& a,const  vector3df& b, vector3df &result)
{
	result.X = (a.X > b.X) ? a.X : b.X;
	result.Y = (a.Y > b.Y) ? a.Y : b.Y;
	result.Z = (a.Z > b.Z) ? a.Z : b.Z;
}

//*****************************************************************************

inline void VectorNormalizeFast(vector3df &vec)
{
    float length = reciprocal_squareroot(vec.X * vec.X + vec.Y * vec.Y + vec.Z * vec.Z + FLT_EPSILON);
    vec.X *= length;
    vec.Y *= length;
    vec.Z *= length;
}
//*****************************************************************************

inline float VectorNormalize(vector3df &vec)
{
	float length = reciprocal_squareroot(vec.X * vec.X + vec.Y * vec.Y + vec.Z * vec.Z + FLT_EPSILON);
	vec.X *= length;
	vec.Y *= length;
	vec.Z *= length;
	return length;
}

//*****************************************************************************

inline void VectorMA(const vector3df& start, float scale, const vector3df& direction, vector3df& dest )
{
	dest.X = start.X + direction.X * scale;
	dest.Y = start.Y + direction.Y * scale;
	dest.Z = start.Z + direction.Z * scale;
}

//********************************************************************************************

inline float Distance(const vector3df& from, const vector3df& to)
{
	vector3df delta;
	VectorSubtract(from, to, delta);
	return VectorLength(delta);
}

//********************************************************************************************

#endif
