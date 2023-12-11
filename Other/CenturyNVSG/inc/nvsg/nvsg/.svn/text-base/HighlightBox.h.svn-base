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
#include "nvsg/HighlightObject.h" // base class definition

namespace nvsg 
{
  /*! \brief A HighlightObject that realizes a bounding box around a Drawable.
   *  \par Namespace: nvsg
   *  \remarks A HighlightBox draws a wireframe box around a Drawable.
   *  \sa Drawable, HighlightObject */
  class HighlightBox : public HighlightObject
  {
    public:
      /*! \brief Default-constructs a HighlightBox. 
       *  \remarks The HighlightBox has a default line width of 2.0 and a default line color of red
       *  (1.0,0.0,0.0). 
       */
      NVSG_API HighlightBox();

      /*! \brief Constructs a HighlightBox from a HighlightObject. 
       *  \remarks The HighlightBox has a default line width of 2.0 and a default line color of red
       *  (1.0,0.0,0.0). 
       */
      NVSG_API explicit HighlightBox( const HighlightObject &rhs );

      /*! \brief Constructs a HighlightBox from another HighlightBox. 
       */
      NVSG_API HighlightBox( const HighlightBox &rhs );

      /*! \brief Validate the highlight geometry
       *  \param drawable A pointer to the constant Drawable to highlight.
       *  \remarks Calculates the bounding box of \a drawable to use as the highlight geometry.
       *  \sa Drawable, HighlightObject */
      NVSG_API virtual void validate( DrawableHandle * drawable );

      /*! \brief Get the vertices of the HighlightBox.
       *  \return A pointer to the constant vertices of the HighlightBox. */
      NVSG_API const nvmath::Vec3f* getVertices() const;

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

      /*! \brief Assignment operator
       *  \param rhs A reference to the constant HighlightBox to copy from.
       *  \return A reference to the assigned HighlightBox.
       *  \remarks The assignment operator calls the assignment operator of HighlightObject and
       *  copies the vertices, the line width and the line color. */
      NVSG_API HighlightBox & operator=(const HighlightBox & rhs);
      
    protected:
      /*! \brief Calculate the bounding sphere of this HighlightBox.
       *  \return A nvmath::Sphere3f that contains the complete HighlightBox.
       *  \remarks This function is called by the framework to determine a sphere that completely
       *  contains the HighlightBox. */
      NVSG_API virtual nvmath::Sphere3f calculateBoundingSphere() const;

    private:
      nvmath::Vec3f m_vertices[8];     //!< the box's edges
      float         m_lineWidth;       //!< line width used for drawing the bounding box
      nvmath::Vec3f m_color;           //!< color used for drawing
  };

  inline nvmath::Sphere3f HighlightBox::calculateBoundingSphere() const
  {
    NVSG_TRACE();
    return( boundingSphere( &m_vertices[0], 8 ) );
  }

  inline const nvmath::Vec3f & HighlightBox::getColor() const
  {
    NVSG_TRACE();
    return( m_color );
  }

  inline float HighlightBox::getLineWidth() const
  { 
    NVSG_TRACE();
    return( m_lineWidth );
  }       

  inline const nvmath::Vec3f* HighlightBox::getVertices() const
  {
    NVSG_TRACE();
    return( m_vertices );
  }

  inline void HighlightBox::setColor( const nvmath::Vec3f &color )
  {
    NVSG_TRACE();
    m_color = color;
  }

  inline void HighlightBox::setLineWidth( float width )
  {
    NVSG_TRACE();
    m_lineWidth = width;
  }

}
