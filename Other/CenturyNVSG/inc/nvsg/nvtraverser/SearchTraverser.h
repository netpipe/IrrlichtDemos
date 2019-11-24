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
  class MeshedPrimitiveSet;
  class Path;
  class StrippedPrimitiveSet;
  template<typename T> class Animation;
  template<typename T> class InterpolatedAnimation;
}

namespace nvtraverser
{
  //! Traverser for searching all objects of one class type.
  /** With a search traverser you can scan over a scene and get a vector of pointers to the objects you're searching.
    * It's possible to search for objects of a specific class or for objects that have a specific base class. */
  class SearchTraverser : public SharedTraverser
  {
    public:
      //! Constructor
      NVSG_API SearchTraverser(void);

      //! Iterator type definition
      typedef std::vector<const nvsg::Object*>::const_iterator iterator;

      //! Get a constant reference to the vector of paths to the search results.
      /** \returns constant reference to the vector of paths to the search results */
      NVSG_API const std::vector<const nvsg::Path*> & getPaths( void ) const;

      //! Get a constant reference to the vector of search results.
      /** \returns  constant reference to the vector of search results  */
      NVSG_API const std::vector<nvsg::ObjectHandle*> & getResults( void ) const;

      //! Set the class name to search for.
      /** The class name to look for needs to be fully qualified, i.e. "class nvsg::LightSource" instead of
        * "LightSource" only. */
      NVSG_API void setClassName( const std::string& name      //!<  class name to look for in a sceene
                                );

      //! Enable/Disable the search for base classes.
      /** If the search for base class is enabled, all classes that are equal to are are derived from the search class
        * are found.
        */
      NVSG_API void setBaseClassSearch( bool enable            //!<  true, to enable base class search, otherwise false
                                      );

    protected:
      //! Destructor
      NVSG_API virtual ~SearchTraverser(void);
      
      //! Set up the search and traverse the scene.
      NVSG_API virtual void  doApply( nvsg::NodeHandle * root );

      //! Routine to handle a \link nvsg::Billboard Billboard \endlink node while traversing the scene graph.
      NVSG_API virtual void handleBillboard(
        const nvsg::Billboard * p //!< Points to the currently visited Billboard object.
        );

      //! Routine to handle a \link nvsg::GeoNode GeoNode \endlink while traversing the scene graph.
      NVSG_API virtual void handleGeoNode(
        const nvsg::GeoNode * p //!< Points to the currently visited GeoNode object.
      );
                                          
      //! Routine to handle a VolumeNode while traversing the scene graph.
      NVSG_API void handleVolumeNode( const nvsg::VolumeNode * p );

      //! Routine to handle a \link nvsg::Group Group \endlink node while traversing the scene graph.
      NVSG_API virtual void handleGroup(
        const nvsg::Group * p //!< Points to the currently visited Group object.
      );
      
      //! Routine to handle a \link nvsg::LOD LOD \endlink (Level Of Detail) node while traversing the scene graph.
      NVSG_API virtual void handleLOD(
        const nvsg::LOD * p //!< Points to the currently visited LOD object.
      );
                                          
      //! Routine to handle a \link nvsg::Switch Switch \endlink node while traversing the scene graph.
      NVSG_API virtual void handleSwitch(
        const nvsg::Switch * p //!< Points to the currently visited Switch object.
      );
                                          
      //! Routine to handle a \link nvsg::Transform Transform \endlink node while traversing the scene graph.
      NVSG_API virtual void handleTransform(
        const nvsg::Transform * p //!< Points to the currently visited Transform object.
      );
      
      //! Routine to handle a \link nvsg::AnimatedTransform AnimatedTransform \endlink node while traversing the scene graph.
      NVSG_API virtual void handleAnimatedTransform(
        const nvsg::AnimatedTransform * p //!< Points to the currently visited AnimatedTransform object.
      );
      
      //! Routine to handle a \link nvsg::DirectedLight DirectedLight \endlink node while traversing the scene graph.
      NVSG_API virtual void handleDirectedLight(
        const nvsg::DirectedLight * p //!< Points to the currently visited DirectedLight object.
      );
      
      //! Routine to handle a \link nvsg::PointLight PointLight \endlink node while traversing the scene graph.
      NVSG_API virtual void handlePointLight(
        const nvsg::PointLight * p //!< Points to the currently visited PointLight object.
      );

      //! Routine to handle a \link nvsg::SpotLight SpotLight \endlink node while traversing the scene graph.
      NVSG_API virtual void handleSpotLight(
        const nvsg::SpotLight * p //!< Points to the currently visited SpotLight object.
      );

      //! Routine to handle a \link nvsg::Lines Lines \endlink object while traversing the scene graph.
      NVSG_API virtual void handleLines(
        const nvsg::Lines * p     //!< Points to the currently visited Lines object.
        );

      //! Routine to handle a \link nvsg::LineStrips LineStrips \endlink object while traversing the scene graph.
      NVSG_API virtual void handleLineStrips(
        const nvsg::LineStrips * p //!< Points to the currently visited LineStrips object.
        );

      //! Routine to handle a \link nvsg::Points Points \endlink object while traversing the scene graph.
      NVSG_API virtual void handlePoints(
        const nvsg::Points * p     //!< Points to the currently visited Points object.
        );

      //! Routine to handle a QuadMeshes object while traversing the scene graph.
      NVSG_API virtual void handleQuadMeshes(
        const nvsg::QuadMeshes * p //!< Points to the currently visited QuadMeshes object.
        );

      //! Routine to handle a \link nvsg::Quads Quads \endlink object while traversing the scene graph.
      NVSG_API virtual void handleQuads(
        const nvsg::Quads * p //!< Points to the currently visited Quads object.
      );

      //! Routine to handle a \link nvsg::AnimatedQuads AnimatedQuads \endlink object while traversing the scene graph.
      NVSG_API virtual void handleAnimatedQuads(
        const nvsg::AnimatedQuads * p //!< Points to the currently visited AnimatedQuads object.
      );

      //! Routine to handle a \link nvsg::QuadStrips QuadStrips \endlink object while traversing the scene graph.
      NVSG_API virtual void handleQuadStrips(
        const nvsg::QuadStrips * p //!< Points to the currently visited QuadStrips object.
      );

      //! Routine to handle a \link nvsg::Triangles Triangles \endlink object while traversing the scene graph.
      NVSG_API virtual void handleTriangles(
        const nvsg::Triangles * p //!< Points to the currently visited Triangles object.
      );

      //! Routine to handle a \link nvsg::AnimatedTriangles AnimatedTriangles \endlink object while traversing the scene graph.

      NVSG_API virtual void handleAnimatedTriangles(
        const nvsg::AnimatedTriangles * p //!< Points to the currently visited AnimatedTriangles object.
      );

      //! Routine to handle a \link nvsg::SkinnedTriangles SkinnedTriangles \endlink object while traversing the scene graph.
      NVSG_API virtual void handleSkinnedTriangles(
        const nvsg::SkinnedTriangles * p //!< Points to the currently visited SkinnedTriangles object.
      );

      //! Routine to handle a \link nvsg::TriFans TriFans \endlink object while traversing the scene graph.
      NVSG_API virtual void handleTriFans(
        const nvsg::TriFans * p //!< Points to the currently visited TriFans object.
      );

      //! Routine to handle a \link nvsg::TriStrips TriStrips \endlink object while traversing the scene graph.
      NVSG_API virtual void handleTriStrips(
        const nvsg::TriStrips * p //!< Points to the currently visited TriStrips object.
      );

      //! Routine to handle a \link nvsg::StateSet StateSet \endlink object while traversing the scene graph.
      NVSG_API virtual void handleStateSet(
        const nvsg::StateSet * p //!< Points to the currently visited StateSet object.
      );

      //! Routine to handle a \link nvsg::StateVariant StateVariant \endlink object while traversing the scene graph.
      NVSG_API virtual void handleStateVariant(
        const nvsg::StateVariant * p //!< Points to the currently visited StateVariant object.
        );

      //! Routine to handle a \link nvsg::StatePass StatePass \endlink object while traversing the scene graph.
      NVSG_API virtual void handleStatePass(
        const nvsg::StatePass * p //!< Points to the currently visited StatePass object.
        );

      //! Routine to handle a \link nvsg::BlendAttribute BlendAttribute \endlink attribute while traversing the scene graph.
      NVSG_API virtual void handleBlendAttribute(
        const nvsg::BlendAttribute * p //!< Points to the currently visited BlendAttribute object.
        );

      //! Routine to handle a \link nvsg::CgFx CgFx \endlink attribute while traversing the scene graph.
      NVSG_API virtual void handleCgFx(
        const nvsg::CgFx * p //!< Points to the currently visited CgFx object.
      );

      //! Routine to handle a \link nvsg::DepthAttribute DepthAttribute \endlink attribute while traversing the scene graph.
      NVSG_API virtual void handleDepthAttribute(
        const nvsg::DepthAttribute * p //!< Points to the currently visited DepthAttribute object.
        );

      //! Routine to handle a \link nvsg::FaceAttribute FaceAttribute \endlink attribute while traversing the scene graph.
      NVSG_API virtual void handleFaceAttribute(
        const nvsg::FaceAttribute * p //!< Points to the currently visited FaceAttribute object.
      );

      //! Routine to handle a \link nvsg::LineAttribute LineAttribute \endlink attribute while traversing the scene graph.
      NVSG_API virtual void handleLineAttribute(
        const nvsg::LineAttribute * p //!< Points to the currently visited LineAttribute object.
        );

      //! Routine to handle a \link nvsg::Material Material \endlink attribute while traversing the scene graph.
      NVSG_API virtual void handleMaterial(
        const nvsg::Material * p //!< Points to the currently visited Material object.
      );

      //! Routine to handle a \link nvsg::PointAttribute PointAttribute \endlink attribute while traversing the scene graph.
      NVSG_API virtual void handlePointAttribute(
        const nvsg::PointAttribute * p //!< Points to the currently visited PointAttribute object.
        );

      //! Routine to handle a \link nvsg::TextureAttribute TextureAttribute \endlink attribute while traversing the scene graph.
      NVSG_API virtual void handleTextureAttribute(
        const nvsg::TextureAttribute * p //!< Points to the currently visited TextureAttribute object.
      );

      //! Routine to handle a \link nvsg::ParallelCamera ParallelCamera \endlink object while traversing the scene graph.
      NVSG_API virtual void handleParallelCamera(
        const nvsg::ParallelCamera * p //!< Points to the currently visited ParallelCamera object.
      );

      //! Routine to handle a \link nvsg::PerspectiveCamera PerspectiveCamera \endlink object while traversing the scene graph.

      NVSG_API virtual void handlePerspectiveCamera(
        const nvsg::PerspectiveCamera * p //!< Points to the currently visited PerspectiveCamera object.
      );

      //! Routine to handle a \link nvsg::Animation FramedAnimation<Trafo> \endlink object while traversing the scene graph.
      NVSG_API virtual void handleFramedTrafoAnimation(
        const nvsg::FramedAnimation<nvmath::Trafo> * p //!< Points to the currently visited Animation object.
      );

      //! Routine to handle a \link nvsg::Animation FramedAnimation<VNVector> \endlink object while traversing the scene graph.

      NVSG_API virtual void handleFramedVNVectorAnimation(
        const nvsg::FramedAnimation<nvsg::VNVector> * p //!< Points to the currently visited Animation object.
      );

      //! Routine to handle a \link nvsg::Animation LinearInterpolatedAnimation<Trafo> \endlink object while traversing the scene graph.
      NVSG_API virtual void handleLinearInterpolatedTrafoAnimation(
        const nvsg::LinearInterpolatedAnimation<nvmath::Trafo> * p //!< Points to the currently visited Animation object.
      );

      //! Routine to handle a \link nvsg::Animation LinearInterpolatedAnimation<VNVector> \endlink object while traversing the scene graph.
      NVSG_API virtual void handleLinearInterpolatedVNVectorAnimation(
        const nvsg::LinearInterpolatedAnimation<nvsg::VNVector> * p //!< Points to the currently visited Animation object.
      );


      //! Search through an Animation<Trafo> object.
      /** This function is called when base class searching is enabled. */
      NVSG_API virtual void search( const nvsg::Animation<nvmath::Trafo> *p //!<  Animation to search through
                                    );

      //! Search through an Animation<VNVector> object.
      /** This function is called when base class searching is enabled. */
      NVSG_API virtual void search( const nvsg::Animation<nvsg::VNVector> *p //!<  Animation to search through
                                  );

      //! Search through a Camera object.
      /** This function is called when base class searching is enabled. */
      NVSG_API virtual void search( const nvsg::Camera *p             //!<  Camera to search through
                                  );

      //! Search through a Drawable object.
      /** This function is called when base class searching is enabled. */
      NVSG_API virtual void search( const nvsg::Drawable *p           //!<  Drawable to search through
                                  );

      //! Search through a Group object.
      /** This function is called when Group is searched through or base class searching is enabled. */
      NVSG_API virtual void search( const nvsg::Group *p                //!<  Group to search through
                                  );

      //! Search through an IndependentPrimitiveSet.
      /** This function is called when a base class searching is enabled. */
      NVSG_API virtual void search( const nvsg::IndependentPrimitiveSet * p    //!< IndependentPrimitiveSet to search through
                                  );

      //! Search through an InterpolatedAnimation<Trafo>.
      /** This function is called when base class searching is enabled. */
      NVSG_API virtual void search( const nvsg::InterpolatedAnimation<nvmath::Trafo> *p  //!<  InterpolatedAnimation to search through
                                  );

      //! Search through an InterpolatedAnimation<VNVector>.
      /** This function is called when base class searching is enabled. */
      NVSG_API virtual void search( const nvsg::InterpolatedAnimation<nvsg::VNVector> *p  //!<  InterpolatedAnimation to search through
                                  );

      //! Search through a LightSource object.
      /** This function is called when base class searching is enabled. */
      NVSG_API virtual void search( const nvsg::LightSource *p        //!<  LightSource to search through
                                  );

      //! Search through a MeshedPrimitiveSet.
      /** This function is called when a base class searching is enabled. */
      NVSG_API virtual void search( const nvsg::MeshedPrimitiveSet * p    //!< MeshedPrimitiveSet to search through
        );

      //! Search through a Node object.
      /** This function is called when base class searching is enabled. */
      NVSG_API virtual void search( const nvsg::Node *p               //!<  Node to search through
                                  );

      //! Search through an Object object.
      /** This function is called when base class searching is enabled. */
      NVSG_API virtual void search( const nvsg::Object *p             //!<  Object to search through
                                  );

      //! Search through a PrimitiveSet object.
      /** This function is called when base class searching is enabled. */
      NVSG_API virtual void search( const nvsg::PrimitiveSet *p          //!<  PrimitiveSet to search through
                                  );

      //! Search through an Quads object.
      /** This function is called when a Quad is search through or a base class searching is enabled. */
      NVSG_API virtual void search( const nvsg::Quads *p               //!<  Quads to search through
                                  );
    
      //! Search through a StateAttribute object.
      /** This function is called when base class searching is enabled. */
      NVSG_API virtual void search( const nvsg::StateAttribute *p     //!<  StateAttribute to search through
                                  );

      //! Search through an StrippedPrimitiveSet.
      /** This function is called when a base class searching is enabled. */
      NVSG_API virtual void search( const nvsg::StrippedPrimitiveSet * p    //!< StrippedPrimitiveSet to search through
                                  );

      //! Search through a Transform object.
      /** This function is called when Transform is searched or a base class searching is enabled. */
      NVSG_API virtual void search( const nvsg::Transform *p            //!<  Transform to search through
                                  );

      //! Search through a Triangles object.
      /** This function is called when a Triangles is searched or a base class searching is enabled. */
      NVSG_API virtual void search( const nvsg::Triangles *p            //!<  Triangles to search through
                                  );

    private:
      void  addItem( const nvsg::Object* T );

      nvsg::Path                        * m_currentPath;
      std::string                         m_className;
      std::vector<const nvsg::Path*>      m_paths;
      std::vector<nvsg::ObjectHandle*>    m_results;
      bool                                m_searchBaseClass;
  };

  inline const std::vector<nvsg::ObjectHandle *> & SearchTraverser::getResults( void ) const
  {
    return( m_results );
  }

  inline const std::vector<const nvsg::Path*> & SearchTraverser::getPaths( void ) const
  {
    return( m_paths );
  }

  inline  void  SearchTraverser::setClassName( const std::string& name )
  {
    m_className = name;
  }

  inline  void  SearchTraverser::setBaseClassSearch( bool enable )
  {
    m_searchBaseClass = enable;
  }

}
