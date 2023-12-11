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

#include  "nvutil/Assert.h"
#include  <float.h>
#include  <math.h>

#include "nvmath/nvmath.h"

namespace nvmath
{
  //! Template class for one dimensional vectors of floats.
  template<unsigned char n> class Vecnf
  {
    public:
      //! Default constructor.
      /** For performance reasons no initialization is performed. */
      Vecnf<n>( void );

      //! Copy constructor
      Vecnf<n>( const Vecnf<n> & v    //!< \c Vecnf to copy
              );

      //! Non-constant subscript operator.
      /** \returns reference to \a i th element of the \c Vecnf  */
      float& operator[]( size_t i           //!<  index to use (i=0,..,n-1) in \c Vecnf
                       );

      //! Constant subscript operator.
      /** \returns constant reference to \a i th element of the Vecnf */
      const float& operator[]( size_t i     //!<  index to use (i=0,..,n-1) in \c Vecnf
                             ) const;

      //! Negation operator.
      /** \returns negated \c Vecnf */
      const Vecnf<n> operator-( void ) const;

      //! \c Vecnf multiplication with a scalar operator.
      /** \returns multiplied \c Vecnf */
      const Vecnf<n> operator*( float  f    //!<  scalar to multiply with
                              ) const;

      //! \c Vecnf multiplication with a double precision scalar operator.
      /** \returns multiplied \c Vecnf */
      const Vecnf<n> operator*( double f    //!<  scalar to multiply with
                              ) const;

      //! \c Vecnf multiplication with a \c Vecnf (i.e. dot product) operator.
      /** \returns dot product of \c this with \a v */
      const float operator*( const Vecnf<n> &v    //!<  \c Vecnf to multiply with
                           ) const;

      //! \c Vecnf division by a scalar operator.
      /** \returns divided \c Vecnf */
      const Vecnf<n> operator/( float f   //!<  scalar to divide through
                              ) const;

      //! \c Vecnf division by a double precision scalar operator.
      /** \returns divided \c Vecnf */
      const Vecnf<n> operator/( double f  //!<  scalar to divide through
                              ) const;

      //! \c Vecnf addition operator.
      /** \returns sum of the \c this and \a v  */
      const Vecnf<n> operator+( const Vecnf<n> &v //!<  \c Vecnf to add
                              ) const;

      //! \c Vecnf subtraction operator.
      /** \returns difference of the two vectors */
      const Vecnf<n> operator-( const Vecnf<n> &v //!<  \c Vecnf to subtract
                              ) const;

      //! \c Vecnf equality operator.
      /** Two \c Vecnf are considered to be equal, if they differ less epsilon in each component.
        * \returns \c true if \c this is equal to \a v, otherwise \c false  */
      bool operator==( const Vecnf<n> &v    //!<  \c Vecnf to compare with
                     ) const;

      //! \c Vecnf inequality operator.
      /** Two \c Vecnf are considered to be equal, if they differ less epsilon in each component.
        * \returns \c true if \c this is not equal to \a v, otherwise \c false  */
      bool operator!=( const Vecnf<n> &v    //!< \c Vecnf to compare with
                     ) const;

      //! \c Vecnf assignment operator.
      /** \returns reference to the assigned \c Vecnf */
      Vecnf<n> & operator=( const Vecnf<n> &v   //!<  \c Vecnf to copy
                          );

      //! \c Vecnf multiplication with a scalar and assignment operator.
      /** \returns reference to the assigned \c Vecnf after multiplication */
      Vecnf<n> & operator*=( float f   //!<  scalar to multiply with
                           );

      //! \c Vecnf multiplication with a double precision scalar and assignment operator.
      /** \returns reference to the assigned \c Vecnf after multiplication */
      Vecnf<n> & operator*=( double f   //!<  scalar to multiply with
                           );

      //! \c Vecnf division by a scalar and assignment operator.
      /** \returns reference to the assigned \c Vecnf after division */
      Vecnf<n> & operator/=( float f   //!<  scalar to divide by
                           );

      //! \c Vecnf division by a double precision scalar and assignment operator.
      /** \returns reference to the assigned \c Vecnf after division */
      Vecnf<n> & operator/=( double f   //!<  scalar to divide by
                           );

      //! \c Vecnf addition and assignment operator.
      /** \returns  reference to the assigned \c Vecnf after addition */
      Vecnf<n> & operator+=( const Vecnf<n> &v    //!<  \c Vecnf to add
                           );

      //! \c Vecnf subtraction and assignment operator.
      /** \returns reference to the assigned \c Vecnf after subtraction  */
      Vecnf<n> & operator-=( const Vecnf<n> &v    //!<  \c Vecnf to subtract
                           );

      //! Get a constant pointer to the elements of the \c Vecnf.
      /** \returns constant pointer to elements of \c this */
      const float* getPtr( void ) const;

      //! Normalize \c this and return the previous length.
      /** \returns length of \c this before normalization */
      float normalize( void );

      //! Orthogonalize \c this with respect to an other \c Vecnf.
      /** Sets \c this to be orthogonal to \a v. */
      void orthogonalize( const Vecnf<n> &v     //!<  \c Vecnf to orthogonalize to
                        );

      //! Orthonormalize \c this with respect to an other \c Vecnf.
      /** Sets \c this to be orthonormal to the normalized \c Vecnf \a v.  */
      void orthonormalize( const Vecnf<n>& v    //!<  \c Vecnf to orthonormalize to
                         );

    private:
      float m_vec[n]; //!< internal Vecnf for storing n floats 
  };

  // - - - - -  - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
  // non-member functions
  // - - - - -  - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
  /*! \relates nvmath::Vecnf
   *  Test if two \c Vecnf are opposite to each other.\n
   *  Two normalized \c Vecnf are considered to be opposite, if their dot product deviates
   *  less than epsilon from -1.
   *  \returns \c true if the \c Vecnf are opposite, otherwise \c false */
   template<unsigned char n> bool areOpposite( const Vecnf<n> &v0
                                             , const Vecnf<n> &v1
                                             , float eps = FLT_EPSILON
                                             )
   {
     NVSG_ASSERT( isNormalized( v0 ) && isNormalized( v1 ) );
     return( ( 1.0f + v0 * v1 ) <= eps );
   }

  /*! \relates nvmath::Vecnf
   *  Test if two \c Vecnf are orthogonal to each other.\n
   *  Two \c Vecnf are considered to be orthogonal, if their dot product deviates
   *  less than epsilon from zero.
   *  \returns \c true if the \c Vecnf are orthogonal, otherwise \c false  */
  template<unsigned char n> bool areOrthogonal( const Vecnf<n> &v0
                                              , const Vecnf<n> &v1
                                              , float eps = FLT_EPSILON //!< optional epsilon
                                              )
  {
    return( abs(v0*v1) <= std::max(1.0f,length(v0)) * std::max(1.0f,length(v1)) * eps );
  }

  /*! \relates nvmath::Vecnf
   *  Test if two \c Vecnf are orthonormal to each other.\n
   *  Two \c Vecnf are orthonormal, if they are both normalized and orthogonal to each other.
   *  \returns \c true if the \c Vecnf are orthonormal, otherwise \c false */
  template<unsigned char n> bool areOrthonormal( const Vecnf<n> &v0
                                               , const Vecnf<n> &v1
                                               , float eps = FLT_EPSILON //!< optional epsilon
                                               )
  {
    return( isNormalized(v0) && isNormalized(v1) && ( abs(v0*v1) <= eps ) );
  }

  /*! \relates nvmath::Vecnf
   *  Test if two \c Vecnf are similar to each other.\n
   *  Two \c Vecnf are considered to be similar, if they differ less than a given tolerance in each component.\n
   *  Without providing the optional parameter \a tol, areSimilar() gives the same result as operator==().
   *  \returns \c true if the \c Vecnf are similar, otherwise \c false */
  template<unsigned char n> bool areSimilar( const Vecnf<n> &v0
                                           , const Vecnf<n> &v1
                                           , float epsilon = FLT_EPSILON //!< optional epsilon
                                           )
  {
    NVSG_ASSERT( 0.0f <= epsilon );
    bool similar = true;
    for ( size_t i=0 ; similar && i<n ; i++ )
    {
      similar = ( abs( v0[i] - v1[i] ) <= epsilon );
    }
    return( similar );
  }

  /*! \relates nvmath::Vecnf
   *  Determine the distance between two \c Vecnf.
   *  If \a p0 and \a p1 are interpreted as two points in n-space, the distance is the euclidean distance in that space.
   *  \returns The distance between \a p0 and \a p1 */
  template<unsigned char n> float distance( const Vecnf<n> &p0
                                          , const Vecnf<n> &p1
                                          )
  {
    return( length( p0 - p1 ) );
  }

  /*! \relates nvmath::Vecnf
   *  Test if a \c Vecnf is normalized.\n
   *  A \c Vecnf is considered to be normalized if its length differs less than epsilon from one.
   *  \returns \c true if the \c Vecnf is normalized, otherwise \c false */
  template<unsigned char n> bool isNormalized( const Vecnf<n> &v            //!< \c Vecnf to test
                                             , float epsilon = FLT_EPSILON  //!< optional epsilon
                                             )
  {
    return( fabs( length( v ) - 1.0f ) <= ( 2 * epsilon ) );
  }

  /*! \relates nvmath::Vecnf
   *  Test if a \c Vecnf is a null \c Vecnf.\n
   *  A \c Vecnf is considered to be a null \c Vecnf if its length differs less than epsilon form zero.
   *  \returns \c true if the \c Vecnf is a null \c Vecnf, otherwise \c false */
  template<unsigned char n> bool isNull( const Vecnf<n> &v            //!< \c Vecnf to test
                                       , float epsilon = FLT_EPSILON  //!< optional epsilon
                                       )
  {
    return( length( v ) <= epsilon );
  }

  /*! \relates nvmath::Vecnf
   *  Calculate the length of a \c Vecnf.
   *  If \a v is interpreted as a vector in n-space, the length is the euclidean length in that space.
   *  \returns length of \a v */
  template<unsigned char n> float length( const Vecnf<n> &v     //!<  \c Vecnf to get length of
                                        )
  {
    return( sqrtf( lengthSquared( v ) ) );
  }

  /*! \relates nvmath::Vecnf
   *  Calculate the length of a \c Vecnf with double precision.
   *  If \a v is interpreted as a vector in n-space, the length is the euclidean length in that space.
   *  \returns length of \a v */
  template<unsigned char n> double lengthd( const Vecnf<n> &v   //!<  \c Vecnf to get length of
                                          )
  {
    double l = square( v[0] );
    for ( size_t i=1 ; i<n ; i++ )
    {
      l += square( v[i] );
    }
    return( sqrt( l ) );
  }

  /*! \relates nvmath::Vecnf
   *  Calculate the squared length of a \c Vecnf.
   *  \returns squared length of \a v */
  template<unsigned char n> float lengthSquared( const Vecnf<n> &v  //!<  \c Vecnf to get squared length of
                                               )
  {
    return( v * v );
  }

  /*! \relates nvmath::Vecnf
   *  Get the maximal absolute value of a \ Vecnf.
   *  \returns maximal element of \a v */
  template<unsigned char n> float maxElement( const Vecnf<n> &v     //!<  Vecnf to get maximal element of
                                            )
  {
    float f = fabsf( v[0] );
    for ( size_t i=1 ; i<n ; i++ )
    {
      float t = fabsf( v[i] );
      if ( f < t )
      {
        f = t;
      }
    }
    return( f );
  }

  /*! \relates nvmath::Vecnf
    *  Get the minimal absolute value of a \c Vecnf.
    *  \returns minimal element of \a v */
  template<unsigned char n> float minElement( const Vecnf<n> &v     //!<  Vecnf to get minimal element of
                                            )
  {
    float f = fabsf( v[0] );
    for ( size_t i=1 ; i<n ; i++ )
    {
      float t = fabsf( v[i] );
      if ( t < f )
      {
        f = t;
      }
    }
    return( f );
  }

  /*! \relates nvmath::Vecnf
   *  Multiplication of a scalar with a \c Vecnf.
   *  \returns product of \a f with \a v */
  template<unsigned char n> Vecnf<n>  operator*( float f            //!<  scalar to multiply with
                                               , const Vecnf<n> &v  //!<  Vecnf to multiply
                                               )
  {
    return( v * f );
  }

  /*! \relates nvmath::Vecnf
  *  Multiplication of a double precision scalar with a \c Vecnf.
  *  \returns product of \a f with \a v */
  template<unsigned char n> Vecnf<n>  operator*( double f           //!<  scalar to multiply with
                                               , const Vecnf<n> &v  //!<  Vecnf to multiply
                                               )
  {
    return( v * f );
  }

  template<unsigned char n> Vecnf<n> orthonormalize( const Vecnf<n> &v0
                                                   , const Vecnf<n> &v1 )
  {
    NVSG_ASSERT( isNormalized( v0 ) && isNormalized( v1 ) && ! areCollinear( v0, v1 ) );
    //  determine the orthogonal projection of v1 on v0: ( v0 * v1 ) * v0
    //  and subtract it from v1 resulting in the orthogonalized version of v1
    return( normalize( v1 - ( v0 * v1 ) * v0 ) );
  }

  // - - - - -  - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
  // "inlines"
  // - - - - -  - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
  template<unsigned char n> Vecnf<n>::Vecnf( void )
  {
  }

  template<unsigned char n> Vecnf<n>::Vecnf( const Vecnf<n> &v )
  {
    for ( size_t i=0 ; i<n ; i++ )
    {
      m_vec[i] = v[i];
    }
  }

  template<unsigned char n> float& Vecnf<n>::operator[]( size_t i ) 
  { 
    NVSG_ASSERT( 0 <= i && i < n );
    return( m_vec[i] );
  }

  template<unsigned char n> const float& Vecnf<n>::operator[]( size_t i ) const 
  { 
    NVSG_ASSERT( 0 <= i && i < n );
    return( m_vec[i] );
  }

  template<unsigned char n> const Vecnf<n> Vecnf<n>::operator-( void ) const
  {
    Vecnf<n> v;
    for ( size_t i=0 ; i<n ; i++ )
    {
      v[i] = -m_vec[i];
    }
    return( v );
  }

  template<unsigned char n> const Vecnf<n> Vecnf<n>::operator*( float f ) const
  {
    Vecnf<n> ret( *this );
    return( ret *= f );
  }

  template<unsigned char n> const Vecnf<n> Vecnf<n>::operator*( double f ) const
  {
    Vecnf<n> ret( *this );
    return( ret *= f );
  }

  template<unsigned char n> const float Vecnf<n>::operator*( const Vecnf<n> &v ) const
  {
    double f(0);
    for ( size_t i=0 ; i<n ; i++ )
    {
      f += m_vec[i] * v[i];
    }
    return( (float)f );
  }

  template<unsigned char n> const Vecnf<n> Vecnf<n>::operator/( float f ) const
  {
    NVSG_ASSERT( FLT_EPSILON < fabs( f ) );
    Vecnf<n> ret( *this );
    return( ret /= f );
  }

  template<unsigned char n> const Vecnf<n> Vecnf<n>::operator/( double f ) const
  {
    NVSG_ASSERT( FLT_EPSILON < fabs( f ) );
    Vecnf<n> ret( *this );
    return( ret /= f );
  }

  template<unsigned char n> const Vecnf<n> Vecnf<n>::operator+( const Vecnf<n> &v ) const
  {
    Vecnf<n> ret( *this );
    return( ret += v );
  }

  template<unsigned char n> const Vecnf<n> Vecnf<n>::operator-( const Vecnf<n> &v ) const
  {
    Vecnf<n> ret( *this );
    return( ret -= v );
  }

  template<unsigned char n> bool Vecnf<n>::operator==( const Vecnf<n> &v ) const
  {
    bool equal = true;
    for ( unsigned char i=0 ; equal && i<n ; i++ )
    {
      equal = ( fabs( m_vec[i] - v[i] ) < FLT_EPSILON );
    }
    return( equal );
  }

  template<unsigned char n> bool Vecnf<n>::operator!=( const Vecnf<n> &v ) const
  {
    return( ! ( *this == v ) );
  }

  template<unsigned char n> Vecnf<n> & Vecnf<n>::operator=( const Vecnf<n> &v )
  {
    for ( size_t i=0 ; i<n ; i++ )
    {
      m_vec[i] = v[i];
    }
    return( *this );
  }

  template<unsigned char n> Vecnf<n> & Vecnf<n>::operator*=( float f )
  {
    for ( size_t i=0 ; i<n ; i++ )
    {
      m_vec[i] *= f;
    }
    return( *this );
  }

  template<unsigned char n> Vecnf<n> & Vecnf<n>::operator*=( double f )
  {
    for ( size_t i=0 ; i<n ; i++ )
    {
      m_vec[i] *= f;
    }
    return( *this );
  }

  template<unsigned char n> Vecnf<n> & Vecnf<n>::operator/=( float f )
  {
    NVSG_ASSERT( FLT_EPSILON < fabs( f ) );
    float invF = 1.0f / f;
    for ( size_t i=0 ; i<n ; i++ )
    {
      m_vec[i] *= invF;
    }
    return( *this );
  }

  template<unsigned char n> Vecnf<n> & Vecnf<n>::operator/=( double f )
  {
    NVSG_ASSERT( FLT_EPSILON < fabs( f ) );
    double invF = 1.0 / f;
    for ( size_t i=0 ; i<n ; i++ )
    {
      m_vec[i] = (float)(m_vec[i] * invF);
    }
    return( *this );
  }

  template<unsigned char n> Vecnf<n> & Vecnf<n>::operator+=( const Vecnf<n> &v )
  {
    for ( size_t i=0 ; i<n ; i++ )
    {
      m_vec[i] += v[i];
    }
    return( *this );
  }

  template<unsigned char n> Vecnf<n> & Vecnf<n>::operator-=( const Vecnf<n> &v )
  {
    for ( size_t i=0 ; i<n ; i++ )
    {
      m_vec[i] -= v[i];
    }
    return( *this );
  }

  template<unsigned char n> const float* Vecnf<n>::getPtr( void ) const
  {
    return( &m_vec[0] );
  }

  template<unsigned char n> float Vecnf<n>::normalize( void )
  {
    double norm = lengthd( *this );
    if ( FLT_EPSILON < norm )
    {
      *this /= norm;
      NVSG_ASSERT( isNormalized( *this ) );
    }
    return( (float)norm );
  }

  template<unsigned char n> void Vecnf<n>::orthonormalize( const Vecnf<n> &v )
  {
    NVSG_ASSERT( isNormalized( *this ) && isNormalized( v ) && !areCollinear( *this, v ) );
    //  determine the orthogonal projection of this on v : ( v * this ) * v
    //  and subtract it from this resulting in the orthogonalized this
    *this = *this - ( v * *this ) * v;
    normalize();    //  and normalize it
    //  don't assert the general case, because this orthonormalization is far from exact
    //  NVSG_ASSERT( areOrthonormal( *this, v0 ) );
  }

}
