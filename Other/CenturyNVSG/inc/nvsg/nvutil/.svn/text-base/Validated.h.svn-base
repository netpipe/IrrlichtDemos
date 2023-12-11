// Copyright NVIDIA Corporation 2002-2004
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

namespace nvutil
{
  //! The Validated class adds a validation state to some data.
  template<typename T> class Validated
  {
    public:
      //! Default constructor
      Validated( void );

      //! Constructor with default value
      Validated( const T & t );

    public:
      //! Ask if the validated data currently is valid.
      /** \return \c true, if the data is valid, otherwise \c false. */
      bool isValid( void ) const;

      //! Invalidate the data.
      void invalidate( void );

      //! Accept the current value as valid.
      void validate( void );

      //! Get the data.
      /** It is assumed that the data is valid. If it's invalid, the returned data is not defined.
        * \return the current data */
      const T & get( void ) const;

      //! Set the data.
      /** Setting the data also validates it. */
      void set( const T & t );

    private:
      T     m_data;
      bool  m_valid;
  };

  template<typename T>
    inline Validated<T>::Validated( void )
    : m_valid(false)
  {
  }

  template<typename T>
    inline Validated<T>::Validated( const T & t )
  {
    set( t );
  }

  template<typename T>
    inline bool Validated<T>::isValid( void ) const
  {
    return( m_valid );
  }

  template<typename T>
    inline void Validated<T>::invalidate( void )
  {
    m_valid = false;
  }

  template<typename T>
    inline void Validated<T>::validate( void )
  {
    m_valid = true;
  }

  template<typename T>
    inline const T & Validated<T>::get( void ) const
  {
    NVSG_ASSERT( m_valid );
    return( m_data );
  }

  template<typename T>
    inline void Validated<T>::set( const T & t )
  {
    m_data = t;
    validate();
  }
}