// Copyright NVIDIA Corporation 2002-2006
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

#include "nvsg/Object.h" // base class definition
#include <vector>

namespace nvsg
{
  /*! \brief Helper class to hold a vector of owners of an Object.
   *  \remarks Some classes, like Drawable, LightSource, or StateAttribute, need to know all the
   *  objects that "own" them, i.e. the objects that are referencing them in the tree hierarchy.
   *  This knowledge is needed to carry information from such objects that are not derived from Node
   *  "up-tree" to the Nodes that hold them. That is, the "owner-relationship" is similar to the
   *  "parent-relationship" of the Nodes. Each OwnedObject can only be owned by one specific type of
   *  owners, specified as the template parameter "OwnerHandle".
   *  \sa Node */
  template<typename OwnerHandle>
  class OwnedObject : public Object
  {
    public:
      /*! \brief Get the number of owners of this OwnedObject.
       *  \return Number of owners of this OwnedObject.
       *  \remarks An OwnedObject that is part of one or more OwnerHandle objects holds a pointer
       *  back to each of those OwnerHandle objects. These are called the owners. So the number
       *  of owners tells you how often this OwnedObject is referenced in a scene tree.
       *  \sa getOwner */
      size_t getNumberOfOwners() const;

      /*! \brief Get the owning OwnerHandle at position \a index.
       *  \param index Position of the OwnerHandle to get in the list of owners.
       *  \return A pointer to the OwnerHandle that owns this OwnedObject.
       *  \remarks An OwnedObject that is part of one or more OwnerHandle objects, holds a
       *  pointer back to each of those OnwerHandle objects. These are called the owners.
       *  \note The behavior is undefined if \a index is greater than or equal to the number of
       *  owners.
       *  \sa getNumberOfOwners */
      OwnerHandle * getOwner( size_t index ) const;

      /*! \brief Assignment operator
       *  \param rhs A reference to the constant OwnedObject to copy from
       *  \return A reference to the assigned OwnedObject
       *  \remarks The assignment operator calls the assignment operator of Object.
       *  \note The owners of rhs are not copied.
       *  \sa Object */
      OwnedObject<OwnerHandle>& operator=(const OwnedObject<OwnerHandle> & rhs);

    protected:
      /*! \brief Protected default constructor to prevent instantiation of an OwnedObject.
       *  \remarks An OwnedObject is not intended to be instantiated, but only classes derived from
       *  it. */
      OwnedObject();

      /*! \brief Protected copy constructor from an Object.
       *  \remarks An OwnedObject is not intended to be instantiated, but only classes derived from
       *  it. */
      OwnedObject( const Object & rhs );

      /*! \brief Protected copy constructor from an OwnedObject.
       *  \remarks An OwnedObject is not intended to be instantiated, but only classes derived from
       *  it. */
      OwnedObject( const OwnedObject<OwnerHandle> &rhs );

      /*! \brief Protected destructor of an OwnedObject.
       *  \remarks An OwnedObject is not intended to be instantiated, but only classes derived from
       *  it. */
      ~OwnedObject();

      /*! \brief Add an owner to the set of owners.
       *  \param oh A pointer to the object to add as an owner.
       *  \remarks The owner \a oh is added to the set of owners.
       *  \note The reference count of the owner \a oh is not increased.
       *  \sa removeOwner, getNumberOfOwners, getOwner, ownersBegin, ownersEnd */
      void addOwner( OwnerHandle * oh );

      /*! \brief Remove an owner from the set of owners.
       *  \param oh A pointer to the object to remove as an owner.
       *  \remarks The owner \a oh is removed from the set of owners.
       *  \note The reference count of the owner \a oh is not decreased.
       *  \sa addOwner, getNumberOfOwners, getOwner, ownersBegin, ownersEnd */
      void removeOwner( OwnerHandle * oh );

      /*! \brief Non-constant iterator type on owners.
       *  \sa ConstOwnerIterator */
      typedef typename std::vector<OwnerHandle *>::iterator OwnerIterator;

      /*! \brief Constant iterator type on owners.
       *  \sa OwnerIterator */
      typedef typename std::vector<OwnerHandle *>::const_iterator ConstOwnerIterator;

      /*! \brief Get a constant iterator to the first owner from the set of owners.
       *  \return  A constant iterator to the first owner from the set of owners.
       *  \sa ownersEnd */
      ConstOwnerIterator ownersBegin() const { return m_owners.begin(); }

      /*! \brief Get an iterator to the first owner from the set of owners.
       *  \return An iterator to the first owner from the set of owners.
       *  \sa ownersEnd */
      OwnerIterator ownersBegin() { return m_owners.begin(); }

      /*! \brief Get a constant iterator to behind the last owner from the set of owners.
       *  \return  A constant iterator to behind the last owner from the set of owners.
       *  \sa ownersBegin */
      ConstOwnerIterator ownersEnd() const { return m_owners.end(); }

      /*! \brief Get an iterator to behind the last owner from the set of owners.
       *  \return  An iterator to behind the last owner from the set of owners.
       *  \sa ownersBegin */
      OwnerIterator ownersEnd() { return m_owners.end(); }

    private:
      std::vector<OwnerHandle *>  m_owners;
  };

  template<typename OwnerHandle>
    OwnedObject<OwnerHandle>::OwnedObject()
    : Object()
  {
  }

  template<typename OwnerHandle>
    OwnedObject<OwnerHandle>::OwnedObject( const Object &rhs )
    : Object(rhs)
  {
    // must not copy owners!!!
    // this is what this implementation of the cpy ctor is for!
    NVSG_ASSERT( m_owners.empty() );
  }

  template<typename OwnerHandle>
    OwnedObject<OwnerHandle>::OwnedObject( const OwnedObject<OwnerHandle> &rhs )
    : Object(rhs)
  {
    // must not copy owners!!!
    // this is what this implementation of the cpy ctor is for!
    NVSG_ASSERT( m_owners.empty() );
  }

  template<typename OwnerHandle>
    OwnedObject<OwnerHandle>::~OwnedObject()
  {
    // if this fires, there was no proper cleanup of owner relationship elsewhere
    NVSG_ASSERT( m_owners.empty() );
  }

  template<typename OwnerHandle>
    size_t OwnedObject<OwnerHandle>::getNumberOfOwners( void ) const
  {
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_READ_LOCKED(this);

    return( m_owners.size() );
  }

  template<typename OwnerHandle>
    OwnerHandle * OwnedObject<OwnerHandle>::getOwner( size_t index ) const
  {
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_READ_LOCKED(this);

    NVSG_ASSERT( index < m_owners.size() );   
    return( m_owners[index] );
  }

  template<typename OwnerHandle>
    OwnedObject<OwnerHandle> & OwnedObject<OwnerHandle>::operator=( const OwnedObject<OwnerHandle> &rhs )
  {
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_WRITE_LOCKED(this);

    Object::operator=( rhs );
    return( *this );
  }

  template<typename OwnerHandle>
    void OwnedObject<OwnerHandle>::addOwner( OwnerHandle * oh )
  {
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_WRITE_LOCKED(this);

    // do not add an owner twice
    if ( m_owners.end() == std::find( m_owners.begin(), m_owners.end(), oh ) )
    { 
      // must not add reference for an owner
      m_owners.push_back( oh );     
    } 
  }

  template<typename OwnerHandle>
    void OwnedObject<OwnerHandle>::removeOwner( OwnerHandle * oh )
  {
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_WRITE_LOCKED(this);

    // the erase-remove idiom is the best way to get rid of specific elements from a vector
    m_owners.erase( remove( m_owners.begin(), m_owners.end(), oh ), m_owners.end() );
    //  NOTE: don't remove a ref from the owner here
  }

} // namespace nvsg
