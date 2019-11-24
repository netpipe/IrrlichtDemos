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
  /*! \brief Class describing a parallel (non-perspective) Camera.
   *  \par Namespace: nvsg
   *  \remarks A ParallelCamera overwrites the projection-specific functions of a Camera.
   *  \sa Camera, PerspectiveCamera */
  class ParallelCamera : public Camera
  {
    public:
      /*! \brief Default-constructs a ParallelCamera. 
       *  \remarks The ParallelCamera initially is positioned at (0.0,0.0,1.0), has the y-axis
       *  as up-vector and looks down the negative z-axis. */
      NVSG_API ParallelCamera();

      /*! \brief Constructs a ParallelCamera from a Camera.
       *  \sa Camera */
      NVSG_API explicit ParallelCamera( const Camera &rhs );

      /*! \brief Copy-constructs a ParallelCamera from another ParallelCamera. */
      NVSG_API ParallelCamera( const ParallelCamera &rhs );

      /*! Destructs a ParallelCamera. */
      NVSG_API virtual ~ParallelCamera();

      /*! \brief Get the projection matrix of this ParallelCamera.
       *  \return The projection transformation.
       *  \sa Camera, getInverseProjection, getWorldToViewMatrix, getViewToWorldMatrix */
      NVSG_API virtual nvmath::Mat44f getProjection() const;

      /*! \brief Get the inverse projection matrix of this ParallelCamera.
       *  \return The inverse projection transformation.
       *  \sa Camera, getProjection,  getWorldToViewMatrix, getViewToWorldMatrix */
      NVSG_API virtual nvmath::Mat44f getInverseProjection()  const;

      /*! \brief Get a pick ray in world coordinates for a given screen point.
       *  \param x          x-coordinate of the input screen point.
       *  \param y          y-coordinate of the input screen point.
       *  \param w          Width of the screen window in pixels.
       *  \param h          Height of the screen window in pixels.
       *  \param rayOrigin  Reference to an nvmath::Vec3f to get the ray origin.
       *  \param rayDir     Reference to an nvmath::Vec3f to get the ray direction.
       *  \remarks The function returns in \a rayOrigin the origin, and in \a rayDir the direction
       *  of the pick ray calculated from the screen point specified by its \a x and \a y
       *  coordinate. The pick ray is a ray parallel to the camera direction, starting from a
       *  point, that is a displacement of the camera position parallel to the projection plane.
       *  \note A screen point of (0,0) indicates the lower left of the currently considered
       *  screen window. */
      NVSG_API virtual void getPickRay( int x
                                      , int y
                                      , int w
                                      , int h
                                      , nvmath::Vec3f & rayOrigin
                                      , nvmath::Vec3f & rayDir ) const;

      /*! \brief Assignment operator
       *  \param rhs A reference to the constant ParallelCamera to copy from.
       *  \return A reference to the assigned ParallelCamera.
       *  \remarks The assignment operator calls the assignment operator of Camera. */
      NVSG_API ParallelCamera & operator=(const ParallelCamera & rhs);

      /*! \brief Determine the CullCode of a Sphere3f relative to the view volume.
       *  \param sphere A reference to the constant Sphere3f to determine the CullCode for.
       *  \return CC_IN, if the Sphere3f \a sphere is completely inside the view volume; CC_OUT
       *  if it is completely out of the view volume; otherwise CC_PART. */
      NVSG_API virtual CullCode determineCullCode( const nvmath::Sphere3f &sphere ) const;
  };
} // namespace
