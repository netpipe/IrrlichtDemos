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
#include "nvmath/Matnnf.h"
#include "nvmath/Vec3f.h"

#if !defined(DOXYGEN_IGNORE)
#if !defined(NDEBUG)
# define NVSG_TRACE_MAT33F(m) \
  NVSG_TRACE_OUT_F(( \
    #m " 3x3-matrix output:\n" \
    "\t%e\t%e\t%e\n\t%e\t%e\t%e\n\t%e\t%e\t%e\n" \
    , m[0][0], m[0][1], m[0][2], m[1][0], m[1][1], m[1][2], m[2][0], m[2][1], m[2][2] )) 
#else
# define NVSG_TRACE_MAT33F(m)
#endif
#endif    // DOXYGEN_IGNORE

namespace nvmath
{
  class Quatf;

  //! Two dimensional matrix of three times three floats.
  /** A \c Mat33f can represent a rotation plus scaling in 3D. To also represent a translation, you need to use a \c Mat44f. */
  class Mat33f : public Matnnf<3>
  {
    public:
      //! Default constructor.
      /** For performance reasons no initialization is performed. */
      NVSG_API Mat33f( void );

      //! Constructor using 9 floats.
      /** The 9 floats are used as the matrix elements. No normalization is performed.  */
      NVSG_API Mat33f( float a00, float a01, float a02
                     , float a10, float a11, float a12
                     , float a20, float a21, float a22 );

      //! Constructor using three row vectors.
      /** The three vectors are used as the three rows of the matrix. No normalization is performed.  */
      NVSG_API Mat33f( const Vec3f &row0
                     , const Vec3f &row1
                     , const Vec3f &row2
                     );

      //! Constructor using a normalized rotation axis and angle (in radians).
      /** This results in a pure rotation matrix. */
      NVSG_API Mat33f( const Vec3f &axis    //!<  rotation axis
                     , float rad            //!<  angle to rotate (in radians)
                     );

      //! Constructor using a normalized quaternion.
      /** This results in a pure rotation matrix. */
      NVSG_API Mat33f( const Quatf &quat  //!<  quaternion for rotation
                     );

      //! Copy constructor
      NVSG_API Mat33f( const Matnnf<3> & m  //!< matrix to copy from
                     );

      //! Set the matrix with three row vectors.
      NVSG_API void set( const Vec3f &row0
                       , const Vec3f &row1
                       , const Vec3f &row2
                       );
  };

  // - - - - -  - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
  // non-member functions
  // - - - - -  - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
  /*! \relates nvmath::Mat33f
   *  Decompose the non-singular \c Mat33f \a mat into scaling, orientation, and scaleOrientation.
   *  If those values are converted back to Mat33f, the following concatenation would be equal to
   *  the original matrix:
   *    ~scaleOrientation * scale * scaleOrientation * orientation */
  NVSG_API void decompose( const Mat33f &mat        //!< matrix to decompose
                         , Quatf &orientation       //!< rotational part
                         , Vec3f &scaling           //!< scaling factors
                         , Quatf &scaleOrientation  //!< scaling rotation
                         );

  /*! \relates nvmath::Mat33f
   *  Determine the determinant of a \c Mat33f.
   *  \returns determinant of the \c Mat33f */
  inline float determinant( const Mat33f &mat  //!<  matrix to determine determinant of
                          )
  {
    return( mat[0] * ( mat[1] ^ mat[2] ) );
  }

  /*! \relates nvmath::Mat33f
   *  Test if a \c Mat33f is a rotation. \n
   *  A \c Mat33f is a rotation if it is normalized, orthogonal, and the determinant differs less
   *  than epsilon from one.
   *  \returns \c true if the \a mat is a rotation, otherwise \c false */
  NVSG_API bool isRotation( const Mat33f &mat           //!<  matrix to test
                          , float epsilon = FLT_EPSILON //!< optional epsilon
                          );

  // - - - - -  - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
  // inlines
  // - - - - -  - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
  inline void Mat33f::set( const Vec3f &row0, const Vec3f &row1, const Vec3f &row2 )
  {
    (*this)[0]=row0;
    (*this)[1]=row1;
    (*this)[2]=row2;
  }

} // end namespace nvmath
