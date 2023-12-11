/*
Copyright (c) 2009, Mathias Gottschlag

Permission to use, copy, modify, and/or distribute this software for any
purpose with or without fee is hereby granted, provided that the above
copyright notice and this permission notice appear in all copies.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

#include "peakgraphics/scene/TerrainSceneNode.hpp"
#include "peakgraphics/Graphics.hpp"
#include <peakengine/support/ScopedLock.hpp>

#include <lf/Lightfeather.h>
using namespace lf;

namespace peak
{
	TerrainSceneNode::TerrainSceneNode(std::string name, std::string heightmap,
		std::string colormap, std::string detailmap, Vector3F scale,
		Graphics *graphics) : SceneNode(graphics), name(name),
		heightmap(heightmap), colormap(colormap), detailmap(detailmap),
		modelscale(scale)
	{
		graphics->registerLoading(this);
	}
	TerrainSceneNode::~TerrainSceneNode()
	{
		if (node)
			node->drop();
	}

	bool TerrainSceneNode::load()
	{
		CResourceManager *resmgr = CResourceManager::getInstancePtr();
		// Load heightmap
		res::CImage *hmap = resmgr->loadImage(heightmap.c_str());
		if (!hmap)
			return false;
		// Load model
		res::CGeoMipMapTerrainModel *model;
		model = new res::CGeoMipMapTerrainModel(name.c_str());
		model->loadHeightMap(hmap, 1.0f, 64.0f);
		hmap->drop();
		// Set material
		res::CTexture *colortex = resmgr->loadTexture(colormap.c_str(),
			colormap.c_str());
		res::CTexture *detailtex = resmgr->loadTexture(detailmap.c_str(),
			detailmap.c_str());
		model->getMaterial(0)->replaceRenderFeature(render::ERPF_DETAIL_MAP);
		model->getMaterial(0)->setTexture(0, colortex);
		model->getMaterial(0)->setTexture(1, detailtex);
		model->setScale(core::vector3df(modelscale.x, modelscale.y, modelscale.z));
		// Create node
		ScopedLock lock(mutex);
		node = new scene::CModelSceneNode(model);
		/*render::CRenderStateFillMode *wire = new render::CRenderStateFillMode(render::EFIM_WIREFRAME);
		((scene::CModelSceneNode*)node)->addRenderStateToMaterials(wire);*/
		// Update parent and position
		// TODO
		return true;
	}
	bool TerrainSceneNode::isLoaded()
	{
		return node != 0;
	}
}
