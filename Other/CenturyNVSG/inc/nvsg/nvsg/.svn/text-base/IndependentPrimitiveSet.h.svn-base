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

#include "nvsgcommon.h"
#include "nvsg/nvsg.h"
#include "nvsg/PrimitiveSet.h"

namespace nvsg
{
  /*! \brief Abstract base class for PrimitiveSet objects with independent primitives.
   *  \par Namespace: nvsg
   *  \remarks An IndependentPrimitiveSet is, for example, a Lines, a Quads, or a Triangles object.
   *  It holds a vector of indices, where each set of consecutive indices describe one primitive.
   *  The size of such a set depends on the primitive type - for example, 3 for Triangles or 4 for
   *  Quads.
   *  \sa Lines, PrimitiveSet, Quads, Triangles*/
  class IndependentPrimitiveSet : public PrimitiveSet
  {
    public:
      /*! \brief Destructs a IndependentPrimitiveSet. */
      NVSG_API virtual ~IndependentPrimitiveSet();

      /*! \brief Query if the data of this IndependentPrimitiveSet is shared.
       *  \return \c true if the data is shared, otherwise \c false.
       *  \sa getDataID */
      NVSG_API virtual bool isDataShared() const;

      /*! \brief Get the DataID, which uniquely identifies the sharable object data.
       *  \return The unique DataID, which identifies the object data.
       *  \sa isDataShared */
      NVSG_API virtual DataID getDataID() const;

      /*! \brief Query if this IndependentPrimitiveSet has indices.
       *  \return \c true, if this IndependentPrimitiveSet has indices, otherwise \c false.
       *  \sa getNumberOfIndices, getIndices, setIndices */
      NVSG_API bool hasIndices() const;

      /*! \brief Get the number of indices.
       *  \return The number of indices of this IndependentPrimitiveSet
       *  \sa hasIndices, getIndices, setIndices */
      NVSG_API size_t getNumberOfIndices() const;

      /*! \brief Get a pointer to the indices.
       *  \return A pointer to the constant indices of this IndependentPrimitiveSet
       *  \sa hasIndices, getNumberOfIndices, setIndices */
      NVSG_API const unsigned int * getIndices() const;

      /*! \brief Set the indices of this IndependentPrimitiveSet.
       *  \param indices A pointer to the constant indices to use.
       *  \param numIndices The number of indices.
       *  \remarks Copies \a numIndices indices from \a indices into this IndependentPrimitiveSet.
       *  \note The behavior is undefined if there are less than \a numIndices values of type
       *  \c unsigned \c int located at \a indices.
       *  \sa hasIndices, getNumberOfIndices, getIndices */
      NVSG_API void setIndices( const unsigned int * indices
                              , size_t numIndices );

      /*! \brief Set a subset of indices of this IndependentPrimitiveSet.
       *  \param pos The start position inside the range of pre-existing indices
       *  \param indices A pointer to the constant indices to use
       *  \param numIndices The number of indices
       *  \remarks Copies \a numIndices indices from \a indices into this IndependentPrimitiveSet,
       *  starting at position \a pos inside the range of pre-existing indices.\n
       *  Pre-existing indices in the range [\a pos, \a pos + \a numIndices) will be replaced.
       *  Pre-existing indices outside this range remain untouched.\n
       *  If you specifiy (-1) for \a pos or \a pos specifies the number of indices currently
       *  stored, the indices pointed to by \a indices will be appended to the pre-existing
       *  indices.\n
       *  \note The behavior is undefined if \a pos is neither a valid position inside the range
       *  of pre-existing indices, nor the number of indices currently stored, nor (-1).
       *  \note The behavior is undefined if there are less than \a numIndices values of type
       *  \c unsigned \c int located at \a indices.
       *  \sa hasIndices, getNumberOfIndices, getIndices */
      NVSG_API void setIndices( size_t pos
                              , const unsigned int * indices
                              , size_t numIndices );

      /*! \brief Test for equivalence with an other IndependentPrimitiveSet.
       *  \param p            A reference to the constant IndependentPrimitiveSet with which to 
       *  to test for equivalence.
       *  \param ignoreNames  Optional parameter to ignore the names of the objects; default is \c
       *  true.
       *  \param deepCompare  Optional parameter to perform a deep comparison; default is \c false.
       *  \return \c true if the IndependentPrimitiveSet \a p is equivalent to \c this, otherwise
       *  \c false.
       *  \remarks Two IndependentPrimitiveSet objects are equivalent if they are equivalent as
       *  PrimitiveSet and all their indices are pairwise equal.
       *  \note The behavior is undefined if \a p is not an IndependentPrimitiveSet nor derived
       *  from one.
       *  \sa PrimitiveSet */
      NVSG_API virtual bool isEquivalent( const Object *p
                                        , bool ignoreNames
                                        , bool deepCompare ) const;

    protected:
      /*! \brief Protected constructor to prevent explicit creation.
       *  \remarks The newly created IndependentPrimitiveSet has no indices.
       *  \note An IndependentPrimitiveSet will not be instantiated directly, but through
       *  instantiating a derived object like Quads or Triangles for example.*/
      NVSG_API IndependentPrimitiveSet();

      /*! \brief Protected partial copy constructor to prevent explicit creation.
       *  \param rhs A reference to a constant PrimitiveSet.
       *  \remarks The PrimitiveSet part of the newly created IndependentPrimitiveSet is copied
       *  from \a rhs; the IndependentPrimitiveSet specific data is initialized as if the default
       *  constructor had been called.
       *  \note An IndependentPrimitiveSet will not be instantiated directly, but by 
       *  instantiating a derived object like Quads or Triangles, for example.*/
      NVSG_API explicit IndependentPrimitiveSet( const PrimitiveSet &rhs );

      /*! \brief Protected copy constructor to prevent explicit creation.
       *  \param rhs A reference to the constant IndependentPrimitiveSet to copy from.
       *  \remarks The newly created IndependentPrimitiveSet is copied from \a rhs.
       *  \note An IndependentPrimitiveSet will not be instantiated directly, but by
       *  instantiating a derived object like Quads or Triangles, for example.*/
      NVSG_API IndependentPrimitiveSet( const IndependentPrimitiveSet& rhs );

      /*! \brief Calculate the bounding sphere of this IndependentPrimitiveSet.
       *  \return A nvmath::Sphere3f that contains the complete IndependentPrimitiveSet.
       *  \remarks This function is called by the framework to determine a sphere that completely
       *  contains the IndependentPrimitiveSet. */
      NVSG_API virtual nvmath::Sphere3f calculateBoundingSphere() const;

      /*! \brief Assignment operator
       *  \param rhs A reference to the constant IndependentPrimitiveSet to copy from.
       *  \return A reference to the assigned IndependentPrimitiveSet.
       *  \remarks The assignment operator calls the assignment operator of PrimitiveSet and copies
       *  the indices. */
      NVSG_API IndependentPrimitiveSet & operator=(const IndependentPrimitiveSet & rhs);

    private:
      nvutil::RCPtr<nvutil::RCVector<unsigned int> > m_indices;
  };


  // - - - - - - - - - - - - - - - - - - - - - - - - - 
  // inlines
  // - - - - - - - - - - - - - - - - - - - - - - - - - 

  inline bool IndependentPrimitiveSet::hasIndices()  const 
  { 
    return( !m_indices->empty() ); 
  }

  inline size_t IndependentPrimitiveSet::getNumberOfIndices()  const
  { 
    return( m_indices->size() ); 
  }

  inline const unsigned int * IndependentPrimitiveSet::getIndices() const 
  { 
    NVSG_TRACE();
    return( &(*m_indices)[0] ); 
  }
}
