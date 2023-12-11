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

#include <list>
#include "nvsgcommon.h"
#include "nvtraverser/OptimizeTraverser.h"

namespace nvtraverser
{
  //! Traverser that combines LODS with the same ranges under one Group or Transform to a single one.
  class LODCombineTraverser : public OptimizeTraverser
  {
    public:
      //! Constructor
      NVSG_API LODCombineTraverser( void );

    protected:
      //! Protected destructor to prevent instantiation of a GeoNodeCombineTraverser on stack.
      NVSG_API virtual ~LODCombineTraverser( void );

      //! Combine all compatible LOD objects directly underneath this AnimatedTransform.
      /** An AnimatedTransform is handled as a Transform. */
      NVSG_API virtual void handleAnimatedTransform( nvsg::AnimatedTransform *p );

      //! Combine all compatible LOD objects directly underneath this Billboard.
      NVSG_API virtual void handleBillboard( nvsg::Billboard * p );

      //! Combine all compatible LOD objects directly underneath this Group.
      NVSG_API virtual void handleGroup( nvsg::Group * p );

      //! Combine all compatible LOD objects directly underneath this Transform.
      NVSG_API virtual void handleTransform( nvsg::Transform * p );

    private:
      void combineLODs( nvsg::Group *p );
  };

}
