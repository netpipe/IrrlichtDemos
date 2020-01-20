// Copyright (C) 2008-2012 Benjamin Hampe
// This file uses parts of the "Irrlicht Engine"
// For conditions of distribution and use, see copyright notice in irrlicht.h

#ifndef __IRR_C_NORMAL_SCENE_NODE_H__
#define __IRR_C_NORMAL_SCENE_NODE_H__

#include <irrlicht.h>

namespace irr{
namespace scene{

class CNormalSceneNode : public IMeshSceneNode
{
public:
	CNormalSceneNode(
		IMeshSceneNode* src,
		f32 normalLength,
		bool monochrom,
		video::SColor color, // default is red
		ISceneNode* parent,
		ISceneManager* mgr,
		s32 id = -1,
		const core::vector3df& position = core::vector3df(0,0,0),
		const core::vector3df& rotation = core::vector3df(0,0,0),
		const core::vector3df& scale = core::vector3df(1,1,1));

public:
	virtual ~CNormalSceneNode();

	virtual void OnRegisterSceneNode();

	virtual const core::aabbox3d<f32>& getBoundingBox() const;

	virtual u32 getMaterialCount() const;

	virtual video::SMaterial& getMaterial(u32 i);

	virtual void render();

	virtual void setMesh(IMesh* mesh);

	virtual IMesh* getMesh(void);

	virtual void setReadOnlyMaterials(bool readonly);

	virtual bool isReadOnlyMaterials() const;
private:
	SMesh* Mesh;
	core::aabbox3df BBox;
};

} // END NAMSPACE SCENE
} // END NAMSPACE IRR
#endif

