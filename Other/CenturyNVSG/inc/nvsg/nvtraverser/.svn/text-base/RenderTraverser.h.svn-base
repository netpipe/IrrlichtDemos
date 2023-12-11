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

#include <algorithm>
#include <list>
#include "nvsgcommon.h"
#include "nvmath/Plane3f.h"
#include "nvmath/Vec4f.h"
#include "nvsg/CgFx.h"
#include "nvsg/DAL.h"
#include "nvsg/FaceAttribute.h"
#include "nvsg/LightSource.h"
#include "nvsg/PrimitiveSet.h"
#include "nvsg/StateAttribute.h"
#include "nvsg/StateSet.h"
#include "nvsg/ViewState.h"
#include "nvtraverser/ModelViewTraverser.h"
#include "nvutil/Incarnation.h"

namespace nvtraverser
{
  // forward declaration of some helper classes used internally only
  class RenderElementPool;
  class ClipPlanesElement;
  class LightsElement;
  class TransformElement;
  class RenderElement;
  class HighlightElement;

  // renderer type IDs
  enum 
  {
    RT_GL  = 0x00010000
#if defined(SUPPORT_D3D9)
  , RT_DX8 = 0x00020000
  , RT_DX9 = 0x00040000
#endif
  };


  //! Pure virtual render traverser class
  /** Use this pure virtual class as a base class to derive your own 
    * render traverser. Render traverser are not allowed to apply changes 
    * to the given scenegraph.
    * This base class contains the model matrix and inverse model matrix stack.
    * This is very handy, because you do not have to ask e.g. OpenGL which 
    * would be normally very expensive. So your special render traverser has to
    * take care of these stacks.
    * \note The render traverser object must not survive the render area it works on.
    * For e.g. RenderArea and GLTraverser make sure to make the render area 
    * current before destroying the traverser.
    * \note Needs a valid ViewState. Call setViewState prior to apply().*/
  class RenderTraverser : public SharedModelViewTraverser
  {
    public:

      enum GeometryCacheTechnique
      {
        GCT_NONE,       //!<  no geometry caching
        GCT_DL,         //!<  cache geometries in display lists
        GCT_VBO         //!<  cache geometries in vertex buffer objects
      };

      enum JitterTechnique
      {
        JT_NONE,        //!<  Render non-jittered
        JT_READ_PIXELS, //!<  Render to backbuffer and use e.g. OpenGL read pixels to read it back.
        JT_ACCUM,       //!<  Render to accumulation buffer.
        JT_PBUFFER,     //!<  Render to PBuffer.
        JT_FBO          //!<  Render to an FBO.
      };    

      enum RenderTechnique
      {
        RT_UNLIT,       //!<  Render scene without lighting
        RT_SHADED,      //!<  Render scene in simple shading mode.
        RT_TEXTURED,    //!<  Render scene in simple shading mode plus textures.
        RT_EFFECTED,    //!<  Render scene fully effected and shaded.
      };

      //! Enumeration of supported shadowing techniques
      enum ShadowTechnique
      {
        ST_NONE,        //!<  don't do any shadowing
        ST_SHADOW_MAP   //!<  shadow using shadow maps
      };

      //! Enumeration of supported sorting policies
      enum SortingPolicy
      {
        SP_NONE,        //!<  don't sort
        SP_DEPTH,       //!<  depth sort
        SP_STATE        //!<  state sort
      };

      enum TransparencyTechnique
      {
        TT_NONE,              //!< Ignore transparency, draw opaque. 
        TT_STIPPLED,          //!< Use stipple patterns to simulate transparency.
        TT_BLENDED,           //!< Use blending to render transparent objects.
        TT_SORTED_BLENDED,    //!< Sort transparent objects and use blending to render them.
        TT_ORDER_INDEPENDENT  //!< Use the "Order Independent Transparency" approach to render transparent objects.
      };          //!< Technique for rendering transparent objects.

      enum OITTechnique
      {
        OT_TEXTURE,   //!< Use textures for OIT.
        OT_PBUFFER,   //!< Use PBuffers for OIT.
        OT_FBO        //!< Use FBOs for OIT.
      };

      //! Get the current geometry caching technique.
      //** \return The currently used geometry caching technique. */
      NVSG_API GeometryCacheTechnique getGeometryCacheTechnique( void ) const;

      //! Test on supported geometry caching techniques.
      /** Use this method to test if this special geometry caching technique is supported.
        * \return true - supported, false - not supported.*/
      NVSG_API virtual bool isGeometryCacheTechniqueSupported( GeometryCacheTechnique gct  //! GeometryCache technique.
                                                              ) const;

      //! Set the geometry caching technique the render should use.
      NVSG_API virtual void setGeometryCacheTechnique( GeometryCacheTechnique gct          //! GeometryCache technique.
                                                      );

      //! Get the current jitter technique.
      /** \return The currently used technique.*/
      NVSG_API JitterTechnique getJitterTechnique( void ) const;

      //! Test on supported jitter techniques.
      /** Use this method to test if this special jitter technique is currently available.
        * \return true - supported, false - not supported */
      NVSG_API virtual bool isJitterTechniqueSupported( JitterTechnique jt               //!< Jitter technique.
                                                      ) const;

      //! Set the jitter technique the render should use.
      NVSG_API virtual void setJitterTechnique( JitterTechnique jt                       //!< Jitter technique
                                              );

      //! Get the front face culling state.
      /** The front face culling state of the RenderTraverser can overwrite any front face culling
        * state in a FaceAttribute. */
      NVSG_API bool isFrontFaceCulling() const;

      //! Get the back face culling state.
      /** The back face culling state of the RenderTraverser can overwrite any back face culling
        * state in a FaceAttribute. */
      NVSG_API bool isBackFaceCulling() const;

      //! Set the face culling states.
      /** The face culling states of the RenderTraverser can overwrite any face culling states in
        * a FaceAttribute. */
      NVSG_API void setFaceCulling( bool front, bool back );

      //! Ask if the face culling states of the RenderTraverser overwrites the face culling states of a FaceAttribute.
      NVSG_API bool isFaceCullingOverwrite() const;

      //! Set the face culling states of the RenderTraverser to overwrite the face culling states of a FaceAttribute.
      NVSG_API void setFaceCullingOverwrite( bool overwrite );

      //! Get the FaceMode to use for faces.
      /** The FaceMode of the RenderTraverser can overwrite any FaceMode in a FaceAttribute. */
      NVSG_API nvsg::FaceMode getFaceMode( void ) const;

      //! Set the FaceMode to use for faces.
      /** The FaceMode of the RenderTraverser can overwrite any FaceMode in a FaceAttribute. */
      NVSG_API void setFaceMode( nvsg::FaceMode mode );

      //! Ask if the FaceMode of the RenderTraverser overwrites the FaceMode of a FaceAttribute.
      NVSG_API bool isFaceModeOverwrite() const;

      //! Set if the FaceMode of the RenderTraverser overwrites the FaceMode of a FaceAttribute.
      NVSG_API void setFaceModeOverwrite( bool overwrite );

      //! Ask if two sided lighting is on.
      /** The two sided lighting state of the RenderTraverser can overwrite and FaceAttribute. */
      NVSG_API bool isTwoSidedLighting() const;

      //! Set the two sided lighting state.
      /** The two sided lighting state of the RenderTraverser can overwrite and FaceAttribute. */
      NVSG_API void setTwoSidedLighting( bool enable );

      //! Ask if the two-sided lighting state of the RenderTraverser overwrite the state of a FaceAttribute. */
      NVSG_API bool isTwoSidedLightingOverwrite() const;

      //! Set if the two-sided lighting state of the RenderTraverser overwrites the state of a FaceAttribute. */
      NVSG_API void setTwoSidedLightingOverwrite( bool overwrite );

      //! Get the current render technique.
      //** \return The currently used render technique. */
      NVSG_API RenderTechnique getRenderTechnique( void ) const;

      //! Test on supported render techniques.
      /** Use this technique to test if this special render technique is currently available.
        * \return true - supported, false - not supported.*/
      NVSG_API virtual bool isRenderTechniqueSupported( RenderTechnique rt               //! Render technique.
                                                      ) const;

      //! Set the render technique the render should use.
      NVSG_API void setRenderTechnique( RenderTechnique rt                       //! Render technique.
                                      );
      
      //! Get the current shadowing technique.
      /** \return the currently used shadowing technique. */
      NVSG_API ShadowTechnique getShadowTechnique( void ) const;

      //! Test on supported shadowing techniques.
      /** Use this to test if this special shadow technique is currently available.
        * \return true - supported, false - not supported. */
      NVSG_API virtual bool isShadowTechniqueSupported( ShadowTechnique st  //! shadow technique to test
                                                      ) const;

      //! Set the shadow technique to use.
      /** \return true - everything went fine, false - not set */
      NVSG_API virtual void setShadowTechnique( ShadowTechnique st          //! shadow technique to use
                                              );

      //! Get the size of the shadow maps.
      /** \returns the size of the shadow maps. */
      NVSG_API size_t getShadowMapSize( void ) const;

      //! Set the size of the shadow maps.
      /** The size must be a power of two. The default size is 1024. */
      NVSG_API virtual void setShadowMapSize( size_t size                   //! size of shadow maps
                                            );

      //! Get the shadow map intensity.
      /** The shadow map intensity describes the depth of the shadows. The intensity is between 0.0 and 1.0. With intensity
        * 0.0 there are no shadows at all. With intensity 1.0 the shadows are completely black. With an intensity in between
        * the darkness (or depth) of the shadows varies linear. The default is 0.9, resulting in a rather dark shadow. */
      NVSG_API float getShadowMapIntensity( void ) const;

      //! Set the shadow map intensity.
      /** The shadow map intensity has to be between 0.0 and 1.0 (inclusive). With intensity 0.0 there are no shadows at all.
        * With intensity 1.0 the shadows are completely black. With an intensity in between the darkness (or depth) of the
        * shadows varies linear. */
      NVSG_API void setShadowMapIntensity( float intensity );

      //! Get the texture size limit.
      /** Textures are scaled down to that limit if they are larger.
        * \returns the current texture size limit. */
      NVSG_API size_t getTextureSizeLimit( void ) const;

      //! Set the texture size limit.
      /** Textures are scaled down to that limit if they are larger.
        * \note Setting the size to 0 (zero), disables texture size limit. */
      NVSG_API virtual void setTextureSizeLimit( size_t size   //!<  texture size limit
                                              );

      //! Get the current transparency technique.
      //** \return The currently used transparency technique. */
      NVSG_API TransparencyTechnique getTransparencyTechnique( void ) const;

      //! Test on supported transparency techniques.
      /** Use this technique to test if this special transparency technique is currently available.
        * \return true - supported, false - not supported. */
      NVSG_API virtual bool isTransparencyTechniqueSupported( TransparencyTechnique tt   //! Transparency technique.
                                                            ) const;

      //! Set the transparency technique the render should use.
      NVSG_API virtual void setTransparencyTechnique( TransparencyTechnique tt           //! Transparency technique.
                                                    );
      
      //! Get the current OIT (Order Independent Transparency) technique.
      //** \return The currently used OIT technique. */
      NVSG_API OITTechnique getOITTechnique( void ) const;

      //! Test on supported OIT techniques.
      /** Use this technique to test if this special OIT technique is currently available.
        * \return true - supported, false - not supported.*/
      NVSG_API virtual bool isOITTechniqueSupported( OITTechnique ot                     //! OIT technique 
                                                    ) const;

      //! Set the OIT technique the render should use.
      /** \return true - everything went fine, false - not set */
      NVSG_API virtual void setOITTechnique( OITTechnique ot                             //! OIT technique 
                                            );

      //! Get maximum OIT passes.
      /** \return Maximum rendering passes to archive the desired effect. */ 
      NVSG_API size_t getOITMaxPasses( void ) const;

      //! Set the number of maximum passes to achieve the desired transparency effect.
      NVSG_API virtual void setOITMaxPasses( size_t maxPasses                            //! Maximum passes to render, even when threshold is not reached.

                                            );

      //! Get the current threshold.
      /** \return threshold [0,1]*/
      NVSG_API float getOITThreshold( void ) const;

      //! Set the threshold.
      /** The threshold tells the refinement step of the OIT algorithm if it needs 
        * another pass or if it is good enough to show the rendered result.
        * The value lies between 0 and 1 (0%-100%). The refinement step compares 
        * the percentage of drawn pixels with the threshold and stops if it is 
        * smaller than the user defined value. */
      NVSG_API void setOITThreshold( float threshold  //!< Threshold [0,1]
                                            );

      //! Set the anti-aliasing mode for wire frame drawings.
      NVSG_API void setLineAntiAliasing( bool onOff    //!<  enables/disables line anti-aliasing
                                       );

      //! Test if the line anti-aliasing is turned on / off.
      /** \returns \c true, if line anti-aliasing is on, otherwise \c false. */
      NVSG_API bool isLineAntiAliasingEnabled( void ) const;


      //! Get the line width for wire frame drawings.
      /** \returns the line width for wire frame drawings. */
      NVSG_API float getLineWidth( void ) const;

      //! Set the line width for wire frame drawings.
      NVSG_API void setLineWidth( float width          //!<  line width to set
                                );

      //! Set the anti-aliasing mode for point mode drawings.
      NVSG_API void setPointAntiAliasing( bool onOff   //!<  enables/disables point anti-aliasing
                                        );

      //! Get the anti-aliasing mode for point mode drawings.
      /** \returns \c true, if point anti-aliasing is on, otherwise \c false. */
      NVSG_API bool isPointAntiAliasingEnabled( void ) const;

      //! Get the point size for point mode drawings.
      /** \returns the point size for point mode drawings. */
      NVSG_API float getPointSize( void ) const;

      //! Set the point size for point mode drawings.
      NVSG_API void setPointSize( float size           //!<  point size to set
                                );

      //! Set the sorting policy
      /** Opaque elements can be sorted by a number of policies. Currently you can switch off
        * any sorting, sort them by states, thus minimizing state switches which can hurt
        * performance, or you can sort by depth from front to back. Sorting by depth is highly
        * recommended if you also enable occlusion culling. */
      NVSG_API void setSortingPolicy( SortingPolicy sp  //!< sorting policy to use
                                    );

      //! Get the current sorting policy.
      /** \returns The current sorting policy. */
      NVSG_API SortingPolicy getSortingPolicy( void ) const;

      //! Test on occlusion culling support.
      /** Any render traverser supporting occlusion culling, should return \c true here.
        * \returns The base implementation returns \c false. */
      NVSG_API virtual bool isOcclusionCullingSupported( void ) const;

      //! Set occlusion Culling.
      NVSG_API virtual void setOcclusionCulling( bool onOff   //!< enables/disables occlusion culling
                                               );

      //! Test if occlusion culling is on.
      /** \returns \c true, if occlusion culling is on, otherwise \c false. */
      NVSG_API bool isOcclusionCulling( void ) const;

    protected:
      //! Default constructor
      NVSG_API RenderTraverser(void);
      //! Default destructor
      NVSG_API virtual ~RenderTraverser(void);

      //! Initiate traversing a given scene graph.
      /** See \link nvtraverser::Traverser::doApply nvtraverser::Traverser::doApply \endlink 
        * for more information.
        * \note All RenderTraverser based classes need a valid \link nvsg::ViewState nvsg::ViewState \endlink.
        * If the ViewState is not valid the runtime behavior is not defined. Call setViewState() 
        * with a valid ViewState prior calling apply().*/
      NVSG_API virtual void doApply( nvsg::NodeHandle * root );

      //! Initiate rendering the prepared scene.
      NVSG_API virtual void doRender( nvsg::NodeHandle * root ) = 0;

      //! Traverse a Billboard
      /** Store \a p as the current transform and continue traversal with
        * ModelViewTraverser::handleBillboard. */
      NVSG_API virtual void handleBillboard( const nvsg::Billboard * p );

      //! Traverse a StateVariant.
      /** Stores some informations of this StateVariant as current. Because it is not necessary to further traverse down
        * the tree, ModelViewTraverser::handleStateVariant is not called here. */
      NVSG_API virtual void handleStateVariant( const nvsg::StateVariant * p  //!< StateVariant to traverse
                                              );

      //! Traverse a Billboard
      /** Store \a p as the current transform and continue traversal with 
        * ModelViewTraverser::handleTranform. */
      NVSG_API virtual void handleTransform( const nvsg::Transform * p );

      //! Traverse a VolumeNode.
      /** Sets the "volumeSize" of the volume CgFx, traverses using
        * SharedModelTraverser::handleVolumeNode(), and handles highlighting of the VolumeNode if
        * the level to display is not the top level (0). */
      NVSG_API virtual void handleVolumeNode( const nvsg::VolumeNode * p );

      //! Prepares render list setup for coherent Drawables
      NVSG_API virtual bool preTraverseDrawables( const nvsg::GeoNode *p //!< The GeoNode which owns the Drawables.
                                                , size_t i //!< Geometry index to the coherent Drawables.
                                                );

      //! Does post-processing after coherent Drawables have been compiled into a render list. */
      NVSG_API virtual void postTraverseDrawables( const nvsg::GeoNode *p //!< The GeoNode which owns the Drawables.
                                                 , size_t i //!< Geometry index to the coherent Drawables. 
                                                 );
      //! Handle a Group before traversing it.
      /** The information on currently active lights is stored, then ModelViewTraverser::preTraverseGroup() is called
        * for further pre traversal processing. */
      NVSG_API virtual bool preTraverseGroup( const nvsg::Group *p    //!<  Group to handle pre-traversal
        );

      //! Handle a Group after traversing it.
      /** After calling ModelViewTraverser::postTraverseGroup() for other post traversal processing on that Group, the
        * information on the currently active lights is restored from what was current before traversing that Group. */
      NVSG_API virtual void postTraverseGroup( const nvsg::Group *p   //!<  Group to handle post-traversal
                                             );

      //! Handle a Transform before traversing it.
      /** The information on the current transform is stored, then ModelViewTraverser::preTraverseTransform() is called
        * for further pre traversal processing. */
      NVSG_API virtual bool preTraverseTransform( const nvmath::Trafo *p    //!< Trafo of node to handle pre-traversal
                                                );

      //! Handle a Transform after traversing it.
      /** After calling ModelViewTraverser::postTraverseTransform() for other post traversal processing on that Transform,
        * transformation informations are restored from what was current before traversing that Transform. */
      NVSG_API virtual void postTraverseTransform( const nvmath::Trafo *p   //!< Trafo of node to handle post-traversal
                                                 );

      //! Handle a LightSource on traversal.
      /** The currently active lights are extended by this LightSource, then ModelViewTraverser::traverseLight() is
        * called for further traversal. */
      NVSG_API virtual void traverseLight( const nvsg::LightSource * p    //!<  LightSource to traverse
                                         );

      //! Handle a PrimitiveSet on traversal.
      /** Depending on a variety of transparency informations, that PrimitiveSet, together with informations on the currently
      * active lights, StateSet, and transformations is stored in either the transparent or the opaque render list.
      * Then ModelViewTraverser::traversePrimitiveSet() is called for further traversal. */
      NVSG_API virtual void traversePrimitiveSet( const nvsg::PrimitiveSet * p    //!<  PrimitiveSet to traverse
                                                );

      //! handleMapArray override for specialized handling of MapArray.
      NVSG_API virtual void handleMapArray( const nvsg::MapArray * p );

      //! preTraverseMap override for specialized handling of MapArray per Map.
      NVSG_API virtual void preTraverseMap( const nvsg::MapArray * p, size_t index );

      //! handleMapElement override for specialized handling of MapElement.
      NVSG_API virtual void handleMapElement( const nvsg::MapElement * p );

      //! handleMapMatrix override for specialized handling of MapMatrix.
      NVSG_API virtual void handleMapMatrix( const nvsg::MapMatrix * p );

      //! preTraverseMap override for specialized handling of MapMatrix per Map.
      NVSG_API virtual void preTraverseMap( const nvsg::MapMatrix * p, size_t col, size_t row );

      //! preTraverseMapColumn override for specialized handling of MapMatrix per column of Maps.
      NVSG_API virtual void preTraverseMapColumn( const nvsg::MapMatrix * p, size_t col );

      //! postTraverseMapColumn override for specialized handling of MapMatrix per column of Maps.
      NVSG_API virtual void postTraverseMapColumn( const nvsg::MapMatrix * p, size_t col );

      //! Determine if the rendering is in the overlay pass.
      /** \returns \c true if currently the overlay geometries are rendered, otherwise \c false. */
      NVSG_API bool isOverlayPass( void ) const;

      //! Determine if the rendering is in the transparent pass.
      /** \returns \c true if currently the transparent geometries are rendered, otherwise \c false. */
      NVSG_API bool isTransparentPass( void ) const;

      /*! \brief Initiates rendering of the built render lists.
      /*  \param cam The camera specifying the view to render.
       *  \remarks After RenderTraverser::doApply() returns, the render lists for opaque and 
       *  transparent geometries are entirely built and ready for being rendered. This function
       *  initiates the rendering from the passed camera's point of view. */
      NVSG_API virtual void render( const nvsg::Camera * cam );

      //! Begin the rendering process.
      /** Before rendering the first object, this function is called. */
      NVSG_API virtual void renderBegin( void );

      //! End the rendering process.
      /** After rendering the last object, this function is called. */
      NVSG_API virtual void renderEnd( void );

      //! Start rendering a general object.
      /** The function first waits to gain read access to the object pointed to by \a object.
      * After that, it calls the corresponding and overloadable render routine.  
      * After the render routine returns, the function releases the read access to the object. 
      * \note The behavior is undefined if \a p points to an invalid location. */
      void renderObject( nvsg::ObjectHandle * oh );

      //! Add a render routine for a concrete \link nvsg::Object Object \endlink derived object.
      /** The function registers the render routine specified by \a render to render a concrete Object that is 
      * explicitly identified by \a objectCode.
      *
      * A render routine must be a member function of a RenderTraverser-derived class. It must return void and expect a
      * pointer to the concrete object as the parameter. A render routine must not have a default argument! If a render
      * routine is intended to remain overloadable, it should be declared virtual.
      *
      * \note A possibly pre-registered render routine for the specified object will be overridden by the new renderer.
      *
      * \remarks For derived classes intended to provide new operations for known objects arranged in a scene graph,
      * it is strongly recommended not to add new render routines for a certain known objects but to overload the 
      * corresponding render routines. In this context, we refer to 'known objects' as objects that are known by the
      * RenderTraverser base class and for which a corresponding render routine is provided.
      *
      * However, \c addObjectRenderer is indispensable if newly invented objects, and hence, objects that are unknown by
      * the RenderTraverser base class, need to be considered for rendering. */
      template<typename RendererType>
        void addObjectRenderer( unsigned int objectCode //!< Object code to identify an object type at run-time.
        , RendererType renderer   //!< Specifies the address of the handler routine.
        );

      //! Start rendering a general object.
      /** The function first waits to gain read access to the object pointed to by \a object.
      * After that, it calls the corresponding and overloadable render routine with the additional argument \a arg.  
      * After the render routine returns, the function releases the read access to the object. 
      * \note The behavior is undefined if \a p points to an invalid location.
      * The behavior is also undefined if the additional argument doesn't match that of the corresponding render routine. */
      template <typename T>
        void renderObject( nvsg::ObjectHandle * oh, T arg );

      //! Render a QuadMeshes out of a render list.
      /** This base implementation is a NOP. */
      NVSG_API virtual void renderQuadMeshes( const nvsg::QuadMeshes *p   //!<  QuadMeshes to render
        );

      //! Render an AnimatedQuads out of a render list.
      /** This base implementation calls renderQuads(). */
      NVSG_API virtual void renderAnimatedQuads( const nvsg::AnimatedQuads *p   //!<  AnimatedQuads to render
                                               );

      //! Render an AnimatedTriangles out of a render list.
      /** This base implementation calls renderTriangles(). */
      NVSG_API virtual void renderAnimatedTriangles( const nvsg::AnimatedTriangles *p   //!<  AnimatedTriangles to render
                                                   );

      //! Render a BlendAttribute out of a render list.
      /** This base implementation is a NOP. */
      NVSG_API virtual void renderBlendAttribute( const nvsg::BlendAttribute *p //!<  BlendAttribute to render
                                                , bool enable                   //!<  enable (\c true) or disable (\c false) this BlendAttribute
                                                );

      //! Render a CgFx out of a render list.
      /** This base implementation determines if there is a current CgFx available now. */
      NVSG_API virtual void renderCgFx( const nvsg::CgFx *p   //!<  CgFx to render
                                      , bool enable           //!<  enable (\c true) or disable (\c false) this CgFx
                                      );

      //! Render a DepthAttribute out of a render list.
      /** This base implementation is a NOP. */
      NVSG_API virtual void renderDepthAttribute( const nvsg::DepthAttribute *p //!<  DepthAttribute to render
                                                , bool enable                   //!<  enable (\c true) or disable (\c false) this DepthAttribute
                                                );

      //! Render a DirectedLight out of a render list.
      /** This base implementation is a NOP. */
      NVSG_API virtual void renderDirectedLight( const nvsg::DirectedLight *p   //!<  DirectedLight to render
                                               , bool enable                    //!<  enable (\c true) or disable (\c false) this DirectedLight
                                               );

      //! Render a FaceAttribute out of a render list.
      /** This base implementation is a NOP. */
      NVSG_API virtual void renderFaceAttribute( const nvsg::FaceAttribute *p   //!<  FaceAttribute to render
                                               , bool enable                    //!<  enable (\c true) or disable (\c false) this FaceAttribute
                                               );

      //! Render a HighlightBox out of a render list.
      /** This base implementation is a NOP. */
      NVSG_API virtual void renderHighlightBox( const nvsg::HighlightBox * p    //!< HighlightBox to render
                                              );

      //! Render a HighlightEdges out of a render list.
      /** This base implementation is a NOP. */
      NVSG_API virtual void renderHighlightEdges( const nvsg::HighlightEdges * p  //!< HighlightEdges to render
                                                );

      //! Render a LineAttribute out of a render list.
      /** This base implementation is a NOP. */
      NVSG_API virtual void renderLineAttribute( const nvsg::LineAttribute *p   //!<  LineAttribute to render
                                               , bool enable                    //!<  enable (\c true) or disable (\c false) this PointAttribute
                                               );

      //! Render a Lines out of a render list.
      /** This base implementation is a NOP. */
      NVSG_API virtual void renderLines( const nvsg::Lines *p         //!<  Lines to render
                                       );

      //! Render a LineStrips out of a render list.
      /** This base implementation is a NOP. */
      NVSG_API virtual void renderLineStrips( const nvsg::LineStrips *p   //!< LineStrips to render
                                            );

      //! Render a PointAttribute out of a render list.
      /** This base implementation is a NOP. */
      NVSG_API virtual void renderPointAttribute( const nvsg::PointAttribute *p   //!<  PointAttribute to render
                                                , bool enable                    //!<  enable (\c true) or disable (\c false) this PointAttribute
                                                );

      //! Render a Points out of a render list.
      /** This base implementation is a NOP. */
      NVSG_API virtual void renderPoints( const nvsg::Points *p           //!< Points to render
                                        );

      //! Render a Material out of a render list.
      /** This base implementation is a NOP. */
      NVSG_API virtual void renderMaterial( const nvsg::Material *p   //!<  Material to render
                                          , bool enable               //!<  enable (\c true) or disable (\c false) this Material
                                          );

      //! Render a ParallelCamera out of a render list.
      /** This base implementation is a NOP. */
      NVSG_API virtual void renderParallelCamera( const nvsg::ParallelCamera *p   //!<  ParallelCamera to render
                                                );

      //! Render a PerspectiveCamera out of a render list.
      /** This base implementation is a NOP. */
      NVSG_API virtual void renderPerspectiveCamera( const nvsg::PerspectiveCamera *p   //!<  PerspectiveCamera to render
                                                   );

      //! Render a PointLight out of a render list.
      /** This base implementation is a NOP. */
      NVSG_API virtual void renderPointLight( const nvsg::PointLight *p   //!<  PointLight to render
                                            , bool enable                 //!<  enable (\c true) or disable (\c false) this PointLight
                                            );

      //! Render a Quads out of a render list.
      /** This base implementation is a NOP. */
      NVSG_API virtual void renderQuads( const nvsg::Quads *p   //!<  Quads to render
                                       );

      //! Render a QuadStrips out of a render list.
      /** This base implementation is a NOP. */
      NVSG_API virtual void renderQuadStrips( const nvsg::QuadStrips *p   //!<  QuadStrips to render
                                            );

      //! Render a SkinnedTriangles out of a render list.
      /** This base implementation calls renderTriangles(). */
      NVSG_API virtual void renderSkinnedTriangles( const nvsg::SkinnedTriangles *p   //!<  SkinnedTriangles to render
                                                  );

      //! Render a SpotLight out of a render list.
      /** This base implementation is a NOP. */
      NVSG_API virtual void renderSpotLight( const nvsg::SpotLight *p   //!<  SpotLight to render
                                           , bool enable                //!<  enable (\c true) or disable (\c false) this SpotLight
                                           );

      //! Render a StatePass out of a render list.
      /** First all currently enabled StateAttributes, that are not part of this StatePass are disabled. All
        * StateAttributes in this StatePass are enabled then. */
      NVSG_API virtual void renderStatePass( const nvsg::StatePass *p   //!<  StatePass to render
                                           );

      //! Render a TextureAttribute out of a render list.
      /** If this TextureAttribute is to be enabled, all its TextureAttributeItems are enabled. Otherwise, this base
        * implementation is a NOP. */
      NVSG_API virtual void renderTextureAttribute( const nvsg::TextureAttribute *p   //!<  TextureAttribute to render
                                                  , bool enable                       //!<  enable (\c true) or disable (\c false) this TextureAttribute
                                                  );

      //! Render a TextureAttributeItem out of a render list.
      /** This base implementation is a NOP. */
      NVSG_API virtual void renderTextureAttributeItem( const nvsg::TextureAttributeItem *p   //!<  TextureAttributeItem to render
                                                      , size_t tu                             //!<  texture unit to use
                                                      );

      //! Render a Triangles out of a render list.
      /** This base implementation is a NOP. */
      NVSG_API virtual void renderTriangles( const nvsg::Triangles *p   //!<  Triangles to render
                                           );

      //! Render a TriFans out of a render list.
      /** This base implementation is a NOP. */
      NVSG_API virtual void renderTriFans( const nvsg::TriFans *p       //!<  TriFans to render
                                         );

      //! Render a TriStrips out of a render list.
      /** This base implementation is a NOP. */
      NVSG_API virtual void renderTriStrips( const nvsg::TriStrips *p   //!<  TriStrips to render
                                           );

      /*! \brief Evaluate the Trafo of a Transform while rendering from a render list.
       *  \param p The read-locked Transform to evaluate.
       *  \remarks The framework maintains a list of transformations that might change on every
       *  frame (for example due to being part of a Billboard node). Those transforms are
       *  re-evaluated on each frame, and this function is used to do this. This function sets the
       *  member variable m_currentTrafo of the RenderTraverser to the Trafo of \a p.
       *  \sa renderBillboard */
      NVSG_API virtual void renderTransform( const nvsg::Transform * p );

      /*! \brief Evaluate the Trafo of a Billboard while rendering from a render list.
       *  \param p The read-locked Billboard to evaluate.
       *  \remarks The framework maintains a list of transformations that might change on every
       *  frame (for example due to being part of a Billboard node). Those transforms are
       *  re-evaluated on each frame, and this function is used to do this. This function sets the
       *  member variable m_currentTrafo of the RenderTraverser to the current Trafo of \a p.
       *  \sa setTransform */
      NVSG_API virtual void renderBillboard( const nvsg::Billboard * p );

      //! Set a clipping plane.
      /** This base implementation is a NOP. */
      NVSG_API virtual void setClipPlane( size_t index                    //!< index of the plane
                                        , const nvmath::Plane3f & plane   //!< plane to use
                                        , bool enable                     //!< enable/disable it
                                        );

      //! Set the current model to world and world to model transforms.
      /** This base implementation is a NOP. */
      NVSG_API virtual void setTransforms( const nvmath::Mat44f &modelToWorld   //!<  current model to world transform
                                         , const nvmath::Mat44f &worldToModel   //!<  current world to model transform
                                         , bool needsNormalize                  //!<  \c true if these transforms include a scaling
                                         , bool needsMirror                     //!<  \c true if these transforms includes a negative scaling
                                         );

      //! Indication of the beginning of the overlay parts.
      NVSG_API virtual void overlayBegin( void );

      //! Indication of the end of the overlay parts.
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
      /** If the function returns \c true, the PrimitiveSets will be re-processed again. */
      NVSG_API virtual bool primitiveSetsEnd();

      //! Get the current CgFx while rendering.
      /** \returns The current CgFx, or NULL if there is none.
        * \note The current CgFx is valid only while rendering (inside render()). */
      NVSG_API nvsg::CgFxHandle * getCurrentCgFx( void ) const;

      //! Assigns current settings of a DirectedLight to the specified effect light parameter.
      /** The purpose of this function is to transmit the settings of the specified DirectedLight
        * to the specified effect light through the CgFxEffect interface. 
        *
        * NVSG effect light is always expected to be in world space. The model-to-world transformation 
        * passed as last argument specifies how the light source is oriented in world space given its 
        * inherent orientation in model space.
        *
        * The orientation of coordinate systems is dependent on the graphic library system used 
        * for rendering. Therefore, the default implementation does nothing here and it is
        * recommended for a derived RenderTraverser to override this function to get the effect light
        * oriented in the right way.
        *
        * The behavior is undefined if either \a light does not specify an effect light parameter, or if
        * \a light is no parameter of the specified effect. */
      NVSG_API virtual void assignDirectedLightToEffectLight( nvsg::CgFxEffect * effect
                                                            , nvsg::CgFxParameter lightParam 
                                                            , const nvsg::DirectedLight * directedLight
                                                            , const nvmath::Mat44f& modelToWorld
                                                            ); 

      //! Assigns current settings of a PointLight to the specified effect light parameter.
      /** The purpose of this function is to transmit the settings of the specified PointLight
        * to the specified effect light through the CgFxEffect interface. 
        *
        * NVSG effect light is always expected to be in world space. The model-to-world transformation 
        * passed as last argument specifies how the light source is positioned in world space given its 
        * inherent position in model space.
        *
        * The orientation of coordinate systems is dependent on the graphic library system used 
        * for rendering. Therefore, the default implementation does nothing here and it is
        * recommended for a derived RenderTraverser to override this function to get the effect light
        * positioned in the right way.
        *
        * The behavior is undefined if either \a light does not specify an effect light parameter, or if
        * \a light is no parameter of the specified effect. */
      NVSG_API virtual void assignPointLightToEffectLight( nvsg::CgFxEffect * effect
                                                         , nvsg::CgFxParameter lightParam 
                                                         , const nvsg::PointLight * pointLight
                                                         , const nvmath::Mat44f& modelToWorld
                                                         ); 

      //! Assigns current settings of a SpotLight to the specified effect light parameter.
      /** The purpose of this function is to transmit the settings of the specified SpotLight
        * to the specified effect light through the CgFxEffect interface. 
        *
        * NVSG effect light is always expected to be in world space. The model-to-world transformation 
        * passed as last argument specifies how the light source is positioned and oriented in world 
        * space given its inherent position and orientation in model space.
        *
        * The orientation of coordinate systems is dependent on the graphic library system used 
        * for rendering. Therefore, the default implementation does nothing here and it is
        * recommended for a derived RenderTraverser to override this function to get the effect light
        * positioned in the right way.
        *
        * The behavior is undefined if either \a light does not specify an effect light parameter, or if
        * \a light is no parameter of the specified effect. */
      NVSG_API virtual void assignSpotLightToEffectLight( nvsg::CgFxEffect * effect
                                                        , nvsg::CgFxParameter lightParam 
                                                        , const nvsg::SpotLight * spotLight
                                                        , const nvmath::Mat44f& modelToWorld
                                                        ); 

      //! Assigns current settings of a custom LightSource to the specified effect light parameter.
      /** The purpose of this function is to transmit the settings of the specified LightSource
        * to the specified effect light through the CgFxEffect interface. 
        *
        * NVSG effect light is always expected to be in world space. The model-to-world transformation 
        * passed as last argument specifies how the light source is positioned and oriented in world 
        * space given its inherent position and orientation in model space.
        *
        * The orientation of coordinate systems is dependent on the graphic library system used 
        * for rendering. Therefore, the default implementation does nothing here and it is
        * recommended for a derived RenderTraverser to override this function to get the effect light
        * positioned in the right way.
        *
        * The behavior is undefined if either \a light does not specify an effect light parameter, or if
        * \a light is no parameter of the specified effect. */
      NVSG_API virtual void assignCustomLightToEffectLight( nvsg::CgFxEffect * effect
                                                          , nvsg::CgFxParameter lightParam 
                                                          , const nvsg::LightSource * customLight
                                                          , const nvmath::Mat44f& modelToWorld
                                                          ); 


      //! Create an occlusion culling ID.
      /** Any RenderTraverser that supports occlusion culling should overwrite this function.
        * It should return an ID to identify one block of occlusion culling, usually one set
        * of PrimitiveSets. This base implementation always returns 0, that is an invalid ID.
        * \returns new occlusion cull ID. */
      NVSG_API virtual unsigned int createOcclusionCullID( void );

      //! Destroy an occlusion culling ID.
      /** Any RenderTraverser that supports occlusion culling should overwrite this function.
        * It should destroy any information held with the occlusion culling ID \a id. \a id
        * was previously returned by createOcclusionCullID(). This base implementation is a NOP. */
      NVSG_API virtual void destroyOcclusionCullID( unsigned int id   //!< occlusion culling ID to destroy
                                                  );

      //! Begin an occlusion culling test.
      /** Any RenderTraverser that supports occlusion culling should overwrite this function.
        * This function is called before a block of drawing elements to be tested for
        * occlusion culling. The RenderTraverser should start to gather occlusion culling
        * informations for the occlusion culling ID \a id. This base implementation is a NOP. */
      NVSG_API virtual void beginOcclusionCullQuery( unsigned int id  //!< occlusion culling ID to begin with
                                                   );

      //! End an occlusion culling test.
      /** Any RenderTraverser that supports occlusion culling should overwrite this function.
        * This function is called after a block of drawing elements to be tested for occlusion
        * culling. The RenderTraverser should stop gathering occlusion culling informations
        * for the occlusion culling ID \a id. This base implementation is a NOP. */
      NVSG_API virtual void endOcclusionCullQuery( unsigned int id    //!< occlusion culling ID to end with
                                                 );

      //! Get the result of an occlusion culling test.
      /** Any RenderTraverser that supports occlusion culling should overwrite this function.
        * This function is called to determine the occlusion culling result with the occlusion
        * culling ID \a id. It should return the number of samples that were rendered between
        * the corresponding calls to beginOcclusionCullQuery and endOcclusionCullQuery. This base
        * implementation always returns a large number, indicating the tested drawing elements
        * would be visible.
        * \returns The number of samples drawn with the occlusion cull test for the ID \a id. */
      NVSG_API virtual unsigned int getOcclusionCullResult( unsigned int id   //!< occlusion culling ID to get result of
                                                    );

      //! Begin rendering of the hidden drawing elements.
      /** Any RenderTraverser that supports occlusion culling should overwrite this function.
        * The base implementation is a NOP. */
      NVSG_API virtual void hiddenBegin( void );

      //! End rendering of the hidden drawing elements.
      /** Any RenderTraverser that supports occlusion culling should overwrite this function.
        * The base implementation is a NOP. */
      NVSG_API virtual void hiddenEnd( void );

      //! Clears all render lists.
      NVSG_API void clearRenderLists( void );

      //! Get the position of the current Map.
      /** While traversing the MapList for rendering, the position of the Map to render
        * can be queried with this function. The position is calculated and updated in
        * RenderTraverser::preTraverseMap just before the handler routine for the Map is
        * called. The position is given in raster coordinates and represents the position
        * in the viewport where the lower left corner of the Map is located. */
      NVSG_API void getMapPosition( size_t &x, size_t &y );

    private:
      void renderList( std::list<RenderElement*> &rlist );
      void renderHiddenList( std::list<RenderElement*> &hlist );
      void renderHighlightList( std::list<HighlightElement*> &hlist );
      void renderElement( RenderElement * re );
      void renderPrimitiveSets(RenderElement* re);
      void renderHighlightObjects(HighlightElement* he);
      void renderStateAttributes(std::list<nvsg::StateAttributeHandle*>& alist, bool enable);
      void setClipPlanesElement(ClipPlanesElement *p);
      void setLightsElement(LightsElement *p);
      void setTransformElement(TransformElement *p);
      void traverseHeadLight( const nvsg::LightSource * p );

      // gathers all available effect lights in a plain vector
      size_t getEffectLights( nvsg::CgFxEffect * effect
                            , std::vector<nvsg::CgFxParameter>& lights) const;
      
      // reset lights[first] to lights[last-1]
      void resetEffectLights( nvsg::CgFxEffect * effect
                            , const std::vector<nvsg::CgFxParameter>& lights
                            , size_t first
                            , size_t last );
      
      // sets effect light values 
      void setEffectLights( nvsg::CgFxEffect * effect
                          , const std::vector<nvsg::CgFxParameter>& lights
                          , LightsElement * headLights
                          , LightsElement * currLights );

     // returns an object code to lookup the right render function for the given object.
     // returns OC_INVALID if no appropriate render function could be found for the object.
     unsigned int getObjectRenderCode(const nvsg::Object * obj);
      
     // update visible and hidden list by the occlusion test results
     void occlusionTest( std::list<RenderElement*> &visibleList, std::list<RenderElement*> &hiddenList );

     void clearRenderList( std::list<RenderElement*> &rlist );
     template <typename REType>
       void clearRenderList( std::list<REType*>& rlist );
     template <typename REType>
       void clearRenderVector( std::vector<REType*>& rlist );
     void updateTransformElement( TransformElement * te );

    private:

      enum 
      {
        // dirty bits - render list requires rebuild if one of these set
        // in addition the m_traverserBits needs to be evaluated for NVSG_TREE_DIRTY_MASK
        NVSG_GEOMETRY_CACHE_TECHNIQUE_CHANGED = BIT0
      , NVSG_TRANSPARENCY_TECHNIQUE_CHANGED   = BIT1
      , NVSG_RENDERLIST_INCOMPLETE            = BIT2
        // mask to check if at least one dirty bit is set
      , NVSG_RENDERLIST_DIRTY_MASK            = NVSG_GEOMETRY_CACHE_TECHNIQUE_CHANGED 
                                              | NVSG_TRANSPARENCY_TECHNIQUE_CHANGED 
                                              | NVSG_RENDERLIST_INCOMPLETE


        // render enable states and overwrites
        // face culling
      , NVSG_FACE_CULLING_BACK                = BIT3
      , NVSG_FACE_CULLING_FRONT               = BIT4
      , NVSG_FACE_CULLING_MASK                = NVSG_FACE_CULLING_BACK | NVSG_FACE_CULLING_FRONT
      , NVSG_FACE_CULLING_OVERWRITE           = BIT5
        // face mode
      , NVSG_FACE_MODE_OVERWRITE              = BIT6
        // two-sided lighting
      , NVSG_TWO_SIDED_LIGHTING               = BIT7
      , NVSG_TWO_SIDED_LIGHTING_OVERWRITE     = BIT8
        // anti aliasing
      , NVSG_LINE_ANTI_ALIASING               = BIT9
      , NVSG_POINT_ANTI_ALIASING              = BIT10 

        // render pass flavours
      , NVSG_OVERLAY_PASS                     = BIT11 // set if in overlay render pass
      , NVSG_TRANSPARENT_PASS                 = BIT12 // set if in transparent render pass

        // per-drawable appearance
      , NVSG_TRANSPARENT_DRAWABLE             = BIT13 // set if current drawable rendered 
                                                      // has transparent appearance

        // extended rendering
      , NVSG_OCCLUSION_CULLING                = BIT14 // occlusion culling enabled if set
      };
      unsigned int m_renderTraverserBits;

      nvsg::FaceMode  m_faceMode;
      float           m_lineWidth;
      float           m_pointSize;
      SortingPolicy   m_sortingPolicy;

      // used with state handling during rendering
      std::list<nvsg::StateAttributeHandle *> m_currentStateAttribs;

      std::stack<ClipPlanesElement *>   m_clipPlanesStack;
      ClipPlanesElement               * m_currentClipPlanes;
      const nvsg::Camera              * m_lockedCamera;
      nvsg::CgFxHandle                * m_currentCgFx;
      LightsElement                   * m_currentLights;
      nvsg::GroupHandle               * m_currentTransform;
      TransformElement                * m_currentTransformElement;
      bool                              m_currentTransformIsFixed;
      RenderElement                   * m_currentRenderElement;
      LightsElement                   * m_headLights;
      RenderElement                   * m_lastRenderElement;
      HighlightElement                * m_currentHighlightElement;
      ClipPlanesElement               * m_lastClipPlanesElement;
      LightsElement                   * m_lastLightsElement;
      TransformElement                * m_lastTransformElement;
      std::stack<nvmath::Vec3f>         m_currentScaling;
      const nvsg::StatePass           * m_currentStatePass;
      nvmath::Trafo                     m_currentTrafo;
      nvsg::StatePassHandle           * m_nullStatePass;
      nvsg::StateVariantHandle        * m_currentStateVariant;
      nvsg::StateVariantHandle        * m_lastStateVariant;
      std::stack<const nvsg::Group*>    m_groupStack;
      RenderElementPool               * m_renderElementPool;
      std::stack<LightsElement *>       m_renderLightsStack;
      std::list<HighlightElement *>     m_highlightList;
      std::list<RenderElement *>        m_overlayList;
      std::stack<bool>                  m_currentOverlay;
      std::stack<bool>                  m_currentCullingPrevented;
      bool                              m_lastCullingPrevented;
      std::list<RenderElement *>        m_visibleOpaqueList;
      std::list<RenderElement *>        m_hiddenOpaqueList;
      std::list<RenderElement *>        m_visibleTransparentList;
      std::list<RenderElement *>        m_hiddenTransparentList;
      std::stack<TransformElement *>    m_renderTransformStack;
      std::vector<TransformElement *>   m_dynamicTransformElements;
      GeometryCacheTechnique            m_geometryCacheTechnique;
      JitterTechnique                   m_jitterTechnique;
      RenderTechnique                   m_renderTechnique;
      ShadowTechnique                   m_shadowTechnique;
      size_t                            m_shadowMapSize;
      float                             m_shadowMapIntensity;
      size_t                            m_textureSizeLimit;
      TransparencyTechnique             m_transparencyTechnique;
      OITTechnique                      m_oitTechnique;
      size_t                            m_oitMaxPasses;
      float                             m_oitThreshold;
      
      // light tracking
      size_t                            m_currentNumLights;
      size_t                            m_lastNumLights;

      //  map handling
      size_t                            m_mapAccSize[2];
      int                               m_mapAnchor[2];
      size_t                            m_mapMaxRows;
      int                               m_mapPos[2];
      size_t                            m_mapSize[2];

  #if !defined(NDEBUG)
      bool                              m_isRendering;
      bool                              m_isTraversing;
  #endif

      nvutil::MemFunTbl<RenderTraverser> * m_mrftbl; // member render function table
  };

  template<typename RendererType>
    inline void RenderTraverser::addObjectRenderer( unsigned int objectCode, RendererType renderer )
  {
    m_mrftbl->addEntry( objectCode, renderer ); 
  }

  inline unsigned int RenderTraverser::getObjectRenderCode(const nvsg::Object * obj)
  {
    NVSG_TRACE();

    unsigned int oc = obj->getObjectCode();
    NVSG_ASSERT(oc!=nvsg::OC_INVALID);

    if ( !m_mrftbl->testEntry(oc) )
    {
      unsigned int orgOC = oc;
      do
      { // came across an unknown object
        // move up the object's hierarchy to find an appropriate handler
        oc = obj->getHigherLevelObjectCode(oc);
        if ( nvsg::OC_INVALID==oc )
        { // proceed immediately without handling the object
          return oc; 
        }
      }
      while ( !m_mrftbl->testEntry(oc) );
      
      // found an appropriate renderer if we get here
      
      // don't loop again for this object - register the handler
      addObjectRenderer(orgOC, (*m_mrftbl)[oc]);
    }
    return oc;
  }

  inline void RenderTraverser::renderObject( nvsg::ObjectHandle * oh )
  {
    NVSG_TRACE();

    if ( m_interrupt )
    {
      // there was an interrupt request.
      // immediately quit rendering
      m_traverserBits |= NVSG_TRAVERSAL_INTERRUPTED;
      return;
    }
    
    // share-lock the object while being rendered
    const nvsg::Object * obj = oh->beginRead();
    
    unsigned int oc = getObjectRenderCode( obj );
    if ( nvsg::OC_INVALID != oc ) 
    {

#if !defined(_WIN64) && defined(_WIN32) && !defined(NDEBUG)
      void * __esp__0;
      void * __esp__1;
      __asm mov __esp__0, esp
#endif
      (this->*(void (RenderTraverser::*)(const nvsg::Object*))(*m_mrftbl)[oc])(obj);
#if !defined(_WIN64) && defined(_WIN32) && !defined(NDEBUG)
      __asm mov __esp__1, esp
        NVSG_ASSERT(__esp__1==__esp__0);
#endif
    }

    oh->endRead();
  }

  template <typename T>
    inline void RenderTraverser::renderObject( nvsg::ObjectHandle * oh, T arg )
  {
    NVSG_TRACE();

    if ( m_interrupt )
    {
      // there was an interrupt request.
      // immediately quit rendering
      m_traverserBits |= NVSG_TRAVERSAL_INTERRUPTED;
      return;
  }

    // share-lock the object while being rendered
    const nvsg::Object * obj = oh->beginRead();

    unsigned int oc = getObjectRenderCode( obj );
    if ( nvsg::OC_INVALID != oc ) 
    {
#if !defined(_WIN64) && defined(_WIN32) && !defined(NDEBUG)
      void * __esp__0;
      void * __esp__1;
      __asm mov __esp__0, esp
#endif
      (this->*(void (RenderTraverser::*)(const nvsg::Object*, T))(*m_mrftbl)[oc])(obj, arg);
#if !defined(_WIN64) && defined(_WIN32) && !defined(NDEBUG)
      __asm mov __esp__1, esp
        NVSG_ASSERT(__esp__1==__esp__0);
#endif
    }

    oh->endRead();
  }

  inline bool RenderTraverser::isOverlayPass( void ) const
  {
    return !!(m_renderTraverserBits  & NVSG_OVERLAY_PASS);
  }

  inline bool RenderTraverser::isTransparentPass( void ) const
  {
    return !!(m_renderTraverserBits  & NVSG_TRANSPARENT_PASS);
  }

  inline RenderTraverser::GeometryCacheTechnique RenderTraverser::getGeometryCacheTechnique( void ) const
  {
    return( m_geometryCacheTechnique );
  }

  inline RenderTraverser::JitterTechnique RenderTraverser::getJitterTechnique( void ) const
  {
    return( m_jitterTechnique );
  }

  inline bool RenderTraverser::isBackFaceCulling() const
  {
    return !!(m_renderTraverserBits & NVSG_FACE_CULLING_BACK);
  }

  inline bool RenderTraverser::isFrontFaceCulling() const
  {
    return !!(m_renderTraverserBits & NVSG_FACE_CULLING_FRONT);
  }

  inline void RenderTraverser::setFaceCulling( bool front, bool back )
  {
    // reset both
    m_renderTraverserBits &= ~(NVSG_FACE_CULLING_MASK);
    // conditional set
    if ( front )
    {
      m_renderTraverserBits |= NVSG_FACE_CULLING_FRONT;
    }
    if ( back )
    {
      m_renderTraverserBits |= NVSG_FACE_CULLING_BACK;
    }
  }

  inline bool RenderTraverser::isFaceCullingOverwrite() const
  {
    return !!(m_renderTraverserBits & NVSG_FACE_CULLING_OVERWRITE);
  }

  inline void RenderTraverser::setFaceCullingOverwrite( bool overwrite )
  {
    // reset
    m_renderTraverserBits &= ~NVSG_FACE_CULLING_OVERWRITE;
    // conditional set
    if ( overwrite )
    {
      m_renderTraverserBits |= NVSG_FACE_CULLING_OVERWRITE;
    }
  }

  inline nvsg::FaceMode RenderTraverser::getFaceMode( void ) const
  {
    return( m_faceMode );
  }

  inline void RenderTraverser::setFaceMode( nvsg::FaceMode mode )
  {
    m_faceMode = mode;
  }

  inline bool RenderTraverser::isFaceModeOverwrite() const
  {
    return !!(m_renderTraverserBits & NVSG_FACE_MODE_OVERWRITE);
  }

  inline void RenderTraverser::setFaceModeOverwrite( bool overwrite )
  {
    // reset
    m_renderTraverserBits &= ~NVSG_FACE_MODE_OVERWRITE;
    // conditional set
    if ( overwrite )
    {
      m_renderTraverserBits |= NVSG_FACE_MODE_OVERWRITE;
    }
  }

  inline bool RenderTraverser::isTwoSidedLighting() const
  {
    return !!(m_renderTraverserBits & NVSG_TWO_SIDED_LIGHTING);
  }

  inline void RenderTraverser::setTwoSidedLighting( bool enable )
  {
    // reset
    m_renderTraverserBits &= ~NVSG_TWO_SIDED_LIGHTING;
    // conditional set
    if ( enable )
    {
      m_renderTraverserBits |= NVSG_TWO_SIDED_LIGHTING;
    }
  }

  inline bool RenderTraverser::isTwoSidedLightingOverwrite() const
  {
    return !!(m_renderTraverserBits & NVSG_TWO_SIDED_LIGHTING_OVERWRITE);
  }

  inline void RenderTraverser::setTwoSidedLightingOverwrite( bool overwrite )
  {
    // reset
    m_renderTraverserBits &= ~NVSG_TWO_SIDED_LIGHTING_OVERWRITE;
    // conditional set
    if ( overwrite )
    {
      m_renderTraverserBits |= NVSG_TWO_SIDED_LIGHTING_OVERWRITE;
    }
  }

  inline RenderTraverser::RenderTechnique RenderTraverser::getRenderTechnique( void ) const
  {
    return( m_renderTechnique );
  }

  inline RenderTraverser::ShadowTechnique RenderTraverser::getShadowTechnique( void ) const
  {
    return( m_shadowTechnique );
  }

  inline size_t RenderTraverser::getShadowMapSize( void ) const
  {
    return( m_shadowMapSize );
  }

  inline float RenderTraverser::getShadowMapIntensity( void ) const
  {
    return( m_shadowMapIntensity );
  }

  inline void RenderTraverser::setShadowMapIntensity( float intensity )
  {
    NVSG_ASSERT( ( 0.0f <= intensity ) && ( intensity <= 1.0f ) );
    m_shadowMapIntensity = intensity;
  }

  inline size_t RenderTraverser::getTextureSizeLimit( void ) const
  {
    return( m_textureSizeLimit );
  }

  inline RenderTraverser::TransparencyTechnique RenderTraverser::getTransparencyTechnique( void ) const
  {
    return( m_transparencyTechnique );
  }

  inline RenderTraverser::OITTechnique RenderTraverser::getOITTechnique( void ) const
  {
    return( m_oitTechnique );
  }

  inline size_t RenderTraverser::getOITMaxPasses( void ) const
  {
    return( m_oitMaxPasses );
  }

  inline float RenderTraverser::getOITThreshold( void ) const
  {
    return( m_oitThreshold );
  }

  inline void RenderTraverser::setOITThreshold( float threshold )
  {
    NVSG_ASSERT( ( 0.0f <= threshold ) && ( threshold <= 1.0f ) );
    m_oitThreshold = threshold;
  }

  inline nvsg::CgFxHandle * RenderTraverser::getCurrentCgFx( void ) const
  {
    NVSG_ASSERT( m_isRendering );
    return( m_currentCgFx );
  }

  inline void RenderTraverser::setLineAntiAliasing( bool onOff )
  {
    // reset
    m_renderTraverserBits &= ~NVSG_LINE_ANTI_ALIASING;
    if ( onOff )
    {
      m_renderTraverserBits |= NVSG_LINE_ANTI_ALIASING;
    }
  }

  inline bool RenderTraverser::isLineAntiAliasingEnabled() const
  {
    return !!(m_renderTraverserBits & NVSG_LINE_ANTI_ALIASING);
  }

  inline float RenderTraverser::getLineWidth( void ) const
  {
    return( m_lineWidth );
  }

  inline void RenderTraverser::setLineWidth( float width )
  {
    m_lineWidth = width;
  }

  inline void RenderTraverser::setPointAntiAliasing( bool onOff )
  {
    // reset
    m_renderTraverserBits &= ~NVSG_POINT_ANTI_ALIASING;
    if ( onOff )
    {
      m_renderTraverserBits |= NVSG_POINT_ANTI_ALIASING;
    }
  }

  inline bool RenderTraverser::isPointAntiAliasingEnabled() const
  {
    return !!(m_renderTraverserBits & NVSG_POINT_ANTI_ALIASING);
  }

  inline float RenderTraverser::getPointSize( void ) const
  {
    return( m_pointSize );
  }

  inline void RenderTraverser::setPointSize( float size )
  {
    m_pointSize = size;
  }

  inline void RenderTraverser::setSortingPolicy( SortingPolicy sp )
  {
    m_sortingPolicy = sp;
  }

  inline RenderTraverser::SortingPolicy RenderTraverser::getSortingPolicy( void ) const
  {
    return( m_sortingPolicy );
  }

  inline bool RenderTraverser::isOcclusionCulling( void ) const
  {
    return !!(m_renderTraverserBits & NVSG_OCCLUSION_CULLING);
  }

  inline void RenderTraverser::getMapPosition( size_t &x, size_t &y )
  {
    x = m_mapPos[0];
    y = m_mapPos[1];
  }
}
