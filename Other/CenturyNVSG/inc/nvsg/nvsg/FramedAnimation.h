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
#include "nvsg/Animation.h"

#if !defined(DOXYGEN_IGNORE)
# if !defined(NDEBUG)
#  ifndef new
#   define new new(__FILE__, __LINE__)
#   define _DEFINED_DBGNEW // restrict the 'new' macro to this header file only
#  endif
# endif
#endif //DOXYGEN_IGNORE

namespace nvsg
{
  class VNVector;
  
  /*! \brief Template class of an Animation that holds a complete data set for each frame.
   *  \par Namespace: nvsg
   *  \remarks A FramedAnimation holds a data set of type \c T, where \c T is the template
   *  parameter, for each frame.
   *  \note The FramedAnimation of type \c T performs no reference counting on the frame values of
   *  type \c T.
   *  \sa Animation, InterpolatedAnimation */
  template<typename T> class FramedAnimation : public Animation<T>
  {
    public:
      /*! \brief Default-constructs a FramedAnimation.
       */
      NVSG_API FramedAnimation();

      /*! \brief Explicit constructor.
       */
      NVSG_API explicit FramedAnimation( const Animation<T> &rhs );

      /*! \brief Constructs a FramedAnimation as a copy of another FramedAnimation.
       */
      NVSG_API FramedAnimation( const FramedAnimation<T> &rhs );

      /*! Destructs a FramedAnimation
       */
      virtual ~FramedAnimation();

      /*! \brief Get the number of frames.
       *  \return The number of frames
       *  \remarks The number of frame equals the size of the vector of values of type \c T held by
       *  this FramedAnimation of type \c T.
       *  \sa setNumberOfFrames */
      virtual size_t getNumberOfFrames() const;

      /*! \brief Set the number of frames.
       *  \param nof  Number of Frames to set.
       *  \remarks Setting the number of frames resizes the vector of values of type \c T. Thus,
       *  the constructor of \c T is called for each new element.\n
       *  The nvutil::Incarnation is increased.
       *  \sa getNumberOfFrames */
      void setNumberOfFrames( size_t nof );

      /*! \brief Test for equivalence with an other FramedAnimation of type \c T.
       *  \param p            A reference to the constant FramedAnimation of type \c T to test for
       *  equivalence with.
       *  \param ignoreNames  Optional parameter to ignore the names of the objects; default is \c
       *  true.
       *  \param deepCompare  Optional parameter to perform a deep comparsion; default is \c false.
       *  \return \c true if the FramedAnimation of type \c T \a p is equivalent to \c this,
       *  otherwise \c false.
       *  \remarks If \a p and \c this are equivalent as Animation of type \c T, they are
       *  equivalent if they have the same number of frames, and each frame has the same value of
       *  type \c T.
       *  \note The behavior is undefined if \a p is not a FramedAnimation of type \c T nor derived
       *  from one.
       *  \sa Animation */
      NVSG_API virtual bool isEquivalent( const Object * p
                                        , bool ignoreNames
                                        , bool deepCompare ) const;

      /*! \brief Constant random access operator.
       *  \param i  Index of the frame to access.
       *  \return The value of frame \a i.
       *  \remarks This operator can be used for easy read access to the values for each frame.
       *  \note The behavior is undefined if \a i is larger than or equal to the number of frames.
       *  \sa getNumberOfFrames, setNumberOfFrames */
      virtual T operator[]( size_t i ) const;

      /*! \brief Random access operator.
       *  \param i  Index of the frame to access.
       *  \return A reference to the value of frame \a i.
       *  \remarks This random access operator can be used for easy write access to the values for
       *  each frame.\n
       *  The nvutil::Incarnation is increased.
       *  \note The behavior is undefined if \a i is larger than or equal to the number of frames.
       *  \sa getNumberOfFrames, setNumberOfFrames */
      T & operator[]( size_t i );
  
      /*! \brief Assignment operator
       *  \param rhs A reference to the constant FramedAnimation of type \c T to copy from.
       *  \return A reference to the assigned FramedAnimation of type \c T.
       *  \remarks The assignment operator calls the assignment operator of Animation of type \c T
       *  and then copies the vector of values of type \c T .
       *  \sa Animation */
      FramedAnimation<T> & operator=( const FramedAnimation<T> & rhs );

    private:
      std::vector<T> m_values;
  };

  template<typename T>
  inline void FramedAnimation<T>::setNumberOfFrames( size_t nof )
  {
    NVSG_TRACE();
    m_values.resize( nof );
    this->increaseIncarnation();  // g++ (>=3.4) uses 2-stage name lookup. Since the direct base class is a template,
                                  // it does not find the non-dependent parameter without qualifier in 1st pass.
  }

  template<typename T>
  inline T & FramedAnimation<T>::operator[]( size_t i )
  {
    NVSG_TRACE();
    NVSG_ASSERT( i < m_values.size() );
    this->increaseIncarnation();  // g++ (>=3.4) uses 2-stage name lookup. Since the direct base class is a template,
                                  // it does not find the non-dependent parameter without qualifier in 1st pass.
    return( m_values[i] );
  }

}

#ifdef _DEFINED_DBGNEW
# undef new
#endif
