#include <stdlib.h>
#include <time.h>
#include "Vector2f.h"
#include <cmath>

//-------------------------------------------------------------------------
// constructors
//-------------------------------------------------------------------------

	vector2f::vector2f()
	{
		x = 0.0f;
		y = 0.0f;
	}

	//---------------------------------------------------------------------

	vector2f::vector2f(float x_, float y_)
	{
		x = x_;
		y = y_;
	}

//-------------------------------------------------------------------------
// member functions
//-------------------------------------------------------------------------

	const float vector2f::Distance(const vector2f p) const
	{
		Vector(p);
		return p.Length();
	}

	//---------------------------------------------------------------------

	const vector2f vector2f::MidPoint(const vector2f p) const
	{
		return vector2f((x+p.x)*0.5f, (y+p.y)*0.5f);
	}

	//---------------------------------------------------------------------

	const vector2f vector2f::Vector(const vector2f p) const
	{
		return vector2f(p.x - x, p.y - y);
	}

	//---------------------------------------------------------------------

	const float vector2f::Length() const
	{
		return sqrtf((x * x) + (y * y));
	}

	//---------------------------------------------------------------------

	const float vector2f::LengthSquared() const
	{
		return (x * x) + (y * y);
	}

	//---------------------------------------------------------------------

	const vector2f vector2f::Normal() const
	{
		float length = Length();
		if (length == 0.0f) length = 1.0f;
		float avoid_div = 1.0f / length;
		return vector2f(avoid_div * x, avoid_div * y);
	}

	//---------------------------------------------------------------------

	const vector2f vector2f::Project(vector2f v) const
	{
		return v * (DotProduct((*this), v) / v.LengthSquared());
	}

	//---------------------------------------------------------------------

	void vector2f::Normalize()
	{
		float length = Length();
		if (length == 0.0f) length = 1.0f;
		float avoid_div = 1.0f / length;
		x *= avoid_div;
		y *= avoid_div;
	}

	//---------------------------------------------------------------------

	void vector2f::Set(const float x_, const float y_)
	{
		x = x_;
		y = y_;
	}

//-------------------------------------------------------------------------
// overloaded operators
//-------------------------------------------------------------------------

//	assignment operators

	void vector2f::operator = (const float s)
	{
		x = s;
		y = s;
	}

	//-------------------------------------------------------------------------

	const vector2f vector2f::operator /= (const float s)
	{
		float avoid_div = 1.0f / s;
		x *= avoid_div;
		y *= avoid_div;
		return (*this);
	}

	//-------------------------------------------------------------------------

	const vector2f vector2f::operator *= (const float s)
	{
		x *= s;
		y *= s;
		return (*this);
	}

	//-------------------------------------------------------------------------

	const vector2f vector2f::operator += (const float s)
	{
		x += s;
		y += s;
		return (*this);
	}

	//-------------------------------------------------------------------------

	const vector2f vector2f::operator -= (const float s)
	{
		x -= s;
		y -= s;
		return (*this);
	}

	//-------------------------------------------------------------------------

	void vector2f::operator = (const vector2f v)
	{
		x = v.x;
		y = v.y;
	}

	//-------------------------------------------------------------------------

	const vector2f vector2f::operator += (vector2f v)
	{
		x += v.x;
		y += v.y;
		return (*this);
	}

	//-------------------------------------------------------------------------

	const vector2f vector2f::operator -= (vector2f v)
	{
		x -= v.x;
		y -= v.y;
		return (*this);
	}

// unary and binary operators

	const vector2f vector2f::operator / (const float s) const
	{
		float avoid_div = 1.0f / s;
		return vector2f(x * avoid_div, y * avoid_div);
	}

	//-------------------------------------------------------------------------

	const vector2f vector2f::operator * (const float s) const
	{
		return vector2f(x * s, y * s);
	}

	//-------------------------------------------------------------------------

	const vector2f vector2f::operator + (const vector2f v) const
	{
		return vector2f(x + v.x, y + v.y);
	}

	//-------------------------------------------------------------------------

	const vector2f vector2f::operator - (const vector2f v) const
	{
		return vector2f(x - v.x, y - v.y);
	}

//	boolean operators

	const bool vector2f::operator==(const vector2f v) const
	{
		return (x == v.x && y == v.y);
	}

	//-------------------------------------------------------------------------

	const bool vector2f::operator!=(const vector2f v) const
	{
		return !(*this == v);
	}

//-------------------------------------------------------------------------
// tool functions
//-------------------------------------------------------------------------

	float DotProduct(const vector2f v1, const vector2f v2)
	{
		return ((v1.x * v2.x) + (v1.y * v2.y));
	}
