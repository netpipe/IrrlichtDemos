#ifndef vector2f_____
#define vector2f_____

class vector2f
{
  public:

	//-------------------------------------------------------------------------
	// members
	//-------------------------------------------------------------------------

	float x, y;

	//-------------------------------------------------------------------------
	// constructors
	//-------------------------------------------------------------------------

	vector2f();
	vector2f(float x_, float y_);

	//-------------------------------------------------------------------------
	// member functions
	//-------------------------------------------------------------------------

	const float		Distance(const vector2f p) const;
	const vector2f	MidPoint(const vector2f p) const;
	const vector2f	Vector(const vector2f p) const;
	const float		Length() const;
	const float		LengthSquared() const;
	const vector2f	Normal() const;
	const vector2f	Project(vector2f v) const;
	void			Normalize();
	void			Set(const float x_, const float y_);

	//-------------------------------------------------------------------------
	// overloaded operators
	//-------------------------------------------------------------------------

	// assignment operators
	void		   operator  = (const float s);
	const vector2f operator /= (const float s);
	const vector2f operator *= (const float s);
	const vector2f operator += (const float s);
	const vector2f operator -= (const float s);
	void		   operator  = (const vector2f v);
	const vector2f operator += (const vector2f v);
	const vector2f operator -= (const vector2f v);

	// unary and binary operators
	const vector2f operator / (const float s) const;
	const vector2f operator * (const float s) const;
	const vector2f operator + (const vector2f v) const;
	const vector2f operator - (const vector2f v) const;

	// boolean operators
	const bool operator == (const vector2f v) const;
	const bool operator != (const vector2f v) const;

};

//-------------------------------------------------------------------------
// tool functions
//-------------------------------------------------------------------------

float	 DotProduct(const vector2f v1, const vector2f v2);

#endif