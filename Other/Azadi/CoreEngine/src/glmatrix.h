#ifndef GLMATRIX_H__
#define GLMATRIX_H__

#include "vector.h"
#include "quaternion.h"

// GL compatible matrix4x4 class --blub
// glmatrix<GLfloat> would be the GL version --blub

template<class T>
class glmatrix
{
private:
	//template<class T>
	class vec3
	{
	public:
		union
		{ // I hope nobody's gonna kill me for that... I'll only use the non-capitals though --blub
			struct { T X, Y, Z; };
			struct { T x, y, z; };
			T xyz[3]; // access the data as T[3]
		};
	};
public:
	glmatrix() { LoadIdentity(); }

	glmatrix(
		T _Xx, T _Yx, T _Zx, T _Ox,
		T _Xy, T _Yy, T _Zy, T _Oy,
		T _Xz, T _Yz, T _Zz, T _Oz,
		T _Xw, T _Yw, T _Zw, T _Ow)
	{
		X.x = _Xx; Y.x = _Yx; Z.x = _Zx; O.x = _Ox;
		X.y = _Xy; Y.y = _Yy; Z.y = _Zy; O.y = _Oy;
		X.z = _Xz; Y.z = _Yz; Z.z = _Zz; O.z = _Oz;
		Xw = _Xw; Yw = _Yw; Zw = _Zw; Ow = _Ow;
	}
	glmatrix(
		const vector3<T> &x,
		const vector3<T> &y,
		const vector3<T> &z,
		const vector3<T> &o)
	{
		/*Xx = x.X; Xy = x.Y; Xz = x.Z;
		Yx = y.X; Yy = y.Y; Yz = y.Z;
		Zx = z.X; Zy = z.Y; Zz = z.Z;
		Ox = o.X; Oy = o.Y; Oz = o.Z;
		Xw = Yw = Zw = 0;
		Ow = 1;*/
		X = x;
		Y = y;
		Z = z;
		Xw = Yw = Zw = 0;
		Ow = 1;
	}
	glmatrix(const glmatrix<T> &m)
	{ // copy constructor used in Transpose --blub
		memcpy(matrix, m.matrix, sizeof(matrix));
	}
	glmatrix(const quaternion<T> &q, const vector3<T> &pos)
	{ // to be done? --blub
#if 0
		// the following is for Q with angle a around U
		// in other words: around 1 axis, thus useless --blub
		// useful code is in the # else
		// let s = sin(a/2), c = cos(a/2)
		// => Qau := ( c, sUx, sUy, sUz )
		// a = 2 * acos(c)
		// s = sin(a/2) = sin ( 2 * acos(c) )
		// U = Qau.vector / s;
		double angle = 2.0 * acos(static_cast<double>(q.W)); // double precision --blub
		double s_rec = 1.0 / sin(angle/2.0);
		(*this) = Rotation(angle, s_rec * q.X, s_rec * q.Y, s_rec * q.Z);
		SetPos(pos);

		// if this doesn't work out, then this might do the job:
#else
		X.x = q.W*q.W + q.X*q.X + q.Y*q.Y - q.Z*q.Z;
		X.y = 2*q.X*q.Y + 2*q.Z*q.W;
		X.z = 2*q.X*q.Z - 2*q.Y*q.W;

		Y.x = 2*q.X*q.Y - 2*q.Z*q.W;
		Y.y = q.W*q.W - q.X*q.X + q.Y*q.Y - q.Z*q.Z;
		Y.z = 2*q.Y*q.Z + 2*q.X*q.W;

		Z.x = 2*q.X*q.Z + 2*q.Y*q.W;
		Z.y = 2*q.Y*q.Z - 2*q.X*q.W;
		Z.z = q.W*q.W - q.X*q.X - q.Y*q.Y + q.Z*q.Z;

		SetPos(pos);
#endif
	}
	/*Matrix = [ w2+x2-y2-z2     2xy - 2wz     2xz + 2wy
            2xy + 2wz    w2-x2+y2-z2     2yz - 2wx
            2xz - 2wy     2yz + 2wx    w2-x2-y2+z2 ]*/

	bool InverseOf(const glmatrix<T> &m)
	{ // returns false if the determinant == 0
		// definitely needs checking! --blub
		double d12, d13, d23, d24, d34, d41;

		d12 = m.matrix[2]  * m.matrix[7]  - m.matrix[3]  * m.matrix[6];
		d13 = m.matrix[2]  * m.matrix[11] - m.matrix[3]  * m.matrix[10];
		d23 = m.matrix[6]  * m.matrix[11] - m.matrix[7]  * m.matrix[10];
		d24 = m.matrix[6]  * m.matrix[15] - m.matrix[7]  * m.matrix[14];
		d34 = m.matrix[10] * m.matrix[15] - m.matrix[11] * m.matrix[14];
		d41 = m.matrix[14] * m.matrix[3]  - m.matrix[15] * m.matrix[2];

		matrix[0] =   m.matrix[5] * d34 - m.matrix[9] * d24 + m.matrix[13] * d23;
		matrix[1] = -(m.matrix[1] * d34 + m.matrix[9] * d41 + m.matrix[13] * d13);
		matrix[2] =   m.matrix[1] * d24 + m.matrix[5] * d41 + m.matrix[13] * d12;
		matrix[3] = -(m.matrix[1] * d23 - m.matrix[5] * d13 + m.matrix[9]  * d12);

		// Calculate the determinant.
		double det = m.matrix[0] * matrix[0] + m.matrix[4] * matrix[1]
			+ m.matrix[8] * matrix[2] + m.matrix[12] * matrix[3];
		if(det == 0.0)
		{ // determinant must not be null.
			LoadIdentity();
			return false;
		}

		det = 1.0 / det;
		matrix[0] *= det;
		matrix[1] *= det;
		matrix[2] *= det;
		matrix[3] *= det;
		matrix[4] = -(m.matrix[4] * d34 - m.matrix[8] * d24 + m.matrix[12] * d23) * det;
		matrix[5] =   m.matrix[0] * d34 + m.matrix[8] * d41 + m.matrix[12] * d13  * det;
		matrix[6] = -(m.matrix[0] * d24 + m.matrix[4] * d41 + m.matrix[12] * d12) * det;
		matrix[7] =   m.matrix[0] * d23 - m.matrix[4] * d13 + m.matrix[8]  * d12  * det;

		d12 = m.matrix[0]  * m.matrix[5]  - m.matrix[1]  * m.matrix[12];
		d13 = m.matrix[0]  * m.matrix[9]  - m.matrix[1]  * m.matrix[8];
		d23 = m.matrix[4]  * m.matrix[9]  - m.matrix[5]  * m.matrix[8];
		d24 = m.matrix[4]  * m.matrix[13] - m.matrix[5]  * m.matrix[12];
		d34 = m.matrix[8]  * m.matrix[13] - m.matrix[9]  * m.matrix[12];
		d41 = m.matrix[12] * m.matrix[1]  - m.matrix[13] * m.matrix[0];

		matrix[8]  =   m.matrix[7] * d34 - m.matrix[11] * d24 + m.matrix[15] * d23 * det;
		matrix[9]  = -(m.matrix[3] * d34 + m.matrix[11] * d41 + m.matrix[15] * d13) * det;
		matrix[10] =   m.matrix[3] * d24 + m.matrix[7]  * d41 + m.matrix[15] * d12 * det;
		matrix[11] = -(m.matrix[3] * d23 - m.matrix[7]  * d13 + m.matrix[11] * d12) * det;
		matrix[12] = -(m.matrix[6] * d34 - m.matrix[10] * d24 + m.matrix[14] * d23) * det;
		matrix[13] =   m.matrix[2] * d34 + m.matrix[10] * d41 + m.matrix[14] * d13 * det;
		matrix[14] = -(m.matrix[2] * d24 + m.matrix[6]  * d41 + m.matrix[14] * d12) * det;
		matrix[15] =   m.matrix[2] * d23 - m.matrix[6]  * d13 + m.matrix[10] * d12 * det;

		return true;
	}

	inline glmatrix<T> &Zero()
	{ // I doubt that this will be used --blub
		memset(matrix, 0, sizeof(matrix));
		return (*this);
	}

	inline glmatrix<T> &LoadIdentity()
	{ // identity matrix
		memset(matrix, 0, sizeof(matrix));
		X.x = 1.0f;
		Y.y = 1.0f;
		Z.z = 1.0f;
		Ow = 1.0f;
		return (*this);
	}

	inline glmatrix<T> &Translate(const vector3<T> &delta)
	{
		O.x += delta.X;
		O.y += delta.Y;
		O.z += delta.Z;
		return (*this);
	}
	inline glmatrix<T> &Translate(T x, T y, T z)
	{
		O.x += x;
		O.y += y;
		O.z += z;
		return (*this);
	}
	inline glmatrix<T> &SetPos(const vector3<T> &pos)
	{
		O.x = pos.X;
		O.y = pos.Y;
		O.z = pos.Z;
		return (*this);
	}
	inline glmatrix<T> &SetPos(T x, T y, T z)
	{
		O.x = x;
		O.y = y;
		O.z = z;
		return (*this);
	}
	inline glmatrix<T> &InverseTranslate()
	{
		O.x = -O.x;
		O.y = -O.y;
		O.z = -O.z;
		return (*this);
	}
	inline vector3<T> GetPos() const
	{ // the axis' indices are aligned in another way, so we need this: --blub
		//return vector3<T>(Ox, Oy, Oz);
		return O;
	}

	inline glmatrix<T> &Rotate(double angle, T x, T y, T z)
	{
		#ifndef GPU_MATHS
		vector3<T> pos = GetPos();
		(*this) = (*this) * Rotation(angle, x, y, z);
		SetPos(pos);
		#else
		glPushMatrix();
		glLoadMatrix(matrix);
		glRotated(angle, x, y, z);
		glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
		glPopMatrix();
		#endif
		return (*this);
	}

	inline T& operator[](const unsigned int n) const
	{
		return const_cast<T&>(this->matrix[n]);
	}

	inline glmatrix<T>& operator=(const glmatrix<T> &m)
	{
		memcpy(matrix, m.matrix, sizeof(matrix));
		return (*this);
	}
	inline bool operator==(const glmatrix<T> &m)
	{ // memcmp == 0 --blub
		return (memcmp(matrix, m.matrix, sizeof(matrix)) == 0);
	}
	inline bool operator!=(const glmatrix<T> &m)
	{ // memcmp != 0 --blub
		return (memcmp(matrix, m.matrix, sizeof(matrix)) != 0);
	}

	inline glmatrix<T> operator+(const glmatrix<T> &m)
	{
		return glmatrix<T>(
			matrix[0] + m[0], matrix[1] + m[1], matrix[2] + m[2], matrix[3] + m[3],
			matrix[4] + m[4], matrix[5] + m[5], matrix[6] + m[6], matrix[7] + m[7],
			matrix[8] + m[8], matrix[9] + m[9], matrix[10] + m[10], matrix[11] + m[11],
			matrix[12] + m[12], matrix[13] + m[13], matrix[14] + m[14], matrix[15] + m[15]
			);
	}
	inline glmatrix<T> operator-(const glmatrix<T> &m)
	{
		return glmatrix<T>(
			matrix[0] - m[0], matrix[1] - m[1], matrix[2] - m[2], matrix[3] - m[3],
			matrix[4] - m[4], matrix[5] - m[5], matrix[6] - m[6], matrix[7] - m[7],
			matrix[8] - m[8], matrix[9] - m[9], matrix[10] - m[10], matrix[11] - m[11],
			matrix[12] - m[12], matrix[13] - m[13], matrix[14] - m[14], matrix[15] - m[15]
			);
	}
	glmatrix<T> operator*(const glmatrix<T> &m)
	{ // should we inline that huge things? --blub
		/*return glmatrix<T>(
			matrix[0]*m.matrix[0] + matrix[4]*m.matrix[1] + matrix[8]*m.matrix[2],
			matrix[1]*m.matrix[0] + matrix[5]*m.matrix[1] + matrix[9]*m.matrix[2],
			matrix[2]*m.matrix[0] + matrix[6]*m.matrix[1] + matrix[10]*m.matrix[2],
			0,
			matrix[0]*m.matrix[4] + matrix[4]*m.matrix[5] + matrix[8]*m.matrix[6],
			matrix[1]*m.matrix[4] + matrix[5]*m.matrix[5] + matrix[9]*m.matrix[6],
			matrix[2]*m.matrix[4] + matrix[6]*m.matrix[5] + matrix[10]*m.matrix[6],
			0,
			matrix[0]*m.matrix[8] + matrix[4]*m.matrix[9] + matrix[8]*m.matrix[10],
			matrix[1]*m.matrix[8] + matrix[5]*m.matrix[9] + matrix[9]*m.matrix[10],
			matrix[2]*m.matrix[8] + matrix[6]*m.matrix[9] + matrix[10]*m.matrix[10],
			0,
			matrix[0]*m.matrix[12] + matrix[4]*m.matrix[13] + matrix[8]*m.matrix[14] + matrix[12],
			matrix[1]*m.matrix[12] + matrix[5]*m.matrix[13] + matrix[9]*m.matrix[14] + matrix[13],
			matrix[2]*m.matrix[12] + matrix[6]*m.matrix[13] + matrix[10]*m.matrix[14] + matrix[14],
			1
			);*/
		return glmatrix<T>( // changed to mathematical repesentation --blub
			(X.x*m.X.x + Y.x*m.X.y + Z.x*m.X.z),
			(X.x*m.Y.x + Y.x*m.Y.y + Z.x*m.Y.z),
			(X.x*m.Z.x + Y.x*m.Z.y + Z.x*m.Z.z),
			0,
			(X.y*m.X.x + Y.y*m.X.y + Z.y*m.X.z),
			(X.y*m.Y.x + Y.y*m.Y.y + Z.y*m.Y.z),
			(X.y*m.Z.x + Y.y*m.Z.y + Z.y*m.Z.z),
			0,
			(X.z*m.X.x + Y.z*m.X.y + Z.z*m.X.z),
			(X.z*m.Y.x + Y.z*m.Y.y + Z.z*m.Y.z),
			(X.z*m.Z.x + Y.z*m.Z.y + Z.z*m.Z.z),
			1
			);
	}
	inline glmatrix<T> operator*(const float f)
	{ // multiply the location too? --blub
		return glmatrix<T>(
			matrix[0] * f, matrix[1] * f, matrix[2] * f, matrix[3], // * f,
			matrix[4] * f, matrix[5] * f, matrix[6] * f, matrix[7], // * f,
			matrix[8] * f, matrix[9] * f, matrix[10] * f, matrix[10], // * f,
			0, 0, 0, 1
			);
	}
	inline glmatrix<T> operator/(const float f)
	{ // multiply the location too? --blub
		return glmatrix<T>(
			matrix[0] / f, matrix[1] / f, matrix[2] / f, matrix[3], // * f,
			matrix[4] / f, matrix[5] / f, matrix[6] / f, matrix[7], // * f,
			matrix[8] / f, matrix[9] / f, matrix[10] / f, matrix[10], // * f,
			0, 0, 0, 1
			);
	}
	inline glmatrix<T> operator+=(const glmatrix<T> &m)
	{
		matrix[0] += m[0];
		return (*this);
	}

	inline vector3<T> operator*(const vector3<T> &v) const
	{
		return vector3<T>(
			v.X*X.x + v.Y*Y.x + v.Z*Z.x,
			v.X*X.y + v.Y*Y.y + v.Z*Z.y,
			v.X*X.z + v.Y*Y.z + v.Z*Z.z
			);
	}

	inline vector3<T> Transform(const vector3<T> &v) const
	{
		return vector3<T>(
			(v.X*X.x + v.Y*Y.x + v.Z*Z.x) + O.x,
			(v.X*X.y + v.Y*Y.y + v.Z*Z.y) + O.y,
			 (v.X*X.z + v.Y*Y.z + v.Z*Z.z)+ O.z
			);
	}

	static glmatrix<T> Rotation(double angle, T x, T y, T z) // rotation matrix around a vector
        {
                glmatrix<T> matrix;
                T sine = static_cast<T>(sin(angle*M_PI/180.0));
		double _cosine = cos(angle*M_PI/180.0);
                T cosine = static_cast<T>(_cosine);
                T oneSubCos = static_cast<T>(1.0 - _cosine);
                T r = 1.0f/sqrt(x*x + y*y + z*z);
                x *= r;
                y *= r;
                z *= r;

                matrix[0] = (x*x) * oneSubCos + cosine;
                matrix[4] = (x*y) * oneSubCos - (z * sine);
                matrix[8] = (x*z) * oneSubCos + (y * sine);

                matrix[1] = (y*x) * oneSubCos + (z * sine);
                matrix[5] = (y*y) * oneSubCos + cosine;
                matrix[9] = (y*z) * oneSubCos - (x * sine);

                matrix[2]  = (z*x) * oneSubCos - (y * sine);
                matrix[6]  = (z*y) * oneSubCos + (x * sine);
                matrix[10] = (z*z) * oneSubCos + cosine;

                return matrix;
        }

	static glmatrix<T> Transpose(const glmatrix<T> &m)
	{
		return glmatrix<T>(
			m[0], m[4], m[8], m[12],
			m[1], m[5], m[9], m[13],
			m[2], m[6], m[10], m[14],
			m[3], m[7], m[11], m[15]
			);
	}

public:
	// that way, we can access matrix[x] or
	// this->X.X, .Y, .Z values --blub
	union {
		T matrix[16];
		struct {
			vec3 X; T Xw;
			vec3 Y; T Yw;
			vec3 Z; T Zw;
			vec3 O; T Ow;
		};
	};
};

static glmatrix<GLfloat> test;

#endif
