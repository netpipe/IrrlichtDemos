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
#include "nvsg/MapObject.h"

namespace nvsg
{
  /*! \brief Class to hold a single Map.
   *  \par Namespace: nvsg
   *  \remarks This class is used to add a single Map to a MapList.
   *  \sa Map, MapArray, MapList, MapMatrix */
  class MapElement : public MapObject
  {
    public:
      /*! \brief Default constructor of a MapElement.
       *  \remarks The default constructor creates an empty MapElement without associated Map.
       *  To set the Map use setMap(). */
      NVSG_API MapElement();

      /*! \brief Copy-constructs a MapElement.
       *  \param rhs Source MapElement. */
      NVSG_API MapElement( const MapElement &rhs );

      /*! \brief Destructs a MapElement. */
      NVSG_API virtual ~MapElement();

      /*! \brief Get a pointer to the constant Map.
       *  \return A pointer to the constant Map of this MapElement.
       *  \sa pickMap */
      NVSG_API MapHandle * getMap() const;

      /*! \brief Get the Map being at the specified raster position.
       *  \param x Horizontal raster coordinate to get Map at.
       *  \param y Vertical raster coordinate to get Map at.
       *  \return A pointer to a MapHandle specifying the Map at the specified raster position, 
       *  or NULL if no Map was found at that position.
       *  \remarks This functions determines if the Map of this MapElement is at the specified
       *  raster position (\a x, \a y). If it is, a pointer to the constant Map is returned,
       *  otherwise NULL.
       *  \sa getMap */
      NVSG_API virtual MapHandle * pickMap( int x, int y ) const;

      /*! \brief Set the Map of the MapElement.
       *  \param m A pointer to a MapHandle specifying the Map to be used by this MapElement. */
      NVSG_API void setMap( MapHandle *m );

      /*! \brief Get the overall size of the MapElement.
       *  \param width Reference to the overall width, in pixels, to get.
       *  \param height Reference to the overall height, in pixels, to get.
       *  \remarks The overall size of the MapElement is the size, in pixels, of it's Map. */
      NVSG_API virtual void getSize( size_t &width, size_t &height ) const;

    protected:
      /*! \brief Validate the Incarnation. 
       *  \remarks This function is called from the framework when the Incarnation is requested
       *  and it is marked as invalid. Then it is validated and the Incarnation of its Map is
       *  requested to continue validation down the tree hierarchy.
       *  \a Incarnation */
      NVSG_API virtual void validateIncarnation() const;  // from top to bottom

    private:
      MapHandle * m_map;
  };

  inline MapHandle * MapElement::getMap() const
  {
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_READ_LOCKED(this);
    return( m_map );
  }
}
