// CRTTSkyBox.h
#ifndef __C_RTT_SKY_BOX_SCENE_NODE_H_INCLUDED__
#define __C_RTT_SKY_BOX_SCENE_NODE_H_INCLUDED__

#include "ISceneNode.h"
#include "ICameraSceneNode.h"
#include "S3DVertex.h"

namespace irr
{
namespace scene
{

   // Skybox, rendererd with zbuffer turned off, before all other nodes.
   class CRTTSkyBoxSceneNode : public ISceneNode
   {
   public:

      //! constructor
      CRTTSkyBoxSceneNode(core::dimension2d<u32> size, ISceneNode* parent, ISceneManager* mgr, s32 id);

      //! destructor
      virtual ~CRTTSkyBoxSceneNode();

      // renders the view from the said position to the skybox
      void renderToSkyBox(core::vector3df pos, video::SColor col=video::SColor(0,0,0,0), bool self=false);

      virtual void OnRegisterSceneNode();

      //! renders the node.
      virtual void render();

      //! returns the axis aligned bounding box of this node
      virtual const core::aabbox3d<f32>& getBoundingBox() const;

      //! returns the material based on the zero based index i. To get the amount
      //! of materials used by this scene node, use getMaterialCount().
      //! This function is needed for inserting the node into the scene hirachy on a
      //! optimal position for minimizing renderstate changes, but can also be used
      //! to directly modify the material of a scene node.
      virtual video::SMaterial& getMaterial(s32 i);

      //! returns amount of materials used by this scene node.
      virtual s32 getMaterialCount();

      scene::ICameraSceneNode *skyCam;

   private:

      core::aabbox3d<f32> Box;
      u16 Indices[6];
      video::S3DVertex Vertices[4*6];
      video::SMaterial Material[6];
      video::ITexture *rt[6];
   };

} // end namespace scene
} // end namespace irr

#endif
