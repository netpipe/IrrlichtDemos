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
#include "nvsg/Node.h"
#include "nvmath/Plane3f.h"

namespace nvsg
{
/*! \brief Provides grouping of Node objects
  * \par Namespace: nvsg
  * \remarks
  * A Group is a special Node that provides grouping of all kinds of Node-derived objects.
  * Nodes that are grouped underneath a Group are referred to as children of the Group.
  */
class Group : public Node
{
public:

  /*! \brief Default-constructs a Group object.
    */
  NVSG_API Group();

  /*! \brief Constructs a Group from a Node.
    * \param
    * rhs Node to construct the Group from.
    * \remarks
    * This is kind of a partial copy constructor only copying the Node part from the source object.
    */
  NVSG_API explicit Group( const Node &rhs );

  /*! \brief Constructs a Group as a copy of another Group.
    * \param
    * rhs Group to serve as source for the newly constructed Group. 
    */
  NVSG_API Group( const Group& rhs );

  /*! \brief Destructs a Group object.
    */
  NVSG_API virtual ~Group();

  /*! \brief Returns the number child nodes for this Group.
    * \return
    * The function returns the number of nodes grouped underneath the Group.
    * \sa Group::getChild
    */
  NVSG_API size_t getNumberOfChildren() const;

  /*! \brief Adds a child node.
    * \param
    * child Pointer to the Node to be added.
    * \param
    * position Optional position where to place the new child.
    * \return
    * The function returns the zero-based position where the new child node has been added.
    * \remarks
    * The function increments the reference count of the specified child node and inserts
    * it at the specified position.
    * \n\n
    * When omitting the \a position parameter, the \a child node will be added at the end.
    * If \a position specifies a position greater than the actual number of child nodes,
    * the new \a child also will be added to the end. If \a position specifies a valid position - 
    * that is, a position between 0 and whatever Group::getNumberOfChildren returns - the child
    * Node will be inserted at that position forcing all subsequent child nodes to move up in
    * the list of children.
    * \n\n
    * Inserting a child node will cause the positions of all subsequent child nodes to
    * change. Group-derived classes that need to manage child positions can override the 
    * protected notifiers Group::preAddChild and Group::postAddChild to perform necessary tasks 
    * immediately before a new child node will be added and after a child node has been added, 
    * respectively.
    * \sa Group::removeChild, Group::preAddChild, Group::postAddChild
    */
  NVSG_API size_t addChild(NodeHandle * child, size_t position = 0xFFFFFFFF);
  
  /*! \brief Returns the child node at a specified position.
    * \param
    * position Zero-based position of the child node to retrieve.
    * \return
    * The function returns a pointer to the const child node at the specified position. 
    * \remarks
    * This function together with Group::getNumberOfChildren is useful for iterating through
    * all child nodes in a Group.
    * \n\n
    * Calling this function with an invalid position - that is, a position greater or equal to
    * whatever Group::getNumberOfChildren returns - results in undefined behavior.
    * \par Example:
    * \code
    * void printNodeNames(const Group * group)
    * {
    *     size_t numChilds = group->getNumberOfChildren();
    *     // iterate through the children and print the name for each in turn
    *     for ( size_t i=0; i<numChilds; ++i )
    *     {
    *         printf("child#%i: %s\n", i, group->getChild(i)->getName().c_str());
    *     }
    * }
    * \endcode
    */
  NVSG_API NodeHandle * getChild(size_t position) const;

  /*! \brief Removes all occurrences of a specified child node from this Group.
    * \param
    * child Pointer to the child node to be removed.
    * \return
    * The function returns \c false if no child node was removed from the Group. Otherwise, the
    * functions returns \c true, indicating that all occurrences of the specified child node 
    * have been removed. 
    * \remarks
    * For each occurrence of the specified child node the function decrements the child node's 
    * reference count and removes it from the internal vector of child nodes. If the child 
    * node's reference count reaches zero, it is automatically deleted.
    * \n\n
    * The function causes the position of subsequent child nodes to change. Group-derived 
    * classes that need to manage child positions can override the protected notifiers 
    * Group::preRemoveChild and Group::postRemoveChild to perform necessary tasks immediately
    * before a child node will be removed and after a child node has been removed, respectively. 
    * \sa Group::addChild, Group::preRemoveChild, Group::postRemoveChild
    */
  NVSG_API bool removeChild(NodeHandle * child);

  /*! \brief Removes the child node at the specified position.
    * \param
    * position Zero-based position of the child node to be removed.
    * \remarks
    * The function removes the child node at the specified zero-pased position and decrements
    * the reference count for that child. If the child node's reference count reaches zero, it
    * will be automatically deleted.
    * \n\n
    * The function causes the position of subsequent child nodes to change. Group-derived 
    * classes that need to manage child positions can override the protected notifiers 
    * Group::preRemoveChild and Group::postRemoveChild to perform necessary tasks immediately
    * before a child node will be removed and after a child node has been removed, respectively. 
    * \n\n
    * Calling this function with an invalid position - that is, a position greater or equal to
    * whatever Group::getNumberOfChildren returns - results in undefined behavior.
    * \sa Group::addChild, Group::preRemoveChild, Group::postRemoveChild
    */
  NVSG_API void removeChild(size_t position);

  /*! \brief Replaces all occurrences of a child node.
    * \param
    * newChild Pointer to the Node to replace the specified child node. 
    * \param
    * oldChild Pointer to the Node to be replaced. 
    * \return
    * The function returns \a false if no child node was replaced. Otherwise, the function
    * returns \c true, indicating that all occurrences of \a oldChild have been replaced by
    * \a newChild.
    * \remarks
    * The function replaces all occurrences of \a oldChild by \a newChild. The reference count
    * of \a oldChild will be decremented and the reference count of \a newChild will be
    * incremented for each replacement taking place. If the reference count of \a oldChild 
    * reaches zero, it will be automatically deleted.
    * \sa Group::addChild, Group::removeChild
    */
  NVSG_API bool replaceChild(NodeHandle * newChild, NodeHandle * oldChild);

  /*! \brief Replaces the child node at the specified position.
    * \param
    * newChild Pointer to the Node to replace the child node at the specified position. 
    * \param
    * position Zero-based position of the child node to be replaced.
    * \remarks
    * The function replaces the child node at the specified position by the node pointed to by
    * \a newChild. The function increments the reference count of \a newChild and decrements
    * the reference count of the replaced child node. If the reference count of the replaced 
    * child node reaches zero, it will be automatically deleted. 
    * \n\n
    * Calling this function with an invalid position - that is, a position greater or equal to
    * whatever Group::getNumberOfChildren returns - results in undefined behavior.
    * \sa Group::addChild, Group::removeChild
    */
  NVSG_API void replaceChild(NodeHandle * newChild, size_t position);
  
  /*! \brief Tests whether a specified Node is a child node of this Group.
    * \param
    * node Pointer to the Node to test.
    * \return
    * The function returns \c true if \a node is detected to be a child node of this Group.
    * Otherwise, the function returns \c false.
    * \sa Group::addChild, Group::removeChild, Group::replaceChild
    */
  NVSG_API bool hasChild(NodeHandle * node) const;
  
  /*! \brief Returns all occurring positions of a specified child node.
    * \param
    * node Pointer to the child node for which to retrieve the positions inside this Group.
    * \param
    * positions Reference to a vector that receives all occurring positions of \a node.
    * \return
    * The function returns \c true if the requested positions for the specified Node could have 
    * been received successfully. Otherwise, the function returns \c false, indicating that the
    * specified Node is not a child of this Group.
    * \sa Group::addChild, Group::removeChild, Group::replaceChild
    */
  NVSG_API bool getChildPositions(NodeHandle * node, std::vector<size_t> & positions) const;

  /*! \brief Tests whether any CgFx attribute is assigned to a geometry underneath this Group.
    * \return
    * The function returns \c true if at least one CgFx attribute is assigned to a geometry
    * underneath this Group.
    * \remarks
    * The function overrides Node::containsCgFx.
    * \n\n
    * The function tests whether somewhere underneath this Group a CgFx attribute is assigned
    * to a geometry. To determine this, the function needs to iterate through the hierarchy
    * underneath this Group. As this might be an expensive operation, the result of the test
    * will be cached to provide faster access with subsequent queries. If the state of CgFx 
    * containment changes somehow, the cache will become invalid, forcing a re-calculation of
    * the result in the very next query.
    * \sa Group::invalidateCgFxContainment
    */
  NVSG_API virtual bool containsCgFx() const;

  /*! \brief Invalidates the CgFx containment cache.
    * \remarks
    * The function overrides Node::invalidateCgFxContainment.
    * \n\n
    * The function invalides this Group's CgFx containment cache and that of the superior 
    * hierarchy. 
    * \sa Group::containsCgFx
    */
  NVSG_API virtual void invalidateCgFxContainment();

  /*! \brief Tests if this Group contains a VolumeNode. 
   *  \return \c true if this Group or any Node beneath it contains a VolumeNode, otherwise \c false.
   *  \remarks The function tests if this Group or any Node beneath it contains a VolumeNode. To
   *  determine this, the function needs to traverse through the hierarchy underneath this Group.
   *  As this might be an expensive operation, the result of the test will be cached to provide
   *  faster access with subsequent queries. If the state of VolumeNode containment changes
   *  somehow, the cache will become invalid, forcing a re-evaluation of the result with the very
   *  next query.
   *  \sa invalidateVolumeNodeContainment */
  NVSG_API virtual bool containsVolumeNode() const;

  /*! \brief Invalidates the VolumenNode containment cache.
   *  \remarks The function invalidates the VolumeNode containment cache of this Group and then
   *  calls Node::invalidateVolumeNodeContainment().
   *  \sa containsVolumeNode */
  NVSG_API virtual void invalidateVolumeNodeContainment();

  /*! \brief Tests whether any LightSource is located in or below this Group.
    * \return
    * The function returns \c true if at least one LightSource is located in or below this Group.
    * Otherwise, the function returns \c false.
    * \remarks
    * The function tests whether at least one LightSource is located in or below this Group.
    * To determine this, the function needs to iterate through the hierarchy underneath this 
    * Group. As this might be an expensive operation, the result of the test will be cached to 
    * provide faster access with the next queries. If the state of LightSource containment 
    * changes somehow, the cache will become invalid, forcing a re-calculation of the result 
    * in the very next query.
    * \sa Group::invalidateLightContainment
    */
  NVSG_API virtual bool containsLight() const;

  /*! \brief Invalidates the LightSource containment cache.
    * \remarks
    * The function invalides this Group's LightSource containment cache and that of the superior 
    * hierarchy. 
    * \sa Group::containsLight
    */
  NVSG_API virtual void invalidateLightContainment();

  /*! \brief Returns the number of animation frames contained
    * \return
    * The function returns the actual number of contained animation frames.        
    * \remarks
    * The function overrides Node::getNumberOfFrames.
    * \n\n
    * The function iterates the underlying hierarchy for animations to query for the number of 
    * frames. As this might be an expensive operation, the result of the test will be cached to 
    * provide faster access with the next queries. If the animation state changes underneath 
    * this Group, the cache will become invalid, forcing a re-calculation of the result in the 
    * very next query. 
    * \n\n
    * There currently is a restriction that a Scene must contain animations with exactly the same 
    * number of frames. 
    * \sa Node::getNumberOfFrames, Group::invalidateNumberOfFrames
    */
  NVSG_API virtual size_t getNumberOfFrames() const;

  /*! \brief Invalidates the cache for the number of frames contained.
    * \remarks
    * The function overrides Node::invalidateNumberOfFrames.
    * \n\n
    * The function invalides this Group's cache for the number of frames contained and that of 
    * the superior hierarchy. 
    * \sa Group::getNumberOfFrames
    */
  NVSG_API virtual void invalidateNumberOfFrames();

  /*! \brief Get the number of clipping planes in this Group.
   *  \return The number of clipping planes on this Group.
   *  \sa addClipPlane, isClipPlaneActive, getClipPlane, setClipPlane */
  NVSG_API size_t getNumberOfClipPlanes() const;

  /*! \brief Get the number of active clipping planes in this Group.
   *  \return The number of active clipping planes in this Group.
   *  \sa getNumberOfClipPlanes */
  NVSG_API size_t getNumberOfActiveClipPlanes() const;

  /*! \brief Ask if the specified clipping plane is active.
   *  \param index The index in the array of clipping planes.
   *  \return \c true, if the specified clipping plane is active, otherwise \c false.
   *  \note The behavior is undefined if \a index is larger than or equal to the number of
   *  clipping planes in this Group.
   *  \sa getNumberOfClipPlanes, getClipPlane, activateClipPlane, deactivateClipPlane */
  NVSG_API bool isClipPlaneActive( size_t index ) const;

  /*! \brief Returns the specified clipping plane.
   *  \param index Specifies the zero based index in the array of clipping planes.
   *  \return The function returns a pointer to a Plane3fHandle.
   *  \note The behavior is undefined if \a index is larger than or equal to the number of
   *  clipping planes in this Group.
   *  \sa getNumberOfClipPlanes, isClipPlaneActive, setClipPlane */
  NVSG_API const nvmath::Plane3f&  getClipPlane( size_t index ) const;

  /*! \brief Adds a clipping plane to this Group.
   *  \param plane A pointer to the Plane3fHandle that specifies the clip plane to be added.
   *  \param active Optional parameter to add the clip plane as active (\c true) or inactive
   *  (\c false); default is active.
   *  \sa getNumberOfClipPlanes, isClipPlaneActive, getClipPlane */
  NVSG_API void addClipPlane( const nvmath::Plane3f& plane, bool active = true );

  /*! \brief Activates the specified clip plane.
   *  \param index The zero-based index in the array of clipping planes.
   *  \note The behavior is undefined if \a index is larger than or equal to the number of
   *  clip planes in this Group.
   *  \sa getNumberOfClipPlanes, isClipPlaneActive, deactivateClipPlane */
  NVSG_API void activateClipPlane( size_t index );

  /*! \brief Deactivates the specified clip plane.
   *  \param index The zero-based index in the array of clip planes.
   *  \note The behavior is undefined if \a index is larger than or equal to the number of
   *  clip planes in this Group.
   *  \sa getNumberOfClipPlanes, isClipPlaneActive, activateClipPlane */
  NVSG_API void deactivateClipPlane( size_t index );

  /*! \brief Sets the plane at the specified index.
   *  \param index The zero-based index in the array of clip planes.
   *  \param plane A pointer to a Plane3fHandle specifying the plane to use as the clip plane.
   *  \note The behavior is undefined if \a index is larger than or equal to the number of
   *  clip planes in this Group.
   *  \sa getNumberOfClipPlanes, activateClipPlane, deactivateClipPlane */
  NVSG_API void setClipPlane( size_t index, const nvmath::Plane3f& plane );

  /*! \brief Returns the number light sources for this Group.
   * \return
   * The function returns the number of light sources in the Group.
   * \sa Group::getLightSource
   */
  NVSG_API size_t getNumberOfLightSources() const;

  /*! \brief Returns the light source at a specified position.
   * \param
   * position Zero-based position of the light source to retrieve.
   * \return
   * The function returns a pointer to the light source at the specified position. 
   * \remarks
   * This function together with Group::getNumberOfLightSources is useful for iterating through
   * all light sources in a Group.
   * \n\n
   * Calling this function with an invalid position - that is, a position greater or equal to
   * whatever Group::getNumberOfLightSources returns - results in undefined behavior.
   * \par Example:
   * \code
   *  void printLightNames(const Group * group)
   *  {
   *    size_t numLights = group->getNumberOfLightSources();
   *    // iterate through the light sources and print the name for each in turn
   *    for ( size_t i=0; i<numLights; ++i )
   *    {
   *      ReadableObject<LightSource> ls(group->getLightSource(i);
   *      printf("light#%i: %s\n", i, ls->getName().c_str());
   *    }
   *  }
   * \endcode
   */
  NVSG_API LightSourceHandle * getLightSource( size_t index ) const;

  /*! \brief Tests whether a specified LightSource is part of this Group.
   * \param
   * node Pointer to the LightSource to test.
   * \return
   * The function returns \c true if \a lsHdl is detected to be a light source of this Group.
   * Otherwise, the function returns \c false.
   * \sa Group::addLightSource, Group::removeLightSource, Group::replaceLightSource
   */
  NVSG_API bool hasLightSource( LightSourceHandle * lsHdl ) const;

  /*! \brief Adds a light source.
   * \param
   * child Pointer to the LightSource to be added.
   * \remarks
   * The function increments the reference count of the specified light source and adds the light
   * source to its list of light sources.
   * \sa Group::removeLightSource
   */
  NVSG_API void addLightSource( LightSourceHandle * lsHdl );

  /*! \brief Replaces the light source at the specified position.
   * \param
   * index Position of the LightSource in this Group to be replaced.
   * \param
   * newChild Pointer to the LightSource to replace the light source at the specified position. 
   * \remarks
   * The function replaces the light source at the specified position by the light source pointed
   * to by \a lsHdl. The function increments the reference count of \a lsHdl and decrements
   * the reference count of the replaced child node. If the reference count of the replaced 
   * child node reaches zero, it will be automatically deleted. 
   * \n\n
   * Calling this function with an invalid position - that is, a position greater or equal to
   * whatever Group::getNumberOfLightSources returns - results in undefined behavior.
   * \sa Group::addLightSource, Group::removeLightSource
   */
  NVSG_API void replaceLightSource( size_t index, LightSourceHandle * lsHdl );

  /*! \brief Replaces a LightSource by an other
   * \param
   * newHdl Pointer to the LightSource to replace the specified light source
   * \param
   * oldHdl Pointer to the LightSource to be replaced. 
   * \remarks
   * The function replaces the LightSource \a oldHdl by \a newHdl. The reference count of \a
   * oldHdl will be decremented and the reference count of \a newHdl will be incremented. If the
   * reference count of \a oldHdl reaches zero, it will be automatically deleted.
   * \sa Group::addLightSource, Group::removeLightSource
   */
  NVSG_API void replaceLightSource( LightSourceHandle * newHdl, LightSourceHandle * oldHdl );

  /*! \brief Removes the light source at the specified position.
   * \param
   * index Zero-based position of the light source to be removed.
   * \remarks
   * The function removes the light source at the specified zero-based position and decrements
   * the reference count for that light source. If the light source's reference count reaches
   * zero, it will be automatically deleted.
   * \n\n
   * Calling this function with an invalid position - that is, a position greater or equal to
   * whatever Group::getNumberOfLightSources returns - results in undefined behavior.
   * \sa Group::addLightSource
   */
  NVSG_API void removeLightSource( size_t index );

  /*! \brief Overrides Object::isEquivalent.  
    * \param
    * p Pointer to the Object to test for equivalence with this Group object.
    * \param
    * ignoreNames Object names will be ignored while testing if this is \c true.
    * \param
    * deepCompare The function performs a deep-compare instead of a shallow compare if this is \c true.
    * \return
    * The function returns \c true if the Object pointed to by \a object is detected to be equivalent
    * to this Group object.
    * \remarks
    * The test will be performed considering the optional control parameters ignoreNames and deepCompare. 
    * If you omit these two, the function ignores the object names
    * and performs a shallow compare only.
    * \sa Object::isEquivalent
    */
  NVSG_API virtual bool isEquivalent(const Object * p, bool ignoreNames, bool deepCompare) const;

  /*! \brief Assigns new content from another Group object. 
    * \param
    * rhs Group object from which to assign the new content.
    * \return
    * A reference to this Group object.
    * \remarks
    * The assignment operator unreferences the old content before assigning the new content. The new
    * content will be a deep-copy of the right-hand-sided object's content.
    * \sa Group::clone
    */
  NVSG_API Group & operator=(const Group & rhs);
  
protected:

  /*! \brief Called from the framework immediately before a child will be removed from the Group.
    * \param
    * position Zero-based position of the child that immediately will be removed from the Group.
    * \remarks
    * It is recommended, for Group-derived classes that need to keep track of child nodes removal, 
    * to override this function to perform necessary tasks immediately before a child node will be 
    * removed from the Group.   
    * \n\n
    * The function will be called with the zero-based index that references the child that
    * will be removed immediately after this call.
    * \sa Group::postRemoveChild
    */
  NVSG_API virtual void preRemoveChild(size_t position);

  /*! \brief Called from the framework immediately after a child has been removed from the Group.
    * \param
    * position Zero-based position that previously referenced the removed child node.
    * \remarks
    * It is recommended, for Group-derived classes that need to keep track of child nodes removal, 
    * to override this notifier to perform necessary tasks immediately after a child node has been 
    * removed from the Group.   
    * \n\n
    * The function will be called with the zero-based position that previously referenced the 
    * removed child. Note that this position can no longer be used to reference the child node, 
    * because the child node already has been removed from this Group.
    * \sa Group::preRemoveChild
    */
  NVSG_API virtual void postRemoveChild(size_t position);

  /*! \brief Called from the framework immediately before a child will be added to the Group.
    * \param
    * position Zero-based position where the child node will be added.
    * \remarks
    * It is recommended, for Group-derived classes that need to keep track of child nodes insertion,
    * to override this notifier to perform necessary tasks immediately before a child node will be 
    * added to the Group.   
    * \n\n
    * The function will be called with the zero-based position where the child node will be added.
    * Note that this position cannot yet be used to reference the child node, because the child
    * node has not been added to this Group at this point.
    * \sa Group::postAddChild
    */
  NVSG_API virtual void preAddChild(size_t position);

  /*! \brief Called from the framework immediately after a child was added to the Group.
    * \param
    * position Zero-based position where the child node was added.
    * \remarks
    * It is recommended, for Group-derived classes that need to keep track of child nodes insertion,
    * to override this notifier to perform necessary tasks immediately after a child node has been 
    * added to the Group.   
    * \n\n
    * The function will be called with the zero-based position where the child node has been added
    * immediately before this call.
    * \sa Group::preAddChild
    */
  NVSG_API virtual void postAddChild(size_t position);

  /*! \brief Called from the framework if re-calculation of the Group's bounding sphere is required.
    * \return
    * The function returns a nvmath::Sphere3f that represents the actual bounding sphere of this 
    * Group.
    * \remarks
    * The function calculates the bounding sphere by accumulating the bounding spheres of all 
    * available child nodes. 
    * \n\n
    * For Group-derived classes to specialize bounding sphere calculation, it is recommended to 
    * override this function. 
    * \sa nvmath::Sphere3f
    */
  NVSG_API virtual nvmath::Sphere3f calculateBoundingSphere() const;

  /*! \brief Determine the hint containment status of this Group.
   *  \param hints An unsigned short whose bits encode the hints containment to determine.
   *  \return An unsigned short whose bits encode the hints that are to be determined and also
   *  set in this Group or any of it's children.
   *  \remarks This function is called by the framework, if \a getHintsContainment is called while the
   *  hint containment flags mark that information as invalid.
   *  \sa containsHints, invalidateHintsContainment, getHintsContainment */
  NVSG_API virtual unsigned short determineHintsContainment( unsigned short hints ) const;

  /*! \brief Called from the framework if re-determination of the transparency containment cache is 
    * required.
    * \return
    * The function returns \c true if the Group contains at least one StateSet that causes the 
    * corresponding geometry to appear transparent. Otherwise, the function returns \c false.
    * \remarks
    * This function will be called from the framework if re-determination of the related 
    * transparency containment cache is required due to the cache becoming invalid, for example, 
    * if child nodes have either been added or removed from the Group.
    * \n\n
    * The function iterates through the child nodes and, for each node, determinates transparency
    * containment. The iteration stops with the first child node found that contains at least 
    * one StateSet that causes the corresponding geometry to appear transparent.  
    * \n\n
    * For Group-derived classes to specialize this determination, it is recommended to override 
    * this function. 
    * \sa StateSet::isTransparent 
    */
  NVSG_API virtual bool determineTransparencyContainment() const;

  /*! \brief Invalidates owning caches depending on whether a child node was added or removed.
    * \param
    * child Pointer to the child that was either added or removed
    * \param
    * insert Indicates if the child node was added or removed. A value of \c true indicates that the
    * child node was inserted. A value of \c false indicates that the child was removed.
    * \remarks
    * The function always invalidates the Group's bounding sphere. If either the CgFX containment, 
    * the LightSource containment, the transparency containment, or the number of animation frames 
    * changes, the function invalidates the corresponding caches - forcing a re-calculation of these 
    * caches with the very next query.
    * \sa Group::containsCgFx, Group::invalidateCgFxContainment, Group::containsLight, 
    * Group::invalidateLightContainment, Group::containsTransparentGeometry,
    * Group::invalidateTransparencyContainment
    */
  NVSG_API void invalidateCaches(NodeHandle *child, bool insert);

  /*! \brief Validates all incarnations down the hierarchy.
    * \remarks
    * The function is called from the framework. It overrides Object::validateIncarnation
    * and validates all incarnations down the hierarchy, starting with this Group object.
    * \sa Object::getIncarnation, Node::invalidateIncarnation
    */
  NVSG_API virtual void validateIncarnation() const;  // from top to bottom

private:
  void copyChildren(const std::vector<NodeHandle*> & children);
  void copyLightSources(const std::vector<LightSourceHandle*> & lightSources);
  void addAsParent(NodeHandle* child);
  void removeAsParent(NodeHandle* child); 
  
private:
  std::vector<NodeHandle*>                      m_children;  // the group's child nodes      
  std::vector<std::pair<bool,nvmath::Plane3f> > m_clipPlanes; // user clip planes
  std::vector<LightSourceHandle*>               m_lightSources;

  mutable size_t m_numberOfFrames;
};

inline size_t Group::getNumberOfChildren() const
{
  NVSG_TRACE();
  return( m_children.size() );
}

inline NodeHandle * Group::getChild( size_t position ) const
{
  NVSG_TRACE();
  NVSG_ASSERT_OBJECT_READ_LOCKED(this);
  NVSG_ASSERT(position<m_children.size()); // undefined behavior for invalid position
  return m_children[position];
}

inline bool Group::hasChild( NodeHandle * child ) const
{
  NVSG_TRACE();
  NVSG_ASSERT_OBJECT_READ_LOCKED(this);
  return std::find(m_children.begin(), m_children.end(), child) != m_children.end();
}

inline size_t Group::getNumberOfClipPlanes() const
{
  NVSG_TRACE();
  NVSG_ASSERT_OBJECT_READ_LOCKED(this);
  return( m_clipPlanes.size() );
}

inline bool Group::isClipPlaneActive( size_t index ) const
{
  NVSG_TRACE();
  NVSG_ASSERT_OBJECT_READ_LOCKED(this);
  NVSG_ASSERT( index < m_clipPlanes.size() );
  return( m_clipPlanes[index].first );
}

inline const nvmath::Plane3f& Group::getClipPlane( size_t index ) const
{
  NVSG_TRACE();
  NVSG_ASSERT_OBJECT_READ_LOCKED(this);
  NVSG_ASSERT( index < m_clipPlanes.size() );
  return( m_clipPlanes[index].second );
}

inline void Group::addClipPlane( const nvmath::Plane3f&plane, bool active )
{
  NVSG_TRACE();
  NVSG_ASSERT_OBJECT_WRITE_LOCKED(this);
  m_clipPlanes.push_back( std::make_pair( active, plane ) );
  increaseIncarnation();
}

inline void Group::setClipPlane( size_t index, const nvmath::Plane3f&plane )
{
  NVSG_TRACE();
  NVSG_ASSERT_OBJECT_WRITE_LOCKED(this);
  NVSG_ASSERT( index < m_clipPlanes.size() );
  m_clipPlanes[index].second = plane;
  increaseIncarnation();
}

inline size_t Group::getNumberOfLightSources() const
{
  NVSG_TRACE();
  NVSG_ASSERT_OBJECT_READ_LOCKED(this);
  return( m_lightSources.size() );
}

inline LightSourceHandle * Group::getLightSource( size_t index ) const
{
  NVSG_TRACE();
  NVSG_ASSERT_OBJECT_READ_LOCKED(this);
  NVSG_ASSERT( index < m_lightSources.size() );
  return( m_lightSources[index] );
}

inline bool Group::hasLightSource( LightSourceHandle * lsHdl ) const
{
  NVSG_TRACE();
  NVSG_ASSERT_OBJECT_READ_LOCKED(this);
  return std::find(m_lightSources.begin(), m_lightSources.end(), lsHdl) != m_lightSources.end();
}

} //  namespace nvsg
