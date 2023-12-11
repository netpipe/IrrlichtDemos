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
#include "nvmath/Vec4f.h"
#include "nvsg/Map.h"

namespace nvsg
{
  /*! \brief A Map with one bit per pixel
   *  \par Namespace: nvsg
   *  \remarks A Bitmap can be used as a Map, i.e. for on-screen menus or display of data. A one(1) bit
   *  represents one pixel in the specified color, all zero(0) bits represent a transparent pixel.
   *  \sa Map, MapElement */
  class Bitmap : public Map
  {
    public:
      /*! \brief Constructor of an empty Bitmap.
       *  \remarks After construction, the Bitmap is empty and needs to be filled with
       *  setBitmapData(). The color initially is set to opaque white. */
      NVSG_API Bitmap();

      /*! \brief Copy-constructs a Bitmap from another Bitmap. 
       *  \param rhs Source Bitmap. */
      NVSG_API Bitmap( const Bitmap &rhs );

      /*! \brief Destructs a Bitmap. */
      NVSG_API virtual ~Bitmap(void);

      /*! \brief Set the complete data of the Bitmap.
       *  \param width The width of the Bitmap in pixels.
       *  \param height The height of the Bitmap in pixels.
       *  \param data A pointer to the constant data of the Bitmap.
       *  \remarks This function allocates <TT>height*((width+7)/8)</TT> bytes to hold the Bitmap
       *  data and copies as many bytes from \a data. */
      NVSG_API void setBitmapData( size_t width, size_t height, unsigned char * data );

      /*! \brief Get the color of the 1-bits in the Bitmap.
       *  \return A reference to the constant color of the Bitmap. The default color is opaque
       *  white (1.0,1.0,1.0,1.0). */
      NVSG_API const nvmath::Vec4f & getColor( void ) const;

      /*! \brief Set the color of the 1-bits in the Bitmap.
       *  \param color A reference to the constant color of the Bitmap.
       *  \remarks This color is used for every 1-bits of the Bitmap. The 0-bits are completely
       *  transparent. The default color is opaque white (1.0,1.0,1.0,1.0). */
      NVSG_API void setColor( const nvmath::Vec4f &color );

     private:
      nvmath::Vec4f m_color;
  };

  inline const nvmath::Vec4f & Bitmap::getColor( void ) const
  {
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_READ_LOCKED(this);
    return( m_color );
  }

  inline void Bitmap::setBitmapData( size_t width, size_t height, unsigned char * data )
  {
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_WRITE_LOCKED(this);
    setMapData( width, height, 1, data );
  }

  inline void Bitmap::setColor( const nvmath::Vec4f &color )
  {
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_WRITE_LOCKED(this);
    m_color = color;
  }
}
