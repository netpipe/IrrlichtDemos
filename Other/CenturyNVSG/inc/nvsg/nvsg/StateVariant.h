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
#include "nvsg/OwnedObject.h"
#include "nvsg/StatePass.h"

namespace nvsg
{
  class StateSet;

  /*! \brief Container class to hold a number of StatePass objects.
   *  \par Namespace: nvsg
   *  \remarks A StateVariant holds a number of StatePass objects. Any geometries associated with
   *  this StateVariant is rendered with each of the StatePass objects, one after the other.\n
   *  A StateVariant can be the child of one or more StateSet objects.
   *  \sa StatePass, StateSet */
  class StateVariant : public OwnedObject<StateSetHandle>
  {
    friend class StateSet;

    public:
      /*! \brief Default-constructs a StateVariant. 
       *  \remarks An empty StateVariant is created. */
      NVSG_API StateVariant();

      /*! \brief Constructs a StateVariant from an Object. 
       *  \remarks An empty StateVariant is created. */
      NVSG_API StateVariant( const Object &rhs );

      /*! \brief Constructs a StateVariant as a copy from another StateVariant. */
      NVSG_API StateVariant( const StateVariant& rhs );

      /*! \brief Destructs a StateVariant. */
      NVSG_API virtual ~StateVariant();

      /*! \brief Get the number of StatePass objects in this StateVariant.
       *  \return The number of StatePass objects in this StateVariant.
       *  \sa StatePass, addPass, getPass, removePass, replacePass */
      NVSG_API size_t getNumberOfPasses() const;

      /*! \brief Get the StatePass at the specified position.
       *  \param index The index of the StatePass to get.
       *  \return A pointer to the constant StatePass at position \a index.
       *  \note The behavior is undefined if \a index is larger than or equal to the number of
       *  StatePass objects of this StateVariant.
       *  \sa StatePass, addPass, getNumberOfPasses, removePass, replacePass */
      NVSG_API StatePassHandle * getPass( size_t index ) const;

      /*! \brief Add a StatePass to this StateVariant.
       *  \param pass A pointer to the constant StatePass to add.
       *  \remarks The reference count of \a pass is increased.
       *  \sa StatePass, getNumberOfPasses, getPass, removePass, replacePass */
      NVSG_API void addPass( StatePassHandle * pass );

      /*! \brief Remove the StatePass at the specified position.
       *  \param passIndex The index of the StatePass to remove.
       *  \remarks The reference count of the StatePass to remove is decreased.
       *  \note The behavior is undefined if \a index is larger than or equal to the number of
       *  StatePass objects of this StateVariant.
       *  \sa StatePass, addPass, getNumberOfPasses, getPass, replacePass */
      NVSG_API void removePass( size_t passIndex );

      /*! \brief Replace the StatePass at the specified position.
       *  \param pass A pointer to the constant StatePass to replace with.
       *  \param index The index of the StatePass in this StateVariant to replace.
       *  \remarks The reference count of \a pass is incremented, and the reference count of the
       *  StatePass at position \a index in this StateVariant is decremented.
       *  \note The behavior is undefined if \a index is larger than or equal to the number of
       *  StatePass objects of this StateVariant.
       *  \sa StatePass, addPass, getNumberOfPasses, getPass */
      NVSG_API void replacePass( StatePassHandle * pass
                               , size_t index );

      /*! \brief Get the number of StateAttribute objects in the specified StatePass.
       *  \param passIndex Optional index of the StatePass to get the number of StateAttribute
       *  objects from; default is 0.
       *  \return The number of StateAttribute objects in the StatePass at position \a passIndex
       *  in this StateVariant.
       *  \sa StateAttribute, addAttribute, getAttribute, getAttributeIndex, removeAttribute */
      NVSG_API size_t getNumberOfAttributes( size_t passIndex = 0 ) const;

      /*! \brief Get the StateAttribute at the specified position.
       *  \param attributeIndex The index of the StateAttribute to get.
       *  \param passIndex Optional index of the StatePass to get the StateAttribute from; default
       *  is 0.
       *  \return A pointer to the constant StateAttribute at the specified position.
       *  \note The behavior is undefined if \a passIndex is larger than or equal to the number
       *  of StatePass objects in this StateVariant, or if \a attributeIndex is larger than or
       *  equal to the number of StateAttribute objects in the specified StatePass in this
       *  StateVariant.
       *  \sa StateAttribute, addAttribute, getNumberOfAttributes, getAttributeByObjectCode,
       *  removeAttribute */
      NVSG_API StateAttributeHandle * getAttribute( size_t attributeIndex
                                                  , size_t passIndex = 0 ) const;

      /*! \brief Get the StateAttribute with the object code \a objectCode in the specified
       *  StatePass.
       *  \param objectCode The object code of the StateAttribute to get.
       *  \param passIndex Optional index of the StatePass to get the StateAttribute from; default
       *  is 0.
       *  \return A pointer to the constant StateAttribute with the object code \a objectCode, or
       *  NULL, if there is none.
       *  \note The behaviour is undefined, if \a passIndex is larger than or equal to the number
       *  of StatePass objects in this StateVariant.
       *  \sa StateAttribute, addAttribute, getAttribute, removeAttributeByObjectCode */
      NVSG_API StateAttributeHandle * getAttributeByObjectCode( unsigned int objectCode
                                                              , size_t passIndex = 0 ) const;

      /*! \brief Get the index of the StateAttribute in the specified StatePass.
       *  \param attribute A pointer to the constant StateAttribute of the index to get.
       *  \param passIndex Optional index of the StatePass containing the index of the StateAttribute;
       *  default is 0.
       *  \return The index of \a attribute in the StatePass \a passIndex in this StateVariant,
       *  or -1 if it is not part of.
       *  \note The behavior is undefined if \a passIndex is larger than or equal to the number
       *  of StatePass objects in this StateVariant.
       *  \sa StateAttribute, addAttribute, getAttribute, getNumberOfAttributes, removeAttribute */
      NVSG_API size_t getAttributeIndex( StateAttributeHandle * attribute
                                       , size_t passIndex = 0 ) const;

      /*! \brief Add a StateAttribute to the specified StatePass.
       *  \param attribute A pointer to the constant StateAttribute to add.
       *  \param passIndex Optional index of the StatePass to add \a attribute to; default is 0.
       *  \remarks The reference count of \a attribute is increased. If there already was a
       *  StateAttribute of the same type in the specified StatePass, the reference count of that
       *  StateAttribute is decremented and then it is removed.\n
       *  If passIndex is greater than or equal to the number of StatePass objects in this
       *  StateVariant, a new StatePass is created and added as the last one, before \a attribute
       *  is added to that StatePass.
       *  \sa StateAttribute, getAttribute, getAttributeIndex, getNumberOfAttributes,
       *  removeAttribute */
      NVSG_API void addAttribute( StateAttributeHandle * attribute
                                , size_t passIndex = 0 );

      /*! \brief Remove the StateAttribute at the specified position.
       *  \param attributeIndex The index of the StateAttribute to remove.
       *  \param passIndex Optional index of the StatePass to remove the StateAttribute from;
       *  default is 0.
       *  \remarks The reference count of the StateAttribute to remove is decreased.
       *  \note The behavior is undefined if \a passIndex is larger than or equal to the number
       *  of StatePass objects in this StateVariant, or if \a attributeIndex is larger than or
       *  equal to the number of StateAttribute objects in the specified StatePass in this
       *  StateVariant.
       *  \sa StateAttribute, addAttribute, getAttribute, getAttributeIndex, getNumberOfAttributes */
      NVSG_API void removeAttribute( size_t attributeIndex
                                   , size_t passIndex = 0 );

      /*! \brief Remove the given StateAttribute from the specified StatePass.
       *  \param attribute A pointer to the constant StateAttribute to remove.
       *  \param passIndex Optional index of the StatePass to remove \a attribute from; default is 0.
       *  \remarks If \a attribute is part of the specified StatePass, it is removed.
       *  \note The behavior is undefined if \a passIndex is larger than or equal to the number
       *  of StatePass objects in this StateVariant.
       *  \sa StateAttribute, addAttribute, getAttributeIndex, removeAttributeByObjectCode */
      NVSG_API void removeAttribute( StateAttributeHandle * attribute
                                   , size_t passIndex = 0 );

      /*! \brief Remove the StateAttribute with the specified object code from the specified StatePass.
       *  \param objectCode The object code of the StateAttribute to remove.
       *  \param passIndex Optional index of the StatePass to remove \a attribute from; default is 0.
       *  \remarks If there is a StateAttribute with the object code \a objectCode in the
       *  specified StatePass, it is removed.
       *  \note The behavior is undefined if \a passIndex is larger than or equal to the number
       *  of StatePass objects in this StateVariant.
       *  \sa StateAttribute, addAttribute, getAttributeByObjectCode, removeAttribute */
      NVSG_API void removeAttributeByObjectCode( unsigned int objectCode
                                               , size_t passIndex );

      /*! \brief Ask if this StateVariant is transparent.
       *  \return \c true, if this StateVariant is transparent, otherwise \c false.
       *  \remarks A StateVariant is considered to be transparent if at least one of its
       *  StatePass objects is transparent.
       *  \sa invalidateTransparencyContainment, isEachPassTransparent */
      NVSG_API bool isTransparent() const;

      /*! \brief Ask if this each StatePass of this StateVariant is transparent.
       *  \return \c true, if each StatePass of this StateVariant is transparent, otherwise \c false.
       *  \sa invalidateTransparencyContainment, isTransparent */
      NVSG_API bool isEachPassTransparent() const;

      /*! \brief Invalidate the transparency containment cache.
       *  \remarks If the transparency containment cache is valid, it is marked as invalid, and
       *  the transparency containment cache of each owner is invalidated.
       *  \sa isTransparent */
      NVSG_API void invalidateTransparencyContainment();

      /*! \brief Ask if this StateVariant contains any StateAttribute of type CgFx.
       *  \return \c true, if this StateVariant contains a StateAttribute of type CgFx, otherwise \c
       *  false.
       *  \sa invalidateCgFxContainment */
      NVSG_API bool containsCgFx() const;

      /*! \brief Invalidate the CgFx containment cache.
       *  \remarks If the CgFx containment cache is valid, it is marked as invalid, and all the
       *  CgFx containment cache of each owner is invalidated.
       *  \sa containsCgFx */
      NVSG_API void invalidateCgFxContainment();

      /*! \brief Assignment operator
       *  \param rhs A reference to the constant StateVariant to copy from
       *  \return A reference to the assigned StateVariant
       *  \remarks The assignment operator calls the assignment operator of OwnedObject. Then any
       *  StatePass attached to this StateVariant is removed, and all StatePass objects of \a rhs
       *  are copied.
       *  \sa OwnedObject, StatePass */
      NVSG_API StateVariant & operator=( const StateVariant & rhs );

      /*! \brief Test for equivalence with an other StateVariant.
       *  \param p A reference to the constant StateVariant to test for equivalence with.
       *  \param ignoreNames Optional parameter to ignore the names of the objects; default is \c
       *  true.
       *  \param deepCompare Optional parameter to perform a deep comparsion; default is \c false.
       *  \return \c true if the StateVariant \a p is equivalent to \c this, otherwise \c false.
       *  \remarks If \a p and \c this are equivalent as Object, they are equivalent, if their
       *  StatePass objects are equivalent. If \a deepCompare is \c true, they are equivalent
       *  if the StatePass objects are pairwise equivalent. If \a deepCompare is \c false, they
       *  are equivalent, if they are pair-wise the same pointers.
       *  \note The behavior is undefined if \a p is not a StateVariant nor derived from one.
       *  \sa Object, StatePass */
      NVSG_API virtual bool isEquivalent( const Object * p
                                        , bool ignoreNames
                                        , bool deepCompare ) const;

    protected:
      /*! \brief Invalidate the nvutil::Incarnation.
       *  \remarks This function is called from the framework when the nvutil::Incarnation of this
       *  StatePass has increased and now is to be marked as invalid. It first increases the
       *  nvutil::Incarnation of all its owners, then its own nvutil::Incarnation is marked as
       *  invalid.
       *  \sa nvutil::Incarnation, validateIncarnation */
      NVSG_API virtual void invalidateIncarnation();      // from bottom to top

      /*! \brief Validate the Incarnation. 
       *  \remarks This function is called from the framework when the Incarnation is requested
       *  and it is marked as invalid. Then it is validated and the Incarnation of all its
       *  StatePass objects is requested to continue validation down the tree hierarchy.
       *  \sa nvutil::Incarnation, StatePass, invalidateIncarnation */
      NVSG_API virtual void validateIncarnation() const;  // from top to bottom

    private:
      typedef std::vector<StatePassHandle*>::const_iterator ConstPassIterator;
      typedef std::vector<StatePassHandle*>::iterator PassIterator;
      std::vector<StatePassHandle *>  m_passes;

      mutable unsigned int  m_flags;
  };

  inline size_t StateVariant::getNumberOfPasses() const
  {
    NVSG_TRACE();
    return( m_passes.size() );
  }

  inline StatePassHandle * StateVariant::getPass( size_t passIndex ) const
  {
    NVSG_TRACE();
    NVSG_ASSERT( passIndex < m_passes.size() );
    return( m_passes[passIndex] );
  }

  inline size_t StateVariant::getNumberOfAttributes( size_t passIndex ) const
  {
    NVSG_TRACE();
    if ( passIndex < m_passes.size() ) 
    {
      nvutil::ReadableObject<StatePass> pass(m_passes[passIndex]);
      return pass->getNumberOfAttributes();
    }
    return 0;
  }

  inline StateAttributeHandle * StateVariant::getAttribute( size_t attributeIndex, size_t passIndex ) const
  {
    NVSG_TRACE();
    NVSG_ASSERT( passIndex < m_passes.size() );
    nvutil::ReadableObject<StatePass> pass(m_passes[passIndex]);
    return pass->getAttribute(attributeIndex);
  }

  inline StateAttributeHandle * StateVariant::getAttributeByObjectCode( unsigned int objectCode, size_t passIndex ) const
  {
    NVSG_TRACE();
    NVSG_ASSERT( passIndex < m_passes.size() );
    nvutil::ReadableObject<StatePass> pass(m_passes[passIndex]);
    return pass->getAttributeByObjectCode(objectCode);
  }

  inline size_t StateVariant::getAttributeIndex( StateAttributeHandle * attribute, size_t passIndex ) const
  {
    NVSG_TRACE();
    NVSG_ASSERT( passIndex < m_passes.size() );
    nvutil::ReadableObject<StatePass> pass(m_passes[passIndex]);
    return pass->getAttributeIndex(attribute);
  }

  inline void StateVariant::removeAttribute( size_t attributeIndex, size_t passIndex )
  {
    NVSG_TRACE();
    NVSG_ASSERT( passIndex < m_passes.size() );
    nvutil::WritableObject<StatePass> sp(m_passes[passIndex]);
    sp->removeAttribute(attributeIndex);
  }

  inline void StateVariant::removeAttribute( StateAttributeHandle * attribute, size_t passIndex )
  {
    NVSG_TRACE();
    NVSG_ASSERT( passIndex < m_passes.size() );
    nvutil::WritableObject<StatePass> sp(m_passes[passIndex]);
    sp->removeAttribute(attribute);
  }

  inline void StateVariant::removeAttributeByObjectCode( unsigned int objectCode, size_t passIndex )
  {
    NVSG_TRACE();
    NVSG_ASSERT( passIndex < m_passes.size() );
    nvutil::WritableObject<StatePass> sp(m_passes[passIndex]);
    sp->removeAttributeByObjectCode(objectCode);
  }

} //  namespace nvsg
