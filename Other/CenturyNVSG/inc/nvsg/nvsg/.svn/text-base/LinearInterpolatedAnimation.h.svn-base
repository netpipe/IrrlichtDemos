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
#include  "nvsg/InterpolatedAnimation.h"

#if !defined(DOXYGEN_IGNORE)
# if !defined(NDEBUG)
#  ifndef new
#   define new new(__FILE__, __LINE__)
#   define _DEFINED_DBGNEW // restrict the 'new' macro to this header file only
#  endif
# endif
#endif

namespace nvsg
{
  /*! \brief An InterpolatedAnimation class that performs linear interpolation.
   *  \par Namespace: nvsg
   *  \remarks The LinearInterpolatedAnimation of type T essentially is an InterpolatedAnimation
   *  with the constant random access operator being overloaded.
   *  \note Currently the only supported types for LinearInterpolatedAnimation are nvmath::Trafo
   *  and VNVector.
   *  \sa InterpolatedAnimation, nvmath::Trafo, VNVector */
  template<typename T> 
  class LinearInterpolatedAnimation : public InterpolatedAnimation<T>
  {
    public:
      /*! \brief Default-constructs a LinearInterpolatedAnimation<T>. */
      NVSG_API LinearInterpolatedAnimation();

      /*! \brief Constructs a LinearInterpolatedAnimation<T> from a InterpolatedAnimation<T>.
       *  \param rhs Source InterpolatedAnimation<T>. */
      NVSG_API explicit LinearInterpolatedAnimation( const InterpolatedAnimation<T> &rhs );

      /*! \brief Copy-constructs a LinearInterpolatedAnimation<T>.
       *  \param rhs Source LinearInterpolatedAnimation<T>. */
      NVSG_API LinearInterpolatedAnimation( const LinearInterpolatedAnimation &rhs );

      /*! \brief Destructs a LinearInterpolatedAnimation<T>. */
      NVSG_API virtual ~LinearInterpolatedAnimation();

      /*! \brief Interpolating random access operator.
       *  \param frame The index of the frame of the Animation to get the value for.
       *  \return A reference to the constant value at animation index \a frame.
       *  \remarks This operator calculates the animation value at index \a frame by linearly
       *  interpolating between the two nearest key-frame values. This is then used as the
       *  currentValue.
       *  \sa getCurrentFrame, getCurrentValue */
      NVSG_API virtual T operator[]( size_t frame ) const;

      /*! \brief Assignment operator
       *  \param rhs A reference to the constant LinearInterpolatedAnimation of type \c T to copy
       *  from.
       *  \return A reference to the assigned LinearInterpolatedAnimation of type \c T.
       *  \remarks The assignment operator just calls the assignment operator of the
       *  InterpolatedAnimation of type \c T. */
      LinearInterpolatedAnimation<T> & operator=(const LinearInterpolatedAnimation<T> & rhs);
  };

  template<typename T> 
    LinearInterpolatedAnimation<T> & LinearInterpolatedAnimation<T>::operator=(const LinearInterpolatedAnimation<T> & rhs)
  {
    NVSG_TRACE();
    return( InterpolatedAnimation<T>::operator=(rhs) );
  }

}

#ifdef _DEFINED_DBGNEW
# undef new
#endif
