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
#include "nvsg/CoreTypes.h"
#include "nvsg/Group.h"
#include "nvsg/Node.h"

namespace nvsg
{
  /*! \brief Container class to hold all scene related information.
   *  \par Namespace: nvsg
   *  \remarks The Scene represents the loaded/created scene. It contains the ambient and back
   *  color, predefined cameras (views), predefined camera animations, and the tree of objects
   *  (the scene graph).
   *  \note The Scene holds a number of predefined cameras, but the ViewState holds the active
   *  camera.
   *  \sa Camera, SceneLoader, SceneSaver, ViewState */
  class Scene : public nvutil::HandledObject
  {
    public:
      /*! \brief Default-constructs a Scene.
       *  \remarks The Scene initially has an ambient color of light grey (0.2, 0.2, 0.2), and a
       *  background color of medium grey (0.4, 0.4, 0.4). By default there are no cameras, 
       *  no camera animations, and no tree object attached to the Scene. */
      NVSG_API Scene();

      /*! \brief Destructs a Scene. */
      NVSG_API virtual ~Scene();

      /*! \brief Get the global ambient color.
       *  \return A reference to the constant ambient color.
       *  \remarks This color represents light that is not from any particular source. The default
       *  is light grey (0.2,0.2,0.2).
       *  \sa getBackColor, setAmbientColor */
      NVSG_API const nvmath::Vec3f & getAmbientColor() const;

      /*! \brief Set the global ambient color.
       *  \param color A reference to the constant ambient color to set.
       *  \remarks This color represents light that is not from any particular source. The default
       *  is light grey (0.2,0.2,0.2).
       *  \sa getAmbientColor, setBackColor */
      NVSG_API void setAmbientColor( const nvmath::Vec3f & color );

      /*! \brief Get the background color.
       *  \return A reference to the constant background color.
       *  \remarks This color is used to clear the viewport. The default is medium grey
       *  (0.4,0.4,0.4).
       *  \sa getAmbientColor, setBackColor */
      NVSG_API const nvmath::Vec3f & getBackColor()  const;

      /*! \brief Set the background color.
       *  \param color A reference to the constant background color to set.
       *  \remarks This color is used to clear the viewport with. The default is medium grey
       *  (0.4,0.4,0.4).
       *  \sa getBackColor, setAmbientColor, */
      NVSG_API void setBackColor( const nvmath::Vec3f & color );

      /*! \brief Get the number of predefined Camera objects.
       *  \return The number of Camera objects in this Scene.
       *  \remarks A scene holds a list of Camera objects from which one can be selected for
       *  viewing.
       *  \sa addCamera, getCamera, removeCamera */
      NVSG_API size_t getNumberOfCameras() const;

      /*! \brief Returns the Camera at the specified index.
       *  \param index Optional index of the Camera to get. The default is 0.
       *  \return A pointer to a CameraHandle specifying the Camera at index \a index.
       *  \note The behavior is undefined if \a index is larger than or equal to the number of
       *  Camera objects in this Scene.
       *  \sa addCamera, getNumberOfCameras, removeCamera */
      NVSG_API CameraHandle * getCamera( size_t index = 0 ) const;

      /*! \brief Adds a Camera to the list of predefined views.
       *  \param camera A pointer to a CameraHandle specifying the Camera to add.
       *  \remarks The reference count of \a camera is incremented.
       *  \sa getCamera, getNumberOfCameras, removeCamera */
      NVSG_API void addCamera( CameraHandle * camera );

      /*! \brief Remove a Camera from the Scene.
       *  \param index Index of the camera to remove.
       *  \remarks The reference count of the Camera at index \a index is decremented.
       *  \note The behavior is undefined if \a index is larger than or equal to the number of
       *  Camera objects in this Scene.
       *  \sa addCamera, getCamera, getNumberOfCameras */
      NVSG_API void removeCamera( size_t index );

      /*! \brief Get the number of camera animations.
       *  \return The number of camera animations in this Scene.
       *  \remarks A scene holds a list of camera animations, from which one can be selected
       *  (independently from the Camera) for animated viewing.
       *  \sa addCameraAnimation, getCameraAnimation, removeCameraAnimation */
      NVSG_API size_t getNumberOfCameraAnimations() const;

      /*! \brief Returns the camera animation at the specified index.
       *  \param index Optional index of the camera animation to get. The default is 0.
       *  \return A pointer to a TrafoAnimationHandle specifying the camera animation at index 
       *  \a index.
       *  \remarks A scene holds a list of camera animations, from which one can be selected
       *  (independently from the Camera) for animated viewing.
       *  \note The behavior is undefined if \a index is larger than or equal to the number of
       *  camera animations in this Scene.
       *  \sa addCameraAnimation, getNumberOfCameraAnimations, removeCameraAnimation */
      NVSG_API TrafoAnimationHandle * getCameraAnimation( size_t index = 0 ) const;

      /*! \brief Adds a camera animation to the Scene.
       *  \param animation A pointer to a TrafoAnomationHandle specifying the camera animation 
       *  to add.
       *  \remarks A scene holds a list of camera animations, from which one can be selected
       *  (independently from the Camera) for animated viewing.\n
       *  The reference count of \a animation is incremented.
       *  \sa getCameraAnimation, getNumberOfCameraAnimations, removeCameraAnimation */
      NVSG_API void addCameraAnimation( TrafoAnimationHandle * animation );

      /*! \brief Remove a camera animation from the Scene.
       *  \param index Index of the camera animation to remove.
       *  \remarks The reference count of the camera animation at index \a index is decremented.
       *  \note The behavior is undefined if \a index is larger than or equal to the number of
       *  camera animations in this Scene.
       *  \sa addCameraAnimation, getCameraAnimation, getNumberOfCameraAnimations */
      NVSG_API void removeCameraAnimation( size_t index );

      /*! \brief Get the number of animation frames.
       *  \return The number of animation frames in this Scene.
       *  \remarks Currently, all animations in a scene must have the same number of frames.
       *  \sa getCameraAnimation */
      NVSG_API size_t getNumberOfFrames() const;

      /*! \brief Returns the root Node of the Scene.
       *  \return A pointer to a NodeHandle specifying the root Node of the Scene, 
       *  or NULL if there is no root Node specified yet.
       *  \sa setRootNode */
      NVSG_API NodeHandle * getRootNode() const;

      /*! \brief Sets the root Node of the Scene.
       *  \param root A pointer to a NodeHandle specifying the Node to set as the root Node.
       *  \remarks The reference count of \a root is incremented, and the reference count of any
       *  previous root node is decremented.
       *  \sa getRootNode */
      NVSG_API void setRootNode( NodeHandle * root );

      /*! \brief Ask if the Scene contains any StateAttribute of type CgFx.
       *  \return \c true, if the Scene contains a CgFx attribute, otherwise \c false.
       *  \remarks Because of the special display handling of a CgFx, it might be of interest if
       *  there's a CgFx attribute in the Scene at all.
       *  \sa containsLight, containsTransparentGeometry */
      NVSG_API bool containsCgFx() const;

      /*! \brief Ask if this Scene contains any LightSource.
       *  \return \c true, if the Scene contains a LightSource, otherwise \c false.
       *  \sa containsCgFx, containsTransparentGeometry */
      NVSG_API bool containsLight() const;

      /*! \brief Ask if this Scene contains any transparent StateAttribute.
       *  \return \c true, if the Scene contains a transparent StateAttribute, otherwise \c false.
       *  \sa containsCgFx, containsLight */
      NVSG_API bool containsTransparentGeometry() const;

    private:
      nvmath::Vec3f                        m_ambientColor;
      nvmath::Vec3f                        m_backColor;
      std::vector<CameraHandle*>           m_cameras;
      std::vector<TrafoAnimationHandle*>   m_cameraAnimations;
      NodeHandle                         * m_root;
  };

  inline void Scene::addCamera( CameraHandle * camera )
  {
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_WRITE_LOCKED(this);
    m_cameras.push_back( camera );
    camera->addRef();
  }

  inline CameraHandle * Scene::getCamera( size_t index ) const
  {
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_READ_LOCKED(this);
    NVSG_ASSERT( index < m_cameras.size() );
    return( m_cameras[index] );
  }

  inline void Scene::removeCamera( size_t index )
  {
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_WRITE_LOCKED(this);
    NVSG_ASSERT(index < m_cameras.size());

    m_cameras[index]->removeRef();
    m_cameras.erase(m_cameras.begin()+index);
  }

  inline void Scene::addCameraAnimation( TrafoAnimationHandle *animation )
  {
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_WRITE_LOCKED(this);
    m_cameraAnimations.push_back( animation );
    animation->addRef();
  }

  inline TrafoAnimationHandle * Scene::getCameraAnimation( size_t index ) const
  {
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_READ_LOCKED(this);
    NVSG_ASSERT( index < m_cameraAnimations.size() );
    return( m_cameraAnimations[index] );
  }

  inline void Scene::removeCameraAnimation( size_t index )
  {
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_WRITE_LOCKED(this);
    NVSG_ASSERT(index < m_cameraAnimations.size());

    m_cameraAnimations[index]->removeRef();
    m_cameraAnimations.erase(m_cameraAnimations.begin()+index);
  }

  inline size_t Scene::getNumberOfCameras() const
  {
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_READ_LOCKED(this);
    return( m_cameras.size() );
  }

  inline size_t Scene::getNumberOfCameraAnimations() const
  {
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_READ_LOCKED(this);
    return( m_cameraAnimations.size() );
  }

  inline NodeHandle * Scene::getRootNode() const
  {
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_READ_LOCKED(this);
    return( m_root );
  }

  inline bool Scene::containsCgFx() const
  {
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_READ_LOCKED(this);
    nvutil::ReadableObject<Node> root(m_root);
    return( root->containsCgFx() );
  }

  inline bool Scene::containsLight() const
  {
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_READ_LOCKED(this);
    return( dynamic_cast<GroupHandle *>(m_root)
            ? nvutil::ReadableObject<Group>(dynamic_cast<GroupHandle*>(m_root))->containsLight()
            : false );
  }

  inline bool Scene::containsTransparentGeometry() const
  {
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_READ_LOCKED(this);
    nvutil::ReadableObject<Node> root(m_root);
    return( root->containsTransparentGeometry() );
  }

  inline const nvmath::Vec3f& Scene::getAmbientColor() const
  {
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_READ_LOCKED(this);
    return( m_ambientColor );
  }

  inline void Scene::setAmbientColor( const nvmath::Vec3f &color )
  {
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_WRITE_LOCKED(this);
    m_ambientColor = color;
  }

  inline const nvmath::Vec3f& Scene::getBackColor() const
  {
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_READ_LOCKED(this);
    return( m_backColor );
  }

  inline void Scene::setBackColor( const nvmath::Vec3f &color )
  {
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_WRITE_LOCKED(this);
    m_backColor = color;
  }                                  
} //  namspace nvsg
