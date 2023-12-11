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
#include "nvsg/HighlightObject.h"
#include "nvsg/PrimitiveSet.h"
#include "nvmath/Sphere3f.h"
#include "nvmath/Vec3f.h"

namespace nvsg
{
  /*! \brief A HighlightObject that realizes a wireframe drawing of the highlighted PrimitiveSet.
   *  \par Namespace: nvsg
   *  \remarks A HighlightEdges draws a wireframe version on top of a PrimitiveSet.
   *  \sa HighlightObject, PrimitiveSet */
  class HighlightEdges : public HighlightObject
  {
    public:
      /*! \brief Default-constructs a HighlightEdges */
      NVSG_API HighlightEdges();

      /*! \brief Constructs a HighlightEdges from a HighlighObject. 
       *  \param rhs HighlightObject to Construct the HighlightEdges from. */
      NVSG_API HighlightEdges( const HighlightObject &rhs);

      /*! \brief Constructs a HighlightEdges as a copy from another HighlighEdges.
       *  \param rhs HighlightEdges to copy from. */
      NVSG_API HighlightEdges( const HighlightEdges &rhs );
      
      /*! \brief Destructs a HighlightEdges. */
      NVSG_API virtual ~HighlightEdges();

      /*! \brief Get the color for drawing the box.
       *  \return A reference to the constant color.
       *  \remarks The default color is red (1.0,0.0,0.0). */
      NVSG_API const nvmath::Vec3f & getColor() const;

      /*! \brief Set the color for drawing the box.
       *  \param color  The line color.
       *  \remarks The default color is red (1.0,0.0,0.0). */
      NVSG_API void setColor( const nvmath::Vec3f &color );

      /*! \brief Get the line width for drawing the box.
       *  \return The width of the lines in pixels.
       *  \remarks The default line width is 2.0. */
      NVSG_API float getLineWidth() const;

      /*! \brief Set the line width for drawing the box.
       *  \param width  The line width.
       *  \remarks The default line width is 2.0. */
      NVSG_API void setLineWidth( float width );

      /*! \brief Returns the assigned PrimitiveSet.
       *  \return A pointer to the handle to the PrimitiveSet.
       *  \remarks A HighlightEdges holds a PrimitiveSet with vertices only. This can be used for
       *  drawing the HighlightEdges.
       *  \sa PrimitiveSet */
      NVSG_API PrimitiveSetHandle * getPrimitiveSet() const;

      /*! \brief Sets the PrimitiveSet.
       *  \param pset A pointer to the handle to the PrimitiveSet to set.
       *  \sa getPrimitiveSet */
      NVSG_API void setPrimitiveSet(PrimitiveSetHandle * pset);

      /*! \brief Assignment operator
       *  \param rhs A reference to the constant HighlightEdges to copy from.
       *  \return A reference to the assigned HighlightEdges.
       *  \remarks The assignment operator calls the assignment operator of HighlightObject and
       *  copies the PrimitiveSet, the line width and the line color. */
      NVSG_API HighlightEdges & operator=(const HighlightEdges & rhs);
      
    protected:
      /*! \brief Calculate the bounding sphere of this HighlightEdges.
       *  \return A nvmath::Sphere3f that contains the complete HighlightEdges.
       *  \remarks This function is called by the framework to determine a sphere that completely
       *  contains the HighlightEdges. */
      NVSG_API virtual nvmath::Sphere3f calculateBoundingSphere() const;

    private:
      void preparePrimitiveSet();

    private :
      PrimitiveSetHandle *  m_primitiveSet;
      float                 m_lineWidth;
      nvmath::Vec3f         m_color;
  };

  inline const nvmath::Vec3f & HighlightEdges::getColor() const
  {
    NVSG_TRACE();
    return( m_color );
  }

  inline float HighlightEdges::getLineWidth() const
  { 
    NVSG_TRACE();
    return( m_lineWidth );
  }       

  inline void HighlightEdges::setColor( const nvmath::Vec3f &color )
  {
    NVSG_TRACE();
    m_color = color;
  }

  inline void HighlightEdges::setLineWidth( float width )
  {
    NVSG_TRACE();
    m_lineWidth = width;
  }

  inline PrimitiveSetHandle * HighlightEdges::getPrimitiveSet() const
  {
    NVSG_TRACE();
    return( m_primitiveSet );
  }
}
