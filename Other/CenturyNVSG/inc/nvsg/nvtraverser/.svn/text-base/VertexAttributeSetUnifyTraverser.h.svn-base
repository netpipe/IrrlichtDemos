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

#include "nvsgcommon.h"
#include "nvtraverser/OptimizeTraverser.h"

namespace nvtraverser
{
  //! Optimizing Traverser that unifies VertexAttributeSets.
  /** Maps identical VertexAttributeSets to the same one. */
  class VertexAttributeSetUnifyTraverser : public OptimizeTraverser
  {
    public:
      //! Constructor
      NVSG_API VertexAttributeSetUnifyTraverser( void );

    protected:
      //! Protected destructor to prevent instantiation of a PrimitiveSetUnifyTraverser on stack.
      NVSG_API virtual ~VertexAttributeSetUnifyTraverser( void );

      //! clear temporary storage after traversing
      NVSG_API virtual void doApply( nvsg::NodeHandle * root );

      NVSG_API virtual void traversePrimitiveSet( nvsg::PrimitiveSet * pset );

    private:
      std::vector<nvsg::VertexAttributeSetHandle *> m_vertexAttributeSets;
  };

}
