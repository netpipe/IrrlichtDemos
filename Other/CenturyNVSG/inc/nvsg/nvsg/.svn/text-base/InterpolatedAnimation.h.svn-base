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

#include  "nvsg/Animation.h"

namespace nvsg
{
  /*! \brief Abstract base class for an interpolated Animation of type \c T.
   *  \par Namespace: nvsg
   *  \remarks An InterpolatedAnimation holds the information for a key-frame based animation. For
   *  each key, it holds the frame corresponding to that key, and a value of type \c T that is to
   *  be interpolated between the frames of succesive keys. It provides the interface needed for an
   *  interpolated animation, but does not perform any interpolations. Interpolations must be
   *  performed by a derived class such as LinearInterpolatedAnimation.
   *  \sa Animation, LinearInterpolatedAnimation */
  template<typename T> class InterpolatedAnimation : public Animation<T>
  {
    public:
      /*! \brief Destructs a InterpolatedAnimation. */
      virtual ~InterpolatedAnimation( void );

      /*! \brief Get the number of animation frames.
       *  \return The number of frames in this InterpolatedAnimation
       *  \remarks The number of frames is the frame of the last key, plus one
       *  \sa getNumberOfKeys */
      virtual size_t getNumberOfFrames( void ) const;

      /*! \brief Get the number of keys in this InterpolatedAnimation.
       *  \return The number of keys
       *  \remarks For each key, there is the frame of this key, and the value of type \c T that is
       *  to be interpolated between the frames of succesive keys.
       *  \sa getNumberOfFrames, setNumberOfKeys, setKeyFrame */
      size_t getNumberOfKeys( void ) const;

      /*! \brief Set the number of keys in this InterpolatedAnimation.
       *  \param count The number of keys
       *  \remarks Resizes the arrays of frames and values. On growing these arrays, the new
       *  elements are not initialized.
       *  \sa getNumberOfKeys, setKeyFrame */
      void setNumberOfKeys( size_t count );

      /*! \brief Set the frame for a key.
       *  \param key The index of the key to set
       *  \param frame The index of the frame corresponding to \a key
       *  \param value The value of type \c T of the frame corresponding to \a key
       *  \note The behavior is undefined if \a key is greater than or equal to the number of
       *  keys.
       *  \sa getNumberOfKeys, setNumberOfKeys */
      void setKeyFrame( size_t key
                      , unsigned int frame
                      , const T & value );

      /*! \brief Get the frame index corresponding to the key \a key.
       *  \param key  The key index from which to get the corresponding frame 
       *  \return The frame index corresponding to the key \a key
       *  \note The behavior is undefined if \a key is greater than or equal to the number of
       *  keys.
       *  \sa getNumberOfKeys, getValue */
      unsigned int getFrame( size_t key ) const;

      /*! \brief Get the value of type \c T corresponding to the key \a key.
       *  \param key The key index to get the corresponding value of type \c T of.
       *  \return The value of type \c T corresponding to the key \c key.
       *  \note The behavior is undefined if \a key is greater than or equal to the number of
       *  keys.
       *  \sa getFrame, getNumberOfKeys */
      const T & getValue( size_t key ) const;

      /*! \brief Test for equivalence with another InterpolatedAnimation of type \c T.
       *  \param p A reference to the constant InterpolatedAnimation of type \c T to test for
       *  equivalence with
       *  \param ignoreNames Optional parameter to ignore the names of the objects; default is \c
       *  true
       *  \param deepCompare Optional parameter to perform a deep comparsion; default is \c false
       *  \return \c true if the InterpolatedAnimation \a p of type \c T is equivalent to \c this,
       *  otherwise \c false.
       *  \remarks Two InterpolatedAnimation objects of type \c T are equivalent if they are
       *  equivalent as Animation of type \c T, and all of their keys and values are pair-wise equal.
       *  \note The behavior is undefined if \a p is not an InterpolatedAnimation of type \c T nor
       *  derived from one.
       *  \sa Animation */
      virtual bool isEquivalent( const Object * p
                               , bool ignoreNames
                               , bool deepCompare ) const;

    protected:
      /*! \brief Protected constructor to prevent explicit creation.
       *  \remarks The current frame is set to be invalid, and there are no keys or values.
       *  \note Because InterpolatedAnimation is an abstract class, its constructor is never
       *  used explicitly, but in the constructor of derived classes. */
      InterpolatedAnimation( void );

      /*! \brief Protected copy constructor to prevent explicit creation.
       *  \remarks Keys and values, as well as the current frame are copied from \a rhs.
       *  \note Because InterpolatedAnimation is an abstract class, its constructor is never
       *  used explicitly, but in the constructor of derived classes. */
      InterpolatedAnimation( const InterpolatedAnimation &rhs );

      /*! \brief Get the current frame index.
       *  \return The current frame index.
       *  \remarks The current frame index is the index of the frame that was most recently requested.
       *  \sa getCurrentValue, setCurrent */
      unsigned int getCurrentFrame( void ) const;

      /*! \brief Get the current value of type \c T.
       *  \return The current value of type \c T.
       *  \remarks The current value is the value of the frame that was most recently requested.
       *  \sa getCurrentFrame, setCurrent */
      T getCurrentValue( void ) const;

      /*! \brief Get the key above or equal to frame \a frame.
       *  \param frame The frame index for which to get the corresponding (or above) key index
       *  \return The key index corresponding to the frame \a frame, or the key index above.
       *  \remarks 
       *  \note The behavior is undefined if \a frame is greater than or equal to the number of
       *  frames.
       *  \sa getNumberOfFrames */
      size_t getKey( unsigned int frame ) const;

      /*! \brief Set the frame and value of key index \a key as current.
       *  \param key The key index to set its frame index and value as current.
       *  \remarks The frame and value corresponding to the key index \a key are set as current.
       *  \note The behavior is undefined if \a key is greater than or equal to the number of 
       *  keys.
       *  \sa getCurrentFrame, getCurrentValue */
      void  setCurrent( size_t key ) const;

      /*! \brief Set \a frame and \a value as the current.
       *  \param frame The frame index to use as the current.
       *  \param value The value to use as the current.
       *  \sa getCurrentFrame, getCurrentValue, setCurrent */
      void  setCurrent( unsigned int frame, const T &value ) const;

      /*! \brief Protected assignment operator 
       *  \param rhs  Reference of the constant InterpolatedAnimation of type \c T to copy from
       *  \return A reference to the assigned InterpolatedAnimation of type \c T
       *  \remarks The assignment operator calls the assignment operator of Animation of type \c T
       *  and copies the keys and values, as well as the current frame information.
       *  \sa Animation */
      InterpolatedAnimation<T> & operator=(const InterpolatedAnimation<T> & rhs);

    private:
      std::vector<unsigned int>  m_keys;
      std::vector<T>             m_values;
      mutable unsigned int  m_currentFrame;
      mutable T             m_currentValue;
  };

  template<typename T> 
  inline InterpolatedAnimation<T>::InterpolatedAnimation( void )
  : Animation<T>()
  , m_currentFrame(0xFFFFFFFF)
  {
    NVSG_TRACE();
  }

  template<typename T> 
  inline InterpolatedAnimation<T>::InterpolatedAnimation( const InterpolatedAnimation &rhs )
  : Animation<T>(rhs)
  , m_keys( rhs.m_keys )
  , m_values( rhs.m_values )
  , m_currentFrame( rhs.m_currentFrame )
  , m_currentValue( rhs.m_currentValue )
  {
    NVSG_TRACE();
  }

  template<typename T> 
  inline InterpolatedAnimation<T>::~InterpolatedAnimation(void)
  {
    NVSG_TRACE();
  }

  template<typename T> 
  inline size_t InterpolatedAnimation<T>::getNumberOfFrames( void ) const
  {
    NVSG_TRACE();
    NVSG_ASSERT( m_keys.size() );                    //  the last key holds the index of the last frame,
    return( 1 + m_keys[m_keys.size()-1] );      //  but the number of frames is one greater
  }

  template<typename T> 
  inline size_t InterpolatedAnimation<T>::getNumberOfKeys( void ) const
  {
    NVSG_TRACE();
    return( m_keys.size() );
  }

  template<typename T> 
  inline void InterpolatedAnimation<T>::setNumberOfKeys( size_t count )
  {
    NVSG_TRACE();
    m_keys.resize( count );
    m_values.resize( count );
    this->increaseIncarnation();  // g++ (>=3.4) uses 2-stage name lookup. Since the direct base class is a template,
                                  // it does not find the non-dependent parameter without qualifier in 1st pass.
  }

  template<typename T> 
  inline void InterpolatedAnimation<T>::setKeyFrame( size_t key, unsigned int frame, const T &value )
  {
    NVSG_TRACE();
    NVSG_ASSERT( key < m_keys.size() );
    m_keys[key]   = frame;
    m_values[key] = value;
    this->increaseIncarnation();  // g++ (>=3.4) uses 2-stage name lookup. Since the direct base class is a template,
                                  // it does not find the non-dependent parameter without qualifier in 1st pass.
  }

  template<typename T> 
  inline unsigned int InterpolatedAnimation<T>::getCurrentFrame( void ) const
  {
    NVSG_TRACE();
    this->enterCS(); // secure exclusive write access to mutable members!
    unsigned int currFrame = m_currentFrame;
    this->leaveCS();
    return( m_currentFrame );
  }

  template<typename T> 
  inline T InterpolatedAnimation<T>::getCurrentValue( void ) const
  {
    NVSG_TRACE();
    this->enterCS(); // secure exclusive write access to mutable members!
    T currVal = m_currentValue;
    this->leaveCS();
    return( currVal );
  }

  template<typename T> 
  inline unsigned int InterpolatedAnimation<T>::getFrame( size_t key ) const
  {
    NVSG_TRACE();
    NVSG_ASSERT( key < m_keys.size() );
    return( m_keys[key] );
  }

  template<typename T> 
  inline const T & InterpolatedAnimation<T>::getValue( size_t key ) const
  {
    NVSG_TRACE();
    NVSG_ASSERT( key < m_keys.size() );
    return( m_values[key] );
  }

  template<typename T> 
  inline size_t InterpolatedAnimation<T>::getKey( unsigned int frame ) const
  {
    NVSG_TRACE();
    NVSG_ASSERT( frame < getNumberOfFrames() );
    std::vector<unsigned int>::const_iterator it = std::lower_bound( m_keys.begin(), m_keys.end(), frame );
    NVSG_ASSERT( it != m_keys.end() );
    return( it - m_keys.begin() );
  }

  template<typename T> 
  inline void InterpolatedAnimation<T>::setCurrent( size_t key ) const
  {
    NVSG_TRACE();
    this->enterCS(); // secure exclusive write access to mutable members!
    NVSG_ASSERT( ( key < m_keys.size() ) && ( m_keys.size() == m_values.size() ) );
    m_currentFrame = m_keys[key];
    m_currentValue = m_values[key];
    this->leaveCS();
  }

  template<typename T> 
  inline void InterpolatedAnimation<T>::setCurrent( unsigned int frame, const T &value ) const
  {
    NVSG_TRACE();
    this->enterCS(); // secure exclusive write access to mutable members!
    m_currentFrame = frame;
    m_currentValue = value;
    this->leaveCS();
  }

  
  template<typename T> 
  InterpolatedAnimation<T> & InterpolatedAnimation<T>::operator=(const InterpolatedAnimation<T> & rhs)
  {
    NVSG_TRACE();
    if (&rhs != this)
    {
      Animation<T>::operator=(rhs);
      m_keys          = rhs.m_keys;
      m_values        = rhs.m_values;
      m_currentFrame  = rhs.m_currentFrame;
      m_currentValue  = rhs.m_currentValue;
    }
    return *this;
  }

  template<typename T> 
  inline bool InterpolatedAnimation<T>::isEquivalent( const Object * p, bool ignoreNames, bool deepCompare) const
  {
    bool equi = Animation<T>::isEquivalent( p, ignoreNames, deepCompare );
    if ( equi )
    {
      const InterpolatedAnimation<T> * iat = dynamic_cast<const InterpolatedAnimation<T>*>(p);
      equi = ( m_keys == iat->m_keys ) && ( m_values == iat->m_values );
    }
    return( equi );
  }

}
