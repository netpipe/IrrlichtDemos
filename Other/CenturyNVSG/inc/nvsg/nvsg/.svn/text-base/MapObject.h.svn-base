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
#include "nvsg/Object.h"

namespace nvsg
{
  /*! \brief Enumeration to define the alignment of a MapObject holding more than one Map.
   *  \par Namespace: nvsg
   *  \remarks In a MapArray and a MapMatrix, the Map objects can be aligned along the lower base
   *  line (bottom or left), the center line, or the upper base line (top or right). 
   *  \sa MapArray, MapMatrix */
  typedef enum
  {
    MA_LOWER,   //!< draw Map objects aligned at the lower coordinate
    MA_CENTER,  //!< draw Map objects aligned at the center coordinate
    MA_UPPER    //!< draw Map objects aligned at the upper coordinate
  } MapAlignment;

  /*! \brief Enumeration to determine the horizontal or vertical orientation of a MapObject.
   *  \par Namespace: nvsg
   *  \remarks A MapArray can either be oriented horizontally or vertically. A MapMatrix has
   *  settings like alignment or spacing, independently in both orientations.
   *  \sa MapArray, MapMatrix */
  typedef enum
  {
    MO_HORIZONTAL,  //!< use the horizontal orientation
    MO_VERTICAL     //!< use the vertical orientation
  } MapOrientation;

  /*! \brief Enumeration to define the anchor relation of the MapObject position.
   *  \par Namespace: nvsg
   *  \remarks The position of a MapObject describes one of nine possible relative positions, such as
   *  the lower left, the center, or the upper right.
   *  \sa MapObject */
  typedef enum
  {
    MR_LOWER_LEFT,    //!< position is lower left of MapObject
    MR_LOWER_CENTER,  //!< position is lower center of MapObject
    MR_LOWER_RIGHT,   //!< position is lower right of MapObject
    MR_CENTER_LEFT,   //!< position is center left of MapObject
    MR_CENTER,        //!< position is center of MapObject
    MR_CENTER_RIGHT,  //!< position is center right of MapObject
    MR_UPPER_LEFT,    //!< position is upper left of MapObject
    MR_UPPER_CENTER,  //!< position is upper center of MapObject
    MR_UPPER_RIGHT    //!< position is upper right of MapObject
  } MapRelation;

  /*! \brief Enumeration to define the traversal direction of a MapObject holding more than one Map.
   *  \par Namespace: nvsg
   *  \remarks A MapArray can be traversed from the lower to the higher index, or vice versa.\n
   *  A columns of a MapMatrix can also be traversed from the lower to the higher index, or vice
   *  versa; each column of a MapMatrix in turn can be traversed from the lower to the higher
   *  index, or vice versa, but all columns of a MapMatrix must be traversed in the same way.
   *  \sa MapArray, MapMatrix */
  typedef enum
  {
    MTD_LOW_TO_HIGH,  //!< traverse from lowest to highest index
    MTD_HIGH_TO_LOW   //!< traverse from highest to lowest index
  } MapTraversalDirection;

  /*! \brief Abstract base class to position one or more Map objects.
   *  \par Namespace: nvsg
   *  \remarks The position of a MapObject is given in raster coordinates, relative to the lower
   *  left corner of the viewport. The MapRelation determines which point of the MapObject
   *  corresponds to that position.\n
   *  The default position is the origin (0,0) with a MapRelation of MR_LOWER_LEFT.
   *  \sa MapArray, MapElement, MapMatrix */
  class MapObject : public Object
  {
    public:
      /*! \brief Destructs a MapObject. */
      NVSG_API virtual ~MapObject(void);

      /*! \brief Get the raster position
       *  \param x A reference to the x-coordinate to get.
       *  \param y A reference to the y-coordinate to get.
       *  \remarks The position of the MapObject is the raster position relative to the lower left
       *  corner of the viewport. The point of the MapObject which corresponds to the position is
       *  defined by the MapRelation.\n
       *  The default position is the origin (0,0).
       *  \sa getMapRelation, setPosition */
      NVSG_API void getPosition( int & x
                               , int & y ) const;

      /*! \brief Set the raster position.
       *  \param x The x-coordinate of the raster position.
       *  \param y The y-coordinate of the raster position.
       *  \remarks The position of the MapObject is the raster position relative to the lower left
       *  corner of the viewport. The point of the MapObject which corresponds to the position is
       *  defined by the MapRelation.\n
       *  The default position is the origin (0,0).
       *  \sa getPosition, setMapReleation */
      NVSG_API void setPosition( int x
                               , int y );

      /*! \brief Get the relation of the MapObject.
       *  \return The MapRelation of the MapObject.
       *  \remarks The MapRelation describes which point of the MapObject is used as the position.\n
       *  The default MapRelation is MR_LOWER_LEFT.
       *  \par Example
       *  If the MapRelation is MR_LOWER_LEFT, the lower left corner of the MapObject corresponds
       *  to the position; that is, the MapObject is displayed to the upper right of the position.\n
       *  If the MapRelation is MR_UPPER_RIGHT, the upper right corner of the MapObject corresponds
       *  to the position; that is, the MapObject is displayed below and to the left of the
       *  position.
       *  \sa getPosition, setRelation */
      NVSG_API MapRelation getRelation( void ) const;

      /*! \brief Set the relation of the MapObject.
       *  \param relation The MapRelation of the MapObject to set.
       *  \remarks The MapRelation describes which point of the MapObject is used as the position.\n
       *  The default MapRelation is MR_LOWER_LEFT.
       *  \par Example
       *  If the MapRelation is MR_LOWER_LEFT, the lower left corner of the MapObject corresponds
       *  to the position; that is, the MapObject is displayed to the upper right of the position.\n
       *  If the MapRelation is MR_UPPER_RIGHT, the upper right corner of the MapObject corresponds
       *  to the position; that is, the MapObject is displayed below and to the left of the
       *  position.
       *  \sa getRelation, setPosition */
      NVSG_API void setRelation( MapRelation relation );

      /*! \brief Get the lower left position of the MapObject
       *  \param x A reference to the x-coordinate to get.
       *  \param y A reference to the y-coordinate to get.
       *  \remarks The lower left position of the MapObject is calculated from its position,
       *  the MapRelation (describing the relative point of the MapObject corresponding to that
       *  position), and the Map objects with the heights, widths, and spacings.
       *  \par Example
       *  If the MapRelation is MR_LOWER_LEFT (the default), the lower left position equals the
       *  position of the MapObject.\n
       *  If the MapRelation is MR_CENTER, the lower left position is half the width of the
       *  MapObject to the left, and half the height to below it's position.
       *  \sa getPosition, getRelation */
      NVSG_API void getLowerLeft( int & x
                                , int & y ) const;

      /*! \brief Interface to get the overall size of the MapObject.
       *  \param width Reference to the overall width, in pixels, to get.
       *  \param height Reference to the overall height, in pixels, to get.
       *  \remarks If the MapObject just holds one Map, the size is the size of the Map. If it
       *  holds more than one Map and/or does some special alignment or other operations on its
       *  Map objects, the size is the overall size of these Map objects after any alignment or
       *  other operations have been applied to the Map objects.
       *  \sa getLowerLeft, getPosition */
      NVSG_API virtual void getSize( size_t & width
                                   , size_t & height ) const = 0;

      /*! \brief Interface to get a Map at a specified raster position.
       *  \param x Horizontal raster coordinate for the Map to get.
       *  \param y Vertical raster coordinate for the Map to get.
       *  \return A pointer to a MapHandle specifying the Map at the specified raster position, 
       *  or NULL if no Map was found at that position. */
      NVSG_API virtual MapHandle * pickMap( int x
                                          , int y ) const = 0;

    protected:
      /*! \brief Protected default constructor to prevent explicit creation.
       *  \remarks The default setting for a MapObject are:\n
       *    - MapRelation to lower left (MR_LOWER_LEFT)\n
       *    - position at the origin (0,0). */
      NVSG_API MapObject(void);

      /*! \brief Protected copy constructor to prevent explicit creation.
       *  \remarks The MapRelation and position is copied over from \a rhs. */
      NVSG_API MapObject( const MapObject &rhs );

    private:
      MapRelation m_relation; // relation of position
      int         m_x, m_y;   // raster position
  };

  inline void MapObject::getPosition( int & x, int & y ) const
  {
    NVSG_TRACE();
    x = m_x;
    y = m_y;
  }

  inline void MapObject::setPosition( int x, int y )
  {
    NVSG_TRACE();
    m_x = x;
    m_y = y;
  }

  inline MapRelation MapObject::getRelation( void ) const
  {
    NVSG_TRACE();
    return( m_relation );
  }

  inline void MapObject::setRelation( MapRelation relation )
  {
    NVSG_TRACE();
    m_relation = relation;
  }

#if ! defined( DOXYGEN_IGNORE )
  //  no need to document these tiny helpers
  inline bool pointInRect( int x, int y, int llx, int lly, size_t w, size_t h )
  {
    return( ( llx <= x ) && ( lly <= y ) && ( x <= (int)(llx+w) ) && ( y <= (int)(lly+h) ) );
  }

  inline bool pointInInterval( int x, int l, size_t s )
  {
    return( ( l <= x ) && ( x <= (int)(l+s) ) );
  }
#endif

}
