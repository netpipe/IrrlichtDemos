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
  /*! \brief PrimitiveAttribute specific for Points.
   *  \par Namespace: nvsg
   *  \remarks A PointAttribute holds the point-specific attributes of a StatePass.
   *  \note Faces and Lines are not affected by a PointAttribute.
   *  \sa FaceAttribute, LineAttribute, PrimitiveAttribute, StatePass */
  class PointAttribute : public PrimitiveAttribute
  {
    public:
      /*! \brief Default-constructs a PointAttribute. 
       *  \remarks A default-constructed PointAttribute has anti-aliasing off, and a point size
       *  of 1.0. */
      NVSG_API PointAttribute();

      /*! \brief Constructs a PointAttribute from a PrimitiveAttribute.
       *  \param rhs Source PrimitiveAttribute
       *  \remarks The PointAttribute will be initially has anti-aliasing off, and a point size
       *  of 1.0. */ 
       NVSG_API explicit PointAttribute( const PrimitiveAttribute &rhs );

      /*! \brief Constructs a PointAttribute as a copy from another PointAttribute. */
      NVSG_API PointAttribute( const PointAttribute &rhs );
 
      /*! \brief Destructs a PointAttribute. */
      NVSG_API virtual ~PointAttribute();

      /*! \brief Get the anti-aliasing state.
       *  \return \c true, if anti-aliasing for Points is enabled, otherwise \c false.
       *  \remarks With anti-aliasing enabled, a point is drawn as a circular group of pixels, and
       *  the pixels on the boundaries are typically drawn at less than full intensity to give the
       *  edge a smoother appearance. By default, anti-aliasing is disabled.
       *  \sa getSize, setAntiAliasing */
      NVSG_API bool isAntiAliased() const;

      /*! \brief Set the anti-aliasing state.
       *  \param aa \c true, if anti-aliasing is to be enabled, otherwise \c false.
       *  \remarks With anti-aliasing enabled, a point is drawn as a circular group of pixels, and
       *  the pixels on the boundaries are typically drawn at less than full intensity to give the
       *  edge a smoother appearance. By default, anti-aliasing is disabled.
       *  \sa isAntiAliasing, setSize */
      NVSG_API void setAntiAliasing( bool aa );

      /*! \brief Get the point size
       *  \return The point size
       *  \remarks The size must be greater than 0.0, and by default is 1.0.\n
       *  \sa isAntiAliased, setSize */
      NVSG_API float getSize() const;

      /*! \brief Set the point size
       *  \param size The point size
       *  \remarks The size must be greater than 0.0, and by default is 1.0.\n
       *  \sa getSize, setAntiAliasing */
      NVSG_API void setSize( float size );

      /*! \brief Assignment operator
       *  \param rhs A reference to the constant PointAttribute to copy from
       *  \return A reference to the assigned PointAttribute
       *  \remarks The assignment operator calls the assignment operator of PrimitiveAttribute and
       *  then copies the PointAttribute specific data.
       *  \sa PrimitiveAttribute */
      NVSG_API PointAttribute & operator=(const PointAttribute & rhs);

      /*! \brief Test for equivalence with an other PointAttribute. 
       *  \param p A reference to the constant PointAttribute to test for equivalence with.
       *  \param ignoreNames Optional parameter to ignore the names of the objects; default is \c true.
       *  \param deepCompare Optional parameter to perform a deep comparsion; default is \c false.
       *  \return \c true if the PointAttribute \a p is equivalent to \c this, otherwise \c false.
       *  \remarks If \a p and \c this are equivalent as PrimitiveAttribute, they are equivalent
       *  if they have the same anti-aliasing state and the same point size.
       *  \note The behavior is undefined if \a p is not a PointAttribute nor derived from one.
       *  \sa PrimitiveAttribute */
      NVSG_API virtual bool isEquivalent( const Object * p
                                        , bool ignoreNames
                                        , bool deepCompare ) const;

    private:
      bool  m_antiAliasing;
      float m_size;
  };

  inline float PointAttribute::getSize() const
  {
    return( m_size );
  }

  inline bool PointAttribute::isAntiAliased() const
  {
    return( m_antiAliasing );
  }

  inline void PointAttribute::setAntiAliasing( bool aa )
  {
    m_antiAliasing = aa;
    increaseIncarnation();
  }

  inline void PointAttribute::setSize( float size )
  {
    NVSG_ASSERT( FLT_EPSILON < size );
    m_size = size;
    increaseIncarnation();
  }

} // namespace nvsg
