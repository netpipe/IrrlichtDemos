#include <stdlib.h>
#include <time.h>
#include "Vector3f.h"
#include <cmath>

//-------------------------------------------------------------------------
// constructors
//-------------------------------------------------------------------------

	vector3f::vector3f()
	{
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
	}

	//---------------------------------------------------------------------

	vector3f::vector3f(float x_, float y_, float z_)
	{
		x = x_;
		y = y_;
		z = z_;
	}

//-------------------------------------------------------------------------
// member functions
//-------------------------------------------------------------------------

	const float vector3f::Distance(const vector3f p) const
	{
		Vector(p);
		return p.Length();
	}

	//---------------------------------------------------------------------

	const vector3f vector3f::MidPoint(const vector3f p) const
	{
		return vector3f((x+p.x)*0.5f, (y+p.y)*0.5f, (z+p.z)*0.5f);
	}

	//---------------------------------------------------------------------

	const vector3f vector3f::Vector(const vector3f p) const
	{
		return vector3f(p.x - x, p.y - y, p.z - z);
	}

	//---------------------------------------------------------------------

	const float vector3f::Length() const
	{
		return sqrtf((x * x) + (y * y) + (z * z));
	}

	//---------------------------------------------------------------------

	const float vector3f::LengthSquared() const
	{
		return (x * x) + (y * y) + (z * z);
	}

	//---------------------------------------------------------------------

	const vector3f vector3f::Normal() const
	{
		float length = Length();
		if (length == 0.0f) length = 1.0f;
		float avoid_div = 1.0f / length;
		return vector3f(avoid_div * x, avoid_div * y, avoid_div * z);
	}

	//---------------------------------------------------------------------

	const vector3f vector3f::Project(vector3f v) const
	{
		return v * (DotProduct((*this), v) / v.LengthSquared());
	}

	//---------------------------------------------------------------------

	void vector3f::Normalize()
	{
		float length = Length();
		if (length == 0.0f) length = 1.0f;
		float avoid_div = 1.0f / length;
		x *= avoid_div;
		y *= avoid_div;
		z *= avoid_div;
	}

	//---------------------------------------------------------------------

	void vector3f::Set(const float x_, const float y_, const float z_)
	{
		x = x_;
		y = y_;
		z = z_;
	}

//-------------------------------------------------------------------------
// overloaded operators
//-------------------------------------------------------------------------

//	assignment operators

	void vector3f::operator = (const float s)
	{
		x = s;
		y = s;
		z = s;
	}

	//-------------------------------------------------------------------------

	const vector3f vector3f::operator /= (const float s)
	{
		float avoid_div = 1.0f / s;
		x *= avoid_div;
		y *= avoid_div;
		z *= avoid_div;
		return (*this);
	}

	//-------------------------------------------------------------------------

	const vector3f vector3f::operator *= (const float s)
	{
		x *= s;
		y *= s;
		z *= s;
		return (*this);
	}

	//-------------------------------------------------------------------------

	const vector3f vector3f::operator += (const float s)
	{
		x += s;
		y += s;
		z += s;
		return (*this);
	}

	//-------------------------------------------------------------------------

	const vector3f vector3f::operator -= (const float s)
	{
		x -= s;
		y -= s;
		z -= s;
		return (*this);
	}

	//-------------------------------------------------------------------------

	void vector3f::operator = (const vector3f v)
	{
		x = v.x;
		y = v.y;
		z = v.z;
	}

	//-------------------------------------------------------------------------

	const vector3f vector3f::operator += (vector3f v)
	{
		x += v.x;
		y += v.y;
		z += v.z;
		return (*this);
	}

	//-------------------------------------------------------------------------

	const vector3f vector3f::operator -= (vector3f v)
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
		return (*this);
	}

// unary and binary operators

	const vector3f vector3f::operator / (const float s) const
	{
		float avoid_div = 1.0f / s;
		return vector3f(x * avoid_div, y * avoid_div, z * avoid_div);
	}

	//-------------------------------------------------------------------------

	const vector3f vector3f::operator * (const float s) const
	{
		return vector3f(x * s, y * s, z * s);
	}

	//-------------------------------------------------------------------------

	const vector3f vector3f::operator + (const vector3f v) const
	{
		return vector3f(x + v.x, y + v.y, z + v.z);
	}

	//-------------------------------------------------------------------------

	const vector3f vector3f::operator - (const vector3f v) const
	{
		return vector3f(x - v.x, y - v.y, z - v.z);
	}

//	boolean operators

	const bool vector3f::operator==(const vector3f v) const
	{
		return (x == v.x && y == v.y && z == v.z);
	}

	//-------------------------------------------------------------------------

	const bool vector3f::operator!=(const vector3f v) const
	{
		return !(*this == v);
	}

//-------------------------------------------------------------------------
// tool functions
//-------------------------------------------------------------------------

	vector3f CrossProduct(const vector3f u, const vector3f v)
	{
		return vector3f((u.y * v.z) - (u.z * v.y),
						-((u.x * v.z) - (u.z * v.x)),
						(u.x * v.y) - (u.y * v.x));
	}

	//-------------------------------------------------------------------------

	float DotProduct(const vector3f v1, const vector3f v2)
	{
		return ((v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z));
	}

	//-------------------------------------------------------------------------

	float AngleBetween(const vector3f v1, const vector3f v2)
	{
		return (DotProduct(v1, v2)) / (v1.Length() * v2.Length());
	}
