/*

Copyright (c) 2010, Jonathan Wayne Parrott

Please see the license.txt file included with this source
distribution for more information.

*/

#ifndef __PHROTATIONMATRIX_H__
#define __PHROTATIONMATRIX_H__

#include "config.h"
#include <math.h>

namespace phoenix
{

    //! 2D Rotation Matrix Class
    /*!
        Provides a fast and efficient way to rotate vectors and polygons. It can be
		created manually, or automatically computed from radians.
    */
    class RotationMatrix
    {

    public:

        //! Blank Constructor.
        /*!
        	Creates an identity matrix<br>
			\f$ \left[\begin{array}{cc} 1 & 0 \\ 0 & 1 \end{array}\right] \f$
        */
        RotationMatrix()
		{
			Elements[0] = 1;
			Elements[1] = 0;
			Elements[2] = 0;
			Elements[3] = 1;
		}

        //! Element-based Constructor.
        /*!
        	Creates a matrix with given elements. <br>
			\f$ \left[\begin{array}{cc} a & b \\ c & d \end{array}\right] \f$
        */
        RotationMatrix( float a, float b, float c, float d)
		{
			Elements[0] = a;
			Elements[1] = b;
			Elements[2] = c;
			Elements[3] = d;
		}

        //! Radian-based Constructor.
        /*!
        	Creates a rotation matrix with the given angle in radians. (Most common).
			\f$ \left[\begin{array}{cc}  \cos{\Theta} & -\sin{\Theta} \\ \sin{\Theta} & \cos{\Theta} \end{array}\right] \f$
        */
        RotationMatrix( float _theta )
		{
			float c = cos(_theta);
			float s = sin(_theta);
			Elements[0] = c;
			Elements[1] = -s;
			Elements[2] = s;
			Elements[3] = c;
		}

        ~RotationMatrix()
		{}

        //! Get element
		/*
			Follows this indexing scheme:
			\f$ \left[\begin{array}{cc} 0 & 1 \\ 2 & 3 \end{array}\right] \f$
		*/
        inline float getElement( int _i ) const { return Elements[_i % 4]; }

        //! Set element
		/*
			Follows this indexing scheme:
			\f$ \left[\begin{array}{cc} 0 & 1 \\ 2 & 3 \end{array}\right] \f$
		*/
        inline void setElement( int _i, float _v ) { Elements[_i % 4] = _v; }

        //! Set rotation in radians.
        inline void setRotation(float _theta) { (*this) = RotationMatrix(_theta); }

        //! Get rotation in radians.
        inline const float getRotation() const { return acos(Elements[0]); }

        //! Element access operator.
        /*!
            Allows you to access the matrix as a one-dimensional array.
			Follows this indexing scheme:
			\f$ \left[\begin{array}{cc} 0 & 1 \\ 2 & 3 \end{array}\right] \f$
			\note Throws if index is out-of-bounds.
        */
		inline float& operator[](int _i) { if( _i >=0 && _i <4 ) { return Elements[_i]; } else { } }

		//! Addition.
        inline const RotationMatrix operator+(const RotationMatrix& rhs) const
		{
			RotationMatrix temp;
			temp[0] = getElement(0) + rhs.getElement(0);
			temp[1] = getElement(1) + rhs.getElement(1);
			temp[2] = getElement(2) + rhs.getElement(2);
			temp[3] = getElement(3) + rhs.getElement(3);
			return temp;
		}

		//! Subtraction.
        inline const RotationMatrix operator-(const RotationMatrix& rhs) const
		{
			RotationMatrix temp;
			temp[0] = getElement(0) - rhs.getElement(0);
			temp[1] = getElement(1) - rhs.getElement(1);
			temp[2] = getElement(2) - rhs.getElement(2);
			temp[3] = getElement(3) - rhs.getElement(3);
			return temp;
		}


		//! Multiplcation.
        inline const RotationMatrix operator*(const RotationMatrix& rhs) const
		{
			RotationMatrix temp;
			temp[0] = ( getElement(0) * rhs.getElement(0) ) + ( getElement(1) * rhs.getElement(2) );
			temp[1] = ( getElement(0) * rhs.getElement(1) ) + ( getElement(1) * rhs.getElement(3) );
			temp[2] = ( getElement(2) * rhs.getElement(0) ) + ( getElement(3) * rhs.getElement(2)  );
			temp[3] = ( getElement(2) * rhs.getElement(1) ) + ( getElement(3) * rhs.getElement(3)  );
			return temp;
		}

		//! Scale (Scalar Multiplication).
        inline const RotationMatrix operator*(float rhs) const
		{
			RotationMatrix temp;
			temp[0] = Elements[0] * rhs;
			temp[1] = Elements[1] * rhs;
			temp[2] = Elements[2] * rhs;
			temp[3] = Elements[3] * rhs;
			return temp;
		}

		//! Division
        inline const RotationMatrix operator/(const RotationMatrix& rhs) const
		{
			RotationMatrix temp(*this);
			RotationMatrix inv = rhs.getInverse();
			return temp * inv;
		}

		//! Inverse Scale (Scalar Divison)
        inline const RotationMatrix operator/(float rhs) const
		{
			RotationMatrix temp;
			temp[0] = Elements[0] / rhs;
			temp[1] = Elements[1] / rhs;
			temp[2] = Elements[2] / rhs;
			temp[3] = Elements[3] / rhs;
			return temp;
		}

        inline const RotationMatrix& operator+=(const RotationMatrix& rhs)
		{
			(*this) = (*this) + rhs;
			return (*this);
		}

        inline const RotationMatrix& operator-=(const RotationMatrix& rhs)
		{
			(*this) = (*this) - rhs;
			return (*this);
		}

        inline const RotationMatrix& operator*=(const RotationMatrix& rhs)
		{
			(*this) = (*this) * rhs;
			return (*this);
		}

        inline const RotationMatrix& operator*=(float rhs)
		{
			(*this) = (*this) * rhs;
			return (*this);
		}

        inline const RotationMatrix& operator/=(const RotationMatrix& rhs)
		{
			(*this) = (*this) / rhs;
			return (*this);
		}

        inline const RotationMatrix& operator/=(float rhs)
		{
			(*this) = (*this) / rhs;
			return (*this);
		}

        //! Determinant.
        /*!
            \return The determinant of the matrix.
            \note This is used by the inverse() function.
        */
        inline const float getDeterminant() const
		{
			float d1 = Elements[0] + Elements[3];
			float d2 = Elements[1] + Elements[2];
			return (d1 - d2);
		}


        //! Inverse.
        /*!
            \return The multiplicative inverse of this matrix.
            \note This is used by the division operators.
        */
        inline const RotationMatrix getInverse() const
		{
			float det = getDeterminant();
			RotationMatrix temp;
			temp[0] = Elements[3];
			temp[1] = -Elements[1];
			temp[2] = -Elements[2];
			temp[3] = Elements[0];
			return temp*(1.0f/det);
		}

		//! Comparison
        inline bool operator==(const RotationMatrix& rhs) const
		{
			if (getElement(0) == rhs.getElement(0) &&
					getElement(1) == rhs.getElement(1) &&
					getElement(2) == rhs.getElement(2) &&
					getElement(3) == rhs.getElement(3))
			{
				return true;
			}
			return false;
		}


        inline bool operator!=(const RotationMatrix& rhs) const
		{
			if ((*this==rhs))
				return false;
			return true;
		}

    protected:

        float Elements[4];

    private:
}; // class Rotation Matrix.

} //namespace phoenix

#endif // __PHROTATIONMATRIX_H__
