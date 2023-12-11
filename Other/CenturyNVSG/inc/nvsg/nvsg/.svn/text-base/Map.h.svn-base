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

namespace nvsg
{
  /*! \brief Abstract base class for Map objects.
   *  \par Namespace: nvsg
   *  \remarks A Map is a two dimensional image that can be used as some kind of overlay on top of
   *  the scene image. It's easy to use for on-screen menus or data display.\n
   *  The map data is organized by rows, each one <TT>(width*bpp+7)/8</TT> byte long, beginning at
   *  the bottom left corner of the map.
   *  \note There is no default constructor available to prevent incompletely-defined Maps.
   *  \sa Bitmap, Object */
  class Map : public Object
  {
    public:
      /*! \brief Destructs a Map. */
      NVSG_API virtual ~Map();

      /*! \brief Get the width of the Map in pixels.
       *  \return The width of the Map in pixels.
       *  \sa getHeight */
      NVSG_API size_t getWidth() const;

      /*! \brief Get the height of the Map in pixels.
       *  \return The height of the Map in pixels.
       *  \sa getWidth */
      NVSG_API size_t getHeight() const;

      /*! \brief Get a pointer to the constant data of the Map.
       *  \return A pointer to the constant data of the Map.
       *  \remarks The map data is organized by rows, each one <TT>(width*bpp+7)/8</TT> byte long,
       *  beginning at the bottom left corner of the map.
       *  \sa getHeight, getWidth */
      NVSG_API const unsigned char * getData() const;

    protected:
      /*! \brief Protected default constructor to prevent explicit creation. */
      NVSG_API Map();

      /*! \brief Protected copy constructor to prevent explicit creation.
       *  \remarks The complete map data is copied over from \a rhs. */
      NVSG_API Map( const Map &rhs );

      /*! \brief Set the complete data of the Map.
       *  \param width The width of the Map in pixels.
       *  \param height The height of the Map in pixels.
       *  \param bitsPerPixel The number of bits used per pixel.
       *  \param data A pointer to the constant data of the Map.
       *  \remarks This function allocates <TT>height*((width*bpp+7)/8)</TT> bytes to hold the map
       *  data and copies as many bytes from \a data. */
      NVSG_API void setMapData( size_t width
                              , size_t height
                              , size_t bitsPerPixel
                              , unsigned char * data );

    private:
      size_t          m_width, m_height;
      size_t          m_bpp;  // bits per pixel
      unsigned char * m_data;
  };

  inline size_t Map::getWidth() const
  {
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_READ_LOCKED(this);
    return( m_width );
  }

  inline size_t Map::getHeight() const
  {
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_READ_LOCKED(this);
    return( m_height );
  }

  inline const unsigned char * Map::getData() const
  {
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_READ_LOCKED(this);
    return( m_data );
  }
}
