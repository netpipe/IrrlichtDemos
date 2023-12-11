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

#include <set>
#include "nvsgcommon.h"
#include "nvsg/nvsg.h"
#include "nvtraverser/OptimizeTraverser.h"

namespace nvtraverser
{  

  //! Traverser that can unifies vertices.
  /** Currently, this VertexUnifyTraverser supports Lines, LineStrips, Points, Quads, QuadStrips, Triangles, TriFans, and
    * TriStrips. */
  class VertexUnifyTraverser : public OptimizeTraverser
  {
    public:
      //! Constructor
      NVSG_API VertexUnifyTraverser(void);

      //! Set the epsilon to use to distinguish vertex values.
      /** This epsilon multiplied by the size of the scene gives the tolerance for detecting equal vertices.  */
      NVSG_API void  setEpsilon( float epsilon                 //!<  Epsilon to use
                               );

    protected:
      //! Protected destructor to prevent instantiation of a VertexUnifyTraverser on stack.
      NVSG_API virtual ~VertexUnifyTraverser(void);

      //! Initiate traversal of the scene with a view state.
      /** Calculates the tolerance for the scene and calls the Traverser::doApply().  */
      NVSG_API virtual void  doApply( nvsg::NodeHandle * root );

      //! Unify all VertexAttributeSets.
      void  handleVertexAttributeSet( nvsg::VertexAttributeSet * p );

      //! Explicitly nop on Animated and Skinned Primitives.
      void  handleAnimatedQuads( nvsg::AnimatedQuads * p );
      void  handleAnimatedTriangles( nvsg::AnimatedTriangles * triangles );
      void  handleSkinnedTriangles( nvsg::SkinnedTriangles * triangles );

    private:
      float                                         m_epsilon;
      float                                         m_tolerance;
      std::set<nvsg::VertexAttributeSetHandle *>    m_multiOwnedHandledVAS;
      std::vector<nvsg::VertexAttributeSetHandle *> m_removedVAS;
  };

  inline  void  VertexUnifyTraverser::setEpsilon( float epsilon )
  {
    m_epsilon = fabsf( epsilon );
  }
}
