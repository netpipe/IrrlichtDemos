// Copyright (C) 2002-2005 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#ifndef __C_MESH_SCENE_NODE_H_INCLUDED__
#define __C_MESH_SCENE_NODE_H_INCLUDED__

#include "irrlicht.h"

namespace irr
{
namespace scene
{

	class CMeshSceneNode1 : public ISceneNode
	{
	public:

		//! constructor
		CMeshSceneNode1(IMesh* mesh, ISceneNode* parent, ISceneManager* mgr,	s32 id,
			const core::vector3df& position = core::vector3df(0,0,0),
			const core::vector3df& rotation = core::vector3df(0,0,0),
			const core::vector3df& scale = core::vector3df(1.0f, 1.0f, 1.0f));

		//! destructor
		virtual ~CMeshSceneNode1();

		//! frame
		virtual void OnPreRender();

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

	protected:

		core::array<video::SMaterial> Materials;
		core::aabbox3d<f32> Box;
		IMesh* Mesh;
		s32 PassCount;
	};

} // end namespace scene
} // end namespace irr

#endif

