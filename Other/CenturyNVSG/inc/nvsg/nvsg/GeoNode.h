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

#include "nvsgcommon.h" // commonly used stuff
#include "nvsg/Node.h" // base class definition
// additional dependencies
#include "nvsg/VertexAttributeSet.h"
#include <vector>

namespace nvsg
{
  /*! \brief Class to hold a set of geometries.
   *  \par Namespace: nvsg
   *  \remarks A GeoNode in a scene graph is always a leaf node. It holds a number of Drawable 
   *  objects, each one associated to a StateSet. For performance reasons, Drawable objects
   *  associated to the same StateSet, together with that StateSet, are logically gathered to one
   *  element of a so-called Geometry. Thus, iterating over all Drawable objects is a two-level
   *  process; on the outer loop iterating over all Geometries, and on the inner loop iterating
   *  over all Drawable objects associated to the StateSet of that Geometry. There cannot be the
   *  same Drawable more than once in a GeoNode.
   *  \sa Drawable, Node, Scene, StateSet */
  class GeoNode : public Node
  {
    public:
      /*! \brief Default-constructs a GeoNode.
       */
      NVSG_API GeoNode();

      /*! \brief Constructs a GeoNode from a Node.
        * \param
        * rhs Node to construct the GeoNode from.
        * \remarks
        * This is kind of a partial copy constructor only copying the Node part from the source 
        * object.
        */
      NVSG_API explicit GeoNode( const Node &rhs );

      /*! \brief Constructs a GeoNode as a copy of another GeoNode.
       */
      NVSG_API GeoNode( const GeoNode& rhs );
 
      /*! \brief Destructs a GeoNode.
       */
      NVSG_API virtual ~GeoNode();

      /*! \brief Get the number of contained geometries.
       *  \return The number of contained geometries.
       *  \remarks A geometry is defined by a pointer to a constant StateSet, which might be the
       *  NULL pointer, and a number of pointers to constant Drawable objects associated with that
       *  StateSet.
       *  \sa Drawable, StateSet */
      NVSG_API size_t getNumberOfGeometries() const;

      /*! \brief Get the index of the geometry holding the specified Drawable.
       *  \param drawable A pointer to the constant Drawable, for which the geometry index is
       *  queried.
       *  \return The index of the Geometry holding the Drawable \a drawable, or (-1) if \a
       *  drawable is not part of any Geometry of this GeoNode.
       *  \sa Drawable, getNumberOfGeometries, getStateSet*/
      NVSG_API size_t getGeometryIndex( DrawableHandle * drawable ) const;

      /*! \brief Get the index of the geometry holding the specified StateSet.
       *  \param stateSet A pointer to the constant StateSet, for which the geometry index is
       *  queried.
       *  \return The index of the Geometry associated with the StateSet \a stateSet, or (-1) if \a
       *  stateSet is not part of any Geometry of this GeoNode.
       *  \sa Drawable, getNumberOfGeometries, getStateSet*/
      NVSG_API size_t getGeometryIndex( StateSetHandle * stateSet ) const;

      /*! \brief Add a geometry to the GeoNode.
       *  \param drawable A pointer to the constant Drawable to add.
       *  \param stateSet An optional pointer to a constant StateSet. The default is NULL.
       *  \remarks The Drawable \a drawable is added to the Geometry associated with the StateSet
       *  \a stateSet. On adding, the reference count of \a drawable is incremented. If there was
       *  no Geometry associated with \a stateSet, a new one is created and the reference count
       *  of \a stateSet is incremented.\n
       *  The nvutil::Incarnation is increased.
       *  \note The behavior is undefined if \a drawable is not a valid pointer.
       *  \sa Drawable, StateSet */
      NVSG_API void addGeometry( DrawableHandle * drawable
                               , StateSetHandle * stateSet = NULL );

      /*! \brief Remove a geometry from the GeoNode.
       *  \param stateSet A pointer to a constant StateSet to remove.
       *  \remarks The Geometry associated with the StateSet \a stateSet is removed. On removal,
       *  the reference count of each Drawable assigned to \a stateSet, as well as the reference
       *  count of \a stateSet, if it is not NULL, is decremented.\n
       *  The nvutil::Incarnation is increased.
       *  \sa Drawable, StateSet*/
      NVSG_API void removeGeometry( StateSetHandle * stateSet );

      /*! \brief Remove a geometry from the GeoNode.
       *  \param drawable A Drawable to remove.
       *  \remarks The Drawable \a drawable is removed from the Geometries of this GeoNode. If a
       *  Geometry of this GeoNode has the Drawable \a drawable assigned, the reference count of \a
       *  drawable is decremented and it is removed from the Geometry. If it was the only Drawable
       *  in that Geometry, the Geometry is removed from the GeoNode and, if the associated
       *  StateSet is not NULL, its reference count is also decremented.\n
       *  The nvutil::Incarnation is increased.
       *  \note The behavior is undefined if \a drawable is NULL.
       *  \sa Drawable, StateSet */
      NVSG_API void removeGeometry( DrawableHandle * drawable );

      /*! \brief Remove all geometries form the GeoNode.
       *  \remarks The reference count of all Drawable objects and all StateSet objects held by
       *  this GeoNode is decremented, and the Geometries are cleared.\n
       *  The nvutil::Incarnation is increased.
       *  \sa Drawable, StateSet */
      NVSG_API void clearGeometries();

      /*! \brief Get the StateSet of the specified geometry.
       *  \param geometryIndex  The index of the geometry to get information.
       *  \return A pointer to a StateSetHandle specifying the StateSet at position \a geometryIndex 
       *  in this GeoNode.
       *  \remarks If the Geometry with index \a geometryIndex has no StateSet bound, NULL is
       *  returned.
       *  \note The behavior is undefined, if \a geometryIndex is larger than or equal to the
       *  number of geometries of this GeoNode.
       *  \sa StateSet, getNumberOfGeometries */
      NVSG_API StateSetHandle * getStateSet( size_t geometryIndex ) const;

      /*! \brief Get the StateSet associated with a specified Drawable
       *  \param drawable A pointer to a DrawableHandle specifying the Drawable for which to query 
       *  the StateSet.
       *  \return A pointer to a StateSetHandle specifying the StateSet that is associated with the
       *  passed Drawable.
       *  \remarks If there is no StateSet associated to the Drawable \a drawable, NULL is
       *  returned.
       *  \note The behavior is undefined if \a drawable is not a valid pointer, or if \a
       *  drawable is not part of any Geometry of this GeoNode.
       *  \sa Drawable, StateSet, getGeometryIndex */
      NVSG_API StateSetHandle * getStateSet( DrawableHandle * drawable ) const;

      /*! \brief Replace a StateSet by another.
       *  \param newStateSet  A pointer to a StateSetHandle specifying the StateSet to replace 
       *  \a oldStateSet.
       *  \param oldStateSet  A pointer to a StateSetHandle specifying the StateSet that will be 
       *  replaced by \a newStateSet.
       *  \remarks All Drawable objects that are associated to \a oldStateSet are changed to be
       *  associated with \a newStateSet. If \a oldStateSet is a valid pointer, its reference
       *  count is decreased; and if \a newStateSet is a valid pointer, its reference count is
       *  increased.\n
       *  The nvutil::Incarnation is increased.
       *  \sa Drawable, StateSet */
      NVSG_API void replaceStateSet( StateSetHandle * newStateSet
                                   , StateSetHandle * oldStateSet );

      /*! \brief Get the number of Drawable objects of the specified Geometry.
       *  \param geometryIndex The index of the Geometry to use.
       *  \return The number of Drawable objects in the Geometry at position \a geometryIndex.
       *  \note The behavior is undefined if \a geometryIndex is larger than or equal to the
       *  number of Geometries of this GeoNode.
       *  \sa Drawable, getGeometryIndex */
      NVSG_API size_t getNumberOfDrawables( size_t geometryIndex ) const;

      /*! \brief Get the Drawable at the specified index of the specified Geometry.
       *  \param geometryIndex The index of the Geometry to use
       *  \param drawableIndex The index of the Drawable to get
       *  \return A pointer to a DrawableHandle specifying the Drawable at the specified position.
       *  \note The behavior is undefined if \a geometryIndex is larger than or equal to the
       *  number of Geometries of this GeoNode, or if \a drawableIndex is large than or equal to
       *  the number of Drawable objects in that Geometry.
       *  \sa Drawable, getNumberOfGeometries, getNumberOfDrawables */
      NVSG_API DrawableHandle * getDrawable( size_t geometryIndex
                                           , size_t drawableIndex ) const;

      /*! \brief Replace a Drawable by another.
       *  \param newDrawable A pointer to a DrawableHandle specifying the Drawable to replaces \a 
       *  oldDrawable.
       *  \param oldDrawable A pointer to a DrawableHandle specifying the Drawable that will be 
       *  replaced by \a newDrawable.
       *  \param stateSet A pointer to a StateSetHandle specifying the StateSet \a oldDrawable is 
       *  associated with.
       *  \remarks In the Geometry defined by \a stateSet, the Drawable \a oldDrawable is
       *  replaced with \a newDrawable. On replacing, the reference count of \a newDrawable is
       *  incremented, the reference count of \a oldDrawable is decremented, and the
       *  nvutil::Incarnation is increased.
       *  \sa Drawable, StateSet */
      NVSG_API void replaceDrawable( DrawableHandle * newDrawable
                                   , DrawableHandle * oldDrawable
                                   , StateSetHandle * stateSet );

      /*! \brief Replace a Drawable by another.
       *  \param newDrawable A pointer to a DrawableHandle specifying the Drawable to replaces \a 
       *  oldDrawable.
       *  \param oldDrawable A pointer to a DrawableHandle specifying the Drawable that will be 
       *  replaced by \a newDrawable.
       *  \remarks The Drawable \a oldDrawable is replaced by \a newDrawable, no matter to what
       *  StateSet \a oldDrawable is associated with. On replacing, the reference count of \a newDrawable
       *  is increased, the reference count of \a oldDrawable is decreased, and the
       *  nvutil::Incarnation is increased.
       *  \sa Drawable */
      NVSG_API void replaceDrawable( DrawableHandle * newDrawable
                                   , DrawableHandle * oldDrawable );

      /*! \brief Clear texture coordinates on all Drawable objects. */
      NVSG_API void clearTexCoords( size_t tu = 0 );

      /*! \brief Query if this GeoNode contains any CgFx StateAttribute.
       *  \return \c true if this GeoNode holds at least one StateSet with a CgFx, otherwise \c false.
       *  \remarks Because a CgFx does not use the standard drawing pipeline, it might be of
       *  interest in several cases to know if there is a CgFx contained in a GeoNode.
       *  \sa CgFx, StateAttribute */
      NVSG_API virtual bool containsCgFx() const;

      /*! \brief Invalidate the CgFx containment cache.
       *  \remarks Invalidating the CgFx containment cache marks this information as invalid and
       *  reports the change to all its owners.
       *  \sa CgFx, StateAttribute */
      NVSG_API virtual void invalidateCgFxContainment();

      /*! \brief Query the number of frames in the Animation objects in this GeoNode.
       *  \return The number of frames in the Animation objects in this GeoNode.
       *  \remarks If there is no Animation part of this GeoNode, the number of frames returned is
       *  zero.
       *  \note Currently, all Animations in a Scene must have the same number of frames.
       *  \sa Animation */
      NVSG_API virtual size_t getNumberOfFrames() const;

      /*! \brief Invalidate the number of animation frames cache.
       *  \remarks Invalidating the number of animation frames cache marks this information as
       *  invalid and reports the change to all its owners.
       *  \sa Animation */
      NVSG_API virtual void invalidateNumberOfFrames();

      /*! \brief Test for equivalence with an other GeoNode.
       *  \param p            A reference to the constant GeoNode to test for equivalence with.
       *  \param ignoreNames  Optional parameter to ignore the names of the objects; default is \c
       *  true.
       *  \param deepCompare  Optional parameter to perform a deep comparison; default is \c false.
       *  \return \c true if the GeoNode \a p is equivalent to \c this, otherwise \c false.
       *  \remarks If \a p and \c this are equivalent as Node, and they have the same number of
       *  StateSet objects with the same number of Drawable objects assigned, they are each tested for
       *  equivalence. If \a deepCompare is false, they are considered to be equivalent if
       *  they are the same pointers. Otherwise a full equivalence test is performed.
       *  \note The behavior is undefined if \a p is not a GeoNode nor derived from one.
       *  \sa Node */
      NVSG_API virtual bool isEquivalent( const Object * p
                                        , bool ignoreNames
                                        , bool deepCompare ) const;

      /*! \brief Assignment operator
       *  \param rhs A reference to the constant GeoNode to copy from
       *  \return A reference to the assigned GeoNode
       *  \remarks The assignment operator calls the assignment operator of Node and copies all
       *  geometries from rhs. On all StateSet and Drawable objects previously in \c this, the
       *  reference count is decremented.
       *  \sa Drawable, Node, StateSet */
      NVSG_API GeoNode & operator=(const GeoNode & rhs);

    protected:
      /*! \brief Calculate the bounding sphere of this GeoNode.
       *  \return A nvmath::Sphere3f that contains the complete GeoNode.
       *  \remarks This function is called by the framework to determine a sphere that completely
       *  contains the GeoNode.
       *  \sa getBoundingSphere, invalidateBoundingSphere */
      NVSG_API virtual nvmath::Sphere3f calculateBoundingSphere() const;

      /*! \brief Determine the transparency containment state.
       *  \return \c true, if this GeoNode holds any transparent geometry, otherwise \c false.
       *  \remarks This function is called by the framework to determine the transparency
       *  containment state of a Node when the transparency containment cache is marked as
       *  invalid.
       *  \sa Node, containsTransparentGeometry, isTransparent */
      NVSG_API virtual bool determineTransparencyContainment() const;

      /*! \brief Invalidate all caches of this GeoNode.
       *  \param stateSet A pointer to the constant StateSet that has been added or removed.
       *  \param insert   \c true, if \a stateSet has been added, otherwise \c false.
       *  \remarks This function is called by the framework whenever a StateSet has been to added
       *  or removed from this GeoNode. Depending on the resulting changes, the specific cache
       *  invalidation functions are then called.
       *  \sa StateSet, invalidateCgFxContainment, invalidateTransparencyContainment */
      NVSG_API void invalidateCaches( StateSetHandle * stateSet
                                    , bool insert );

      /*! \brief Validate the nvutil::Incarnation.
       *  \remarks This function is called from the framework when the nvutil::Incarnation of this
       *  GeoNode is requested but marked as invalid. Then it is validated and the
       *  nvutil::Incarnation of all StateSet and Drawable objects is requested to continue the
       *  validation down the tree hierarchy.
       *  \sa Drawable, Node, StateSet, nvutil::Incarnation, invalidateIncarnation */
      NVSG_API virtual void validateIncarnation() const;  // from top to bottom

    private:
      typedef struct
      {
        StateSetHandle                * m_stateSet;
        std::vector<DrawableHandle *>   m_drawables;
      } Geometry;

    private:

      void copyGeometry( Geometry &lhs, const Geometry &rhs );
      void deleteGeometry( size_t gi );
      void removeDrawable( Geometry * geometry, DrawableHandle * drawable );
      void removeGeometry( size_t gi );
      void replaceDrawable( Geometry * geometry, DrawableHandle * newDrawable, DrawableHandle * oldDrawable );

    private:
      std::vector<Geometry> m_geometries;
      //  State caches
      mutable size_t m_numberOfFrames;
  };

  /*! \relates GeoNode
   *  \brief Generate tangents and binormals for all QuadMeshes, Quads, QuadStrips, Triangles,
   *  TriFans, and TriStrips in a GeoNode.
   *  \param gn A pointer to the constant GeoNode to create tangents and binormals for.
   *  \param tu The optional texture unit that holds texture coordinates to use; default is 0.
   *  \param tg The optional texture unit that is going to hold the tangents; default is 1.
   *  \param bn The optional texture unit that is going to hold the binormals; default is 2.
   *  \param overwrite An optional flag that marks if texture coordinates are to be overwritten.
   *  \remarks On all VertexAttributeSets that have at least two-dimensional texture coordinates
   *  in texture unit \a tu, tangents and binormals can be calculated. If \a overwrite is \c true
   *  (the default), tangents and binormals are calculated. If otherwise \a overwrite is \c false
   *  they are only calculated, if there are no texture coordinates in the texture units \a tg
   *  and \a bn.
   *  \note The behavior is undefined if a VertexAttributeSet has no normals.
   *  \sa GeoNode, QuadMeshes, Quads, QuadStrips, Triangles, TriFans, TriStrips, generateNormals,
   *  generateTexCoords */
  NVSG_API void calculateTangentSpace( GeoNode * gn
                                     , size_t tu = 0
                                     , size_t tg = 1
                                     , size_t bn = 2
                                     , bool overwrite = true );

  /*! \relates GeoNode
   *  \brief Generate normals for all QuadMeshes, Quads, QuadStrip, Triangles, TriFans, and
   *  TriStrip in a GeoNode.
   *  \param gn A pointer to the constant GeoNode to create normals for.
   *  \sa GeoNode, QuadMeshes, Quads, QuadStrips, Triangles, TriFans, TriStrips, generateTexCoords */
  NVSG_API void generateNormals( GeoNode * gn );

  /*! \relates GeoNode
   *  \brief Generate texture coordinates on all Drawable objects.
   *  \param gn A pointer to the constant GeoNode to generate texture coordinates for.
   *  \param tct The TextureCoordType to use for texture coordinate generation.
   *  \param tu  The optional texture unit to fill with texture coordinates; default is 0.
   *  \param overwrite An optional flag that marks if texture coordinates are to be overwritten.
   *  \remarks If \a overwrite is \c true (the default), on all VertexAttributeSets in this
   *  GeoNode texture coordinates are generated on texture unit /a tu. If otherwise \a overwrite
   *  is \c false, texture coordinates are only generated on those VertexAttributeSets that do
   *  not already have texture coordinates on texture unit \a tu. The texture coordinates can be
   *  created as cylindrical, planar, or spherical, determined by \a tct. The coordinates are
   *  created relative to the bounding sphere of the complete GeoNode.\n
   *  \sa PrimitiveSet, TextureCoordType */
  NVSG_API void generateTexCoords( GeoNode * gn
                                 , TextureCoordType tct
                                 , size_t tu = 0
                                 , bool overwrite = true );

  // - - - - - - - - - - - - - - - - - - - - - - - - - 
  // inlines
  // - - - - - - - - - - - - - - - - - - - - - - - - - 

  inline DrawableHandle * GeoNode::getDrawable(size_t geometryIndex, size_t drawableIndex) const
  {
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_READ_LOCKED(this);
    NVSG_ASSERT(geometryIndex<m_geometries.size());
    NVSG_ASSERT(drawableIndex<m_geometries[geometryIndex].m_drawables.size());
    return( m_geometries[geometryIndex].m_drawables[drawableIndex] );
  }

  inline size_t GeoNode::getNumberOfDrawables(size_t geometryIndex) const
  {
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_READ_LOCKED(this);
    NVSG_ASSERT(geometryIndex<m_geometries.size());
    return( m_geometries[geometryIndex].m_drawables.size() );
  }

  inline size_t GeoNode::getNumberOfGeometries() const
  {
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_READ_LOCKED(this);
    return( m_geometries.size() );
  }

  inline StateSetHandle * GeoNode::getStateSet( size_t geometryIndex ) const
  {
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_READ_LOCKED(this);
    NVSG_ASSERT(geometryIndex<m_geometries.size());
    return( m_geometries[geometryIndex].m_stateSet );
  }

  inline StateSetHandle * GeoNode::getStateSet( DrawableHandle * drawable ) const
  {
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_READ_LOCKED(this);
    NVSG_ASSERT( drawable );
    size_t gi = getGeometryIndex( drawable );
    return( ( gi != -1 ) ? m_geometries[gi].m_stateSet : NULL );
  }

} //  namespace nvsg
