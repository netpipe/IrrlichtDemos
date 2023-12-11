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
#include "nvsg/Drawable.h"
#include "nvsg/StateSet.h"
#include "nvsg/Object.h"
#include "nvmath/Sphere3f.h"
#include "nvutil/BitMask.h"
#include <vector>

namespace nvsg
{
/*! \brief Serves as base class for all tree nodes
  * \par Namespace: nvsg
  */
class Node : public Object
{
  public:
    /*! \brief Hints to give to a Node.
     *  \remarks There is a hint system part of Node. It allows to add some hinting information to
     *  that Node. This information might be considered by any Traverser, but it does not have to.
     *  A number of hints are predefined, some space for further hints is reserved for future use
     *  within NVSG, and some space is available to customer hints.\n
     *  All hints should be treated as some kind of exceptional behavior. That is, if the
     *  corresponding bit is set, the Node might be handled somewhat different than is the
     *  default. If it is not set, it is handled as usual.
     *  \par Example
     *  To prevent a Node from being culled (maybe because there's a CgFx effect attached that
     *  changes it's vertices) do the following:
     *  \code
     *    NodeHandle * hNode = [...]
     *    {
     *      WritableObject< Node > pNode( hNode );
     *      pNode->addHints( NH_CULL_NEVER );
     *    }
     *  \endcode
     *  \par Example
     *  To change the culling hint back to the default state, do the following:
     *  \code
     *    NodeHandle * hNode = [...]
     *    {
     *      WritableObject< Node > pNode( hNode );
     *      pNode->removeHints( NH_CULL_MASK );
     *    }
     *  \endcode
     *  \note With the hint NH_OVERLAY, a Node is placed in the overlay part of the scene. A Node
     *  in the overlay part of the scene is drawn after and above all other objects. Currently,
     *  to make sure that it's visible, the depth compare is off, and can not be changed for this
     *  Node and beneath. The Node objects marked to be in the overlay part are drawn from back
     *  to front, to make sure that the spatial relationship between them (but not with the rest
     *  of the scene!) is correct.
     *  \sa Node */
    enum NodeHints
    {
      NH_CULL_MASK              = BIT0 | BIT1,  //!< Mask for culling hints.
      NH_CULL_NEVER             = BIT0,         //!< Never cull this Node, it's always inside.
      NH_CULL_ALWAYS            = BIT1,         //!< Always cull this Node, it's always outside.

      NH_NO_SHADOW_CASTER       = BIT2,         //!< This Node does not cast shadows.
      NH_NO_SHADOW_RECEIVER     = BIT3,         //!< This Node does not receive shadows.

      NH_OVERLAY                = BIT4,         //!< This Node is to be placed in the overlay list.

      NH_MODIFY_FREQUENCY_MASK  = BIT5 | BIT6,  //!< Not yet used! Mask for modifying frequency hints.
      NH_MODIFY_FREQUENCY_LOW   = BIT5,         //!< Not yet used! This Node is rarely modified; aggressive optimization is allowed here
      NH_MODIFY_FREQUENCY_HIGH  = BIT6,         //!< Not yet used! This Node is often modified; no time consuming operations allowed here

      NH_RESERVED_BITS          = 0x80,         //!< These bits are reserved for future use.
      NH_CUSTOM_BITS            = 0xFF00,       //!< These bits are free for custom use.
    };

  public:

    /*! \brief Default-constructs a Node.
      */
    NVSG_API Node(void);

    /*! \brief Constructs a Node as a copy of another Node.
      */
    NVSG_API Node(const Node& rhs);

    /*! \brief Destructs a Node. 
      */
    NVSG_API virtual ~Node(void);

    /*! \brief Returns the number of parent Group nodes for this Node.
      * \returns The actual number of Group nodes that have this Node as child node. */
    NVSG_API size_t getNumberOfParents() const;

    /*! \brief Returns a parent Group by index.
      * \param
      * index Zero-based index of the parent Group to retrieve.
      * \return
      * The function returns a pointer to the \a index-th parent Group.
      * \remarks
      * The function together with Node::getNumberOfParents lets you iterate through all parent
      * Group nodes of the Node.
      * \n\n
      * The behavior is undefined if the function is called with an invalid index - that is, an
      * index greater than or equal to whatever Node::getNumberOfParents returns.
      */
    NVSG_API GroupHandle * getParent(size_t index) const;

    /*! \brief Returns the Node's bounding sphere.
      * \return
      * The function returns a reference to a nvmath::Sphere3f that represents the Node's 
      * bounding sphere.
      * \remarks
      * If the current bounding sphere turns out to be invalid, the function first calculates the 
      * bounding sphere from the containing geometry. 
      * \sa Node::calculateBoundingSphere
      */
    NVSG_API const nvmath::Sphere3f & getBoundingSphere() const;

    /*! \brief Invalidates the Node's bounding sphere.
      * \remarks
      * The function invalidates this Node's bounding sphere and that of all nodes up the 
      * hierarchy. This forces a re-calculation of the bounding sphere the next time the Node's
      * bounding sphere will be queried through Node::getBoundingSphere.
      */
    NVSG_API void invalidateBoundingSphere();

    /*! \brief Tests whether the Node contains transparent geometry.
      * \return
      * The function returns \c true if the Node contains transparent geometry. Otherwise, the
      * function returns \c false.
      * \remarks
      * The function iterates through the underlying hierarchy to detect transparent geometry.
      * Dependent on how complex the underlying hierarchy is, this could be an expensive operation. 
      * The result of this determination will be cached to make subsequent queries much faster as
      * long as the cache remains valid. The cache might become invalid as geometry will be 
      * added to or removed from the Node.
      * \sa Node::invalidateTransparencyContainment
      */
    NVSG_API bool containsTransparentGeometry() const;

    /*! \brief Invalidates the transparency containment cache.
      * \remarks
      * The function invalidates this Node's transparency containment cache and that of all nodes
      * up the hierarchy. This forces a re-calculation of these caches with the very next query
      * through Node::containsTransparentGeometry.
      */
    NVSG_API void invalidateTransparencyContainment();

    /*! \brief Tests whether the Node contains CgFx attributes.
      * \return
      * The function returns \c true if the Node contains CgFx attributes. Otherwise, the
      * function returns \c false.
      * \remarks
      * The function iterates through the underlying hierarchy to detect CgFx attributes.
      * Dependent on how complex the underlying hierarchy is, this could be an expensive operation. 
      * The result of this determination will be cached to make subsequent queries much faster as
      * long as the cache remains valid. The cache might become invalid as geometry will be added 
      * to or removed from the Node.
      * \sa Node::invalidateCgFxContainment
      */
    NVSG_API virtual bool containsCgFx() const;

    /*! \brief Invalidates the CgFx containment cache.
      * \remarks
      * The function invalidates this Node's CgFx containment cache and that of all nodes
      * up the hierarchy. This forces a re-calculation of these caches with the very next query
      * through Node::containsCgFx.
      */
    NVSG_API virtual void invalidateCgFxContainment();

    /*! \brief Returns whether lights are located down the scene graph hierarchy.
      * \return
      * The default implementation always returns \c false.
      * \remarks
      * Node derived classes that can contain LightSource objects should override this
      * function to return the proper result.
      * \sa invalidateLightContainment
      */
    NVSG_API virtual bool containsLight() const;

    /*! \brief Invalidates the LightSource containment cache.
      * \remarks
      * The function invalides this Node's LightSource containment cache and that of the superior 
      * hierarchy. 
      * \sa containsLight
      */
    NVSG_API virtual void invalidateLightContainment();

    /*! \brief Tests if this Node contains a VolumeNode. 
     *  \return \c false.
     *  \remarks Each class derived from Node that might contain a VolumeNode (like Group, or
     *  VolumeNode itself) should overwrite this virtual function. This default implementation
     *  just returns \c false, because a Node can not contain a VolumeNode.
     *  \sa invalidateVolumeNodeContainment */
    NVSG_API virtual bool containsVolumeNode() const;

    /*! \brief Invalidates the VolumenNode containment cache.
     *  \remarks The invalidateVolumeNodeContainment function of every parent of this Node is
     *  called.
     *  \sa containsVolumeNode */
    NVSG_API virtual void invalidateVolumeNodeContainment();

    /*! \brief Returns the number of animation frames contained.
      * \return
      * The function returns the actual number of contained animation frames.        
      * \remarks
      * The function iterates the underlying hierarchy for animations to query for the number of 
      * frames. As this might be an expensive operation, the result of the test will be cached to 
      * provide faster access with the next queries. If the animation state changes underneath 
      * this Group, the cache will become invalid, forcing a re-calculation of the result the 
      * very next query. 
      * \n\n
      * There currently is a restriction that a Scene can contain animations with only the exact same 
      * number of frames. 
      * \sa Node::invalidateNumberOfFrames
      */
    NVSG_API virtual size_t getNumberOfFrames() const;

    /*! \brief Invalidates the cache for the number of frames contained.
      * \remarks
      * The function invalidates this Nodes's cache for the number of frames contained and that of 
      * all Nodes up the hierarchy. This forces a re-calculation of these caches with the very 
      * next query through Node::getNumberOfFrames.
      */
    NVSG_API virtual void invalidateNumberOfFrames();

    /*! \brief Specifies an annotation to be appended to this Node.
      * \param
      * anno Reference to a STL string that holds the annotation to set.
      * \remarks
      * The function lets you append an optional annotation as wanted.
      * \sa Node::getAnnotation
      */
    NVSG_API void setAnnotation(const std::string& anno);

    /*! \brief Retrieves the Node's annotation. 
      * \return
      * The function returns a reference to a STL string holding the annotation as specified
      * through Node::setAnnotation. If no annotation was previously specified, the function returns
      * an empty string.
      * \sa Node::setAnnotation
      */
    NVSG_API const std::string& getAnnotation() const;

    /*! \brief Get the hints to use on this Node.
     *  \param mask An optional unsigned short to mask the hints to get. With the default mask
     *  every hint is returned.
     *  \return An unsigned short whose bits encode the hints on this Node.
     *  \sa setHints, addHints, removeHints, containsHints, invalidateHintsContainment */
    NVSG_API unsigned short getHints( unsigned short mask = 0xFFFF ) const;

    /*! \brief Set the hints to use on this Node.
     *  \param hints An unsigned short whose bits encode the hints on this Node.
     *  \sa NodeHints */
    NVSG_API void setHints( unsigned short hints );

    /*! \brief Convenience function to add some hints to the current set of hints.
     *  \param hints An unsigned short whose bits encode the hints to add to the set of hints on
     *  this Node.
     *  \remarks The current set of hints is bitwise or'ed with \a hints.
     *  \sa getHints, setHints, removeHints, containsHints, invalidateHintsContainment */
    NVSG_API void addHints( unsigned short hints );

    /*! \brief Convenience function to remove some hints from the current set of hints.
     *  \param hints An unsigned short whose bits encode the hints to remove from the set of
     *  hints on this Node.
     *  \remarks The current set of hints is bitwise and'ed with the bitwise complement of \a hints.
     *  \sa getHints, setHints, addHints, containsHints, invalidateHintsContainment */
    NVSG_API void removeHints( unsigned short hints );

    /*! \brief Query if this Node or something beneath contains the given hints.
     *  \param hints An unsigned short whose bits encode the hints that are queried.
     *  \return \c true, if each hint in \a hints is marked on this Node or somewhere beneath,
     *  Otherwise \c false.
     *  \remarks Internally calls \a getHintsContainment
     *  \sa getHints, setHints, addHints, removeHints, invalidateHintsContainment, getHintsContainment */
    NVSG_API bool containsHints( unsigned short hints ) const;

    /*! \brief Evaluate hints for this Node and all Nodes beneath for this set of hints
      * \param hints An unsigned short whose bits encode the hints that are queried.
      * \return An unsigned short that encodes the hints evaluated for the set of \a hints queried
      * \remarks This function is also called by \a containsHints
      * to determine if a set of hints is contained by a \a node or its children
      * \sa getHints, setHints, addHints, removeHints, invalidateHintsContainment, containsHints */
    NVSG_API unsigned short getHintsContainment( unsigned short hints ) const;

    /*! \brief Invalidates the containment flags for a set of hints.
     *  \param hints An unsigned short whose bits encode the hints to invalidate the hint
     *  containment for.
     *  \remarks If at least one hint in \a hints is valid in this Node, all the hints in \a hints
     *  are marked as invalid, and invalidateHintsContainment is called on each parent.
     *  \sa getHints, setHints, addHints, removeHints, containsHints */
    NVSG_API void invalidateHintsContainment( unsigned short hints );

  #if !defined( NDEBUG )
    /*! \brief Prompts an assertion failed message if the Node's bounding box is valid.
      * \remarks
      * This function is available only in the NVSG debug build.
      */
    NVSG_API void assertInvalidBoundingSphere() const;
  #endif

  protected:
    /*! \brief Called from the framework if re-calculation of the Node's bounding sphere is required.
      * \return
      * The function returns a nvmath::Sphere3f that represents the actual bounding sphere of this 
      * Node.
      * \remarks
      * The function is required to be implemented by concrete Node-derived classes to provide proper 
      * bounding sphere calculation.
      * \sa nvmath::Sphere3f
      */
    NVSG_API virtual nvmath::Sphere3f calculateBoundingSphere() const = 0;

    /*! \brief Determine the hints containment status of this Node.
     *  \param hints An unsigned short whose bits encode the hints containment to determine.
     *  \return An unsigned short whose bits encode the hints on this Node or'ed with the \a hints.
     *  \remarks This function is called by the framework, if \a getHintsContainment is called while the
     *  hint containment flags mark that information as invalid.
     *  \sa containsHints, invalidateHintsContainment, getHintsContainment */
    NVSG_API virtual unsigned short determineHintsContainment( unsigned short hints ) const;

    /*! \brief Called from the framework if re-determination of the transparency containment cache is 
      * required.
      * \return
      * The function returns \c true if the Node contains transparent geometry. Otherwise, the 
      * function returns \c false.
      * \remarks
      * The function is required to be implemented by concrete Node-derived classes to provide proper
      * determination of transparency containment.
      * \sa Node::containsTransparentGeometry, Node::invalidateTransparencyContainment
      */
    NVSG_API virtual bool determineTransparencyContainment() const = 0;

    /*! \brief Assigns new content from another Node. 
      * \param
      * rhs Reference to a Node from which to assign the new content.
      * \return
      * A reference to this object.
      * \remarks
      * The assignment operator unreferences the old content before assigning the new content. The new
      * content will be a deep-copy of the content of right-hand-sided object. 
      */
    NVSG_API Node & operator=(const Node & rhs);

    /*! \brief Overrides Object::invalidateIncarnation.
      * \remarks
      * The function invalidates this Node's incarnation and that of the Node's parents. That is, 
	    * it invalidates all incarnations of the Nodes up the hierarchy (bottom to top).
      */
    NVSG_API virtual void invalidateIncarnation();

    /*! \brief Overrides Object::validateIncarnation.
     * \remarks
     * The function validates all caches maintained by the Node and then calls the base class
     * implementation.
     * \sa Object::getIncarnation, Node::invalidateIncarnation
     */
    NVSG_API virtual void validateIncarnation() const;

    /*! \brief Adds this Node as an owner to \a drawable.
     *  \param drawable A pointer to a DrawableHandle to add this Node as an owner to.
     *  \sa removeAsOwnerFrom */
    NVSG_API void addAsOwnerTo( DrawableHandle * drawable );

    /*! \brief Removes this Node as an owner from \a drawable.
     *  \param drawable A pointer to the DrawableHandle to remove this Node as an owner from.
     *  \sa addAsOwnerTo */
    NVSG_API void removeAsOwnerFrom( DrawableHandle * drawable );
    
    /*! \brief Adds this Node as an owner to \a stateSet.
     *  \param stateSet A pointer to a StateSetHandle to add this Node as an owner to.
     *  \sa removeAsOwnerFrom */
    void addAsOwnerTo(StateSetHandle* stateSet);
    
    /*! \brief Removes this Node as an owner from \a stateSet.
     *  \param stateSet A pointer to the StateSetHandle to remove this Node as an owner from.
     *  \sa addAsOwnerTo */
    void removeAsOwnerFrom(StateSetHandle* stateSet);

  protected:

    // bit defines visible for derived nodes
    enum
    {
      // bounding sphere 
      NF_BOUNDING_SPHERE_VALID        = BIT0,
      // transparency containment
      NF_CONTAINS_TRANSPARENCY        = BIT1,
      NF_CONTAINS_TRANSPARENCY_VALID  = BIT2,
      // CgFx containment
      NF_CONTAINS_CGFX                = BIT3,
      NF_CONTAINS_CGFX_VALID          = BIT4,
      // light containment
      NF_CONTAINS_LIGHT               = BIT5,
      NF_CONTAINS_LIGHT_VALID         = BIT6,
      // animation frames
      NF_NUMBER_OF_FRAMES_VALID       = BIT7,
      // volume node containment
      NF_CONTAINS_VOLUME_NODE         = BIT8,
      NF_CONTAINS_VOLUME_NODE_VALID   = BIT9
    };

    mutable unsigned int      m_flags; // node flags visible to all derived 

  private:
    // helpers with managing parent/child relationships
    friend class Group; // Group needs access to these helpers - no one else!
    void addParent(GroupHandle * parent);
    void removeParent(GroupHandle * parent);

    // Validates all caches maintained by the Group.
    // Will be called from validateIncarnation
    void validateCaches() const;

  private:
    typedef std::vector< nvutil::PointerReference<GroupHandle> > ParentVector;
    typedef ParentVector::const_iterator ConstParentIterator;
    typedef ParentVector::iterator ParentIterator;

    std::string    * m_annotation; // optional annotation specified for the node
    unsigned short   m_hints;      // optional hints specified. see NodeHints
    ParentVector     m_parents;    // node's parents
          
    mutable nvmath::Sphere3f  m_boundingSphere; // ... the node's bounding sphere
    mutable unsigned short    m_hintsContainment;
    mutable unsigned short    m_hintsContainmentValid;
};

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

inline size_t Node::getNumberOfParents() const
{
  return m_parents.size();
}

inline GroupHandle * Node::getParent( size_t i ) const
{
  NVSG_ASSERT(i < m_parents.size()); // undefined behavior for invalid index!
  return m_parents[i];
}

inline unsigned short Node::getHints( unsigned short mask ) const
{
  return( m_hints & mask );
}

inline void Node::setHints( unsigned short hints )
{
  if ( m_hints != hints )
  {
    unsigned short changedHints = ( hints & ~m_hints ) | ( ~hints & m_hints );
    m_hints = hints;
    invalidateHintsContainment( changedHints );
  }
}

inline void Node::addHints( unsigned short hints )
{
  unsigned short changedHints = hints & ~m_hints;
  if ( changedHints )
  {
    m_hints |= hints;
    invalidateHintsContainment( changedHints );
  }
}

inline void Node::removeHints( unsigned short hints )
{
  unsigned short changedHints = hints & m_hints;
  if ( changedHints )
  {
    m_hints &= ~hints;
    invalidateHintsContainment( changedHints );
  }
}

inline void Node::addAsOwnerTo( DrawableHandle * drawable )
{
  NVSG_TRACE();
  NVSG_ASSERT(drawable); // required valid
  nvutil::WritableObject<Drawable>(drawable)->addOwner(nvutil::getHandle<NodeHandle>(this));
}

inline void Node::removeAsOwnerFrom( DrawableHandle * drawable )
{
  NVSG_TRACE();
  NVSG_ASSERT(drawable); // required valid
  nvutil::WritableObject<Drawable>(drawable)->removeOwner(nvutil::getHandle<NodeHandle>(this));
}

inline void Node::addAsOwnerTo( StateSetHandle * stateSet )
{
  NVSG_TRACE();
  NVSG_ASSERT(stateSet); // required valid
  nvutil::WritableObject<StateSet>(stateSet)->addOwner(nvutil::getHandle<NodeHandle>(this));
}

inline void Node::removeAsOwnerFrom( StateSetHandle * stateSet )
{
  NVSG_TRACE();
  NVSG_ASSERT(stateSet); // required valid
  nvutil::WritableObject<StateSet>(stateSet)->removeOwner(nvutil::getHandle<NodeHandle>(this));
}


// convenient helper

/*! \brief Tests whether the bounding sphere of the specified Node is empty.
  * \param node Pointer to a NodeHandle specifying the Node to query.
  * \return \c true if the bounding sphere is empty, \c false otherwise.
  * \remarks
  * The function temporary read-locks the passed Node to query its bounding sphere. 
  */
inline bool isEmptyBoundingSphere(NodeHandle* node)
{
  return nvmath::isEmpty(nvutil::ReadableObject<Node>(node)->getBoundingSphere());
}

/*! \brief Returns a copy of the specified Node's bounding sphere.
  * \param node Pointer to a NodeHandle specifying the Node to get the bounding sphere from.
  * \return A copy of the node's bounding sphere.
  * \remarks
  * The function temporary read-locks the passed Node to access its bounding sphere. 
  */
inline nvmath::Sphere3f getBoundingSphere(NodeHandle * node)
{
  return nvutil::ReadableObject<Node>(node)->getBoundingSphere();
}

template <typename T>
inline T node_cast(const Node * node)
{
  NVSG_CTASSERT(!!(nvutil::Conversion<T,const Node*>::exists));
  return dynamic_cast<T>(node);
}

template <typename T>
inline T node_cast(Node * node)
{
  NVSG_CTASSERT(!!(nvutil::Conversion<T,Node*>::exists));
  return dynamic_cast<T>(node);
}

} // namespace
