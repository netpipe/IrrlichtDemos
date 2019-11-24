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
#include "nvsg/ViewState.h"
#include "nvtraverser/ModelViewTraverser.h"

namespace nvtraverser {

//! AppTraverser Class
/** This class provides the mechanism to apply changes and update the scene graph. 
  * (The 'apply' step of the application.) Use an AppTraverser derived class as a unique 
  * entry point for manipulating nodes in the scene, such as data from gadgets, 
  * animations, and interactions.
  * \note The current implementation takes care of the animation frames, camera clip planes
  * highlighting, etc...
  * \note Needs a valid ViewState. Call setViewState prior to apply().*/
class AppTraverser : public ExclusiveModelViewTraverser
{
public:
  //! Default constructor.
  NVSG_API AppTraverser();

protected:
  //!Default destructor.
  NVSG_API virtual ~AppTraverser();

  //! Determines if the AppTraverser really should traverse the tree.
  /** Depending on some internal data, the traversal is recognized to be necessary.
    * The AppTraverser needs to traverse if the ViewState, the Scene, or the animation
    * frame changed. */
  NVSG_API virtual bool needsTraversal( nvsg::NodeHandle * root ) const;

  //! doApply override
  NVSG_API virtual void doApply( nvsg::NodeHandle * root );

  // handler routine overrides
  NVSG_API virtual void handleGeoNode( nvsg::GeoNode *gnode );
  NVSG_API virtual void handleVolumeNode( nvsg::VolumeNode * vnode );
  NVSG_API virtual void handleAnimatedTransform( nvsg::AnimatedTransform *p );
  NVSG_API virtual void handleAnimatedQuads( nvsg::AnimatedQuads *quads );
  NVSG_API virtual void handleAnimatedTriangles( nvsg::AnimatedTriangles *triangles );
  NVSG_API virtual void handleSkinnedTriangles( nvsg::SkinnedTriangles *triangles );
  NVSG_API virtual void handleCgFx( nvsg::CgFx *cgfx );
  NVSG_API virtual void handleHighlightBox( nvsg::HighlightBox *p, nvsg::PrimitiveSet * primitive );

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

  // traverseLight override
  NVSG_API virtual void traverseLight( nvsg::LightSource *light );

  // data accessible for derived classes
  bool   m_bApplyingCgFx;  //!< Flag that signals if a CgFX effect needs to be handled (tangentspace calculation).
  size_t m_animationFrame; //!< Current animation frame.
  
private:
  // Some data to cache so we can decide if we really have to apply the animation data. 
  // This does not help to prevent from unneeded traversal steps since we can't 
  // decide for derived classes. 
  nvsg::ViewStateHandle     * m_pLastViewState;
  nvsg::NodeHandle          * m_pLastRoot;
  size_t                      m_lastAnimationFrame;
  bool                        m_bApplyAnimationKey;
  std::stack<nvmath::Vec3f>   m_currentScaling;
};

} // namespace nvtraverser
