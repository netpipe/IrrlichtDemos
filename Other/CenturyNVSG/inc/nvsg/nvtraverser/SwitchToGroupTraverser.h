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
  //! Traverser that replaces Switch objects by Group objects holding the active children only.
  class SwitchToGroupTraverser : public OptimizeTraverser
  {
    public:
      //! Constructor
      NVSG_API SwitchToGroupTraverser( void );

    protected:
      //! Protected destructor to prevent instantiation of an SwitchToGroupTraverser on stack.
      NVSG_API virtual ~SwitchToGroupTraverser( void );

      //! If the root node is a Switch, it is replaced by a Group.
      NVSG_API virtual void doApply( nvsg::NodeHandle * root );

      //! Replace all Switch objects beneath that Transform to a Group objects.
      /** An AnimatedTransform is handled as a Transform.h */
      NVSG_API virtual void handleAnimatedTransform( nvsg::AnimatedTransform *p );

      //! Replace all Switch objects beneath that Billboard to Group objects.
      NVSG_API virtual void handleBillboard( nvsg::Billboard *p );

      //! Replace all Switch objects beneath that Group to Group objects.
      NVSG_API virtual void handleGroup( nvsg::Group *p );

      //! Replace all Switch objects beneath that LOD to Group objects.
      NVSG_API virtual void handleLOD( nvsg::LOD *p );

#if defined( _DEBUG )
      //! No handleSwitch here, because they are all replaced before
      /** This holds just an assertion. */
      NVSG_API virtual void handleSwitch( nvsg::Switch *p );
#endif
      
      //! Replace all Switch objects beneath that Transform to Group objects.
      NVSG_API virtual void handleTransform( nvsg::Transform *p );

    private:
      nvsg::GroupHandle * createGroupFromSwitch( nvsg::SwitchHandle * pSwitch );
      void                replaceSwitches( nvsg::Group *p );
  };
}
