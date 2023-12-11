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
#include "nvmath/Mat33f.h"
#include "nvmath/Vec4f.h"

#if !defined(DOXYGEN_IGNORE)
#if !defined(NDEBUG)
# define NVSG_TRACE_MAT44F(m) \
  NVSG_TRACE_OUT_F(( \
    #m " 4x4-matrix output:\n" \
    "\t%f\t%f\t%f\t%f\n\t%f\t%f\t%f\t%f\n\t%f\t%f\t%f\t%f\n\t%f\t%f\t%f\t%f\n" \
    , m[0][0], m[0][1], m[0][2], m[0][3], m[1][0], m[1][1], m[1][2], m[1][3] \
    , m[2][0], m[2][1], m[2][2], m[2][3], m[3][0], m[3][1], m[3][2], m[3][3] )) 
#else
# define NVSG_TRACE_MAT44F(m)
#endif
#endif    // DOXYGEN_IGNORE

namespace nvmath
{
  //! Two dimensional matrix of four times four floats.
  /** A \c Mat44f can represent a full transformation (including rotation, scaling, and
    * translation). A Mat44f seen as a transformation, holds the translational part in the last
    * row. That is, if the values of a Mat44f are seen as 16 float values, the translational
    * part are the 13. to 15. value there. */
  class Mat44f : public Matnnf<4>
  {
    public:
      //! Default constructor.
      /** For performance reasons no initialization is performed. */
      NVSG_API Mat44f( void );

      //! Constructor using 16 floats.
      /** The 16 floats are used as the matrix elements. No normalization is performed.  */
      NVSG_API Mat44f( float a00, float a01, float a02, float a03
                     , float a10, float a11, float a12, float a13
                     , float a20, float a21, float a22, float a23
                     , float a30, float a31, float a32, float a33 );

      //! Constructor using four row vectors.
      /** The four vectors are used as the four rows of the matrix. No normalization is performed.  */
      NVSG_API Mat44f( const Vec4f &row0
                     , const Vec4f &row1
                     , const Vec4f &row2
                     , const Vec4f &row3
                     );

      //! Constructor by Translation and Rotation.
      /** This constructs a matrix that represents the concatenation R * T, that is a rotation
        * followed by a translation. */
      NVSG_API Mat44f( const Vec3f &trans                               //!< translation
                     , const Quatf &rot                                 //!< quaternion for rotation
                     );

      //! Constructor by Translation, Rotation, and Scaling.
      /** This constructs a matrix that represents the concatenation S * R * T, that is a scaling,
        * followed by a rotation, followed by a translation. */
      NVSG_API Mat44f( const Vec3f &trans                               //!< translation
                     , const Quatf &rot                                 //!< quaternion for rotation
                     , const Vec3f &scale                               //!< scaling
                     );


      //! Copy constructor
      NVSG_API Mat44f( const Matnnf<4> & m    //!< matrix to copy from
                     );

      //! Set the matrix with four row vectors.
      NVSG_API void set( const Vec4f &row0
                       , const Vec4f &row1
                       , const Vec4f &row2
                       , const Vec4f &row3
                       );
  };

  // - - - - -  - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
  // non-member functions
  // - - - - -  - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
  /*! \relates nvmath::Mat44f
   *  Decompose the non-singular \c Mat44f \a mat into translation, orientation, scaling, and
   *  scaleOrientation.\n
   *  The matrix is assumed not to be a projection matrix. \n
   *  The last row is considered to hold the translation. \n
   *  The scaling, orientation, and scaleOrientation is calculated by decomposing the upper left
   *  3x3 sub-matrix. That is, if those values are converted back to Mat33f, the following
   *  concatentation would be equal to the original matrix:
   *    ~scaleOrientation * scale * scaleOrientation * orientation * translation */
  inline  void  decompose( const Mat44f &mat        //!<  matrix to decompose
                         , Vec3f &translation       //!<  translation vector
                         , Quatf &orientation       //!<  rotational part
                         , Vec3f &scaling           //!<  scaling factors
                         , Quatf &scaleOrientation  //!< scaling rotation
                         )
  {
    translation = Vec3f( mat[3][0], mat[3][1], mat[3][2] );
    Mat33f  m33( mat[0][0], mat[0][1], mat[0][2],
                 mat[1][0], mat[1][1], mat[1][2],
                 mat[2][0], mat[2][1], mat[2][2] );
    decompose( m33, orientation, scaling, scaleOrientation );
  }

  //! global identity matrix.
  extern NVSG_API const Mat44f  cIdentity44f;

} // end namespace nvmath
