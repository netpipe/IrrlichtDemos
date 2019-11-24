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
#include "nvsg/Camera.h"

namespace nvsg
{
  /*! \brief Class describing a perspective Camera.
   *  \par Namespace: nvsg
   *  \remarks A PerspectiveCamera overwrites the projection specific functions of a Camera.
   *  \sa Camera, ParallelCamera */
  class PerspectiveCamera : public Camera
  {
    public:
      /*! \brief Default-constructs a PerspectiveCamera. 
       *  \remarks The PerspectiveCamera initially is positioned at (0.0,0.0,1.0), has the y-axis
       *  as up-vector and looks down the negative z-axis. */
      NVSG_API PerspectiveCamera();

      /*! \brief Constructs a PerspectiveCamera from a Camera.
       *  \sa Camera */
      NVSG_API explicit PerspectiveCamera( const Camera &rhs );

      /*! \brief Copy-constructs a PerspectiveCamera from another PerspectiveCamera. */
      NVSG_API PerspectiveCamera( const PerspectiveCamera &rhs );

      /*! Destructs a PerspectiveCamera. */
      NVSG_API virtual ~PerspectiveCamera();

      /*! \brief Get the field of view.
       *  \return The field of view in radians.
       *  \remarks The field of view is the complete view angle (in radians) in the y-direction.
       *  \sa setFieldOfView */
      NVSG_API float getFieldOfView() const;

      /*! \brief Set the field of view.
       *  \param fov The field of view in radians.
       *  \remarks The field of view is the complete view angle (in radians) in the y-direction.
       *  \sa getFieldOfView */
      NVSG_API void setFieldOfView( float fov );

      //! Set the distance to the projection plane.
      /** On perspective cameras, setting the focus distance keeps the field of view constant. This means that
        * the window size is adjusted accordingly.  */

      /*! \brief Set the distance to the projection plane.
       *  \param fd The distance from the PerspectiveCamera to the projection plane.
       *  \remarks Setting the focus distance keeps the field of view constant. Therefore, the
       *  window size is adjusted accordingly.
       *  \sa getFieldOfView, getFocusDistance */
      NVSG_API virtual void setFocusDistance( float fd );

      /*! \brief Get the projection matrix of this PerspectiveCamera.
       *  \return The projection transformation.
       *  \sa Camera, getInverseProjection, getWorldToViewMatrix, getViewToWorldMatrix */
      NVSG_API virtual nvmath::Mat44f getProjection() const;

      /*! \brief Get the inverse projection matrix of this PerspectiveCamera.
       *  \return The inverse projection transformation.
       *  \sa Camera, getProjection,  getWorldToViewMatrix, getViewToWorldMatrix */
      NVSG_API virtual nvmath::Mat44f getInverseProjection() const;

      /*! \brief Get a pick ray in world coordinates for a given screen point.
       *  \param x          x-coordinate of the input screen point.
       *  \param y          y-coordinate of the input screen point.
       *  \param w          Width of the screen window in pixels.
       *  \param h          Height of the screen window in pixels.
       *  \param rayOrigin  Reference to an nvmath::Vec3f to get the ray origin.
       *  \param rayDir     Reference to an nvmath::Vec3f to get the ray direction.
       *  \remarks The function returns in \a rayOrigin the origin, and in \a rayDir the direction
       *  of the pick ray calculated from the screen point specified by its \a x and \a y
       *  coordinate. The ray origin is the camera position.
       *  \note A screen point of (0,0) indicates the lower left of the currently considered
       *  screen window. */
      NVSG_API virtual void getPickRay( int x
                                      , int y
                                      , int w
                                      , int h
                                      , nvmath::Vec3f & rayOrigin
                                      , nvmath::Vec3f & rayDir ) const;

      /*! \brief Assignment operator
       *  \param rhs A reference to the constant PerspectiveCamera to copy from.
       *  \return A reference to the assigned PerspectiveCamera.
       *  \remarks The assignment operator calls the assignment operator of Camera. */
      NVSG_API PerspectiveCamera & operator=(const PerspectiveCamera & rhs);

      /*! \brief Determine the CullCode of a Sphere3f relative to the view frustum.
       *  \param sphere A reference to the constant Sphere3f to determine the CullCode for.
       *  \return CC_IN, if the Sphere3f \a sphere is completely inside the view frustum; CC_OUT
       *  if it is completely out of the view frustum; otherwise CC_PART. */
      NVSG_API virtual CullCode determineCullCode( const nvmath::Sphere3f &sphere ) const;
  };

  inline float PerspectiveCamera::getFieldOfView() const
  {
    NVSG_TRACE();
    return( 2.0f * atanf( 0.5f * getWindowSize()[1] / getFocusDistance() ) );
  }
} // namespace
