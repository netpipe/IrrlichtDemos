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

#include <set>
#include "nvmath/Mat44f.h"
#include "nvmath/Plane3f.h"
#include "nvsg/Node.h" // base class definition

namespace nvsg
{
  /*! \brief Class to hold a volume.
   *  \par Namespace: nvsg
   *  \remarks A VolumeNode in a scene graph is always a leaf node. It holds the data for one
   *  volume and the associated CgFx shader that does the rendering.
   *  \sa CgFx */
  class VolumeNode : public Node
  {
    public:
      /*! \brief Default constructor of a VolumeNode. */
      NVSG_API VolumeNode();

      /*! \brief Partial copy constructor of a VolumeNode.
       *  \param rhs Node to construct the VolumeNode from.
       *  \remarks This is kind of a partial copy constructor only copying the Node part from the
       *  source object. */
      NVSG_API explicit VolumeNode( const Node &rhs );

      /*! \brief Copy constructor of a VolumeNode. */
      NVSG_API VolumeNode( const VolumeNode& rhs );

      /*! \brief Destructor of a VolumeNode. */
      NVSG_API virtual ~VolumeNode();

      /*! \brief Create the active list of VolumeBricks for subsequent operations.
       *  \param cameraHandle A pointer to the Camera to use for selection.
       *  \param modelToView A reference to the constant model to view matrix.
       *  \param maxScalingFactor The maximal scaling factor used in the model to view matrix.
       *  \remarks This function selects some VolumeBricks as active for subsequent operations. A
       *  VolumeBrick is considered to be active, if it is a leaf (i.e. does not consist of
       *  subbricks), or isActive returns \c true.
       *  \par Example: The AppTraverser determines the active bricks as follows:
       *  \code
       *    p->determineActiveBricks( m_lockedViewState->getCamera()
       *                            , m_transformStack.getModelToView()
       *                            , maxElement( m_currentScaling.top() ) );
       *  \endcode
       *  \sa getActiveBricks, isActive */
      NVSG_API void determineActiveBricks( nvsg::CameraHandle * cameraHandle
                                         , const nvmath::Mat44f & modelToView
                                         , float maxScalingFactor );

      /*! \brief Get the currently active list of VolumeBricks.
       *  \return A reference to the constant list of currently active VolumeBricks.
       *  \remarks The active list of VolumeBricks is created by a call to determineActiveBricks.
       *  \par Example:
       *  \code
       *    const set<VolumeBrickHandle*> & activeBricks = p->getActiveBricks();
       *    for ( set<VolumeBrickHandle*>::const_iterator it=activeBricks.begin(); it!=activeBricks.end(); ++it )
       *    {
       *      // ... work on *it
       *    }
       *  \endcode
       *  \sa determineActiveBricks */
      NVSG_API const std::set<nvsg::VolumeBrickHandle*> & getActiveBricks() const;

      /*! \brief Get the shader that does the volume rendering
       *  \return A pointer to the CgFxHandle that does the volume rendering.
       *  \remarks This sampler has to have a sampler with the texture parameter named "volume"
       *  in order to do the correct shading. On rendering, this sampler is fed with the
       *  TextureImageHandle representing the volume data to render.
       *  Note also that the user must call VolumeNode::effectChanged method, if 
       *  the underlying effect is modified in any way. This includes tweakable updates
       *  change of sampler texture,  change in technique, etc.
       *  \sa setShader, effectChanged */
      NVSG_API CgFxHandle * getShader() const;

      /*! \brief Set the shader that does the volume rendering.
       *  \param shader A pointer to the CgFxHandle that has to do the volume rendering.
       *  \remarks This sampler has to have a sampler with the texture parameter named "volume"
       *  in order to do the correct shading. On rendering, this sampler is fed with the
       *  TextureImageHandle representing the volume data to render.
       *  \sa getShader, setVolumeData */
      NVSG_API void setShader( CgFxHandle * shader );

      /*! \brief Inform the VolumeNode that the underlying effect of the shader was modified
      *   \remarks The user must call VolumeNode::effectChanged method if 
      *   the underlying effect is modified in any way. This includes tweakable updates
      *   change of sampler texture,  change in technique, etc. The behavior is undefined
      *   if a this method is called if the shader is not set.
      *   \sa setShader, effectChanged */
      NVSG_API void effectChanged( void ) const;

      /*! \brief Get the name of the volume texture data set.
       *  \return A reference to the constant name of the volume texture data set.
       *  \sa getTopBrickTextureName, setFileNames */
      NVSG_API const std::string & getVolumeTextureName() const;

      /*! \brief Get the name of the top level brick texture data set.
       *  \return A reference to the constant name of the top level brick texture data set.
       *  \remarks The top level brick texture data set is a texture data set representing the
       *  complete volume texture data set at a lower resolution. There should be other brick
       *  texture data sets at the same place representing the volume texture data set at better
       *  resolutions. If for example <name> is the name of the top level brick texture data set,
       *  there can be eight files <name0>...<name7>, each representing one octel of the complete
       *  texture data set, each one with the same resolution as <name>. Next, there can be files
       *  <name00>...<name07>, each representing one octel of <name0>, and so on for each of the
       *  octel texture data set files. The final level, then, holds the same texture data as the
       *  original volume texture data set, but divided in a number of brick files.
       *  \sa getVolumeTextureName, setFileNames */
      NVSG_API const std::string & getTopBrickTextureName() const;

      /*! \brief Set the names of the volume data set and the top level brick data set.
       *  \param volumeTextureName The name of the volume texture data set.
       *  \param topBrickTextureName The name of the top level brick texture data set.
       *  \param searchPaths A vector of paths, where to search for the files
       *  \remarks The top level brick texture data set is a texture data set representing the
       *  complete volume texture data set at a lower resolution. There should be other brick
       *  data sets at the same place representing the volume texture data set at better
       *  resolutions. If for example <name> is the name of the top level brick texture data set,
       *  there can be eight files <name0>...<name7>, each representing one octel of the complete
       *  data set, each one with the same resolution as <name>. Next, there can be files
       *  <name00>...<name07>, each representing one octel of <name0>, and so on for each of the
       *  octel texture data set files. The final level, then, holds the same texture data as the
       *  original volume texture data set, but divided into a number of brick texture files.
       *  \sa getVolumeTextureName, getTopBrickTextureName */
      NVSG_API void setFileNames( const std::string & volumeTextureName
                                , const std::string & topBrickTextureName
                                , const std::vector<std::string> & searchPaths );

      /*! \brief Get the handle to the top level brick.
       *  \return A pointer to the top level VolumeBrick.
       *  \remarks A VolumeNode holds an octree of VolumeBricks, that represent the volume data
       *  set in a bricked form.
       *  \sa getBrickTextureSize, getBricksOverallSize, getTopBrickTextureName, setFileNames */
      NVSG_API VolumeBrickHandle * getTopLevelBrick() const;

      /*! \brief Get the size of the volume data.
       *  \return The size of the volume data represented by a 3D texture.
       *  \remarks The volume size is the size of the 3D texture with width in the first
       *  component of the returned array of integers, height in the second, and depth in the third.
       *  \sa getBrickTextureSize, getBricksOverallSize */
      NVSG_API const unsigned int * getVolumeTextureSize() const;

      /*! \brief Get the size of a brick data set.
       *  \return The size of a brick data set.
       *  \remarks Each brick has the same size, and it is the number of voxels in the three
       *  directions.
       *  \sa getBricksOverallSize, getVolumeTextureSize */
      NVSG_API const unsigned int * getBrickTextureSize() const;

      /*! \brief Get the overall size of the bricks.
       *  \return The overall size of the bricks
       *  \remarks The overall size of the bricks might be larger than the volume size, because the
       *  bricks might contain empty space to align their sizes to powers of two. The brick overall
       *  size is the size represented by the bricks.
       *  \sa getBrickTextureSize, getVolumeTextureSize */
      NVSG_API const nvmath::Vec3f & getBricksOverallSize() const;

      /*! \brief Get a TriFansHandle representing the front clipping plane of the VolumeNode.
       *  \return A pointer to the TriFansHandle representing the front clipping plane of the
       *  VolumeNode, or NULL if there is none.
       *  \remarks If there is a front clipping plane set for this VolumeNode, there is an
       *  internally created TriFansHandle returned.
       *  \sa setFrontClipPlane */
      NVSG_API const nvmath::Plane3f & getFrontClipPlane() const;

      /*! \brief Create a TriFansHandle representing the clip plane.
       *  \param plane A nvmath::Plane3f that describes the clip plane.
       *  \remarks If the nvmath::Plane3f \a plane intersects the box of this VolumeNode, a
       *  TriFansHandle representing the intersection is created. This TriFansHandle is used to
       *  generate fragments in front of the clip plane.
       *  \sa getFrontPlane */
      NVSG_API void setFrontClipPlane( const nvmath::Plane3f &plane );

      /*! \brief Query if this VolumeNode contains a CgFx StateAttribute.
       *  \return \c true if this VolumeNode is completely defined, otherwise \c false.
       *  \remarks Because a CgFx does not use the standard drawing pipeline, it might be of
       *  interest in several cases to know if there is a CgFx contained in a Node.
       *  \sa CgFx */
      NVSG_API virtual bool containsCgFx() const;

      /*! \brief Tests if this Node contains a VolumeNode. 
       *  \return \c true */
      NVSG_API virtual bool containsVolumeNode() const;

      /*! \brief Get the voxel limit fraction.
       *  \return The voxel limit fraction.
       *  \remarks The voxel limit fraction is the maximal fraction of the window a single voxel
       *  should span. This is used in determining if a VolumeBrick that has sub-bricks (i.e. is not
       *  a leaf in the octree of VolumeBricks) should be used for rendering, or if it's sub-bricks
       *  are to be tested for usage. If the approximated size of a voxel in the focus plane,
       *  divided by the smaller of the window size in x or y, is less than this voxel limit
       *  fraction, the VolumeBrick is used, otherwise, it's sub-bricks are tested for usage.
       *  \sa setVoxelLimitFraction, isActive */
      NVSG_API float getVoxelLimitFraction() const;

      /*! \brief Set the voxel limit fraction.
       *  \param vlf The voxel limit fraction.
       *  \remarks The voxel limit fraction is the maximal fraction of the window a single voxel
       *  should span. This is used in determining if a VolumeBrick that has sub-bricks (i.e. is not
       *  a leaf in the octree of VolumeBricks) should be used for rendering, or if it's sub-bricks
       *  are to be tested for usage. If the approximated size of a voxel in the focus plane,
       *  divided by the smaller of the window size in x or y, is less than this voxel limit
       *  fraction, the VolumeBrick is used, otherwise, it's sub-bricks are tested for usage.
       *  \sa getVoxelLimitFraction, isActive */
      NVSG_API void setVoxelLimitFraction( float vlf );

      /*! \brief Test for equivalence with an other VolumeNode.
       *  \param p A reference to the constant VolumeNode to test for equivalence with.
       *  \param ignoreNames Optional parameter to ignore the names of the objects; default is \c
       *  true.
       *  \param deepCompare Optional parameter to perform a deep comparison; default is \c false.
       *  \return \c true if the VolumeNode \a p is equivalent to \c this, otherwise \c false.
       *  \remarks If \a p and \c this are equivalent as Node, and they have the same number of
       *  StateSet objects with the same number of Drawable objects assigned, they are each tested for
       *  equivalence. If \a deepCompare is false, they are considered to be equivalent if
       *  they are the same pointers. Otherwise a full equivalence test is performed.
       *  \note The behavior is undefined if \a p is not a VolumeNode nor derived from one.
       *  \sa Node */
      NVSG_API virtual bool isEquivalent( const Object * p
                                        , bool ignoreNames
                                        , bool deepCompare ) const;

      /*! \brief Assignment operator
       *  \param rhs A reference to the constant VolumeNode to copy from
       *  \return A reference to the assigned VolumeNode
       *  \remarks The assignment operator calls the assignment operator of Node and copies
       *  from rhs.
       *  \sa Node */
      NVSG_API VolumeNode & operator=(const VolumeNode & rhs);

    protected:
      /*! \brief Calculate the bounding sphere of this VolumeNode.
       *  \return A nvmath::Sphere3f that contains the complete VolumeNode.
       *  \remarks This function is called by the framework to determine a sphere that completely
       *  contains the VolumeNode.
       *  \sa getBoundingSphere, invalidateBoundingSphere */
      NVSG_API virtual nvmath::Sphere3f calculateBoundingSphere() const;

      /*! \brief Determine the transparency containment state.
       *  \return \c true, if this VolumeNode holds any transparent data, otherwise \c false.
       *  \remarks This function is called by the framework to determine the transparency
       *  containment state of a Node when the transparency containment cache is marked as
       *  invalid.
       *  \sa Node, containsTransparentGeometry, isTransparent */
      NVSG_API virtual bool determineTransparencyContainment() const;

      /*! \brief Validate the nvutil::Incarnation.
       *  \remarks This function is called from the framework when the nvutil::Incarnation of
       *  this VolumeNode is requested but marked as invalid. Then it is validated and the
       *  nvutil::Incarnation of ... is requested to continue the validation down the tree
       *  hierarchy.
       *  \sa Node, nvutil::Incarnation, invalidateIncarnation */
      NVSG_API virtual void validateIncarnation() const;  // from top to bottom

      /*! \brief Determine if a VolumeBrick is active.
       *  \param vb A pointer to the read-locked VolumeBrick that is tested for being active.
       *  \param bs A constant reference to the correctly transformed and resized
       *  bounding sphere of the VolumeBrick \a vb.
       *  \param camera A pointer to the read-locked Camera to use.
       *  \return \c true, if the VolumeBrick \a vb is active, otherwise \c false.
       *  \remarks This function is called during determineActiveBricks for each VolumeBrick, that
       *  is not a leaf (has at least one sub-VolumeBrick). If \c true is returned, the VolumeBrick
       *  \a vb is considered to be active, and none of it's sub-VolumeBricks. Otherwise, all of
       *  it's sub-VolumeBricks are tested to be active.
       *  \sa determineActiveBricks */
      NVSG_API virtual bool isActive( const nvsg::VolumeBrick * vb, const nvmath::Sphere3f & bs
                                    , const nvsg::Camera * camera );

    private:
      NVSG_API void gatherActiveBricks( nvsg::VolumeBrickHandle * vbh
                                      , const nvsg::Camera * c
                                      , const nvmath::Mat44f & modelToView
                                      , float maxScalingFactor
                                      , std::set<nvsg::VolumeBrickHandle*> & bricks );

    private:
      std::string           m_topBrickTextureName;    // path to the bricks data files
      CgFxHandle          * m_shader;
      std::string           m_volumeTextureName;    // file name of volume texture data
      float                 m_voxelLimitFraction;

      std::set<nvsg::VolumeBrickHandle*>    m_activeBricks;           // set of active bricks
      VolumeBrickHandle                   * m_topLevelBrick;          // octree of bricks
      nvmath::Vec3f                         m_bricksOverallSize;      // overall size of the bricks
      unsigned int                          m_brickMinimalVoxelCount; // minimal size of the brick (in voxels)
      unsigned int                          m_brickOverlapSize;       // size of the overlap
      unsigned int                          m_brickTextureSize[3];    // size of the brick texture
      nvmath::Plane3f                       m_frontPlane;             // front clipping plane
      nvmath::Vec3f                         m_volumeSize;
      unsigned int                          m_volumeTextureSize[3];   // size of the data in the file m_volumeTextureName
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - 
  // inlines
  // - - - - - - - - - - - - - - - - - - - - - - - - - 

  inline const std::set<nvsg::VolumeBrickHandle*> & VolumeNode::getActiveBricks() const
  {
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_READ_LOCKED(this);
    return( m_activeBricks );
  }

  inline CgFxHandle * VolumeNode::getShader() const
  {
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_READ_LOCKED(this);
    return( m_shader );
  }

  inline VolumeBrickHandle * VolumeNode::getTopLevelBrick() const
  {
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_READ_LOCKED(this);
    return( m_topLevelBrick );
  }

  inline const unsigned int * VolumeNode::getVolumeTextureSize() const
  {
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_READ_LOCKED(this);
    return( m_volumeTextureSize );
  }

  inline const nvmath::Vec3f & VolumeNode::getBricksOverallSize() const
  {
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_READ_LOCKED(this);
    return( m_bricksOverallSize );
  }

  inline const std::string & VolumeNode::getTopBrickTextureName() const
  {
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_READ_LOCKED(this);
    return( m_topBrickTextureName );
  }

  inline const std::string & VolumeNode::getVolumeTextureName() const
  {
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_READ_LOCKED(this);
    return( m_volumeTextureName );
  }

  inline const nvmath::Plane3f & VolumeNode::getFrontClipPlane() const
  {
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_READ_LOCKED(this);
    return( m_frontPlane );
  }

  inline float VolumeNode::getVoxelLimitFraction() const
  {
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_READ_LOCKED(this);
    return( m_voxelLimitFraction );
  }

  inline void VolumeNode::setVoxelLimitFraction( float vlf )
  {
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_WRITE_LOCKED(this);
    m_voxelLimitFraction = vlf;
  }

} //  namespace nvsg
