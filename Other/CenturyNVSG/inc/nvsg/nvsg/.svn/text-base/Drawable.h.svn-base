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

#include "nvsgcommon.h"  // commonly used stuff
#include "nvmath/Sphere3f.h"
#include "nvsg/OwnedObject.h" // base class definition

namespace nvsg
{
  /*! \brief The abstract base class of all drawable objects.
   *  \par Namespace: nvsg
   *  \remarks All drawable objects like PrimitiveSet or Shape derive from Drawable.\n
   *  When deriving from Drawable, you have to overload the member functions clone(), and
   *  calculateBoundingSphere(). Moreover, you can overload getNumberOfFrames(),
   *  validateIncarnation(), and invalidateIncarnation().
   *  \sa nvutil::Incarnation, OwnedObject, PrimitiveSet, Shape */
  class Drawable : public OwnedObject<NodeHandle>
  {
    friend class Node;

    public:
      /*! \brief Destructs a Drawable.
       */
      NVSG_API virtual ~Drawable( void );

      /*! \brief Get the bounding sphere of this Drawable.
       *  \return A reference to a constant nvmath::Sphere3f that bounds the complete Drawable.
       *  \remarks If the bounding sphere currently is marked as invalid, calculateBoundingSphere()
       *  is used to calculate it. If there is a highlight object attached, this is combined with
       *  the bounding sphere of the highlight object. Then the bounding sphere is marked as valid.
       *  \sa HighlightObject, calculateBoundingSphere, invalidateBoundingSphere */
      NVSG_API const nvmath::Sphere3f & getBoundingSphere( void ) const;

      /*! \brief Invalidate the bounding sphere of this Drawable.
       *  \remarks If the bounding sphere currently is marked as invalid, the nvutil::Incarnation
       *  is increased, thus reporting a change up the tree hierarchy. Otherwise, the
       *  nvutil::Incarnation is only invalidated, but on all owners and on an attached highlight
       *  object the bounding sphere is invalidated, thus also reporting the change up the tree
       *  hierarchy.
       *  \sa HighlightObject, calculateBoundingSphere, getBoundingSphere */
      NVSG_API void invalidateBoundingSphere( void );

      /*! \brief Get the number of animation frames.
       *  \return The number of animation frames of this Drawable.
       *  \remarks Objects derived from Drawable that have an Animation attached should return the
       *  number of frames of that Animation. The default implementation returns zero, thus
       *  indicating that there is no Animation attached.
       *  \sa Animation, invalidateNumberOfFrames */
      NVSG_API virtual size_t getNumberOfFrames( void ) const;

      /*! \brief Invalidate the number of animation frames.
       *  \remarks If the number of frames of an animation attached to the Drawable changes, this
       *  function should be called. It invalidates the number of frames of each owner, thus
       *  reporting the change up the tree hierarchy.
       *  \sa Animation, getNumberOfFrames */
      NVSG_API void invalidateNumberOfFrames( void );

      /*! \brief Attach a HighlightObject to this Drawable.
       *  \param obj Optional pointer to the handle to the HighlightObject to attach. Default is NULL.
       *  \remarks If \a obj is not NULL, it is validated and its reference count is increased.
       *  The reference count of any previously attached HighlightObject is decreased. The
       *  bounding sphere of this Drawable is invalidated, because the new HighlightObject might
       *  influence that.
       *  \sa HighlightObject, getHighlightObject */
      NVSG_API void attachHighlightObject( HighlightObjectHandle* obj = NULL );

      /*! \brief Get the attached HighlightObject.
       *  \return A pointer to the handle to the attached HighlightObject.
       *  \sa attachHighlightObject */
      NVSG_API HighlightObjectHandle * getHighlightObject( void ) const;

    protected:
      /*! \brief Protected constructor to prevent explicit creation.
       *  \remarks Because Drawable is an abstract class, its constructor is never used
       *  explicitly, but in the constructor of derived classes.
       *  \sa PrimitiveSet, Shape */
      NVSG_API Drawable( void );

      /*! \brief Protected copy constructor to prevent explicit creation.
       *  \remarks Because Drawable is an abstract class, its constructor is never used
       *  explicitly, but in the constructor of derived classes.
       *  \sa PrimitiveSet, Shape */
      NVSG_API Drawable( const Drawable& );

      /*! \brief Interface for calculating the bounding sphere of this Drawable.
       *  \return A nvmath::Sphere3f that contains the complete Drawable.
       *  \remarks This function is called by the framework to determine a sphere that completely
       *  contains the Drawable. Every concrete class deriving from Drawable has to implement this
       *  function.
       *  \sa getBoundingSphere, invalidateBoundingSphere */
      NVSG_API virtual nvmath::Sphere3f calculateBoundingSphere( void ) const = 0;

      /*! \brief Validate the nvutil::Incarnation.
       *  \remarks This function is called from the framework when the nvutil::Incarnation of this
       *  Drawable is requested but marked as invalid. Then it is validated and the
       *  nvutil::Incarnation of any HighlightObject is requested to continue the validation down
       *  the tree hierarchy.
       *  \sa HighlightObject, nvutil::Incarnation, invalidateIncarnation */
      NVSG_API virtual void validateIncarnation( void ) const;  // from top to bottom

      /*! \brief Invalidate the nvutil::Incarnation.
       *  \remarks This function is called from the framework when the nvutil::Incarnation of this
       *  Drawable has increased and now is to be marked as invalid. It first increases the
       *  nvutil::Incarnation of all its owners, then its own nvutil::Incarnation is marked as
       *  invalid.
       *  \sa GeoNode, nvutil::Incarnation, validateIncarnation */
      NVSG_API virtual void invalidateIncarnation( void );      // from bottom to top

      /*! \brief Assignment operator
       *  \param rhs A reference to the constant Drawable to copy from
       *  \return A reference to the assigned Drawable
       *  \remarks The assignment operator calls the assignment operator of OwnedObject and
       *  copies the bounding sphere.
       *  \note If rhs has a HighlightObject attached, this is not copied. If \c this has a
       *  HighlightObject attached, its reference count is decremented and it is detached.
       *  \sa HighlightObject, OwnedObject */
      NVSG_API Drawable & operator=(const Drawable & rhs);

    private:
      HighlightObjectHandle * m_highlightObject; // optional attached HighlightObject

      mutable nvmath::Sphere3f m_boundingSphere; 
      mutable bool m_boundingSphereValid;
  };

  inline HighlightObjectHandle * Drawable::getHighlightObject( void ) const
  {
    NVSG_TRACE();
    return( m_highlightObject );
  }

  template <typename T>
  inline T drawable_cast(const Drawable * drwbl)
  {
    NVSG_CTASSERT(!!(nvutil::Conversion<T,const Drawable*>::exists));
    return dynamic_cast<T>(drwbl);
  }

  template <typename T>
  inline T drawable_cast(Drawable * drwbl)
  {
    NVSG_CTASSERT(!!(nvutil::Conversion<T,Drawable*>::exists));
    return dynamic_cast<T>(drwbl);
  }
} // namespace nvsg
