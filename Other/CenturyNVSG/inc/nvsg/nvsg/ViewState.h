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
#include "nvsg/nvsg.h"
#include "nvsg/Camera.h"
#include "nvsg/MapList.h"

namespace nvsg
{
  class CullData;

  /*! \brief The ViewState class encapsulates the view-specific states. 
   *  \par Namespace: nvsg
   *  \remarks The ViewState encapsulates the view-specific information into one object.
   *  The ViewState defines how the user looks at the scene. It contains the camera and specific viewing
   *  settings like stereo or jitter view, does the bookkeeping of animation information, and
   *  it contains the actual cull information for this specific view into the scene.
   *  \n\n
   *  nvui::RenderArea, nvsg::ViewState and nvsg::Scene are the three major components that need 
   *  to come together to produce a rendered image. 
   *  The scene contains basically the tree, the ViewState defines how to look at
   *  the tree, and the RenderArea connects the the other two and provides a place and tools 
   *  (traverser) to render the final image.
   *  \par Example
   *  The following pseudo code shows how to set up this triple for Rendering:
   *  \code
   *  [...]
   *  // setup the Scene
   *  RCObject_AutoPtr< SceneHandle > hScene( CreateHandle( SceneHandle ) );
   *  {
   *    WritableObject< Scene > scene( hScene.get() );
   *    scene->setRootNode(pRoot);
   *  }
   *
   *  // setup the ViewState
   *  RCObject_AutoPtr< ViewStateHandle > hViewState( CreateHandle( ViewStateHandle ) );
   * 
   *  {
   *    WritableObject< ViewState > viewState( hViewState.get() );
   *    viewState->setCamera(CreateHandle( PerspectiveCameraHandle ) );
   *  }
   *  
   *  // bring RenderArea, Scene and ViewState together
   *  pRA = setupRenderArea();
   *  pRA->setSceneData( hScene.get(), hViewState.get() );
   *  [...]
   *  \endcode
   *  \sa nvui::RenderArea, Scene */
  class ViewState : public nvutil::HandledObject
  {
    public:
      /*! \brief Default-constructs a ViewState. 
       *  \remarks After instantiation, the ViewState has the following properties:
       *   - no animation running 
       *   - no camera defined
       *   - no jitter
       *   - no stereo 
       *   - no LOD range scaling
       *   - no cull information */
      NVSG_API ViewState();

      /*! \brief Copy-constructs a ViewState from another ViewState.
       *  \param rhs Source ViewState. */
      NVSG_API ViewState( const ViewState& rhs );

      /*! \brief Destructs a ViewState. */
      NVSG_API virtual ~ViewState();

      /*! \brief set the current animation frame number. 
       *  \param frame Null-based animation frame number.
       *  \remarks Set the number of the animation frame that should be used for rendering. 
       *  \sa getAnimationFrame */
      NVSG_API void setAnimationFrame(size_t frame );

      /*! \brief Get the current animation frame number.
       *  \return Null-based animation frame number.
       *  \remarks This function can be used to retrieve the current animation frame number. 
       *  \sa setAnimationFrame*/
      NVSG_API size_t getAnimationFrame() const;

      /*! \brief Test if there is currently an animation running.
       *  \return The return value of this function is true when an animation is currently running.
       *  \remarks This function can be used to determine the animation state of the view. 
       *  \sa setAnimation */
      NVSG_API bool isAnimating() const;
      
      /*! \brief Set the animation state of this view. 
       *  \param state Parse in true to enable animation, otherwise false.
       *  \remarks This function controls the animation state of the view. Animation can be turned
       *  on and off.
       *  \par Example
       *  The following sample code shows how to turn on/off the animation.
       *  \code
       *  [...]
       *  WritableObject< ViewState > pViewState( handleViewState );
       *
       *  if (!pViewState->isAnimating())
       *  {
       *    pViewState->setAnimation(true);
       *  
       *    m_animationManip = new AnimationManipulator;
       *    m_animationManip->setRenderArea(m_pRA);
       *  
       *    // We need to ask the scene for the complete numbers of frames.
       *    // Do not forget - there could be other animations even when there is no camera animation!
       *    m_animationManip->setNumberOfFrames(GetDocument()->getScene()->getNumberOfFrames());
       * 
       *    // Could be NULL, if there is no camera animation
       *    m_animationManip->setCameraAnimation(GetDocument()->getScene()->getCameraAnimation()); 
       *  } 
       *  else
       *  {
       *    pViewState->setAnimation(false);
       *
       *    if (m_animationManip)
       *    {
       *      delete m_animationManip;
       *      m_animationManip = NULL;
       *    }
       *  }
       *  [...]
       *  \endcode
       *  \sa nvui::AnimationManipulator, isAnimating */
      NVSG_API void setAnimation(bool state);

      /*! \brief Sets the Camera for this view. 
       *  \param camera Pointer to a CameraHandle specifying the Camera to set.
       *  \remarks The given camera is used for the rendering.
       *  \sa getCamera */
      NVSG_API void setCamera(CameraHandle * camera);

      /*! \brief Returns the current Camera. 
       *  \return A pointer to a CameraHandle specifying the Camera which is used in this view. 
       *  If there is no Camera associated with this view, the function returns NULL.
       *  \sa setCamera */
      NVSG_API CameraHandle * getCamera() const;

      /*! \brief Test if the view is in jitter mode. 
       *  \return The function returns true when the view is in jitter mode. 
       *  \remarks Jitter mode means that the current camera is used for several passes for 
       *  rendering but with a slightly changed (jittered) camera position. This mode can
       *  be used for software full-screen antialiasing, or motion blur, etc.
       *  \sa setJitter, getJitters, getNumberOfJitters */
      NVSG_API bool isJitter() const;

      /*! \brief Get the number of jitters set for this view.
       *  \return Number of jitter passes and jitter information.
       *  \remarks The number of jitters corresponds to the number of passes. So every jitter is one 
       *  pass during the rendering of the final image.
       *  \sa isJitter, setJitter, getJitters*/
      NVSG_API size_t getNumberOfJitters() const;

      /*! \brief Set the camera jitter information for this view.
       *  \param on Set this parameter to true if you want to turn on camera jittering.
       *  \param jitters Vector of two-dimensional displacement vectors.  
       *  \remarks Jitter values can be used for performing software full-screen antialiasing, for example.
       *  The displacements represent the number of pixels or fragment of the pixel the 
       *  camera is jittered in the x- and y-direction in screen space to achieve the effect.\n 
       *  Every jitter entry leads to a new rendering pass with the slightly moved camera.       
       *  \par Example
       *  This example shows how to set up 2x software FSAA (two pass rendering):
       *  \code
       *  [...]
       *  // set the jitter technique, in this case use the accumulation buffer
       *  RenderTraverser * renderTraverser = m_pRA->getRenderTraverser();
       *  renderTraverser->setJitterTechnique(JT_ACCUM);  
       *  
       *  // define the small camera offsets (jitter)
       *  vector<Vec2f> 
       *  jitter.resize(2); // 2x fsaa
       *  jitter[0] = Vec2f(0.25f, 0.75f);  // first pass
       *  jitter[1] = Vec2f(0.75f, 0.25f);  // second pass
       *  {
       *    WritableObject< ViewState > viewState( m_pRA->getViewState() );
       *    viewState->setJitter(true, jitter);
       *  }
       *  [...]
       *  \endcode
       *  \sa getJitters, getNumberOfJitters, isJitter
       *  RenderTraverser::setJitterTechnique, RenderTraverser::JitterTechnique */
      NVSG_API void setJitter(bool on, const std::vector<nvmath::Vec2f> &jitters);

      /*! \brief Retrieve the currently used jitter values. 
       *  \return Pointer to a constant array of jitter vectors (first element). 
       *  If there are no jitters defined, the return value is NULL.
       *  \remarks This function can be used to retrieve the currently used offsets for
       *  the camera jittering. The application should call ViewState::getNumberOfJitters
       *  to determine the size of the array containing the jitter values.
       *  \sa setJitter, getNumberOfJitters, isJitter*/
      NVSG_API const nvmath::Vec2f * getJitters() const;

      /*! \brief Get the distance to the target point.
       *  \return The distance from the current view point to the target point.
       *  \remarks The target point is used as the reference point for rotations of the camera.
       *  \sa setTargetDistance*/
      NVSG_API float getTargetDistance() const;

      /*! \brief Set the distance to the target point.
       *  \param dist distance between the view point (camera position) and the target point.
       *  \remarks The target point is used as the reference point for rotations of the camera.
       *  \sa getTargetDistance*/
      NVSG_API void setTargetDistance(float dist);

      /*! \brief Test if the view is in stereo mode.
       *  \return This function returns true when the view is rendered in stereo.
       *  \sa setStereo */
      NVSG_API bool isStereo() const;

      /*! \brief Turn stereo rendering on and off. 
       *  \param state To turn stereo on the application must pass in true. 
       *  False turns stereo rendering off.
       *  \remarks This function controls whether stereoscopic rendering is turned on or off. 
       *  \sa isStereo, toggleStereo */
      NVSG_API void setStereo(bool state);

      /*! \brief Toggle stereo rendering on and off. 
       *  \remarks Use this method to toggle between stereoscopic and non-stereoscopic rendering. 
       *  \sa isStereo, setStereo */
      NVSG_API void toggleStereo();

      /*! \brief Test on automatic eye distance calculation.   
       *  \return This function returns true when the distance is automatically calculated. 
       *  \remarks This function can be used to test if the automatic eye distance
       *  calculation for stereo is turned on. The automatic eye distance is calculated by  
       *  multiplying the adjustment factor by the focus distance of the camera.
       *  \sa setStereoAutomaticEyeDistanceAdjustment, getStereoAutomaticEyeDistanceFactor,
       *  setStereoAutomaticEyeDistanceFactor */
      NVSG_API bool isStereoAutomaticEyeDistanceAdjustment() const;

      /*! \brief Enable/Disable automatic eye distance adjustment.
       *  \param state Pass in true to enable automatic eye distance calculation. 
       *  \remarks The automatic eye distance is calculated by multiplying 
       *  the adjustment factor by the focus distance of the camera.
       *  \sa isStereoAutomaticEyeDistanceAdjustment, getStereoAutomaticEyeDistanceFactor,
       *  setStereoAutomaticEyeDistanceFactor */
      NVSG_API void setStereoAutomaticEyeDistanceAdjustment(bool state);

      /*! \brief Get the automatic eye distance adjustment factor.
       *  \return This function returns the eye distance adjustment factor. 
       *  \remarks The automatic eye distance is calculated by multiplying 
       *  the adjustment factor by the focus distance of the camera.\n 
       *  \par Example
       *  \sa setStereoAutomaticEyeDistanceAdjustment, getStereoAutomaticEyeDistanceFactor,
       *  setStereoAutomaticEyeDistanceFactor */
      NVSG_API float getStereoAutomaticEyeDistanceFactor() const;
      
      /*! \brief Set the automatic eye distance adjustment factor.
       *  \param factor Distance factor. 
       *  \remarks The automatic eye distance is calculated by multiplying 
       *  the adjustment factor by the focus distance of the camera.\n 
       *  The default value is 0.03 (3%). This value represents the following setup:\n
       *  A person with an eye distance of about six cm sitting in front of the monitor, 
       *  where the monitor is about one meter away. This setup leads to very natural stereo 
       *  impression.\n
       *  \sa isStereoAutomaticEyeDistanceAdjustment, getStereoAutomaticEyeDistanceFactor,
       *  setStereoAutomaticEyeDistanceFactor */
      NVSG_API void setStereoAutomaticEyeDistanceFactor(float factor);

      /*! \brief Get the eye distance for stereo rendering. 
       *  \return This function returns the eye distance. If the camera of this ViewState is not 
       *  valid, the behavior is undefined.
       *  \remarks The eye distance can be automatically calculated or manually set by the 
       *  application. Make sure that a valid camera is defined when asking for the eye distance 
       *  since the the automatic eye distance calculation is based on the focus distance of
       *  the camera of this ViewState.
       *  \sa setStereoEyeDistance, 
       *  isStereoAutomaticEyeDistanceAdjustment, isStereoAutomaticEyeDistanceAdjustment,
       *  getStereoAutomaticEyeDistanceFactor,  setStereoAutomaticEyeDistanceFactor */
      NVSG_API float getStereoEyeDistance() const;

      /*! \brief Set the eye distance for stereo rendering. 
       *  \param distance Distance between the left and the right eye. 
       *  \remarks This function manually sets the eye distance for stereo rendering.
       *  \sa setStereoEyeDistance, 
       *  isStereoAutomaticEyeDistanceAdjustment, isStereoAutomaticEyeDistanceAdjustment,
       *  getStereoAutomaticEyeDistanceFactor,  setStereoAutomaticEyeDistanceFactor */
      NVSG_API void setStereoEyeDistance(float distance);

      /*! \brief Reverse the left and the right eye for stereo rendering. 
       *  \remark You will see the image for the left eye on the right eye and vice versa.
       *  \sa isStereoReversedEyes */
      NVSG_API void stereoReverseEyes();

      /*! \brief Test on reversed eyes in stereo rendering. 
       *  \return This function returns true when the left and the right eye are reversed. 
       *  \remarks If the eyes are reversed you will see the image for the left eye on the 
       *  right eye and vice versa.
       *  \sa stereoReverseEyes */
      NVSG_API bool isStereoReversedEyes() const;
      
      /*! \brief Set the LOD range scale factor.
       *  \param factor The factor to scale the LOD scale ranges. The default value is 1.f, 
       *  so the unscaled ranges are used.
       *  \remarks This function sets a scaling factor for LOD ranges of the LOD nodes in the tree.\n
       *  The scale factor can be used to globally scale the ranges without changing the LOD node 
       *  ranges directly. This can be used, for example, for scenes that were initially created for viewing 
       *  on small monitors. You can use this scaling to fine-tune these scenes for large projection
       *  walls by scaling the LOD levels to switch later to a lower resolution representation. 
       *  \sa LOD::getLODToUse, getLODRangeScale */
      NVSG_API void setLODRangeScale(float factor);

      //! Get the LOD range scale factor
      /** Default value is 1.0, so the unscaled ranges are used. */
      /*! \brief Get the LOD range scale factor.
       *  \return The LOD range scale factor for all the LOD nodes in the tree. 
       *  By default this factor is 1.f, so unscaled ranges of the LOD node will be used.
       *  \remarks The scale factor can be used to globally scale the ranges without changing the LOD node 
       *  ranges directly. This can be used, for example, for scenes that were initially created for viewing 
       *  on small monitors. You can use this scaling to fine-tune these scenes for large projection
       *  walls by scaling the LOD levels to switch later to a lower resolution representation. 
       *  \sa LOD::getLODToUse, setLODRangeScale */
      NVSG_API float getLODRangeScale() const;

      /*! \brief Clear the current cull-data tree. 
       *  \remarks This function simply clears the cull tree, 
       *  which means that all the culling information is reset.\n
       *  One scene graph can be used in different views, so every ViewState contains 
       *  a special cull tree corresponding to the view. 
       *  \sa getCullTree*/
      NVSG_API void clearCullTree();

      /*! \brief Get the root of the cull-data tree.
       *  \return A pointer to the constant cull-data tree.
       *  \remarks This function can be used to retrieve the current cull-data tree.
       *  One scene graph can be used in different views, so every ViewState contains 
       *  a special cull tree corresponding to the view.\n
       *  Each CullData corresponds to one object in a scene tree, so the cull-data tree
       *  is a somewhat simplified representation of the original scene-graph.
       *  \sa CullData, setCullTree */
      NVSG_API const CullData * getCullTree() const;

      /*! \brief set an existing cull-data tree as the current cull-data tree.
       *  \param cullTree Pointer to a constant cull-data tree. 
       *  \remarks Replace the existing cull-data tree with the given one. This function 
       *  takes care of cleaning up the old cull-data tree before the new one will be assigned.
       *  This function can be used when a special cull traverser generated a cull-data tree that
       *  needs to be set at once.
       *  \sa CullData, getCullTree*/
      NVSG_API void setCullTree( const CullData *cullTree );

      /*! \brief Returns the current MapList.
       *  \return A pointer to a MapListHandle specifying the MapList. 
       *  \remarks This function retrieves the current MapList.\n
       *  A MapList contains two-dimensional maps that can be used to render, for example, overlays 
       *  for Menus or textual information on top of the scene. 
       *  \par Example 
       *  See the MapListViewer tutorial on how to use this functionality.
       *  \sa MapList, MapObject, setMapList */
      NVSG_API MapListHandle * getMapList() const;

      /*! \brief Sets the MapList.
       *  \return A pointer to a MapListHandle specifying the MapList to set. 
       *  \remarks This function assigns a MapList.\n
       *  A MapList contains two-dimensional maps that can be used to render, for example, overlays
       *  for Menus or textual information on top of the scene. 
       *  \par Example 
       *  See the MapListViewer tutorial on how to use this functionality.
       *  \sa MapList, MapObject, getMapList */
      NVSG_API void setMapList( MapListHandle * ml );

      /*! \brief Returns the actual incarnation count of this ViewState.
       *  \return The function returns the actual incarnation count of this ViewState.
       *
       *  The returned incarnation count usually is used to test it against a related
       *  incarnation count maintained by some client to detect if changes have been
       *  made to the ViewState. */
      NVSG_API const nvutil::Incarnation& getIncarnation() const;
      NVSG_API const nvutil::Incarnation& getCullTreeIncarnation() const;

    private:
      // assigning a ViewState is not permitted!
      ViewState& operator=(const ViewState&);   

    private:
      size_t                        m_animationFrame; //!< frame number of the actual animation step
      CameraHandle                * m_camera;         //!< this camera renders the tree
      bool                          m_jitterState;
      std::vector<nvmath::Vec2f>    m_jitters;
      float                         m_targetDistance;
      bool                          m_stereoState;
      bool                          m_stereoAutomaticEyeDistanceAdjustment;
      float                         m_stereoAutomaticEyeDistanceFactor;
      float                         m_stereoEyeDistance;
      bool                          m_reversedEyes;
      float                         m_scaleLODRange;
      // the animation state might be modified by concurrently executing threads, 
      // declare it as volatile for this reason
      volatile bool                 m_animationState;

      const CullData * m_cullTree;
      const CullData * m_prevCullTree;

      MapListHandle * m_mapList;

      // will be incremented on edits
      nvutil::Incarnation m_incarnation;
      nvutil::Incarnation m_cullTreeIncarnation;
  };

  
  inline size_t ViewState::getAnimationFrame() const
  {
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_READ_LOCKED(this);
    return( m_animationFrame );
  }

  inline bool ViewState::isAnimating() const
  {
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_READ_LOCKED(this);
    return( m_animationState );
  }

  inline CameraHandle * ViewState::getCamera() const
  {
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_READ_LOCKED(this);
    return( m_camera );
  }

  inline void ViewState::setAnimationFrame( size_t frame )
  {
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_WRITE_LOCKED(this);
    m_animationFrame = frame;
    ++m_incarnation;
  }

  inline void ViewState::setAnimation( bool state )
  {
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_WRITE_LOCKED(this);
    m_animationState = state;
    ++m_incarnation;
  }

  inline void ViewState::setCamera( CameraHandle * pCamera )
  {
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_WRITE_LOCKED(this);
    if ( m_camera != pCamera )
    {
      if ( m_camera )
      {
        m_camera->removeRef();
      }
      m_camera = pCamera;
      if ( m_camera )
      {
        m_camera->addRef();
        if ( fabsf( m_targetDistance ) < FLT_EPSILON )
        {
          nvutil::ReadableObject<Camera> camera(m_camera);
          m_targetDistance = camera->getFocusDistance();
        }
      }
      ++m_incarnation;
	  // a new camera makes the current CullTree obsolete
	  ++m_cullTreeIncarnation;
    }
  }

  inline bool ViewState::isJitter() const
  {
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_READ_LOCKED(this);
    return( m_jitterState );
  }

  inline size_t ViewState::getNumberOfJitters() const
  {
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_READ_LOCKED(this);
    return( m_jitters.size() );
  }

  inline void ViewState::setJitter( bool bOn, const std::vector<nvmath::Vec2f> &jitters )
  {
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_WRITE_LOCKED(this);
    m_jitterState = bOn;
    m_jitters = jitters;
    ++m_incarnation;
  }

  inline const nvmath::Vec2f * ViewState::getJitters() const
  { // return address of first element or null if no elements available
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_READ_LOCKED(this);
    return( !m_jitters.empty() ? &m_jitters[0] : NULL );
  }

  inline float ViewState::getTargetDistance() const
  {
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_READ_LOCKED(this);
    return( m_targetDistance );
  }

  inline void ViewState::setTargetDistance( float dist )
  {
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_WRITE_LOCKED(this);
    m_targetDistance = dist;
    ++m_incarnation;
  }

  inline bool ViewState::isStereo() const
  {
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_READ_LOCKED(this);
    return( m_stereoState );
  }

  inline void ViewState::setStereo( bool state )
  {
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_WRITE_LOCKED(this);
    m_stereoState = state;
    ++m_incarnation;
  }

  inline void ViewState::toggleStereo()
  {
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_WRITE_LOCKED(this);
    m_stereoState = ! m_stereoState;
    ++m_incarnation;
  }

  inline bool ViewState::isStereoAutomaticEyeDistanceAdjustment() const
  {
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_READ_LOCKED(this);
    return( m_stereoAutomaticEyeDistanceAdjustment );
  }

  inline void ViewState::setStereoAutomaticEyeDistanceAdjustment( bool state )
  {
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_WRITE_LOCKED(this);
    m_stereoAutomaticEyeDistanceAdjustment = state;
    ++m_incarnation;
  }

  inline float ViewState::getStereoAutomaticEyeDistanceFactor() const
  {
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_READ_LOCKED(this);
    return( m_stereoAutomaticEyeDistanceFactor );
  }

  inline void ViewState::setStereoAutomaticEyeDistanceFactor( float factor )
  {
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_WRITE_LOCKED(this);
    m_stereoAutomaticEyeDistanceFactor = factor;
    ++m_incarnation;
  }

  inline float ViewState::getStereoEyeDistance() const
  {
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_READ_LOCKED(this);
    if (m_stereoAutomaticEyeDistanceAdjustment)
    {
      NVSG_ASSERT(m_camera);
      nvutil::ReadableObject<Camera> camera(m_camera);
      return(m_stereoAutomaticEyeDistanceFactor * camera->getFocusDistance());
    }
    else
    {
      return(m_stereoEyeDistance);
    }
  }

  inline void ViewState::setStereoEyeDistance( float distance )
  {
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_WRITE_LOCKED(this);
    m_stereoEyeDistance = distance;
    ++m_incarnation;
  }

  inline void ViewState::stereoReverseEyes()
  {
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_WRITE_LOCKED(this);
    m_reversedEyes = !m_reversedEyes;
    ++m_incarnation;
  }

  inline bool ViewState::isStereoReversedEyes() const
  {
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_READ_LOCKED(this);
    return( m_reversedEyes );
  }

  inline const CullData * ViewState::getCullTree() const
  {
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_READ_LOCKED(this);
    return( m_cullTree );
  }

  inline void ViewState::setLODRangeScale(float factor)
  {
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_WRITE_LOCKED(this);
    m_scaleLODRange = factor;
    ++m_incarnation;
  }
  
  inline float ViewState::getLODRangeScale() const
  {
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_READ_LOCKED(this);
    return m_scaleLODRange;
  }

  inline MapListHandle * ViewState::getMapList() const
  {
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_READ_LOCKED(this);
    return( m_mapList );
  }

  inline void ViewState::setMapList( MapListHandle * ml )
  {
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_WRITE_LOCKED(this);
    if ( ml )
    {
      ml->addRef();
    }
    if ( m_mapList )
    {
      m_mapList->removeRef();
    }
    m_mapList = ml;
    ++m_incarnation;
  }

  inline const nvutil::Incarnation& ViewState::getIncarnation() const
  {
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_READ_LOCKED(this);
    return m_incarnation;
  }

  inline const nvutil::Incarnation& ViewState::getCullTreeIncarnation() const
  {
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_READ_LOCKED(this);
    return m_cullTreeIncarnation;
  }
} // namespace
