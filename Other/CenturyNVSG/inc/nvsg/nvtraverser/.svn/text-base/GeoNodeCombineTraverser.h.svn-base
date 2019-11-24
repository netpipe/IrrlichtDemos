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

#include <list>

#include "nvsgcommon.h"

#include "nvtraverser/OptimizeTraverser.h"

namespace nvtraverser
{
  //! Traverser that combines multiple GeoNodes under one Group to a single one.
  /** All geometries of all GeoNodes under one Group are reparented to a single GeoNode. */
  class GeoNodeCombineTraverser : public OptimizeTraverser
  {
    public:
      //! Constructor
      NVSG_API GeoNodeCombineTraverser( void );

    protected:
      //! Protected destructor to prevent instantiation of a GeoNodeCombineTraverser on stack.
      NVSG_API virtual ~GeoNodeCombineTraverser( void );

      //! Combine all GeoNodes directly underneath this AnimatedTransform.
      NVSG_API virtual void handleAnimatedTransform( nvsg::AnimatedTransform *p );

      //! Combine all GeoNodes directly underneath this Billboard.
      NVSG_API virtual void handleBillboard( nvsg::Billboard * p );

      //! Combine all GeoNodes directly underneath this Group.
      NVSG_API virtual void handleGroup( nvsg::Group * p );

      //! Combine all GeoNodes directly underneath this Transform.
      NVSG_API virtual void handleTransform( nvsg::Transform * p );

    private:
      void combineGeoNodes( nvsg::Group *p );
  };

}
