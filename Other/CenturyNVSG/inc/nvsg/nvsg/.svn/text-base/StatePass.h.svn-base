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
#include "nvutil/BitMask.h"

namespace nvsg
{
  /*! \brief Container class to hold a number of StateAttribute objects.
   *  \par Namespace: nvsg
   *  \remarks A StatePass holds a number of StateAttribute objects, that together define one
   *  rendering pass. A StatePass can be the child of one or more StateVariant objects.
   *  \sa StateAttribute, StateVariant */
  class StatePass : public OwnedObject<StateVariantHandle>
  {
    friend class StateVariant;

    public:
      /*! \brief Default-constructs a StatePass. 
       *  \remarks An empty StatePass is created. */
      NVSG_API StatePass();

      /*! \brief Constructs a StatePass from an Object. 
       *  \remarks An empty StatePass is created. */
      NVSG_API StatePass( const Object &rhs );

      /*! \brief Constructs a StatePass as a copy from another StatePass. */ 
      NVSG_API StatePass( const StatePass& rhs );

      /*! \brief Destructs a StatePass. */
      NVSG_API virtual ~StatePass();

      /*! \brief Get the number of StateAttribute objects in this StatePass.
       *  \return The number of StateAttribute objects in this StatePass.
       *  \sa StateAttribute, addAttribute, getAttribute, getAttributeIndex, removeAttribute */
      NVSG_API size_t getNumberOfAttributes() const;

      /*! \brief Get the StateAttribute at the specified position.
       *  \param index The index of the StateAttribute to get.
       *  \return A pointer to the constant StateAttribute at position \a index.
       *  \note The behavior is undefined if \a index is larger than or equal to the number of
       *  StateAttribute objects of this StatePass.
       *  \sa StateAttribute, addAttribute, getNumberOfAttributes, getAttributeByObjectCode,
       *  removeAttribute */
      NVSG_API StateAttributeHandle * getAttribute( size_t index ) const;

      /*! \brief Get the StateAttribute with the object code \a objectCode.
       *  \param objectCode The object code of the StateAttribute to get.
       *  \return A pointer to the constant StateAttribute with the object code \a objectCode, or
       *  NULL, if there is none.
       *  \sa StateAttribute, addAttribute, getAttribute, removeAttributeByObjectCode */
      NVSG_API StateAttributeHandle * getAttributeByObjectCode( unsigned int objectCode ) const;

      /*! \brief Get the index of the specified StateAttribute in this StatePass.
       *  \param attribute A pointer to the constant StateAttribute to get the index in this StatePass for.
       *  \return The index of \a attribute in this StatePass, or -1 if it is not part of this StatePass.
       *  \sa StateAttribute, addAttribute, getAttribute, getNumberOfAttributes, removeAttribute */
      NVSG_API size_t getAttributeIndex( StateAttributeHandle * attribute ) const;

      /*! \brief Add a StateAttribute to this StatePass.
       *  \param attribute A pointer to the constant StateAttribute to add.
       *  \remarks The reference count of \a attribute is increased. If there already was a
       *  StateAttribute of the same type in this StatePass, the reference count of that
       *  StateAttribute is decremented and then it is removed.
       *  \sa StateAttribute, getAttribute, getAttributeIndex, getNumberOfAttributes,
       *  removeAttribute */
      NVSG_API void addAttribute( StateAttributeHandle * attribute );

      /*! \brief Remove the StateAttribute at the specified position.
       *  \param index The index of the StateAttribute to remove.
       *  \remarks The reference count of the StateAttribute to remove is decreased.
       *  \note The behavior is undefined if \a index is larger than or equal to the number of
       *  StateAttribute objects of this StatePass.
       *  \sa StateAttribute, addAttribute, getAttribute, getAttributeIndex, getNumberOfAttributes */
      NVSG_API void removeAttribute( size_t index );

      /*! \brief Remove the given StateAttribute.
       *  \param attribute A pointer to the constant StateAttribute to remove.
       *  \remarks If \a attribute is part of this StatePass, it is removed.
       *  \sa StateAttribute, addAttribute, getAttributeIndex, removeAttributeByObjectCode */
      NVSG_API void removeAttribute( StateAttributeHandle * attribute );

      /*! \brief Remove the StateAttribute with the specified object code.
       *  \param objectCode The object code of the StateAttribute to remove.
       *  \remarks If there is a StateAttribute with the object code \a objectCode in this
       *  StatePass, it is removed.
       *  \sa StateAttribute, addAttribute, getAttributeByObjectCode, removeAttribute */
      NVSG_API void removeAttributeByObjectCode( unsigned int objectCode );

      /*! \brief Ask if this StatePass is transparent.
       *  \return \c true, if this StatePass is transparent, otherwise \c false.
       *  \remarks A StatePass is considered to be transparent, if at least one of its
       *  StateAttribute objects is transparent.
       *  \sa invalidateTransparencyContainment */
      NVSG_API bool isTransparent() const;

      /*! \brief Invalidate the transparency containment cache.
       *  \remarks If the transparency containment cache is valid, it is marked as invalid, and
       *  the transparency containment cache of each owner is invalidated.
       *  \sa isTransparent */
      NVSG_API void invalidateTransparencyContainment();

      /*! \brief Ask if this StatePass contains any StateAttribute of type CgFx.
       *  \return \c true, if this StatePass contains a StateAttribute of type CgFx, otherwise \c
       *  false.
       *  \sa invalidateCgFxContainment */
      NVSG_API bool containsCgFx() const;

      /*! \brief Invalidate the CgFx containment cache.
       *  \remarks If the CgFx containment cache is valid, it is marked as invalid, and all the
       *  CgFx containment cache of each owner is invalidated.
       *  \sa containsCgFx */
      NVSG_API void invalidateCgFxContainment();

      /*! \brief Assignment operator
       *  \param rhs A reference to the constant StatePass to copy from
       *  \return A reference to the assigned StatePass
       *  \remarks The assignment operator calls the assignment operator of OwnedObject. Then any
       *  StateAttribute attached to this StatePass is removed, and all StateAttribute objects
       *  of \a rhs are copied.
       *  \sa OwnedObject, StateAttribute */
      NVSG_API StatePass & operator=(const StatePass & rhs);

      /*! \brief Test for equivalence with an other StatePass.
       *  \param p A reference to the constant StatePass to test for equivalence with.
       *  \param ignoreNames Optional parameter to ignore the names of the objects; default is \c
       *  true.
       *  \param deepCompare Optional parameter to perform a deep comparsion; default is \c false.
       *  \return \c true if the StatePass \a p is equivalent to \c this, otherwise \c false.
       *  \remarks If \a p and \c this are equivalent as Object, they are equivalent if their
       *  StateAttribute objects are equivalent. If \a deepCompare is \c true, they are equivalent
       *  if for each StateAttribute in \c this there is an equivalent StateAttribute in \a p. If
       *  \a deepCompare is \c false, they are equivalent if each StateAttribute in \c this is also
       *  found in \c p.
       *  \note The behavior is undefined if \a p is not a StatePass nor derived from one.
       *  \sa Object, StateAttribute */
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
       *  StateAttribute objects is requested to continue validation down the tree hierarchy.
       *  \sa nvutil::Incarnation, StateAttribute, invalidateIncarnation */
      NVSG_API virtual void validateIncarnation() const;  // from top to bottom

    private:
      void invalidateCaches( StateAttributeHandle *attribute, bool insert );

    private:
      typedef std::vector<StateAttributeHandle*>::const_iterator ConstAttributeIterator;
      typedef std::vector<StateAttributeHandle*>::iterator AttributeIterator;
      std::vector<StateAttributeHandle *> m_attribs;

      mutable unsigned int  m_flags;
  };

  inline size_t StatePass::getNumberOfAttributes() const
  {
    NVSG_TRACE();
    return( m_attribs.size() );
  }

  inline StateAttributeHandle * StatePass::getAttribute( size_t index ) const
  {
    NVSG_TRACE();
    NVSG_ASSERT( index < m_attribs.size() );
    return( m_attribs[index] );
  }

  inline size_t StatePass::getAttributeIndex( StateAttributeHandle * attribute ) const
  {
    NVSG_TRACE();
    std::vector<StateAttributeHandle*>::const_iterator ai =
      std::find( m_attribs.begin(), m_attribs.end(), attribute );
    return( ( ai != m_attribs.end() ) ? ai - m_attribs.begin() : -1 );
  }

} //  namespace nvsg
