#ifndef vector3f_____
#define vector3f_____

class vector3f
{
  public:

	//-------------------------------------------------------------------------
	// members
	//-------------------------------------------------------------------------

	float x, y, z;

	//-------------------------------------------------------------------------
	// constructors
	//-------------------------------------------------------------------------

	vector3f();
	vector3f(float x_, float y_, float z_);

	//-------------------------------------------------------------------------
	// member functions
	//-------------------------------------------------------------------------

	const float		Distance(const vector3f p) const;
	const vector3f	MidPoint(const vector3f p) const;
	const vector3f	Vector(const vector3f p) const;
	const float		Length() const;
	const float		LengthSquared() const;
	const vector3f	Normal() const;
	const vector3f	Project(vector3f v) const;
	void			Normalize();
	void			Set(const float x_, const float y_, const float z_);

	//-------------------------------------------------------------------------
	// overloaded operators
	//-------------------------------------------------------------------------

	// assignment operators
	void		   operator  = (const float s);
	const vector3f operator /= (const float s);
	const vector3f operator *= (const float s);
	const vector3f operator += (const float s);
	const vector3f operator -= (const float s);
	void		   operator  = (const vector3f v);
	const vector3f operator += (const vector3f v);
	const vector3f operator -= (const vector3f v);

	// unary and binary operators
	const vector3f operator / (const float s) const;
	const vector3f operator * (const float s) const;
	const vector3f operator + (const vector3f v) const;
	const vector3f operator - (const vector3f v) const;

	// boolean operators
	const bool operator == (const vector3f v) const;
	const bool operator != (const vector3f v) const;

};

//-------------------------------------------------------------------------
// tool functions
//-------------------------------------------------------------------------

vector3f CrossProduct(const vector3f v1, const vector3f v2);
float	 DotProduct(const vector3f v1, const vector3f v2);
float	 AngleBetween(const vector3f v1, const vector3f v2);

#endif