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
#include "nvsg/OwnedObject.h" // base class definition
#include "nvsg/StateVariant.h"
#include "nvutil/BitMask.h"

namespace nvsg
{
  class Node;

  typedef unsigned int VariantKey; //!< Key to a specific StateVariant

  // shared state flag defines
  enum
  {
    SF_CONTAINS_CGFX                 = BIT0,
    SF_CONTAINS_CGFX_VALID           = BIT1,
    SF_CONTAINS_TRANSPARENCY         = BIT2,
    SF_CONTAINS_TRANSPARENCY_VALID   = BIT3
  };

  /*! \brief Helper class used for iterating the StateVariant objects in a StateSet.
   *  \par Namespace: nvsg
   *  \remarks A VariantIterator is used to iterate over all StateVariant objects of a StateSet.
   *  \par Example
   *  To iterate over all StateVariant objects in a StateSet \a ss, do something similar to the following:
   *  \code
   *  StateSet * ss;
   *  for ( VariantIterator vi = ss->getFirstVariantIterator()
   *      ; vi != ss->getLastVariantIterator()
   *      ; vi = ss->getNextVariantIterator( vi ) )
   *  {
   *    VariantKey vk = ss->getVariantKey( vi );
   *    NVSG_ASSERT( vk != INVALID_VARIANT_KEY );
   *    const nvsg::StateVariant * sv = p->getVariant( vk );
   *    // ...
   *  }
   *  \endcode
   *  \sa */
  class VariantIterator
  {
    friend class StateSet;

    public:
      /*! \brief Test two VariantIterator objects for inequality
       *  \param it The VariantIterator to test with.
       *  \return \c true, if \a this and \a it are not equal, otherwise \c false. */
      bool operator!=( VariantIterator it ) const;

      /*! \brief Test two VariantIterator objects for equality
       *  \param it The VariantIterator to test with.
       *  \return \c true, if \a this and \a it are equal, otherwise \c false. */
      bool operator==( VariantIterator it ) const;

    private:
      typedef std::map<VariantKey, StateVariantHandle *> container_type;
      typedef container_type::const_iterator             wrapped_type;
      typedef wrapped_type::pointer                      pointer;
      typedef wrapped_type::reference                    reference;

      VariantIterator();
      explicit VariantIterator(wrapped_type it);
      pointer operator->() const { return m_iter.operator->(); }
      reference operator*() const { return m_iter.operator*(); }
      VariantIterator& operator++() { ++m_iter; return *this; } // preincrement
      VariantIterator operator++(int) { return VariantIterator(m_iter++); } // postincrement
      operator wrapped_type() const { return m_iter; } // implicit conversion to wrapped_type
    private:
      wrapped_type m_iter;
  };

  inline VariantIterator::VariantIterator()
  : m_iter()
  {
  }

  inline VariantIterator::VariantIterator( wrapped_type it )
  : m_iter(it)
  {
  }

  inline bool VariantIterator::operator!=( VariantIterator it ) const
  {
    return m_iter!=it.m_iter;
  }

  inline bool VariantIterator::operator==( VariantIterator it ) const
  {
    return m_iter==it.m_iter;
  }

  /*! \brief Container class to hold a number of StateVariant objects.
   *  \par Namespace: nvsg
   *  \remarks A StateSet holds a number of StateVariant objects and a key to select one of them.
   *  Only the selected StateVariant is used on standard traversal. That way, it is easy to switch
   *  between different StateVariant objects to use.
   *  \sa StateVariant */
  class StateSet : public OwnedObject<NodeHandle>
  {
    friend class Node;

    public:
      static const VariantKey INVALID_VARIANT_KEY = 0xFFFFFFFF;

      /*! \brief Default-constructs a StateSet. 
       *  \remarks An empty StateSet with an invalid key is created. */
      NVSG_API StateSet();

      /*! \brief Constructs a StateSet from an Object. 
       *  \remarks An empty StateSet with an invalid key is created. */
      NVSG_API StateSet( const Object &rhs );

      /*! \brief Constructs a StateSet as a copy from another StateSet. */ 
      NVSG_API StateSet( const StateSet& rhs );

      /*! \brief Destructs a StateSet */ 
      NVSG_API virtual ~StateSet();

      /*! \brief Get the number of StateVariant objects in this StateSet.
       *  \return The number of StateVariant objects in this StateSet.
       *  \sa StateVariant, addVariant, getVariant, getVariantKey, removeVariant, replaceVariant */
      NVSG_API size_t getNumberOfVariants() const;

      /*! \brief Get an iterator on the first pair of VariantKey and StateVariant.
       *  \return A VariantIterator to the first pair of VariantKey and StateVariant.
       *  \sa getLastVariantIterator, getNextVariantIterator */
      NVSG_API VariantIterator getFirstVariantIterator() const;

      /*! \brief Get an iterator on the next pair of VariantKey and StateVariant.
       *  \param it The VariantIterator to get the next one for.
       *  \return A VariantIterator to the next pair of VariantKey and StateVariant.
       *  \sa getFirstVariantIterator, getLastVariantIterator */
      NVSG_API VariantIterator getNextVariantIterator( VariantIterator it ) const;

      /*! \brief Get an iterator beyond the last pair of VariantKey and StateVariant.
       *  \return A VariantIterator beyond the last pair of VariantKey and StateVariant.
       *  \sa getFirstVariantIterator, getNextVariantIterator */
      NVSG_API VariantIterator getLastVariantIterator() const;

      /*! \brief Get the currently active StateVariant
       *  \return A pointer to the constant StateVariant currently active, or NULL, if there is none.
       *  \remarks There is only one StateVariant active in a StateSet. Only that StateVariant is
       *  used in traversals.
       *  \sa StateVariant, addVariant, getNumberOfVariants, getVariantKey, getActiveVariant */
      NVSG_API StateVariantHandle * getActiveVariant() const;

      /*! \brief Get the StateVariant associated with the specified VariantKey.
       *  \param key The VariantKey to get the StateVariant of.
       *  \return A pointer to the constant StateVariant, that is associated with \a key.
       *  \a key, or NULL, if there is none.
       *  \sa StateVariant, VariantKey, addVariant, getNumberOfVariants, getVariantKey, getActiveVariant */
      NVSG_API StateVariantHandle * getVariant( VariantKey key ) const;

      /*! \brief Get the StateVariant corresponding to the VariantIterator \a it.
       *  \param it A VariantIterator from getFirstVariantIterator or getNextVariantIterator
       *  \return A pointer to the constant StateVariant corresponding to \a it.
       *  \note The behavior is undefined if \a it is not a valid VariantIterator.
       *  \sa getFirstVariantIterator, getNextVariantIterator, getLastVariantIterator */
      NVSG_API StateVariantHandle * getVariant( VariantIterator it ) const;

      /*! \brief Get the VariantKey corresponding to the VariantIterator \a it.
       *  \param it A VariantIterator from getFirstVariantIterator or getNextVariantIterator
       *  \return The VariantKey corresponding to \ it.
       *  \note The behavior is undefined if \a it is not a valid VariantIterator.
       *  \sa getFirstVariantIterator, getNextVariantIterator, getLastVariantIterator */
      NVSG_API VariantKey getVariantKey( VariantIterator it ) const;

      /*! \brief Get the currently active VariantKey.
       *  \return The currently active VariantKey.
       *  \remarks There is only one StateVariant active in a StateSet. Only that StateVariant is
       *  used in traversals. If there is no StateVariant active, the INVALID_VARIANT_KEY is
       *  returned.
       *  \sa VariantKey, getActiveVariant, getVariantKey, setActiveVariantKey */
      NVSG_API VariantKey getActiveVariantKey() const;

      /*! \brief Set the active VariantKey
       *  \param key The VariantKey to set active.
       *  \remarks There is only one StateVariant active in a StateSet. Only that StateVariant is
       *  used in traversals. Only a VariantKey that is part of the StateSet can be used.
       *  \sa VariantKey, getActiveVariant, getActiveVariantKey */
      NVSG_API void setActiveVariantKey( VariantKey key );

      /*! \brief Add a combination of VariantKey and StateVariant to this StateSet.
       *  \param variant A pointer to the constant StateVariant to add.
       *  \param key The VariantKey to associate with \a variant.
       *  \remarks The reference count of \ variant is increased. If there was a StateVariant
       *  associated with \a key before, the reference count of that StateVariant is decreased.
       *  \note The behavior is undefined if \a variant is not a valid pointer to a constant
       *  StateVariant.
       *  \note The behavior is undefined if \a key is INVALID_VARIANT_KEY.
       *  \sa StateVariant, VariantKey, getNumberOfVariants, getVariant, getActiveVariant */
      NVSG_API void addVariant( StateVariantHandle * variant
                              , VariantKey key );

      /*! \brief Remove the StateVariant associated with the specified VariantKey.
       *  \param key The VariantKey that is to be removed.
       *  \remarks If \a key is part of this StateSet, the reference count of the associated
       *  StateVariant is decreased.
       *  \sa StateVariant, addVariant, getNumberOfVariants, getVariant, replaceVariant */
      NVSG_API void removeVariant( VariantKey key );

      /*! \brief Replace the StateVariant associated with \a key by \a variant.
       *  \param key The VariantKey associated with the StateVariant to replace.
       *  \param variant A pointer to the constant StateVariant to use as the replacement.
       *  \remarks If \a key is part of this StateSet, the StateVariant associated with \a key is
       *  replaced by \a variant. The reference count of the formerly associated StateVariant is
       *  decremented, and the reference count of \a variant is incremented.
       *  \sa StateVariant, VariantKey, addVariant, replaceVariantKey */
      NVSG_API void replaceVariant( VariantKey key
                                  , StateVariantHandle * variant );

      /*! \brief Replace the association with \a oldKey to \a newKey.
       *  \param oldKey The VariantKey, that is to be replaced.
       *  \param newKey The VariantKey, that replaces \a oldKey.
       *  \remarks If there is a StateVariant associated with \a oldKey part of this StateSet,
       *  that StateVariant is re-associated with newKey. The old association is lost.
       *  \sa StateVariant, VariantKey, addVariant, removeVariant, replaceVariant */
      NVSG_API void replaceVariantKey( VariantKey oldKey
                                     , VariantKey newKey );

      /*! \brief Get the number of StatePass objects in the specified StateVariant.
       *  \param key Optional VariantKey of the StateVariant to get the number of StatePass
       *  objects from; default is 0.
       *  \return The number of StatePass objects in the StateVariant associated with the
       *  VariantKey \a key.
       *  \sa StateVariant, VariantKey, addPass, getPass, removePass, replacePass */
      NVSG_API size_t getNumberOfPasses( VariantKey key = 0 ) const;

      /*! \brief Get the StatePass at the specified position in the StateVariant associated with
       *  \a key.
       *  \param passIndex The index of the StatePass to get.
       *  \param key Optional VariantKey of the StateVariant to get the StatePass from; default is 0.
       *  \return A pointer to the constant StatePass at position \a passIndex in the StateVariant
       *  associated with the VariantKey \a key.
       *  \note The behavior is undefined if there is no StateVariant associated with the
       *  VariantKey \a key, or if \a passIndex is larger than or equal to the number of StatePass
       *  objects in the specified StateVariant.
       *  \sa StatePass, StateVariant, VariantKey, addPass, getNumberOfPasses, removePass,
       *  replacePass */
      NVSG_API StatePassHandle * getPass( size_t passIndex
                                        , VariantKey key = 0 ) const;

      /*! \brief Add a StatePass to the StateVariant associated with \a key.
       *  \param pass A pointer to the constant StatePass to add.
       *  \param key Optional VariantKey of the StateVariant to add the StatePass to; default is 0.
       *  \remarks If there is no StateVariant associated to \a key in this StateSet, such a
       *  StateVariant is created. While adding the StatePass, its reference count is incremented.
       *  \sa StatePass, StateVariant, VariantKey, getNumberOfPasses, getPass, removePass,
       *  replacePass */
      NVSG_API void addPass( StatePassHandle * pass
                           , VariantKey key = 0 );

      /*! \brief Remove the StatePass at the specified position from the StateVariant associated
       *  with \a key.
       *  \param passIndex The index of the StatePass to remove.
       *  \param key Optional VariantKey of the StateVariant to remove the StatePass from; default
       *  is 0.
       *  \remarks The reference count of the StatePass to remove is decreased.
       *  \note The behaviour is undefined, if there is no StateVariant associated with the
       *  VariantKey \a key, or if \a passIndex is larger than or equal to the number of StatePass
       *  objects in the specified StateVariant.
       *  \sa StatePass, StateVariant, VariantKey, addPass, getNumberOfPasses, getPass, replacePass */
      NVSG_API void removePass( size_t passIndex
                              , VariantKey key = 0 );

      /*! \brief Replace the StatePass at the specified position in the StateVariant associated
       *  with \a key.
       *  \param pass A pointer to the constant StatePass to replace with.
       *  \param passIndex The index of the StatePass in the StateVariant to replace.
       *  \param key Optional VariantKey of the StateVariant to replace the StatePass from; default
       *  is 0.
       *  \remarks The reference count of \a pass is incremented, and the reference count of the
       *  StatePass at position \a index in the StateVariant associated with \a key is decremented.
       *  \note The behavior is undefined if there is no StateVariant associated with the
       *  VariantKey \a key, or if \a passIndex is larger than or equal to the number of StatePass
       *  objects in the specified StateVariant.
       *  \sa StatePass, StateVariant, VariantKey, addPass, getNumberOfPasses, getPass */
      NVSG_API void replacePass( StatePassHandle * pass
                               , size_t passIndex
                               , VariantKey key = 0 );

      /*! \brief Get the number of StateAttribute objects in the specified StatePass.
       *  \param passIndex Optional index of the StatePass to get the number of StateAttribute
       *  objects from; default is 0.
       *  \param key Optional VariantKey of the StateVariant to use the StatePass in; default
       *  is 0.
       *  \return The number of StateAttribute objects in the StatePass at position \a passIndex
       *  in the StateVariant associated with the VariantKey \a key.
       *  \sa StateAttribute, StatePass, StateVariant, VariantKey, addAttribute, getAttribute,
       *  getAttributeIndex, removeAttribute */
      NVSG_API size_t getNumberOfAttributes( size_t passIndex = 0
                                           , VariantKey key = 0 ) const;

      /*! \brief Get the StateAttribute at the specified position.
       *  \param attributeIndex The index of the StateAttribute to get.
       *  \param passIndex Optional index of the StatePass to get the StateAttribute from; default
       *  is 0.
       *  \param key Optional VariantKey of the StateVariant to use the StatePass in; default
       *  is 0.
       *  \return A pointer to the constant StateAttribute at the specified position.
       *  \note The behavior is undefined if there is no StateVariant associated with the
       *  VariantKey \a key, or if \a passIndex is larger than or equal to the number of StatePass
       *  objects in the specified StateVariant, of if \a attributeIndex is larger than or equal
       *  to the number of StateAttribute objects in the specified StatePass.
       *  \sa StateAttribute, StatePass, StateVariant, VariantKey, addAttribute, getAttribute,
       *  getAttributeIndex, getNumberOfAttributes, removeAttribute*/
      NVSG_API StateAttributeHandle * getAttribute( size_t attributeIndex
                                                  , size_t passIndex = 0
                                                  , VariantKey key = 0 ) const;

      /*! \brief Get the StateAttribute with the object code \a objectCode in the specified
       *  StatePass.
       *  \param objectCode The object code of the StateAttribute to get.
       *  \param passIndex Optional index of the StatePass to get the StateAttribute from; default
       *  is 0.
       *  \param key Optional VariantKey of the StateVariant to use the StatePass in; default
       *  is 0.
       *  \return A pointer to the constant StateAttribute with the object code \a objectCode, or
       *  NULL, if there is none.
       *  \note The behavior is undefined if there is no StateVariant associated with the
       *  VariantKey \a key, or if \a passIndex is larger than or equal to the number of StatePass
       *  objects in the specified StateVariant.
       *  \sa StateAttribute, StatePass, StateVariant, VariantKey, addAttribute, getAttribute,
       *  removeAttributeByObjectCode */
      NVSG_API StateAttributeHandle * getAttributeByObjectCode( unsigned int objectCode
                                                              , size_t passIndex = 0
                                                              , VariantKey key = 0 ) const;

      /*! \brief Get the index of the StateAttribute in the specified StatePass.
       *  \param attribute A pointer to the constant StateAttribute to get the index for.
       *  \param passIndex Optional index of the StatePass to get the index of the StateAttribute
       *  from; default is 0.
       *  \param key Optional VariantKey of the StateVariant to use the StatePass in; default
       *  is 0.
       *  \return The index of \a attribute in the StatePass \a passIndex in the specified
       *  StateVariant, or -1 if it is not part of.
       *  \note The behavior is undefined if there is no StateVariant associated with the
       *  VariantKey \a key, or if \a passIndex is larger than or equal to the number of StatePass
       *  objects in the specified StateVariant.
       *  \sa StateAttribute, StatePass, StateVariant, VariantKey, addAttribute, getAttribute,
       *  getNumberOfAttributes, removeAttribute */
      NVSG_API size_t getAttributeIndex( StateAttributeHandle * attribute
                                       , size_t passIndex = 0
                                       , VariantKey key = 0 ) const;

      /*! \brief Add a StateAttribute to the specified StatePass.
       *  \param attribute A pointer to the constant StateAttribute to add.
       *  \param passIndex Optional index of the StatePass to add \a attribute to; default is 0.
       *  \param key Optional VariantKey of the StateVariant to use the StatePass in; default
       *  is 0.
       *  \remarks The reference count of \a attribute is increased. If there already was a
       *  StateAttribute of the same type in the specified StatePass, the reference count of that
       *  StateAttribute is decremented and then it is removed.\n
       *  If there is no StateVariant associated to the VariantKey \a key in this StateSet, a new
       *  StateVariant associated to \a key is created and added. If passIndex is greater than or
       *  equal to the number of StatePass objects in the specified StateVariant, a new StatePass
       *  is created and added as the last one, before \a attribute is added to that StatePass.
       *  \sa StateAttribute, StatePass, StateVariant, VariantKey, getAttribute,
       *  getAttributeIndex, getNumberOfAttributes, removeAttribute */
      NVSG_API void addAttribute( StateAttributeHandle * attribute
                                , size_t passIndex = 0
                                , VariantKey key = 0 );

      /*! \brief Remove the StateAttribute at the specified position.
       *  \param attributeIndex The index of the StateAttribute to remove.
       *  \param passIndex Optional index of the StatePass to remove the StateAttribute from;
       *  default is 0.
       *  \param key Optional VariantKey of the StateVariant to use the StatePass in; default
       *  is 0.
       *  \remarks The reference count of the StateAttribute to remove is decreased.
       *  \note The behavior is undefined if there is no StateVariant associated with the
       *  VariantKey \a key, or if \a passIndex is larger than or equal to the number of StatePass
       *  objects in the specified StateVariant, or if \a attributeIndex is larger than or
       *  equal to the number of StateAttribute objects in the specified StatePass.
       *  \sa StateAttribute, StatePass, StateVariant, VariantKey, addAttribute, getAttribute,
       *  getAttributeIndex, getNumberOfAttributes */
      NVSG_API void removeAttribute( size_t attributeIndex
                                   , size_t passIndex = 0
                                   , VariantKey key = 0 );

      /*! \brief Remove the given StateAttribute from the specified StatePass.
       *  \param attribute A pointer to the constant StateAttribute to remove.
       *  \param passIndex Optional index of the StatePass to remove \a attribute from; default is 0.
       *  \param key Optional VariantKey of the StateVariant to use the StatePass in; default
       *  is 0.
       *  \remarks If \a attribute is part of the specified StatePass, it is removed.
       *  \note The behavior is undefined if there is no StateVariant associated with the
       *  VariantKey \a key, or if \a passIndex is larger than or equal to the number of StatePass
       *  objects in the specified StateVariant.
       *  \sa StateAttribute, StatePas, StateVariant, VariantKey, addAttribute, getAttributeIndex,
       *  removeAttributeByObjectCode */
      NVSG_API void removeAttribute( StateAttributeHandle * attribute
                                   , size_t passIndex = 0
                                   , VariantKey key = 0 );

      /*! \brief Remove the StateAttribute with the specified object code from the specified StatePass.
       *  \param objectCode The object code of the StateAttribute to remove.
       *  \param passIndex Optional index of the StatePass to remove \a attribute from; default is 0.
       *  \param key Optional VariantKey of the StateVariant to use the StatePass in; default
       *  is 0.
       *  \remarks If there is a StateAttribute with the object code \a objectCode in the
       *  specified StatePass, it is removed.
       *  \note The behavior is undefined if there is no StateVariant associated with the
       *  VariantKey \a key, or if \a passIndex is larger than or equal to the number of StatePass
       *  objects in the specified StateVariant.
       *  \sa StateAttribute, StatePass, StateVariant, VariantKey, addAttribute,
       *  getAttributeByObjectCode, removeAttribute */
      NVSG_API void removeAttributeByObjectCode( unsigned int objectCode
                                               , size_t passIndex = 0
                                               , VariantKey key = 0 );

      /*! \brief Ask if the currently active StateVariant is transparent.
       *  \return \c true, if the currently active StateVariant is transparent, otherwise \c false.
       *  \remarks A StateVariant is considered to be transparent, if at least one of its
       *  StatePass objects is transparent.
       *  \sa invalidateTransparencyContainment */
      NVSG_API bool isTransparent() const;

      /*! \brief Invalidate the transparency containment cache.
       *  \remarks If the transparency containment cache is valid, it is marked as invalid, and
       *  the transparency containment cache of each owner is invalidated.
       *  \sa isTransparent */
      NVSG_API void invalidateTransparencyContainment();

      /*! \brief Ask if the currently active StateVariant contains any StateAttribute of type CgFx.
       *  \return \c true, if the currently active StateVariant contains a StateAttribute of type
       *  CgFx, otherwise \c false.
       *  \sa invalidateCgFxContainment */
      NVSG_API bool containsCgFx() const;

      /*! \brief Invalidate the CgFx containment cache.
       *  \remarks If the CgFx containment cache is valid, it is marked as invalid, and all the
       *  CgFx containment cache of each owner is invalidated.
       *  \sa containsCgFx */
      NVSG_API void invalidateCgFxContainment();

      /*! \brief Assignment operator
       *  \param rhs A reference to the constant StateSet to copy from
       *  \return A reference to the assigned StateSet
       *  \remarks The assignment operator calls the assignment operator of OwnedObject. Then any
       *  StateVariant attached to this StateSet is removed, and all StateVariant objects of \a
       *  rhs are copied.
       *  \sa OwnedObject, StateVariant */
      NVSG_API StateSet & operator=(const StateSet & rhs);

      /*! \brief Test for equivalence with an other StateSet.
       *  \param p A reference to the constant StateSet to test for equivalence with.
       *  \param ignoreNames Optional parameter to ignore the names of the objects; default is \c
       *  true.
       *  \param deepCompare Optional parameter to perform a deep comparsion; default is \c false.
       *  \return \c true if the StateSet \a p is equivalent to \c this, otherwise \c false.
       *  \remarks If \a p and \c this are equivalent as Object, they are equivalent if they have
       *  the same number of StateVariant/VariantKey pairs and have pair-wise equal VariantKey values.
       *  If \a deepCompare is \c true, they are equivalent if the StateVariant objects associated
       *  with the same VariantKey values are pair-wise equivalent. If \a deepCompare is \c false,
       *  they are equivalent if they are pair-wise the same pointers.
       *  \note The behavior is undefined if \a p is not a StateSet nor derived from one.
       *  \sa Object, StateVariant, VariantKey */
      NVSG_API virtual bool isEquivalent( const Object * p
                                        , bool ignoreNames
                                        , bool deepCompare ) const;

    protected:
      /*! \brief Invalidate the nvutil::Incarnation.
       *  \remarks This function is called from the framework when the nvutil::Incarnation of this
       *  StateSet has increased and now is to be marked as invalid. It first increases the
       *  nvutil::Incarnation of all it's owners, then it's own nvutil::Incarnation is marked as
       *  invalid.
       *  \sa nvutil::Incarnation, validateIncarnation */
      NVSG_API virtual void invalidateIncarnation();      // from bottom to top

      /*! \brief Validate the Incarnation. 
       *  \remarks This function is called from the framework when the Incarnation is requested
       *  and it is marked as invalid. Then it is validated and the Incarnation of all its
       *  StateVariant objects is requested to continue validation down the tree hierarchy.
       *  \sa nvutil::Incarnation, StateVariant, invalidateIncarnation */
      NVSG_API virtual void validateIncarnation() const;  // from top to bottom

    private:
      VariantKey m_variantKey;
      std::map<VariantKey,StateVariantHandle *> m_variants;
      mutable unsigned m_flags;
  };

#if defined( _DEBUG )
  inline bool find( std::map<VariantKey,StateVariantHandle*>::const_iterator it
                  , const std::map<VariantKey,StateVariantHandle*> &m )
  {
    for ( std::map<VariantKey,StateVariantHandle*>::const_iterator i = m.begin() ; i!=m.end() ; ++i )
    {
      if ( i == it )
      {
        return( true );
      }
    }
    return( false );
  }
#endif

  inline VariantKey StateSet::getActiveVariantKey() const
  {
    NVSG_TRACE();
    return( m_variantKey );
  }

  inline StateVariantHandle * StateSet::getActiveVariant() const
  {
    NVSG_TRACE();
    return( getVariant( m_variantKey ) );
  }

  inline size_t StateSet::getNumberOfVariants() const
  {
    NVSG_TRACE();
    return( m_variants.size() );
  }

  inline StateVariantHandle * StateSet::getVariant( VariantKey key ) const
  {
    NVSG_TRACE();
    std::map<VariantKey,StateVariantHandle*>::const_iterator vi=m_variants.find( key );
    return( ( vi != m_variants.end() ) ? vi->second : NULL );
  }

  inline VariantIterator StateSet::getFirstVariantIterator() const
  {
    NVSG_TRACE();
    return( VariantIterator(m_variants.begin()) );
  }

  inline VariantIterator StateSet::getNextVariantIterator( VariantIterator it ) const
  {
    NVSG_TRACE();
    return( VariantIterator(++it) );
  }

  inline VariantIterator StateSet::getLastVariantIterator() const
  {
    NVSG_TRACE();
    return( VariantIterator(m_variants.end()) );
  }

  inline StateVariantHandle * StateSet::getVariant( VariantIterator it ) const
  {
    NVSG_TRACE();
#if defined( _DEBUG )
    NVSG_ASSERT( find( it, m_variants ) );
#endif
    return( it->second );
  }

  inline VariantKey StateSet::getVariantKey( VariantIterator it ) const
  {
    NVSG_TRACE();
#if defined( _DEBUG )
    NVSG_ASSERT( find( it, m_variants ) );
#endif
    return( it->first );
  }

  inline size_t StateSet::getNumberOfPasses( VariantKey key ) const
  {
    NVSG_TRACE();
    std::map<VariantKey,StateVariantHandle*>::const_iterator vi=m_variants.find( key );
    if ( vi != m_variants.end() )
    {
      nvutil::ReadableObject<StateVariant> v(vi->second);
      return v->getNumberOfPasses();
    }
    return 0;
  }

  inline StatePassHandle * StateSet::getPass( size_t passIndex, VariantKey key ) const
  {
    NVSG_TRACE();
    std::map<VariantKey,StateVariantHandle*>::const_iterator vi=m_variants.find( key );
    NVSG_ASSERT( vi != m_variants.end() );
    nvutil::ReadableObject<StateVariant> v(vi->second);
    return v->getPass(passIndex);
  }

  inline size_t StateSet::getNumberOfAttributes( size_t passIndex, VariantKey key ) const
  {
    NVSG_TRACE();
    std::map<VariantKey,StateVariantHandle*>::const_iterator vi = m_variants.find( key );
    if ( vi != m_variants.end() )
    {
      nvutil::ReadableObject<StateVariant> v(vi->second);
      return v->getNumberOfAttributes(passIndex);
    }
    return 0;
  }

  inline StateAttributeHandle * StateSet::getAttribute( size_t attributeIndex, size_t passIndex, VariantKey key ) const
  {
    NVSG_TRACE();
    std::map<VariantKey,StateVariantHandle*>::const_iterator vi=m_variants.find( key );
    NVSG_ASSERT( vi != m_variants.end() );
    nvutil::ReadableObject<StateVariant> v(vi->second);
    return v->getAttribute( attributeIndex, passIndex );
  }

  inline StateAttributeHandle * StateSet::getAttributeByObjectCode( unsigned int objectCode, size_t passIndex, VariantKey key ) const
  {
    NVSG_TRACE();
    std::map<VariantKey,StateVariantHandle*>::const_iterator vi=m_variants.find( key );
    NVSG_ASSERT( vi != m_variants.end() );
    nvutil::ReadableObject<StateVariant> v(vi->second);
    return( v->getAttributeByObjectCode( objectCode, passIndex ) );
  }

  inline size_t StateSet::getAttributeIndex( StateAttributeHandle * attribute, size_t passIndex, VariantKey key ) const
  {
    NVSG_TRACE();
    std::map<VariantKey,StateVariantHandle*>::const_iterator vi=m_variants.find( key );
    NVSG_ASSERT( vi != m_variants.end() );
    nvutil::ReadableObject<StateVariant> v(vi->second);
    return( v->getAttributeIndex( attribute, passIndex ) );
  }

} //  namespace nvsg
