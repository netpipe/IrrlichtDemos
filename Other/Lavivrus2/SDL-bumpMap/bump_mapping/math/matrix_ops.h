#ifndef __MATRIX_OPERATIONS_H__
#define __MATRIX_OPERATIONS_H__

#include "matrix.h"
//#include <math.h>
//#include <valarray>
#include <cmath>


// Matrix Operations
//
// Matrix Multpilication (Optimized for 2x2 * 2x2; 3x3 * 3x3; 4x4 * 4x4; 
//			2x2 * 2x1; 3x3 * 3x1; 4x4 * 4x1; 1x2 * 2x2; 1x3 * 3x3; 1x4 * 4x4)
// Matrix Addition
// Matrix Subtraction
// Matrix Invertation
// Matrix Transpose
// Matrix Determinant
// Vector Dot Product
// 3D Vector Cross Product

template<unsigned int m, unsigned int n, unsigned int o, class T>
inline matrix<m,o,T> mul(const matrix<m,n,T> & lhs, const matrix<n,o,T> & rhs)
{
	matrix<m,o,T> tmp;

	for (unsigned int i = 0; i < m; i++)
		for (unsigned int j = 0; j < o; j++)
		{
			tmp(i,j) = 0;
			for (unsigned int k = 0; k < n; k++)
				tmp(i,j) += lhs(i,k)*rhs(k,j);
		}

	return tmp;
}

template<unsigned int m, unsigned int n, class T>
inline matrix<m,n,T> add(const matrix<m,n,T> & lhs, const matrix<m,n,T> & rhs)
{
	matrix<m,n,T> tmp;

	for (unsigned int i = 0; i < n*m; i++)
		tmp[i] = lhs[i] + rhs[i];

	return tmp;
}

template<unsigned int m, unsigned int n, class T>
inline matrix<m,n,T> sub(const matrix<m,n,T> & lhs, const matrix<m,n,T> & rhs)
{
	matrix<m,n,T> tmp;

	for (unsigned int i = 0; i < n*m; i++)
		tmp[i] = lhs[i] - rhs[i];

	return tmp;
}

template<unsigned int m, class T>
inline T dot(const matrix<m,1,T> & lhs, const matrix<m,1,T> & rhs)
{
	T tmp = 0;

	for (unsigned int i = 0; i < m; i++)
		tmp += lhs[i] * rhs[i];

	return tmp;
}

template<unsigned int m, unsigned int n, class T, class U>
inline matrix<m,n,T> scale(const matrix<m,n,T> & lhs, const U rhs)
{
	matrix<m,n,T> tmp;
	for (unsigned int i = 0; i < m*n; i++)
		tmp[i] = lhs[i] * rhs;
	return tmp;
}

template<class T>
inline matrix<3,1,T> cross(const matrix<3,1,T> & lhs, const matrix<3,1,T> & rhs)
{
	matrix<3,1,T> tmp;
	
	tmp[0] = lhs[1]*rhs[2] - lhs[2]*rhs[1];
	tmp[1] = lhs[2]*rhs[0] - lhs[0]*rhs[2];
	tmp[2] = lhs[0]*rhs[1] - lhs[1]*rhs[0];

	return tmp;
}

template<class T>
inline matrix<1,3,T> cross(const matrix<1,3,T> & lhs, const matrix<1,3,T> & rhs)
{
	matrix<1,3,T> tmp;
	
	tmp[0] = lhs[1]*rhs[2] - lhs[2]*rhs[1];
	tmp[1] = lhs[2]*rhs[0] - lhs[0]*rhs[2];
	tmp[2] = lhs[0]*rhs[1] - lhs[1]*rhs[0];

	return tmp;
}

template<unsigned int m, unsigned int n, class T>
inline T lenght(const matrix<m, n, T> & rhs)
{ return static_cast<T> (std::sqrt(dot(rhs,rhs))); }

template<unsigned int m, unsigned int n, class T>
inline void normalize(matrix<m, n, T> & rhs)
{
	T tmp = lenght(rhs);
	tmp = 1 / tmp;
	for (unsigned int i = 0; i < m*n; i++)
		rhs[i] *= tmp;
}

template<unsigned int m, unsigned int n, class T>
inline matrix<m, n, T> get_normalized(matrix<m, n, T> & rhs)
{
	matrix<m, n, T> tmp(rhs);
	normalize(tmp);
	return tmp;
}

// optimized matrix operations for vec2, vec3, vec4, mat2, mat3 and mat4
inline matrix<2,2,float> mul(const matrix<2,2,float> & lhs, const matrix<2,2,float> & rhs)
{
	matrix<2,2,float> tmp;

	tmp[0] = lhs[0]*rhs[0] + lhs[1]*rhs[2];
	tmp[1] = lhs[0]*rhs[1] + lhs[1]*rhs[3];

	tmp[2] = lhs[2]*rhs[0] + lhs[3]*rhs[2];
	tmp[3] = lhs[2]*rhs[1] + lhs[3]*rhs[3];

	return tmp;
}

inline matrix<3,3,float> mul(const matrix<3,3,float> & lhs, const matrix<3,3,float> & rhs)
{
	matrix<3,3,float> tmp;

	tmp[0] = lhs[0]*rhs[0] + lhs[1]*rhs[3] + lhs[2]*rhs[6];
	tmp[1] = lhs[0]*rhs[1] + lhs[1]*rhs[4] + lhs[2]*rhs[7];
	tmp[2] = lhs[0]*rhs[2] + lhs[1]*rhs[5] + lhs[2]*rhs[8];

	tmp[3] = lhs[3]*rhs[0] + lhs[4]*rhs[3] + lhs[5]*rhs[6];
	tmp[4] = lhs[3]*rhs[1] + lhs[4]*rhs[4] + lhs[5]*rhs[7];
	tmp[5] = lhs[3]*rhs[2] + lhs[4]*rhs[5] + lhs[5]*rhs[8];

	tmp[6] = lhs[6]*rhs[0] + lhs[7]*rhs[3] + lhs[8]*rhs[6];
	tmp[7] = lhs[6]*rhs[1] + lhs[7]*rhs[4] + lhs[8]*rhs[7];
	tmp[8] = lhs[6]*rhs[2] + lhs[7]*rhs[5] + lhs[8]*rhs[8];

	return tmp;
}

inline matrix<4,4,float> mul(const matrix<4,4,float> & lhs, const matrix<4,4,float> & rhs)
{
	matrix<4,4,float> tmp;

	tmp[0] = lhs[0]*rhs[0] + lhs[1]*rhs[4] + lhs[2]*rhs[8] + lhs[3]*rhs[12];
	tmp[1] = lhs[0]*rhs[1] + lhs[1]*rhs[5] + lhs[2]*rhs[9] + lhs[3]*rhs[13];
	tmp[2] = lhs[0]*rhs[2] + lhs[1]*rhs[6] + lhs[2]*rhs[10] + lhs[3]*rhs[14];
	tmp[3] = lhs[0]*rhs[3] + lhs[1]*rhs[7] + lhs[2]*rhs[11] + lhs[3]*rhs[15];

	tmp[4] = lhs[4]*rhs[0] + lhs[5]*rhs[4] + lhs[6]*rhs[8] + lhs[7]*rhs[12];
	tmp[5] = lhs[4]*rhs[1] + lhs[5]*rhs[5] + lhs[6]*rhs[9] + lhs[7]*rhs[13];
	tmp[6] = lhs[4]*rhs[2] + lhs[5]*rhs[6] + lhs[6]*rhs[10] + lhs[7]*rhs[14];
	tmp[7] = lhs[4]*rhs[3] + lhs[5]*rhs[7] + lhs[6]*rhs[11] + lhs[7]*rhs[15];

	tmp[8] = lhs[8]*rhs[0] + lhs[9]*rhs[4] + lhs[10]*rhs[8] + lhs[11]*rhs[12];
	tmp[9] = lhs[8]*rhs[1] + lhs[9]*rhs[5] + lhs[10]*rhs[9] + lhs[11]*rhs[13];
	tmp[10] = lhs[8]*rhs[2] + lhs[9]*rhs[6] + lhs[10]*rhs[10] + lhs[11]*rhs[14];
	tmp[11] = lhs[8]*rhs[3] + lhs[9]*rhs[7] + lhs[10]*rhs[11] + lhs[11]*rhs[15];

	tmp[12] = lhs[12]*rhs[0] + lhs[13]*rhs[4] + lhs[14]*rhs[8] + lhs[15]*rhs[12];
	tmp[13] = lhs[12]*rhs[1] + lhs[13]*rhs[5] + lhs[14]*rhs[9] + lhs[15]*rhs[13];
	tmp[14] = lhs[12]*rhs[2] + lhs[13]*rhs[6] + lhs[14]*rhs[10] + lhs[15]*rhs[14];
	tmp[15] = lhs[12]*rhs[3] + lhs[13]*rhs[7] + lhs[14]*rhs[11] + lhs[15]*rhs[15];

	return tmp;
}

inline matrix<2,1,float> mul(const matrix<2,2,float> & lhs, const matrix<2,1,float> & rhs)
{
	matrix<2,1,float> tmp;

	tmp[0] = lhs[0]*rhs[0] + lhs[1]*rhs[1];
	tmp[1] = lhs[2]*rhs[0] + lhs[3]*rhs[1];

	return tmp;
}

inline matrix<3,1,float> mul(const matrix<3,3,float> & lhs, const matrix<3,1,float> & rhs)
{
	matrix<3,1,float> tmp;

	tmp[0] = lhs[0]*rhs[0] + lhs[1]*rhs[1] + lhs[2]*rhs[2];
	tmp[1] = lhs[3]*rhs[0] + lhs[4]*rhs[1] + lhs[5]*rhs[2];
	tmp[2] = lhs[6]*rhs[0] + lhs[7]*rhs[1] + lhs[8]*rhs[2];

	return tmp;
}

inline matrix<4,1,float> mul(const matrix<4,4,float> & lhs, const matrix<4,1,float> & rhs)
{
	matrix<4,1,float> tmp;

	tmp[0] = lhs[0]*rhs[0] + lhs[1]*rhs[1] + lhs[2]*rhs[2] + lhs[3]*rhs[3];
	tmp[1] = lhs[4]*rhs[0] + lhs[5]*rhs[1] + lhs[6]*rhs[2] + lhs[7]*rhs[3];
	tmp[2] = lhs[8]*rhs[0] + lhs[9]*rhs[1] + lhs[10]*rhs[2] + lhs[11]*rhs[3];
	tmp[3] = lhs[12]*rhs[0] + lhs[13]*rhs[1] + lhs[14]*rhs[2] + lhs[15]*rhs[3];

	return tmp;
}

inline matrix<2,1,float> mul(const matrix<2,1,float> & lhs, const matrix<2,2,float> & rhs)
{
	matrix<2,1,float> tmp;

	tmp[0] = rhs[0]*lhs[0] + rhs[2]*lhs[1];
	tmp[1] = rhs[1]*lhs[0] + rhs[3]*lhs[1];

	return tmp;
}

inline matrix<3,1,float> mul(const matrix<3,1,float> & lhs, const matrix<3,3,float> & rhs)
{
	matrix<3,1,float> tmp;

	tmp[0] = rhs[0]*lhs[0] + rhs[3]*lhs[1] + rhs[6]*lhs[2];
	tmp[1] = rhs[1]*lhs[0] + rhs[4]*lhs[1] + rhs[7]*lhs[2];
	tmp[2] = rhs[2]*lhs[0] + rhs[5]*lhs[1] + rhs[8]*lhs[2];

	return tmp;
}

inline matrix<4,1,float> mul(const matrix<4,1,float> & lhs, const matrix<4,4,float> & rhs)
{
	matrix<4,1,float> tmp;

	tmp[0] = rhs[0]*lhs[0] + rhs[4]*lhs[1] + rhs[8]*lhs[2] + rhs[12]*lhs[3];
	tmp[1] = rhs[1]*lhs[0] + rhs[5]*lhs[1] + rhs[9]*lhs[2] + rhs[13]*lhs[3];
	tmp[2] = rhs[2]*lhs[0] + rhs[6]*lhs[1] + rhs[10]*lhs[2] + rhs[14]*lhs[3];
	tmp[3] = rhs[3]*lhs[0] + rhs[7]*lhs[1] + rhs[11]*lhs[2] + rhs[15]*lhs[3];

	return tmp;
}

inline float dot(const matrix<2,1,float> & lhs, const matrix<2,1,float> & rhs)
{ return lhs[0]*rhs[0] + lhs[1]*rhs[1]; }

inline float dot(const matrix<3,1,float> & lhs, const matrix<3,1,float> & rhs)
{ return lhs[0]*rhs[0] + lhs[1]*rhs[1] + lhs[2]*rhs[2]; }

inline float dot(const matrix<4,1,float> & lhs, const matrix<4,1,float> & rhs)
{ return lhs[0]*rhs[0] + lhs[1]*rhs[1] + lhs[2]*rhs[2] + lhs[3]*rhs[3]; }


template<unsigned int m, unsigned int n, class T>
inline matrix<m,n,T> operator * (T rhs, const matrix<m,n,T> & lhs)
{
	matrix<m,n,T> tmp;
	for (unsigned int i = 0; i < m*n; i++)
		tmp[i] = lhs[i] * rhs;
	return tmp;
}

template<unsigned int m, unsigned int n, class T>
inline matrix<m,n,T> operator * (const matrix<m,n,T> & lhs, T rhs)
{
	matrix<m,n,T> tmp;
	for (unsigned int i = 0; i < m*n; i++)
		tmp[i] = lhs[i] * rhs;
	return tmp;
}

template<unsigned int m, unsigned int n, unsigned int o, class T>
inline matrix<m,o,T> operator * (const matrix<m,n,T> & lhs, const matrix<n,o,T> & rhs)
{ return mul(lhs, rhs); }

template<unsigned int m, class T>
inline T operator * (const matrix<m,1,T> & lhs, const matrix<m,1,T> & rhs)
{ return dot(lhs, rhs); }

template<unsigned int m, class T>
inline T operator * (const matrix<1,m,T> & lhs, const matrix<1,m,T> & rhs)
{ return dot(lhs, rhs); }

template<unsigned int m, unsigned int n, class T>
inline matrix<m,n,T> operator + (const matrix<m,n,T> & lhs, const matrix<m,n,T> & rhs)
{ return add(lhs,rhs); }

template<unsigned int m, unsigned int n, class T>
inline matrix<m,n,T> operator - (const matrix<m,n,T> & lhs, const matrix<m,n,T> & rhs)
{ return sub(lhs,rhs); }

template<class T>
inline matrix<3,1,T> operator % (const matrix<3,1,T> & lhs, const matrix<3,1,T> & rhs)
{ return cross(lhs, rhs); }

template<class T>
inline matrix<1,3,T> operator % (const matrix<1,3,T> & lhs, const matrix<1,3,T> & rhs)
{ return cross(lhs, rhs); }




template<unsigned int m, class T>
inline void set_identity(matrix<m,m,T> & rhs)
{
	for (unsigned int i = 0; i < m; i++)
		for (unsigned int j = 0; j < m; j++)
			rhs(i,j) = static_cast<T> (i == j ? 1 : 0);
}

template<unsigned int m, unsigned int n, class T>
inline void set_zero(matrix<m,n,T> & rhs)
{
	for (unsigned int i = 0; i < m; i++)
		for (unsigned int j = 0; j < n; j++)
			rhs(i,j) = static_cast<T> (0);
}


template<unsigned int m, unsigned int n, class T>
inline matrix<n,m,T> get_transpose(const matrix<m,n,T> & rhs)
{
	matrix<n,m,T> tmp;

	for (unsigned int i = 0; i < n; i++)
		for (unsigned int j = 0; j < m; j++)
			tmp[i*m+j] = rhs[j*n+i];

	return tmp;		
}

template<unsigned int m, class T>
inline void transpose(matrix<m,m,T> & rhs)
{ rhs = get_transpose(rhs); }


inline float determinant(const matrix<2,2,float> & rhs)
{ return rhs[0]*rhs[3] - rhs[1]*rhs[2]; }

inline float determinant(const matrix<3,3,float> & rhs)
{ return rhs[0]*rhs[4]*rhs[8] + rhs[1]*rhs[5]*rhs[6] + rhs[2]*rhs[3]*rhs[7]
		- rhs[6]*rhs[4]*rhs[2] - rhs[7]*rhs[5]*rhs[0] - rhs[8]*rhs[3]*rhs[1]; }

template<unsigned int m, class T>
inline T determinant(const matrix<m,m,T> & rhs)
{
	T tmp = 0;

	matrix<m,m-1,T> mattmp0 = rhs.del_column(0);
	for (unsigned int i = 0; i < m; i++)
	{
		matrix<m-1,m-1,T> mattmp1 = mattmp0.del_row(i);
		T det = determinant(mattmp1);
		T part = rhs(i,0);
		tmp += det * part * (((2+i) % 2 == 0) ? 1 : -1);
	}

	return tmp;
}

template<unsigned int m, class T>
inline matrix<m,m,T> get_invert(const matrix<m,m,T> & rhs)
{
	matrix<m,m,T> tmp;
	T det = determinant(rhs);

	if (det == 0)
	{
		#ifdef _DEBUG
			throw 0;
		#endif
		return tmp;
	}

	det = 1 / det;

    for (unsigned int i = 0; i < m; i++)
	{
		matrix<m,m-1,T> mattmp0 = rhs.del_column(i);
		for (unsigned int j = 0; j < m; j++)
		{
			matrix<m-1,m-1,T> mattmp1 = mattmp0.del_row(j);
            
			tmp(i,j) = determinant(mattmp1) * det *
				(((2+i+j) % 2 == 0) ? 1 : -1);
		}
	}

	return tmp;
}

template<unsigned int m, class T>
inline void invert(matrix<m,m,T> & rhs)
{ rhs = get_invert(rhs); }

template<unsigned int m, unsigned int n, class T>
inline void clamp(matrix<m,n,T> & rhs, T min, T max)
{
	for (unsigned int i = 0; i < m*n; i++)
	{
		rhs[i] = rhs[i] < min ? min : rhs[i];
		rhs[i] = rhs[i] > max ? max : rhs[i];
	}
}

template<unsigned int m, unsigned int n>
inline void clamp_to_01(matrix<m,n,float> & rhs)
{ clamp(rhs, 0.0f, 1.0f); }

template<unsigned int m, unsigned int n>
inline void scale_to_01(matrix<m,n,float> & rhs)
{
	normalize(rhs);
	rhs = rhs * 0.5f + matrix<m,n,float>(0.5f);
}

inline matrix<2,1> get_vec2(float x, float y)
{
	matrix<2,1> tmp;
	tmp[0] = x;
	tmp[1] = y;
	return tmp;
}

inline matrix<2,1> get_vec2(float const * const x)
{
	matrix<2,1> tmp;
	tmp[0] = x[0];
	tmp[1] = x[1];
	return tmp;
}

inline matrix<3,1> get_vec3(float x, float y, float z)
{
	matrix<3,1> tmp;
	tmp[0] = x;
	tmp[1] = y;
	tmp[2] = z;
	return tmp;
}

inline matrix<3,1> get_vec3(float const * const x)
{
	matrix<3,1> tmp;
	tmp[0] = x[0];
	tmp[1] = x[1];
	tmp[2] = x[2];
	return tmp;
}

inline matrix<4,1> get_vec4(float x, float y, float z, float w)
{
	matrix<4,1> tmp;
	tmp[0] = x;
	tmp[1] = y;
	tmp[2] = z;
	tmp[3] = w;
	return tmp;
}

inline matrix<3,1> get_vec4(float const * const x)
{
	matrix<3,1> tmp;
	tmp[0] = x[0];
	tmp[1] = x[1];
	tmp[2] = x[2];
	tmp[3] = x[3];
	return tmp;
}

inline matrix<4,4> get_rot_x_mat4(float angle)
{
	matrix<4,4> tmp;
	set_identity(tmp);

	float rad = angle * (float)DEG_to_RAD;

	tmp[5] = static_cast<float> (std::cos(rad));
	tmp[6] = - static_cast<float> (std::sin(rad));
	tmp[9] = static_cast<float> (std::sin(rad));
	tmp[10] = static_cast<float> (std::cos(rad));

	return tmp;
}

inline matrix<4,4> get_rot_y_mat4(float angle)
{
	matrix<4,4> tmp;
	set_identity(tmp);

	float rad = angle * (float)DEG_to_RAD;

	tmp[0] = static_cast<float> (std::cos(rad));
	tmp[2] = static_cast<float> (std::sin(rad));
	tmp[8] = - static_cast<float> (std::sin(rad));
	tmp[10] = static_cast<float> (std::cos(rad));

	return tmp;
}

inline matrix<4,4> get_rot_z_mat4(float angle)
{
	matrix<4,4> tmp;
	set_identity(tmp);

	float rad = angle * (float)DEG_to_RAD;

	tmp[0] = static_cast<float> (std::cos(rad));
	tmp[1] = - static_cast<float> (std::sin(rad));
	tmp[4] = static_cast<float> (std::sin(rad));
	tmp[5] = static_cast<float> (std::cos(rad));

	return tmp;
}

inline matrix<3,3> get_rot_x_mat3(float angle)
{
	matrix<3,3> tmp;
	set_identity(tmp);

	float rad = angle * (float)DEG_to_RAD;

	tmp[4] = static_cast<float> (std::cos(rad));
	tmp[5] = - static_cast<float> (std::sin(rad));
	tmp[7] = static_cast<float> (std::sin(rad));
	tmp[8] = static_cast<float> (std::cos(rad));

	return tmp;
}

inline matrix<3,3> get_rot_y_mat3(float angle)
{
	matrix<3,3> tmp;
	set_identity(tmp);

	float rad = angle * (float)DEG_to_RAD;

	tmp[0] = static_cast<float> (std::cos(rad));
	tmp[2] = static_cast<float> (std::sin(rad));
	tmp[6] = - static_cast<float> (std::sin(rad));
	tmp[8] = static_cast<float> (std::cos(rad));

	return tmp;
}

inline matrix<3,3> get_rot_z_mat3(float angle)
{
	matrix<3,3> tmp;
	set_identity(tmp);

	float rad = angle * (float)DEG_to_RAD;

	tmp[0] = static_cast<float> (std::cos(rad));
	tmp[1] = - static_cast<float> (std::sin(rad));
	tmp[3] = static_cast<float> (std::sin(rad));
	tmp[4] = static_cast<float> (std::cos(rad));

	return tmp;
}

inline matrix<4,4> get_scale_mat4(float x, float y, float z)
{
	matrix<4,4> tmp;
	set_identity(tmp);

	tmp[0] = x;
	tmp[5] = y;
	tmp[10] = z;

	return tmp;
}

inline matrix<4,4> get_translation_mat4(float x, float y, float z)
{
	matrix<4,4> tmp;
	set_identity(tmp);

	tmp[3] = x;
	tmp[7] = y;
	tmp[11] = z;

	return tmp;
}

inline matrix<4,4> get_look_at_mat4(vec3 eye,
									vec3 center,
									vec3 up)
{
	vec3 f = center - eye;
	normalize(f);
	//normalize(up);
	vec3 s = cross(f, up);
	//normalize(side);
	vec3 u = cross(s, f);

    matrix<4,4> tmp;

	tmp[0] = s[0];
	tmp[4] = s[1];
	tmp[8] = s[2];
	tmp[12] = 0.0f;

	tmp[1] = u[0];
	tmp[5] = u[1];
	tmp[9] = u[2];
	tmp[13] = 0.0f;

	tmp[2] = -f[0];
	tmp[6] = -f[1];
	tmp[10] = -f[2];
	tmp[14] = 0.0f;

	tmp[3] = 0.0f;
	tmp[7] = 0.0f;
	tmp[11] = 0.0f;
	tmp[15] = 1.0f;

	return get_invert(tmp) * get_translation_mat4(-eye[0], -eye[1], -eye[2]);
}

inline matrix<4,4> get_look_at_mat4(float eyeX, float eyeY, float eyeZ,
									float centerX, float centerY, float centerZ,
									float upX, float upY, float upZ)
{
	vec3 eye = get_vec3(eyeX, eyeY, eyeZ);
	vec3 center = get_vec3(centerX, centerY, centerZ);
	vec3 up = get_vec3(upX, upY, upZ);

	vec3 f = center - eye;
	normalize(f);
	//normalize(up);
	vec3 s = cross(f, up);
	//normalize(side);
	vec3 u = cross(s, f);

    matrix<4,4> tmp;

	tmp[0] = s[0];
	tmp[4] = s[1];
	tmp[8] = s[2];
	tmp[12] = 0.0f;

	tmp[1] = u[0];
	tmp[5] = u[1];
	tmp[9] = u[2];
	tmp[13] = 0.0f;

	tmp[2] = -f[0];
	tmp[6] = -f[1];
	tmp[10] = -f[2];
	tmp[14] = 0.0f;

	tmp[3] = 0.0f;
	tmp[7] = 0.0f;
	tmp[11] = 0.0f;
	tmp[15] = 1.0f;

	return get_invert(tmp) * get_translation_mat4(-eyeX, -eyeY, -eyeZ);
}

inline matrix<4,4> get_perspective_mat4(float fovy, float aspect,
										float zNear, float zFar)
{
	float f = 1.0f/tanf(fovy*(float)DEG_to_RAD/2.0f);

	matrix<4,4> tmp;

    set_zero(tmp);

	tmp[0] = f / aspect;
	tmp[5] = f;
	tmp[10] = (zFar + zNear)/(zNear-zFar);
	tmp[11] = (2*zFar*zNear)/(zNear-zFar);
	tmp[14] = -1.0f;

	return tmp;
}

inline matrix<3,3> to_mat3(const matrix<4,4> & rhs)
{
	matrix<3,3> tmp;
	tmp[0] = rhs[0];
	tmp[1] = rhs[1];
	tmp[2] = rhs[2];

	tmp[3] = rhs[4];
	tmp[4] = rhs[5];
	tmp[5] = rhs[6];

	tmp[6] = rhs[8];
	tmp[7] = rhs[9];
	tmp[8] = rhs[10];

	return tmp;
}

inline matrix<3,1> to_vec3(const matrix<4,1> & rhs)
{
	matrix<3,1> tmp;
	tmp[0] = rhs[0];
	tmp[1] = rhs[1];
	tmp[2] = rhs[2];

	return tmp;
}

inline matrix<2,1> to_vec2(const matrix<4,1> & rhs)
{
	matrix<2,1> tmp;
	tmp[0] = rhs[0];
	tmp[1] = rhs[1];

	return tmp;
}

inline matrix<2,1> to_vec2(const matrix<3,1> & rhs)
{
	matrix<2,1> tmp;
	tmp[0] = rhs[0];
	tmp[1] = rhs[1];

	return tmp;
}

inline matrix<2,2> get_identity2()
{
	matrix<2,2> tmp;
	set_identity(tmp);
	return tmp;
}

inline matrix<3,3> get_identity3()
{
	matrix<3,3> tmp;
	set_identity(tmp);
	return tmp;
}

inline matrix<4,4> get_identity4()
{
	matrix<4,4> tmp;
	set_identity(tmp);
	return tmp;
}

#endif