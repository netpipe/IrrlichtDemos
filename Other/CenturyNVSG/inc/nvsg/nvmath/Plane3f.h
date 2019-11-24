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
#include "nvmath/nvmath.h"
#include "nvmath/Vec3f.h"

namespace nvmath
{
  //! Plane class
  /** This represents a plane in 3-space. It's defined by a normal \c n and an offset \c c,
    * such that for any point \c x in the plane, the equation \code n x + c = 0 \endcode holds. */
  class Plane3f
  {
    public:
      //! Default constructor.
      /** For performance reasons no initialization is performed. */
      NVSG_API Plane3f( void );

      //! Constructor by normal and offset
      NVSG_API Plane3f( const Vec3f &n  //!<  normal
                      , float c         //!<  offset
                      );

      //! Constructor by normal and point on plane
      NVSG_API Plane3f( const Vec3f &n  //!<  normal
                      , const Vec3f &p  //!<  point in plane
                      );

      //! Constructor by three points
      NVSG_API Plane3f( const Vec3f &p0
                      , const Vec3f &p1
                      , const Vec3f &p2
                      );

      //! Negation operator.
      /** \returns complementary plane (with reversed normal). */
      NVSG_API Plane3f operator-( void ) const;

      //! Assignment operator
      /** \returns reference to the assigned plane */
      NVSG_API Plane3f & operator=( const Plane3f &pl //!<  plane to assign
                                  );

      //! Equality operator.
      /** Two planes are considered to be equal, if the normals are equal and the
        * offsets differ by less than epsilon.
        * \returns \c true if the plane is equal to \a pl, otherwise \c false  */
      NVSG_API bool operator==( const Plane3f& pl     //!<  plane to compare with
                              ) const;

      //! Inequality operator.
      /** Two planes are considered to be equal, if the normals are equal and the
        * offsets differ by less than epsilon.
        * \returns \c true if the plane is not equal to \a pl, otherwise \c false  */
      NVSG_API bool operator!=( const Plane3f& pl     //!<  plane to compare with
                              ) const;

      //! Calling operator
      /** The point \c p is set into the plane equation, that is the signed distance
        * of the point to the plane is calculated.
        * \returns the signed distance of \a p to the plane. */
      NVSG_API float operator()( const Vec3f &p       //!<  point to determine distance from plane
                               ) const;

      //! Non-constant access to the plane normal.
      /** \returns reference to the plane normal. */
      NVSG_API Vec3f & n( void );

      //! Constant access to the plane normal.
      /** \returns constant reference to the plane normal. */
      NVSG_API const Vec3f & n( void ) const;

      //! Non-constant access to the plane offset.
      /** \returns reference to the plane offset. */
      NVSG_API float & c( void );

      //! Constant access to the plane offset.
      /** \returns constant reference to the plane offset. */
      NVSG_API const float & c( void ) const;

    private:
      Vec3f m_normal;
      float m_offset;
  };

  // - - - - -  - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
  // non-member functions
  // - - - - -  - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
  /*! \relates nvmath::Plane3f
   *  Determine if two points are on opposite sides of a plane.
   *  \returns \c true, if \a p0 and \a p1 are on opposite sides of \a plane. */
  inline bool areOnOppositeSides( const Plane3f &plane    //!<  plane to use
                                , const Vec3f &p0         //!<  first point
                                , const Vec3f &p1         //!<  second point
                                )
  {
    return( plane( p0 ) * plane( p1 ) < 0 );
  }

  /*! \relates nvmath::Plane3f
   *  Determine if two points are on the same side of a plane.
   *  \returns \c true, if \a p0 and \a p1 are on the same side of \a plane. */
  inline bool areOnSameSide( const Plane3f &plane    //!<  plane to use
                           , const Vec3f &p0         //!<  first point
                           , const Vec3f &p1         //!<  second point
                           )
  {
    return( plane( p0 ) * plane( p1 ) > 0 );
  }

  /*! \relates nvmath::Plane3f
   *  Determine signed distance between the Plane \a pl and the Point \a p.
   *  \returns signed distance between \a pl and \a p. */
  inline float signedDistance( const Plane3f &pl   //!<  plane
                             , const Vec3f &p      //!<  point
                             )
  {
    return( pl( p ) );
  }

  /*! \relates nvmath::Plane3f
   *  Determine distance between the Plane \a pl and the Point \a p.
   *  \returns distance between \a pl and \a p. */
  inline float distance( const Plane3f &pl   //!<  plane
                       , const Vec3f &p      //!<  point
                       )
  {
    return( fabsf( pl( p ) ) );
  }

  /*! \relates nvmath::Plane3f
   *  Determine the point on the plane \a pl nearest to the point \a p.
   *  \returns the point on the plane \a pl nearest to the point \a p. */
  inline Vec3f nearestPoint( const Plane3f &pl  //!<  plane
                           , const Vec3f &p     //!<  point
                           )
  {
    return( p - pl( p ) * pl.n() );
  }

  // - - - - -  - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
  // inlines
  // - - - - -  - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
  inline Plane3f Plane3f::operator-( void ) const
  {
    return( Plane3f( -m_normal, -m_offset ) );
  }

  inline Plane3f & Plane3f::operator=( const Plane3f &pl )
  {
    m_normal = pl.n();
    m_offset = pl.c();
    return( *this );
  }

  inline bool Plane3f::operator==( const Plane3f &pl ) const
  {
    return( m_normal == pl.n() && ( fabsf( m_offset - pl.c() ) < FLT_EPSILON ) );
  }

  inline bool Plane3f::operator!=( const Plane3f &pl ) const
  {
    return( ! (this->operator==( pl )) );
  }

  inline Vec3f & Plane3f::n( void )
  {
    return( m_normal );
  }

  inline const Vec3f & Plane3f::n( void ) const
  {
    return( m_normal );
  }

  inline float & Plane3f::c( void )
  {
    return( m_offset );
  }

  inline const float & Plane3f::c( void ) const
  {
    return( m_offset );
  }

} // end namespace nvmath
