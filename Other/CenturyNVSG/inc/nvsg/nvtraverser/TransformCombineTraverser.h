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
  //! Traverser that combines identical Transforms.
  /** Identical Transforms beneath a Group or an other Transform are recognized and their children are combined to a single
    * Transform. This way, redundant objects are removed, thereby reducing storage space as well as run time.
    * \note When there is a LightSource inbetween of some Transform nodes, unifying is not allowed (and therefore not
    * performed) across this LightSource. */
  class TransformCombineTraverser : public OptimizeTraverser
  {
    public:
      //! Constructor
      NVSG_API TransformCombineTraverser( void );

    protected:
      //! Protected destructor to prevent instantiation of a StateSetUnifyTraverser on stack.
      NVSG_API virtual ~TransformCombineTraverser( void );

      //! Combine compatible Transform objects directly underneath this AnimatedTransform.
      /** An AnimatedTransform is handled as a Transform. */
      NVSG_API virtual void handleAnimatedTransform( nvsg::AnimatedTransform *p );

      //! Combine compatible Transform objects directly underneath this Billboard.
      /** A Billboard is handled as a Group. */
      NVSG_API virtual void handleBillboard( nvsg::Billboard * p );

      //! Combine compatible Transform objects directly underneath this Group.
      NVSG_API virtual void handleGroup( nvsg::Group * p );

      //! Combine compatible Transform objects directly underneath this Transform.
      /** A Transform is handled as a Group. */
      NVSG_API virtual void handleTransform( nvsg::Transform * p );

    private:
      void combineTransforms( nvsg::Group *p );
  };

}
