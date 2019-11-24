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
#include "nvtraverser/OptimizeTraverser.h"

namespace nvtraverser
{
  //! Traverser that eliminates unneccessary LODs.
  /** A LOD is unneccessary if it holds only one child (and no ranges). Thent the LOD can simply be replaced by
    * it's child. */
  class LODEliminateTraverser : public OptimizeTraverser
  {
    public:
      //! Constructor
      NVSG_API LODEliminateTraverser( void );

    protected:
      //! Protected destructor to prevent instantiation of a LODEliminateTraverser on stack.
      NVSG_API virtual ~LODEliminateTraverser( void );

      //! If the root node is a LOD with a single child, it is removed.
      NVSG_API virtual void  postApply( nvsg::NodeHandle * root );

      //! If the AnimatedTransform holds LODS with one child, they are replaced by their respective child.
      /** An AnimatedTransform is handled as a Transform. */
      NVSG_API virtual void handleAnimatedTransform( nvsg::AnimatedTransform *p );

      //! If the Billboard holds LODs with one child, they are replaced by their respective child.
      NVSG_API virtual void handleBillboard( nvsg::Billboard *p );

      //! If the Group holds LODs with one child, they are replaced by their respective child.
      NVSG_API virtual void handleGroup( nvsg::Group * p );

      //! If the LOD holds LODs with one child, they are replaced by their respective child.
      NVSG_API virtual void handleLOD( nvsg::LOD *p );

      //! If the Switch holds LODs with one child, they are replaced by their respective child.
      NVSG_API virtual void handleSwitch( nvsg::Switch *p );

      //! If the Transform holds LODs with one child, they are replaced by their respective child.
      NVSG_API virtual void handleTransform( nvsg::Transform *p );

    private:
      void eliminateLODs( nvsg::Group *p );
      bool isLODCandidate( const nvsg::LOD * p );
  };
}
