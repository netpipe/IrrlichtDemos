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
#include "nvsg/PrimitiveAttribute.h"
#include <float.h>

namespace nvsg
{
  /*! \brief PrimitiveAttribute specific for Drawables with lines, like Lines or LineStrips.
   *  \par Namespace: nvsg
   *  \remarks A LineAttribute holds the line-specific attributes of a StatePass.
   *  \note Faces and points are not affected by a LineAttribute.
   *  \sa FaceAttribute, PointAttribute, PrimitiveAttribute, StatePass */
  class LineAttribute : public PrimitiveAttribute
  {
    public:
      /*! \brief Default-constructs a LineAttribute. 
       *  \remarks A default-constructed LineAttribute initially has anti-aliasing off, has
       *  has a stipple factor of 1, a stipple pattern of 0xFFFF, and a line with of 1.0. */
      NVSG_API LineAttribute();

      /*! \brief Constructs a LineAttribute from a PrimitiveAttribute
       *  \param rhs Source PrimitiveAttribute
       *  \remarks The LineAttribute initially has anti-aliasing off, has has a stipple factor of 1, 
       *  a stipple pattern of 0xFFFF, and a line with of 1.0. */
      NVSG_API explicit LineAttribute( const PrimitiveAttribute &rhs );

      /*! \brief Constructs a LineAttribute as a copy from another LineAttribute. */
      NVSG_API LineAttribute( const LineAttribute &rhs );

      /*! \brief Destructs a LineAttribute. */
      NVSG_API virtual ~LineAttribute();

      /*! \brief Get the stipple pattern.
       *  \return The 16-bit stipple pattern
       *  \remarks The stipple pattern is a 16-bit series of 0s and 1s, and is repeated as
       *  necessary to stipple a given line. A 1 indicates that drawing occurs, and a 0 indicates 
       *  that it does not, on a pixel-by-pixel basis, beginning with the low-order bit of the pattern.
       *  The pattern can be stretched out by using the stipple factor. The default stipple pattern is
       *  0xFFFF, resulting in a non-stippled line.
       *  \sa getStippleFactor, setStipplePattern */
      NVSG_API unsigned short getStipplePattern() const;

      /*! \brief Set the stipple pattern.
       *  \param pattern The 16-bit stipple pattern to use
       *  \remarks The stipple pattern is a 16-bit series of 0s and 1s, and is repeated as
       *  necessary to stipple a given line. A 1 indicates that drawing occurs, and a 0 indicates 
       *  that it does not, on a pixel-by-pixel basis, beginning with the low-order bit of the pattern. 
       *  The pattern can be stretched out by using the stipple factor. The default stipple pattern is
       *  0xFFFF, resulting in a non-stippled line.
       *  \sa getStipplePattern, setStippleFactor */
      NVSG_API void setStipplePattern( unsigned short pattern );

      /*! \brief Get the stipple factor.
       *  \return The stipple factor.
       *  \remarks The stipple factor can be used to stretch out the stipple pattern. It multiplies
       *  each sub-series of consecutive 1s and 0s. Thus, if three consecutive 1s appear in the
       *  pattern, they're stretched to six if the stipple factor is 2. The stipple factor is
       *  clamped to lie between 1 and 255, with the default being 1.
       *  \sa getStipplePattern, setStippleFactor */
      NVSG_API unsigned short getStippleFactor() const;

      /*! \brief Set the stipple factor.
       *  \param factor The stipple factor.
       *  \remarks The stipple factor can be used to stretch out the stipple pattern. It multiplies
       *  each sub-series of consecutive 1s and 0s. Thus, if three consecutive 1s appear in the
       *  pattern, they're stretched to six if the stipple factor is 2. The stipple factor is
       *  clamped to lie between 1 and 255, with the default being 1.
       *  \sa getStippleFactor, setStipplePattern */
      NVSG_API void setStippleFactor( unsigned short factor );

      /*! \brief Get the line width.
       *  \return The line width.
       *  \remarks The line width is the width for rendered lines in pixels. It is greater than 0.0
       *  and defaults to 1.0.
       *  \sa setWidth */
      NVSG_API float getWidth() const;

      /*! \brief Set the line width.
       *  \param width The line width to set.
       *  \remarks The line width is the width for rendered lines in pixels. It has to be greater
       *  than 0.0 and defaults to 1.0.
       *  \sa getWidth */
      NVSG_API void setWidth( float width );

      /*! \brief Get the line anti-aliasing state.
       *  \return \c true, if anti-aliasing for lines is on, otherwise \c false.
       *  \sa setAntiAliasing */
      NVSG_API bool isAntiAliased() const;

      /*! \brief Set the line anti-aliasing state.
       *  \param aa \c true, if line anti-aliasing is to switch on, otherwise \c false.
       *  \sa isAntiAliased */
      NVSG_API void setAntiAliasing( bool aa );

      /*! \brief Test for equivalence with an other LineAttribute. 
       *  \param p A reference to the constant LineAttribute to test for equivalence with.
       *  \param ignoreNames Optional parameter to ignore the names of the objects; default is \c true.
       *  \param deepCompare Optional parameter to perform a deep comparsion; default is \c false.
       *  \return \c true if the LineAttribute \a p is equivalent to \c this, otherwise \c false.
       *  \remarks If \a p and \c this are equivalent as PrimitiveAttribute, they are equivalent
       *  if they have the same anti-aliasing state, stipple factor, stipple pattern, and width.
       *  \sa PrimitiveAttribute */
      NVSG_API virtual bool isEquivalent( const Object * p
                                        , bool ignoreNames
                                        , bool deepCompare ) const;

      /*! \brief Assignment operator
       *  \param rhs A reference to the constant LineAttribute to copy from
       *  \return A reference to the assigned LineAttribute
       *  \remarks The assignment operator calls the assignment operator of PrimitiveAttribute and
       *  copies the LineAttribute specific data then.
       *  \sa PrimitiveAttribute */
      NVSG_API LineAttribute & operator=(const LineAttribute & rhs);

    private:
      bool            m_antiAliasing;
      unsigned short  m_stippleFactor;
      unsigned short  m_stipplePattern;
      float           m_width;
  };

  inline unsigned short LineAttribute::getStippleFactor() const
  {
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_READ_LOCKED(this);
    return( m_stippleFactor );
  }

  inline unsigned short LineAttribute::getStipplePattern() const
  {
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_READ_LOCKED(this);
    return( m_stipplePattern );
  }

  inline float LineAttribute::getWidth() const
  {
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_READ_LOCKED(this);
    return( m_width );
  }

  inline bool LineAttribute::isAntiAliased() const
  {
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_READ_LOCKED(this);
    return( m_antiAliasing );
  }

  inline void LineAttribute::setAntiAliasing( bool aa )
  {
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_WRITE_LOCKED(this);
    m_antiAliasing = aa;
    increaseIncarnation();
  }

  inline void LineAttribute::setStippleFactor( unsigned short factor )
  {
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_WRITE_LOCKED(this);
    NVSG_ASSERT( ( 0 < factor ) && ( factor < 256 ) );
    m_stippleFactor = factor;
    increaseIncarnation();
  }

  inline void LineAttribute::setStipplePattern( unsigned short pattern )
  {
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_WRITE_LOCKED(this);
    m_stipplePattern = pattern;
    increaseIncarnation();
  }

  inline void LineAttribute::setWidth( float width )
  {
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_WRITE_LOCKED(this);
    NVSG_ASSERT( FLT_EPSILON < width );
    m_width = width;
    increaseIncarnation();
  }

} // namespace nvsg
