// Copyright (C) 2008-2012 Benjamin Hampe
// This file uses parts of the "Irrlicht Engine"
// For conditions of distribution and use, see copyright notice in irrlicht.h

#ifndef __IRR_C_CAPSULE_SCENE_NODE_H__
#define __IRR_C_CAPSULE_SCENE_NODE_H__

#include <irrlicht.h>

namespace irr{
namespace scene{

class CCapsuleSceneNode : public ISceneNode
{
public:
	CCapsuleSceneNode(
		ISceneManager* mgr, ISceneNode* parent,
		f32 radius, f32 height, u32 cols, u32 rows);

	virtual ~CCapsuleSceneNode();

	virtual void OnRegisterSceneNode();

	virtual const core::aabbox3d<f32>& getBoundingBox() const;

	virtual u32 getMaterialCount() const;

	virtual video::SMaterial& getMaterial(u32 i);

	virtual void render();

	static SMeshBuffer* createCylinderMeshBuffer(
		f32 radius, f32 height, u32 cols, const core::vector3df& pos = core::vector3df(0,0,0));

	static SMeshBuffer* createUpperSphereHalfMeshBuffer(
		f32 radius, u32 cols, u32 rows, const core::vector3df& pos = core::vector3df(0,0,0));

	static SMeshBuffer* createLowerSphereHalfMeshBuffer(
		f32 radius, u32 cols, u32 rows, const core::vector3df& pos = core::vector3df(0,0,0));


private:
	SMeshBuffer* Top;
	SMeshBuffer* Center;
	SMeshBuffer* Bottom;
	core::aabbox3df BoundingBox;

	//! Debug Normals, only visible if setDebugDataVisible(true)
	u32 VertexCount;
	u32 IndexCount;
	SMeshBuffer* TopNormals;
	SMeshBuffer* CenterNormals;
	SMeshBuffer* BottomNormals;

};

} // END NAMSPACE SCENE
} // END NAMSPACE IRR
#endif
