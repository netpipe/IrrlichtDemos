// Copyright (C) Pazystamo
// This file is part of the "irrWeatherManager" weather management library for the Irrlicht rendering engine.
// The code in this file was separated from its original file, and the code belongs to Pazystamo from the Irrlicht forums.

//! The class was totally rewritten by Gawaboumga (Copyright (C) 20013-2014 Youri Hubaut)
//! and mostly inspired of the original version of this project.

#ifndef __IAtmosphereStarSceneNode_H_INCLUDED__
#define __IAtmosphereStarSceneNode_H_INCLUDED__

#include <irr/irrlicht.h>

namespace irr
{

class IAtmosphereStarSceneNode : public scene::ISceneNode
{
    public:
        //! Constructor.
        IAtmosphereStarSceneNode(scene::ISceneNode* parent, scene::ISceneManager* smgr, s32 id,
            video::ITexture* sun, video::ITexture* moon, const core::dimension2d<f32>& size = core::dimension2d<f32>(128.f, 128.f));

        //! Destructor.
        ~IAtmosphereStarSceneNode();

        virtual void OnRegisterSceneNode();

        //! Renders the node.
        virtual void render();

        //! Returns the axis aligned bounding box of this node
        virtual const core::aabbox3d<f32>& getBoundingBox() const;

        //! Changes the sun texture.
        void setSunTexture(video::ITexture* const newSunTexture);

        //! Changes the moon texture.
        void setMoonTexture(video::ITexture* const newMoonTexture);

        //! Returns the material based on the zero based index i. To get the amount
        //! of materials used by this scene node, use getMaterialCount().
        //! This function is needed for inserting the node into the scene hirachy on a
        //! optimal position for minimizing renderstate changes, but can also be used
        //! to directly modify the material of a scene node.
        virtual video::SMaterial& getMaterial(u32 i);

        virtual video::SMaterial& getMoonMaterial(u32 i);

        //! Returns amount of materials used by this scene node.
        virtual u32 getMaterialCount() const;

        //! Returns type of the scene node
		virtual scene::ESCENE_NODE_TYPE getType() const { return scene::ESNT_BILLBOARD; }

        //! Creates a clone of this scene node and its children.
		virtual scene::ISceneNode* clone(scene::ISceneNode* newParent=0, scene::ISceneManager* newManager=0);

        //! Sets the size of the billboard.
        void setSize(const core::dimension2d<f32>& size);

        //! Returns the size of the sun & the moon.
        const core::dimension2d<f32>& getSize();

        //! Sets the sun postion.
        void setSunPosition(core::vector3df& newSunPos);

        //! Sets the moon postion.
        void setMoonPosition(core::vector3df& newMoonPos);

    private:

        void generateMesh();

        core::dimension2d<f32> Size;

        scene::SMeshBuffer* BufferSun;
        scene::SMeshBuffer* BufferMoon;

        core::vector3df SunPosition;
        core::vector3df MoonPosition;
};

} // namespace

#endif // __IAtmosphereStarSceneNode_H_INCLUDED__
