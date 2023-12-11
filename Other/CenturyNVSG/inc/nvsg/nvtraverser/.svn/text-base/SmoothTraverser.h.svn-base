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
#include "nvtraverser/Traverser.h"

namespace nvsg
{
  class IndependentPrimitiveSet;
}

namespace nvtraverser
{
  class DestrippingTraverser;

  //! Traverser that can smooth some drawables.
  /** Currently, this SmoothTraverser supports Triangles only. If these Triangles are non-trivially indexed, they are
    * changed to be trivially indexed.  */
  class SmoothTraverser : public ExclusiveTraverser
  {
    public:
      //! Constructor
      NVSG_API SmoothTraverser(void);

      //! Set the crease angle for smoothing.
      NVSG_API void  setCreaseAngle( float creaseAngle );

    protected:
      //! Protected destructor to prevent instantiation of a SmoothTraverser.
      NVSG_API virtual ~SmoothTraverser(void);

      //! doApply override
      NVSG_API virtual void doApply( nvsg::NodeHandle * root );

      //! Handle a GeoNode object.
      /** The GeoNode is the primary object to smooth.  */
      NVSG_API virtual void  handleGeoNode( nvsg::GeoNode *p              //!<  GeoNode to handle
                                          );

      //! Handle a Quads object.
      NVSG_API virtual void  handleQuads( nvsg::Quads *p                  //!<  Quads to handle
                                        );

      //! Handle a Triangles object.
      NVSG_API virtual void  handleTriangles( nvsg::Triangles *p          //!<  Triangles to handle
                                            );

#if !defined(NDEBUG)
      //! Handle a QuadStrips object.
      /** Just assert here; shouldn't happen at all! */
      NVSG_API virtual void  handleQuadStrips( nvsg::QuadStrips *p        //!<  QuadStrips to handle
                                             );

      //! Handle a TriFans object.
      /** Just assert here; shouldn't happen at all! */
      NVSG_API virtual void  handleTriFans( nvsg::TriFans *p              //!<  TriFans to handle
                                          );

      //! Handle a TriStrips object.
      /** Just assert here; shouldn't happen at all! */
      NVSG_API virtual void  handleTriStrips( nvsg::TriStrips *p          //!<  TriStrips to handle
                                            );
#endif

    private:
      void  handleIndependentPrimitiveSet( nvsg::IndependentPrimitiveSet *p, size_t primitiveSize );

    private:
      float                                     m_creaseAngle;
      DestrippingTraverser                    * m_destrippingTraverser;
      std::vector<const nvsg::PrimitiveSet*>    m_primitiveSets;
  };
}
