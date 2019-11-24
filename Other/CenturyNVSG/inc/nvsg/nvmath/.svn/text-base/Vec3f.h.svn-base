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

#include <algorithm>
#include <algorithm>
#include <vector>
#include "nvmath/nvmath.h"
#include "nvmath/Vecnf.h"

#if !defined(NDEBUG)
# define NVSG_TRACE_VEC3F(v) \
  NVSG_TRACE_OUT_F((#v " 3-component-vector output:\n\t%f\t%f\t%f\n", v[0], v[1], v[2])) 
#else
# define NVSG_TRACE_VEC3F(v)
#endif

namespace nvmath
{
  class Sphere3f;
  class Vec4f;

  //! Vector class with 3 float components.
  class Vec3f : public Vecnf<3>
  {
    public:
      //! Default constructor.
      /** For performance reasons no initialization is performed. */
      NVSG_API Vec3f( void );

      //! Constructor using three floats.
      /** A \c Vec3f with the given values is constructed.  */
      NVSG_API Vec3f( float x
                    , float y
                    , float z
                    );

      //! Copy constructor.
      NVSG_API Vec3f( const Vecnf<3> &v //!<  \c Vec3f to copy
                    );

      //! Constructor by a \c Vec4f.
      /** Ignores fourth component of constructing \c Vec4f.  */
      NVSG_API Vec3f( const Vecnf<4> &v //!<  \c Vec4f to use first three components from
                    );

      //! Assignment operator.
      /** Ignores the fourth component of the assigning \c Vec4f.
          \returns reference to the assigned \c Vec3f */
      NVSG_API Vec3f& operator=( const Vec4f &v   //!<  \c Vec4f to copy first three components from
                               );

      //! Negation operator.
      /** \returns negated \c Vec3f */
      NVSG_API const Vec3f operator-(void) const;
      // don't hide base class' operator-
      using Vecnf<3>::operator-;

      //! Set the \c Vec3f with three float values.
      NVSG_API void set( float x
                       , float y
                       , float z
                       );
  };

  // - - - - -  - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
  // non-member functions
  // - - - - -  - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
  /*! \relates nvmath::Vec3f
   *  Calculate the normalized version of \a v.
   *  \returns normalized version of \a v */
  inline Vec3f normalize( const Vec3f& v  //!<  \c Vec3f to determine normalized \c Vec3f from
                        )
  {
    float norm = std::max( length(v), FLT_EPSILON );
    // return value optimization: safe construction/destruction of a temporary object 
    return( Vec3f( v[0]/norm, v[1]/norm, v[2]/norm ) );
  }

  /*! \relates nvmath::Vec3f
   *  Calculate an array of normalized \c Vec3f from array of \c Vec3f. */
  inline  void  normalize( const std::vector<Vec3f> &vecIn    //!< array of \c Vec3f to normalize
                         , std::vector<Vec3f> &vecOut         //!< array of normalized \c Vec3f
                         )
  {
    std::vector<Vec3f> tmp;
    std::transform( vecIn.begin(), vecIn.end()
                  , std::inserter(tmp, tmp.begin()), (Vec3f (*)(const Vec3f&))&normalize );
    tmp.swap( vecOut );
  }

  /*! \relates nvmath::Vec3f
   *  Multiplication of a scalar with a \c Vec3f.
   *  \returns product of \a f with \a v */
  inline const Vec3f operator*( float f         //!<  scalar to multiply with
                              , const Vec3f &v  //!<  \c Vec3f to multiply
                              )
  {
    return( v * f );
  }

  /*! \relates nvmath::Vec3f
   *  Calculation of the cross product of \a v0 with \a v1.\n
   *  The cross product of two \c Vec3f is a \c Vec3f that is orthogonal to both arguments
   *  \returns cross product of \a v0 and \a v1. */
  inline const Vec3f  operator^( const Vecnf<3> &v0, const Vecnf<3> &v1 )
  {
    Vec3f ret;
    ret[0] = v0[1] * v1[2] - v0[2] * v1[1];
    ret[1] = v0[2] * v1[0] - v0[0] * v1[2];
    ret[2] = v0[0] * v1[1] - v0[1] * v1[0];
    NVSG_ASSERT( areOrthogonal( ret, v0 ) && areOrthogonal( ret, v1 ) );
    return( ret );
  }

  /*! \relates nvmath::Vec3f
   *  Test two \c Vec3f for collinearity.\n
   *  Two \c Vec3f are considered to be collinear, if their cross product differs less than epsilon from zero.
   *  \returns \c true if \a t0 and \a t1 are collinear, otherwise \c false */
  inline bool areCollinear( const Vec3f &v0
                          , const Vec3f &v1
                          , float epsilon = FLT_EPSILON //!< optional epsilon
                          )
  {
    return( length( v0 ^ v1 ) < epsilon );
  }

  /*! \relates nvmath::Vec3f
   *  Determine smoothed normals for a set of vertices.\n
   *  Given some vertices with normals and their bounding \a sphere,
   *  this function calculates smoothed normals for the vertices.\n
   *  For all vertices, that are similar within a tolerance that depends on the
   *  radius of \a sphere, and whose normals differ less than \a creaseAngle 
   *  (in radians), their normals are set to the average of their normals. */
  NVSG_API void  smoothNormals( const std::vector<Vec3f> &vertices  //!< vertices to determine smoothed normals for
                              , const Sphere3f &sphere              //!< bounding sphere of the vertices
                              , float creaseAngle                   //!< crease angle to follow
                              , std::vector<Vec3f> &normals         //!< in: normals of \a vertices; out: smoothed normals
                              );

  // - - - - -  - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
  // inlines
  // - - - - -  - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
  inline void Vec3f::set( float x, float y, float z )
  {
    (*this)[0]=x;
    (*this)[1]=y;
    (*this)[2]=z;
  }

  inline const Vec3f Vec3f::operator-(void) const
  {
    return (Vec3f(-(*this)[0], -(*this)[1], -(*this)[2]));
  }

} // end namespace nvmath
