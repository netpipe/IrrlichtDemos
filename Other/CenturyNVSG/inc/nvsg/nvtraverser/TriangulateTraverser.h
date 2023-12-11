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
#include  "nvtraverser/Traverser.h"

namespace nvtraverser
{

//! \c Traverser that converts all flavors of quads to triangles.
/** Converts \c AnimatedQuads to \c AnimatedTriangles, \c Quads to \c Triangles, and \c QuadStrips to \c TriangleStrips. */
  class TriangulateTraverser : public ExclusiveTraverser
{
  public:
    //! Constructor
    NVSG_API TriangulateTraverser(void);

  protected:
    //! Protected destructor to prevent instantiation of a StrippingTraverser.
    NVSG_API virtual ~TriangulateTraverser(void);

    //! Handle an AnimatedQuads object.
    /** Replaces an AnimatedQuads object by an AnimatedTriangles object. */
    NVSG_API virtual void  handleAnimatedQuads( 
      nvsg::AnimatedQuads *p      //!<  AnimatedQuads to handle
    );

    //! Handle a GeoNode object.
    /** The GeoNode is the primary object for quads-to-triangles conversion.  */
    NVSG_API virtual void  handleGeoNode( 
      nvsg::GeoNode *p            //!<  GeoNode to handle
    );

    //! Handle a Quads object.
    /** Replaces a Quads object by a Triangles object. */
    NVSG_API virtual void  handleQuads(
      nvsg::Quads *p              //!<  Quads to handle
    );

    //! Handle a QuadStrips object.
    /** Replaces a QuadStrips object by a TrianglesStrips object.  */
    NVSG_API virtual void  handleQuadStrips(
      nvsg::QuadStrips *p         //!<  QuadStrips to handle
    );

  private:
    std::vector<std::pair<nvsg::PrimitiveSetHandle*,nvsg::PrimitiveSetHandle*> >  m_primitiveSetReplaces;
};

}
