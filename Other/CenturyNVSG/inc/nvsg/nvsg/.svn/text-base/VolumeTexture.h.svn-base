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
#include "nvmath/Vec3f.h"
#include "nvsg/CoreTypes.h"
#include "nvsg/TextureImage.h"

namespace nvsg
{
  /*! \brief A class representing a VolumeTexture.
   *  \remarks A VolumeTexture is a TextureImage with some additional informations, like slice
   *  thickness.
   *  \sa TextureImage */
  class VolumeTexture : public TextureImage
  {
    public:
      /*! \brief Default constructor.
       *  \remarks After the TextureImage default constructor returned, the slice thickness is
       *  initialized to one, for each direction. */
      NVSG_API VolumeTexture();

      /*! \brief Virtual destructor of a VolumeTexture. */
      NVSG_API virtual ~VolumeTexture();

      /*! \brief Get the size of the overlap in voxels.
      *  \return The size of the overlap in voxels.
      *  \sa setOverlapSize */
      NVSG_API unsigned int getOverlapSize() const;

      /*! \brief Get the slice thickness of this VolumeNode.
       *  \return A Vec3f representing the slice thicknesses in the x-, y-, and z-direction.
       *  \remarks Some volume textures have voxels that are not cubes, but cuboids. The slice
       *  thickness is used to scale them accordingly.
       *  \sa setSliceThickness */
      NVSG_API nvmath::Vec3f getSliceThickness() const;

      /*! \brief Set the size of the overlap in voxels.
       *  \param overlap The size of the overlap in voxels.
       *  \sa getOverlapSize */
      NVSG_API void setOverlapSize( unsigned int overlap );

      /*! \brief Set the slice thickness of this VolumeNode.
       *  \param thickness A Vec3f representing the slice thicknesses in the x-, y-, and z-direction.
       *  \remarks Some volume textures have voxels that are not cubes, but cuboids. The slice
       *  thickness is used to scale them accordingly.
       *  \sa getSliceThickness */
      NVSG_API void setSliceThickness( const nvmath::Vec3f & thickness );

    private:
      unsigned int  m_overlap;          //!< overlap of the texture in voxels
      nvmath::Vec3f m_sliceThickness;
  };

  inline unsigned int VolumeTexture::getOverlapSize() const
  {
    NVSG_TRACE();
    return( m_overlap );
  }

  inline nvmath::Vec3f VolumeTexture::getSliceThickness() const
  {
    NVSG_TRACE();
    return( m_sliceThickness );
  }

  inline void VolumeTexture::setOverlapSize( unsigned int overlap )
  {
    NVSG_TRACE();
    m_overlap = overlap;
  }

  inline void VolumeTexture::setSliceThickness( const nvmath::Vec3f & thickness )
  {
    NVSG_TRACE();
    m_sliceThickness = thickness;
  }
}