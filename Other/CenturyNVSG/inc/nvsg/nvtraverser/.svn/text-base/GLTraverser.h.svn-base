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
#include "nvsgcommon.h" // common used stuff 
#include "nvgl/GLContext.h"
#include "nvgl/GLDALData.h"
#include "nvmath/Mat44f.h"
#include "nvtraverser/RenderTraverser.h" // base class definition

namespace nvgl
{
  class FBOFrameBuffer; // internal class, not meant to be publicly used
  class PBuffer;        // internal class, not meant to be publicly used
}

namespace nvtraverser
{  
//! Use this to make sure that the right OpenGL context is current.
#if defined(_WIN32) && !defined(NDEBUG)
  #define NVSG_ASSERT_CONTEXT() assertContext(); 
#else
  #define NVSG_ASSERT_CONTEXT()
#endif

  //! RenderTraverser using OpenGL
  /** \note This traverser requires a valid view state when you call 
    * the \link nvtraverser::Traverser::apply apply method\endlink. 
    * \note Needs a valid ViewState. Call setViewState prior to apply().*/
  class GLTraverser : public RenderTraverser
  {
    public:
      //! Default constructor
      NVSG_API GLTraverser(void);

      //! Test on supported geometry caching techniques.
      /** Use this method to test if this special geometry caching technique is supported.
        * \return true - supported, false - not supported.
        * \note GCT_NONE and GCT_DL are supported. GCT_VBO is only supported when the OpenGL extension
        * GL_ARB_vertex_buffer_object is available. */
      NVSG_API virtual bool isGeometryCacheTechniqueSupported( GeometryCacheTechnique gct  //! GeometryCache technique.
                                                             ) const;

      //! Set the geometry caching technique the render should use.
      NVSG_API virtual void setGeometryCacheTechnique( GeometryCacheTechnique gct          //! GeometryCache technique.
                                                     );

      //! Test on supported jitter techniques.
      /** Use this method to test if this special jitter technique is currently available.
        * \return true - supported, false - not supported
        * \note JT_NONE and JT_READ_PIXELS are supported. JT_ACCUM is only supported while the shadow technique is not 
        * ST_SHADOW_MAP (because this technique already uses the accumulation buffer). JT_PBUFFER is available under
        * Windows, when the OpenGL extension GL_NV_fragment_program is available. */
      NVSG_API virtual bool isJitterTechniqueSupported( JitterTechnique jt               //!< Jitter technique.
                                                      ) const;

      //! Set the jitter technique the render should use.
      NVSG_API virtual void setJitterTechnique( JitterTechnique jt                       //!< Jitter technique
                                              );

      //! Test on supported render techniques.
      /** Use this technique to test if this special render technique is currently available.
        * \return true - supported, false - not supported.
        * \note All render techniques from RT_POINTS to RT_EFFECTED are available. */
      NVSG_API virtual bool isRenderTechniqueSupported( RenderTechnique rt               //! Render technique.
                                                      ) const;

      //! Test on supported shadowing techniques.
      /** Use this to test if this special shadow technique is currently available.
        * \return true - supported, false - not supported.
        * \note ST_NONE is supported. ST_SHADOW_MAP is only supported, if neither the jitter technique is JT_ACCUM (because
        * the ST_SHADOW_MAP would also need the accumulation buffer) nor the transparency technique is
        * TT_ORDER_INDEPENDENT (because the current implementation of order independent transparency uses some small
        * fragment programs, that don't handle texture coordinate generation and an additional depth texture). */
      NVSG_API virtual bool isShadowTechniqueSupported( ShadowTechnique st  //! shadow technique to test
                                                      ) const;

      //! Set the size of the shadow maps.
      /** The size must be a power of two. The default size is 1024. */
      NVSG_API virtual void setShadowMapSize( size_t size                   //! size of the shadow map (power of two!)
                                            );

      //! Set the shadow technique to use.
      /** \return true - everything went fine, false - not set */
      NVSG_API virtual void setShadowTechnique( ShadowTechnique st          //! shadow technique to use
                                              );

      //! Set the texture size limit.
      /** Textures are scaled down to that limit if they are larger.
        * \note Setting the size to 0 (zero), disables texture size limit. */
      NVSG_API virtual void setTextureSizeLimit( size_t size );

      //! Test on supported transparency techniques.
      /** Use this technique to test if this special transparency technique is currently available.
        * \return true - supported, false - not supported.
        * \note TT_NONE, TT_STIPPLED, TT_BLENDED, and TT_SORTED_BLENDED are supported. TT_ORDER_INDEPENDENT is only
        * supported, if the OpenGL extension GL_NV_fragment_program is available and the shadow technique is not
        * ST_SHADOW_MAP (because the current implementation of order independent transparency uses some small
        * fragment programs, that don't handle texture coordinate generation and an additional depth texture needed
        * for shadow mapping). */
      NVSG_API virtual bool isTransparencyTechniqueSupported( TransparencyTechnique tt   //! Transparency technique.
                                                            ) const;

      //! Set the transparency technique the render should use.
      NVSG_API virtual void setTransparencyTechnique( TransparencyTechnique tt           //! Transparency technique.
                                                    );

      //! Test on supported OIT techniques.
      /** Use this technique to test if this special OIT technique is currently available.
        * \return true - supported, false - not supported.
        * \note Both OT_TEXTURE and OT_PBUFFER are supported if order independent transparency itself is supported. */
      NVSG_API virtual bool isOITTechniqueSupported( OITTechnique ot                     //! OIT technique 
                                                   ) const;

      //! Set the OIT technique the render should use.
      /** \return true - everything went fine, false - not set */
      NVSG_API virtual void setOITTechnique( OITTechnique ot                             //! OIT technique 
                                           );

      //! Set the number of maximum passes to achieve the desired transparency effect.
      NVSG_API virtual void setOITMaxPasses( size_t maxPasses                            //! Maximum passes to render, even when threshold is not reached.
                                           );

      //! Enable/Disable "local viewer"
      /** Use OpenGL light model: "local viewer"
        * After creating a new GLTraverser "local viewer" is turned off. */
      NVSG_API void setLocalViewer( bool onOff  //!< enables/disables local viewer
                                  );
      //! Test if "local viewer" is enabled/disabled.
      /** Test if "local viewer" is enabled. See glLightModel() and GL_LIGHT_MODEL_LOCAL_VIEWER
        * in the OpenGL documentation for more information.
        * \returns \c true, if "local viewer" is enabled, otherwise \c false. */
      NVSG_API bool isLocalViewerEnabled() const;

      //! Enable/Disable "separate specular color"
      /** Use OpenGL light model: "separate specular color"
        * After creating a new GLTraverser "separate specular color" is turned on. */
      NVSG_API void setSeparateSpecularColor( bool onOff  //!< enables/disables separate specular color
                                            );

      //! Test if "separate specular color" is enabled/disabled.
      /** Test if "separate specular color" is enabled. See glLightModel() and GL_LIGHT_MODEL_COLOR_CONTROL
        * in the OpenGL documentation for more information.
        * \returns \c true, if "separate specular color" is enabled, otherwise \c false. */
      NVSG_API bool isSeparateSpecularColorEnabled( void ) const;

      NVSG_API virtual bool isOcclusionCullingSupported( void ) const;
      NVSG_API virtual unsigned int createOcclusionCullID( void );
      NVSG_API virtual void   destroyOcclusionCullID( unsigned int id );
      NVSG_API virtual unsigned int getOcclusionCullResult( unsigned int id );
      NVSG_API virtual void   beginOcclusionCullQuery( unsigned int id );
      NVSG_API virtual void   endOcclusionCullQuery( unsigned int id );
      NVSG_API virtual void   hiddenBegin( void );
      NVSG_API virtual void   hiddenEnd( void );

    protected:
      //! Default-instantiates a GLTraverser object.
      NVSG_API virtual ~GLTraverser(void);

      //! doApply override to perform a specialized treatment.
      NVSG_API virtual void doApply( nvsg::NodeHandle * root );

      //! doRender override to perform GL-rendering
      NVSG_API virtual void doRender( nvsg::NodeHandle * root );

      //! handleBitmap override for specialized handling of Bitmap.
      NVSG_API virtual void handleBitmap( const nvsg::Bitmap * p );

      //! handleLines override for specialized handling of Lines.
      NVSG_API virtual void handleLines(const nvsg::Lines * p);

      //! handleLineStrips override for specialized handling of LineStrips.
      NVSG_API virtual void handleLineStrips(const nvsg::LineStrips * p);

      //! handleMapList override for specialized handling of MapList.
      NVSG_API virtual void handleMapList( const nvsg::MapList * p );

      //! handlePoints override for specialized handling of Points.
      NVSG_API virtual void handlePoints(const nvsg::Points * p);

      //! handleQuadMeshes override for specialized handling of QuadMeshes.
      NVSG_API virtual void handleQuadMeshes(const nvsg::QuadMeshes * p);

      //! handleQuads override for specialized handling of Quads.
      NVSG_API virtual void handleQuads(const nvsg::Quads * p);

      //! handleQuadStrips override for specialized handling of QuadStrips.
      NVSG_API virtual void handleQuadStrips(const nvsg::QuadStrips * p);

      //! handleTriangles override for specialized handling of Triangles.
      NVSG_API virtual void handleTriangles(const nvsg::Triangles * p);

      //! handleTriFans override for specialized handling of TriFans.
      NVSG_API virtual void handleTriFans(const nvsg::TriFans * p);

      //! handleTriStrips override for specialized handling of TriStrips.
      NVSG_API virtual void handleTriStrips(const nvsg::TriStrips * p);

      //! handleVertexAttributeSet override for specialized handling of VertexAttributeSet.
      NVSG_API virtual void handleVertexAttributeSet( const nvsg::VertexAttributeSet *p );

      //! Render an AnimatedQuads out of a render list.
      /** Draws the current Quads. */
      NVSG_API virtual void renderAnimatedQuads( const nvsg::AnimatedQuads *p   //!<  AnimatedQuads to render
                                               );

      //! Render an AnimatedTriangles out of a render list.
      /** Draws the current Triangles. */
      NVSG_API virtual void renderAnimatedTriangles( const nvsg::AnimatedTriangles *p   //!<  AnimatedTriangles to render
                                                   );

      //! Begin the rendering process.
      /** Before rendering the first object, this function is called. The LightModel parameters are set here. */
      NVSG_API virtual void renderBegin( void );

      //! End the rendering process.
      /** After rendering the last object, this function is called. */
      NVSG_API virtual void renderEnd( void );

      //! Render a BlendAttribute out of a render list.
      /** Sets or resets the parameters of the BlendAttribute. */
      NVSG_API virtual void renderBlendAttribute( const nvsg::BlendAttribute *p //!<  BlendAttribute to render
                                                , bool enable                   //!<  enable (\c true) or disable (\c false) this BlendAttribute
                                                );

      //! Render a DepthAttribute out of a render list.
      /** Sets or resets the parameters of the DepthAttribute. */
      NVSG_API virtual void renderDepthAttribute( const nvsg::DepthAttribute *p //!<  DepthAttribute to render
                                                , bool enable                   //!<  enable(\c true) or disable (\c false) this DepthAttribute
                                                );

      //! Render a DirectedLight out of a render list.
      /** Enables or disables the DirectedLight and sets the lights parameters accordingly. */
      NVSG_API virtual void renderDirectedLight( const nvsg::DirectedLight *p   //!<  DirectedLight to render
                                               , bool enable                    //!<  enable (\c true) or disable (\c false) this DirectedLight
                                               );

      //! Render a FaceAttribute out of a render list.
      /** Sets or resets the parameters of the FaceAttribute. */
      NVSG_API virtual void renderFaceAttribute( const nvsg::FaceAttribute *p   //!<  FaceAttribute to render
                                               , bool enable                    //!<  enable (\c true) or disable (\c false) this FaceAttribute
                                               );

      //! Render a HighlightBox out of a render list.
      /** Draws the HighlightBox. */
      NVSG_API virtual void renderHighlightBox( const nvsg::HighlightBox * p    //!< HighlightBox to render
                                              );

      //! Render a HighlightEdges out of a render list.
      /** Draws the HighlightEdges. */
      NVSG_API virtual void renderHighlightEdges( const nvsg::HighlightEdges * p  //!< HighlightEdges to render
                                                );

      //! Render a LineAttribute out of a render list.
      /** Sets or resets the parameters of the LineAttribute. */
      NVSG_API virtual void renderLineAttribute( const nvsg::LineAttribute *p   //!<  LineAttribute to render
                                               , bool enable                    //!<  enable (\c true) or disable (\c false) this LineAttribute
                                               );

      //! Render a Lines out of a render list.
      /** Draws the Lines. */
      NVSG_API virtual void renderLines( const nvsg::Lines *p                  //!<  Lines to render
                                       );

      //! Render a LineStrips out of a render list.
      /** Draws the LineStrips. */
      NVSG_API virtual void renderLineStrips( const nvsg::LineStrips *p        //!<  LineStrips to render
                                            );

      //! Render a PointAttribute out of a render list.
      /** Sets or resets the parameters of the PointAttribute. */
      NVSG_API virtual void renderPointAttribute( const nvsg::PointAttribute *p //!<  PointAttribute to render
                                                , bool enable                   //!<  enable (\c true) or disable (\c false) this PointAttribute
                                                );

      //! Render a Points out of a render list.
      /** Draws the Points. */
      NVSG_API virtual void renderPoints( const nvsg::Points*p                 //!<  Points to render
                                        );

      //! Render a Material out of a render list.
      /** Sets or resets the parameters of the Material. */
      NVSG_API virtual void renderMaterial( const nvsg::Material *p             //!<  Material to render
                                          , bool enable                         //!<  enable (\c true) or disable (\c false) this Material
                                          );

      //! Render a ParallelCamera out of a render list.
      /** Sets the current Projection and ModelView matrix. */
      NVSG_API virtual void renderParallelCamera( const nvsg::ParallelCamera *p //!<  ParallelCamera to render
                                                );

      //! Render a PerspectiveCamera out of a render list.
      /** Sets the current Projection and ModelView matrix. */
      NVSG_API virtual void renderPerspectiveCamera( const nvsg::PerspectiveCamera *p //!<  PerspectiveCamera to render
                                                   );

      //! Render a PointLight out of a render list.
      /** Enables or disables the PointLight and sets the lights parameters accordingly. */
      NVSG_API virtual void renderPointLight( const nvsg::PointLight *p         //!<  PointLight to render
                                            , bool enable                       //!<  enable (\c true) or disable (\c false) this PointLight
                                            );

      //! Render a QuadMeshes out of a render list.
      /** Draws the QuadMeshes. */
      NVSG_API virtual void renderQuadMeshes( const nvsg::QuadMeshes *p         //!<  QuadMeshes to render
        );

      //! Render a Quads out of a render list.
      /** Draws the Quads. */
      NVSG_API virtual void renderQuads( const nvsg::Quads *p                   //!<  Quads to render
                                       );

      //! Render a QuadStrips out of a render list.
      /** Draws the QuadStrips. */
      NVSG_API virtual void renderQuadStrips( const nvsg::QuadStrips *p         //!<  QuadStrips to render
                                            );

      //! Render a SkinnedTriangles out of a render list.
      /** Draws the current Triangles. */
      NVSG_API virtual void renderSkinnedTriangles( const nvsg::SkinnedTriangles *p   //!<  SkinnedTriangles to render
                                                  );

      //! Render a SpotLight out of a render list.
      /** Enables or disables the SpotLight and sets the lights parameters accordingly. */
      NVSG_API virtual void renderSpotLight( const nvsg::SpotLight *p           //!<  SpotLight to render
                                           , bool enable                        //!<  enable (\c true) or disable (\c false) this SpotLight
                                           );

      //! Render a StateSet out of a render list.
      /** Calls RenderTraverser::renderStatePass() and if order independent transparency is active and its a transparent
        * pass, a special fragment program is bound. */
      NVSG_API virtual void renderStatePass( const nvsg::StatePass *p            //!<  StateSet to render
                                           );

      //! Render a TextureAttribute out of a render list.
      /** Calls RenderTraverser::renderTextureAttribute() and disables all unused texture units. */
      NVSG_API virtual void renderTextureAttribute( const nvsg::TextureAttribute *p //!<  TextureAttribute to render
                                                  , bool enable                     //!<  enable (\c true) or disable (\c false) this TextureAttribute
                                                  );

      //! Render a TextureAttributeItem out of a render list.
      /** Loads the Texture if necessary, sets the texture matrix, parameters, and environments accordingly, enables
        * the texture unit \a tu and binds the texture. */
      NVSG_API virtual void renderTextureAttributeItem( const nvsg::TextureAttributeItem *p //!<  TextureAttributeItem to render
                                                      , size_t tu                           //!<  texture unit to use
                                                      );

      //! Render a Triangles out of a render list.
      /** Draws the Triangles. */
      NVSG_API virtual void renderTriangles( const nvsg::Triangles *p           //!<  Triangles to render
                                           );

      //! Render a TriFans out of a render list.
      /** Draws the TriFans. */
      NVSG_API virtual void renderTriFans( const nvsg::TriFans *p               //!<  TriFans to render
                                         );

      //! Render a TriStrips out of a render list.
      /** Draws the TriStrips. */
      NVSG_API virtual void renderTriStrips( const nvsg::TriStrips *p           //!<  TriStrips to render
                                           );

      //! Set a clipping plane.
      /** Sets \a plane as a clipping plane. */
      NVSG_API virtual void setClipPlane( size_t index                    //!< index of the plane
                                        , const nvmath::Plane3f & plane   //!< plane to use
                                        , bool enable                     //!< enable/disable it
                                        );

      //! Set the current model to world and world to model transforms.
      /** Enables or disables normal normalization according to \a needsNormalize and loads the ModelView matrix as the
        * concatenation of the modelToWorld and the current worldToView (from the latest camera setting). */
      NVSG_API virtual void setTransforms( const nvmath::Mat44f &modelToWorld   //!<  current model to world transform
                                         , const nvmath::Mat44f &worldToModel   //!<  current world to model transform
                                         , bool needsNormalize                  //!<  \c true if these transforms include a scaling
                                         , bool needsMirror                     //!<  \c true if these transforms includes a negative scaling
                                         );

      //! Assigns current settings of a DirectedLight to the specified effect light parameter.
      /** The purpose of this function is to transmit the settings of the specified DirectedLight
        * to the specified effect light through the CgFxEffect interface. */
      NVSG_API virtual void assignDirectedLightToEffectLight( nvsg::CgFxEffect * effect
                                                            , nvsg::CgFxParameter lightParam 
                                                            , const nvsg::DirectedLight * directedLight
                                                            , const nvmath::Mat44f& modelToWorld
                                                            ); 

      //! Assigns current settings of a PointLight to the specified effect light parameter.
      /** The purpose of this function is to transmit the settings of the specified PointLight
        * to the specified effect light through the CgFxEffect interface. */
      NVSG_API virtual void assignPointLightToEffectLight( nvsg::CgFxEffect * effect
                                                         , nvsg::CgFxParameter lightParam 
                                                         , const nvsg::PointLight * pointLight
                                                         , const nvmath::Mat44f& modelToWorld
                                                         ); 

      //! Assigns current settings of a SpotLight to the specified effect light parameter.
      /** The purpose of this function is to transmit the settings of the specified SpotLight
        * to the specified effect light through the CgFxEffect interface. */
      NVSG_API virtual void assignSpotLightToEffectLight( nvsg::CgFxEffect * effect
                                                        , nvsg::CgFxParameter lightParam 
                                                        , const nvsg::SpotLight * spotLight
                                                        , const nvmath::Mat44f& modelToWorld
                                                        ); 

      //! Indication of the beginning of rendering overlay parts.
      NVSG_API virtual void overlayBegin( void );

      //! Indication of the end of rendering overlay parts.
      NVSG_API virtual void overlayEnd( void );

      //! Indication of the beginning of rendering transparent parts.
      NVSG_API virtual void transparentBegin( void );

      //! Indication of the end of rendering transparent parts.
      NVSG_API virtual void transparentEnd( void );

      //! Indication of the beginning of rendering highlighted parts.
      NVSG_API virtual void highlightBegin( void );

      //! Indication of the end of rendering highlighted parts.
      NVSG_API virtual void highlightEnd( void );

      //! Initiates PrimitiveSet rendering
      NVSG_API virtual void primitiveSetsBegin();

      //! Post-processes PrimitiveSet rendering
      /** \returns \c true if the PrimitiveSets need to be re-processed again. 
        * Otherwise the function returns \c false. */
      NVSG_API virtual bool primitiveSetsEnd();

    private:


      // A little helper class for screen door transparency.
      class StipplePatterns
      {
	      public:
          StipplePatterns();
          unsigned char  * getBits(int index);

        private:
          unsigned char  patBytes[ 64 ][ 8 ];
		      unsigned long stippleBits[64][32];

          StipplePatterns( const StipplePatterns &stipplePatterns );
          StipplePatterns& operator=( const StipplePatterns &stipplePatterns );
      };


    private:
      void                  createJitterFragmentPrograms( void );
      void                  doApplyShadowExit( void );
      void                  doApplyShadowInit( nvsg::NodeHandle * root );
      void                  drawMeshedPrimitiveSet( const nvsg::MeshedPrimitiveSet *p, GLenum mode, size_t numberOfMeshes, const nvsg::IndexMesh *meshes );
      void                  drawMeshedPrimitiveSetDL( const nvsg::MeshedPrimitiveSet *p, GLenum mode, size_t numberOfMeshes, const nvsg::IndexMesh *meshes );
      void                  drawMeshedPrimitiveSetVBO( const nvsg::MeshedPrimitiveSet *p, GLenum mode, size_t numberOfMeshes, const nvsg::IndexMesh *meshes );
      void                  drawIndependentPrimitiveSet( const nvsg::IndependentPrimitiveSet *p, GLenum mode, GLsizei count, const GLvoid *indices );
      void                  drawIndependentPrimitiveSetDL( const nvsg::IndependentPrimitiveSet *p, GLenum mode, GLsizei count, const GLvoid *indices );
      void                  drawIndependentPrimitiveSetVBO( const nvsg::IndependentPrimitiveSet *p, GLenum mode, GLsizei count, const GLvoid *indices );
      void                  drawStrippedPrimitiveSet( const nvsg::StrippedPrimitiveSet *p, GLenum mode, size_t numberOfStrips, const nvsg::IndexSet *strips );
      void                  drawStrippedPrimitiveSetDL( const nvsg::StrippedPrimitiveSet *p, GLenum mode, size_t numberOfStrips, const nvsg::IndexSet *strips );
      void                  drawStrippedPrimitiveSetVBO( const nvsg::StrippedPrimitiveSet *p, GLenum mode, size_t numberOfStrips, const nvsg::IndexSet *strips );
      void                  evaluateLightingAndColorMaterial(const nvsg::PrimitiveSet *p);
      nvgl::GLDisplayListCache * getDisplayListCache( const nvsg::PrimitiveSet *p ) const;
      nvgl::GLElementArrayBufferCache * getElementArrayBufferCache(const nvsg::PrimitiveSet * p) const;
      nvgl::GLArrayBufferCache * getArrayBufferCache( const nvsg::VertexAttributeSet * p ) const;
      nvgl::GLTextureCache * getTextureCache(const nvsg::TextureImage * p) const;
      void                  handleIndependentPrimitiveSetVBO( const nvsg::IndependentPrimitiveSet * p );
      void                  handleMeshedPrimitiveSetVBO( const nvsg::MeshedPrimitiveSet * p );
      void                  handleStrippedPrimitiveSetVBO( const nvsg::StrippedPrimitiveSet * p );
      void                  indexMeshesToIndexSets( size_t numberOfMeshes, const nvsg::IndexMesh * meshes, std::vector<nvsg::IndexSet> &sets );
      static bool           isDisplayListCacheValid( const nvgl::GLDisplayListCache* info );
      static bool           isArrayBufferCacheValid( const nvgl::GLArrayBufferCache *info);
      static bool           isElementArrayBufferCacheValid( const nvgl::GLElementArrayBufferCache *info);
      static bool           isTextureCacheValid( const nvgl::GLTextureCache *info);
      void                  jitterClear( void );
      void                  jitterInit( void );
      void                  jitterVerify(); // called on instantiation to find the best matching jitter technique
                                            // starting with the default inherited from the base class
      void                  prepareEffect( nvsg::CgFxEffect * effect );
      void                  processPrimitiveSet( const nvsg::PrimitiveSet *p );
      void                  processPrimitiveSetVBO( const nvsg::PrimitiveSet *p, const nvgl::GLElementArrayBufferCache * vboInfoI, const nvgl::GLArrayBufferCache * vboInfoV );
      void                  renderCamera( const nvsg::Camera *p );
      void                  renderIndependentPrimitiveSet( const nvsg::IndependentPrimitiveSet *p, GLenum mode );
      void                  renderMeshedPrimitiveSet( const nvsg::MeshedPrimitiveSet *p, GLenum mode );
      void                  renderStrippedPrimitiveSet( const nvsg::StrippedPrimitiveSet *p, GLenum mode );
      void                  renderJitteredAccum( const nvsg::ViewState *p );
      void                  renderJitteredFBO( const nvsg::ViewState *p );
      void                  renderJitteredPBuffer( const nvsg::ViewState *p );
      void                  renderJitteredReadPixels( const nvsg::ViewState *p );
      bool                  renderLightSource( const nvsg::LightSource *p, bool enable, GLenum &lightId );
      void                  renderMono( const nvsg::ViewState *viewState );
      void                  renderShadow( const nvsg::Camera *p );
      void                  renderStereo( const nvsg::ViewState *viewState );
      void                  renderTransparent( const nvsg::Camera *p );
      void                  renderTransparentBlended( const nvsg::Camera *p );
      void                  renderTransparentOrderIndependent( const nvsg::Camera *p );
      void                  setPolygonMode( GLenum face, nvsg::FaceMode mode, const nvsg::FaceAttribute *p = NULL );
      void                  shadowClear( void );
      void                  transparencyOITAllocPBuffers( void );
      void                  transparencyOITAllocFBOs( void );
      void                  transparencyOITAllocTextures( void );
      void                  transparencyClear( void );
      void                  transparencyInit( void );
      void                  transparencyOITFreeFBOBuffers( void );
      void                  transparencyOITFreePBuffers( void );
      void                  transparencyOITFreeTextures( void );
      void                  transparencyOITSetupRendering( void );
      bool                  uploadTextureImage( nvsg::TextureImage * texImg, nvgl::GLTextureCache * cache);

      void genVBOVertices( nvgl::GLArrayBufferCache * viv, const nvsg::VertexAttributeSet * p );
      GLsizeiptr getVBOIndexOffset( const nvsg::VertexAttributeSet * p);
      void genVBO_VtxBuf_Generic(nvgl::GLArrayBufferCache* vi, const nvsg::VertexAttributeSet * p);
      void genVBO_VtxBuf_V3F_N3F(nvgl::GLArrayBufferCache* vi, const nvsg::VertexAttributeSet * p);
      void genVBO_VtxBuf_V3F_N3F_T2F(nvgl::GLArrayBufferCache* vi, const nvsg::VertexAttributeSet * p);
      void genVBO_VtxBuf_V3F_N3F_VC4F_T2F(nvgl::GLArrayBufferCache* vi, const nvsg::VertexAttributeSet * p);
      void genVBO_VtxBuf_V3F_N3F_T2F_TG3F_BN3F(nvgl::GLArrayBufferCache* vi, const nvsg::VertexAttributeSet * p);
      void genVBO_IdxBuf(nvgl::GLElementArrayBufferCache* vi, GLsizeiptr offs, size_t count, const unsigned int * indices);
      void genVBO_IdxBuf(nvgl::GLElementArrayBufferCache* vi, GLsizeiptr offs, size_t count, const nvsg::IndexSet * strips);

    private:
#if !defined(NDEBUG) && defined(_WIN32)
      HGLRC                           m_hglrc;
#endif
      bool                                      m_blendAttributeEnabled;
      nvmath::Mat44f                            m_clipToView;
      size_t                                    m_currentTextureCount;
      bool                                      m_halfFloatSupport;
      bool                                      m_hiddenObject;
      bool                                      m_inShadowMapCreation;
      GLuint                                    m_jitterDataSize;                     // for read-pixels-based jittering
      GLubyte                                 * m_jitterImageData;
      GLushort                                * m_jitterAccumData;
      bool                                      m_jitterAccumulationInitialized;      // for pbuffer-based jittering
      GLuint                                    m_jitterFPAccumulate;
      GLuint                                    m_jitterFPLoad;
      GLuint                                    m_jitterFPReturn;
      nvgl::PBuffer                           * m_jitterFBuffer[2];
      nvgl::PBuffer                           * m_jitterPBuffer;
      nvgl::FBOFrameBuffer                    * m_jitterFBODepth;
      nvgl::FBOFrameBuffer                    * m_jitterFBOColor[2];
      size_t                                    m_lightCount;
      nvsg::LightSourceHandle                 * m_lights[8];
      GLint                                     m_maxClipPlanes;
      GLint                                     m_maxTextureSize;
      GLint                                     m_maxTextureUnits;
      nvmath::Mat44f                            m_modelToWorld;
      bool                                      m_occlusionQuerySupport;
      bool                                      m_pboSupport;
      bool                                      m_primitiveRestartSupport;
      nvmath::Vec3f                             m_shadowBackColor;
      std::vector<nvsg::CameraHandle *>         m_shadowLightCameras;
      std::vector<nvsg::LightSourceHandle *>    m_shadowLights;
      size_t                                    m_shadowPass;
      std::vector<nvgl::PBuffer*>               m_shadowPBuffers;
      std::vector<nvsg::LightSourceHandle *>    m_shadowReplacementLights;
      StipplePatterns                           m_stipple;  // for screen door transparency
      std::vector<nvsg::IndexSet>               m_tempIndexSets;
      bool                                      m_textureNonPowerOfTwoSupport;
      GLint                                     m_transparencyOITDepthFormat;
      GLuint                                    m_transparencyOITDepthTexture;
      GLuint                                    m_transparencyOITFragmentProgram[2];
      bool                                      m_transparencyOITInitialized;
      std::vector<GLuint>                     * m_transparencyOITOcclusionQueries;
      std::vector<GLuint>                       m_transparencyOITPBufferQueries;
      std::vector<nvgl::PBuffer*>               m_transparencyOITPBuffers;
      std::vector<GLuint>                       m_transparencyOITRGBATextureQueries;
      std::vector<GLuint>                       m_transparencyOITRGBATextures;
      std::vector<GLuint>                       m_transparencyOITFBOQueries;
      std::vector<nvgl::FBOFrameBuffer *>       m_transparencyOITFBOs;
      bool                                      m_transparencyOITRendering;
      bool                                      m_transparencyOITSetupPending;
      bool                                      m_transparencyOITTransparentPass;
      int                                       m_transparencyOITViewportHeight;
      int                                       m_transparencyOITViewportWidth;
      int                                       m_viewportWidth;
      int                                       m_viewportHeight;
      nvmath::Mat44f                            m_worldToModel;
      nvmath::Mat44f                            m_worldToView;
      nvmath::Mat44f                            m_viewToClip;
      nvmath::Mat44f                            m_viewToWorld;
      bool                                      m_localViewer;
      bool                                      m_separateSpecularColor;
      nvsg::MaterialHandle *                    m_currentMaterial;
      nvsg::MaterialHandle *                    m_defaultMaterial;
      nvsg::CgFxEffect *                        m_currentCgFxEffect;
      nvsg::CgFxTechnique                       m_currentCgFxTechnique;
      size_t                                    m_currentCgFxNumPasses;
      size_t                                    m_currentCgFxPass;

      // incarnation maps
      std::map<nvsg::TextureAttributeItemHandle*, nvutil::Incarnation> m_perTexItemIncarnationMap;
  };

  inline  unsigned char *  GLTraverser::StipplePatterns::getBits( int index )
  {
    return( (unsigned char *) &stippleBits[ index ][ 0 ] );
  }

  inline void GLTraverser::setLocalViewer( bool onOff )
  {
    m_localViewer = onOff;
  }

  inline bool GLTraverser::isLocalViewerEnabled() const
  {
    return m_localViewer;
  }

  inline void GLTraverser::setSeparateSpecularColor( bool onOff )
  {
    m_separateSpecularColor = onOff;
  }

  inline bool GLTraverser::isSeparateSpecularColorEnabled( void ) const
  {
    return m_separateSpecularColor;
  }
}
