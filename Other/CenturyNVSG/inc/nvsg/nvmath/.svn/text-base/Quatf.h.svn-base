// Copyright NVIDIA Corporation 2002-2006
// TO THE MAXIMUM EXTENT PERMITTED BY APPLICABLE LAW, THIS SOFTWARE IS PROVIDED
// *AS IS* AND NVIDIA AND ITS SUPPLIERS DISCLAIM ALL WARRANTIES, EITHER EXPRESS
// OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY
// AND FITNESS FOR A PARTICULAR PURPOSE.  IN NO EVENT SHALL NVIDIA OR ITS SUPPLIERS
// BE LIABLE FOR ANY SPECIAL, INCIDENTAL, INDIRECT, OR CONSEQUENTIAL DAMAGES
// WHATSOEVER (INCLUDING, WITHOUT LIMITATION, DAMAGES FOR LOSS OF BUSINESS PROFITS,
// BUSINESS INTERRUPTION, LOSS OF BUSINESS INFORMATION, OR ANY OTHER PECUNIARY LOSS)
// ARISING OUT OF THE USE OF OR INABILITY TO USE THIS SOFTWARE, EVEN IF NVIDIA HAS
// BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES 

#pragma once
/** @file */

#include "nvsgcommon.h"

#include "nvmath/nvmath.h"
#include "nvmath/Vec4f.h"
#include "nvmath/Vec3f.h"
#include "nvmath/Mat33f.h"

#if !defined(NDEBUG)
# define NVSG_TRACE_QUATF(q) \
  NVSG_TRACE_OUT_F((#q " quaternion output:\n\t%f\t%f\t%f\t%f\n", q[0], q[1], q[2], q[3])) 
#else
# define NVSG_TRACE_QUATF(q)
#endif

namespace nvmath
{
  //! Quaternion class.
  /** Quaternions are an alternative to the 3x3 matrices that are typically used for 3-D rotations.
    * A unit quaternion represents an axis in 3-D space and a rotation around that axis. Every
    * rotation can be expressed that way.
    * \note Only unit quaternions represent a rotation.
    */
  class Quatf
  {
    public:
      //! Default constructor.
      /** For performance reasons no initialization is performed. */
      NVSG_API Quatf( void );

      //! Constructor using a Vec4f.
      /** The four values of \c v are just copied over to the quaternion. It is assumed
        * that this operation gives a normalized quaternion. */
      NVSG_API Quatf( const Vec4f& v   //!<  vector to construct from
                    );

      //! Constructor using four floats.
      /** The four values \c x, \c y, \c z, and \c w are used for the quaternion. It is assumed
        * that this operation gives a normalized quaternion.
        * \note \c x, \c y, and \c z are \b not the x,y,z-component of the rotation axis,
        * and \c w is \b not the rotation angle. If you have such values handy, use the
        * constructor that takes the axis as a Vec3f and an angle. */
      NVSG_API Quatf( float x
                    , float y
                    , float z
                    , float w
                    );

      //! Constructor using an axis and an angle
      /** The resulting quaternion represents a rotation by \c angle radians around \c axis. */
      NVSG_API Quatf( const Vec3f& axis   //!<  axis to use for quaternion
                    , float angle         //!<  rotation angle in radians
                    );

      //! Constructor by two vectors.
      /** The resulting quaternion represents the rotation that maps \a vec0 to \a vec1. */
      NVSG_API Quatf( const Vec3f& vec0    //!<  start vector
                    , const Vec3f& vec1    //!<  end vector
                    );

      //! Constructor by a rotation matrix
      /** The resulting quaternion represents the same rotation as the matrix. */
      NVSG_API explicit Quatf( const Mat33f &rot   //!< rotation matrix
                             );

      //! Non-constant subscript operator.
      /** \returns reference to \a i th element of the quaternion  */
      NVSG_API float& operator[]( size_t i   //!<  index to element to use (i=0,1,2,3)
                                );

      //! Constant subscript operator.
      /** \returns constant reference to \a i th element of the quaternion  */
      NVSG_API const float& operator[]( size_t i   //!<  index to element to use (i=0,1,2,3)
                                      ) const;

      //! Assignment operator.
      /** \returns reference to the assigned quaternion  */
      NVSG_API Quatf& operator=( const Quatf& q   //!<  quaternion to copy
                               );

      //! Equality operator.
      /** Two quaternions are considered to be equal, if each component of the one
        * quaternion deviates less than epsilon from the corresponding element of
        * the other quaternion.
        * \returns \c true if the quaternion is equal to \a q, otherwise \c false  */
      NVSG_API bool operator==( const Quatf& q    //!<  quaternion to compare with
                              ) const;

      //! Inequality operator.
      /** Two quaternions are considered to be equal, if each component of the one
        * quaternion deviates less than epsilon from the corresponding element of
        * the other quaternion.
        * \returns \c true if the quaternion is not equal to \a q, otherwise \c false */
      NVSG_API bool operator!=( const Quatf& q    //!<  quaternion to compare
                              ) const;

      //! Negation operator.
      /** The negation of a quaternion is a rotation around the same axis by the negated angle.
        * \returns negated quaternion  */
      NVSG_API const Quatf operator-( void ) const;

      //! Quaternion multiplication with a quaternion (i.e. concatenation) and assignment operator.
      /** Multiplying two quaternions give a quaternion that represents the concatenation of the
        * rotations represented by the two quaternions.
        * \returns reference to the assigned quaternion after multiplication (concatenation) */
      NVSG_API Quatf& operator*=( const Quatf& q    //!<  quaternion to use in the multiplication
                                );

      /*! \brief Multiplication with a quaternion (i.e. concatentation of rotations).
       *  \param q The quaternion to multiply with
       *  \return The resulting quaternion (again representing a rotation).
       *  \remarks Multiplying two quaternions gives a quaternion that represents the
       *  concatentation of the rotation represented by the two quaternions. Besides rounding
       *  errors, the following equation holds:
       *  \code
       *    Mat33f( q0 * q1 ) == Mat33f( q0 ) * Mat33f( q1 )
       *  \endcode */
      NVSG_API Quatf operator*( const Quatf& q    //!<  quaternion to multiply with
                              ) const;

      //! Quaternion multiplication with a Vec3f.
      /** Multiplying a quaternion with a Vec3f \a v applies the rotation to \a v.
      * \return \a v rotated by the quaternions rotation. */
      NVSG_API const Vec3f operator*( const Vec3f &v    //!< vector to rotate
                                    ) const;

      //! Quaternion division by a quaternion and assignment operator.
      /** Dividing a quaternion by an other gives a quaternion that represents the concatenation
        * of the rotation represented by the numerator quaternion and the rotation represented
        * by the conjugated denumerator quaternion. 
        * \returns reference to the assigned quaternion after division */
      NVSG_API Quatf& operator/=( const Quatf& q //!<  quaternion to divide by
                                );

      //! Quaternion division by a quaternion operator.
      /** Dividing a quaternion by an other gives a quaternion that represents the concatenation
        * of the rotation represented by the numerator quaternion and the rotation represented
        * by the conjugated denumerator quaternion. 
        * \returns divided quaternion */
      NVSG_API const Quatf operator/( const Quatf& q    //!<  quaternion to divide by
                                    ) const;

      //! Quaternion conjugation operator.
      /** The conjugation of a quaternion is a rotation of the same angle around the negated axis.
        * \returns conjugated quaternion */
      NVSG_API const Quatf operator~( void ) const;

      //! Normalize the quaternion.
      /** \note We always assume a quaternion to be normalized. But when getting data from outside
        * or due to numerically instabilities, a quaternion might become unnormalized. You can use
        * this function then to normalize it again.
        * \returns a reference to the normalized quaternion. */
      NVSG_API Quatf& normalize( void );

    private:
      float m_quat[4];
  };

  // - - - - -  - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
  // non-member functions
  // - - - - -  - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
  /*! \relates nvmath::Quatf
   *  Decompose a quaternion into axis and angle. \n
   *  The quaternion \c q represents the rotation by \c angle radians around \c axis. */
  inline  void  decompose( const Quatf &q   //!<  Quaternion to decompose
                         , Vec3f &axis      //!<  Resulting axis
                         , float &angle     //!<  Resulting angle (in radians)
                         )
  {
    angle = 2.0f * acosf( q[3] );
    if ( angle < FLT_EPSILON )
    {
      //  no angle to rotate about => take just any one
      axis[0] = 0.0f;
      axis[1] = 0.0f;
      axis[2] = 1.0f;
    }
    else
    {
      float dummy = 1.0f / sinf( 0.5f * angle );
      axis[0] = q[0] * dummy;
      axis[1] = q[1] * dummy;
      axis[2] = q[2] * dummy;
      axis.normalize();
    }
  }

  /*! \relates nvmath::Quatf
   *  Determine the distance between two quaternions.
   *  \returns The distance between \a q0 and \a q1 */
  inline float distance( const Quatf &q0
                       , const Quatf &q1
                       )
  {
    return( sqrtf( square( q0[0] - q1[0] ) + square( q0[1] - q1[1] ) + square( q0[2] - q1[2] ) + square( q0[3] - q1[3] ) ) );
  }

  /*! \relates nvmath::Quatf
   *  Calculate the magnitude of a quaternion. \n
   *  The magnitude of a normalized quaternion is one.
   *  \returns magnitude of the quaternion */
  inline float magnitude( const Quatf &q   //!< Quaternion to determine magnitude of
                        )
  {
    return( sqrtf( square( q[0] ) + square( q[1] ) + square( q[2] ) + square( q[3] ) ) );
  }

  /*! \relates nvmath::Quatf
   *  Test if a quaternion is normalized. \n
   *  A quaternion is considered to be normalized, when it's magnitude differs less than epsilon from one.
   *  \returns \c true if the quaternion is normalized, otherwise \c false */
  inline bool isNormalized( const Quatf &q              //!< Quaternion to test
                          , float epsilon = FLT_EPSILON //!< optional epsilon
                          )
  {
    return( fabsf( magnitude( q ) - 1.0f ) <= 7*epsilon );
  }

  /*! \relates nvmath::Quatf
   *  Test if a quaternion is null.\n
   *  A quaternion is considered to be null, if it's magnitude is less than epsilon.
   *  \returns \c true if the quaternion is null, otherwise \c false */
  inline bool isNull( const Quatf &q              //!< Quaternion to test
                    , float epsilon = FLT_EPSILON //!< optional epsilon
                    )
  {
    return( magnitude( q ) <= epsilon );
  }

  /*! \relates nvmath::Quatf
   *  Spherical linear interpolation between two quaternions \a q0 and \a q1. \n
   *  This calculates the spherical linear interpolation analog to
   *  \code ( 1 - alpha ) * q0 + alpha * q1 \endcode */
  template<>
  NVSG_API Quatf lerp<Quatf>( float alpha          //!<  interpolation parameter
                            , const Quatf &q0      //!<  starting value
                            , const Quatf &q1      //!<  ending value
                            );

  /*! \relates nvmath::Quatf
   *  Multiplying a Vec3f \a with a Quatf \a q applies the rotation of \a q to \a v.
   * \return \a v rotated by the rotation of \a q. */
  inline Vec3f operator*( const Vec3f &v, const Quatf & q )
  {
    return( v * Mat33f(q) );
  }

  // - - - - -  - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
  // inlines
  // - - - - -  - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
  inline Quatf::Quatf( void )
  {
  }

  inline Quatf::Quatf( float f0, float f1, float f2, float f3 )
  {
    m_quat[0] = f0;
    m_quat[1] = f1;
    m_quat[2] = f2;
    m_quat[3] = f3;
  }

  inline Quatf::Quatf( const Vec4f& v )
  {
    m_quat[0] = v[0];
    m_quat[1] = v[1];
    m_quat[2] = v[2];
    m_quat[3] = v[3];
    NVSG_ASSERT( isNormalized( *this ) );
  }

  inline float& Quatf::operator[]( size_t i ) 
  { 
    NVSG_ASSERT( 0 <= i && i <= 3 );
    return( m_quat[i] );
  }

  inline const float& Quatf::operator[]( size_t i ) const 
  { 
    NVSG_ASSERT( 0 <= i && i <= 3 );
    return( m_quat[i] );
  }

  inline Quatf& Quatf::operator=( const Quatf& q )
  {
    m_quat[0] = q[0];
    m_quat[1] = q[1];
    m_quat[2] = q[2];
    m_quat[3] = q[3];
    return( *this );
  }

  inline bool Quatf::operator==( const Quatf& q ) const 
  {
    return(     ( fabsf( m_quat[0] - q[0] ) < FLT_EPSILON )
            &&  ( fabsf( m_quat[1] - q[1] ) < FLT_EPSILON )
            &&  ( fabsf( m_quat[2] - q[2] ) < FLT_EPSILON )
            &&  ( fabsf( m_quat[3] - q[3] ) < FLT_EPSILON ) );
  }

  inline bool Quatf::operator!=( const Quatf& q ) const 
  { 
    return(     m_quat[0] != q[0]
            ||  m_quat[1] != q[1]
            ||  m_quat[2] != q[2]
            ||  m_quat[3] != q[3]
          );
  }

  inline const Quatf Quatf::operator-( void ) const
  {
    return( Quatf( m_quat[0], m_quat[1], m_quat[2], -m_quat[3] ) );
  }

  inline Quatf& Quatf::operator*=( const Quatf & q )
  {
    NVSG_ASSERT( isNormalized( *this ) && isNormalized( q ) );
    *this = Quatf( q[1]*m_quat[2] - q[2]*m_quat[1] + q[3]*m_quat[0] + q[0]*m_quat[3],
                   q[2]*m_quat[0] - q[0]*m_quat[2] + q[3]*m_quat[1] + q[1]*m_quat[3],
                   q[0]*m_quat[1] - q[1]*m_quat[0] + q[3]*m_quat[2] + q[2]*m_quat[3],
                   q[3]*m_quat[3] - q[0]*m_quat[0] - q[1]*m_quat[1] - q[2]*m_quat[2] );
    normalize();
    NVSG_ASSERT( isNormalized( *this ) );
    return( *this );
  }

  inline Quatf Quatf::operator*( const Quatf & q ) const
  {
    NVSG_ASSERT( isNormalized( *this, FLT_EPSILON ) );
    NVSG_ASSERT( isNormalized( q, FLT_EPSILON ) );
    Quatf r(*this);
    r *= q;
    NVSG_ASSERT( isNormalized( r ) );
    return( r );
  }

  inline const Vec3f Quatf::operator*( const Vec3f &v ) const
  {
    return( Mat33f(*this) * v );
  }

  inline Quatf& Quatf::operator/=( const Quatf & q )
  {
    Quatf tmp = *this / q;
    *this = tmp;
    return( *this );
  }

  inline const Quatf Quatf::operator/( const Quatf & q ) const
  {
    NVSG_ASSERT( isNormalized( q ) );
    return( *this * ~q /*/ magnitude( q )*/ );
  }

  inline const Quatf Quatf::operator~( void ) const
  {
    return( Quatf( -m_quat[0], -m_quat[1], -m_quat[2], m_quat[3] ) );
  }
}
