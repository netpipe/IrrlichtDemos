// Copyright (C) 2002-2005 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#include "CMeshSceneNode.h"
//#include "IVideoDriver.h"
//#include "ISceneManager.h"
//#include "S3DVertex.h"
//#include <SViewFrustrum.h>
//#include "ICameraSceneNode.h"

namespace irr
{
namespace scene
{



//! constructor
CMeshSceneNode1::CMeshSceneNode1(IMesh* mesh, ISceneNode* parent, ISceneManager* mgr, s32 id,
			const core::vector3df& position, const core::vector3df& rotation,
			const core::vector3df& scale)
: ISceneNode(parent, mgr, id, position, rotation, scale), Mesh(mesh), PassCount(0)
{
	#ifdef _DEBUG
	setDebugName("CMeshSceneNode1");
	#endif

	if (Mesh)
	{
		// get materials.
		video::SMaterial mat;
		for (s32 i=0; i<Mesh->getMeshBufferCount(); ++i)
		{
			IMeshBuffer* mb = Mesh->getMeshBuffer(i);
			if (mb)
				mat = mb->getMaterial();

			Materials.push_back(mat);
		}

		// grab the mesh

		Mesh->grab();
	}
}



//! destructor
CMeshSceneNode1::~CMeshSceneNode1()
{
	if (Mesh)
		Mesh->drop();
}



//! frame
void CMeshSceneNode1::OnPreRender()
{
	if (IsVisible)
	{
		// because this node supports rendering of mixed mode meshes consisting of
		// transparent and solid material at the same time, we need to go through all
		// materials, check of what type they are and register this node for the right
		// render pass according to that.

		video::IVideoDriver* driver = SceneManager->getVideoDriver();

		PassCount = 0;
		int transparentCount = 0;
		int solidCount = 0;

		// count transparent and solid materials in this scene node
		for (u32 i=0; i<Materials.size(); ++i)
		{
			video::IMaterialRenderer* rnd =
				driver->getMaterialRenderer(Materials[i].MaterialType);

			if (rnd && rnd->isTransparent())
				++transparentCount;
			else
				++solidCount;

			if (solidCount && transparentCount)
				break;
		}

		// register according to material types counted

		if (solidCount)
			SceneManager->registerNodeForRendering(this, scene::ESNRP_SOLID);

		if (transparentCount)
			SceneManager->registerNodeForRendering(this, scene::ESNRP_TRANSPARENT);
	}

	///ISceneNode::OnPreRender();
}



//! renders the node.
void CMeshSceneNode1::render()
{
	video::IVideoDriver* driver = SceneManager->getVideoDriver();

	if (!Mesh || !driver)
		return;

	bool isTransparentPass =
		SceneManager->getSceneNodeRenderPass() == scene::ESNRP_TRANSPARENT;

	++PassCount;

	driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);
	Box = Mesh->getBoundingBox();

	// for debug purposes only:
	if (DebugDataVisible && PassCount==1)
	{
		video::SMaterial m;
		m.Lighting = false;
		driver->setMaterial(m);
		driver->draw3DBox(Box, video::SColor(0,255,255,255));
	}

	for (s32 i=0; i<Mesh->getMeshBufferCount(); ++i)
	{
		video::IMaterialRenderer* rnd = driver->getMaterialRenderer(Materials[i].MaterialType);
		bool transparent = (rnd && rnd->isTransparent());

		// only render transparent buffer if this is the transparent render pass
		// and solid only in solid pass
		if (transparent == isTransparentPass)
		{
			scene::IMeshBuffer* mb = Mesh->getMeshBuffer(i);
			driver->setMaterial(Materials[i]);
			driver->drawMeshBuffer(mb);
		}
	}
}


//! returns the axis aligned bounding box of this node
const core::aabbox3d<f32>& CMeshSceneNode1::getBoundingBox() const
{
	return Mesh ? Mesh->getBoundingBox() : Box;
}


//! returns the material based on the zero based index i. To get the amount
//! of materials used by this scene node, use getMaterialCount().
//! This function is needed for inserting the node into the scene hirachy on a
//! optimal position for minimizing renderstate changes, but can also be used
//! to directly modify the material of a scene node.
video::SMaterial& CMeshSceneNode1::getMaterial(s32 i)
{
	if (i < 0 || i >= (s32)Materials.size())
		return ISceneNode::getMaterial(i);

	return Materials[i];
}



//! returns amount of materials used by this scene node.
s32 CMeshSceneNode1::getMaterialCount()
{
	return Materials.size();
}



} // end namespace scene
} // end namespace irr

