//! This Class was partly rewritten by Gawaboumga (Youri Hubaut).
//! But all rights remain at tbw.

#ifndef _CLOUDSCENENODE_H
#define _CLOUDSCENENODE_H

#include <irr/irrlicht.h>

#define CLOUDSUBDIV 16

namespace irr
{

class CCloudSceneNode : public scene::ISceneNode
{
    public:
        //! Constructor.
        CCloudSceneNode(scene::ISceneNode* parent, scene::ISceneManager* mgr, s32 id = -1);

        //! Destructor.
		~CCloudSceneNode();

        virtual void OnRegisterSceneNode();

        //! Animates the node.
        virtual void OnAnimate(u32 timeMs);

        //! Renders the node.
		virtual void render();

        //! Returns the axis aligned bounding box of this node.
		virtual const core::aabbox3d<f32>& getBoundingBox() const;

        //! Returns the material based on the zero based index i. To get the amount
		//! of materials used by this scene node, use getMaterialCount().
		//! This function is needed for inserting the node into the scene hirachy on a
		//! optimal position for minimizing renderstate changes, but can also be used
		//! to directly modify the material of a scene node.
		virtual video::SMaterial& getMaterial(u32 i);

        //! Returns amount of materials used by this scene node.
		virtual u32 getMaterialCount() const;

		//! Returns type of the scene node
		virtual scene::ESCENE_NODE_TYPE getType() const;

		//! Writes attributes of the scene node.
		virtual void serializeAttributes(io::IAttributes* out, io::SAttributeReadWriteOptions* options=0) const;

		//! Reads attributes of the scene node.
		virtual void deserializeAttributes(io::IAttributes* in, io::SAttributeReadWriteOptions* options=0);

        //! Creates a clone of this scene node and its children.
		virtual scene::ISceneNode* clone(scene::ISceneNode* newParent=0, scene::ISceneManager* newManager=0);

        //! Returns the translation of the node.
        virtual core::vector2d<f32>& getTranslation() { return Translation; }

        //! Sets the translation of the node.
        virtual void setTranslation(const core::vector2d<f32>& translation);

        //! Returns the scale of the cloud texture.
        virtual f32 getTextureScale() { return TextureScale; }

        //! Sets the scale of the cloud texture.
        virtual void setTextureScale(f32 textureScale);

        //! Returns the inner radius.
        virtual f32 getInnerRadius() { return InnerRadius; }

        //! Returns the outer radius.
        virtual f32 getOuterRadius() { return OuterRadius; }

        //! Returns the center height.
        virtual f32 getCenterHeight() { return CenterHeight; }

        //! Returns the inner height.
        virtual f32 getInnerHeight() { return InnerHeight; }

        //! Returns the outer height.
        virtual f32 getOuterHeight() { return OuterHeight; }

        //! Returns the center color.
        virtual video::SColor& getCenterColor() { return CenterColor; }

        //! Returns the inner color.
        virtual video::SColor& getInnerColor() { return InnerColor; }

        //! Returns the outer color.
        virtual video::SColor& getOuterColor() { return OuterColor; }

        //! Sets the cloud radius.
        virtual void setCloudRadius(
            f32 innerRadius,
            f32 outerRadius);

        //! Sets the cloud height.
        virtual void setCloudHeight(
            f32 centerHeight,
            f32 innerHeight,
            f32 outerHeight);

        //! Sets the cloud colors.
        virtual void setCloudColor(
            const video::SColor& centerColor = video::SColor(220,220,220,220),
            const video::SColor& innerColor = video::SColor(180,180,180,180),
            const video::SColor& outerColor = video::SColor(0,0,0,0));

    private:

        //! Creates the vertices and indices used by the node.
        void createCloudLayer();

        //! Vertices and indices.
        scene::SMeshBuffer* BufferInner;
        scene::SMeshBuffer* BufferOuter;

        //! Material of the node.
        video::SMaterial Material;

        //! Last update of the node.
        u32 LastUpdate;

        //! Scale of the cloud.
        f32 TextureScale;

        //! Inner and outer radius.
        f32 InnerRadius;
        f32 OuterRadius;

        //! Heights of the cloud (center, inner and outer).
        f32 CenterHeight;
        f32 InnerHeight;
        f32 OuterHeight;

        //! Colors of the cloud.
        video::SColor CenterColor;
        video::SColor InnerColor;
        video::SColor OuterColor;

        //! Cloud translation.
        core::vector2d<f32> Translation;

        //! Current translation (next translation step = current + elapsed time * Translation).
        core::vector2d<f32> CurrentTranslation;
};

} // namespace

#endif // __CLOUDESCENENODE_H_INCLUDED__

