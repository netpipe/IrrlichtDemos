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
#include "nvmath/Vecnf.h"

namespace nvmath
{
  //! Template for two dimensional matrices of floats.
  /** A \c Matnnf is organized as n \c Vecnf. */
  template<unsigned char n> class Matnnf
  {
    public:
      //! Default constructor.
      /** For performance reasons no initialization is performed. */
      Matnnf<n>( void );

      //! Calculate the inverse of a matrix.
      /** \note If the matrix is not invertible, the resulting matrix is undefined.
        * \returns \c true, if the matrix was invertible, otherwise \c false */
      bool  invert( void );

      //! Non-constant subscript operator.
      /** \returns reference to \a i th row of the matrix  */
      Vecnf<n> & operator[]( size_t i   //!<  index to row to use (i=0,..,n-1)
                           );

      //! Constant subscript operator.
      /** \returns constant reference to \a i th row of the matrix */
      const Vecnf<n> & operator[]( size_t i   //!<  index to row to use (i=0,..,n-1)
                                 ) const;

      //! Transpose operator.
      /** \returns transposed matrix */
      const Matnnf<n> operator~( void ) const;

      //! Negation operator.
      /** \returns negated matrix  */
      const Matnnf<n> operator-( void ) const;

      //! Matrix multiplication with a scalar operator.
      /** \returns multiplied matrix */
      const Matnnf<n> operator*( float f                //!<  scalar to multiply with
                               ) const;

      //! Matrix multiplication with a vector operator.
      /** \returns matrix times \a v */
      const Vecnf<n> operator*( const Vecnf<n>& v       //!<  vector to multiply with
                              ) const;

      //! Matrix division by a scalar operator.
      /** \returns divided matrix */
      const Matnnf<n> operator/( float f                //!<  scalar to divide through
                               ) const;

      //! Matrix addition operator.
      /** \returns sum of the two matrices  */
      const Matnnf<n> operator+( const Matnnf<n> &v     //!<  matrix to add
                               ) const;

      //! Matrix subtraction operator.
      /** \returns difference of the two matrices  */
      const Matnnf<n> operator-( const Matnnf<n> &v     //!<  matrix to add
                               ) const;

      //! Equality operator.
      /** \returns \c true if the matrix is equal to \a m, otherwise \c false */
      bool operator==( const Matnnf<n> &m         //!<  matrix to compare with
                     ) const;

      //! Inequality operator.
      /** \returns \c true if the matrix is not equal to \a m, otherwise \c false  */
      bool operator!=( const Matnnf<n> &m         //!< matrix to compare with
                     ) const;

      //! Assignment operator.
      /** \returns reference to the assigned matrix */
      Matnnf<n> & operator=( const Matnnf<n> &m   //!<  matrix to copy
                           );

      //! Matrix multiplication with a scalar and assignment operator.
      /** \returns reference to the assigned matrix after multiplication */
      Matnnf<n> & operator*=( float f    //!<  scalar to multiply with
                            );

      //! Matrix multiplication with a matrix (i.e. concatenation) and assignment operator.
      /** \returns reference to the assigned matrix after multiplication (concatenation) */
      Matnnf<n> & operator*=( const Matnnf<n> &m    //!<  matrix to multiply
                           );

      //! Matrix division by a scalar and assignment operator.
      /** \returns reference to the assigned matrix after division */
      Matnnf<n> & operator/=( float f    //!<  scalar to divide through
                            );

      //! Matrix addition and assignment operator.
      /** \returns  reference to the assigned matrix after addition */
      Matnnf<n> & operator+=( const Matnnf<n> &m    //!<  matrix to add
                           );

      //! Matrix subtraction and assignment operator.
      /** \returns  reference to the assigned matrix after addition */
      Matnnf<n> & operator-=( const Matnnf<n> &m    //!<  matrix to subtract
                           );

      //! Get a constant pointer to the n times n floats of the matrix.
      /** \returns constant pointer to matrix elements */
      const float* getPtr( void ) const;

    private:
      Vecnf<n>  m_mat[n];
  };

  // - - - - -  - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
  // non-member functions
  // - - - - -  - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

  /*! \relates nvmath::Matnnf
   *  Calculate the inverse of a matrix.
   *  \param m A constant reference to an invertible matrix.
   *  \return The inverse matrix of \a m.
   *  \remarks If \a m is not invertible, the result is undefined. */
  template<unsigned char n> Matnnf<n> inverse( const Matnnf<n> & m )
  {
    Matnnf<n> ret( m );
    bool ok = ret.invert();
    NVSG_ASSERT( ok );
    return( ret );
  }

  /*! \relates nvmath::Matnnf
   *  Test if a \c Matnnf is the identity. \n
   *  A \c Matnnf is considered to be the identity, if each of the diagonal elements differ less
   *  than epsilon from one, and each of the other matrix elements differ less than epsilon from zero.
   *  \returns \c true if the matrix is the identity, otherwise \c false */
  template<unsigned char n> bool isIdentity( const Matnnf<n> &m           //!<  matrix to test
                                           , float epsilon = FLT_EPSILON  //!< optional epsilon
                                           )
  {
    bool identity = true;
    for ( size_t i=0 ; identity && i<n ; i++ )
    {
      for ( size_t j=0 ; identity && j<i ; j++ )
      {
        identity = ( fabsf( m[i][j] ) <= epsilon );
      }
      if ( identity )
      {
        identity = ( fabsf( m[i][i] - 1.0f ) <= epsilon );
      }
      for ( size_t j=i+1 ; identity && j<n ; j++ )
      {
        identity = ( fabsf( m[i][j] ) <= epsilon );
      }
    }
    return( identity );
  }

  /*! \relates nvmath::Matnnf
   *  Test if a \c Matnnf is normalized. \n
   *  A \c Matnnf is normalized when all rows and all columns are normalized.
   * \returns \c true if the matrix is normalized, otherwise \c false */
  template<unsigned char n> bool isNormalized( const Matnnf<n> &mat         //!< matrix to test
                                             , float epsilon = FLT_EPSILON  //!< optional epsilon
                                             )
  {
    bool normalized = true;
    for ( size_t i=0 ; normalized && i<n ; i++ )
    {
      normalized = isNormalized( mat[i], epsilon );
    }
    for ( size_t i=0 ; normalized && i<n ; i++ )
    {
      Vecnf<n> v;
      for ( size_t j=0 ; j<n ; j++ )
      {
        v[j] = mat[j][i];
      }
      normalized = isNormalized( v, epsilon );
    }
    return( normalized );
  }

  /*! \relates nvmath::Matnnf
   *  Test if a \c Matnnf is a null matrix. \n
   *  A \c Matnnf is considered to be a null matrix when each of it's rows is a null vector.
   *  \returns \c true if the matrix is a null matrix, otherwise \c false */
  template<unsigned char n> bool isNull( const Matnnf<n> &mat     //!< matrix to test
                                       , float eps = FLT_EPSILON  //!< optional epsilon
                                       )
  {
    bool null = true;
    for ( size_t i=0 ; null && i<n ; i++ )
    {
      null = isNull( mat[i], 5*eps );
    }
    return( null );
  }

  /*! \relates nvmath::Matnnf
   *  Test if a \c Matnnf is orthogonal. \n
   *  A \c Matnnf is orthogonal when every row is orthogonal to each other row and every column
   *  is orthogonal to each other column.
   *  \returns \c true if the matrix is orthogonal, otherwise \c false */
  template<unsigned char n> bool isOrthogonal( const Matnnf<n> &mat         //!< matrix to test
                                             , float epsilon = FLT_EPSILON  //!< optional epsilon
                                             )
  {
    bool orthogonal = true;
    for ( size_t i=0 ; orthogonal && i<n-1 ; i++ )
    {
      for ( size_t j=i+1 ; orthogonal && j<n ; j++ )
      {
        orthogonal = areOrthogonal( mat[i], mat[j], epsilon );
      }
    }
    if ( orthogonal )
    {
      Matnnf<n> tmp = ~mat;
      for ( size_t i=0 ; orthogonal && i<n-1 ; i++ )
      {
        for ( size_t j=i+1 ; orthogonal && j<n ; j++ )
        {
          orthogonal = areOrthogonal( tmp[i], tmp[j], epsilon );
        }
      }
    }
    return( orthogonal );
  }

  /*! \brief Get the value of the maximal absolute element of a matrix.
   *  \param m A constant reference to a matrix to get the maximal element from.
   *  \return The value of the maximal absolute element of \a m.
   *  \sa minElement */
  template<unsigned char n>
  float maxElement( const Matnnf<n> & m )
  {
    float me = maxElement( m[0] );
    for ( unsigned char i=1 ; i<n ; ++i )
    {
      float t = maxElement( m[i] );
      if ( me < t )
      {
        me = t;
      }
    }
    return( me );
  }

  /*! \brief Get the value of the minimal absolute element of a matrix.
   *  \param m A constant reference to a matrix to get the minimal element from.
   *  \return The value of the minimal absolute element of \a m.
   *  \sa maxElement */
  template<unsigned char n>
  float minElement( const Matnnf<n> & m )
  {
    float me = minElement( m[0] );
    for ( unsigned char i=1 ; i<n ; ++i )
    {
      float t = minElement( m[i] );
      if ( t < me )
      {
        me = t;
      }
    }
    return( me );
  }

  /*! \relates nvmath::Matnnf
   *  Matrix multiplication with a scalar.
   *  \return multiplied matrix */
  template<unsigned char n> const Matnnf<n> operator*( float f, const Matnnf<n> &m )
  {
    return( m * f );
  }

  /*! \relates nvmath::Matnnf
   *  Multiplication of a Vecnf<n> with a Matnnf<n>.
   *  \return \a v times \a m */
  template<unsigned char n> Vecnf<n> operator*( const Vecnf<n> & v, const Matnnf<n> & m )
  {
    Vecnf<n>  ret;
    for ( size_t i=0 ; i<n ; i++ )
    {
      ret[i] = v[0] * m[0][i];
      for ( size_t j=1 ; j<n ; j++ )
      {
        ret[i] += v[j] * m[j][i];
      }
    }
    return( ret );
  }

  /*! \relates nvmath::Matnnf
   *  Matrix multiplication with a matrix (i.e. concatenation).
   *  \returns multiplied (concatenated) matrix  */
  template<unsigned char n> const Matnnf<n> operator*( const Matnnf<n> &m0, const Matnnf<n> &m1 )
  {
    Matnnf<n> ret;
    for ( size_t i=0 ; i<n ; i++ )
    {
      for ( size_t j=0 ; j<n ; j++ )
      {
        double sum = 0.0;
        for ( size_t k=0 ; k<n ; k++ )
        {
          sum += m0[i][k] * m1[k][j];
        }
        ret[i][j] = (float)sum;
      }
    }
    return( ret );
  }

  /*! \relates nvmath::Matnnf
   *  Set Matrix to identity. */
  template<unsigned char n> void setIdentity( Matnnf<n> &m )
  {
    for ( size_t i=0 ; i<n ; i++ )
    {
      for ( size_t j=0 ; j<n ; j++ )
      {
        m[i][j] = 0.0f;
      }
      m[i][i] = 1.0f;
    }
  }

  // - - - - -  - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
  // "inlines"
  // - - - - -  - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
  template<unsigned char n> Matnnf<n>::Matnnf( void )
  {
  }

  template<unsigned char n> bool Matnnf<n>::invert( void )
  {
    size_t  p[n];

    bool ok = true;
    for ( size_t k=0 ; k<n && ok ; k++ )
    {
      float max = 0.0f;
      p[k] = 0;
      for ( size_t i=k ; i<n && ok ; i++ )
      {
        float s = 0.0f;
        for ( size_t j=k ; j<n ; j++ )
        {
          s += fabsf( m_mat[i][j] );
        }
        ok = ( FLT_EPSILON < fabsf(s) );
        if ( ok )
        {
          float q = fabsf( m_mat[i][k] ) / s;
          if ( q > max )
          {
            max = q;
            p[k] = i;
          }
        }
      }

      ok = ( FLT_EPSILON < max );
      if ( ok )
      {
        if ( p[k] != k )
        {
          for ( size_t j=0 ; j<n ; j++ )
          {
            std::swap( m_mat[k][j], m_mat[p[k]][j] );
          }
        }

        float pivot = m_mat[k][k];
        ok = ( FLT_EPSILON < fabsf( pivot ) );
        if ( ok )
        {
          for ( size_t j=0 ; j<n ; j++ )
          {
            if ( j != k )
            {
              m_mat[k][j] /= - pivot;
              for ( size_t i=0 ; i<n ; i++ )
              {
                if ( i != k )
                {
                  m_mat[i][j] += m_mat[i][k] * m_mat[k][j];
                }
              }
            }
          }

          for ( size_t i=0 ; i<n ; i++ )
          {
            m_mat[i][k] /= pivot;
          }
          m_mat[k][k] = 1.0f / pivot;
        }
      }
    }

    if ( ok )
    {
      for ( size_t k=n-2 ; k<n ; k-- )  //  NOTE: ( size_t k < n ) <=> ( int k >= 0 )
      {
        if ( p[k] != k )
        {
          for ( size_t i=0 ; i<n ; i++ )
          {
            std::swap( m_mat[i][k], m_mat[i][p[k]] );
          }
        }
      }
    }

    return( ok );
  }

  template<unsigned char n> Vecnf<n> & Matnnf<n>::operator[]( size_t i ) 
  { 
    NVSG_ASSERT( 0 <= i && i < n );
    return( m_mat[i] );
  }

  template<unsigned char n> const Vecnf<n> & Matnnf<n>::operator[]( size_t i ) const
  { 
    NVSG_ASSERT( 0 <= i && i < n );
    return( m_mat[i] );
  }

  template<unsigned char n> const Matnnf<n> Matnnf<n>::operator~( void ) const
  {
    Matnnf<n> ret;
    for ( size_t i=0 ; i<n ; i++ )
    {
      for ( size_t j=0 ; j<n ; j++ )
      {
        ret[i][j] = m_mat[j][i];
      }
    }
    return( ret );
  }

  template<unsigned char n> const Matnnf<n> Matnnf<n>::operator-( void ) const
  {
    Matnnf<n> ret;
    for ( size_t i=0 ; i<n ; i++ )
    {
      ret[i] = - m_mat[i];
    }
    return( ret );
  }

  template<unsigned char n> const Matnnf<n> Matnnf<n>::operator*( float f ) const
  {
    Matnnf<n> ret( *this );
    return( ret *= f );
  }

  template<unsigned char n> const Vecnf<n> Matnnf<n>::operator*( const Vecnf<n> &v ) const
  {
    Vecnf<n>  ret;
    for ( size_t i=0 ; i<n ; i++ )
    {
      ret[i] = m_mat[i] * v;
    }
    return( ret );
  }

  template<unsigned char n> const Matnnf<n> Matnnf<n>::operator/( float f ) const
  {
    NVSG_ASSERT( FLT_EPSILON < fabs( f ) );
    Matnnf<n> ret( *this );
    return( ret /= f );
  }

  template<unsigned char n> const Matnnf<n> Matnnf<n>::operator+( const Matnnf<n> &m ) const
  {
    Matnnf<n> ret( *this );
    return( ret += m );
  }

  template<unsigned char n> const Matnnf<n> Matnnf<n>::operator-( const Matnnf<n> &m ) const
  {
    Matnnf<n> ret( *this );
    return( ret -= m );
  }

  template<unsigned char n> bool Matnnf<n>::operator==( const Matnnf<n> &m ) const
  {
    bool equal = true;
    for ( unsigned char i=0 ; equal && i<n ; i++ )
    {
      equal = ( m_mat[i] == m[i] );
    }
    return( equal );
  }

  template<unsigned char n> bool Matnnf<n>::operator!=( const Matnnf<n> &m ) const
  {
    return( ! ( *this == m ) );
  }

  template<unsigned char n> Matnnf<n> & Matnnf<n>::operator=( const Matnnf<n> &m )
  {
    for ( size_t i=0 ; i<n ; i++ )
    {
      m_mat[i] = m[i];
    }
    return( *this );
  }

  template<unsigned char n> Matnnf<n> & Matnnf<n>::operator*=( float f )
  {
    for ( size_t i=0 ; i<n ; i++ )
    {
      m_mat[i] *= f;
    }
    return( *this );
  }

  template<unsigned char n> Matnnf<n> & Matnnf<n>::operator*=( const Matnnf<n> &m )
  {
    *this = *this * m;
    return( *this );
  }

  template<unsigned char n> Matnnf<n> & Matnnf<n>::operator/=( float f )
  {
    NVSG_ASSERT( FLT_EPSILON < fabs( f ) );
    for ( size_t i=0 ; i<n ; i++ )
    {
      m_mat[i] /= f;
    }
    return( *this );
  }

  template<unsigned char n> Matnnf<n> & Matnnf<n>::operator+=( const Matnnf<n> &m )
  {
    for ( size_t i=0 ; i<n ; i++ )
    {
      m_mat[i] += m[i];
    }
    return( *this );
  }

  template<unsigned char n> Matnnf<n> & Matnnf<n>::operator-=( const Matnnf<n> &m )
  {
    for ( size_t i=0 ; i<n ; i++ )
    {
      m_mat[i] -= m[i];
    }
    return( *this );
  }

  template<unsigned char n> const float * Matnnf<n>::getPtr( void ) const
  {
    return( m_mat[0].getPtr() );
  }

} // end namespace nvmath
