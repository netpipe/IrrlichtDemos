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
#include "nvmath/Vec3f.h"
#include "nvutil/BitMask.h"


namespace nvsg
{
  class FaceAttribute;

  typedef enum
  {
    FM_POINT,
    FM_LINE,
    FM_FACE
  } FaceMode;

  /*! \brief PrimitiveAttribute specific for Drawables with faces, like Triangles or QuadStrips.
   *  \par Namespace: nvsg
   *  \remarks A FaceAttribute holds the face-specific attributes of a StatePass.
   *  \note Lines and Points are not affected by a FaceAttribute.
   *  \sa LineAttribute, PointAttribute, PrimitiveAttribute, StatePass */
  class FaceAttribute : public PrimitiveAttribute
  {
    public:
      /*! \brief Default-constructs a FaceAttribute. 
       *  \remarks A default constructed FaceAttribute has back-face culling on, front-face culling
       *  off, polygon mode disabled, and two-sided lighting off. */
      NVSG_API FaceAttribute();

      /*! \brief Constructs a FaceAttribute from a PrimitiveAttribute.
       *  \param rhs Source PrimitiveAttribute
       *  \remarks After instantiation, the FaceAttribute has back-face culling on, front-face 
       *  culling off, polygon mode disabled, and two-sided lighting off. */ 
      NVSG_API explicit FaceAttribute( const PrimitiveAttribute &rhs );

      /*! \brief Copy-constructs a FaceAttribute from another FaceAttribute. */
      NVSG_API FaceAttribute( const FaceAttribute &rhs );

      /*! \brief Destructs a FaceAttribute. */
      NVSG_API virtual ~FaceAttribute();

      /*! \brief Ask if the front faces are culled.
       *  \return \c true, if front faces are culled, otherwise \c false.
       *  \remarks By convention, faces whose vertices appear in counterclockwise order on the
       *  screen are called front-facing. When front face culling is on, those faces are not
       *  drawn.\n
       *  By default front face culling is off.
       *  \sa isBackFaceCulling, setCullMode */
      NVSG_API bool isFrontFaceCulling() const;

      /*! \brief Ask if the back faces are culled.
       *  \return \c true, if back faces are culled, otherwise \c false.
       *  \remarks By convention, faces whose vertices appear in clockwise order on the screen are
       *  called back-facing. When back face culling is on, those faces are not drawn.\n
       *  By default back face culling is on.
       *  \sa isFrontFaceCulling, setCullMode */
      NVSG_API bool isBackFaceCulling() const;

      /*! \brief Set the culling mode for front and back faces.
       *  \param frontCulling If \c true, enables front face culling, otherwise disables it.
       *  \param backCulling If \c true, enables back face culling, otherwise disables it.
       *  \remarks By convention, faces whose vertices appear in counterclockwise order on the
       *  screen are called front-facing, and faces whose vertices appear in clockwise order on
       *  the screen are called back-facing. With \a frontCulling, culling of the front faces can
       *  be enabled and disabled, and with \a backCulling, culling of the back faces can be
       *  enabled and disabled.\n
       *  By default front face culling is off, and back face culling is on.
       *  \sa isBackFaceCulling, isFrontFaceCulling */
      NVSG_API void setCullMode( bool frontCulling, bool backCulling );

      /*! \brief Get the mode front faces are to be displayed.
       *  \return The FaceMode that is used for the front faces.
       *  \remarks A face can be displayed as a number of zero dimensional points (FM_POINT),
       *  one dimensional lines (FM_LINE), or as a two dimensional face (FM_FACE). This can be
       *  selected independently for front and back faces.\n
       *  The default for both front and back faces is FM_FACE.
       *  \sa getBackFaceMode, isFrontFaceCulling, setFaceMode */
      NVSG_API FaceMode getFrontFaceMode() const;

      /*! \brief Get the mode back faces are to be displayed.
       *  \return The FaceMode that is used for the back faces.
       *  \remarks A face can be displayed as a number of zero dimensional points (FM_POINT),
       *  one dimensional lines (FM_LINE), or as a two dimensional face (FM_FACE). This can be
       *  selected independently for front and back faces.\n
       *  The default for both front and back faces is FM_FACE.
       *  \sa getFrontFaceMode, isBackFaceCulling, setFaceMode */
      NVSG_API FaceMode getBackFaceMode() const;

      /*! \brief Set the mode front and back faces are to be displayed.
       *  \param frontMode The FaceMode for the front faces.
       *  \param backMode The FaceMode for the back faces.
       *  \remarks A face can be displayed as a number of zero dimensional points (FM_POINT),
       *  one dimensional lines (FM_LINE), or as a two dimensional face (FM_FACE). This can be
       *  selected independently for front and back faces.\n
       *  The default for both front and back faces is FM_FACE.
       *  \sa getFrontFaceMode, getBackFaceMode, setCullMode */
      NVSG_API void setFaceMode( FaceMode frontMode, FaceMode backMode );

      /*! \brief Query if polygon offset is on.
       *  \return \c true, if polygon offset is on, otherwise \c false.
       *  \remarks The polygon offset flag specifies how polygon offset is handled. If it is \c
       *  false, no polygon offset is used. Otherwise, each fragment's depth value will be offset
       *  after it is interpolated from the depth values of the appropriate vertices. The offset
       *  is added before the depth test is performed and before the value is written into the
       *  depth buffer. The default is false.
       *  \sa getPolygonOffsetFactor, getPolygonOffsetUnits, setPolygonOffset */
      NVSG_API bool isPolygonOffset() const;

      /*! \brief Get the polygon offset scale factor.
       *  \return The polygon offset scale factor
       *  \remarks The polygon offset scale factor is used to create a variable depth offset for
       *  each polygon. The default is 0.0.
       *  \sa getPolygonOffsetUnits, isPolygonOffset, setPolygonOffset */
      NVSG_API float getPolygonOffsetFactor() const;

      /*! \brief Get the polygon offset units.
       *  \return The polygon offset units
       *  \remarks The polygon offset units is multiplied by an implementation-specific value to
       *  create a constant depth offset. The default is 0.0.
       *  \sa getPolygonOffsetFactor, isPolygonOffset, setPolygonOffset */
      NVSG_API float getPolygonOffsetUnits() const;

      /*! \brief %Switch polygon offset on or off.
       *  \param onOff  \c true to switch polygon offset on, \c false to switch it off.
       *  \remarks The polygon offset flag specifies how polygon offset is handled. If it is \c
       *  false, no polygon offset is used. Otherwise, each fragment's depth value will be offset
       *  after it is interpolated from the depth values of the appropriate vertices. The offset
       *  is added before the depth test is performed and before the value is written into the
       *  depth buffer. The default is false.
       *  Setting the polygon offset flag increases the nvutil::Incarnation.
       *  \sa isPolygonOffset, setPolygonOffset */
      NVSG_API void setPolygonOffset( bool onOff );

      /*! \brief Set the polygon offset scale factor and units.
       *  \param factor The polygon offset scale factor.
       *  \param units  The polygon offset units.
       *  \remarks The polygon offset scale factor is used to create a variable depth offset for
       *  each polygon. The default is 0.0. The polygon offset units is multiplied by an
       *  implementation-specific value to create a constant depth offset. The default is 0.0.\n
       *  Setting the polygon offset values increases the nvutil::Incarnation.
       *  \sa isPolygonOffset, getPolygonOffsetFactor, getPolygonOffsetUnits */
      NVSG_API void setPolygonOffset( float factor
                                    , float units );

      /*! \brief Query of two-sided lighting is enabled.
       *  \return \c true, if two-sided lighting is enabled, otherwise \c false.
       *  \remarks The two-sided lighting flag specifies whether one or two-sided lighting
       *  calculations are performed for polygons. If it is disabled, one-sided lighting is
       *  specified. Otherwise, two-sided lighting is specified. In that case, vertices of
       *  back-facing polygons are lighted, and have their normals reversed before the lighting
       *  equation is evaluated. Vertices of front-facing polygons are always lighted, with no
       *  change to their normals. The default is false.\n
       *  Setting the two-sided lighting flag increases the nvutil::Incarnation.
       *  \sa setTwoSidedLighting */
      NVSG_API bool isTwoSidedLighting() const;

      /*! \brief %Switch two-sided lighting on or off.
       *  \param onOff \c true to switch two-sided lighting on, \c false to switch it off.
       *  \remarks The two-sided lighting flag specifies whether one or two-sided lighting
       *  calculations are performed for polygons. If it is disabled, one-sided lighting is
       *  specified. Otherwise, two-sided lighting is specified. In that case, vertices of
       *  back-facing polygons are lighted, and have their normals reversed before the lighting
       *  equation is evaluated. Vertices of front-facing polygons are always lighted, with no
       *  change to their normals. The default is false.
       *  \sa isTwoSidedLighting */
      NVSG_API void setTwoSidedLighting( bool onOff );

      /*! \brief Assignment operator
       *  \param rhs A reference to the constant FaceAttribute to copy from
       *  \return A reference to the assigned FaceAttribute
       *  \remarks The assignment operator calls the assignment operator of PrimitiveAttribute and
       *  copies the FaceAttribute specific data then.
       *  \sa PrimitiveAttribute */
      NVSG_API FaceAttribute & operator=( const FaceAttribute & rhs );

      /*! \brief Test for equivalence with another FaceAttribute. 
       *  \param p            A reference to the constant FaceAttribute to test for equivalence with.
       *  \param ignoreNames  Optional parameter to ignore the names of the objects; default is \c true.
       *  \param deepCompare  Optional parameter to perform a deep comparsion; default is \c false.
       *  \return \c true if the FaceAttribute \a p is equivalent to \c this, otherwise \c false.
       *  \remarks If \a p and \c this are equivalent as PrimitiveAttribute, they are equivalent
       *  if they have the same cull mode, face mode, polygon offset settings, and two-sided
       *  lighting setting.
       *  \note The behavior is undefined if \a p is not an FaceAttribute nor derived from one.
       *  \sa PrimitiveAttribute */
      NVSG_API virtual bool isEquivalent( const Object * p
                                        , bool ignoreNames
                                        , bool deepCompare ) const;

    private:
      bool            m_cullMode[2];
      FaceMode        m_faceMode[2];
      struct
      {
        bool  enabled;
        float factor;
        float units;
      }               m_polygonOffset;
      bool            m_twoSidedLighting;
  };

  inline bool FaceAttribute::isFrontFaceCulling() const
  {
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_READ_LOCKED(this);
    return( m_cullMode[0] );
  }

  inline bool FaceAttribute::isBackFaceCulling() const
  {
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_READ_LOCKED(this);
    return( m_cullMode[1] );
  }

  inline void FaceAttribute::setCullMode( bool frontCulling, bool backCulling )
  {
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_WRITE_LOCKED(this);
    m_cullMode[0] = frontCulling;
    m_cullMode[1] = backCulling;
    increaseIncarnation();
  }

  inline FaceMode FaceAttribute::getFrontFaceMode() const
  {
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_READ_LOCKED(this);
    return( m_faceMode[0] );
  }

  inline FaceMode FaceAttribute::getBackFaceMode() const
  {
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_READ_LOCKED(this);
    return( m_faceMode[1] );
  }

  inline void FaceAttribute::setFaceMode( FaceMode frontMode, FaceMode backMode )
  {
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_WRITE_LOCKED(this);
    m_faceMode[0] = frontMode;
    m_faceMode[1] = backMode;
    increaseIncarnation();
  }

  inline void FaceAttribute::setPolygonOffset( bool onOff )
  {
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_READ_LOCKED(this);
    m_polygonOffset.enabled = onOff;
    increaseIncarnation();
  }

  inline void FaceAttribute::setPolygonOffset( float factor, float units )
  {
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_WRITE_LOCKED(this);
    m_polygonOffset.factor = factor;
    m_polygonOffset.units = units;
    increaseIncarnation();
  }

  inline bool FaceAttribute::isPolygonOffset() const
  {
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_READ_LOCKED(this);
    return( m_polygonOffset.enabled );
  }

  inline float FaceAttribute::getPolygonOffsetFactor() const
  {
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_READ_LOCKED(this);
    return( m_polygonOffset.factor );
  } 

  inline float FaceAttribute::getPolygonOffsetUnits() const
  {
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_READ_LOCKED(this);
    return( m_polygonOffset.units );
  } 

  inline void FaceAttribute::setTwoSidedLighting( bool onOff )
  {
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_WRITE_LOCKED(this);
    m_twoSidedLighting = onOff;
    increaseIncarnation();
  }

  inline bool FaceAttribute::isTwoSidedLighting() const
  {
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_READ_LOCKED(this);
    return( m_twoSidedLighting );
  }

} // namespace nvsg
