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
  //! Optimzing Traverser that unifies PrimitiveSets.
  /** Maps identical PrimitiveSets to the same one. */
  class PrimitiveSetUnifyTraverser : public OptimizeTraverser
  {
    public:
      //! Constructor
      NVSG_API PrimitiveSetUnifyTraverser( void );

    protected:
      //! Protected destructor to prevent instantiation of a PrimitiveSetUnifyTraverser on stack.
      NVSG_API virtual ~PrimitiveSetUnifyTraverser( void );

      //! clear temporary storage after traversing
      NVSG_API virtual void doApply( nvsg::NodeHandle * root );

      //! Store the current GeoNode to use in the Drawable handlers.
      NVSG_API virtual void handleGeoNode( nvsg::GeoNode * p );

      //! Test if a Lines equivalent to this has been encountered and replace it by that. */
      NVSG_API virtual void handleLines( nvsg::Lines *p );

      //! Test if a LineStrips equivalent to this has been encountered and replace it by that. */
      NVSG_API virtual void handleLineStrips( nvsg::LineStrips * p );

      //! Test if a Points equivalent to this has been encountered and replace it by that. */
      NVSG_API virtual void handlePoints( nvsg::Points *p );

      //! Test if a QuadMeshes equivalent to this has been encountered and replace it by that. */
      NVSG_API virtual void handleQuadMeshes( nvsg::QuadMeshes *p );

      //! Test if a Quads equivalent to this has been encountered and replace it by that. */
      NVSG_API virtual void handleQuads( nvsg::Quads *p );

      //! Test if a QuadStrips equivalent to this has been encountered and replace it by that. */
      NVSG_API virtual void handleQuadStrips( nvsg::QuadStrips *p );

      //! Test if a Triangles equivalent to this has been encountered and replace it by that. */
      NVSG_API virtual void handleTriangles( nvsg::Triangles *p );

      //! Test if a TriFans equivalent to this has been encountered and replace it by that. */
      NVSG_API virtual void handleTriFans( nvsg::TriFans *p );

      //! Test if a TriStrips equivalent to this has been encountered and replace it by that. */
      NVSG_API virtual void handleTriStrips( nvsg::TriStrips *p );

    private:
      template<typename T, typename TH>
        void testAndExchange( std::vector<TH *> &v, T *pT );

    private:
      nvsg::GeoNode                         * m_currentGeoNode;
      std::vector<nvsg::LinesHandle *>        m_lines;
      std::vector<nvsg::LineStripsHandle *>   m_lineStrips;
      std::vector<nvsg::PointsHandle *>       m_points;
      std::vector<nvsg::QuadMeshesHandle *>   m_quadMeshes;
      std::vector<nvsg::QuadsHandle *>        m_quads;
      std::vector<nvsg::QuadStripsHandle *>   m_quadStrips;
      std::vector<nvsg::TrianglesHandle *>    m_triangles;
      std::vector<nvsg::TriFansHandle *>      m_triFans;
      std::vector<nvsg::TriStripsHandle *>    m_triStrips;
  };

}
