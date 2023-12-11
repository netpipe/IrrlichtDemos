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
#pragma warning( disable : 4503 )

#include <list>
#include "nvsgcommon.h"
#include "nvsg/nvsg.h"
#include "nvsg/Face.h"
#include "nvsg/IndependentPrimitiveSet.h"
#include "nvsg/MeshedPrimitiveSet.h"
#include "nvsg/StrippedPrimitiveSet.h"
#include "nvtraverser/OptimizeTraverser.h"

namespace nvtraverser
{
  //! Optimizing Traverser that combines PrimitiveSets under one GeoNode.
  /** All PrimitiveSets of the same type that are bound to the same StateSet are combined to one Drawable. */
  class PrimitiveSetCombineTraverser : public OptimizeTraverser
  {
    public:
      //! Constructor
      NVSG_API PrimitiveSetCombineTraverser( void );

      //! Get the 'ignore shares' flag.
      /** If the 'ignore shares' flag is set, shared PrimitiveSets are also combined. This may result in a larger memory
        * footprint.
        * \return true if the shares will be ignored, otherwise false */
      NVSG_API bool getIgnoreShares( void ) const;

      //! Set the 'ignore shares' flags.
      /** If the 'ignore shares' flag is set, shared PrimitiveSets are also combined. This may result in a larger memory
        * footprint. The default is to not ignore shares. */
      NVSG_API void setIgnoreShares( bool ignore   //!<  set true to ignore shares on combining
                                   );

      //! Get the maximal number of vertices for combined PrimitiveSet objects.
      /** If two combinable PrimitiveSet objects together would have more than this maximal number
        * of vertices, they are not combined.
        * \return The maximal number of vertices for combined PrimitiveSet objects. */
      NVSG_API size_t getMaxNumberOfVertices() const;

      //! Get the maximal number of vertices for combined PrimitiveSet objects.
      /** If two combinable PrimitiveSet objects together would have more than this maximal number
        * of vertices, they are not combined. */
      NVSG_API void   setMaxNumberOfVertices( size_t mnov );

    protected:
      //! Protected destructor to prevent instantiation of a PrimitiveSetCombineTraverser on stack.
      NVSG_API virtual ~PrimitiveSetCombineTraverser( void );

      //! Routine to handle a \link nvsg::GeoNode GeoNode \endlink while traversing the scene graph.
      /** After traversing, drawables of the same type bound to the same StateSet are combined. */
      NVSG_API virtual void handleGeoNode( nvsg::GeoNode * p );

      //! Routine to handle a \link nvsg::Lines Lines \endlink while traversing the scene graph.
      /** Just gathers lines for combining (done in handleGeoNode). */
      NVSG_API virtual void handleLines( nvsg::Lines * p );

      //! Routine to handle a \link nvsg::LineStrips LineStrips \endlink while traversing the scene graph.
      /** Just gathers line strips for combining (done in handleGeoNode). */
      NVSG_API virtual void handleLineStrips( nvsg::LineStrips * p );

      //! Routine to handle a \link nvsg::Points Points \endlink while traversing the scene graph.
      /** Just gathers points for combining (done in handleGeoNode). */
      NVSG_API virtual void handlePoints( nvsg::Points * p );

      //! Routine to handle a QuadMeshes while traversing the scene graph.
      /** Just gathers quad strips  for combining (done in handleGeoNode). */
      NVSG_API virtual void handleQuadMeshes( nvsg::QuadMeshes * p );

      //! Routine to handle a \link nvsg::Quads Quads \endlink while traversing the scene graph.
      /** Just gathers quads for combining (done in handleGeoNode). */
      NVSG_API virtual void handleQuads( nvsg::Quads * p );

      //! Routine to handle a \link nvsg::QuadStrips QuadStrips \endlink while traversing the scene graph.
      /** Just gathers quad strips  for combining (done in handleGeoNode). */
      NVSG_API virtual void handleQuadStrips( nvsg::QuadStrips * p );

      //! Routine to handle a \link nvsg::Triangles Triangles \endlink while traversing the scene graph.
      /** Just gathers triangles for combining (done in handleGeoNode). */
      NVSG_API virtual void handleTriangles( nvsg::Triangles * p );

      //! Routine to handle a \link nvsg::TriFans TriFans \endlink while traversing the scene graph.
      /** Just gathers triangle fans for combining (done in handleGeoNode). */
      NVSG_API virtual void handleTriFans( nvsg::TriFans * p );

      //! Routine to handle a \link nvsg::TriStrips TriStrips \endlink while traversing the scene graph.
      /** Just gathers triangle strips for combining (done in handleGeoNode). */
      NVSG_API virtual void handleTriStrips( nvsg::TriStrips * p );

      //! Routine to handle a \link nvsg::StateSet StateSet \endlink while traversing the scene graph.
      /** Hold the active StateSet for the next PrimitiveSet for combining (done in handleGeoNode). */
      NVSG_API virtual void handleStateSet( nvsg::StateSet * p );

    private:
      void combine( nvsg::PrimitiveSet *p0, const nvsg::PrimitiveSet *p1 );
      void combine( nvsg::IndependentPrimitiveSetHandle *p0, nvsg::IndependentPrimitiveSetHandle *p1 );
      void combine( nvsg::MeshedPrimitiveSetHandle *p0, nvsg::MeshedPrimitiveSetHandle *p1 );
      void combine( nvsg::StrippedPrimitiveSetHandle *p0, nvsg::StrippedPrimitiveSetHandle *p1 );
      template<typename TH>
        void combineDrawables( nvsg::GeoNode *p, std::map<nvsg::StateSetHandle*,std::vector<std::vector<TH*> > > &mvvT );
      void reduceVertexAttributeSet( nvsg::IndependentPrimitiveSetHandle * p );
      void reduceVertexAttributeSet( nvsg::MeshedPrimitiveSetHandle * p );
      void reduceVertexAttributeSet( nvsg::StrippedPrimitiveSetHandle * p );
      nvsg::VertexAttributeSetHandle * reduceVertexAttributeSet( const nvsg::VertexAttributeSet *p, std::map<unsigned int, unsigned int> &indexMap );
      template<typename TH>
        void storeDrawable( TH *p, std::map<nvsg::StateSetHandle *, std::vector<std::vector<TH*> > > &mvvT );
      nvsg::IndependentPrimitiveSetHandle * simpleCombine( nvsg::IndependentPrimitiveSetHandle * p0
                                                         , nvsg::IndependentPrimitiveSetHandle * p1 );
      nvsg::MeshedPrimitiveSetHandle * simpleCombine( nvsg::MeshedPrimitiveSetHandle * p0
                                                    , nvsg::MeshedPrimitiveSetHandle * p1 );
      nvsg::StrippedPrimitiveSetHandle * simpleCombine( nvsg::StrippedPrimitiveSetHandle * p0
                                                      , nvsg::StrippedPrimitiveSetHandle * p1 );

    private:
      bool                                                                                      m_ignoreShares;
      size_t                                                                                    m_maxNumberOfVertices;
      nvsg::StateSetHandle                                                                    * m_currentStateSet;
      //  For each primitive type, hold a map from the current StateSet to a vector of vector of primitives.
      //  The Primitives in the second level vector are compatible to each of them.
      std::map<nvsg::StateSetHandle *, std::vector<std::vector<nvsg::LinesHandle *> > >         m_lines;
      std::map<nvsg::StateSetHandle *, std::vector<std::vector<nvsg::LineStripsHandle *> > >    m_lineStrips;
      std::map<nvsg::StateSetHandle *, std::vector<std::vector<nvsg::PointsHandle *> > >        m_points;
      std::map<nvsg::StateSetHandle *, std::vector<std::vector<nvsg::QuadMeshesHandle *> > >    m_quadMeshes;
      std::map<nvsg::StateSetHandle *, std::vector<std::vector<nvsg::QuadsHandle *> > >         m_quads;
      std::map<nvsg::StateSetHandle *, std::vector<std::vector<nvsg::QuadStripsHandle *> > >    m_quadStrips;
      std::map<nvsg::StateSetHandle *, std::vector<std::vector<nvsg::TrianglesHandle *> > >     m_triangles;
      std::map<nvsg::StateSetHandle *, std::vector<std::vector<nvsg::TriFansHandle *> > >       m_triFans;
      std::map<nvsg::StateSetHandle *, std::vector<std::vector<nvsg::TriStripsHandle *> > >     m_triStrips;
  };


  inline bool PrimitiveSetCombineTraverser::getIgnoreShares( void ) const
  {
    return( m_ignoreShares );
  }

  inline void PrimitiveSetCombineTraverser::setIgnoreShares( bool ignore )
  {
    m_ignoreShares = ignore;
  }

  inline size_t PrimitiveSetCombineTraverser::getMaxNumberOfVertices() const
  {
    return( m_maxNumberOfVertices );
  }

  inline void PrimitiveSetCombineTraverser::setMaxNumberOfVertices( size_t mnov )
  {
    m_maxNumberOfVertices = mnov;
  }
}
