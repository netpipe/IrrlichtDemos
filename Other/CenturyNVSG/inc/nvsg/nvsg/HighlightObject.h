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
#include "nvsg/Object.h"
#include "nvmath/Sphere3f.h"

namespace nvsg
{
  class Drawable;

  /*! \brief Abstract base class for highlighting objects.
   *  \par Namespace: nvsg
   *  \remarks A HighlightObject draws some highlighting on top of a Drawable.
   *  \sa Object */
  class HighlightObject : public Object
  {
    public:
      /*! \brief Destructs a HighlightObject
       */
      NVSG_API virtual ~HighlightObject();

      /*! \brief Get the bounding sphere of this .
       *  \return A nvmath::Sphere3f that bounds the complete HighlightObject.
       *  \remarks If the bounding sphere currently is marked as invalid, calculateBoundingSphere()
       *  is used to calculate it and it is marked as valid.
       *  \sa calculateBoundingSphere, invalidateBoundingSphere */
      NVSG_API const nvmath::Sphere3f & getBoundingSphere() const;

      /*! \brief Invalidate the bounding sphere of this HighlightObject.
       *  \remarks If the bounding sphere currently is marked as valid, it is marked as invalid.
       *  \sa calculateBoundingSphere, getBoundingSphere */
      NVSG_API void invalidateBoundingSphere();

      /*! \brief Invalidate the HighlightObject.
       *  \remarks When the HighlightObject is marked as invalid, it needs recalculation.
       *  \sa isValid, validate */
      NVSG_API void invalidate();

      /*! \brief Validate the HighlightObject.
       *  \param drawable A pointer to the handle to the Drawable to use for validation.
       *  \remarks The base implementation just marks this HighlightObject to be valid.
       *  \sa invalidate, isValid */
      NVSG_API virtual void validate(DrawableHandle * drawable);

      /*! \brief Query, if the HighlightObject is valid
       *  \return \c true, if the HighlightObject is valid, otherwise \c false.
       *  \remarks When the HighlightObject is marked as invalid, it needs recalculation. This is
       *  the case before the first rendering, and every time the associated geometry is changed.
       *  \sa invalidate, validate */
      NVSG_API bool isValid() const;

  protected:
      /*! \brief Protected constructor to prevent explicit creation.
       *  \remarks The newly created HighlightObject is marked as invalid.
       *  \note A HighlightObject will not be instantiated directly, but by instantiating a
       *  derived object like HighlightBox or HighlightEdges, for example.*/
      NVSG_API HighlightObject();

      /*! \brief Protected copy constructor to prevent explicit creation.
       *  \param rhs A reference to the constant HighlightObject to copy from.
       *  \note A HighlightObject will not be instantiated directly, but by instantiating a
       *  derived object like HighlightBox or HighlightEdges, for example.*/
      NVSG_API HighlightObject( const HighlightObject &rhs );
      
      /*! \brief Interface for calculating the bounding sphere of this HighlightObject.
       *  \return A nvmath::Sphere3f that contains the complete HighlightEdges.
       *  \remarks This function is called by the framework to determine a sphere that completely
       *  contains the HighlightObject. */
      NVSG_API virtual nvmath::Sphere3f calculateBoundingSphere() const = 0;

      /*! \brief Assignment operator
       *  \param rhs A reference to the constant HighlightObject to copy from.
       *  \return A reference to the assigned HighlightObject.
       *  \remarks The assignment operator calls the assignment operator of Object and copies the
       *  valid flag and the bounding sphere. */
      NVSG_API HighlightObject & operator=(const HighlightObject & rhs);
      
    private:
      mutable nvmath::Sphere3f m_boundingSphere;
      mutable bool             m_boundingSphereValid;
      bool                     m_valid;
  };

  inline bool HighlightObject::isValid() const 
  { 
    NVSG_TRACE();
    return( m_valid ); 
  }
    
  inline void HighlightObject::invalidate() 
  { 
    NVSG_TRACE();
    m_valid = false; 
  }    
}
