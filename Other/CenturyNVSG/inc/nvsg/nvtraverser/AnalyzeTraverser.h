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
#include "nvsg/IndependentPrimitiveSet.h"
#include "nvsg/StrippedPrimitiveSet.h"
#include "nvtraverser/Traverser.h"

#include <map>
#include <set>

namespace nvtraverser
{
  /*! \brief Base class of the analyzing results determined with the AnalyzeTraverser.
   *  \par Namespace: nvsg
   *  \remarks The AnalyzeResult holds the \a objectCode of the object that this result refers to,
   *  and the number of objects that have been encountered in the scene graph.
   *  \sa AnalyzeTraverser, CombinableResult, DenormalizedNormalsResult, EmptyResult,
   *  EquivalenceResult, IdentityResult, MissingResult, NullNormalsResult, SingleChildResult,
   *  ShortStrippedResult */
  class AnalyzeResult
  {
    public:
      /*! \brief Virtual Destructor */
      virtual ~AnalyzeResult() {};

    public:
      unsigned int  objectCode;   //!< The code of objects, this result refers to.
      size_t        objectCount;  //!< The number of objects of this type.
  };

  /*! \brief AnalyzeResult indicating that a number of Objects can be combined.
   *  \par Namespace: nvsg
   *  \remarks Reports if, and how many, combinable Objects are in the tree. It tells how many
   *  objects of the type \a objectCode are combinable, and by how many objects they could be
   *  replaced with. Currently, the following objects are checked for combinability: nvsg::GeoNode,
   *  nvsg::Lines, nvsg::LineStrips, nvsg::Points, nvsg::QuadMeshes, nvsg::Quads, nvsg::QuatStrips,
   *  nvsg::Triangles, nvsg::TriFans, and nvsg::TriStrips.
   *  \sa AnalyzeResult AnalyzeTraverser */
  class CombinableResult : public AnalyzeResult
  {
    public:
      virtual ~CombinableResult() {};

    public:
      size_t  combinableCount;  //!< The number of combinable objects of this type.
      size_t  reducedCount;     //!< The number of objects of this type, if combinable objects would be combined.
  };

  /*! \brief AnalyzeResult indicating a VertexAttributeSet with denormalized normals.
   *  \par Namespace: nvsg
   *  \remarks Reports if there is a VertexAttributeSet with denormalized normals.
   *  \sa AnalyzeResult, AnalyzeTraverser */
  class DenormalizedNormalsResult : public AnalyzeResult
  {
    public:
      virtual ~DenormalizedNormalsResult() {}

    public:
      size_t  denormalizedNormalsCount;   //!< The number of objects with denormalized normals.
      size_t  denormalizedNormalsNumber;  //!< The number of denormalized normals in those objects.
  };

  /*! \brief AnalyzeResult indicating an empty object.
   *  \par Namespace: nvsg
   *  \remarks All empty object in the tree (e.g. Groups without children, Triangles without
   *  indices,...) are reported with an EmptyResult.
   *  \sa AnalyzeResult, AnalyzeTraverser */
  class EmptyResult : public AnalyzeResult
  {
    public:
      /*! \brief Virtual Destructor */
      virtual ~EmptyResult() {}

    public:
      size_t  emptyCount;   //!< The number of empty objects of this type.
  };

  /*! \brief AnalyzeResult indicating equivalent objects.
   *  \par Namespace: nvsg
   *  \remarks All equivalent objects in the tree (e.g. StateAttributes with the same settings)
   *  are reported with an EquivalenceResult.
   *  \sa AnalyzeResult, AnalyzeTraverser */
  class EquivalenceResult : public AnalyzeResult
  {
    public:
      /*! \brief Virtual Destructor */
      virtual ~EquivalenceResult() {}

    public:
      size_t  equivalentCount;  //!< The number of equivalent objects of this type.
      size_t  reducedCount;     //!< The number of objects of this type, if equivalent objects would be combined.
  };

  /*! \brief AnalyzeResult indicating identity transformations.
   *  \par Namespace: nvsg
   *  \remarks All Transform objects in the tree that are in fact identity transforms are reported
   *  with an IdentityResult.
   *  \sa AnalyzeResult, AnalyzeTraverser */
  class IdentityResult : public AnalyzeResult
  {
    public:
      /*! \brief Virtual Destructor */
      virtual ~IdentityResult() {}

    public:
      size_t  identityCount;  //!< The number of identity Transforms.
  };

  /*! \brief AnalyzeResult indicating non-complete objects.
   *  \par Namespace: nvsg
   *  \remarks All Objects with missing information (e.g. an AnimatedTransform without an
   *  Animation) are reported with a MissingResult.
   *  \sa AnalyzeResult, AnalyzeTraverser */
  class MissingResult : public AnalyzeResult
  {
    public:
      /*! \brief Virtual Destructor */
      virtual ~MissingResult()  {}

    public:
      size_t  missingCount;   //!< The number of objects with missing information.
  };

  /*! \brief AnalyzeResult indicating a VertexAttributeSet with null normals.
   *  \par Namespace: nvsg
   *  \remarks Reports if there is a VertexAttributeSet with null normals.
   *  \sa AnalyzeResult, AnalyzeTraverser */
  class NullNormalsResult : public AnalyzeResult
  {
    public:
      virtual ~NullNormalsResult() {}

    public:
      size_t  nullNormalsCount;   //!< The number of objects with null normals.
      size_t  nullNormalsNumber;  //!< The number of null normals in those objects.
  };

  /*! \brief AnalyzeResult indicating a Group with a single child.
   *  \par Namespace: nvsg
   *  \remarks All Group (and Group-derived) objects with only one child are reported with a
   *  SingleChildResult.
   *  \sa AnalyzeResult, AnalyzeTraverser */
  class SingleChildResult : public AnalyzeResult
  {
    public:
      /*! \brief Virtual Destructor */
      virtual ~SingleChildResult()  {}

    public:
      size_t  singleChildCount;   //!< The number of objects with a single child.
  };

  /*! \brief AnalyzeResult indicating a StrippedPrimitiveSet with very short strips.
   *  \par Namespace: nvsg
   *  \remarks All QuadStrips with strips of length up to four, and all TriFans and TriStrips
   *  with fans or strips of length up to three are reported.
   *  \sa AnalyzeResult, AnalyzeTraverser, QuadStrips, TriFans, TriStrips */
  class ShortStrippedResult : public AnalyzeResult
  {
    public:
      virtual ~ShortStrippedResult() {}

    public:
      size_t shortStrippedCount;    //!< The number of objects with short strips.
      size_t shortStrippedNumber;   //!< The number of short strips in those objects.
  };

  /*! \brief Traverser that analyzes a tree and reports about potential deficiencies.
   *  \par Namespace: nvsg
   *  \remarks The AnalyzeTraverser is a scene graph analyzing tool. It can give you hints on
   *  potential problems in your scene graph.
   *  \par Example
   *  To get the results of the AnalyzeTraverser on a given Scene, do something like that:
   *  \code
   *    AnalyzeTraverser * analyzeTraverser = new AnalyzeTraverser;
   *    analyzeTraverser->addRef();
   *    std::vector<AnalyzeResult *> results;
   *    analyzeTraverser->doApply( pScene );
   *    size_t numberOfResults = analyzeTraverser->getAnalysis( results );
   *    for ( size_t i=0 ; i<numberOfResult ; i++ )
   *    {
   *      if ( dynamic_cast<EmptyResult *>( results[i] ) != NULL )
   *      {
   *        EmptyResult * emptyResult = dynamic_cast<EmptyResult *>( results[i] );
   *        // handle empty results
   *      }
   *      else if ( dynamic_cast<EquivalenceResult *>( results[i] ) != NULL )
   *      {
   *        EquivalenceResult * equivalenceResult = dynamic_cast<EquivalenceResult *>( results[i] );
   *        // handle equivalence results
   *      }
   *      else if ( ... )
   *      // handle other known results
   *      // ...
   *      else
   *      {
   *        // handle any unknown AnalyzeResult
   *      }
   *      // delete the result after interpretation
   *      delete results[i];
   *    }
   *    analyzeTraverser->removeRef();
   *  \endcode
   *  \sa AnalyzeResult, Traverser */
  class AnalyzeTraverser : public SharedTraverser
  {
    public:
      /*! \brief Default Constructor */
      NVSG_API AnalyzeTraverser( void );

      /*! \brief Get the analysis results of the latest traversal.
       *  \param results A reference to the vector pointers to an AnalyzeResult object to fill.
       *  \return The number of AnalyzeResult objects generated.
       *  \remarks On return, each element of the vector \a results holds a pointer to an
       *  AnalyzeResult. Those objects are owned by the requester, then. There can be multiple
       *  AnalyzeResult objects of the same type, but referring to different types of objects in
       *  the Scene.
       *  \sa AnalyzeResult */
      NVSG_API size_t getAnalysis( std::vector<AnalyzeResult*> &results ) const;

    protected:
      /*! \brief Protected destructor to prevent explicit creation.
       *  \remarks The destructor is never used explicitly, but only when the reference count of
       *  the AnalyzeTraverser is decremented to zero. */
      NVSG_API virtual ~AnalyzeTraverser( void );

      /*! \brief Override of the doApply method.
       *  \param root A pointer to the read-locked root node of the tree to analyze.
       *  \remarks The doApply method is the traversal entry point of a Traverser. The local data
       *  is cleared and Traverser::doApply() is called to start traversal.
       *  \sa nvsg::Scene, Traverser */
      NVSG_API virtual void doApply( nvsg::NodeHandle * root );

      /*! \brief Analyze an nvsg::AnimatedQuads.
       *  \param p A pointer to the read-locked nvsg::AnimatedQuads to analyze.
       *  \remarks An nvsg::AnimatedQuads is tested for emptiness (no indices), equivalence with
       *  any previously encountered AnimatedQuads, and for a missing nvsg::Animation.
       *  \sa nvsg::AnimatedQuads, EmptyResult, EquivalenceResult, MissingResult */
      NVSG_API virtual void handleAnimatedQuads( const nvsg::AnimatedQuads * p );

      /*! \brief Analyze an nvsg::AnimatedTransform.
       *  \param p A pointer to the read-locked nvsg::AnimatedTransform to analyze.
       *  \remarks An nvsg::AnimatedTransform is tested for emptiness (no children), equivalence
       *  with any previously encountered nvsg::AnimatedTransform, and for a missing nvsg::Animation.
       *  \sa nvsg::AnimatedTransform, EmptyResult, EquivalenceResult, MissingResult */
      NVSG_API virtual void handleAnimatedTransform( const nvsg::AnimatedTransform *p );

      /*! \brief Analyze an nvsg::AnimatedTriangles.
       *  \param p A pointer to the read-locked nvsg::AnimatedTriangles to analyze.
       *  \remarks An nvsg::AnimatedTriangles is tested for emptiness (no indices), equivalence
       *  with any previously encountered nvsg::AnimatedTriangles, and for a missing nvsg::Animation.
       *  \sa nvsg::AnimatedTriangles, EmptyResult, EquivalenceResult, MissingResult */
      NVSG_API virtual void handleAnimatedTriangles( const nvsg::AnimatedTriangles * p );

      /*! \brief Analyze a nvsg::Billboard.
       *  \param p A pointer to the read-locked nvsg::Billboard to analyze.
       *  \remarks A nvsg::Billboard is tested for emptiness (no children), and equivalence with
       *  any previously encountered nvsg::Billboard.
       *  \sa nvsg::Billboard, EmptyResult, EquivalenceResult */
      NVSG_API virtual void handleBillboard( const nvsg::Billboard * p );

      /*! \brief Analyze a nvsg::BlendAttribute.
       *  \param p A pointer to the read-locked nvsg::BlendAttribute to analyze.
       *  \remarks A nvsg::BlendAttribute is tested for equivalence with any previously
       *  encountered nvsg::BlendAttribute.
       *  \sa nvsg::BlendAttribute, EquivalenceResult */
      NVSG_API virtual void handleBlendAttribute( const nvsg::BlendAttribute * p );

      /*! \brief Analyze a nvsg::CgFx.
       *  \param p A pointer to the read-locked nvsg::CgFx to analyze.
       *  \remarks A nvsg::CgFx is tested for equivalence with any previously encountered nvsg::CgFx.
       *  \sa nvsg::CgFx, EquivalenceResult */
      NVSG_API virtual void handleCgFx( const nvsg::CgFx * p );

      /*! \brief Analyze a nvsg::DepthAttribute.
       *  \param p A pointer to the read-locked nvsg::DepthAttribute to analyze.
       *  \remarks A nvsg::DepthAttribute is tested for equivalence with any previously
       *  encountered nvsg::DepthAttribute.
       *  \sa nvsg::DepthAttribute, EquivalenceResult */
      NVSG_API virtual void handleDepthAttribute( const nvsg::DepthAttribute * p );

      /*! \brief Analyze a nvsg::DirectedLight.
       *  \param p A pointer to the read-locked nvsg::DirectedLight to analyze.
       *  \remarks A nvsg::DirectedLight is tested for equivalence with any previously encountered
       *  nvsg::DirectedLight.
       *  \sa nvsg::DirectedLight, EquivalenceResult */
      NVSG_API virtual void handleDirectedLight( const nvsg::DirectedLight * p );

      /*! \brief Analyze a nvsg::FaceAttribute.
       *  \param p A pointer to the read-locked nvsg::FaceAttribute to analyze.
       *  \remarks A nvsg::FaceAttribute is tested for equivalence with any previously encountered
       *  nvsg::FaceAttribute.
       *  \sa nvsg::FaceAttribute, EquivalenceResult */
      NVSG_API virtual void handleFaceAttribute( const nvsg::FaceAttribute * p );

      /*! \brief Analyze a nvsg::FramedAnimation of type nvmath::Trafo.
       *  \param p A pointer to the read-locked nvsg::FramedAnimation of type nvmath::Trafo to analyze.
       *  \remarks A nvsg::FramedAnimation of type nvmath::Trafo is tested for emptiness (no
       *  frames) and for equivalence with any previously encountered nvsg::FramedAnimation of
       *  type nvmath::Trafo.
       *  \sa nvsg::FramedAnimation, nvmath::Trafo, EmptyResult, EquivalenceResult */
      NVSG_API virtual void handleFramedTrafoAnimation( const nvsg::FramedAnimation<nvmath::Trafo> * p );

      /*! \brief Analyze a nvsg::FramedAnimation of type nvsg::VNVector.
       *  \param p A pointer to the read-locked nvsg::FramedAnimation of type nvsg::VNVector to analyze.
       *  \remarks A nvsg::FramedAnimation of type nvsg::VNVector is tested for emptiness (no
       *  frames) and for equivalence with any previously encountered nvsg::FramedAnimation of
       *  type nvsg::VNVector.
       *  \sa nvsg::FramedAnimation, nvsg::VNVector, EmptyResult, EquivalenceResult */
      NVSG_API virtual void handleFramedVNVectorAnimation( const nvsg::FramedAnimation<nvsg::VNVector> * p );

      /*! \brief Analyze a nvsg::GeoNode.
       *  \param p A pointer to the read-locked nvsg::GeoNode to analyze.
       *  \remarks A nvsg::GeoNode is tested for emptiness (no geometries) and for equivalence
       *  with any previously encountered nvsg::GeoNode.
       *  \sa nvsg::GeoNode, EmptyResult, EquivalenceResult */
      NVSG_API virtual void handleGeoNode( const nvsg::GeoNode * p );

      /*! \brief Analyze a nvsg::Group.
       *  \param p A pointer to the read-locked nvsg::Group to analyze.
       *  \remarks A nvsg::Group is tested for emptiness (no children), for equivalence with any
       *  previously encountered nvsg::Group, and for holding a single child only.
       *  \sa nvsg::Group, EmptyResult, EquivalenceResult, SingleChildResult */
      NVSG_API virtual void handleGroup( const nvsg::Group * p );

      /*! \brief Analyze a nvsg::LinearInterpolatedAnimation of type nvmath::Trafo.
       *  \param p A pointer to the read-locked nvsg::LinearInterpolatedAnimation of type
       *  nvmath::Trafo to analyze.
       *  \remarks A nvsg::LinearInterpolatedAnimation of type nvmath::Trafo is tested for
       *  emptiness (no keys) and for equivalence with any previously encountered
       *  nvsg::LinearInterpolatedAnimation of type nvmath::Trafo.
       *  \sa nvsg::LinearInterpolatedAnimation, nvmath::Trafo, EmptyResult, EquivalenceResult */
      NVSG_API virtual void handleLinearInterpolatedTrafoAnimation( const nvsg::LinearInterpolatedAnimation<nvmath::Trafo> * p );

      /*! \brief Analyze a nvsg::LinearInterpolatedAnimation of type nvsg::VNVector.
       *  \param p A pointer to the read-locked nvsg::LinearInterpolatedAnimation of type
       *  nvsg::VNVector to analyze.
       *  \remarks A nvsg::LinearInterpolatedAnimation of type nvsg::VNVector is tested for
       *  emptiness (no keys) and for equivalence with any previously encountered
       *  nvsg::LinearInterpolatedAnimation of type nvsg::VNVector.
       *  \sa nvsg::LinearInterpolatedAnimation, nvsg::VNVector, EmptyResult, EquivalenceResult */
      NVSG_API virtual void handleLinearInterpolatedVNVectorAnimation( const nvsg::LinearInterpolatedAnimation<nvsg::VNVector> * p );

      /*! \brief Analyze a nvsg::LineAttribute.
       *  \param p A pointer to the read-locked nvsg::LineAttribute to analyze.
       *  \remarks A nvsg::LineAttribute is tested for equivalence with any previously encountered
       *  nvsg::LineAttribute.
       *  \sa nvsg::LineAttribute, EquivalenceResult */
      NVSG_API virtual void handleLineAttribute( const nvsg::LineAttribute * p );

      /*! \brief Analyze a nvsg::Lines.
       *  \param p A pointer to the read-locked nvsg::Lines to analyze.
       *  \remarks A nvsg::Lines is tested for emptiness (no indices) and for equivalence with any
       *  previously encountered nvsg::Lines.
       *  \sa nvsg::Lines, EmptyResult, EquivalenceResult */
      NVSG_API virtual void handleLines( const nvsg::Lines * p );

      /*! \brief Analyze a nvsg::LineStrips.
       *  \param p A pointer to the read-locked nvsg::LineStrips to analyze.
       *  \remarks A nvsg::LineStrips is tested for emptiness (no strips) and for equivalence with
       *  any previously encountered nvsg::LineStrips.
       *  \sa nvsg::LineStrips, EmptyResult, EquivalenceResult */
      NVSG_API virtual void handleLineStrips( const nvsg::LineStrips * p );

      /*! \brief Analyze a nvsg::LOD.
       *  \param p A pointer to the read-locked nvsg::LOD to analyze.
       *  \remarks A nvsg::LOD is tested for emptiness (no children), for equivalence with any
       *  previously encountered nvsg::LOD, and for holding a single child only.
       *  \sa nvsg::LOD, EmptyResult, EquivalenceResult, SingleChildResult */
      NVSG_API virtual void handleLOD( const nvsg::LOD * p );

      /*! \brief Analyze a nvsg::Material.
       *  \param p A pointer to the read-locked nvsg::Material to analyze.
       *  \remarks A nvsg::Material is tested for equivalence with any previously encountered
       *  nvsg::Material.
       *  \sa nvsg::Material, EquivalenceResult */
      NVSG_API virtual void handleMaterial( const nvsg::Material * p );

      /*! \brief Analyze a nvsg::PointAttribute.
       *  \param p A pointer to the read-locked nvsg::PointAttribute to analyze.
       *  \remarks A nvsg::PointAttribute is tested for equivalence with any previously
       *  encountered nvsg::PointAttribute.
       *  \sa nvsg::PointAttribute, EquivalenceResult */
      NVSG_API virtual void handlePointAttribute( const nvsg::PointAttribute * p );

      /*! \brief Analyze a nvsg::PointLight.
       *  \param p A pointer to the read-locked nvsg::PointLight to analyze.
       *  \remarks A nvsg::PointLight is tested for equivalence with any previously encountered
       *  nvsg::PointLight.
       *  \sa nvsg::PointLight, EquivalenceResult */
      NVSG_API virtual void handlePointLight( const nvsg::PointLight * p );

      /*! \brief Analyze a nvsg::Points.
       *  \param p A pointer to the read-locked nvsg::Points to analyze.
       *  \remarks A nvsg::Points is tested for emptiness (no indices) and for equivalence with
       *  any previously encountered nvsg::Points.
       *  \sa nvsg::Points, EmptyResult, EquivalenceResult */
      NVSG_API virtual void handlePoints( const nvsg::Points * p );

      /*! \brief Analyze a nvsg::QuadMeshes.
       *  \param p A pointer to the read-locked nvsg::QuadMeshes to analyze.
       *  \remarks A nvsg::QuadMeshes is tested for emptiness (no meshes) and for equivalence with
       *  any previously encountered nvsg::QuadMeshes.
       *  \sa nvsg::QuadMeshes, EmptyResult, EquivalenceResult */
      NVSG_API virtual void handleQuadMeshes( const nvsg::QuadMeshes * p );

      /*! \brief Analyze a nvsg::Quads.
       *  \param p A pointer to the read-locked nvsg::Quads to analyze.
       *  \remarks A nvsg::Quads is tested for emptiness (no indices) and for equivalence with any
       *  previously encountered nvsg::Quads.
       *  \sa nvsg::Quads, EmptyResult, EquivalenceResult */
      NVSG_API virtual void handleQuads( const nvsg::Quads * p );

      /*! \brief Analyze a nvsg::QuadStrips.
       *  \param p A pointer to the read-locked nvsg::QuadStrips to analyze.
       *  \remarks A nvsg::QuadStrips is tested for emptiness (no strips), for equivalence with
       *  any previously encountered nvsg::Quads, and for holding strips of length up to only four.
       *  \sa nvsg::QuadStrips, EmptyResult, EquivalenceResult, ShortStrippedResult */
      NVSG_API virtual void handleQuadStrips( const nvsg::QuadStrips * p );

      /*! \brief Analyze a nvsg::Shape.
       *  \param p A pointer to the read-locked nvsg::Shape to analyze.
       *  \remarks A nvsg::Shape is tested for emptiness (no primitive sets) and for equivalence
       *  with any previously encountered nvsg::Shape.
       *  \sa nvsg::Shape, EmptyResult, EquivalenceResult */
      NVSG_API virtual void handleShape( const nvsg::Shape * p );

      /*! \brief Analyze a nvsg::SkinnedTriangles.
       *  \param p A pointer to the read-locked nvsg::SkinnedTriangles to analyze.
       *  \remarks A nvsg::SkinnedTriangles is tested for emptiness (no skins) and for equivalence
       *  with any previously encountered nvsg::SkinnedTriangles.
       *  \sa nvsg::SkinnedTriangles, EmptyResult, EquivalenceResult */
      NVSG_API virtual void handleSkinnedTriangles( const nvsg::SkinnedTriangles * p );

      /*! \brief Analyze a nvsg::SpotLight.
       *  \param p A pointer to the read-locked nvsg::SpotLight to analyze.
       *  \remarks A nvsg::SpotLight is tested for equivalence with any previously encountered
       *  nvsg::SpotLight.
       *  \sa nvsg::SpotLight, EquivalenceResult */
      NVSG_API virtual void handleSpotLight( const nvsg::SpotLight * p );

      /*! \brief Analyze a nvsg::StatePass.
       *  \param p A pointer to the read-locked nvsg::StatePass to analyze.
       *  \remarks A nvsg::StatePass is tested for emptiness (no attributes) and for equivalence
       *  with any previously encountered nvsg::StatePass.
       *  \sa nvsg::StatePass, EmptyResult, EquivalenceResult */
      NVSG_API virtual void handleStatePass( const nvsg::StatePass * p );

      /*! \brief Analyze a nvsg::StateSet.
       *  \param p A pointer to the read-locked nvsg::StateSet to analyze.
       *  \remarks A nvsg::StateSet is tested for emptiness (no variants) and for equivalence with
       *  any previously encountered nvsg::StateSet.
       *  \sa nvsg::StateSet, EmptyResult, EquivalenceResult */
      NVSG_API virtual void handleStateSet( const nvsg::StateSet * p );

      /*! \brief Analyze a nvsg::StateVariant.
       *  \param p A pointer to the read-locked nvsg::StateVariant to analyze.
       *  \remarks A nvsg::StateVariant is tested for emptiness (no passes) and for equivalence
       *  with any previously encountered nvsg::StateVariant.
       *  \sa nvsg::StateVariant, EmptyResult, EquivalenceResult */
      NVSG_API virtual void handleStateVariant( const nvsg::StateVariant * p );

      /*! \brief Analyze a nvsg::Switch.
       *  \param p A pointer to the read-locked nvsg::Switch to analyze.
       *  \remarks A nvsg::Switch is tested for emptiness (no children) and for equivalence with
       *  any previously encountered nvsg::Switch.
       *  \sa nvsg::Switch, EmptyResult, EquivalenceResult */
      NVSG_API virtual void handleSwitch( const nvsg::Switch * p );

      /*! \brief Analyze a nvsg::TextureAttribute.
       *  \param p A pointer to the read-locked nvsg::TextureAttribute to analyze.
       *  \remarks A nvsg::TextureAttribute is tested for emptiness (no items) and for equivalence
       *  with any previously encountered nvsg::TextureAttribute.
       *  \sa nvsg::TextureAttribute, EmptyResult, EquivalenceResult */
      NVSG_API virtual void handleTextureAttribute( const nvsg::TextureAttribute * p );

      /*! \brief Analyze a nvsg::TextureAttributeItem.
       *  \param p A pointer to the read-locked nvsg::TextureAttributeItem to analyze.
       *  \param tu The texture unit, this nvsg::TextureAttributeItem is bound to.
       *  \remarks A nvsg::TextureAttributeItem is tested for equivalence with any previously
       *  encountered nvsg::TextureAttributeItem.
       *  \sa nvsg::TextureAttributeItem, EmptyResult, EquivalenceResult */
      NVSG_API virtual void handleTextureAttributeItem( const nvsg::TextureAttributeItem * p
                                                      , size_t tu );

      /*! \brief Analyze a nvsg::Transform.
       *  \param p A pointer to the read-locked nvsg::Transform to analyze.
       *  \remarks A nvsg::Transform is tested for emptiness (no children), for equivalence with
       *  any previously encountered nvsg::Transform, and for being the identity transform.
       *  \sa nvsg::Transform, EmptyResult, EquivalenceResult, IdentityResult */
      NVSG_API virtual void handleTransform( const nvsg::Transform * p );

      /*! \brief Analyze a nvsg::Triangles.
       *  \param p A pointer to the read-locked nvsg::Triangles to analyze.
       *  \remarks A nvsg::Triangles is tested for emptiness (no indices) and for equivalence with
       *  any previously encountered nvsg::Triangles.
       *  \sa nvsg::Triangles, EmptyResult, EquivalenceResult */
      NVSG_API virtual void handleTriangles( const nvsg::Triangles * p );

      /*! \brief Analyze a nvsg::TriFans.
       *  \param p A pointer to the read-locked nvsg::TriFans to analyze.
       *  \remarks A nvsg::TriFans is tested for emptiness (no strips), for equivalence with any
       *  previously encountered nvsg::TriFans, and for holding fans of length up to only three.
       *  \sa nvsg::TriFans, EmptyResult, EquivalenceResult, ShortStrippedResult */
      NVSG_API virtual void handleTriFans( const nvsg::TriFans * p );

      /*! \brief Analyze a nvsg::TriStrips.
       *  \param p A pointer to the read-locked nvsg::TriStrips to analyze.
       *  \remarks A nvsg::TriStrips is tested for emptiness (no strips), for equivalence with any
       *  previously encountered nvsg::TriStrips, and for holding strips of length up to only three.
       *  \sa nvsg::TriStrips, EmptyResult, EquivalenceResult, ShortStrippedResult */
      NVSG_API virtual void handleTriStrips( const nvsg::TriStrips * p );

      /*! \brief Analyze a nvsg::VertexAttributeSet.
       *  \param p A pointer to the read-locked nvsg::VertexAttributeSet to analyze.
       *  \remarks A nvsg::VertexAttributeSet is tested for emptiness (no vertices), for
       *  equivalence with any previously encountered nvsg::VertexAttributeSet, and for holding
       *  denormalized normals.
       *  \sa nvsg::VertexAttributeSet, EmptyResult, EquivalenceResult, DenormalizedNormalsResult,
       *  NullNormalsResult */
      NVSG_API virtual void handleVertexAttributeSet( const nvsg::VertexAttributeSet * p );

      void postTraverseGroup( const nvsg::Group * grp );
      bool preTraverseGroup( const nvsg::Group * grp );

    private:
      template<typename T>
        void analyzeCombinable( const T *p
                              , std::map<const nvsg::StateSet*,std::vector<std::pair<const T*,size_t> > > &mvpT );
      void analyzeCombinable( const nvsg::GeoNode *p, std::vector<const nvsg::GeoNode *> &vgn );
      void analyzeEmpty( const nvsg::Object * p, size_t numberOfElements );
      void analyzeEquivalent( const nvsg::Object *p );
      void analyzeNormalsNormalized( const nvsg::VertexAttributeSet * p );
      void analyzeMissing( const nvsg::Object * p, const void * ptr );
      void analyzeShortStripped( const nvsg::StrippedPrimitiveSet * p, size_t minCount );
      void analyzeSingleChild( const nvsg::Group * p );
      template<typename T>
        void extractCombinableInfo( unsigned int oc
                                  , std::map<const nvsg::StateSet*,std::vector<std::pair<const T*,size_t> > > &mvpT );
      void extractCombinableInfo( std::vector<const nvsg::GeoNode *> &vgn );
      bool isToBeHandled( const nvsg::Object *p );

    private:
      class EquivalenceInfo
      {
        public :
          std::vector<const nvsg::Object *> uniqueObjects;
          std::set<const nvsg::Object *>    equivalentObjects;
      };

      std::map<unsigned int,size_t>                     m_countMap;
      std::stack<std::vector<const nvsg::GeoNode*> >    m_combinableGeoNodes;
      std::map<unsigned int,std::pair<size_t,size_t> >  m_combinableInfo;
      std::map<const nvsg::StateSet*,std::vector<std::pair<const nvsg::Lines*,size_t> > >       m_combinableLines;
      std::map<const nvsg::StateSet*,std::vector<std::pair<const nvsg::LineStrips*,size_t> > >  m_combinableLineStrips;
      std::map<const nvsg::StateSet*,std::vector<std::pair<const nvsg::Points*,size_t> > >      m_combinablePoints;
      std::map<const nvsg::StateSet*,std::vector<std::pair<const nvsg::QuadMeshes*,size_t> > >  m_combinableQuadMeshes;
      std::map<const nvsg::StateSet*,std::vector<std::pair<const nvsg::Quads*,size_t> > >       m_combinableQuads;
      std::map<const nvsg::StateSet*,std::vector<std::pair<const nvsg::QuadStrips*,size_t> > >  m_combinableQuadStrips;
      std::map<const nvsg::StateSet*,std::vector<std::pair<const nvsg::Triangles*,size_t> > >   m_combinableTriangles;
      std::map<const nvsg::StateSet*,std::vector<std::pair<const nvsg::TriFans*,size_t> > >     m_combinableTriFans;
      std::map<const nvsg::StateSet*,std::vector<std::pair<const nvsg::TriStrips*,size_t> > >   m_combinableTriStrips;
      const nvsg::StateSet *                            m_currentStateSet;
      std::pair<size_t,size_t>                          m_denormalizedNormals;
      std::map<unsigned int, size_t>                    m_emptyMap;
      std::map<unsigned int, EquivalenceInfo>           m_equivalenceMap;
      size_t                                            m_identityCount;
      std::map<unsigned int, size_t>                    m_missingMap;
      std::pair<size_t,size_t>                          m_nullNormals;
      std::set<const nvsg::Object *>                    m_sharedObjects;
      std::map<unsigned int, size_t>                    m_singleChildMap;
      std::map<unsigned int, std::pair<size_t,size_t> > m_shortStripped;
  };
}
