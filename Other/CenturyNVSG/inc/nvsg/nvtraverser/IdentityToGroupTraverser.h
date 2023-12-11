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

#include <map>
#include "nvsgcommon.h"
#include "nvtraverser/OptimizeTraverser.h"

namespace nvtraverser
{
  //! Traverser that replaces identity transforms by groups.
  class IdentityToGroupTraverser : public OptimizeTraverser
  {
    public:
      //! Constructor
      NVSG_API IdentityToGroupTraverser( void );

    protected:
      //! Protected destructor to prevent instantiation of an IdentityToGroupTraverser on stack.
      NVSG_API virtual ~IdentityToGroupTraverser( void );

      //! If the root node is an identity Transform, it is replaced by a Group.
      NVSG_API virtual void postApply( nvsg::NodeHandle * root );

      //! Replace any identity Transform of it's children to a Group.
      /** An AnimatedTransform is handled as a Transform. */
      NVSG_API virtual void handleAnimatedTransform( nvsg::AnimatedTransform * p );

      //! Replace any identity Transform of it's children to a Group.
      NVSG_API virtual void handleBillboard( nvsg::Billboard *p );

      //! Replace any identity Transform of it's children to a Group.
      NVSG_API virtual void handleGroup( nvsg::Group *p );

      //! Replace any identity Transform of it's children to a Group.
      NVSG_API virtual void handleLOD( nvsg::LOD *p );

      //! Replace any identity Transform of it's children to a Group.
      NVSG_API virtual void handleSwitch( nvsg::Switch *p );

      //! Replace any identity Transform of it's children to a Group.
      NVSG_API virtual void handleTransform( nvsg::Transform *p );

    private:
      nvsg::GroupHandle * createGroupFromTransform( nvsg::TransformHandle *th );
      void                replaceTransforms( nvsg::Group *p );
  };
}
