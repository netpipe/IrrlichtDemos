// Copyright NVIDIA Corporation 2002-2005
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

#include "nvmath/Vec3f.h"
#include "nvmath/Vecnf.h"

#if !defined(NDEBUG)
# define NVSG_TRACE_VEC4F(v) \
  NVSG_TRACE_OUT_F((#v " 4-component-vector output:\n\t%f\t%f\t%f\t%f\n", v[0], v[1], v[2], v[3])) 
#else
# define NVSG_TRACE_VEC4F(v)
#endif

namespace nvmath
{
  //! Vector class with 4 float components.
  class Vec4f : public Vecnf<4>
  {
    public:
      //! Default constructor.
      /** For performance reasons no initialization is performed. */
      NVSG_API Vec4f( void );

      //! Constructor using four floats.
      /** A \c Vec4f with the given values is constructed.  */
      NVSG_API Vec4f( float x
                    , float y
                    , float z
                    , float w
                    );

      //! Constructor using a \c Vec3f and a float
      NVSG_API Vec4f( const Vec3f &v3 //!<  \c Vec3f with x, y, z component
                    , float w         //!<  w component
                    );
     
      //! Copy constructor.
      NVSG_API Vec4f( const Vecnf<4> &v //!<  \c Vec4f to copy
                    );

      //! Set the \c Vec4f with four float values.
      NVSG_API void set( float x
                       , float y
                       , float z
                       , float w
                       );
  };

  // - - - - -  - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
  // non-member functions
  // - - - - -  - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
  /*! \relates nvmath::Vec4f
   *  Multiplication of a scalar with a \c Vec4f.
   *  \returns product of \a f with \a v */
  inline const Vec4f operator*( float f         //!<  scalar to multiply with
                              , const Vec4f &v  //!<  \c Vec4f to multiply
                              )
  {
    return( v * f );
  }

  // - - - - -  - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
  // inlines
  // - - - - -  - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
  inline Vec4f::Vec4f( void )
  {
  }

  inline Vec4f::Vec4f( float x, float y, float z, float w )
  {
    (*this)[0]=x;
    (*this)[1]=y;
    (*this)[2]=z;
    (*this)[3]=w;
  }

  inline Vec4f::Vec4f( const Vec3f &v3, float w )
  {
    (*this)[0] = v3[0];
    (*this)[1] = v3[1];
    (*this)[2] = v3[2];
    (*this)[3] = w;
  }

  inline Vec4f::Vec4f( const Vecnf<4> &v )
    : Vecnf<4>( v )
  {
  }

  inline void Vec4f::set( float x, float y, float z, float w )
  {
    (*this)[0]=x;
    (*this)[1]=y;
    (*this)[2]=z;
    (*this)[3]=w;
  }

} // end namespace nvmath
