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
#include <vector>

namespace nvsg
{
  /*! \brief Class to hold a matrix of Map objects.
   *  \par Namespace: nvsg
   *  \remarks This class is used to add a matrix of Map objects to a MapList. Each Map is
   *  positioned in a raster field, with each field as large as the largest Map in the MapMatrix.
   *  The Map objects can be aligned along a common base, a center, or a top line. The traversal
   *  direction can be from the lower to the higher index, or vice versa. A spacing can be
   *  determined to be between each of the Map objects.\n
   *  The MapMatrix is organized as a row of columns, where each column can have a different number
   *  of entries.
   *  \sa Map, MapElement, MapList, MapMatrix */
  class MapMatrix : public MapObject
  {
    public:
      /*! \brief Default-constructs a MapMatrix.
       *  \remarks The MapMatrix initially is aligned to the lower base line (MA_LOWER),
       *  has a spacing of 1 pixel, and will be traversed from low index to high index 
       * (MTD_LOW_TO_HIGH). */
      NVSG_API MapMatrix( void );

      /*! \brief Copy-constructs a MapMatrix.
       *  \param rhs Source MapMatrix. */
      NVSG_API MapMatrix( const MapMatrix &rhs );

      /*! \brief Destructs a MapMatrix. */
      NVSG_API virtual ~MapMatrix(void);

      /*! \brief Get the alignment of the MapMatrix along the \a orientation.
       *  \param orientation The MapOrientation to get the alignment for.
       *  \return The MapAlignment of the Map objects in the MapOrienation \a orientation.
       *  \remarks If the Map objects of the MapMatrix have different sizes, they can be aligned
       *  along their lower (MA_LOWER), center (MA_CENTER), or upper (MA_UPPER) coordinate. The
       *  default is MA_LOWER.
       *  \par Example
       *  If the Map objects have different heights, they can have a common base line (MA_LOWER), a
       *  common center line (MA_CENTER), or a common top line (MA_UPPER).\n
       *  If the Map objects have different widths, they can have a common left margin (MA_LOWER),
       *  a common center line (MA_CENTER), or a common right margin (MA_UPPER).
       *  \sa setAlignment */
      NVSG_API MapAlignment getAlignment( MapOrientation orientation ) const;

      /*! \brief Set the alignment of the MapMatrix along the \a orientation.
       *  \param orientation The MapOrientation along which to set the alignment.
       *  \param alignment The MapAlignment of the Map objects along the MapOrientation \a
       *  orientation.
       *  \remarks If the Map objects of the MapMatrix have different sizes, they can be aligned
       *  along their lower (MA_LOWER), center (MA_CENTER), or upper (MA_UPPER) coordinate. The
       *  default is MA_LOWER.
       *  \par Example
       *  If the Map objects have different heights, they can have a common base line (MA_LOWER), a
       *  common center line (MA_CENTER), or a common top line (MA_UPPER).\n
       *  If the Map objects have different widths, they can have a common left margin (MA_LOWER),
       *  a common center line (MA_CENTER), or a common right margin (MA_UPPER).
       *  \sa getAlignment */
      NVSG_API void setAlignment( MapOrientation orientation
                                , MapAlignment alignment );

      /*! \brief Get the spacing between consecutive Map objects along the \a orientation.
       *  \param orientation The MapOrientation to use to get the spacing.
       *  \return The spacing between consecutive Map objects in the MapOrientation \a orientation.
       *  \remarks Between each Map of the MapMatrix, there can be some open space left. Each
       *  direction can have a different spacing, but between each Map, then, the spacing is
       *  equal. The default is one in both directions.
       *  \sa setSpacing */
      NVSG_API size_t getSpacing( MapOrientation orientation ) const;

      /*! \brief Set the spacing between consecutive Map objects.
       *  \param orientation The MapOrientation to use when setting the spacing.
       *  \param spacing The spacing between consecutive Map objects.
       *  \remarks Between each Map of the MapMatrix, there can be some open space left. Each
       *  direction can have a different spacing, but between each Map, then, the spacing is 
       *  equal. The default is one in both directions.
       *  \sa getSpacing */
      NVSG_API void setSpacing( MapOrientation orientation
                              , size_t spacing );

      /*! \brief Get the traversal direction of the MapMatrix along the \a orientation.
       *  \param orientation The MapOrientation to use for getting the traversal direction.
       *  \return The MapTraversalDirection of the MapMatrix in the MapOrientation \a orientation.
       *  \remarks The Map objects of the MapMatrix can be traversed either from lower to higher
       *  index (MTD_LOW_TO_HIGH) or from higher to lower index (MTD_HIGH_TO_LOW), independently in
       *  both orientations. The default is MTD_LOW_TO_HIGH in both orientations.
       *  \par Example
       *  If the traversal direction for the \a orientation MO_VERTICAL is MTD_LOW_TO_HIGH, in
       *  each column of the MapMatrix, the Map with index 0 is drawn at the bottom, the Map with
       *  index 1 is drawn above, and so on.
       *  \sa getAlignment, setTraversalDirection */
      NVSG_API MapTraversalDirection getTraversalDirection( MapOrientation orientation ) const;

      /*! \brief Set the traversal direction of the MapMatrix along the \a orientation.
       *  \param orientation The MapOrientation to use for setting the traversal direction.
       *  \param direction The MapTraversalDirection of the MapMatrix in the MapOrientation \a
       *  orientation. 
       *  \remarks The Map objects of the MapMatrix can be traversed either from lower to higher
       *  index (MTD_LOW_TO_HIGH) or from higher to lower index (MTD_HIGH_TO_LOW), independently in
       *  both orientations. The default is MTD_LOW_TO_HIGH in both orientations.
       *  \par Example
       *  If the traversal direction for the \a orientation MO_VERTICAL is MTD_LOW_TO_HIGH, in
       *  each column of the MapMatrix, the Map with index 0 is drawn at the bottom, the Map with
       *  index 1 is drawn above, and so on.
       *  \sa getTraversalDirection, setAlignment */
      NVSG_API void setTraversalDirection( MapOrientation orientation
                                         , MapTraversalDirection direction );

      /*! \brief Get the number of columns of Map objects.
       *  \return The number of columns of Map objects.
       *  \sa addEmptyColumn, insertEmptyColumn, removeColumn */
      NVSG_API size_t getNumberOfColumns( void ) const;

      /*! \brief Add an empty column of Map objects as the last column.
       *  \sa addMap, getNumberOfColumns, insertEmptyColumn, removeColumn*/
      NVSG_API void addEmptyColumn( void );

      /*! \brief Insert an empty column of Map objects in front of column \a col.
       *  \param col Index to insert an empty column of Map objects before.
       *  \note The behavior is undefined if \a col is larger than or equal to the current number
       *  of columns.
       *  \sa addEmptyColumn, addMap, getNumberOfColumns, removeColumn */
      NVSG_API void insertEmptyColumn( size_t col );

      /*! \brief Remove the column \a col of Map objects.
       *  \param col Index of column of Map objects to remove.
       *  \note The behavior is undefined if \a col is larger than or equal to the current number
       *  of columns.
       *  \sa addEmptyColumn, getNumberOfColumns, insertEmptyColumn */
      NVSG_API void removeColumn( size_t col );

      /*! \brief Get the number of Map objects in the column \a col.
       *  \param col Index of the column to use to get the number of Map objects.
       *  \return The number of Map objects in the column \a col.
       *  \sa addMap, getMap, getNumberOfColumns, insertMap */
      NVSG_API size_t getNumberOfMaps( size_t col ) const;

      /*! \brief Get a pointer to the constant Map at the specified position.
       *  \param col Index of the column.
       *  \param row Index of the row in column \a col.
       *  \return A pointer to the constant Map object at the specified position.
       *  \note The behavior is undefined if \a col is larger than or equal to the number of
       *  columns, or if \a row is larger than or equal to the number of Map objects in the column
       *  \a col.
       *  \sa addMap, getIndexOfMap, getNumberOfColumns, getNumberOfMaps, pickMap */
      NVSG_API MapHandle * getMap( size_t col
                                 , size_t row ) const;

      /*! \brief Get the index of a Map in this MapMatrix.
       *  \param map A pointer to the constant Map to use for getting the index.
       *  \param col A reference to get the column, where \a map is in.
       *  \param row A reference to get the row, where \a map is in.
       *  \return \c true, if \a map is part of this MapMatrix, otherwise \c false.
       *  \sa getMap, getNumberOfColumns, getNumberOfMaps, pickMap */
      NVSG_API bool getIndexOfMap( MapHandle * map
                                 , size_t & col
                                 , size_t & row ) const;

      /*! \brief Add a Map at the end of the column \a col.
       *  \param col Index of the column to add the Map to.
       *  \param map A pointer to the constant Map to add.
       *  \remarks The reference count of \a map is increased.
       *  \note The behavior is undefined if \a col is larger than or equal to the number of
       *  columns.
       *  \sa getNumberOfColumns, getNumberOfMaps, insertMap, removeMap, replaceMap */
      NVSG_API void addMap( size_t col
                          , MapHandle * map );

      /*! \brief Insert a Map in the column \a col in front of index \a row.
       *  \param col Index of the column to insert the Map into.
       *  \param row Index of the row in the column \a col to insert the Map before.
       *  \param map A pointer to the constant Map to insert.
       *  \remarks The reference count of \a map is increased.
       *  \note The behavior is undefined if \a col is larger than or equal to the number of
       *  columns, or if \a row is larger than or equal to the number of Map objects in the column
       *  \a col.
       *  \sa addMap, getNumberOfColumns, getNumberOfMaps, removeMap, replaceMap */
      NVSG_API void insertMap( size_t col
                             , size_t row
                             , MapHandle * map );

      /*! \brief Replace the Map in the column \a col at index \a row.
       *  \param col Index of the column to replace the Map in.
       *  \param row Index of the row in the column \a col to replace the Map at.
       *  \param map A pointer to the constant Map to replace with.
       *  \remarks The reference count of \a map is incremented, and the reference count of the Map
       *  previously at column \a col, row \a row, is decremented.
       *  \note The behavior is undefined if \a col is larger than or equal to the number of
       *  columns, or if \a row is larger than or equal to the number of Map objects in the column
       *  \a col.
       *  \sa addMap, getNumberOfColumns, getNumberOfMaps, insertMap, removeMap */
      NVSG_API void replaceMap( size_t col
                              , size_t row
                              , MapHandle * map );

      /*! \brief Remove the Map in the column \a col at index \a row.
       *  \param col Index of the column to remove the Map in.
       *  \param row Index of the row in the column \a col to remove the Map at.
       *  \note The behavior is undefined if \a col is larger than or equal to the number of
       *  columns, or if \a row is larger than or equal to the number of Map objects in the column
       *  \a col.
       *  \sa addMap, getNumberOfColumns, getNumberOfMaps, insertMap, replaceMap */
      NVSG_API void removeMap( size_t col
                             , size_t row );

      /*! \brief Get the maximal width and height of the Map objects.
       *  \param width Reference to the maximal width, in pixels, to get.
       *  \param height Reference to the maximal height, in pixels, to get.
       *  \remarks The entry size is the width and height of the matrix cells, in pixels, where
       *  each Map fits in. It is the maximal width and maximal height of all the Map objects.
       *  \sa getSize */
      NVSG_API void getEntrySize( size_t &width
                                , size_t &height ) const;

      /*! \brief Get the overall size of the MapMatrix.
       *  \param width Reference to the overall width, in pixels, to get.
       *  \param height Reference to the overall height, in pixels, to get.
       *  \remarks The overall size of a MapMatrix is the size, in pixels, the complete MapMatrix
       *  has. The overall width is the maximal width of the Map objects times the number of
       *  columns, plus the horizontal spacing between each of these. The overall height is the
       *  maximal height of the Map objects times the maximal number of rows of all columns, plus
       *  the vertical spacing between each of these.
       *  \sa getEntrySize, getNumberOfColumns, getNumberOfMaps */
      NVSG_API virtual void getSize( size_t &width
                                   , size_t &height ) const;

      /*! \brief Get the Map located at the specified raster position.
       *  \param x Horizontal raster coordinate to use for getting the Map.
       *  \param y Vertical raster coordinate to use for getting the Map.
       *  \return A pointer to the constant Map located at the specified raster position, or NULL if
       *  there is none.
       *  \remarks This function determines the Map of this MapMatrix at the specified raster
       *  position (\a x, \a y), if any. It takes into account the position, alignment, direction,
       *  and spacing. Each Map is considered to be the size of the largest Map in this MapMatrix.
       *  If the position is in the spacing between two Map objects, or outside of the overall size
       *  of the MapMatrix, NULL is returned.
       *  \sa getEntrySize, getSize, getMap */
      NVSG_API virtual MapHandle * pickMap( int x
                                          , int y ) const;

    protected:
      /*! \brief Validate the Incarnation. 
       *  \remarks This function is called from the framework when the Incarnation is requested
       *  and it is marked as invalid. Then it is validated and the Incarnation of all its Map
       *  objects is requested to continue validation down the tree hierarchy.
       *  \a Incarnation */
      NVSG_API virtual void validateIncarnation( void ) const;  // from top to bottom

    private:
      MapAlignment                            m_alignment[2];
      MapTraversalDirection                   m_direction[2];
      std::vector<std::vector<MapHandle *> >  m_maps;
      size_t                                  m_spacing[2];
  };

  inline MapAlignment MapMatrix::getAlignment( MapOrientation orientation ) const
  {
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_READ_LOCKED(this);
    return( m_alignment[orientation] );
  }

  inline void MapMatrix::setAlignment( MapOrientation orientation, MapAlignment alignment )
  {
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_WRITE_LOCKED(this);
    m_alignment[orientation] = alignment;
  }

  inline MapTraversalDirection MapMatrix::getTraversalDirection( MapOrientation orientation ) const
  {
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_READ_LOCKED(this);
    return( m_direction[orientation] );
  }

  inline void MapMatrix::setTraversalDirection( MapOrientation orientation, MapTraversalDirection direction )
  {
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_WRITE_LOCKED(this);
    m_direction[orientation] = direction;
  }

  inline size_t MapMatrix::getSpacing( MapOrientation orientation ) const
  {
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_READ_LOCKED(this);
    return( m_spacing[orientation] );
  }

  inline void MapMatrix::setSpacing( MapOrientation orientation, size_t spacing )
  {
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_WRITE_LOCKED(this);
    m_spacing[orientation] = spacing;
  }

  inline void MapMatrix::addEmptyColumn( void )
  {
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_WRITE_LOCKED(this);
    m_maps.resize( m_maps.size() + 1 );
  }

  inline void MapMatrix::insertEmptyColumn( size_t col )
  {
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_WRITE_LOCKED(this);
    NVSG_ASSERT( col < m_maps.size() );
    m_maps.insert( m_maps.begin() + col, std::vector<MapHandle *>() );
  }

  inline void MapMatrix::addMap( size_t col, MapHandle * map )
  {
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_WRITE_LOCKED(this);
    NVSG_ASSERT( map && ( col < m_maps.size() ) );
    map->addRef();
    m_maps[col].push_back( map );
  }

  inline void MapMatrix::insertMap( size_t col, size_t row, MapHandle * map )
  {
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_WRITE_LOCKED(this);
    NVSG_ASSERT( map && ( col < m_maps.size() ) && ( row < m_maps[col].size() ) );
    map->addRef();
    m_maps[col].insert( m_maps[col].begin() + row, map );
  }

  inline void MapMatrix::replaceMap( size_t col, size_t row, MapHandle * map )
  {
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_WRITE_LOCKED(this);
    NVSG_ASSERT( map && ( col < m_maps.size() ) && ( row < m_maps[col].size() ) );
    map->addRef();
    m_maps[col][row]->removeRef();
    m_maps[col][row] = map;
  }

  inline void MapMatrix::removeMap( size_t col, size_t row )
  {
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_WRITE_LOCKED(this);
    NVSG_ASSERT( ( col < m_maps.size() ) && ( row < m_maps[col].size() ) );
    m_maps[col][row]->removeRef();
    m_maps[col].erase( m_maps[col].begin() + row );
  }

  inline size_t MapMatrix::getNumberOfColumns( void ) const
  {
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_READ_LOCKED(this);
    return( m_maps.size() );
  }

  inline size_t MapMatrix::getNumberOfMaps( size_t col ) const
  {
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_READ_LOCKED(this);
    NVSG_ASSERT( col < m_maps.size() );
    return( m_maps[col].size() );
  }

  inline MapHandle * MapMatrix::getMap( size_t col, size_t row ) const
  {
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_READ_LOCKED(this);
    NVSG_ASSERT( ( col < m_maps.size() ) && ( row < m_maps[col].size() ) );
    return( m_maps[col][row] );
  }

}
