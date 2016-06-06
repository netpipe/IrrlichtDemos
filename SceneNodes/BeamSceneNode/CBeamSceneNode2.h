// Copyright (C) 2009 Gaz Davidson
// Released under The Irrlicht Engine License, see irrlicht.h

#ifndef __C_BEAM_SCENE_NODE_H_INCLUDED__
#define __C_BEAM_SCENE_NODE_H_INCLUDED__

#include "ISceneNode.h"
#include "S3DVertex.h"

namespace irr
{
namespace scene
{

//! Scene node
class CBeamSceneNode2 : public ISceneNode
{
public:

	//! constructor
	CBeamSceneNode2(ISceneNode* parent, ISceneManager* mgr, s32 id,
			const core::vector3df& startPosition, const core::vector3df& endPosition, f32 size);

	//! pre render event
	virtual void OnRegisterSceneNode();

	//! render
	virtual void render();

	//! returns the axis aligned bounding box of this node
	virtual const core::aabbox3d<f32>& getBoundingBox() const;

	//! sets the size of the billboard
	virtual void setSize(f32 size);

	//! gets the size of the billboard
	virtual f32 getSize() const;

	//! sets the position of start and end points
	virtual void setPosition(const core::vector3df& startPos, const core::vector3df& endPos);

	//! Returns the material
	virtual video::SMaterial& getMaterial(u32 i);

	//! returns amount of materials used by this scene node.
	virtual u32 getMaterialCount() const;



	//! Writes attributes of the scene node.
	virtual void serializeAttributes(io::IAttributes* out, io::SAttributeReadWriteOptions* options=0) const;

	//! Reads attributes of the scene node.
	virtual void deserializeAttributes(io::IAttributes* in, io::SAttributeReadWriteOptions* options=0);

	//! Returns type of the scene node
	virtual ESCENE_NODE_TYPE getType() const { return ESCENE_NODE_TYPE(MAKE_IRR_ID('B','E','A','M')); }

	//! Creates a clone of this scene node and its children.
	//virtual ISceneNode* clone(ISceneNode* newParent=0, ISceneManager* newManager=0);

private:

	void recalculateBoundingBox();

	f32 Size;
	core::vector3df EndPos;

	core::aabbox3d<f32> BBox;
	video::SMaterial Material;

	video::S3DVertex vertices[12];
};


} // end namespace scene
} // end namespace irr

#endif

