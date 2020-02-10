// Copyright (C) 2002-2006 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#ifndef __IRR_MATRIX_H_INCLUDED__
#define __IRR_MATRIX_H_INCLUDED__

#include "irrTypes.h"
#include "vector3d.h"
#include "plane3d.h"
#include "aabbox3d.h"
#include "rect.h"
#include <cstring>

namespace irr
{
namespace core
{


	//! 4x4 matrix. Mostly used as transformation matrix for 3d calculations.
	/* Matrix4 is mainly used by the Irrlicht engine for doing transformations.
	The matrix is a D3D style matrix, row major with translations in the 4th row.
	*/
	class matrix4
	{
		public:

			//! Constructor
			matrix4();

			//! Simple operator for directly accessing every element of the matrix.
			f32& operator()(s32 row, s32 col) { return M[col * 4 + row]; }

			//! Simple operator for directly accessing every element of the matrix.
			const f32& operator()(s32 row, s32 col) const {  return M[col * 4 + row]; }

			//! Sets this matrix equal to the other matrix.
			inline matrix4& operator=(const matrix4 &other);

			//! Returns true if other matrix is equal to this matrix.
			bool operator==(const matrix4 &other) const;

			//! Returns true if other matrix is not equal to this matrix.
			bool operator!=(const matrix4 &other) const;

			//! Multiply by another matrix.
			matrix4& operator*=(const matrix4& other);

			//! Multiply by another matrix.
			matrix4 operator*(const matrix4& other) const;

			//! Set matrix to identity. 
			inline void makeIdentity();

			//! Returns true if the matrix is the identity matrix
			bool isIdentity() const;

			//! Set the translation of the current matrix. Will erase any previous values.
			void setTranslation( const vector3df& translation );

			//! Gets the current translation
			vector3df getTranslation() const;

			//! Set the inverse translation of the current matrix. Will erase any previous values.
			void setInverseTranslation( const vector3df& translation );	

			//! Make a rotation matrix from Euler angles. The 4th row and column are unmodified.
			inline void setRotationRadians( const vector3df& rotation );			

			//! Make a rotation matrix from Euler angles. The 4th row and column are unmodified.
			void setRotationDegrees( const vector3df& rotation );			

			//! Returns the rotation, as set by setRotation(). This code was orginally written by by Chev.
			core::vector3df getRotationDegrees() const;			

			//! Make an inverted rotation matrix from Euler angles. The 4th row and column are unmodified.
			inline void setInverseRotationRadians( const vector3df& rotation );	

			//! Make an inverted rotation matrix from Euler angles. The 4th row and column are unmodified.
			void setInverseRotationDegrees( const vector3df& rotation );	

			//! Set Scale
			void setScale( const vector3df& scale );
			
			//! Get Scale
			core::vector3df getScale() const;

			//! Translate a vector by the inverse of the translation part of this matrix.
			void inverseTranslateVect( vector3df& vect ) const;			

			//! Rotate a vector by the inverse of the rotation part of this matrix.
			void inverseRotateVect( vector3df& vect ) const;				

			//! Rotate a vector by the rotation part of this matrix.
			void rotateVect( vector3df& vect ) const;				

			//! Transforms the vector by this matrix
			void transformVect( vector3df& vect) const;

			//! Transforms input vector by this matrix and stores result in output vector 
			void transformVect( const vector3df& in, vector3df& out) const;

			//! An alternate transform vector method, writing into an array of 4 floats
			void transformVect(f32 *out,const core::vector3df &in) const;

			//! Translate a vector by the translation part of this matrix.
			void translateVect( vector3df& vect ) const;

			//! Transforms a plane by this matrix
			void transformPlane( core::plane3d<f32> &plane) const;

			//! Transforms a plane by this matrix
			void transformPlane( const core::plane3d<f32> &in, core::plane3d<f32> &out) const;

			//! Transforms a axis aligned bounding box
			/** The result box of this operation may not be very accurate. For
			accurate results, use transformBoxEx() */
			void transformBox(core::aabbox3d<f32>& box) const;

			//! Transforms a axis aligned bounding box more accurately than transformBox()
			/** The result box of this operation should by quite accurate, but this operation
			is slower than transformBox(). */
			void transformBoxEx(core::aabbox3d<f32>& box) const;

			//! Multiplies this matrix by a 1x4 matrix
			void multiplyWith1x4Matrix(f32* matrix) const;

			//! Calculates inverse of matrix. Slow.
			//! \return Returns false if there is no inverse matrix.
			bool makeInverse();

			//! returns the inversed matrix of this one
			//! \param out: where result matrix is written to.
			//! \return Returns false if there is no inverse matrix.
			bool getInverse(matrix4& out) const;

			//! Builds a right-handed perspective projection matrix based on a field of view
			void buildProjectionMatrixPerspectiveFovRH(f32 fieldOfViewRadians, f32 aspectRatio, f32 zNear, f32 zFar);

			//! Builds a left-handed perspective projection matrix based on a field of view
			void buildProjectionMatrixPerspectiveFovLH(f32 fieldOfViewRadians, f32 aspectRatio, f32 zNear, f32 zFar);

			//! Builds a right-handed perspective projection matrix.
			void buildProjectionMatrixPerspectiveRH(f32 widthOfViewVolume, f32 heightOfViewVolume, f32 zNear, f32 zFar);

			//! Builds a left-handed perspective projection matrix.
			void buildProjectionMatrixPerspectiveLH(f32 widthOfViewVolume, f32 heightOfViewVolume, f32 zNear, f32 zFar);

			//! Builds a left-handed orthogonal projection matrix.
			void buildProjectionMatrixOrthoLH(f32 widthOfViewVolume, f32 heightOfViewVolume, f32 zNear, f32 zFar);

			//! Builds a right-handed orthogonal projection matrix.
			void buildProjectionMatrixOrthoRH(f32 widthOfViewVolume, f32 heightOfViewVolume, f32 zNear, f32 zFar);

			//! Builds a left-handed look-at matrix.
			void buildCameraLookAtMatrixLH(const vector3df& position, const vector3df& target, const vector3df& upVector);

			//! Builds a right-handed look-at matrix.
			void buildCameraLookAtMatrixRH(const vector3df& position, const vector3df& target, const vector3df& upVector);

			//! Builds a matrix that flattens geometry into a plane.
			//! \param light: light source
			//! \param plane: plane into which the geometry if flattened into
			//! \param point: value between 0 and 1, describing the light source. 
			//! If this is 1, it is a point light, if it is 0, it is a directional light.
			void buildShadowMatrix(const core::vector3df& light, core::plane3df plane, f32 point=1.0f);

			//! Builds a matrix which transforms a normalized Device Corrdinate to Device Coordinates. 
			/** Used to scale <-1,-1><1,1> to viewport, for example from von <-1,-1> <1,1> to the viewport <0,0><0,640> */
			void buildNDCToDCMatrix( const core::rect<s32>& area, f32 zScale);
			
			//! creates a new matrix as interpolated matrix from to other ones.
			//! \param b: other matrix to interpolate with
			//! \param time: Must be a value between 0 and 1.
			matrix4 interpolate(const core::matrix4& b, f32 time) const;

			//! returns transposed matrix
			matrix4 getTransposed() const;

			//! Matrix data, stored in column-major order
			f32 M[16];
	};


	inline matrix4::matrix4()
	{
		makeIdentity();
	}


	//! multiply by another matrix
	inline matrix4& matrix4::operator*=(const matrix4& other)
	{
		f32 newMatrix[16];
		const f32 *m1 = M, *m2 = other.M;

		newMatrix[0] = m1[0]*m2[0] + m1[4]*m2[1] + m1[8]*m2[2] + m1[12]*m2[3];
		newMatrix[1] = m1[1]*m2[0] + m1[5]*m2[1] + m1[9]*m2[2] + m1[13]*m2[3];
		newMatrix[2] = m1[2]*m2[0] + m1[6]*m2[1] + m1[10]*m2[2] + m1[14]*m2[3];
		newMatrix[3] = m1[3]*m2[0] + m1[7]*m2[1] + m1[11]*m2[2] + m1[15]*m2[3];
		
		newMatrix[4] = m1[0]*m2[4] + m1[4]*m2[5] + m1[8]*m2[6] + m1[12]*m2[7];
		newMatrix[5] = m1[1]*m2[4] + m1[5]*m2[5] + m1[9]*m2[6] + m1[13]*m2[7];
		newMatrix[6] = m1[2]*m2[4] + m1[6]*m2[5] + m1[10]*m2[6] + m1[14]*m2[7];
		newMatrix[7] = m1[3]*m2[4] + m1[7]*m2[5] + m1[11]*m2[6] + m1[15]*m2[7];
		
		newMatrix[8] = m1[0]*m2[8] + m1[4]*m2[9] + m1[8]*m2[10] + m1[12]*m2[11];
		newMatrix[9] = m1[1]*m2[8] + m1[5]*m2[9] + m1[9]*m2[10] + m1[13]*m2[11];
		newMatrix[10] = m1[2]*m2[8] + m1[6]*m2[9] + m1[10]*m2[10] + m1[14]*m2[11];
		newMatrix[11] = m1[3]*m2[8] + m1[7]*m2[9] + m1[11]*m2[10] + m1[15]*m2[11];
		
		newMatrix[12] = m1[0]*m2[12] + m1[4]*m2[13] + m1[8]*m2[14] + m1[12]*m2[15];
		newMatrix[13] = m1[1]*m2[12] + m1[5]*m2[13] + m1[9]*m2[14] + m1[13]*m2[15];
		newMatrix[14] = m1[2]*m2[12] + m1[6]*m2[13] + m1[10]*m2[14] + m1[14]*m2[15];
		newMatrix[15] = m1[3]*m2[12] + m1[7]*m2[13] + m1[11]*m2[14] + m1[15]*m2[15];
		
		memcpy(M, newMatrix, 16*sizeof(f32));

		return *this;
	}



	//! multiply by another matrix
	inline matrix4 matrix4::operator*(const matrix4& other) const
	{
		matrix4 tmtrx;
		const f32 *m1 = M, *m2 = other.M;
		f32 *m3 = tmtrx.M;

		m3[0] = m1[0]*m2[0] + m1[4]*m2[1] + m1[8]*m2[2] + m1[12]*m2[3];
		m3[1] = m1[1]*m2[0] + m1[5]*m2[1] + m1[9]*m2[2] + m1[13]*m2[3];
		m3[2] = m1[2]*m2[0] + m1[6]*m2[1] + m1[10]*m2[2] + m1[14]*m2[3];
		m3[3] = m1[3]*m2[0] + m1[7]*m2[1] + m1[11]*m2[2] + m1[15]*m2[3];
		
		m3[4] = m1[0]*m2[4] + m1[4]*m2[5] + m1[8]*m2[6] + m1[12]*m2[7];
		m3[5] = m1[1]*m2[4] + m1[5]*m2[5] + m1[9]*m2[6] + m1[13]*m2[7];
		m3[6] = m1[2]*m2[4] + m1[6]*m2[5] + m1[10]*m2[6] + m1[14]*m2[7];
		m3[7] = m1[3]*m2[4] + m1[7]*m2[5] + m1[11]*m2[6] + m1[15]*m2[7];
		
		m3[8] = m1[0]*m2[8] + m1[4]*m2[9] + m1[8]*m2[10] + m1[12]*m2[11];
		m3[9] = m1[1]*m2[8] + m1[5]*m2[9] + m1[9]*m2[10] + m1[13]*m2[11];
		m3[10] = m1[2]*m2[8] + m1[6]*m2[9] + m1[10]*m2[10] + m1[14]*m2[11];
		m3[11] = m1[3]*m2[8] + m1[7]*m2[9] + m1[11]*m2[10] + m1[15]*m2[11];
		
		m3[12] = m1[0]*m2[12] + m1[4]*m2[13] + m1[8]*m2[14] + m1[12]*m2[15];
		m3[13] = m1[1]*m2[12] + m1[5]*m2[13] + m1[9]*m2[14] + m1[13]*m2[15];
		m3[14] = m1[2]*m2[12] + m1[6]*m2[13] + m1[10]*m2[14] + m1[14]*m2[15];
		m3[15] = m1[3]*m2[12] + m1[7]*m2[13] + m1[11]*m2[14] + m1[15]*m2[15];

		return tmtrx;
	}



	inline vector3df matrix4::getTranslation() const
	{
		return vector3df(M[12], M[13], M[14]);
	}


	inline void matrix4::setTranslation( const vector3df& translation )
	{
		M[12] = translation.X;
		M[13] = translation.Y;
		M[14] = translation.Z;
	}

	inline void matrix4::setInverseTranslation( const vector3df& translation )
	{
		M[12] = -translation.X;
		M[13] = -translation.Y;
		M[14] = -translation.Z;
	}

	inline void matrix4::setScale( const vector3df& scale )
	{
		M[0] = scale.X;
		M[5] = scale.Y;
		M[10] = scale.Z;
	}

	inline vector3df matrix4::getScale() const
	{
		return vector3df(M[0],M[5],M[10]);
	}

	inline void matrix4::setRotationDegrees( const vector3df& rotation )
	{
		setRotationRadians( rotation * core::DEGTORAD );
	}

	inline void matrix4::setInverseRotationDegrees( const vector3df& rotation )
	{
		setInverseRotationRadians( rotation * core::DEGTORAD );
	}

	inline void matrix4::setRotationRadians( const vector3df& rotation )
	{
		f64 cr = cos( rotation.X );
		f64 sr = sin( rotation.X );
		f64 cp = cos( rotation.Y );
		f64 sp = sin( rotation.Y );
		f64 cy = cos( rotation.Z );
		f64 sy = sin( rotation.Z );

		M[0] = (f32)( cp*cy );
		M[1] = (f32)( cp*sy );
		M[2] = (f32)( -sp );

		f64 srsp = sr*sp;
		f64 crsp = cr*sp;

		M[4] = (f32)( srsp*cy-cr*sy );
		M[5] = (f32)( srsp*sy+cr*cy );
		M[6] = (f32)( sr*cp );

		M[8] = (f32)( crsp*cy+sr*sy );
		M[9] = (f32)( crsp*sy-sr*cy );
		M[10] = (f32)( cr*cp );
	}

	//! Returns the rotation, as set by setRotation(). This code was sent
	//! in by Chev.
	inline core::vector3df matrix4::getRotationDegrees() const
	{
		const matrix4 &mat = *this; 

		f64 Y = -asin(mat(2,0)); 
		f64 C = cos(Y); 
		Y *= GRAD_PI; 

		f64 rotx, roty, X, Z; 

		if (fabs(C)>0.0005f)  
		{ 
			rotx = mat(2,2) / C; 
			roty = mat(2,1)  / C; 
			X = atan2( roty, rotx ) * GRAD_PI; 
			rotx = mat(0,0) / C; 
			roty = mat(1,0) / C; 
			Z = atan2( roty, rotx ) * GRAD_PI; 
		} 
		else 
		{ 
			X  = 0.0f; 
			rotx = mat(1,1); 
			roty = -mat(0,1); 
			Z  = atan2( roty, rotx ) * (f32)GRAD_PI; 
		} 

		// fix values that get below zero 
		// before it would set (!) values to 360 
		// that where above 360: 
		if (X < 0.0) X += 360.0; 
		if (Y < 0.0) Y += 360.0; 
		if (Z < 0.0) Z += 360.0; 

		return vector3df((f32)X,(f32)Y,(f32)Z);
	}

	inline void matrix4::setInverseRotationRadians( const vector3df& rotation )
	{
		f64 cr = cos( rotation.X );
		f64 sr = sin( rotation.X );
		f64 cp = cos( rotation.Y );
		f64 sp = sin( rotation.Y );
		f64 cy = cos( rotation.Z );
		f64 sy = sin( rotation.Z );

		M[0] = (f32)( cp*cy );
		M[4] = (f32)( cp*sy );
		M[8] = (f32)( -sp );

		f64 srsp = sr*sp;
		f64 crsp = cr*sp;

		M[1] = (f32)( srsp*cy-cr*sy );
		M[5] = (f32)( srsp*sy+cr*cy );
		M[9] = (f32)( sr*cp );

		M[2] = (f32)( crsp*cy+sr*sy );
		M[6] = (f32)( crsp*sy-sr*cy );
		M[10] = (f32)( cr*cp );
	}


	inline void matrix4::makeIdentity()
	{
		memset(M, 0, 16*sizeof(f32));
		M[0] = M[5] = M[10] = M[15] = 1.0f;
	}


	inline bool matrix4::isIdentity() const
	{
		for (s32 i=0; i<4; ++i)
			for (s32 j=0; j<4; ++j)
			if (j != i)
			{
				if ((*this)(i,j) < -0.0000001f ||
					(*this)(i,j) >  0.0000001f)
					return false;
			}
			else
			{
				if ((*this)(i,j) < 0.9999999f ||
					(*this)(i,j) > 1.0000001f)
					return false;
			}

		return true;
	}

	inline void matrix4::rotateVect( vector3df& vect ) const
	{
		vector3df tmp = vect;
		vect.X = tmp.X*M[0] + tmp.Y*M[4] + tmp.Z*M[8];
		vect.Y = tmp.X*M[1] + tmp.Y*M[5] + tmp.Z*M[9];
		vect.Z = tmp.X*M[2] + tmp.Y*M[6] + tmp.Z*M[10]; 
	}

	inline void matrix4::inverseRotateVect( vector3df& vect ) const
	{
		vector3df tmp = vect;
		vect.X = tmp.X*M[0] + tmp.Y*M[1] + tmp.Z*M[2];
		vect.Y = tmp.X*M[4] + tmp.Y*M[5] + tmp.Z*M[6];
		vect.Z = tmp.X*M[8] + tmp.Y*M[9] + tmp.Z*M[10];
	}

	inline void matrix4::transformVect( vector3df& vect) const
	{
		f32 vector[3];

		vector[0] = vect.X*M[0] + vect.Y*M[4] + vect.Z*M[8] + M[12];
		vector[1] = vect.X*M[1] + vect.Y*M[5] + vect.Z*M[9] + M[13];
		vector[2] = vect.X*M[2] + vect.Y*M[6] + vect.Z*M[10] + M[14];

		vect.X = vector[0];
		vect.Y = vector[1];
		vect.Z = vector[2];
	}

	inline void matrix4::transformVect( const vector3df& in, vector3df& out) const
	{
		out.X = in.X*M[0] + in.Y*M[4] + in.Z*M[8] + M[12];
		out.Y = in.X*M[1] + in.Y*M[5] + in.Z*M[9] + M[13];
		out.Z = in.X*M[2] + in.Y*M[6] + in.Z*M[10] + M[14];
	}


	//! Transforms a plane by this matrix
	inline void matrix4::transformPlane( core::plane3d<f32> &plane) const
	{
		vector3df member;
		transformVect(plane.getMemberPoint(), member);

		vector3df origin(0,0,0);
		transformVect(plane.Normal);
		transformVect(origin);

		plane.Normal -= origin;
		plane.D = - member.dotProduct(plane.Normal);
	}

	//! Transforms a plane by this matrix
	inline void matrix4::transformPlane( const core::plane3d<f32> &in, core::plane3d<f32> &out) const
	{
		out = in;
        transformPlane( out );
	}

	//! Transforms a axis aligned bounding box
	inline void matrix4::transformBox(core::aabbox3d<f32>& box) const
	{
		transformVect(box.MinEdge);
		transformVect(box.MaxEdge);
		box.repair();
	}

	//! Transforms a axis aligned bounding box more accurately than transformBox()
	inline void matrix4::transformBoxEx(core::aabbox3d<f32>& box) const
	{
		core::vector3df edges[8];
		box.getEdges(edges);

		int i;
		for (i=0; i<8; ++i)
			transformVect(edges[i]);

		box.reset(edges[0]);

		for (i=1; i<8; ++i)
			box.addInternalPoint(edges[i]);
	}

	//! Multiplies this matrix by a 1x4 matrix
	inline void matrix4::multiplyWith1x4Matrix(f32* matrix) const
	{
		/*
		0  1  2  3
		4  5  6  7
		8  9  10 11
		12 13 14 15
		*/

		f32 mat[4];
		mat[0] = matrix[0];
		mat[1] = matrix[1];
		mat[2] = matrix[2];
		mat[3] = matrix[3];

		matrix[0] = M[0]*mat[0] + M[4]*mat[1] + M[8]*mat[2] + M[12]*mat[3];
		matrix[1] = M[1]*mat[0] + M[5]*mat[1] + M[9]*mat[2] + M[13]*mat[3];
		matrix[2] = M[2]*mat[0] + M[6]*mat[1] + M[10]*mat[2] + M[14]*mat[3];
		matrix[3] = M[3]*mat[0] + M[7]*mat[1] + M[11]*mat[2] + M[15]*mat[3];
	}

	inline void matrix4::inverseTranslateVect( vector3df& vect ) const
	{
		vect.X = vect.X-M[12];
		vect.Y = vect.Y-M[13];
		vect.Z = vect.Z-M[14];
	}

	inline void matrix4::translateVect( vector3df& vect ) const
	{
		vect.X = vect.X+M[12];
		vect.Y = vect.Y+M[13];
		vect.Z = vect.Z+M[14];
	}


	inline bool matrix4::getInverse(matrix4& out) const
	{
		/// Calculates the inverse of this Matrix 
		/// The inverse is calculated using Cramers rule.
		/// If no inverse exists then 'false' is returned.

		const matrix4 &m = *this;

		f32 d = (m(0, 0) * m(1, 1) - m(1, 0) * m(0, 1)) * (m(2, 2) * m(3, 3) - m(3, 2) * m(2, 3))	- (m(0, 0) * m(2, 1) - m(2, 0) * m(0, 1)) * (m(1, 2) * m(3, 3) - m(3, 2) * m(1, 3))
				+ (m(0, 0) * m(3, 1) - m(3, 0) * m(0, 1)) * (m(1, 2) * m(2, 3) - m(2, 2) * m(1, 3))	+ (m(1, 0) * m(2, 1) - m(2, 0) * m(1, 1)) * (m(0, 2) * m(3, 3) - m(3, 2) * m(0, 3))
				- (m(1, 0) * m(3, 1) - m(3, 0) * m(1, 1)) * (m(0, 2) * m(2, 3) - m(2, 2) * m(0, 3))	+ (m(2, 0) * m(3, 1) - m(3, 0) * m(2, 1)) * (m(0, 2) * m(1, 3) - m(1, 2) * m(0, 3));
		
		if (d == 0.f)
			return false;

		d = 1.f / d;

		out(0, 0) = d * (m(1, 1) * (m(2, 2) * m(3, 3) - m(3, 2) * m(2, 3)) + m(2, 1) * (m(3, 2) * m(1, 3) - m(1, 2) * m(3, 3)) + m(3, 1) * (m(1, 2) * m(2, 3) - m(2, 2) * m(1, 3)));
		out(1, 0) = d * (m(1, 2) * (m(2, 0) * m(3, 3) - m(3, 0) * m(2, 3)) + m(2, 2) * (m(3, 0) * m(1, 3) - m(1, 0) * m(3, 3)) + m(3, 2) * (m(1, 0) * m(2, 3) - m(2, 0) * m(1, 3)));
		out(2, 0) = d * (m(1, 3) * (m(2, 0) * m(3, 1) - m(3, 0) * m(2, 1)) + m(2, 3) * (m(3, 0) * m(1, 1) - m(1, 0) * m(3, 1)) + m(3, 3) * (m(1, 0) * m(2, 1) - m(2, 0) * m(1, 1)));
		out(3, 0) = d * (m(1, 0) * (m(3, 1) * m(2, 2) - m(2, 1) * m(3, 2)) + m(2, 0) * (m(1, 1) * m(3, 2) - m(3, 1) * m(1, 2)) + m(3, 0) * (m(2, 1) * m(1, 2) - m(1, 1) * m(2, 2)));
		out(0, 1) = d * (m(2, 1) * (m(0, 2) * m(3, 3) - m(3, 2) * m(0, 3)) + m(3, 1) * (m(2, 2) * m(0, 3) - m(0, 2) * m(2, 3)) + m(0, 1) * (m(3, 2) * m(2, 3) - m(2, 2) * m(3, 3)));
		out(1, 1) = d * (m(2, 2) * (m(0, 0) * m(3, 3) - m(3, 0) * m(0, 3)) + m(3, 2) * (m(2, 0) * m(0, 3) - m(0, 0) * m(2, 3)) + m(0, 2) * (m(3, 0) * m(2, 3) - m(2, 0) * m(3, 3)));
		out(2, 1) = d * (m(2, 3) * (m(0, 0) * m(3, 1) - m(3, 0) * m(0, 1)) + m(3, 3) * (m(2, 0) * m(0, 1) - m(0, 0) * m(2, 1)) + m(0, 3) * (m(3, 0) * m(2, 1) - m(2, 0) * m(3, 1)));
		out(3, 1) = d * (m(2, 0) * (m(3, 1) * m(0, 2) - m(0, 1) * m(3, 2)) + m(3, 0) * (m(0, 1) * m(2, 2) - m(2, 1) * m(0, 2)) + m(0, 0) * (m(2, 1) * m(3, 2) - m(3, 1) * m(2, 2)));
		out(0, 2) = d * (m(3, 1) * (m(0, 2) * m(1, 3) - m(1, 2) * m(0, 3)) + m(0, 1) * (m(1, 2) * m(3, 3) - m(3, 2) * m(1, 3)) + m(1, 1) * (m(3, 2) * m(0, 3) - m(0, 2) * m(3, 3)));
		out(1, 2) = d * (m(3, 2) * (m(0, 0) * m(1, 3) - m(1, 0) * m(0, 3)) + m(0, 2) * (m(1, 0) * m(3, 3) - m(3, 0) * m(1, 3)) + m(1, 2) * (m(3, 0) * m(0, 3) - m(0, 0) * m(3, 3)));
		out(2, 2) = d * (m(3, 3) * (m(0, 0) * m(1, 1) - m(1, 0) * m(0, 1)) + m(0, 3) * (m(1, 0) * m(3, 1) - m(3, 0) * m(1, 1)) + m(1, 3) * (m(3, 0) * m(0, 1) - m(0, 0) * m(3, 1)));
		out(3, 2) = d * (m(3, 0) * (m(1, 1) * m(0, 2) - m(0, 1) * m(1, 2)) + m(0, 0) * (m(3, 1) * m(1, 2) - m(1, 1) * m(3, 2)) + m(1, 0) * (m(0, 1) * m(3, 2) - m(3, 1) * m(0, 2)));
		out(0, 3) = d * (m(0, 1) * (m(2, 2) * m(1, 3) - m(1, 2) * m(2, 3)) + m(1, 1) * (m(0, 2) * m(2, 3) - m(2, 2) * m(0, 3)) + m(2, 1) * (m(1, 2) * m(0, 3) - m(0, 2) * m(1, 3)));
		out(1, 3) = d * (m(0, 2) * (m(2, 0) * m(1, 3) - m(1, 0) * m(2, 3)) + m(1, 2) * (m(0, 0) * m(2, 3) - m(2, 0) * m(0, 3)) + m(2, 2) * (m(1, 0) * m(0, 3) - m(0, 0) * m(1, 3)));
		out(2, 3) = d * (m(0, 3) * (m(2, 0) * m(1, 1) - m(1, 0) * m(2, 1)) + m(1, 3) * (m(0, 0) * m(2, 1) - m(2, 0) * m(0, 1)) + m(2, 3) * (m(1, 0) * m(0, 1) - m(0, 0) * m(1, 1)));
		out(3, 3) = d * (m(0, 0) * (m(1, 1) * m(2, 2) - m(2, 1) * m(1, 2)) + m(1, 0) * (m(2, 1) * m(0, 2) - m(0, 1) * m(2, 2)) + m(2, 0) * (m(0, 1) * m(1, 2) - m(1, 1) * m(0, 2)));

		return true;
	}


	inline bool matrix4::makeInverse()
	{
		matrix4 temp;

		if (getInverse(temp))
		{
			*this = temp;
			return true;
		}

		return false;
	}



	inline matrix4& matrix4::operator=(const matrix4 &other)
	{
		memcpy(M, other.M, 16*sizeof(f32));

		return *this;
	}



	inline bool matrix4::operator==(const matrix4 &other) const
	{
		for (s32 i = 0; i < 16; ++i)
			if (M[i] != other.M[i])
				return false;

		return true;
	}



	inline bool matrix4::operator!=(const matrix4 &other) const
	{
		return !(*this == other);
	}



	//! Builds a right-handed perspective projection matrix based on a field of view
	inline void matrix4::buildProjectionMatrixPerspectiveFovRH(f32 fieldOfViewRadians, f32 aspectRatio, f32 zNear, f32 zFar)
	{
		f32 h = (f32)(cos(fieldOfViewRadians/2.0) / sin(fieldOfViewRadians/2.0));
		f32 w = h * aspectRatio;

		(*this)(0,0) = 2.0f*zNear/w;
		(*this)(1,0) = 0.0f;
		(*this)(2,0) = 0.0f;
		(*this)(3,0) = 0.0f;

		(*this)(0,1) = 0.0f;
		(*this)(1,1) = 2.0f*zNear/h;
		(*this)(2,1) = 0.0f;
		(*this)(3,1) = 0.0f;

		(*this)(0,2) = 0.0f;
		(*this)(1,2) = 0.0f;
		(*this)(2,2) = zFar/(zFar-zNear);
		(*this)(3,2) = -1.0f;

		(*this)(0,3) = 0.0f;
		(*this)(1,3) = 0.0f;
		(*this)(2,3) = zNear*zFar/(zNear-zFar);
		(*this)(3,3) = 0.0f;
	}



	//! Builds a left-handed perspective projection matrix based on a field of view
	inline void matrix4::buildProjectionMatrixPerspectiveFovLH(f32 fieldOfViewRadians, f32 aspectRatio, f32 zNear, f32 zFar)
	{
		f32 h = (f32)(cos(fieldOfViewRadians/2.0) / sin(fieldOfViewRadians/2.0));
		f32 w = h * aspectRatio;

		(*this)(0,0) = 2.0f*zNear/w;
		(*this)(1,0) = 0.0f;
		(*this)(2,0) = 0.0f;
		(*this)(3,0) = 0.0f;

		(*this)(0,1) = 0.0f;
		(*this)(1,1) = 2.0f*zNear/h;
		(*this)(2,1) = 0.0f;
		(*this)(3,1) = 0.0f;

		(*this)(0,2) = 0.0f;
		(*this)(1,2) = 0.0f;
		(*this)(2,2) = zFar/(zFar-zNear);
		(*this)(3,2) = 1.0f;

		(*this)(0,3) = 0.0f;
		(*this)(1,3) = 0.0f;
		(*this)(2,3) = zNear*zFar/(zNear-zFar);
		(*this)(3,3) = 0.0f;
	}



	//! Builds a left-handed orthogonal projection matrix.
	inline void matrix4::buildProjectionMatrixOrthoLH(f32 widthOfViewVolume, f32 heightOfViewVolume, f32 zNear, f32 zFar)
	{
		(*this)(0,0) = 2/widthOfViewVolume;
		(*this)(1,0) = 0;
		(*this)(2,0) = 0;
		(*this)(3,0) = 0;

		(*this)(0,1) = 0;
		(*this)(1,1) = 2/heightOfViewVolume;
		(*this)(2,1) = 0;
		(*this)(3,1) = 0;

		(*this)(0,2) = 0;
		(*this)(1,2) = 0;
		(*this)(2,2) = 1/(zNear-zFar);
		(*this)(3,2) = 0;

		(*this)(0,3) = 0;
		(*this)(1,3) = 0;
		(*this)(2,3) = zNear/(zNear-zFar);
		(*this)(3,3) = 1;
	}



	//! Builds a right-handed orthogonal projection matrix.
	inline void matrix4::buildProjectionMatrixOrthoRH(f32 widthOfViewVolume, f32 heightOfViewVolume, f32 zNear, f32 zFar)
	{
		(*this)(0,0) = 2/widthOfViewVolume;
		(*this)(1,0) = 0;
		(*this)(2,0) = 0;
		(*this)(3,0) = 0;

		(*this)(0,1) = 0;
		(*this)(1,1) = 2/heightOfViewVolume;
		(*this)(2,1) = 0;
		(*this)(3,1) = 0;

		(*this)(0,2) = 0;
		(*this)(1,2) = 0;
		(*this)(2,2) = 1/(zNear-zFar);
		(*this)(3,2) = 0;

		(*this)(0,3) = 0;
		(*this)(1,3) = 0;
		(*this)(2,3) = zNear/(zNear-zFar);
		(*this)(3,3) = -1;
	}


	//! Builds a right-handed perspective projection matrix.
	inline void matrix4::buildProjectionMatrixPerspectiveRH(f32 widthOfViewVolume, f32 heightOfViewVolume, f32 zNear, f32 zFar)
	{
		(*this)(0,0) = 2*zNear/widthOfViewVolume;
		(*this)(1,0) = 0;
		(*this)(2,0) = 0;
		(*this)(3,0) = 0;

		(*this)(0,1) = 0;
		(*this)(1,1) = 2*zNear/heightOfViewVolume;
		(*this)(2,1) = 0;
		(*this)(3,1) = 0;

		(*this)(0,2) = 0;
		(*this)(1,2) = 0;
		(*this)(2,2) = zFar/(zNear-zFar);
		(*this)(3,2) = -1;

		(*this)(0,3) = 0;
		(*this)(1,3) = 0;
		(*this)(2,3) = zNear*zFar/(zNear-zFar);
		(*this)(3,3) = 0;
	}


	//! Builds a left-handed perspective projection matrix.
	inline void matrix4::buildProjectionMatrixPerspectiveLH(f32 widthOfViewVolume, f32 heightOfViewVolume, f32 zNear, f32 zFar)
	{
		(*this)(0,0) = 2*zNear/widthOfViewVolume;
		(*this)(1,0) = 0;
		(*this)(2,0) = 0;
		(*this)(3,0) = 0;

		(*this)(0,1) = 0;
		(*this)(1,1) = 2*zNear/heightOfViewVolume;
		(*this)(2,1) = 0;
		(*this)(3,1) = 0;

		(*this)(0,2) = 0;
		(*this)(1,2) = 0;
		(*this)(2,2) = zFar/(zFar-zNear);
		(*this)(3,2) = 1;

		(*this)(0,3) = 0;
		(*this)(1,3) = 0;
		(*this)(2,3) = zNear*zFar/(zNear-zFar);
		(*this)(3,3) = 0;
	}


	//! Builds a matrix that flattens geometry into a plane.
	inline void matrix4::buildShadowMatrix(const core::vector3df& light, core::plane3df plane, f32 point)
	{
		plane.Normal.normalize();
		f32 d = plane.Normal.dotProduct(light);

		(*this)(0,0) = plane.Normal.X * light.X + d;
		(*this)(1,0) = plane.Normal.X * light.Y;
		(*this)(2,0) = plane.Normal.X * light.Z;
		(*this)(3,0) = plane.Normal.X * point;

		(*this)(0,1) = plane.Normal.Y * light.X;
		(*this)(1,1) = plane.Normal.Y * light.Y + d;
		(*this)(2,1) = plane.Normal.Y * light.Z;
		(*this)(3,1) = plane.Normal.Y * point;

		(*this)(0,2) = plane.Normal.Z * light.X;
		(*this)(1,2) = plane.Normal.Z * light.Y;
		(*this)(2,2) = plane.Normal.Z * light.Z + d;
		(*this)(3,2) = plane.Normal.Z * point;

		(*this)(0,3) = plane.D * light.X + d;
		(*this)(1,3) = plane.D * light.Y;
		(*this)(2,3) = plane.D * light.Z;
		(*this)(3,3) = plane.D * point;
	}

	//! Builds a left-handed look-at matrix.
	inline void matrix4::buildCameraLookAtMatrixLH(	const vector3df& position,
													const vector3df& target,
													const vector3df& upVector)
	{
		vector3df zaxis = target - position;
		zaxis.normalize();

		vector3df xaxis = upVector.crossProduct(zaxis);
		xaxis.normalize();

		vector3df yaxis = zaxis.crossProduct(xaxis);

		(*this)(0,0) = xaxis.X;
		(*this)(1,0) = yaxis.X;
		(*this)(2,0) = zaxis.X;
		(*this)(3,0) = 0;

		(*this)(0,1) = xaxis.Y;
		(*this)(1,1) = yaxis.Y;
		(*this)(2,1) = zaxis.Y;
		(*this)(3,1) = 0;

		(*this)(0,2) = xaxis.Z;
		(*this)(1,2) = yaxis.Z;
		(*this)(2,2) = zaxis.Z;
		(*this)(3,2) = 0;

		(*this)(0,3) = -xaxis.dotProduct(position);
		(*this)(1,3) = -yaxis.dotProduct(position);
		(*this)(2,3) = -zaxis.dotProduct(position);
		(*this)(3,3) = 1.0f;
	}



	//! Builds a right-handed look-at matrix.
	inline void matrix4::buildCameraLookAtMatrixRH(	const vector3df& position,
													const vector3df& target,
													const vector3df& upVector)
	{
		vector3df zaxis = position - target;
		zaxis.normalize();

		vector3df xaxis = upVector.crossProduct(zaxis);
		xaxis.normalize();

		vector3df yaxis = zaxis.crossProduct(xaxis);

		(*this)(0,0) = xaxis.X;
		(*this)(1,0) = yaxis.X;
		(*this)(2,0) = zaxis.X;
		(*this)(3,0) = 0;

		(*this)(0,1) = xaxis.Y;
		(*this)(1,1) = yaxis.Y;
		(*this)(2,1) = zaxis.Y;
		(*this)(3,1) = 0;

		(*this)(0,2) = xaxis.Z;
		(*this)(1,2) = yaxis.Z;
		(*this)(2,2) = zaxis.Z;
		(*this)(3,2) = 0;

		(*this)(0,3) = -xaxis.dotProduct(position);
		(*this)(1,3) = -yaxis.dotProduct(position);
		(*this)(2,3) = -zaxis.dotProduct(position);
		(*this)(3,3) = 1.0f;
	}


	//! creates a new matrix as interpolated matrix from to other ones.
	//! \param time: Must be a value between 0 and 1.
	inline matrix4 matrix4::interpolate(const core::matrix4& b, f32 time) const
	{
		f32 ntime = 1.0f - time;
		matrix4 mat;
		for (s32 i=0; i<16; ++i)
			mat.M[i] = M[i]*ntime + b.M[i]*time;

		return mat;
	}

	//! returns transposed matrix
	inline matrix4 matrix4::getTransposed() const
	{
		matrix4 t;
		for (s32 c=0; c<4; ++c)
			for (s32 r=0; r<4; ++r)
				t(r,c) = (*this)(c,r);
		return t;
	}


	// transform (x,y,z,1)
	inline void matrix4::transformVect(f32 *out,const vector3df &in) const
	{
		out[0] = in.X*M[0] + in.Y*M[4] + in.Z*M[8] + M[12];
		out[1] = in.X*M[1] + in.Y*M[5] + in.Z*M[9] + M[13];
		out[2] = in.X*M[2] + in.Y*M[6] + in.Z*M[10] + M[14];
		out[3] = in.X*M[3] + in.Y*M[7] + in.Z*M[11] + M[15];
	}

	// used to scale <-1,-1><1,1> to viewport
	inline void matrix4::buildNDCToDCMatrix( const core::rect<s32>& viewport, f32 zScale)
	{
		f32 scaleX = (viewport.getWidth() - 0.75f ) / 2.f;
		f32 scaleY = -(viewport.getHeight() - 0.75f ) / 2.f;

		f32 dx = -0.5f + ( (viewport.UpperLeftCorner.X + viewport.LowerRightCorner.X ) / 2.f );
		f32 dy = -0.5f + ( (viewport.UpperLeftCorner.Y + viewport.LowerRightCorner.Y ) / 2.f );

		makeIdentity();
		M[0] = scaleX;
		M[5] = scaleY;
		M[10] = zScale;
		M[12] = dx;
		M[13] = dy;
	}
} // end namespace core
} // end namespace irr

#endif 

