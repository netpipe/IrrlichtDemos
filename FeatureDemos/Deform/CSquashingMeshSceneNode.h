// Copyright (C) 2002-2005 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#ifndef __C_SQUASHING_MESH_SCENE_NODE_H_INCLUDED__
#define __C_SQUASHING_MESH_SCENE_NODE_H_INCLUDED__

#include "CMeshSceneNode.h"

namespace irr
{
namespace scene
{

	class CSquashingMeshSceneNode : public CMeshSceneNode1
	{
	public:

		//! constructor
		CSquashingMeshSceneNode(
			IMesh* mesh, ISceneNode* parent, ISceneManager* mgr,	s32 id,
			const core::vector3df& position = core::vector3df(0,0,0),
			const core::vector3df& rotation = core::vector3df(0,0,0),
			const core::vector3df& scale = core::vector3df(1.0f, 1.0f, 1.0f));

		//! destructor
		virtual ~CSquashingMeshSceneNode();

		//! frames
		virtual void OnPreRender();
		virtual void OnPostRender(u32 t);

		//! deforming the mesh
		virtual void deformMesh(u32 t);


	private:
        u32 lasttime;                     // the time last loop
		IMesh* OriginalMesh;              // the original mesh
	};

} // end namespace scene
} // end namespace irr

#endif

