#ifndef _CLOUDSCENENODE_H
#define _CLOUDSCENENODE_H

#include <irrlicht.h>

namespace irr
{
namespace scene
{

#define CLOUDSUBDIV 16

	class CCloudSceneNode : public scene::ISceneNode
	{
	public:
		// constructor
		CCloudSceneNode(scene::ISceneNode* parent, scene::ISceneManager* mgr, s32 id = -1);

	protected:
		// vertices and indices
		video::S3DVertex InnerVertices[CLOUDSUBDIV+1];
		u16 InnerIndices[CLOUDSUBDIV+2];
		video::S3DVertex OuterVertices[2*CLOUDSUBDIV];
		u16 OuterIndices[2*CLOUDSUBDIV+2];

		// scale of the cloud
		f32 TextureScale;

		// inner and outer radius
		f32 InnerRadius;
		f32 OuterRadius;

		// height of the cloud (center, inner and outer)
		f32 CenterHeight;
		f32 InnerHeight;
		f32 OuterHeight;

		// color of the cloud
		video::SColor CenterColor;
		video::SColor InnerColor;
		video::SColor OuterColor;

		// last update of the node
		u32 LastUpdate;

		// cloud translation
		core::vector2d<f32> Translation;
		// current translation (next translation step = current + elapsed time * Translation)
		core::vector2d<f32> CurrentTranslation;
		// Bounding box
		core::aabbox3d<f32> BBox;
		// material of the node
		video::SMaterial Material;

		// creates the vertices and indices used by the node
		void createCloudLayer();

	public:
		// typical OnRegisterSceneNode function
		virtual void OnRegisterSceneNode();
		// renders the node
		virtual void render();
		// returns the bounding box
		virtual const core::aabbox3d<f32>& getBoundingBox() const;
		// returns the node type
		virtual ESCENE_NODE_TYPE getType() const;
		// returns the material count
		virtual u32 getMaterialCount() const;
		// returns the material
		virtual video::SMaterial& getMaterial(u32 i);
		// clones the node
		virtual ISceneNode* clone(ISceneNode* newParent = 0, ISceneManager* newManager = 0);
		// animates the node
		virtual void OnAnimate(u32 timeMs);

		// writes attributes of the scene node.
		virtual void serializeAttributes(io::IAttributes* out, io::SAttributeReadWriteOptions* options = 0) const;

		// reads attributes of the scene node.
		virtual void deserializeAttributes(io::IAttributes* in,	io::SAttributeReadWriteOptions* options = 0);

		// returns the translation of the node
		virtual core::vector2d<f32>& getTranslation() { return Translation; }
		// sets the translation of the node
		virtual void setTranslation(core::vector2df translation);

		// returns the scale of the cloud texture
		virtual f32 getTextureScale() { return TextureScale; }
		// sets the scale of the cloud texture
		virtual void setTextureScale(f32 textureScale);

		// returns the inner radius
		virtual f32 getInnerRadius() { return InnerRadius; }
		// returns the outer radius
		virtual f32 getOuterRadius() { return OuterRadius; }

		// returns the center height
		virtual f32 getCenterHeight() { return CenterHeight; }
		// returns the inner height
		virtual f32 getInnerHeight() { return InnerHeight; }
		// returns the outer height
		virtual f32 getOuterHeight() { return OuterHeight; }

		// returns the center color
		virtual video::SColor& getCenterColor() { return CenterColor; }
		// returns the inner color
		virtual video::SColor& getInnerColor() { return InnerColor; }
		// returns the outer color
		virtual video::SColor& getOuterColor() { return OuterColor; }

		// sets the clous radius
		virtual void setCloudRadius(
			f32 innerRadius,
			f32 outerRadius);

		// sets the cloud height
		virtual void setCloudHeight(
			f32 centerHeight,
			f32 innerHeight,
			f32 outerHeight);

		// sets the cloud colors
		virtual void setCloudColor(
			const video::SColor& centerColor = video::SColor(220,220,220,220),
			const video::SColor& innerColor = video::SColor(180,180,180,180),
			const video::SColor& outerColor = video::SColor(0,0,0,0));
	};

} // end namespace scene
} // end namespace irr

#endif
