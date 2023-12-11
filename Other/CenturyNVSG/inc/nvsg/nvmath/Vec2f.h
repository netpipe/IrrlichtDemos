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
#include  "nvmath/Vecnf.h"

#if !defined(NDEBUG)
# define NVSG_TRACE_VEC2F(v) \
  NVSG_TRACE_OUT_F((#v " 2-component-vector output:\n\t%f\t%f\n", v[0], v[1])) 
#else
# define NVSG_TRACE_VEC2F(v)
#endif

namespace nvmath
{
  //! Vector class with 2 float components.
  class Vec2f : public Vecnf<2>
  {
    public:
      //! Default constructor.
      /** For performance reasons no initialization is performed. */
      NVSG_API Vec2f( void );

      //! Constructor using two floats.
      /** A \c Vec2f with the given values is constructed.  */
      NVSG_API Vec2f( float x
                    , float y
                    );

      //! Copy constructor
      NVSG_API Vec2f( const Vecnf<2> & v );

      //! Set \c this with two float values.
      NVSG_API void set( float x
                       , float y
                       );
  };

  // - - - - -  - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
  // non-member functions
  // - - - - -  - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
  /*! \relates nvmath::Vec2f
   *  Multiplication of a scalar with a \c Vec2f.
   *  \returns product of \a f with \a v */
  inline const Vec2f operator*( float f         //!<  scalar to multiply with
                              , const Vec2f &v  //!<  Vec2f to multiply
                              )
  {
    return( v * f );
  }

  // - - - - -  - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
  // inlines
  // - - - - -  - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
  inline Vec2f::Vec2f( void )
  {
  }

  inline Vec2f::Vec2f( float x, float y )
  {
    (*this)[0] = x;
    (*this)[1] = y;
  }

  inline Vec2f::Vec2f( const Vecnf<2> &v )
    : Vecnf<2>( v )
  {
  }

  inline void Vec2f::set( float x, float y )
  {
    (*this)[0]=x;
    (*this)[1]=y;
  }

} // end namespace nvmath
