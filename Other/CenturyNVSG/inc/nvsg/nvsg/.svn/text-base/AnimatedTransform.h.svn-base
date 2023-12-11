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

#include "nvsgcommon.h" // commonly used stuff
#include "nvsg/Transform.h" // base class definition

namespace nvsg
{
  /*! \brief Class representing an animated Transform.
   *  \par Namespace: nvsg
   *  \remarks An AnimatedTransform is a Transform that also holds an Animation of nvmath::Trafo
   *  objects. The nvmath::Trafo to use is determined from the Animation of each frame.
   *  \sa Animation, nvmath::Trafo, Transform */
  class AnimatedTransform : public Transform
  {
    public:
      /*! \brief Default-constructs a AnimatedTransform. 
       */
      NVSG_API AnimatedTransform();

      /*! \brief Constructs a AnimatedTransform as a copy of another AnimatedTransform. 
       */
      NVSG_API AnimatedTransform(const AnimatedTransform& rhs);

      /*! Destructs a AnimatedTransform
       */
      NVSG_API virtual ~AnimatedTransform( void );

      /*! \brief Get a pointer to the constant Animation.
       *  \return A pointer to the constant Animation, or \c NULL if there is none
       *  \sa Animation, nvmath::Trafo */
      NVSG_API TrafoAnimationHandle * getAnimation( void ) const;

      /*! \brief Set the Animation. 
       *  \param animation  Pointer to the constant Animation to set
       *  \remarks The reference count of the Animation \a animation is incremented and the
       *  reference count of any previous Animation is decremented. If these two Animation objects
       *  hold a different number of frames, the number of frames is invalidated. The current
       *  frame is reset to zero.
       *  \note The behavior is undefined if \a animation is not a valid pointer.
       *  \sa Animation, nvmath::Trafo, invalidateNumberOfFrames, addRef,
       *  removeRef */
      NVSG_API void setAnimation( TrafoAnimationHandle *animation );

      /*! \brief Set the current animation frame 
       *  \param frame  Index of the current frame
       *  \remarks The nvmath::Trafo of the animation step \a frame is used as the current transformation. 
       *  \note The behavior is undefined if \a frame is out of the range of the current
       *  Animation.
       *  \sa Animation, nvmath::Trafo */
      NVSG_API void setAnimationFrame( size_t frame );

      /*! \brief Set the nvmath::Trafo to use.
       *  \param trafo  Reference to the constant nvmath::Trafo to use
       *  \remarks In addition to setting the nvmath::Trafo by calling Transform::setTrafo, the
       *  current frame is invalidated.
       *  \sa nvmath::Trafo, Transform */
      NVSG_API virtual void setTrafo( const nvmath::Trafo &trafo );

      /*! \brief Get the number of animation frames. 
       *  \return Number of frames in the Animation */
      NVSG_API virtual size_t getNumberOfFrames( void ) const;

      /*! \brief Assignment operator 
       *  \param rhs  Reference to a constant AnimatedTransform to copy from
       *  \return A reference to the assigned AnimatedTransform
       *  \remarks The assignment operator first calls the assignment operator of Transform. Then
       *  the reference count of the current Animation is decremented, the Animation of \a rhs is
       *  cloned, and it's reference count is incremented.
       *  \sa Animation, Transform, addRef, removeRef */
      NVSG_API AnimatedTransform & operator=( const AnimatedTransform & rhs );
      
      /*! \brief Test for equivalence with another AnimatedTransform. 
       *  \param p            Pointer to the constant AnimatedTransform to test for equivalence
       *  with
       *  \param ignoreNames  Optional parameter to ignore the names of the objects; default is \c
       *  true
       *  \param deepCompare  Optional parameter to perform a deep comparsion; default is \c false
       *  \return \c true if the AnimatedTransform \a p is equivalent to \c this, otherwise \c
       *  false.
       *  \remarks If \a p and \c this are equivalent as Transform objects, their Animation
       *  objects are tested for equivalence. If \a deepCompare is false, they are considered to
       *  be equivalent if they are the same pointers. Otherwise, a full equivalence test is
       *  performed.
       *  \note The behavior is undefined if \a p is not an AnimatedTransform nor derived from
       *  one.
       *  \sa Animation, Object, Transform */
      NVSG_API virtual bool isEquivalent( const Object *p
                                        , bool ignoreNames
                                        , bool deepCompare ) const;

    protected:
      /*! \brief Validate the Incarnation. 
       *  \remarks This function is called from the framework when the Incarnation is requested
       *  and is marked as invalid. Then it is validated and the Incarnation of the Animation
       *  is requested to continue validation down the tree hierarchy.
       *  \sa Incarnation */
      NVSG_API virtual void validateIncarnation( void ) const;  // from top to bottom

    private:
      TrafoAnimationHandle * m_animation;
      size_t m_currentFrame;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - 
  // inlines
  // - - - - - - - - - - - - - - - - - - - - - - - - - 

  inline TrafoAnimationHandle * AnimatedTransform::getAnimation( void ) const
  {
    NVSG_TRACE();
    return( m_animation );
  }

} //  namespace nvsg
