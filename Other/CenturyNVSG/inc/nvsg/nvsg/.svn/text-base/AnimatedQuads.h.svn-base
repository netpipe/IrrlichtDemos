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
/** \file */

#include "nvsgcommon.h"
#include "nvsg/Quads.h" // base class definition

namespace nvsg
{
  /*! \brief Class representing an animated Quads.
   *  \par Namespace: nvsg
   *  \remarks This class is a Quads with animation of a VNVector. That is, the vertices and
   *  normals of the Quads can be animated.
   *  \sa AnimatedTriangles, Quads */
  class AnimatedQuads : public Quads
  {
    public:
      /*! \brief Constructs an AnimatedQuads. 
       * \param animation Optional Animation to assign to this AnimateQuads. */
      NVSG_API AnimatedQuads( VNVectorAnimationHandle *animation = NULL );

      /*! \brief Constructs an AnimatedQuads from a Quads. 
       * \param rhs Quads to construct the AnimatedQuads from.
       * \param animation Optional Animation to use. */
      NVSG_API AnimatedQuads( const Quads &rhs, VNVectorAnimationHandle *animation );

      /*! \brief Constructs an AnimatedQuads as a copy from another AnomatedQuads. */ 
      NVSG_API AnimatedQuads( const AnimatedQuads &rhs );

      /*! \brief Destructs an AnimatedQuads. */
      NVSG_API virtual ~AnimatedQuads( void );

      /*! \brief Get a pointer to the constant Animation.
       *  \return A pointer to the constant Animation, or \c NULL if there is none.
       *  \sa Animation, VNVector */
      NVSG_API VNVectorAnimationHandle * getAnimation( void ) const;

      /*! \brief Set the Animation. 
       *  \param animation  Pointer to the constant Animation to use
       *  \remarks The reference count of the Animation \a animation is incremented and the
       *  reference count of any previous Animation is decremented. If these two Animation objects
       *  contain a different number of frames, the number of frames is invalidated. The current
       *  frame is reset to zero.
       *  \note The behavior is undefined if \a animation is not a valid pointer.
       *  \sa Animation, VNVector, invalidateNumberOfFrames, addRef, removeRef */
      NVSG_API void setAnimation( VNVectorAnimationHandle *animation );

      /*! \brief Set the current animation frame 
       *  \param frame  Index of the current frame
       *  \remarks The vertices and normals of the animation step \a frame are used for the
       *  current Animation.
       *  \note The behaviour is undefined if \a frame is out of the range of the current
       *  Animation.
       *  \sa Animation, VNVector */
      NVSG_API void setAnimationFrame( size_t frame );

      /*! \brief Get the number of animation frames.
       *  \return Number of frames in the Animation. */
      NVSG_API virtual size_t getNumberOfFrames( void ) const;

      /*! \brief Assignment operator 
       *  \param rhs  Reference to a constant AnimatedQuads to copy from
       *  \return A reference to the assigned AnimatedQuads
       *  \remarks The assignment operator first calls the assignment operator of Quads. Then the
       *  reference count of the current Animation is decremented, the Animation of \a rhs is
       *  cloned, and its reference count is incremented.
       *  \sa Animation, Quads, addRef, removeRef */
      NVSG_API AnimatedQuads & operator=( const AnimatedQuads & rhs );

      /*! \brief Test for equivalence with an other AnimatedQuads. 
       *  \param p            Pointer to a constant AnimatedQuads to test for equivalence with
       *  \param ignoreNames  Optional parameter to ignore the names of the objects; default is \c
       *  true
       *  \param deepCompare  Optional parameter to perform a deep comparison; default is \c false
       *  \return \c true if the AnimatedQuads \a p is equivalent to \c this, otherwise \c false.
       *  \remarks If \a p and \c this are equivalent Quads, their Animation objects are tested
       *  for equivalence. If \a deepCompare is false, they are considered to be equivalent if
       *  they are the same pointers, otherwise a full equivalence test is performed.
       *  \note The behavior is undefined if \a p is not an AnimatedQuads nor derived from one.
       *  \sa Animation, Object, Quads */
      NVSG_API virtual bool isEquivalent( const Object *p
                                        , bool ignoreNames
                                        , bool deepCompare ) const;

    protected:

      /*! \brief Validate the Incarnation. 
       *  \remarks This function is called from the framework when the Incarnation is requested
       *  and it is marked as invalid. Then it is validated and the Incarnation of the Animation
       *  is requested to continue validation down the tree hierarchy.
       *  \a Incarnation */
      NVSG_API virtual void validateIncarnation( void ) const;  // from top to bottom

    private:
      VNVectorAnimationHandle * m_animation;
      size_t m_currentFrame;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - 
  // inlines
  // - - - - - - - - - - - - - - - - - - - - - - - - - 

  inline VNVectorAnimationHandle * AnimatedQuads::getAnimation( void ) const
  {
    NVSG_TRACE();
    return( m_animation );
  }

} //  namespace nvsg
