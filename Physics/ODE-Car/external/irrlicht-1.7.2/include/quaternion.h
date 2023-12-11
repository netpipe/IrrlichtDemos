// Copyright (C) 2002-2010 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#ifndef __IRR_QUATERNION_H_INCLUDED__
#define __IRR_QUATERNION_H_INCLUDED__

#include "irrTypes.h"
#include "irrMath.h"
#include "matrix4.h"
#include "vector3d.h"

namespace irr
{
namespace core
{

//! Quaternion class for representing rotations.
/** It provides cheap combinations and avoids gimbal locks.
Also useful for interpolations. */
class quaternion
{
	public:

		//! Default Constructor
		quaternion() : X(0.0f), Y(0.0f), Z(0.0f), W(1.0f) {}

		//! Constructor
		quaternion(f32 x, f32 y, f32 z, f32 w) : X(x), Y(y), Z(z), W(w) { }

		//! Constructor which converts euler angles (radians) to a quaternion
		quaternion(f32 x, f32 y, f32 z);

		//! Constructor which converts euler angles (radians) to a quaternion
		quaternion(const vector3df& vec);

		//! Constructor which converts a matrix to a quaternion
		quaternion(const matrix4& mat);

		//! Equalilty operator
		bool operator==(const quaternion& other) const;

		//! inequality operator
		bool operator!=(const quaternion& other) const;

		//! Assignment operator
		inline quaternion& operator=(const quaternion& other);

		//! Matrix assignment operator
		inline quaternion& operator=(const matrix4& other);

		//! Add operator
		quaternion operator+(const quaternion& other) const;

		//! Multiplication operator
		quaternion operator*(const quaternion& other) const;

		//! Multiplication operator with scalar
		quaternion operator*(f32 s) const;

		//! Multiplication operator with scalar
		quaternion& operator*=(f32 s);

		//! Multiplication operator
		vector3df operator*(const vector3df& v) const;

		//! Multiplication operator
		quaternion& operator*=(const quaternion& other);

		//! Calculates the dot product
		inline f32 dotProduct(const quaternion& other) const;

		//! Sets new quaternion
		inline quaternion& set(f32 x, f32 y, f32 z, f32 w);

		//! Sets new quaternion based on euler angles (radians)
		inline quaternion& set(f32 x, f32 y, f32 z);

		//! Sets new quaternion based on euler angles (radians)
		inline quaternion& set(const core::vector3df& vec);

		//! Sets new quaternion from other quaternion
		inline quaternion& set(const core::quaternion& quat);

		//! returns if this quaternion equals the other one, taking floating point rounding errors into account
		inline bool equals(const quaternion& other,
				const f32 tolerance = ROUNDING_ERROR_f32 ) const;

		//! Normalizes the quaternion
		inline quaternion& normalize();

		//! Creates a matrix from this quaternion
		matrix4 getMatrix() const;

		//! Creates a matrix from this quaternion
		void getMatrix( matrix4 &dest, const core::vector3df &translation ) const;

		/*!
			Creates a matrix from this quaternion
			Rotate about a center point
			shortcut for
			core::quaternion q;
			q.rotationFromTo ( vin[i].Normal, forward );
			q.getMatrixCenter ( lookat, center, newPos );

			core::matrix4 m2;
			m2.setInverseTranslation ( center );
			lookat *= m2;

			core::matrix4 m3;
			m2.setTranslation ( newPos );
			lookat *= m3;

		*/
		void getMatrixCenter( matrix4 &dest, const core::vector3df &center, const core::vector3df &translation ) const;

		//! Creates a matrix from this quaternion
		inline void getMatrix_transposed( matrix4 &dest ) const;

		//! Inverts this quaternion
		quaternion& makeInverse();

		//! Set this quaternion to the result of the interpolation between two quaternions
		quaternion& slerp( quaternion q1, quaternion q2, f32 interpolate );

		//! Create quaternion from rotation angle and rotation axis.
		/** Axis must be unit length.
		The quaternion representing the rotation is
		q = cos(A/2)+sin(A/2)*(x*i+y*j+z*k).
		\param angle Rotation Angle in radians.
		\param axis Rotation axis. */
		quaternion& fromAngleAxis (f32 angle, const vector3df& axis);

		//! Fills an angle (radians) around an axis (unit vector)
		void toAngleAxis (f32 &angle, core::vector3df& axis) const;

		//! Output this quaternion to an euler angle (radians)
		void toEuler(vector3df& euler) const;

		//! Set quaternion to identity
		quaternion& makeIdentity();

		//! Set quaternion to represent a rotation from one vector to another.
		quaternion& rotationFromTo(const vector3df& from, const vector3df& to);

		//! Quaternion elements.
		f32 X; // vectorial (imaginary) part
		f32 Y;
		f32 Z;
		f32 W; // real part
};


// Constructor which converts euler angles to a quaternion
inline quaternion::quaternion(f32 x, f32 y, f32 z)
{
	set(x,y,z);
}


// Constructor which converts euler angles to a quaternion
inline quaternion::quaternion(const vector3df& vec)
{
	set(vec.X,vec.Y,vec.Z);
}


// Constructor which converts a matrix to a quaternion
inline quaternion::quaternion(const matrix4& mat)
{
	(*this) = mat;
}


// equal operator
inline bool quaternion::operator==(const quaternion& other) const
{
	return ((X == other.X) &&
		(Y == other.Y) &&
		(Z == other.Z) &&
		(W == other.W));
}

// inequality operator
inline bool quaternion::operator!=(const quaternion& other) const
{
	return !(*this == other);
}

// assignment operator
inline quaternion& quaternion::operator=(const quaternion& other)
{
	X = other.X;
	Y = other.Y;
	Z = other.Z;
	W = other.W;
	return *this;
}


// matrix assignment operator
inline quaternion& quaternion::operator=(const matrix4& m)
{
	const f32 diag = m(0,0) + m(1,1) + m(2,2) + 1;

	if( diag > 0.0f )
	{
		const f32 scale = sqrtf(diag) * 2.0f; // get scale from diagonal

		// TODO: speed this up
		X = ( m(2,1) - m(1,2)) / scale;
		Y = ( m(0,2) - m(2,0)) / scale;
		Z = ( m(1,0) - m(0,1)) / scale;
		W = 0.25f * scale;
	}
	else
	{
		if ( m(0,0) > m(1,1) && m(0,0) > m(2,2))
		{
			// 1st element of diag is greatest value
			// find scale according to 1st element, and double it
			const f32 scale = sqrtf( 1.0f + m(0,0) - m(1,1) - m(2,2)) * 2.0f;

			// TODO: speed this up
			X = 0.25f * scale;
			Y = (m(0,1) + m(1,0)) / scale;
			Z = (m(2,0) + m(0,2)) / scale;
			W = (m(2,1) - m(1,2)) / scale;
		}
		else if ( m(1,1) > m(2,2))
		{
			// 2nd element of diag is greatest value
			// find scale according to 2nd element, and double it
			const f32 scale = sqrtf( 1.0f + m(1,1) - m(0,0) - m(2,2)) * 2.0f;

			// TODO: speed this up
			X = (m(0,1) + m(1,0) ) / scale;
			Y = 0.25f * scale;
			Z = (m(1,2) + m(2,1) ) / scale;
			W = (m(0,2) - m(2,0) ) / scale;
		}
		else
		{
			// 3rd element of diag is greatest value
			// find scale according to 3rd element, and double it
			const f32 scale = sqrtf( 1.0f + m(2,2) - m(0,0) - m(1,1)) * 2.0f;

			// TODO: speed this up
			X = (m(0,2) + m(2,0)) / scale;
			Y = (m(1,2) + m(2,1)) / scale;
			Z = 0.25f * scale;
			W = (m(1,0) - m(0,1)) / scale;
		}
	}

	return normalize();
}


// multiplication operator
inline quaternion quaternion::operator*(const quaternion& other) const
{
	quaternion tmp;

	tmp.W = (other.W * W) - (other.X * X) - (other.Y * Y) - (other.Z * Z);
	tmp.X = (other.W * X) + (other.X * W) + (other.Y * Z) - (other.Z * Y);
	tmp.Y = (other.W * Y) + (other.Y * W) + (other.Z * X) - (other.X * Z);
	tmp.Z = (other.W * Z) + (other.Z * W) + (other.X * Y) - (other.Y * X);

	return tmp;
}


// multiplication operator
inline quaternion quaternion::operator*(f32 s) const
{
	return quaternion(s*X, s*Y, s*Z, s*W);
}

// multiplication operator
inline quaternion& quaternion::operator*=(f32 s)
{
	X*=s;
	Y*=s;
	Z*=s;
	W*=s;
	return *this;
}

// multiplication operator
inline quaternion& quaternion::operator*=(const quaternion& other)
{
	return (*this = other * (*this));
}

// add operator
inline quaternion quaternion::operator+(const quaternion& b) const
{
	return quaternion(X+b.X, Y+b.Y, Z+b.Z, W+b.W);
}


// Creates a matrix from this quaternion
inline matrix4 quaternion::getMatrix() const
{
	core::matrix4 m;
	getMatrix_transposed(m);
	return m;
}


/*!
	Creates a matrix from this quaternion
*/
inline void quaternion::getMatrix( matrix4 &dest, const core::vector3df &center ) const
{
	f32 * m = dest.pointer();

	m[0] = 1.0f - 2.0f*Y*Y - 2.0f*Z*Z;
	m[1] = 2.0f*X*Y + 2.0f*Z*W;
	m[2] = 2.0f*X*Z - 2.0f*Y*W;
	m[3] = 0.0f;

	m[4] = 2.0f*X*Y - 2.0f*Z*W;
	m[5] = 1.0f - 2.0f*X*X - 2.0f*Z*Z;
	m[6] = 2.0f*Z*Y + 2.0f*X*W;
	m[7] = 0.0f;

	m[8] = 2.0f*X*Z + 2.0f*Y*W;
	m[9] = 2.0f*Z*Y - 2.0f*X*W;
	m[10] = 1.0f - 2.0f*X*X - 2.0f*Y*Y;
	m[11] = 0.0f;

	m[12] = center.X;
	m[13] = center.Y;
	m[14] = center.Z;
	m[15] = 1.f;

	//dest.setDefinitelyIdentityMatrix ( matrix4::BIT_IS_NOT_IDENTITY );
	dest.setDefinitelyIdentityMatrix ( false );
}



/*!
	Creates a matrix from this quaternion
	Rotate about a center point
	shortcut for
	core::quaternion q;
	q.rotationFromTo ( vin[i].Normal, forward );
	q.getMatrix ( lookat, center );

	core::matrix4 m2;
	m2.setInverseTranslation ( center );
	lookat *= m2;
*/
inline void quaternion::getMatrixCenter(matrix4 &dest,
					const core::vector3df &center,
					const core::vector3df &translation) const
{
	f32 * m = dest.pointer();

	m[0] = 1.0f - 2.0f*Y*Y - 2.0f*Z*Z;
	m[1] = 2.0f*X*Y + 2.0f*Z*W;
	m[2] = 2.0f*X*Z - 2.0f*Y*W;
	m[3] = 0.0f;

	m[4] = 2.0f*X*Y - 2.0f*Z*W;
	m[5] = 1.0f - 2.0f*X*X - 2.0f*Z*Z;
	m[6] = 2.0f*Z*Y + 2.0f*X*W;
	m[7] = 0.0f;

	m[8] = 2.0f*X*Z + 2.0f*Y*W;
	m[9] = 2.0f*Z*Y - 2.0f*X*W;
	m[10] = 1.0f - 2.0f*X*X - 2.0f*Y*Y;
	m[11] = 0.0f;

	dest.setRotationCenter ( center, translation );
}

// Creates a matrix from this quaternion
inline void quaternion::getMatrix_transposed( matrix4 &dest ) const
{
	dest[0] = 1.0f - 2.0f*Y*Y - 2.0f*Z*Z;
	dest[4] = 2.0f*X*Y + 2.0f*Z*W;
	dest[8] = 2.0f*X*Z - 2.0f*Y*W;
	dest[12] = 0.0f;

	dest[1] = 2.0f*X*Y - 2.0f*Z*W;
	dest[5] = 1.0f - 2.0f*X*X - 2.0f*Z*Z;
	dest[9] = 2.0f*Z*Y + 2.0f*X*W;
	dest[13] = 0.0f;

	dest[2] = 2.0f*X*Z + 2.0f*Y*W;
	dest[6] = 2.0f*Z*Y - 2.0f*X*W;
	dest[10] = 1.0f - 2.0f*X*X - 2.0f*Y*Y;
	dest[14] = 0.0f;

	dest[3] = 0.f;
	dest[7] = 0.f;
	dest[11] = 0.f;
	dest[15] = 1.f;
	//dest.setDefinitelyIdentityMatrix ( matrix4::BIT_IS_NOT_IDENTITY );
	dest.setDefinitelyIdentityMatrix ( false );
}



// Inverts this quaternion
inline quaternion& quaternion::makeInverse()
{
	X = -X; Y = -Y; Z = -Z;
	return *this;
}

// sets new quaternion
inline quaternion& quaternion::set(f32 x, f32 y, f32 z, f32 w)
{
	X = x;
	Y = y;
	Z = z;
	W = w;
	return *this;
}


// sets new quaternion based on euler angles
inline quaternion& quaternion::set(f32 x, f32 y, f32 z)
{
	f64 angle;

	angle = x * 0.5;
	const f64 sr = sin(angle);
	const f64 cr = cos(angle);

	angle = y * 0.5;
	const f64 sp = sin(angle);
	const f64 cp = cos(angle);

	angle = z * 0.5;
	const f64 sy = sin(angle);
	const f64 cy = cos(angle);

	const f64 cpcy = cp * cy;
	const f64 spcy = sp * cy;
	const f64 cpsy = cp * sy;
	const f64 spsy = sp * sy;

	X = (f32)(sr * cpcy - cr * spsy);
	Y = (f32)(cr * spcy + sr * cpsy);
	Z = (f32)(cr * cpsy - sr * spcy);
	W = (f32)(cr * cpcy + sr * spsy);

	return normalize();
}

// sets new quaternion based on euler angles
inline quaternion& quaternion::set(const core::vector3df& vec)
{
	return set(vec.X, vec.Y, vec.Z);
}

// sets new quaternion based on other quaternion
inline quaternion& quaternion::set(const core::quaternion& quat)
{
	return (*this=quat);
}


//! returns if this quaternion equals the other one, taking floating point rounding errors into account
inline bool quaternion::equals(const quaternion& other, const f32 tolerance) const
{
	return core::equals(X, other.X, tolerance) &&
		core::equals(Y, other.Y, tolerance) &&
		core::equals(Z, other.Z, tolerance) &&
		core::equals(W, other.W, tolerance);
}


// normalizes the quaternion
inline quaternion& quaternion::normalize()
{
	const f32 n = X*X + Y*Y + Z*Z + W*W;

	if (n == 1)
		return *this;

	//n = 1.0f / sqrtf(n);
	return (*this *= reciprocal_squareroot ( n ));
}


// set this quaternion to the result of the interpolation between two quaternions
inline quaternion& quaternion::slerp(quaternion q1, quaternion q2, f32 time)
{
	f32 angle = q1.dotProduct(q2);

	if (angle < 0.0f)
	{
		q1 *= -1.0f;
		angle *= -1.0f;
	}

	f32 scale;
	f32 invscale;

	if ((angle + 1.0f) > 0.05f)
	{
		if ((1.0f - angle) >= 0.05f) // spherical interpolation
		{
			const f32 theta = acosf(angle);
			const f32 invsintheta = reciprocal(sinf(theta));
			scale = sinf(theta * (1.0f-time)) * invsintheta;
			invscale = sinf(theta * time) * invsintheta;
		}
		else // linear interploation
		{
			scale = 1.0f - time;
			invscale = time;
		}
	}
	else
	{
		q2.set(-q1.Y, q1.X, -q1.W, q1.Z);
		scale = sinf(PI * (0.5f - time));
		invscale = sinf(PI * time);
	}

	return (*this = (q1*scale) + (q2*invscale));
}


// calculates the dot product
inline f32 quaternion::dotProduct(const quaternion& q2) const
{
	return (X * q2.X) + (Y * q2.Y) + (Z * q2.Z) + (W * q2.W);
}


//! axis must be unit length
//! angle in radians
inline quaternion& quaternion::fromAngleAxis(f32 angle, const vector3df& axis)
{
	const f32 fHalfAngle = 0.5f*angle;
	const f32 fSin = sinf(fHalfAngle);
	W = cosf(fHalfAngle);
	X = fSin*axis.X;
	Y = fSin*axis.Y;
	Z = fSin*axis.Z;
	return *this;
}


inline void quaternion::toAngleAxis(f32 &angle, core::vector3df &axis) const
{
	const f32 scale = sqrtf(X*X + Y*Y + Z*Z);

	if (core::iszero(scale) || W > 1.0f || W < -1.0f)
	{
		angle = 0.0f;
		axis.X = 0.0f;
		axis.Y = 1.0f;
		axis.Z = 0.0f;
	}
	else
	{
		const f32 invscale = reciprocal(scale);
		angle = 2.0f * acosf(W);
		axis.X = X * invscale;
		axis.Y = Y * invscale;
		axis.Z = Z * invscale;
	}
}

inline void quaternion::toEuler(vector3df& euler) const
{
	const f64 sqw = W*W;
	const f64 sqx = X*X;
	const f64 sqy = Y*Y;
	const f64 sqz = Z*Z;

	// heading = rotation about z-axis
	euler.Z = (f32) (atan2(2.0 * (X*Y +Z*W),(sqx - sqy - sqz + sqw)));

	// bank = rotation about x-axis
	euler.X = (f32) (atan2(2.0 * (Y*Z +X*W),(-sqx - sqy + sqz + sqw)));

	// attitude = rotation about y-axis
	euler.Y = asinf( clamp(-2.0f * (X*Z - Y*W), -1.0f, 1.0f) );
}


inline vector3df quaternion::operator* (const vector3df& v) const
{
	// nVidia SDK implementation

	vector3df uv, uuv;
	vector3df qvec(X, Y, Z);
	uv = qvec.crossProduct(v);
	uuv = qvec.crossProduct(uv);
	uv *= (2.0f * W);
	uuv *= 2.0f;

	return v + uv + uuv;
}

// set quaternion to identity
inline core::quaternion& quaternion::makeIdentity()
{
	W = 1.f;
	X = 0.f;
	Y = 0.f;
	Z = 0.f;
	return *this;
}

inline core::quaternion& quaternion::rotationFromTo(const vector3df& from, const vector3df& to)
{
	// Based on Stan Melax's article in Game Programming Gems
	// Copy, since cannot modify local
	vector3df v0 = from;
	vector3df v1 = to;
	v0.normalize();
	v1.normalize();

	const f32 d = v0.dotProduct(v1);
	if (d >= 1.0f) // If dot == 1, vectors are the same
	{
		return makeIdentity();
	}
	else if (d <= -1.0f) // exactly opposite
	{
		core::vector3df axis(1.0f, 0.f, 0.f);
		axis = axis.crossProduct(core::vector3df(X,Y,Z));
		if (axis.getLength()==0)
		{
			axis.set(0.f,1.f,0.f);
			axis.crossProduct(core::vector3df(X,Y,Z));
		}
		return this->fromAngleAxis(core::PI, axis);
	}

	const f32 s = sqrtf( (1+d)*2 ); // optimize inv_sqrt
	const f32 invs = 1.f / s;
	const vector3df c = v0.crossProduct(v1)*invs;
	X = c.X;
	Y = c.Y;
	Z = c.Z;
	W = s * 0.5f;

	return *this;
}


} // end namespace core
} // end namespace irr

#endif

