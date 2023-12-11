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
  //! Optimizing Traverser that unifies GeoNodes.
  /** Maps identical GeoNodes to the same one. */
  class GeoNodeUnifyTraverser : public OptimizeTraverser
  {
    public:
      //! Constructor
      NVSG_API GeoNodeUnifyTraverser( void );

    protected:
      //! Protected destructor to prevent instantiation of a GeoNodeCombineTraverser on stack.
      NVSG_API virtual ~GeoNodeUnifyTraverser( void );

      //! clear temporary storage after traversing
      NVSG_API virtual void doApply( nvsg::NodeHandle * root );

      NVSG_API virtual void handleAnimatedTransform( nvsg::AnimatedTransform *trafo );
      NVSG_API virtual void handleBillboard( nvsg::Billboard * billboard );
      NVSG_API virtual void handleGroup( nvsg::Group * group );
      NVSG_API virtual void handleLOD( nvsg::LOD *lod );
      NVSG_API virtual void handleSwitch( nvsg::Switch * swtch );
      NVSG_API virtual void handleTransform( nvsg::Transform * trafo );

    private:
      void unifyGeoNodes( nvsg::Group *p );

    private:
      std::vector<nvsg::GeoNodeHandle *> m_geoNodes;
  };

}
