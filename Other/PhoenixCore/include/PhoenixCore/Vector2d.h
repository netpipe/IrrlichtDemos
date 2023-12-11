/*

Copyright (c) 2010, Jonathan Wayne Parrott

Please see the license.txt file included with this source
distribution for more information.

*/

#ifndef __PHVECTOR2D_H__
#define __PHVECTOR2D_H__

#include <math.h>
#include "config.h"
#include "RotationMatrix.h"

namespace phoenix
{

    //! Radians to degrees.
    /*!
        Converts radians to degrees
        \param rad Angle in radians.
        \return Angle in degrees.
        \sa phoenix::DegreesToRadians()
    */
    inline float RadiansToDegrees(float rad)
    {
        return rad * 57.2957795f; // 180/pi
    }

    //! Degrees to radians.
    /*!
        Converts degrees to radians.
        \param deg Angle in degrees.
        \return Angle in radians.
        \sa phoenix::RadiansToDegrees()
    */
    inline float DegreesToRadians(float deg)
    {
        return deg * 0.0174532925f; // pi/180
    }

    //! Vector class
    /*!
        Provides a fast and efficient way to do vector math.
        \note Although this class has 3 components (x,y, and z), it is <b>not</b> a true 3d vector. Some functions do not take Z into account. These functions are getAngle(), rotate(), cross product, and project().
    */
    class Vector2d
    {
    public:

        Vector2d()
            : x(0), y(0), z(0) {}
        Vector2d(float a, float b, float c = 0)
            : x(a), y(b), z(c) {}

        ~Vector2d()
            {}

        inline const float getX() const
        {
            return x;
        }
        inline const float getY() const
        {
            return y;
        }
        inline const float getZ() const
        {
            return z;
        }

        inline void setX( const float &a )
        {
            x = a;
        }
        inline void setY( const float &b )
        {
            y = b;
        }
        inline void setZ( const float &c )
        {
            y = c;
        }

        //! Add & Assign
        inline Vector2d& operator+=(const Vector2d &rhs)
        {
            x += rhs.x;
            y += rhs.y;
            z += rhs.z;
            return (*this);
        }
        //! Subtract & Assign
        inline Vector2d& operator-=(const Vector2d &rhs)
        {
            x -= rhs.x;
            y -= rhs.y;
            z -= rhs.z;
            return (*this);
        }

		//! Binary Addition
        inline const Vector2d operator+(const Vector2d &rhs) const { return Vector2d( *this ) += rhs; }

		//! Binary Subtration
        inline const Vector2d operator-(const Vector2d &rhs) const { return Vector2d( *this ) -= rhs; }

        //! Scalar addition, adds Vector2d( rhs, rhs ).
        inline const Vector2d operator+(const float &rhs) const { return (*this) + Vector2d(rhs,rhs); }

        //! Scalar subtraction.
        inline const Vector2d operator-(const float &rhs) const { return (*this) - Vector2d(rhs,rhs); }

        //! Scalar multiplication ( Scale ).
		/*!
			\f$ \vec{v} = ( \vec{v}_x * R, \vec{v}_y * R, \vec{v}_z * R ) \f$
		*/
        inline const Vector2d operator*(const float &rhs) const { return Vector2d( x * rhs, y * rhs, z * rhs ); }

        //! Scalar division ( Inverse Scale ).
        inline const Vector2d operator/(const float &rhs) const { return Vector2d( x / rhs, y / rhs, z / rhs ); }

        //! Scalar add & assign
        inline Vector2d& operator+=(const float &rhs) { (*this) = (*this)+rhs; return (*this); }

        //! Scalar sub & assign
        inline Vector2d& operator-=(const float &rhs) { (*this) = (*this)-rhs; return (*this); }

        //! Scalar mult & assign
        inline Vector2d& operator*=(const float &rhs) { (*this) = (*this)*rhs; return (*this); }

        //! Scalar div & assign
        inline Vector2d& operator/=(const float &rhs) { (*this) = (*this)/rhs; return (*this); }

        //! Unary - (Negate)
		/*!
			\f$ -\vec{v} \f$
		*/
        inline const Vector2d operator-(void) const { return Vector2d() - (*this); }

        //! Equal to
        inline bool operator == (const Vector2d &rhs) const
        {
            if( x == rhs.x && y == rhs.y && z == rhs.z )
            {
                return true;
            }
            return false;
        }

        //! Not equal to
        inline bool operator != (const Vector2d &rhs) const { return !( (*this) == rhs ); }

        //! Dot Product
		/*!
			\f$ \vec{v} \cdot \vec{r} = (\vec{v}_x * \vec{r}_x) + (\vec{v}_y * \vec{r}_y) + (\vec{v}_z * \vec{r}_z) \f$
		*/
        inline const float operator*(const Vector2d &rhs) const { return (x*rhs.x) + (y*rhs.y) + (z*rhs.z); }

        //! Cross Product
		/*!
			\note Does not take Z into account.
			\f$ \vec{v} \times \vec{r} = ( \vec{v}_x * \vec{r}_y ) - ( \vec{v}_y * \vec{r}_x ) \f$
		*/
        inline const float operator^(const Vector2d &rhs) const { return (x * rhs.y) - (y * rhs.x); }

        //! Normalization.
        /*!
            Normalizes the vector, turning it into a unit-length vector.<br>
			\f$ \vec{v} = |\vec{v}| \f$
            \return The length of the vector before normalization.
        */
        inline const float normalize()
        {
            float fLength = getMagnitude();

            // Don't divide by zero.
            if (fLength == 0.0f)
                return 0.0f;

            (*this) /= fLength;

            return fLength;
        }

		//! Magnitude.
        /*!
			The magnitude (length) of the vector.
			\f$ |\vec{v}| = \sqrt{ x^2 + y^2 + z^2 } \f$
            \sa getManitudeSquared();
        */
        inline const float getMagnitude() const { return sqrt( x*x + y*y + z*z ); }

        //! Magnitude squared.
        /*!
			The squared magnitude (length) of the vector.
            \note This is much faster than getMagnitude() because it does not bother to take the square root.
			\f$ ( x^2 + y^2 + z^2 ) \f$
            \sa getMagnitude();
        */
        inline const float getMagnitudeSquared() const { return (x*x + y*y + z*z); }

        //! Projection.
        /*!
            Projects this vector onto vector b.<br>
			\f$ \vec{v}_x = \vec{v} \cdot \vec{b} * \vec{b}_x \f$<br>\f$ \vec{v}_y = \vec{v} \cdot \vec{b} * \vec{b}_y \f$
            \param rhs Vector to project this vector onto
            \returns The product of projection.
        */
        inline const Vector2d project(const Vector2d& rhs) const
        {
            Vector2d v;
            v = rhs * ((*this) * rhs / rhs.getMagnitudeSquared());
            return v;
        }

        //! Direction.
        /*!
			\f$ |\vec{v}| \f$
            \returns A unit-length (normalized) vector with the same angle measure as this one.
        */
        inline const Vector2d getDirection(void) const
        {
            Vector2d Temp(*this);
            Temp.normalize();
            return Temp;
        }

        //! Rotate.
        /*!
			\f$ \vec{v} = \vec{v} * \left[M\right] \f$<br>
            Rotates this vector by a given rotation matrix.
        */
        inline const Vector2d& rotate(const RotationMatrix& _mtx) { return (*this) *= _mtx; }

        //! Get angle.
        /*!
            Get the measure of this vector relative to the reference vector given in radians. 
			By defualt this is (1,0) (0 radians). but it can be changed.<br>
			\f$ \theta = atan2( \vec{v} \times e, \vec{v} \cdot \vec{e} )\f$
            \param _refv The reference vector, by defualt this is <1,0>
        */
        inline const float getAngle(const Vector2d& _refv = Vector2d(1.0f,0) ) const
        {
            float dot = (*this) * _refv;
            float cross = (*this) ^ _refv;

            // angle between segments
            float angle = (float) atan2(cross, dot);

            // Clamp return values to {0,360}
            if (angle > 0)
                return angle;
            else
                return (angle)+DegreesToRadians(360.0f);
        }

        //! Matrix multiplication.
        /*!
			\f$ \vec{v} = \vec{v} * \left[M\right] \f$<br>
            Essentially rotates this vector by the matrix.
            \note this is technically mathematically illegal. The matrix should be multiplied by the vector.
        */
        inline const Vector2d operator * (const RotationMatrix& rhs) const
        {
			Vector2d temp(*this);
			temp.setX( ( rhs.getElement(0) * getX() ) + ( rhs.getElement(1) * getY() ) );
			temp.setY( ( rhs.getElement(2) * getX() ) + ( rhs.getElement(3) * getY() ) );
			return temp;
        }

		//! Matrix multiply and assign;
        inline const Vector2d& operator *= (const RotationMatrix& rhs)
        {
            *this = *this * rhs;
            return *this;
        }

        //! Angle based sort.
        /*!
            When used with std::sort, will sort a container of Vector2ds in order
            of increasing angle (Counter-clockwise).
        */
        static bool AngleSort( const Vector2d& lhs, const Vector2d& rhs )
        {
            if( lhs.getAngle() < rhs.getAngle() )
                return true;
            else
                return false;
        }

    protected:
        float x,y,z;
    private:
    };

    //! Left-hand float * Vector2d
    inline const Vector2d operator*(const float &lhs, const Vector2d &rhs) { return rhs * lhs; }

	//! Matrix by Vector multiplication. (Rotate)
	/*!
		Mutliplies the Matrix times the Column Matrix represented by the given vector.
		Essentially rotates the given vector by the given matrix.
		\f$  \vec{v} = M \times \left[\begin{array}{c}x \\ y \\ z\end{array}\right]  \f$
	*/
	inline const Vector2d operator*(const RotationMatrix& lhs, const phoenix::Vector2d& rhs)
	{
		return rhs * lhs;
	}


} //namespace phoenix

#endif // __PHVECTOR2D_H__
