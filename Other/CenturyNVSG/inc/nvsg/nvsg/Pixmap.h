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
#include "nvsg/Map.h"

namespace nvsg
{
#if 0
  //! Class to hold a Map with more than one bit per pixel.
  class Pixmap : public Map
  {
      //! Create a Pixmap.
      /** \returns a constant pointer to a Pixmap. */
      NVSG_API static const Pixmap * create( size_t width, size_t height, PixelFormat format, PixelDataType type, unsigned char * data );

      //! Get a clone of this Pixmap object
      /** \returns A constant pointer to a Pixmap. */
      NVSG_API virtual const Pixmap * clone( void ) const;

    protected:
      //! Constructor
      NVSG_API Pixmap( size_t width, size_t height, PixelFormat format, PixelDataType type, unsigned char * data );

      //! Copy Constructor
      NVSG_API Pixmap( const Pixmap &rhs );

      //! Protected destructor to prevent explicit creation on stack.
      NVSG_API virtual ~Pixmap(void);

    private:
      Pixmap( void );

    private:
      PixelFormat   m_format;
      PixelDataType m_type;
  };
#endif
}