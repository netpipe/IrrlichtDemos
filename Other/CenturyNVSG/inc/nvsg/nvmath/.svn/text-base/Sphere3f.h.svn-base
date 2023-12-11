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

#if !defined(NDEBUG)
# define NVSG_TRACE_SPHERE3F(s) \
  NVSG_TRACE_OUT_F(( \
    #s " sphere output:\n" \
    "sphere center: \t%f\t%f\t%f\n" \
    "sphere radius: \t%f%\n" \
    , s.getCenter()[0], s.getCenter()[1], s.getCenter()[2], s.getRadius() )) 
#else
# define NVSG_TRACE_SPHERE3F(v)
#endif

namespace nvmath
{
  //! Sphere class
  /** This is a sphere in 3-space, determined by a center point and a radius.\n
    * A \c Sphere3f is often used as a bounding sphere around some data, so there
    * is a variety of routines to calculate the bounding sphere. */
  class Sphere3f
  {
    public:
      //! Default constructor.
      /** A default sphere is initialized to be invalid.\n
        * A \c Sphere3f is considered to be invalid if the radius is negative. */
      NVSG_API Sphere3f( void );

      //! Constructor by center and radius
      NVSG_API Sphere3f( const Vec3f &center  //!<  center
                       , float radius         //!<  radius
                       );

      //! Assignment operator
      /** \returns reference to the assigned sphere */
      NVSG_API Sphere3f & operator=( const Sphere3f &s //!<  sphere to assign
                                   );
      
      //! Equality operator.
      /** Two \c Sphere3f are considered to be equal, if the center points are equal
        * and the radii differ by less than epsilon.
        * \returns \c true if the sphere is equal to \a s, otherwise \c false  */
      NVSG_API bool operator==( const Sphere3f& s      //!<  sphere to compare with
                              ) const;

      //! Inequality operator.
      /** Two \c Sphere3f are considered to be equal, if the center points are equal
        * and the radii differ by less than epsilon.
        * \returns \c true if the sphere is not equal to \a s, otherwise \c false  */
      NVSG_API bool operator!=( const Sphere3f& s      //!<  sphere to compare with
                              ) const;

      //! Calling operator
      /** Calculates the distance of a point to the sphere. When the point is inside the sphere, the distance is
        * negative; when the point is outside, the distance is positive.
        * \returns the signed distance of \a p to the sphere. */
      NVSG_API float operator()( const Vec3f &p             //!<  point to determine distance from sphere
                               ) const;

      //! Get the center of the sphere.
      /** \returns the center of the sphere. */
      NVSG_API const Vec3f & getCenter( void ) const;

      //! Set the center of the sphere.
      NVSG_API void setCenter( const Vec3f &center    //!<  point to set as center
                             );

      //! Get the radius of the sphere.
      /** \returns the radius of the sphere. */
      NVSG_API float getRadius( void ) const;

      //! Set the radius of the sphere.
      NVSG_API void setRadius( float radius           //!<  radius to set
                             );

    private:
      Vec3f m_center;
      float m_radius;
  };

  // - - - - -  - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
  // non-member functions and inlines
  // - - - - -  - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
  /*! \relates nvmath::Sphere3f
   *  Determine a bounding sphere of a number of points
   *  \note The determined sphere is not necessarily the smallest possible.
   *  \returns A reasonable small sphere around the given points */
  NVSG_API Sphere3f boundingSphere( const Vec3f *points      //!<  points to to include
                                  , size_t numberOfPoints    //!<  number of points
                                  );

  /*! \relates nvmath::Sphere3f
   *  Determine the bounding sphere of a number of indexed points.
   *  \note The determined sphere is not necessarily the smallest possible.
   *  \returns A reasonable small sphere around the given points */
  NVSG_API Sphere3f boundingSphere( const Vec3f *points            //!< points to use
                                  , const unsigned int * indices   //!< indices to use in points
                                  , size_t numberOfIndices         //!< number of indices
                                  );

  /*! \relates nvmath::Sphere3f
  *  Determine the bounding sphere of a number of strip-indexed points.
  *  \note The determined sphere is not necessarily the smallest possible.
  *  \returns A reasonable small sphere around the given points */
  NVSG_API Sphere3f boundingSphere( const Vec3f *points                      //!< points to use
                                  , const std::vector<unsigned int> *strips  //!< strips of indices to use in points
                                  , size_t numberOfStrips                    //!< number of strips
                                  );

  /*! \relates nvmath::Sphere3f
   *  Determine the bounding sphere around a sphere \a s and a point \a p.
   *  \returns The bounding sphere around \a s and \a p. */
  NVSG_API Sphere3f boundingSphere( const Sphere3f &s
                                  , const Vec3f &p
                                  );

  /*! \relates nvmath::Sphere3f
   *  Determine the bounding sphere around two given spheres.
   *  \returns the smallest sphere that includes both given spheres \a s0 and \a s1. */
  NVSG_API Sphere3f boundingSphere( const Sphere3f &s0
                                  , const Sphere3f &s1
                                  );

  /*! \relates nvmath::Sphere3f
   *  Determine the bounding sphere of the intersection of two given spheres.
   *  \returns the smallest sphere that includes the intersection of the two spheres \a s0 and \a s1. */
  NVSG_API Sphere3f intersectingSphere( const Sphere3f &s0
                                      , const Sphere3f &s1
                                      );

  /*! \relates nvmath::Sphere3f 
   *  Invalidate a \c Sphere3f.\n
   *  A \c Sphere3f is invalidated by setting it's radius to minus one. */
  inline void invalidate( Sphere3f & s  //!< sphere to invalidate
                        )
  {
    s.setRadius( -1.0f );
  }

  /*! \relates nvmath::Sphere3f 
   *  Check if the given sphere is an empty sphere. \n
   *  A sphere is considered to be empty if the radius is not negative. */
  inline bool isEmpty( const Sphere3f & s //!< sphere to test
                     )
  {
    return( s.getRadius() < 0.0f );
  }

  // - - - - -  - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
  // misc
  // - - - - -  - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
  
  /*! \relates nvmath::Sphere3f 
   * Global unit sphere.*/
  extern NVSG_API const Sphere3f cUnitSphere;


  // - - - - -  - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
  // inlines Sphere3f
  // - - - - -  - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
  inline Sphere3f & Sphere3f::operator=( const Sphere3f &s )
  {
    m_center = s.m_center;
    m_radius = s.m_radius;
    return( *this );
  }

  inline bool Sphere3f::operator==( const Sphere3f &s ) const
  {
    return( ( m_center == s.m_center ) && ( fabsf( m_radius - s.m_radius ) < FLT_EPSILON ) );
  }

  inline bool Sphere3f::operator!=( const Sphere3f &s ) const
  {
    return( ! (this->operator==( s )) );
  }

  inline float Sphere3f::operator()( const Vec3f &p ) const
  {
    return( distance( m_center, p ) - m_radius );
  }

  inline const Vec3f & Sphere3f::getCenter( void ) const
  {
    return( m_center );
  }

  inline void Sphere3f::setCenter( const Vec3f &center )
  {
    m_center = center;
  }

  inline float Sphere3f::getRadius( void ) const
  {
    return( m_radius );
  }

  inline void Sphere3f::setRadius( float radius )
  {
    m_radius = radius;
  }
  
} // end namespace nvmath
