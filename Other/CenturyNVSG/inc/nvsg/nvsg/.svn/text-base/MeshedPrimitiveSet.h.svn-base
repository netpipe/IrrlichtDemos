// Copyright NVIDIA Corporation 2002-2004
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

#include "nvsgcommon.h"
#include "nvsg/nvsg.h"
#include "nvsg/PrimitiveSet.h"

namespace nvsg
{
  /*! \brief IndexSet specialization used with a MeshedPrimitiveSet.
   *  \par Namespace: nvsg
   *  \sa IndexSet */
  class IndexMesh : public IndexSet
  {
    public:
      IndexMesh( void );
      IndexMesh( unsigned int width, unsigned int height );
      unsigned int getWidth( void ) const;
      unsigned int getHeight( void ) const;
      void setDimensions( unsigned int width, unsigned int height );

    private :
      unsigned int  m_width;
      unsigned int  m_height;
  };

  /*! \brief Abstract base class for PrimitiveSet objects with points organized as meshes.
   *  \par Namespace: nvsg
   *  \remarks This class holds a vector of IndexMesh elements used in meshed primitives like
   *  QuadMeshes. The meshes are defined by a vector of IndexMesh elements. Each IndexMesh holds
   *  the width and height of the mesh and is an IndexSet with width times height elements, and
   *  represents one mesh.
   *  \sa IndependentPrimitiveSet, QuadMeshes, StrippedPrimitiveSet */
  class MeshedPrimitiveSet : public PrimitiveSet
  {
    public:
      /*! \brief Destructs a MeshedPrimtiveSet. */
      NVSG_API MeshedPrimitiveSet();

      /*! \brief Destructor
       *  \remarks The destructor is never used explicitly, but only when the reference count of
       *  the MeshedPrimitiveSet is decremented to zero.
       *  \sa addRef, removeRef */
      NVSG_API virtual ~MeshedPrimitiveSet();

      /*! \brief Query whether the data of this MeshedPrimitiveSet is shared with other
       *  MeshedPrimitiveSet objects.
       *  \return \c true, if the data of this MeshedPrimitiveSet is shared with other
       *  MeshedPrimitiveSet objects, otherwise \c false.
       *  \remarks A MeshedPrimitiveSet object shares it's data with another MeshedPrimitiveSet
       *  object, if it was either instantiated as a copy of the other, using
       *  MeshedPrimitiveSet::clone, or if it was assigned to by the other object using the
       *  assignment operator. In that case, the two objects have the same DataID.
       *  \sa clone, getDataID, operator=() */
      NVSG_API virtual bool isDataShared() const;

      /*! \brief Get the unique data identifier of this object.
       *  \return A 64-bit value that uniquely identifies the embedded data.
       *  \remarks A MeshedPrimitiveSet object shares it's data with another MeshedPrimitiveSet
       *  object, if it was either instantiated as a copy of the other, using
       *  MeshedPrimitiveSet::clone, or if it was assigned to by the other object using the
       *  assignment operator. In that case, the two objects have the same DataID.
       *  \sa clone, isDataShared, operator=() */
      NVSG_API virtual DataID getDataID() const;

      /*! \brief Query if this MeshedPrimitiveSet has meshes.
       *  \return \c true, if this MeshedPrimitiveSet has at least one IndexMesh, otherwise \c false.
       *  \sa getNumberOfMeshes, getMeshes, setMeshes */
      NVSG_API bool hasMeshes()  const;

      /*! \brief Get the number of meshes.
       *  \return The number of meshes of this MeshedPrimitiveSet.
       *  \sa hasMeshes, getMeshes, setMeshes */
      NVSG_API size_t getNumberOfMeshes() const;

      /*! \brief Get a pointer to the meshes.
       *  \return A pointer to the constant IndexMesh elements of this MeshedPrimitiveSet.
       *  \sa hasMeshes, getNumberOfMeshes, setMeshes */
      NVSG_API const IndexMesh * getMeshes() const;

      /*! \brief Set the meshes of this MeshedPrimitiveSet.
       *  \param meshes A pointer to the constant IndexMesh elements to use.
       *  \param numMeshes The number of meshes.
       *  \remarks Copies \a numMeshes meshes from \a meshes into this MeshedPrimitiveSet.
       *  \note The behaviour is undefined, if there are less than \a numMeshes values of type
       *  IndexMesh located at \a meshes.
       *  \sa hasMeshes, getNumberOfMeshes, getMeshes */
      NVSG_API void setMeshes( const IndexMesh * meshes
                             , size_t numMeshes );

      /*! \brief Set a subset of meshes of this MeshedPrimitiveSet.
       *  \param pos The start position inside the range of pre-existing meshes.
       *  \param meshes A pointer to the constant meshes to use.
       *  \param numMeshes The number of meshes.
       *  \remarks Copies \a numMeshes of IndexMesh elements from \a meshes into this
       *  MeshedPrimitiveSet, starting at position \a pos inside the range of pre-existing
       *  indices.\n
       *  Pre-existing meshes in the range [\a pos, \a pos + \a numMeshes) will be replaced.
       *  Pre-existing meshes outside this range remain untouched.\n
       *  If you specifiy (-1) for \a pos or \a pos specifies the number of meshes currently
       *  stored, the meshes pointed to by \a meshes will be appended to the pre-existing meshes.\n
       *  \note The behaviour is undefined, if \a pos is neither a valid position inside the range
       *  of pre-existing meshes, nor the number of meshes currently stored, nor (-1).
       *  \note The behaviour is undefined, if there are less than \a numMeshes values of type
       *  IndexMesh located at \a meshes.
       *  \sa hasMeshes, getNumberOfMeshes, getMeshes */
      NVSG_API void setMeshes( size_t pos                 //!< Start position inside the range of pre-existing meshes
                             , const IndexMesh * meshes  //!< meshes to set
                             , size_t numMeshes           //!< number of meshes
                             );

      /*! \brief Test for equivalence with an other MeshedPrimitiveSet.
       *  \param p A reference to the constant MeshedPrimitiveSet to test for equivalence with.
       *  \param ignoreNames Optional parameter to ignore the names of the objects; default is \c true.
       *  \param deepCompare Optional parameter to perform a deep comparsion; default is \c false.
       *  \return \c true if the MeshedPrimitiveSet \a p is equivalent to \c this, otherwise \c false.
       *  \remarks Two MeshedPrimitiveSet objects are equivalent, if they are equivalent as
       *  PrimitiveSet, and all their IndexMesh elements are pairwise equal.
       *  \note The behaviour is undefined if \a p is not an MeshedPrimitiveSet or derived from one.
       *  \sa PrimitiveSet */
      NVSG_API virtual bool isEquivalent( const Object * p
                                        , bool ignoreNames
                                        , bool deepCompare ) const;

    protected:
      /*! \brief Protected copy constructor to prevent explicit creation.
       *  \param rhs A reference to the constant MeshedPrimitiveSet to copy from.
       *  \remarks The newly created MeshedPrimitiveSet is copied from \a rhs.
       *  \note A MeshedPrimitiveSet will not be instantiated directly, but through instantiating
       *  a derived object like QuadMeshes for example.*/
      NVSG_API MeshedPrimitiveSet( const MeshedPrimitiveSet& rhs );

      /*! \brief Calculate the bounding sphere of this MeshedPrimitiveSet.
       *  \return A nvmath::Sphere3f that contains the complete MeshedPrimitiveSet.
       *  \remarks This function is called by the framework to determine a sphere that completely
       *  contains the MeshedPrimitiveSet. */
      NVSG_API virtual nvmath::Sphere3f calculateBoundingSphere() const;

      /*! \brief Assignment operator
       *  \param rhs A reference to the constant MeshedPrimitiveSet to copy from.
       *  \return A reference to the assigned MeshedPrimitiveSet.
       *  \remarks The assignment operator calls the assignment operator of PrimitiveSet and copies
       *  the IndexMesh elements. */
      NVSG_API MeshedPrimitiveSet & operator=(const MeshedPrimitiveSet & rhs);

    private:
      nvutil::RCPtr<nvutil::RCVector<IndexMesh> > m_meshes;
  };


  // - - - - - - - - - - - - - - - - - - - - - - - - - 
  // inlines
  // - - - - - - - - - - - - - - - - - - - - - - - - - 

  inline bool MeshedPrimitiveSet::hasMeshes()  const 
  { 
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_READ_LOCKED(this);
    return( !m_meshes->empty() ); 
  }

  inline size_t MeshedPrimitiveSet::getNumberOfMeshes()  const
  { 
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_READ_LOCKED(this);
    return( m_meshes->size() ); 
  }

  inline const IndexMesh * MeshedPrimitiveSet::getMeshes() const 
  { 
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_READ_LOCKED(this);
    return( &(*m_meshes)[0] ); 
  }
  inline IndexMesh::IndexMesh( void )
    : m_width(0)
    , m_height(0)
  {
  }

  inline IndexMesh::IndexMesh( unsigned int width, unsigned int height )
    : std::vector<unsigned int>(width*height)
    , m_width(width)
    , m_height(height)
  {
  }

  inline unsigned int IndexMesh::getWidth( void ) const
  {
    return( m_width );
  }

  inline void IndexMesh::setDimensions( unsigned int width, unsigned int height )
  {
    m_width = width;
    m_height = height;
    resize( m_width * m_height );
  }

  inline unsigned int IndexMesh::getHeight( void ) const
  {
    return( m_height );
  }
} // namespace nvsg