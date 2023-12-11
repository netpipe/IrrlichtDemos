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
#include "nvsg/Object.h"

namespace nvsg
{
  /*! \brief Class to hold a list of two-dimensional maps.
   *  \par Namespace: nvsg
   *  \remarks A MapList is used in a ViewState to hold a list of two-dimensional maps. These maps
   *  are displayed on top of the scene. They can be, for example, some information overlay or type 
   *  of menus.\n
   *  The MapList holds a list of MapObject objects. A MapObject can be a MapElement, holding a
   *  single Map; a MapArray, holding a row or column of Map objects; or a MapMatrix, holding a
   *  matrix of Map objects.\n
   *  The MapObject objects are positioned in the viewport by raster coordinates.
   *  \sa Map, MapArray, MapElement, MapObject, MapMatrix */
  class MapList : public Object
  {
    public:
      /*! \brief Constructs an empty MapList. */
      NVSG_API MapList();

      /*! \brief Copy-Constructs a MapList from another MapList. */
      NVSG_API MapList( const MapList &rhs );

      /*! \brief Destructs a MapList. */
      NVSG_API virtual ~MapList(void);

      /*! \brief Get the number of MapObject objects in this MapList.
       *  \return The number of contained MapObject objects.
       *  \sa addMapObject, getMapObject */
      NVSG_API size_t getNumberOfMapObjects( void ) const;

      /*! \brief Get a pointer to the constant MapObject at the specified index.
       *  \param index Index of the MapObject to get.
       *  \return A pointer to the constant MapObject at the specified index in the MapList.
       *  \note The behavior is undefined if \a index is larger than or equal to the number of
       *  MapObject objects in this MapList.
       *  \sa addMapObject, getNumberOfMapObjects, pickMap */
      NVSG_API MapObjectHandle * getMapObject( size_t index ) const;

      /*! \brief Add a MapObject to the end of the MapList.
       *  \param mo A pointer to the constant MapObject to add
       *  \remarks The reference count of \a mo is increased.
       *  \sa getMapObject, getNumberOfMapObjects, insertMapObject */
      NVSG_API void addMapObject( MapObjectHandle * mo );

      /*! \brief Insert a MapObject in front of the specified index.
       *  \param index Index to insert a MapObject before.
       *  \param mo A pointer to the constant MapObject to insert.
       *  \remarks The reference count of \a mo is incremented.
       *  \note The behavior is undefined if \a index is larger than or equal to the number of
       *  MapObject objects in this MapList.
       *  \sa addMapObject, getMapObject, getNumberOfMapObjects */
      NVSG_API void insertMapObject( size_t index
                                   , MapObjectHandle * mo );

      /*! \brief Replace the MapObject at the specified index.
       *  \param index Index to determine where to replace the MapObject.
       *  \param mo A pointer to the constant MapObject to replace with.
       *  \remarks The reference count of \a mo is incremented, and the reference count of the
       *  MapObject previously at index \a index is decremented.
       *  \note The behavior is undefined if \a index is larger than or equal to the number of
       *  MapObject objects in this MapList.
       *  \sa addMapObject, getMapObject, getNumberOfMapObjects, removeMapObject */
      NVSG_API void replaceMapObject( size_t index
                                    , MapObjectHandle * mo );

      /*! \brief Remove the MapObject at the specified index.
       *  \param index Index to determine where to remove the MapObjec.
       *  \remarks The reference count of the MapObject at index \a index is decremented.
       *  \note The behavior is undefined if \a index is larger than or equal to the number of
       *  MapObject objects in this MapList.
       *  \sa addMapObject, getMapObject, getNumberOfMapObjects, replaceMapObject */
      NVSG_API void removeMapObject( size_t index );

      /*! \brief Clear the MapList
       *  \remarks The reference count of every MapObject in this MapList is decremented, and the
       *  list is cleared.
       *  \sa addMapObject, getNumberOfMapObjects, removeMapObject */
      NVSG_API void clear( void );

      /*! \brief Get the Map located at the specified raster position.
       *  \param x Horizontal raster coordinate to use for getting the Map.
       *  \param y Vertical raster coordinate to use for getting the Map.
       *  \param index Optional pointer to get the index of the MapObject that holds the picked Map.
       *  \return A pointer to a MapHandle specifying the Map at the specified raster position, 
       *  or NULL if no Map was found at that position. If a Map is picked and \a index is not NULL, 
       *  it returns the index of the MapObject holding the picked Map.
       *  \remarks This function determines the Map of this MapList at the specified raster
       *  position (\a x, \a y), if any. It asks each of its MapObject objects if it has a Map at the
       *  specified position, and if so, that Map is returned. If \a index is not NULL, the index
       *  of the MapObject holding that Map is returned there, too.
       *  \sa getMapObject, getNumberOfMapObjects */
      NVSG_API MapHandle * pickMap( int x
                                  , int y
                                  , size_t * index = NULL ) const;

    protected:
      /*! \brief Validate the Incarnation. 
       *  \remarks This function is called from the framework when the Incarnation is requested
       *  and it is marked as invalid. Then it is validated and the Incarnation of all its Map
       *  objects is requested to continue validation down the tree hierarchy.
       *  \a Incarnation */
      NVSG_API virtual void validateIncarnation( void ) const;  // from top to bottom

    private:
      std::vector<MapObjectHandle *>  m_list;
  };

  inline void MapList::addMapObject( MapObjectHandle * mo )
  {
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_WRITE_LOCKED(this);
    NVSG_ASSERT( mo );
    mo->addRef();
    m_list.push_back( mo );
  }

  inline MapObjectHandle * MapList::getMapObject( size_t index ) const
  {
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_READ_LOCKED(this);
    NVSG_ASSERT( index < m_list.size() );
    return( m_list[index] );
  }

  inline size_t MapList::getNumberOfMapObjects( void ) const
  {
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_READ_LOCKED(this);
    return( m_list.size() );
  }

  inline void MapList::insertMapObject( size_t index, MapObjectHandle * mo )
  {
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_WRITE_LOCKED(this);
    NVSG_ASSERT( mo && ( index < m_list.size() ) );
    mo->addRef();
    m_list.insert( m_list.begin() + index, mo );
  }

  inline void MapList::removeMapObject( size_t index )
  {
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_WRITE_LOCKED(this);
    NVSG_ASSERT( index < m_list.size() );
    m_list[index]->removeRef();
    m_list.erase( m_list.begin() + index );
  }

  inline void MapList::replaceMapObject( size_t index, MapObjectHandle * mo )
  {
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_WRITE_LOCKED(this);
    NVSG_ASSERT( mo && ( index < m_list.size() ) );
    mo->addRef();
    m_list[index]->removeRef();
    m_list[index] = mo;
  }
}
