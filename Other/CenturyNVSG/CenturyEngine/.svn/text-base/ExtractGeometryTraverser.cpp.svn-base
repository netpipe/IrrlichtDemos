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

#include "nvsg/AnimatedTransform.h"
#include "nvsg/Billboard.h"
#include "nvsg/Camera.h"
#include "nvsg/Transform.h"
#include "nvsg/ViewState.h"
#include "nvsg/LOD.h"
#include "nvsg/PrimitiveSet.h"
#include "nvsg/Triangles.h"
#include "nvsg/Quads.h"
#include "nvsg/VertexAttributeSet.h"
#include "nvutil/Handle.h"
#include "ExtractGeometryTraverser.h"
#include <vector>

#include "nvutil/DbgNew.h"    // memory leak detection

using namespace std;
using namespace nvsg;
using namespace nvtraverser;
using namespace nvmath;
using namespace nvutil;

ExtractGeometryTraverser::ExtractGeometryTraverser()
{
  NVSG_TRACE();
  m_transformStack.setWorldToView( cIdentity44f, cIdentity44f );
}

ExtractGeometryTraverser::~ExtractGeometryTraverser(void)
{
  NVSG_TRACE() 
  NVSG_ASSERT( m_transformStack.getStackDepth() == 1 );
}

void ExtractGeometryTraverser::handleAnimatedTransform( 
    const AnimatedTransform *p )
{
  // not sure if this will be what we want..
  handleTransform( p );
}

void  ExtractGeometryTraverser::handleBillboard( const Billboard *p )
{
  NVSG_TRACE();

  // ignore billboards for the moment
}

void  ExtractGeometryTraverser::handleSwitch( const Switch *p )
{
  NVSG_TRACE();

  // ignore switches for the moment
}

void ExtractGeometryTraverser::handleLOD( const LOD *p )
{
  NVSG_TRACE();
  
  // Pushing the index of the active LOD level onto the lod stack
  // allows us to use it in the traverse (LOD) methods of the base class.

  if( p->getNumberOfRanges() )
  {
    // we only traverse the highest LOD 
    m_lodStack.push( 0 );

    SharedTraverser::handleLOD(p);

    m_lodStack.pop();
  }
  else
  {
    SharedTraverser::handleLOD(p);
  }
}

void  ExtractGeometryTraverser::handleTransform( const Transform *p )
{
  NVSG_TRACE();
  
  if( p->getAnnotation() == "TERRAIN:FALSE" )
    return;

  // multiply trafo on top of current matrices
  const Trafo & trafo = p->getTrafo();
  m_transformStack.pushModelToWorld( trafo.getMatrix(), trafo.getInverse() );

  //  call the (overloadable) preTraverse() between stack adjustment and traversal
  if ( preTraverseTransform( &trafo ) )
  {
    SharedTraverser::handleTransform( p );

    //  call the (overloadable) postTraverse() between stack adjustment and traversal
    postTraverseTransform( &trafo );
  }
  
  // pop off view matrices after proceeding
  m_transformStack.popModelToWorld();
}

bool ExtractGeometryTraverser::preTraverseTransform( const Trafo *p )
{
  return( true );
}

void  ExtractGeometryTraverser::postTraverseTransform( const Trafo *p )
{
}

//
// Here is where most of the work gets done
//
void
ExtractGeometryTraverser::traversePrimitiveSet( const PrimitiveSet * pset )
{
  NVSG_TRACE();

  VertexAttributeSetHandle * vash = pset->getVertexAttributeSet();

  const Quads * quads     = dynamic_cast< const Quads * > ( pset );
  const Triangles * tris  = dynamic_cast< const Triangles * > ( pset );

  if( !quads && !tris )
  {
    // ignore lines and points - and mesh and strips for now
    return;
  }

  //
  // Extract the verts and transform them by the current matrix
  //
  if ( vash )
  {
    const Mat44f & transform = m_transformStack.getModelToWorld();
    ReadableObject< VertexAttributeSet > vas( vash );

    size_t numVerts = vas->getNumberOfVertices();
    const Vec3f * verts = vas->getVertices();

    vector<Vec3f> transformedVerts( numVerts );

    for( unsigned int i = 0; i < numVerts; i ++ )
    {
      Vec4f v4f = Vec4f( verts[i], 1.0f ) * transform;

      transformedVerts[i] = Vec3f( v4f[0], v4f[1], v4f[2] );
    }

    vector<unsigned int> finalIndices;

    // now figure out what type of primitive set this is
    if( quads )
    {
      const unsigned int * indices = quads->getIndices();

      // need to triangulate the quads
      for( unsigned int i = 0; i < quads->getNumberOfIndices(); i += 4 )
      {
        // create 2 triangles for each quad
        finalIndices.push_back( indices[i+0] );
        finalIndices.push_back( indices[i+1] );
        finalIndices.push_back( indices[i+2] );

        finalIndices.push_back( indices[i+2] );
        finalIndices.push_back( indices[i+3] );
        finalIndices.push_back( indices[i+0] );
      }
    }
    else 
    {
      const unsigned int * indices = tris->getIndices();

      // just copy them into the list for now
      for( unsigned int i = 0; i < tris->getNumberOfIndices(); i ++ )
      {
        finalIndices.push_back( indices[i] );
      }
    }

    submitIndexedTriangleSet( finalIndices, transformedVerts );
  }
}
