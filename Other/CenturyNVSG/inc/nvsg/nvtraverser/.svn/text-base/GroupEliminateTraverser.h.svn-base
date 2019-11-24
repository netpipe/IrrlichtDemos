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
  //! Traverser that eliminates unneccessary Groups.
  /** A Group is unneccessary if one of the following holds:
    * - It is beneath a Group or a Transform and holds no lights
    * - It is beneath a LOD or a Switch or is the root node of the tree, and holds exactly one child.
    * Then the Group can simply be replaced by it child/children. */
  class GroupEliminateTraverser : public OptimizeTraverser
  {
    public:
      //! Constructor
      NVSG_API GroupEliminateTraverser( void );

    protected:
      //! Protected destructor to prevent instantiation of a GroupEliminateTraverser on stack.
      NVSG_API virtual ~GroupEliminateTraverser( void );

      //! If the root node is a Group with a single child, it is removed.
      NVSG_API virtual void  doApply( nvsg::NodeHandle * root );

      //! If the AnimatedTransform holds other Groups without lights, they are replaced by their children.
      /** An AnimatedTransform is handled as a Transform. */
      NVSG_API virtual void handleAnimatedTransform( nvsg::AnimatedTransform *p );

      //! If the Billboard holds Groups without lights, they are replaced by their children.
      NVSG_API virtual void handleBillboard( nvsg::Billboard *p );

      //! If the Group holds other Groups without lights, they are replaced by their children.
      NVSG_API virtual void handleGroup( nvsg::Group * p );

      //! If the LOD holds Groups with one child (no light!), they are replaced by their respective child.
      NVSG_API virtual void handleLOD( nvsg::LOD *p );

      //! If the Switch holds Groups with one child (no light!), they are replaced by their respective child.
      NVSG_API virtual void handleSwitch( nvsg::Switch *p );

      //! If the Transform holds Groups without lights, they are replaced by their children.
      NVSG_API virtual void handleTransform( nvsg::Transform *p );

    private:
      void eliminateGroups( nvsg::Group *p );
      void eliminateOneChildGroups( nvsg::Group *p );
      bool isOneChildGroupCandidate( const nvsg::Group * p );
  };
}
