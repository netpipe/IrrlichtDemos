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

#include "nvmath/Plane3f.h"
#include "nvmath/Vec3f.h"
#include "nvsg/Object.h"
#include "nvutil/BitMask.h"

namespace nvsg
{
  // Forward declaration
  class ChangeCallback;

  /*! \brief Helper function to get the position of a brick inside its surrounding brick.
   *  \param pos The position of the surrounding brick.
   *  \param halfSize Half the size of the surrounding brick (and the complete size of the sub brick.
   *  \param brickIndex Index of the brick to determine the position for.
   *  \return The position of the sub brick determined by \a brickIndex inside the surrounding brick
   *  positioned at \a pos.
   *  \remarks  The \a brickIndex is between 0 and 7, where 0 is the lower left back, and 7 is the
   *  upper right front. The three bits used in \a brickIndex determine the sub brick to be lower or
   *  upper (bit 0), right or left (bit 1), and back and front (bit 2). */
  inline nvmath::Vec3f getSubBrickPosition( const nvmath::Vec3f & pos
                                          , const nvmath::Vec3f & halfSize
                                          , unsigned int brickIndex )
  {
    return( nvmath::Vec3f( pos[0] + nvutil::bit(0,brickIndex) * halfSize[0]
                         , pos[1] + nvutil::bit(1,brickIndex) * halfSize[1]
                         , pos[2] + nvutil::bit(2,brickIndex) * halfSize[2] ) );
  }

  /*! \brief Class to represent one brick of a volume.
   *  \par Namespace: nvsg
   *  \remarks A VolumeBrick is part of a VolumeNode. Each VolumeNode holds one single VolumeBrick,
   *  that represents the complete volume in the lowest available resolution. Each VolumeBrick in
   *  turn can hold up to eight sub bricks, each one again a VolumeBrick. Each of those VolumeBricks
   *  holds one octel of the surrounding VolumeBrick, now in a resolution twice as high. If the
   *  VolumeBrick holds no sub bricks, it represents that part of the complete volume in the highest
   *  available resolution. If the VolumeBrick holds only some sub bricks, the sub bricks not hold
   *  are plain empty space in the complete volume. 
   *  \sa VolumeNode */
  class VolumeBrick : public nvutil::HandledObject
  {
    public:
      /*! \brief Default constructor of a VolumeBrick. */
      NVSG_API VolumeBrick();

      /*! \brief Destructor of a VolumeBrick. */
      NVSG_API virtual ~VolumeBrick();

      /*! \brief Get the geometric representation of this VolumeBrick.
       *  \return A pointer to the QuadsHandle describing a box containing the volume dataset.
       *  \remarks The geometric representation of a VolumeBrick is a box. It's position and size
       *  is completely determines by the represented volume texture.
       *  \sa getFrontPlane */
      NVSG_API QuadsHandle * getBrickBox() const;

      /*! \brief Get the TextureImage representing the volume data inside that VolumeBrick.
       *  \return A pointer to the TextureImageHandle holding the volume data.
       *  \remarks Each VolumeBrick holds a TextureImage representing the volume data. That
       *  TextureImage is either a down-sampled version of that part of the complete volume that is
       *  represented by that VolumeBrick. In that case, the VolumeBrick does have at least one sub
       *  brick. Or that TextureImage is just that part of the complete volume with the exact same
       *  resolution as the complete volume, that is represented by that VolumeBrick. In that case,
       *  it is a "leaf" in the octree of VolumeBricks and holds no more sub bricks. */
      NVSG_API TextureImageHandle * getBrickData() const;

      /*! \brief Get the geometric representation of the front plane.
       *  \return A pointer to a TriFansHandle if the front clip plane cuts the brick box, or NULL.
       *  \remarks When the front clipping plane of the view frustum clips the brick box, no
       *  fragments would be generated in that area. Therefore, a geometric representation of that
       *  area is created as a TriFans, slightly inside the view frustum. That TriFans can be used
       *  to create fragments inside of the VolumeBrick. If this function returns NULL, there is no
       *  front plane, because the front clip plane does not intersect the brick box.
       *  \sa getBrickBox, setFrontClipPlane */
      NVSG_API TriFansHandle * getFrontPlane() const;

      /*! \brief Get the visual appearance of the VolumeBrick.
       *  \return A pointer to a StateVariantHandle, describing the visual appearance of the brick.
       *  \remarks Each VolumeBrick holds a StateVariant that in turn holds a CgFx, that describes
       *  the volume and the rendering of this brick. */
      NVSG_API StateVariantHandle * getStateVariant() const;

      /*! \brief Determine if this VolumeBrick contains further sub bricks.
       *  \return \c false, if this VolumeBrick contains further sub bricks, otherwise \c true.
       *  \remarks A VolumeBrick is called a "leaf", if it does not hold any sub bricks, i.e. the
       *  volume texture it holds is in the highest resolution available for that part of the
       *  volume. */
      NVSG_API bool isLeaf() const;

      /*! \brief Get one sub brick of this VolumeBrick.
       *  \param index The index of the sub brick to get.
       *  \return A pointer to the VolumeBrick representing the sub volume determined by \a index,
       *  or NULL, if there is none.
       *  \remarks  The \a brickIndex is between 0 and 7, where 0 is the lower left back, and 7 is
       *  the upper right front. The tree bits used in \a brickIndex determine the sub brick to be
       *  lower or upper (bit 0), right or left (bit 1), and back and front (bit 2).
       *  \note The behavior is undefined if \a index is larger than seven.
       *  \sa getSubBrickPosition */
      NVSG_API VolumeBrickHandle * operator[]( unsigned int index ) const;

      /*! \brief Set all the data of the VolumeBrick.
       *  \param brickName The name of the texture file containing the volume data for this brick.
       *  \param volumeSize The size of the complete volume texture.
       *  \param brickPosition The position of this brick in the complete volume.
       *  \param brickSize The size of the brick.
       *  \remarks The \a brickName is used to create the file names of the sub bricks by appending
       *  an index between 0 and 7 for each possible sub brick. If a file with that name exists, it
       *  is used to create and set the corresponding sub brick. */
      NVSG_API void setData( const std::string & brickName
                           , const nvmath::Vec3f & volumeSize
                           , const nvmath::Vec3f & brickPosition
                           , const nvmath::Vec3f & brickSize );

      /*! \brief Set the front clip plane.
       *  \param plane The definition of a plane, representing the front clip plane.
       *  \remarks If the geometric representation of this VolumeBrick intersects the clip plane
       *  \a plane, a TriFans representing that intersection is created, and the clip plane of each
       *  sub brick is determined recursively. If otherwise \a plane does not intersect the
       *  geometric representation of this VolumeBrick, any previously determined clip plane
       *  representation is removed, and recursively in each sub brick.
       *  \sa getFrontPlane */
      NVSG_API void setFrontClipPlane( const nvmath::Plane3f &plane );

      /*! \brief Set the master shader for this VolumeBrick.
       *  \param shader A pointer to the master shader to use.
       *  \remarks A VolumeNode holds a CgFx that defines the visual appearance of the volume. That
       *  shader is used as the master shader for the active VolumeBricks. Each VolumeBrick creates
       *  a clone out of the master shader, and adjusts some parameters, as size, position, and
       *  texture file. The shader of the VolumeNode is called "master shader" here, because every
       *  change in that shader is automatically carried into the clones held by the VolumeBricks. */
      NVSG_API void setMasterShader( CgFxHandle * shader );
      
    private:
      NVSG_API void enterCS() const;
      NVSG_API void leaveCS() const;
      NVSG_API void removeFrontClipPlane();

    private:
      QuadsHandle                 * m_brickBox;
      std::string                   m_brickName;
      nvmath::Vec3f                 m_brickPosition;
      nvmath::Vec3f                 m_brickSize;
      TriFansHandle               * m_frontPlane;           // proxy geometry in case of front plane clipping
      nvutil::SWMRSync              m_lock;                 // helps managing exclusive access to mutable members while reading the object 
      CgFxHandle                  * m_masterShader;         // The master (VolumeNode's) shader from which the brick shader will be cloned
      VolumeBrickHandle           * m_octels[8];
      nvmath::Vec3f                 m_volumeSize;

      mutable TextureImageHandle  * m_brickData;
      mutable ChangeCallback      * m_ChangeCallBack;       // The CallBack responsible for updating the Brick's shader with the (possibly changed) master shader
      mutable CgFxHandle          * m_shader;               // The shader for the brick
      mutable StateVariantHandle  * m_stateVariant;
  };

  inline QuadsHandle * VolumeBrick::getBrickBox() const
  {
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_READ_LOCKED(this);
    return( m_brickBox );
  }

  inline TriFansHandle * VolumeBrick::getFrontPlane() const
  {
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_READ_LOCKED(this);
    return( m_frontPlane );
  }

  inline bool VolumeBrick::isLeaf() const
  {
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_READ_LOCKED(this);
    return( !(    m_octels[0] || m_octels[1] || m_octels[2] || m_octels[3]
              ||  m_octels[4] || m_octels[5] || m_octels[6] || m_octels[7] ) );
  }

  inline VolumeBrickHandle * VolumeBrick::operator[]( unsigned int index ) const
  {
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_READ_LOCKED(this);
    NVSG_ASSERT( index < 8 );
    return( m_octels[index] );
  }
  
  inline void VolumeBrick::enterCS() const
  {
    NVSG_TRACE();
    m_lock.lockExclusive();
  }

  inline void VolumeBrick::leaveCS() const
  {
    NVSG_TRACE();
    m_lock.unlockExclusive();
  }

}