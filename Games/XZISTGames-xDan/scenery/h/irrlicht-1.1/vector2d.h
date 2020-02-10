// Copyright (C) 2002-2006 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#ifndef __IRR_POINT_2D_H_INCLUDED__
#define __IRR_POINT_2D_H_INCLUDED__

#include <math.h> 
#include "irrMath.h"

namespace irr
{
namespace core
{


//! 2d vector template class with lots of operators and methods.
template <class T>
class vector2d
{
public:

	vector2d() : X(0), Y(0) {};
	vector2d(T nx, T ny) : X(nx), Y(ny) {};
	vector2d(const vector2d<T>& other) : X(other.X), Y(other.Y) {};

	// operators

	vector2d<T> operator-() const { return vector2d<T>(-X, -Y);   }

	vector2d<T>& operator=(const vector2d<T>& other)	{ X = other.X; Y = other.Y; return *this; }

	vector2d<T> operator+(const vector2d<T>& other) const { return vector2d<T>(X + other.X, Y + other.Y);	}
	vector2d<T>& operator+=(const vector2d<T>& other)	{ X+=other.X; Y+=other.Y; return *this; }

	vector2d<T> operator-(const vector2d<T>& other) const { return vector2d<T>(X - other.X, Y - other.Y);	}
	vector2d<T>& operator-=(const vector2d<T>& other)	{ X-=other.X; Y-=other.Y; return *this; }

	vector2d<T> operator*(const vector2d<T>& other) const { return vector2d<T>(X * other.X, Y * other.Y);	}
	vector2d<T>& operator*=(const vector2d<T>& other)	{ X*=other.X; Y*=other.Y; return *this; }
	vector2d<T> operator*(const T v) const { return vector2d<T>(X * v, Y * v);	}
	vector2d<T>& operator*=(const T v) { X*=v; Y*=v; return *this; }

	vector2d<T> operator/(const vector2d<T>& other) const { return vector2d<T>(X / other.X, Y / other.Y);	}
	vector2d<T>& operator/=(const vector2d<T>& other)	{ X/=other.X; Y/=other.Y; return *this; }
	vector2d<T> operator/(const T v) const { return vector2d<T>(X / v, Y / v);	}
	vector2d<T>& operator/=(const T v) { X/=v; Y/=v; return *this; }

	bool operator==(const vector2d<T>& other) const { return other.X==X && other.Y==Y; }
	bool operator!=(const vector2d<T>& other) const { return other.X!=X || other.Y!=Y; }

	// functions

	void set(T nx, T ny) {X=nx; Y=ny; }
	void set(const vector2d<T>& p) { X=p.X; Y=p.Y;}

	//! Returns the length of the vector
	//! \return Returns the length of the vector.
	f64 getLength() const { return sqrt(X*X + Y*Y); }

	//! Returns the dot product of this vector with an other.
	T dotProduct(const vector2d<T>& other) const
	{
		return X*other.X + Y*other.Y;
	}

	//! Returns distance from an other point. Here, the vector is interpreted as
	//! point in 2 dimensional space.
	f64 getDistanceFrom(const vector2d<T>& other) const
	{
		return vector2d<T>(X - other.X, Y - other.Y).getLength();
	}

	//! rotates the point around a center by an amount of degrees.
	void rotateBy(f64 degrees, const vector2d<T>& center)
	{
		degrees *= GRAD_PI2;
		T cs = (T)cos(degrees);
		T sn = (T)sin(degrees);

		X -= center.X;
		Y -= center.Y;

		set(X*cs - Y*sn, X*sn + Y*cs);

		X += center.X;
		Y += center.Y;
	}

	//! normalizes the vector.
	vector2d<T>& normalize()
	{
		T l = (T)getLength();
		if (l == 0)
			return *this;

		l = (T)1.0 / l;
		X *= l;
		Y *= l;
		return *this;
	}

	//! Calculates the angle of this vector in grad in the trigonometric sense.
	//! This method has been suggested by Pr3t3nd3r.
	//! \return Returns a value between 0 and 360.
	f64 getAngleTrig() const
	{
		if (X == 0.0)
			return Y < 0.0 ? 270.0 : 90.0;
		else
		if (Y == 0)
			return X < 0.0 ? 180.0 : 0.0;

		if ( Y > 0.0)
			if (X > 0.0)
				return atan(Y/X) * GRAD_PI;
			else
				return 180.0-atan(Y/-X) * GRAD_PI;
		else
			if (X > 0.0)
				return 360.0-atan(-Y/X) * GRAD_PI;
			else
				return 180.0+atan(-Y/-X) * GRAD_PI;
	} 

	//! Calculates the angle of this vector in grad in the counter trigonometric sense.
	//! \return Returns a value between 0 and 360.
	inline f64 getAngle() const
	{
		if (Y == 0.0)  // corrected thanks to a suggestion by Jox
			return X < 0.0 ? 180.0 : 0.0; 
		else if (X == 0.0) 
			return Y < 0.0 ? 90.0 : 270.0;

		f64 tmp = Y / getLength();
		tmp = atan(sqrt(1 - tmp*tmp) / tmp) * GRAD_PI;

		if (X>0.0 && Y>0.0)
			return tmp + 270;
		else
		if (X>0.0 && Y<0.0)
			return tmp + 90;
		else
		if (X<0.0 && Y<0.0)
			return 90 - tmp;
		else
		if (X<0.0 && Y>0.0)
			return 270 - tmp;

		return tmp;
	}

	//! Calculates the angle between this vector and another one in grad.
	//! \return Returns a value between 0 and 90.
	inline f64 getAngleWith(const vector2d<T>& b) const
	{
		f64 tmp = X*b.X + Y*b.Y;

		if (tmp == 0.0)
			return 90.0;

		tmp = tmp / sqrt((X*X + Y*Y) * (b.X*b.X + b.Y*b.Y));
		if (tmp < 0.0) tmp = -tmp;

		return atan(sqrt(1 - tmp*tmp) / tmp) * GRAD_PI;
	}


	//! returns interpolated vector
	//! \param other: other vector to interpolate between
	//! \param d: value between 0.0f and 1.0f.
	vector2d<T> getInterpolated(const vector2d<T>& other, f32 d) const
	{
		f32 inv = 1.0f - d;
		return vector2d<T>(other.X*inv + X*d, other.Y*inv + Y*d);
	}

	//! sets this vector to the interpolated vector between a and b. 
	void interpolate(const vector2d<T>& a, const vector2d<T>& b, const f32 t)
	{
		X = b.X + ( ( a.X - b.X ) * t );
		Y = b.Y + ( ( a.Y - b.Y ) * t );
	}

	// member variables
	T X, Y;
};

	//! Typedef for f32 2d vector.
	typedef vector2d<f32> vector2df;
	//! Typedef for integer 2d vector.
	typedef vector2d<s32> vector2di;

	template<class S, class T> vector2d<T> operator*(const S scalar, const vector2d<T>& vector) { return vector*scalar; }

} // end namespace core
} // end namespace irr

#endif

