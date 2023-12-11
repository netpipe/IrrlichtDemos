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

#include "peakgraphics/scene/ModelSceneNode.hpp"
#include "peakgraphics/Graphics.hpp"
#include <peakengine/support/ScopedLock.hpp>

#include <lf/Lightfeather.h>
using namespace lf;

#include <iostream>

namespace peak
{
	ModelSceneNode::ModelSceneNode(std::string name, Graphics *graphics)
		: SceneNode(graphics), name(name)
	{
		graphics->registerLoading(this);
	}
	ModelSceneNode::~ModelSceneNode()
	{
		if (node)
			node->drop();
	}

	bool ModelSceneNode::load()
	{
		// Load model
		CResourceManager *resmgr = CResourceManager::getInstancePtr();
		res::CModel *model = resmgr->getModel(name.c_str());
		if (!model)
		{
			std::cout << "Model \"" << name << "\" not available." << std::endl;
			return false;
		}
		ScopedLock lock(mutex);
		node = new scene::CModelSceneNode(model);
		// Update parent and position
		// TODO
		return true;
	}
	bool ModelSceneNode::isLoaded()
	{
		return node != 0;
	}
}
