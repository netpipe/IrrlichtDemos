// Copyright (C) Pazystamo
// This file is part of the "irrWeatherManager" weather management library for the Irrlicht rendering engine.
// The code in this file was separated from its original file, and the code belongs to Pazystamo from the Irrlicht forums.

//! The class was totally rewritten by Gawaboumga (Copyright (C) 20013-2014 Youri Hubaut)
//! and mostly inspired of the original version of this project.

#ifndef __IAtmosphereSkySceneNode_H_INCLUDED__
#define __IAtmosphereSkySceneNode_H_INCLUDED__

#include <irr/irrlicht.h>

namespace irr
{

class IAtmosphereSkySceneNode : public scene::ISceneNode
{
    public:
        //! Constructor.
        IAtmosphereSkySceneNode(scene::ISceneNode* parent, scene::ISceneManager* smgr, s32 id,
            video::ITexture* texture, const u16 numberOfFaces = 16, f32 radius = 100.f);

        //! Destructor.
		~IAtmosphereSkySceneNode();

		virtual void OnRegisterSceneNode();

        //! Renders the node.
		virtual void render();

        //! Returns the axis aligned bounding box of this node.
		virtual const core::aabbox3d<f32>& getBoundingBox() const;

        /// Changes the sky texture.
        virtual video::SMaterial& setMaterial(video::ITexture* const tex);

		//! Returns the material based on the zero based index i. To get the amount
		//! of materials used by this scene node, use getMaterialCount().
		//! This function is needed for inserting the node into the scene hirachy on a
		//! optimal position for minimizing renderstate changes, but can also be used
		//! to directly modify the material of a scene node.
		virtual video::SMaterial& getMaterial(u32 i);

        //! Returns amount of materials used by this scene node.
		virtual u32 getMaterialCount() const;

		//! Returns type of the scene node
		virtual scene::ESCENE_NODE_TYPE getType() const { return scene::ESNT_SKY_DOME; }

		//! Writes attributes of the scene node.
		virtual void serializeAttributes(io::IAttributes* out, io::SAttributeReadWriteOptions* options=0) const;

		//! Reads attributes of the scene node.
		virtual void deserializeAttributes(io::IAttributes* in, io::SAttributeReadWriteOptions* options=0);

        //! Creates a clone of this scene node and its children.
		virtual scene::ISceneNode* clone(scene::ISceneNode* newParent=0, scene::ISceneManager* newManager=0);

        //! Updates UV mapping X coordinate.
        void setUvX(f32 uvx);

	private:

		void generateMesh();

		scene::SMeshBuffer* Buffer;
        video::SMaterial Material;
        u16 Faces;
        f32 Radius;
        f32 UvX;
};

} //namespace

#endif // __IAtmosphereSkySceneNode_H_INCLUDED__
