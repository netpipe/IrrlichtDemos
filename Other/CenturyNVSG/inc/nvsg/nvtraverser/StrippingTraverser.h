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
#include <set>
#include "nvsgcommon.h"
#include "nvsg/nvsg.h"
#include "nvsg/Face.h"
#include "nvsg/IndependentPrimitiveSet.h"
#include "nvtraverser/Traverser.h"

namespace nvtraverser
{  

  //! Traverser to convert Triangles to TriStrips, and Quads to QuadStrips
  /** It is strongly recommended to use a VertexUnifyTraverser before using this StrippingTraverser, because it 
    * works only on indices. */
  class StrippingTraverser : public ExclusiveTraverser
  {
    public:
      //! Constructor
      NVSG_API StrippingTraverser(void);

      //! Get the minimal lengths for TriStrips and QuadStrips.
      /** A strip with less than the minimal length is reconverted to triangles or quads. */
      void  getMinimalLengths( size_t &triStripLength, size_t &quadStripLength ) const;

      //! Set the minimal lengths for TriStrips and QuadStrips.
      /** A strip with less than the minimal length is reconverted to triangles or quads. */
      void  setMinimalLengths( size_t triStripLength, size_t quadStripLength );

    protected:
      //! Protected destructor to prevent instantiation of a StrippingTraverser.
      NVSG_API virtual ~StrippingTraverser(void);

      //! Convert each Quads to a QuadStrips and a Quads, and each Triangles to a TriStrips and a Triangles.
      NVSG_API virtual void  handleGeoNode( nvsg::GeoNode *p );

      //! Hold the Quads to strip in handleGeoNode.
      NVSG_API virtual void handleQuads( nvsg::Quads *p );

      //! Hold the Triangles to strip in handleGeoNode.
      NVSG_API virtual void handleTriangles( nvsg::Triangles *p );

    private:
      void    addQuadStrip( const std::list<unsigned int> &vertexList, std::vector<nvsg::IndexSet> &strips
                          , nvsg::IndexSet &quads );
      void    addTriStrip( const std::list<unsigned int> &vertexList, std::vector<nvsg::IndexSet> &strips
                          , nvsg::IndexSet &triangles );
      void    connectFaces( const unsigned int *indices, size_t fi, size_t ps, size_t i0, size_t i1
                          , const std::vector<std::list<size_t> > &verticesToFaceLists, std::vector<size_t> &faceConnections );
      void    copyVertexAttributeData( nvsg::VertexAttributeSetHandle *from
                                     , nvsg::VertexAttributeSetHandle *to
                                     , std::vector<nvsg::IndexSet> &strips );
      void    copyVertexAttributeData( nvsg::VertexAttributeSetHandle *from
                                     , nvsg::VertexAttributeSetHandle *to
                                     , nvsg::IndexSet &indices );
      void    cutConnections( const std::list<size_t> &list, size_t vpf, std::vector<std::set<size_t> > &faceSets
                            , std::vector<size_t> &faceConnections, std::vector<size_t> &faceConnectionCounts );
      void    determineList( const nvsg::Face3 *faces, size_t fi, size_t le, std::list<size_t> &faceList
                           , const std::vector<size_t> &faceConnections, std::list<unsigned int> &vertexList );
      void    determineList( const nvsg::Face4 *faces, size_t fi, size_t le, std::list<size_t> &faceList
                           , const std::vector<size_t> &faceConnections, std::list<unsigned int> &vertexList );
      size_t  getNextFaceIndex( const unsigned int *indices, size_t primitiveSize, std::vector<std::set<size_t> > &faceSets
                              , nvsg::IndexSet &faces );
      void    prepareStripping( const nvsg::IndependentPrimitiveSet *p, size_t primitiveSize
                              , std::vector<size_t> &faceConnections, std::vector<size_t> &faceConnectionCounts
                              , std::vector<std::set<size_t> > &faceSets );

      std::pair<nvsg::QuadsHandle*,nvsg::QuadStripsHandle*> createQuadStripsFromQuads( nvsg::QuadsHandle *p );
      std::pair<nvsg::TrianglesHandle*,nvsg::TriStripsHandle*>    createTriStripsFromTriangles( nvsg::TrianglesHandle *p );

    private:
      size_t  m_minimalQuadStrip;
      size_t  m_minimalTriStrip;

      std::vector<nvsg::QuadsHandle *>    m_quads;
      std::vector<nvsg::TrianglesHandle*> m_triangles;
      std::map<nvsg::QuadsHandle*,std::pair<nvsg::QuadsHandle*,nvsg::QuadStripsHandle*> >         m_quadsToQuadStrips;
      std::map<nvsg::TrianglesHandle*,std::pair<nvsg::TrianglesHandle*,nvsg::TriStripsHandle*> >  m_trianglesToTriStrips;
  };

  inline  void StrippingTraverser::getMinimalLengths( size_t &triStripLength, size_t &quadStripLength ) const
  {
    triStripLength  = m_minimalTriStrip;
    quadStripLength = m_minimalQuadStrip;
  }

  inline  void  StrippingTraverser::setMinimalLengths( size_t triStripLength, size_t quadStripLength )
  {
    m_minimalTriStrip   = triStripLength;
    m_minimalQuadStrip  = quadStripLength;
  }
}
