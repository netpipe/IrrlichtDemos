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
#include "nvmath/Trafo.h"
#include "nvsg/Group.h"

namespace nvsg
{
  /*! \brief Class that adds billboard properties to a Group.
   *  \par Namespace: nvsg
   *  \remarks The Billboard is a special transforming Group that performs a rotation around the
   *  \a rotationAxis, such that the contents of the Group is always oriented towards the viewer.
   *  \sa Group, Transform */
  class Billboard : public Group
  {
    public:
      /*! \brief Default-constructs a Billboard. 
       *  \remarks. The Billboard initially has the y-axis as rotation axis and is not screen 
       *  aligned. */
      NVSG_API Billboard();

      /*! \brief Constructs a Billboard from a Group.
       *  \param rhs Source Group.
       *  \remarks. The Billboard initially has the y-axis as rotation axis and is not screen 
       *  aligned. */
      NVSG_API explicit Billboard( const Group &rhs );

      /*! \brief Copy-constructs a Billboard from another Billboard.
       *  \param rhs Source Billboard. */
      NVSG_API Billboard( const Billboard &rhs );

      /*! \brief Destructs a Billboard. */
      NVSG_API virtual ~Billboard();

      /*! \brief Get the current Trafo if the Billboard is not screen aligned.
       *  \param viewerPosition A reference to the constant position of the viewer relative to
       *  the Billboard.
       *  \return An nvmath::Trafo, that represents the transformation of the Billboard.
       *  \remarks The \a viewerPosition is defined relative to the Billboard. It is used as the
       *  new local z-axis.
       *  \note If the Billboard is screen aligned, the result is undefined.
       *  \note The Trafo is not defined, if the rotation axis and the new z-axis are collinear.
       *  \sa isViewerAligned, getRotationAxis, getTrafoViewerAligned */
      NVSG_API nvmath::Trafo getTrafoAxisAligned( const nvmath::Vec3f & viewerPosition ) const;

      /*! \brief Get the current Trafo if the Billboard is viewer aligned.
       *  \param viewerPosition A reference to the constant position of the viewer relative to
       *  the Billboard.
       *  \param upVector A reference to the normalized constant up vector to use in the
       *  calculation.
       *  \return An nvmath::Trafo, that represents the transformation of the Billboard.
       *  \remarks The \a viewerPosition is defined relative to the Billboard. It is used as the
       *  new local z-axis. The \a upVector also is in the Billboard's local coordinate system.
       *  It's projection on the plane perpendicular to the local z-axis is used as the new local
       *  y-axis.
       *  \note If the Billboard is not viewer aligned, the result is undefined.
       *  \note If the upVector and the new z-axis are collinear, the minimal rotation from the
       *  default z-axis to the new z-axis is calculated.
       *  \sa isViewerAligned, getTrafoAxisAligned */
      NVSG_API nvmath::Trafo getTrafoViewerAligned( const nvmath::Vec3f & viewerPosition
                                                  , const nvmath::Vec3f & upVector ) const;

      /*! \brief Query if the Billboard is to viewer aligned.
       *  \return \c true if the Billboard is to viewer aligned, otherwise \c false
       *  \remarks A Billboard that is viewer aligned is one that always rotates to stay aligned
       *  with the viewer; even when the viewer elevates, pitches, and rolls. That is, any
       *  rotation axis is ignored. The default is \c false.
       *  \sa getRotationAxis, getTrafo, setViewerAligned */
      NVSG_API bool isViewerAligned() const;

      /*! \brief Set the Billboard to be screen aligned
       *  \param on \c true, if Billboard is to be screen aligned, otherwise \c false
       *  \remarks A Billboard that is screen aligned is one that it always rotates to stay aligned
       *  with the viewer; even when the viewer elevates, pitches, and rolls. That is, any
       *  rotation axis is ignored. The default is \c false.
       *  \sa isViewerAligned, setRotationAxis */
      NVSG_API void setViewerAligned( bool on );

      /*! \brief Get the rotation axis.
       *  \return A reference to the constant rotation axis.
       *  \remarks The rotation axis specifies which axis to use to perform the rotation. This
       *  axis is defined in the local coordinates of the Billboard. The default is the local
       *  y-axis, that is (0.0,1.0,0.0). The rotation axis is used only if the Billboard is not
       *  to be screen aligned. 
       *  \sa isViewerAligned, setRotationAxis */
      NVSG_API const nvmath::Vec3f & getRotationAxis() const;

      /*! \brief Set the rotation axis.
       *  \param axis A reference to the constant rotation axis.
       *  \remarks The rotation axis specifies which axis to use to perform the rotation. This
       *  axis is defined in the local coordinates of the Billboard. The default is the local
       *  y-axis, that is (0.0,1.0,0.0). The rotation axis is used only if the Billboard is not
       *  to be screen aligned. 
       *  \sa getRotationAxis, setViewerAligned */
      NVSG_API void setRotationAxis( const nvmath::Vec3f &axis );
    
      /*! \brief Assignment operator
       *  \param rhs Reference to the constant Billboard to copy from
       *  \return A reference to the assigned Billboard
       *  \remarks The assignment operator first calls the assignment operator of Group. Then the
       *  rotation axis and the screen alignment flag are copied.
       *  \sa Group */
      NVSG_API Billboard & operator=( const Billboard & rhs );
      
      /*! \brief Test for equivalence with another Billboard. 
       *  \param p            Pointer to the constant Billboard to test for equivalence with
       *  \param ignoreNames  Optional parameter to ignore the names of the objects; default is \c
       *  true
       *  \param deepCompare  Optional parameter to perform a deep comparison; default is \c false
       *  \return \c true if the Billboard \a p is equivalent to \c this, otherwise \c false.
       *  \remarks If \a p and \c this are equivalent as a group, they are equivalent if they are
       *  both screen aligned, or if they are not screen aligned but have the same rotation axis.
       *  \note The behavior is undefined if \a p is not a Billboard nor derived from one.
       *  \sa Group, getRotationAxis, isViewerAligned */
      NVSG_API virtual bool isEquivalent( const Object *p
                                        , bool ignoreNames
                                        , bool deepCompare ) const;

    private:
      nvmath::Vec3f m_rotationAxis;
      bool          m_viewerAligned;
  };

  inline const nvmath::Vec3f & Billboard::getRotationAxis() const
  {
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_READ_LOCKED(this);
    return( m_rotationAxis );
  }

  inline bool Billboard::isViewerAligned() const
  {
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_READ_LOCKED(this);
    return( m_viewerAligned );
  }

  inline void Billboard::setRotationAxis( const nvmath::Vec3f &rotationAxis )
  {
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_WRITE_LOCKED(this);
    m_rotationAxis = rotationAxis;
    invalidateBoundingSphere();
  }

  inline void Billboard::setViewerAligned( bool on )
  {
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_WRITE_LOCKED(this);
    m_viewerAligned = on;
    invalidateBoundingSphere();
  }
}
