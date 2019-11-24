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
#include  "nvsg/Object.h"

namespace nvsg
{
  /*! \brief Class defining the common interface of an animation.
   *  \par Namespace: nvsg
   *  \remarks An Animation is a pure virtual template class derived from Object, and defines the
   *  common interface of an animation.
   *  \sa FramedAnimation, InterpolatedAnimation */
  template<typename T> class Animation : public Object
  {
    public:

      /*! \brief Protected destructor to prevent explicit creation.
       *  \sa addRef, removeRef */
      virtual ~Animation();

      /*! \brief Interface for getting the number of animation frames.
       *  \return Number of frames in the Animation. */
      virtual size_t getNumberOfFrames() const = 0;

      /*! \brief Interface for the constant random access operator.
       *  \param i Index of frame to access
       *  \return The value of the \a i-th element of the Animation.
       *  \remarks Only the constant random access operator is required. But a concrete class
       *  based on Animation also might define a non-constant random access operator if
       *  appropriate.
       *  \note The behavior is undefined if \a i is out of the range of the Animation. */
      virtual T operator[]( size_t i ) const = 0;

    protected:
      /*! \brief Protected constructor to prevent explicit creation.
       *  \remarks Because Animation is an abstract class, its constructor is never used
       *  explicitly, but rather in the constructor of derived classes.
       *  \sa FramedAnimation, InterpolatedAnimation */
      Animation( void );

      /*! \brief Protected assignment operator 
       *  \param rhs  Reference of the constant Animation to copy from
       *  \return A reference to the assigned Animation.
       *  \remarks The assignment operator just calls the assignment operator of Object.
       *  \sa Object */
      Animation<T> & operator=( const Animation<T> & rhs );
  };

  template<typename T> 
  Animation<T>::Animation( void )
  {
    NVSG_TRACE();
  }

  template<typename T> 
  Animation<T>::~Animation( void )
  {
    NVSG_TRACE();
  }

  template<typename T> 
  Animation<T> & Animation<T>::operator=(const Animation<T> & rhs)
  {
    NVSG_TRACE();
    if (&rhs != this)
    {
      Object::operator=(rhs);
    }
    return *this;
  }

} //  namespace nvsg
