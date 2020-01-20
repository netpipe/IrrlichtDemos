
#include "Quaternion.h"


	Quaternion::Quaternion(void)
{
	n = 0;
	v.X = 0;
	v.Y =  0;
	v.Z = 0;





}

	Quaternion::Quaternion(float e0, float e1, float e2, float e3)
{
	n = e0;
	v.X = e1;
	v.Y = e2;
	v.Z = e3;
}

	float	Quaternion::Magnitude(void)
{
	return (float) sqrt(n*n + v.X*v.X + v.Y*v.Y + v.Z*v.Z);
}

	vector3df	Quaternion::Getvector3df(void)
{
	return vector3df(v.X, v.Y, v.Z);
}

	float	Quaternion::GetScalar(void)
{
	return n;
}

	Quaternion	Quaternion::operator+=(Quaternion q)
{
	n += q.n;
	v.X += q.v.X;
	v.Y += q.v.Y;
	v.Z += q.v.Z;
	return *this;
}

	Quaternion	Quaternion::operator-=(Quaternion q)
{
	n -= q.n;
	v.X -= q.v.X;
	v.Y -= q.v.Y;
	v.Z -= q.v.Z;
	return *this;
}

	Quaternion Quaternion::operator*=(float s)
{
	n *= s;
	v.X *= s;
	v.Y *= s;
	v.Z *= s;
	return *this;
}

	Quaternion Quaternion::operator/=(float s)
{
	n /= s;
	v.X /= s;
	v.Y /= s;
	v.Z /= s;
	return *this;
}
