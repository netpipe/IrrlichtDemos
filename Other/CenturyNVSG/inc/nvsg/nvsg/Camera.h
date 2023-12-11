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
#include "nvsg/Object.h" // base class definition
#include "nvsg/CullData.h"
#include "nvmath/Vec2f.h"
#include "nvmath/Vec3f.h"
#include "nvmath/Mat44f.h"
#include "nvmath/Quatf.h"

//  forward declarations
namespace nvmath
{
  class Mat44f;
  class Vec2f;
}

namespace nvsg
{
  /*! \brief Abstract base class for parallel and perspective cameras.
   *  \par Namespace: nvsg
   *  \remarks Besides defining the interface for a camera, a Camera provides some basic
   *  functionalities needed or used in all Camera classes.\n
   *  A Camera has a focus, a near distance, and a far distance. These define planes that are orthogonal to the
   *  view direction. The near and far distance bind the volume
   *  seen by this Camera. The focus distance is somewhere in between the near and far distance. At
   *  that distance the window size is used, defining the sides of the viewing volume. For stereo
   *  vision, the focus distance determines the plane in focus.\n
   *  A Camera holds zero or more LightSource objects as head lights that are positioned relative
   *  to the Camera position. These LightSource objects are not part of a Scene, but part
   *  of the Camera.\n
   *  For deriving your own Camera, you have to overload the member functions clone(),
   *  getProjection(), getInverseProjection(), and getPickRay(). You can also overload
   *  setFocusDistance() and validateIncarnation().
   *  \sa Object, ParallelCamera, PerspectiveCamera */
  class Camera : public Object
  {
    public:
      /*! \brief Destructs a Camera. */
      NVSG_API virtual ~Camera( void );

      /*! \brief Get the position of the Camera.
       *  \return A reference to the constant position of the Camera
       *  \remarks The position of the Camera is given in world coordinates. The default position
       *  of a Camera is at (0.0,0.0,1.0).
       *  \sa getDirection, getOrientation, getUpVector, setPosition */
      NVSG_API const nvmath::Vec3f & getPosition( void ) const;

      /*! \brief Set the position of the Camera.
       *  \param pos A reference to the constant position to set
       *  \remarks The position of the Camera is given in world coordinates. The default position
       *  of a Camera is at (0.0,0.0,1.0). Setting the position increases the Incarnation of the
       *  Camera.
       *  \sa getPosition, setDirection, setOrientation, setUpVector */
      NVSG_API void setPosition( const nvmath::Vec3f & pos );

      /*! \brief Get the orientation of the Camera.
       *  \return A reference to the constant orientation of the Camera
       *  \remarks The orientation quaternion describes the rotation of the Camera from its
       *  standard orientation. The standard orientation is the negative z-axis (0.0,0.0,-1.0) as
       *  the view direction and the y-axis (0.0,1.0,0.0) as the up vector.
       *  \sa getDirection, getUpVector, setOrientation */
      NVSG_API const nvmath::Quatf & getOrientation( void ) const;

      /*! \brief Set the orientation of the Camera by a Quaternion.
       *  \param quat A reference to the constant orientation of the Camera.
       *  \remarks The orientation of the Camera is the rotation from the default to the desired
       *  view direction and up vector. The default view direction is the negative z-axis
       *  (0.0,0.0,-1.0), and the default up vector is the y-axis (0.0,1.0,0.0). Setting the
       *  orientation is a simple way to set the view direction and the up vector in a consistent
       *  way. Setting the orientation increases the Incarnation.
       *  \sa getOrientation, setDirection, setUpVector */
      NVSG_API void setOrientation( const nvmath::Quatf & quat );

      /*! \brief Set the orientation of the Camera by a view direction and an up vector.
       *  \param dir  A reference to the constant view direction to set
       *  \param up   A reference to the constant up vector to set.
       *  \remarks Sets the orientation of the Camera by simultaneously setting the view direction
       *  and the up vector. The direction and the up vector have to be normalized and orthogonal
       *  to each other. Setting the orientation increases the Incarnation.
       *  \sa getOrientation, setDirection, setUpVector */
      NVSG_API void setOrientation( const nvmath::Vec3f & dir
                                  , const nvmath::Vec3f & up );

      /*! \brief Get the view direction.
       *  \return The view direction of the Camera
       *  \remarks The view direction is the vector in world space that defines the direction to which the
       *  Camera looks. The default view direction is the negative z-axis (0.0,0.0,-1.0).
       *  \sa getOrientation, getPosition, getUpVector, setDirection */
      NVSG_API nvmath::Vec3f getDirection( void ) const;

      /*! \brief Set the viewing direction.
       *  \param dir A reference to the constant view direction to set
       *  \remarks The view direction is the vector in world space that defines the direction to which the
       *  Camera looks. The default view direction is the negative z-axis (0.0,0.0,-1.0). Setting
       *  the view direction changes the orientation of the Camera and might also change the up
       *  vector. Setting the view direction also increases the Incarnation of the Camera.
       *  \sa getDirection, setOrientation, setPosition, setUpVector */
      NVSG_API void setDirection( const nvmath::Vec3f & dir );

      /*! \brief Get the up vector of the Camera.
       *  \return The up vector of the Camera
       *  \remarks The up vector is the vector in world space that defines the up direction of the
       *  Camera. The default up vector is the y-axis (0.0,1.0,0.0).
       *  \sa getDirection, getOrientation, getPosition, setUpVector */
      NVSG_API nvmath::Vec3f getUpVector( void ) const;

      /*! \brief Set the up vector of the Camera.
       *  \param up A reference to the constant up vector to set
       *  \remarks The up vector is the vector in world space that defines the up direction of the
       *  Camera. The default up vector is the y-axis (0.0,1.0,0.0). Setting the up vector changes
       *  the orientation of the Camera and might also change the view direction. Setting the up vector 
       *  also increases the Incarnation of the Camera.
       *  \sa getUpVector, setDirection, setOrientation, setPosition */
      NVSG_API void setUpVector( const nvmath::Vec3f & up );

      /*! \brief Get the world-to-view transformation.
       *  \return The world-to-view transformation
       *  \remarks Based on the position and orientation of the Camera, the transformation as a
       *  nvmath::Mat44f is calculated.
       *  \sa getOrientation, getPosition, getViewToWorldMatrix */
      NVSG_API nvmath::Mat44f getWorldToViewMatrix( void ) const;

      /*! \brief Get the view-to-world transformation.
       *  \return The view-to-world transformation.
       *  \remarks Based on the position and orientation of the Camera, the transformation as a
       *  nvmath::Mat44f is calculated.
       *  \sa getOrientation, getPosition, getWorldToViewMatrix */
      NVSG_API nvmath::Mat44f getViewToWorldMatrix( void ) const;

      /*! \brief Get the distance to the near clipping plane.
       *  \return The distance from the Camera to the near clipping plane
       *  \remarks The distance to the near clipping plane determines the front plane of the view
       *  frustum of this Camera. All objects in front of this plane are clipped.\n
       *  The near clipping distance has to be positive and less than the far clipping distance.
       *  The ratio of far-to-near gives the depth resolution of the Camera. Therefore, near
       *  should be as large as possible, while far should be as small as possible.\n
       *  \sa calcNearFarDistances, getFarDistance, setNearDistance */
      NVSG_API float getNearDistance( void ) const;

      /*! \brief Set the distance to the near clipping plane.
       *  \param nd The distance from the Camera to the near clipping plane
       *  \remarks The distance to the near clipping plane determines the front plane of the view
       *  frustum of this Camera. All objects in front of this plane are clipped.\n
       *  The near clipping distance has to be positive and less than the far clipping distance.
       *  The ratio of far-to-near gives the depth resolution of the Camera. Therefore, near
       *  should be as large as possible, while far should be as small as possible.\n
       *  \sa calcNearFarDistances, getNearDistance, setFarDistance */
      NVSG_API void setNearDistance( float nd );

      /*! \brief Get the distance to the projection plane.
       *  \return The distance from the Camera position to the projection plane.
       *  \remarks The focus distance is the orthogonal distance between the Camera and the
       *  projection plane. The focus distance should be greater-than or equal-to the near
       *  distance and less-than or equal-to the far distance. The default focus distance is 1.0.
       *  \sa getFarDistance, getNearDistance, setFocusDistance*/
      NVSG_API float getFocusDistance( void ) const;

      /*! \brief Set the distance to the projection plane.
       *  \param fd Distance to the projection plane to set
       *  \remarks The focus distance is the orthogonal distance between the Camera and the
       *  projection plane. The focus distance should be greater-than or equal-to the near
       *  distance and less-than or equal-to the far distance. The default focus distance is 1.0.
       *  Setting the focus distance increases the Incarnation of the Camera.
       *  \sa getFocusDistance, setNearDistance, setFarDistance */
      NVSG_API virtual void setFocusDistance( float fd );

      /*! \brief Get the distance to the far clipping plane.
       *  \return The distance from the Camera to the far clipping plane
       *  \remarks The distance to the far clipping plane determines the back plane of the view
       *  frustum of this Camera. All objects behind this plane are clipped.\n
       *  The far clipping distance has to be greater than the near clipping distance. The ratio of
       *  far-to-near gives the depth resolution of the Camera. Therefore, near should be as large
       *  as possible, while far should be as small as possible.\n
       *  \sa calcNearFarDistances, getNearDistance, setFarDistance */
      NVSG_API float getFarDistance( void ) const;

      /*! \brief Set the distance to the far clipping plane.
       *  \param fd The distance from the Camera to the far clipping plane
       *  \remarks The distance to the far clipping plane determines the back plane of the view
       *  frustum of this Camera. All objects in behind this plane are clipped.\n
       *  The far clipping distance has to be greater than the near clipping distance. The ratio of
       *  far-to-near gives the depth resolution of the Camera. Therefore, near should be as large
       *  as possible, while far should be as small as possible.\n
       *  \sa calcNearFarDistances, getFarDistance, setNearDistance */
      NVSG_API void setFarDistance( float fd );

      /*! \brief Calculate the distances to the near and far clipping planes.
       *  \param sphere A reference to the constant sphere, where the clipping planes are to be
       *  adjusted to.
       *  \remarks Calculates the optimal near and far clipping distances to prevent clipping of
       *  the nvmath::Sphere3f \a sphere without changing the position or orientation of the
       *  Camera.\n
       *  If the \a sphere is completely "behind" the Camera, nothing is changed. Otherwise, the
       *  near and far distances are adjusted. The far distance is adjusted to be at the far end
       *  of the sphere. If the \a sphere is partly "behind" the Camera, the near distance is set
       *  to be at the far distance divided by 16384. This gives an acceptable depth resolution,
       *  but might clip objects that are close to, and in front of, the Camera that would otherwise be
       *  visible. If the \a sphere is completely in front of the Camera, the near distance is
       *  adjusted to be at the near end of the sphere.
       *  Behavior is undefined if sphere is empty. This can be checked using nvmath::isEmpty
       *  \sa getFarDistance, getNearDistance, isAutoClipPlanes */
      NVSG_API void calcNearFarDistances( const nvmath::Sphere3f &sphere );

      /*! \brief Query if automatic clip plane adjustment is on.
       *  \return \c true, if automatic clip plane adjustment is on, otherwise \c false.
       *  \remarks When automatic clip plane adjustment is \c true, the framework lets the Camera
       *  calculate the clip planes to ensure the whole depth of the scene is visible. If the user
       *  has additional information about the scene that might help clipping, this should be
       *  turned off.
       *  \sa calcNearFarDistances, setAutoClipPlane */
      NVSG_API bool isAutoClipPlanes( void ) const;

      /*! \brief Enable/Disable automatic clip plane adjustment.
       *  \param on \c true enables automatic clip plane adjustment, \c disables it.
       *  \remarks When automatic clip plane adjustment is \c true, the framework lets the Camera
       *  calculate the clip planes to ensure the whole depth of the scene is visible. If the user
       *  has additional information about the scene that might help clipping, this should be
       *  turned off.
       *  \sa calcNearFarDistances, isAutoClipPlanes */
      NVSG_API void setAutoClipPlanes( bool on );

      /*! \brief Interface for getting the projection matrix of this Camera.
       *  \return The projection transformation
       *  \sa getInverseProjection, getWorldToViewMatrix, getViewToWorldMatrix */
      NVSG_API virtual nvmath::Mat44f getProjection( void ) const = 0;

      /*! \brief Interface for getting the inverse projection matrix of this Camera.
       *  \return The inverse projection transformation
       *  \sa getProjection,  getWorldToViewMatrix, getViewToWorldMatrix */
      NVSG_API virtual nvmath::Mat44f getInverseProjection( void ) const = 0;

      /*! \brief Get the window size.
       *  \return A constant reference to the size of the view window in world space
       *  \remarks The window size is the size of the world window that is viewed by this Camera.
       *  The size is the size of the view frustum at focus distance from the Camera. It is
       *  symmetric about the view direction and it thus defines the aspect ratio of the world
       *  window, as well as, for a perspective Camera, the field of view. The default window size
       *  is (1.0,1.0).
       *  \sa setWindowSize */
      NVSG_API const nvmath::Vec2f & getWindowSize( void ) const;

      /*! \brief Set the window size.
       *  \param size A constant reference to the size of the view in world space.
       *  \remarks The window size is the size of the world window that is viewed by this Camera.
       *  The size is the size of the view frustum at focus distance from the Camera. It is
       *  symmetric about the view direction and it thus defines the aspect ratio of the world
       *  window, as well as, for a perspective Camera, the field of view. The default window size
       *  is (1.0,1.0). Setting the window size increases the Incarnation of the Camera.
       *  \sa getWindowSize */
      NVSG_API void setWindowSize( const nvmath::Vec2f & size );

      /*! \brief Get the aspect ratio of the Camera window.
       *  \return The aspect ratio of the Camera window
       *  \remarks The aspect ratio is the width over the height of the Camera window.
       *  \sa getWindowSize, setAspectRatio */
      NVSG_API float getAspectRatio( void ) const;

      /*! \brief Set the aspect ratio of the Camera window.
       *  \param ar         Aspect ratio (width over height) of the Camera window.
       *  \param keepWidth  Optional boolean that controls whether the width or the height of the
       *  Camera window is modified.
       *  \remarks Setting the aspect ratio of the Camera window changes either the width or the
       *  height. With \a keepWidth equal to \c false (the default), the width is changed. With \a
       *  keepWidth equal to \a true, the height is changed. Setting the aspect ratio increases the
       *  Incarnation of the Camera.
       *  \sa getAspectRatio, setWindowSize */
      NVSG_API void setAspectRatio( float ar
                                  , bool keepWidth = false );

      /*! \brief Get the window offset.
       *  \return A reference to the constant window offset.
       *  \remarks The window offset is a value in world space that describes how to offset the
       *  view frustum for projection purposes. By default, the offset is (0.0,0.0). That is, the
       *  view frustum is symmetric about the view direction. Setting an offset, this view frustum
       *  is sheared a bit. This can be used, for example, in full scene antialiasing (FSAA) or
       *  stereo rendering.\n
       *  In FSAA mode, several passes are drawn where the Camera is kept in place, but the window
       *  is slightly offset (jittered) each time. This way, a somewhat blurred image is achieved.\n
       *  In stereo mode, two passes are drawn - once with the Camera shifted half the eye distance
       *  to the left, and once to the right. To adjust the view frustum accordingly, it is sheared a
       *  bit to the right and to the left, respectively.
       *  \sa setWindowOffset */
      NVSG_API const nvmath::Vec2f & getWindowOffset( void ) const;

      /*! \brief Set the window offset
       *  \param offset The window offset.
       *  \remarks The window offset is a value in world space that describes how to offset the
       *  view frustum for projection purposes. By default, the offset is (0.0,0.0). That is, the
       *  view frustum is symmetric about the view direction. Setting an offset, this view frustum
       *  is sheared a bit. This can be used, for example, in full scene antialiasing (FSAA) or
       *  stereo rendering.\n
       *  In FSAA mode, several passes are drawn where the Camera is kept in place, but the window
       *  is slightly offset (jittered) each time. This way, a somewhat blurred image is achieved.\n
       *  In stereo mode, two passes are drawn - once with the Camera shifted half the eye distance
       *  to the left, and once to the right. To adjust the view frustum accordingly, it is sheared a
       *  bit to the right and to the left, respectively.
       *  \sa getWindowOffset */
      NVSG_API void setWindowOffset( const nvmath::Vec2f &offset );

      /*! \brief Get the window region
       *  \param lowerLeft  A reference to the relative position of the lower left of the window
       *  region
       *  \param upperRight A reference to the relative position of the upper right of the window
       *  region
       *  \remarks The window region describes a relative part of the viewing window. Usually the
       *  view frustum of the Camera is symmetric about the view direction. To describe an
       *  asymmetric view frustum, you can use the window region.\n
       *  The default for the window region is (0.0,0.0) for the \a lowerLeft, and (1.0,1.0) for
       *  the \a upperRight, thus using the whole view window. To only use the upper left quarter
       *  of the complete view frustum, for example, you could specify \a lowerLeft as (0.0,0.5)
       *  and \a upperRight as (0.5,1.0).
       *  \sa getWindowSize, setWindowRegion */
      NVSG_API void getWindowRegion( nvmath::Vec2f & lowerLeft
                                   , nvmath::Vec2f & upperRight ) const;

      /*! \brief Set the window region
       *  \param lowerLeft  A reference to the constant relative position of the lower left of the
       *  window region to set
       *  \param upperRight A reference to the constant relative position of the upper right of the
       *  window region to set
       *  \remarks The window region describes a relative part of the viewing window. Usually the
       *  view frustum of the Camera is symmetric about the view direction. To describe an
       *  asymmetric view frustum, you can use the window region.\n
       *  The default for the window region is (0.0,0.0) for the \a lowerLeft, and (1.0,1.0) for
       *  the \a upperRight, thus using the whole view window. To only use the upper left quarter
       *  of the complete view frustum, for example, you could specify \a lowerLeft as (0.0,0.5)
       *  and \a upperRight as (0.5,1.0).
       *  \sa getWindowRegion, setWindowSize */
      NVSG_API void setWindowRegion( const nvmath::Vec2f & lowerLeft
                                   , const nvmath::Vec2f & upperRight );

      /*! \brief Get the number of headlights attached to this Camera.
       *  \return The number of headlights attached to this Camera
       *  \remarks A Camera can have zero or more headlights that are LightSource objects
       *  positioned relative to the Camera position.
       *  \sa LightSource, getHeadLight */
      NVSG_API size_t getNumberOfHeadLights( void ) const;

      /*! \brief Returns the camera's headlight at position \a index.
       *  \param index  Zero-based index of the headlight to get.
       *  \return A pointer to a LightSourceHandle specifying the headlight at position \a index.
       *  \remarks A Camera can have zero or more headlights that are LightSource objects
       *  positioned relative to the Camera position.
       *  \note The behavior is undefined if there are less than \a index LightSource objects
       *  attached.
       *  \sa LightSource, getNumberOfHeadLights, addHeadLight */
      NVSG_API LightSourceHandle * getHeadLight( size_t index ) const;

      /*! \brief Adds a LightSource as headlight to this Camera.
       *  \param light  A pointer to a LightSourceHandle specifying the LightSource to add as 
       *  headlight.
       *  \remarks A Camera can have zero or more headlights that are LightSource objects
       *  positioned relative to the Camera position. The default position and orientation of a
       *  headlight is the Camera position and orientation. The LightSource is added at the last
       *  position in the list of headlights of this Camera, and its reference count is increased.
       *  Adding a headlight increases the Incarnation of the Camera.
       *  \sa LightSource, getNumberOfHeadLights, getHeadLight, removeHeadLight */
      NVSG_API void addHeadLight( LightSourceHandle * light );

      /*! \brief Remove a headlight from this Camera.
       *  \param index  Index of headlight to remove.
       *  \remarks A Camera can have zero or more headlights that are LightSource objects
       *  positioned relative to the Camera position. The LightSorce at position \a index in the
       *  list of headlights is removed, and its reference count is decreased. Removing a
       *  headlight increases the Incarnation of the Camera.
       *  \note The behavior is undefined if there are less than \a index LightSource objects
       *  attached.
       *  \sa LightSource, getNumberOfHeadLights, addHeadLight */
      NVSG_API void removeHeadLight( size_t index );

      /*! \brief Clear the list of headlights.
       *  \remarks A Camera can have zero or more headlights that are LightSource objects
       *  positioned relative to the Camera position. The reference count of each LightSource in
       *  the list of headlights is decreased, and the list is cleared. Clearing the list of
       *  headlights increases the Incarnation of the Camera if there was at least one headlight
       *  in the list.
       *  \sa removeHeadeLight, getNumberOfHeadLights */
      NVSG_API void clearHeadLights( void );

      /*! \brief Interface for getting a pick ray in world coordinates for a given screen point.
       *  \param x          x-coordinate of the input screen point
       *  \param y          y-coordinate of the input screen point
       *  \param w          Width of the screen window in pixels
       *  \param h          Height of the screen window in pixels
       *  \param rayOrigin  Reference to an nvmath::Vec3f to get the ray origin
       *  \param rayDir     Reference to an nvmath::Vec3f to get the ray direction
       *  \remarks The function returns in \a rayOrigin the origin, and in \a rayDir the direction
       *  of the pick ray calculated from the screen point specified by its \a x and \a y
       *  coordinate.
       *  \note A screen point of (0,0) indicates the lower left of the currently considered screen
       *  window. */
      NVSG_API virtual void getPickRay( int x
                                      , int y
                                      , int w
                                      , int h
                                      , nvmath::Vec3f & rayOrigin
                                      , nvmath::Vec3f & rayDir ) const = 0;

      /*! \brief Move the Camera.
       *  \param delta          A reference to the constant delta to move
       *  \param cameraRelative Optional boolean controlling Camera relative or world relative
       *  movement. Default is Camera relative.
       *  \remarks Moves the Camera by \a delta. If \a cameraRelative is true (the default), the
       *  Camera is moved the corresponding amounts along the local axes. Otherwise \a delta is
       *  just added to the position of the Camera (being in world space).\n
       *  Moving the Camera increases the Incarnation.
       *  \sa moveX, moveY, moveZ, orbit, rotate, zoom, */
      NVSG_API void move( const nvmath::Vec3f & delta
                        , bool cameraRelative = true );

      /*! \brief Orbit the Camera.
       *  \param axis           A reference to the normalized axis to orbit around in Camera
       *  relative coordinates.
       *  \param targetDistance Distance to target point to orbit around.
       *  \param angle          Angle in radians to orbit.
       *  \param cameraRelative Optional boolean controlling Camera relative or world relative
       *  rotation. Default is Camera relative.
       *  \remarks Orbiting a Camera performs a rotation by \a angle around the point at
       *  targetDistance along the view direction, using the \a axis of rotation. If \a
       *  cameraRelative is \c true (the default), \a axis is a vector in Camera local coordinates.
       *  This way, the Camera is moved on a great circle around the target point, modifying the
       *  view direction to keep that target point. If otherwise \a cameraRelative is \c false \a
       *  axis is in world coordinates.\n
       *  Orbiting the Camera increases the Incarnation.
       *  \sa move, orbitX, orbitY, orbitZ, rotate, */
      NVSG_API void orbit( const nvmath::Vec3f & axis
                         , float targetDistance
                         , float angle
                         , bool  cameraRelative = true );

      /*! \brief Rotate the Camera.
       *  \param axis   A reference to the constant normalized axis to rotate around.
       *  \param angle  Angle in radians to rotate
       *  \param cameraRelative Optional boolean controlling Camera relative or world relative
       *  rotation. Default is Camera relative.
       *  \remarks Rotates the Camera around its position using \a axis of rotation by \a angle
       *  radians. If \a cameraRelative is true (the default), \a axis is a vector in Camera local
       *  coordinates. Otherwise \a axis is in world coordinates.\n
       *  Rotating the Camera increases the Incarnation.
       *  \sa move, orbit, rotateX, rotateY, rotateZ */
      NVSG_API void rotate( const nvmath::Vec3f& axis
                          , float angle
                          , bool  cameraRelative = true );

      /*! \brief Zoom by a positive factor.
       *  \param factor   Factor to use in zooming
       *  \remarks Zooming by a factor changes the window size of the Camera. The window size is
       *  the size of the world that is viewed at the projection plane. This plane is at a target
       *  distance along the view direction and orthogonal to the view direction.\n
       *  When the factor is less than one, the window size is reduced, and thus the viewport shows
       *  less content in the same space. That is, it is zoomed in. When the factor is greater than
       *  one, it is zoomed out.\n
       *  Zooming the Camera increases the Incarnation.
       *  \sa move, orbit, rotate, zoom */
      NVSG_API void zoom( float factor );

      /*! \brief Zoom to fit view to a sphere.
       *  \param sphere A reference to the constant sphere to zoom to.
       *  \param fovy   Optional parameter describing the field of view. Default keeps the current
	   *  field of view.
       *  \remarks Zooming to a sphere moves the Camera to view the complete sphere. The direction
       *  is not changed, but the position and the window size are changed. If the automatic clip plane
       *  adjustment is on, the near and far distances are set accordingly.\n
       *  Zooming the Camera increases the Incarnation.
       *  Behavior is undefined if sphere is empty. This can be checked using nvmath::isEmpty
       *  \sa setAutoClipPlanes, setPosition, setWindowSize */
      NVSG_API void zoom( const nvmath::Sphere3f & sphere
                        , float fovy = -1.0f );

      /*! \brief Convenience function to move to the right or left in Camera space.
       *  \param val  Amount to move to the right or left
       *  \remarks With positive \a val the Camera is moved to the right; with negative \a val it
       *  is moved to the left.
       *  \sa move */
      void moveX( float val );

      /*! \brief Convenience function to move up or down in Camera space.
       *  \param val  Amount to move up or down
       *  \remarks With positive \a val the Camera is moved up; with negative \a val it is moved
       *  down.
       *  \sa move */
      void moveY( float val );

      /*! \brief Convenience function to move backward or forward in Camera space.
       *  \param val  Amount to move backward or forward
       *  \remarks With positive \a val the Camera is moved backward; with negative \a val it is
       *  moved to the forward.
       *  \sa move */
      void moveZ( float val );

      /*! \brief Convenience function to orbit around the x-axis of the Camera.
       *  \param targetDistance Distance to target point to orbit around.
       *  \param angle          Angle in radians to orbit.
       *  \remarks With positive \a angle the Camera moves down while looking up; with negative
       *  \a angle it moves up while looking down.
       *  \sa orbit */
      void orbitX( float targetDistance
                 , float angle );

      /*! \brief Convenience function to orbit around the y-axis of the Camera.
       *  \param targetDistance Distance to target point to orbit around
       *  \param angle          Angle in radians to orbit
       *  \remarks With positive \a angle the Camera moves to the right while looking to the left;
       *  with negative \a angle the Camera moves to the left while looking to the right.
       *  \sa orbit */
      void orbitY( float targetDistance
                 , float angle );

      /*! \brief Convenience function to orbit around the z-axis of the Camera.
       *  \param targetDistance Distance to target point to orbit around
       *  \param angle          Angle in radians to orbit
       *  \remarks Orbiting around the z-axis of the Camera has the same effect as rotating around
       *  it (rolling). With positive \a angle it rolls to the left; with positive \a angle it
       *  rolls to the right.
       *  \sa orbit, rotateZ */
      void orbitZ( float targetDistance
                 , float angle );

      /*! \brief Convenience function to rotate around the x-axis of the Camera.
       *  \param angle  Angle in radians to rotate
       *  \remarks The Rotation around the x-axis of the Camera is also called Pitch. With positive
       *  \a angle the Camera looks up; with negative \a angle it looks down.
       *  \sa rotate */
      void rotateX( float angle );

      /*! \brief Convenience function to rotate around the y-axis of the Camera.
       *  \param angle  Angle in radians to rotate
       *  \remarks The Rotation around the y-axis of the Camera is also called Yaw. With positive
       *  \a angle the Camera looks to the left; with negative \a angle it looks to the right.
       *  \sa rotate */
      void rotateY( float angle );

      /*! \brief Convenience function to rotate around the z-axis of the Camera.
       *  \param angle  Angle in radians to rotate
       *  \remarks The Rotation around the y-axis of the Camera is also called Roll. With positive
       *  \a angle the Camera rolls to the left; with negative \a angle it rolls to the right. 
       *  \sa orbitZ, rotate */
      void rotateZ( float angle );

      /*! \brief Interface for determining the CullCode of a Sphere3f relative to the view volume.
       *  \param sphere A reference to the constant Sphere3f to determine the CullCode for.
       *  \return CC_IN, if the Sphere3f \a sphere is completely inside the view volume; CC_OUT if
       *  it is completely out of the view volume; otherwise CC_PART.
       *  \remarks Behavior is undefined if sphere is empty. This can be checked using nvmath::isEmpty() */
      NVSG_API virtual CullCode determineCullCode( const nvmath::Sphere3f &sphere ) const = 0;

    protected:
      /*! \brief Protected constructor to prevent explicit creation.
       *  \remarks Because Camera is an abstract class, its constructor is never used explicitly,
       *  but in the constructor of derived classes.
       *  \sa ParallelCamera, PerspectiveCamera */
      NVSG_API Camera( void );

      /*! \brief Protected copy constructor to prevent explicit creation.
       *  \param rhs       Reference to the constant Camera to copy from
       *  \remarks Because Camera is an abstract class, its constructor is never used explicitly,
       *  but in the constructor of derived classes.
       *  \sa ParallelCamera, PerspectiveCamera */
      NVSG_API Camera( const Camera& rhs );

      /*! \brief Protected assignment operator 
       *  \param rhs  Reference to the constant Camera to copy from
       *  \return A reference to the assigned Camera.
       *  \remarks Besides assigning the Camera as an Object, each head light is copied as well as
       *  all Camera defining parameters.
       *  \sa LightSource, Object */
      NVSG_API Camera & operator=( const Camera & rhs );

      /*! \brief Validate the Incarnation. 
       *  \remarks This function is called from the framework when the Incarnation is requested and
       *  marked as invalid. Then it is validated and the Incarnation of each headlight is
       *  requested to continue validation down the tree hierarchy.
       *  \a Incarnation, LightSource */
      NVSG_API virtual void validateIncarnation( void ) const;  // from top to bottom

    private:
      void copyHeadLights(const Camera& rhs);
      void removeHeadLights();

    private:
      typedef std::vector<LightSourceHandle *>::const_iterator ConstHeadlightIterator;
      typedef std::vector<LightSourceHandle *>::iterator HeadlightIterator;
      std::vector<LightSourceHandle *>  m_headLights;       //!< vector of the headlights

      nvmath::Quatf m_orientation;      //!< orientation of the Camera
      nvmath::Vec3f m_position;         //!< position of Camera

      bool          m_isAutoClipPlanes; //!< true, to use automatic generated clipping planes
      float         m_nearDist;         //!< distance from position to near clipping plane
      float         m_focusDist;        //!< distance from position to projection plane
      float         m_farDist;          //!< distance from position to far clipping plane

      nvmath::Vec2f m_windowSize;       //!< width (x) and height (y) of viewing window
      nvmath::Vec2f m_windowOffset;     //!< offset from the viewing reference point to the center of the viewing window
      nvmath::Vec2f m_lowerLeft;        //!< lower left position of window region to view
      nvmath::Vec2f m_upperRight;       //!< upper right position of window region to view

      mutable nvmath::Mat44f  m_viewToWorld;      //!< cached view to world matrix
      mutable bool            m_viewToWorldValid; //!< view to world matrix cache is valid
      mutable nvmath::Mat44f  m_worldToView;      //!< cached world to view matrix
      mutable bool            m_worldToViewValid; //!< world to view matrix cache is valid
  };

  inline float Camera::getAspectRatio( void ) const 
  { 
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_READ_LOCKED(this);
    return( m_windowSize[0] / m_windowSize[1] );  
  }

  inline nvmath::Vec3f Camera::getDirection( void ) const 
  { 
    NVSG_TRACE();  
    NVSG_ASSERT_OBJECT_READ_LOCKED(this);
    //  NOTE: normalizing shouldn't be necessary, but rounding errors do catch us!
    return( normalize( nvmath::Vec3f( 0.0f, 0.0f, -1.0f ) * m_orientation ) );
  }

  inline float Camera::getFarDistance( void ) const
  { 
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_READ_LOCKED(this);
    NVSG_ASSERT( m_nearDist < m_farDist );
    return( m_farDist ); 
  }

  inline float Camera::getFocusDistance( void ) const
  { 
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_READ_LOCKED(this);
    return( m_focusDist ); 
  }

  inline LightSourceHandle * Camera::getHeadLight( size_t index ) const
  {
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_READ_LOCKED(this);
    NVSG_ASSERT( index < m_headLights.size() );
    return m_headLights[index];
  }

  inline float Camera::getNearDistance( void ) const
  { 
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_READ_LOCKED(this);
    NVSG_ASSERT( m_nearDist < m_farDist );
    return( m_nearDist );  
  }

  inline size_t Camera::getNumberOfHeadLights( void ) const
  {
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_READ_LOCKED(this);
    return( m_headLights.size() );
  }

  inline const nvmath::Quatf & Camera::getOrientation( void ) const
  {
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_READ_LOCKED(this);
    return( m_orientation );
  }

  inline const nvmath::Vec3f & Camera::getPosition( void ) const 
  { 
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_READ_LOCKED(this);
    return( m_position ); 
  }

  inline nvmath::Vec3f Camera::getUpVector( void ) const 
  {
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_READ_LOCKED(this);
    NVSG_ASSERT( isNormalized( m_orientation ) );
    //  NOTE: normalizing shouldn't be necessary, but rounding errors do catch us!
    return( normalize( nvmath::Vec3f( 0.0f, 1.0f, 0.0f ) * m_orientation ) );
  }

  inline const nvmath::Vec2f & Camera::getWindowOffset( void ) const
  {
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_READ_LOCKED(this);
    return m_windowOffset;
  }

  inline void Camera::getWindowRegion( nvmath::Vec2f & lowerLeft, nvmath::Vec2f & upperRight ) const
  {
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_READ_LOCKED(this);
    lowerLeft = m_lowerLeft;
    upperRight = m_upperRight;
  }

  inline const nvmath::Vec2f & Camera::getWindowSize( void ) const 
  { 
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_READ_LOCKED(this);
    return( m_windowSize ); 
  }

  inline bool Camera::isAutoClipPlanes( void ) const 
  {
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_READ_LOCKED(this);
    return( m_isAutoClipPlanes );  
  }

  inline void Camera::setFarDistance( float fd )
  {
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_WRITE_LOCKED(this);
    NVSG_ASSERT( ( FLT_EPSILON < fd ) );
    m_farDist = fd;  
    increaseIncarnation();
  }

  inline void Camera::setNearDistance( float nd )
  { 
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_WRITE_LOCKED(this);
    NVSG_ASSERT( ( FLT_EPSILON <= nd ) );
    m_nearDist = nd; 
    increaseIncarnation();
  }

  inline void Camera::setWindowSize( const nvmath::Vec2f & size )
  {
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_WRITE_LOCKED(this);
    m_windowSize = size;  
    increaseIncarnation();
  }

  inline void Camera::setWindowOffset( const nvmath::Vec2f & offset )
  {
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_WRITE_LOCKED(this);
    m_windowOffset = offset;
    increaseIncarnation();
  }

  inline void Camera::setWindowRegion( const nvmath::Vec2f & lowerLeft, const nvmath::Vec2f & upperRight )
  {
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_WRITE_LOCKED(this);
    m_lowerLeft  = lowerLeft;
    m_upperRight = upperRight;
    increaseIncarnation();
  }

  inline void Camera::zoom( float factor )
  {
    NVSG_TRACE(); 
    NVSG_ASSERT_OBJECT_WRITE_LOCKED(this);
    NVSG_ASSERT( FLT_EPSILON < factor );
    m_windowSize *= factor;
    increaseIncarnation();
  }

  inline void Camera::moveX( float val )
  { 
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_WRITE_LOCKED(this);
    move( nvmath::Vec3f( val, 0.f, 0.f ) );
  }

  inline void Camera::moveY( float val )
  {
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_WRITE_LOCKED(this);
    move( nvmath::Vec3f( val, 1.f, 0.f ) );
  }

  inline void  Camera::moveZ( float val )
  { 
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_WRITE_LOCKED(this);
    move( nvmath::Vec3f( val, 0.f, 1.f ) ); 
  }

  inline void  Camera::orbitX( float targetDistance, float rad )
  { 
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_WRITE_LOCKED(this);
    orbit( nvmath::Vec3f( 1.f, 0.f, 0.f ), targetDistance, rad );
  }

  inline void  Camera::orbitY( float targetDistance, float rad )
  {
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_WRITE_LOCKED(this);
    orbit( nvmath::Vec3f( 0.f, 1.f, 0.f ), targetDistance, rad );
  }

  inline void  Camera::orbitZ( float targetDistance, float rad )
  { 
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_WRITE_LOCKED(this);
    orbit( nvmath::Vec3f( 0.f, 0.f, 1.f ), targetDistance, rad ); 
  }

  inline void  Camera::rotateX( float rad )
  {
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_WRITE_LOCKED(this);
    rotate( nvmath::Vec3f(1.f, 0.f, 0.f), rad );  
  }

  inline void  Camera::rotateY( float rad )
  { 
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_WRITE_LOCKED(this);
    rotate( nvmath::Vec3f(0.f, 1.f, 0.f), rad );  
  }

  inline void  Camera::rotateZ( float rad )
  {
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_WRITE_LOCKED(this);
    rotate( nvmath::Vec3f(0.f, 0.f, 1.f), rad ); 
  }

  template <typename T>
  inline T camera_cast(const Camera * camera)
  {
    NVSG_CTASSERT(!!(nvutil::Conversion<T,const Camera*>::exists));
    return dynamic_cast<T>(camera);
  }

  template <typename T>
  inline T camera_cast(Camera * camera)
  {
    NVSG_CTASSERT(!!(nvutil::Conversion<T,Camera*>::exists));
    return dynamic_cast<T>(camera);
  }
} // namespace
