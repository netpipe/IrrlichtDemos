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

#include "peakgraphics/scene/CameraSceneNode.hpp"
#include "peakgraphics/Graphics.hpp"
#include <peakengine/support/ScopedLock.hpp>

#include <lf/Lightfeather.h>
using namespace lf;

namespace peak
{
	CameraSceneNode::CameraSceneNode(Graphics *graphics) : SceneNode(graphics)
	{
		graphics->registerLoading(this);
	}
	CameraSceneNode::~CameraSceneNode()
	{
		if (node)
			node->drop();
	}

	bool CameraSceneNode::load()
	{
		// Create camera
		scene::C3DCamera *camera = new scene::C3DCamera(graphics->getWindow(),
			core::PI / 3.0f, 0.3f, 1000.0f, true);
		camera->setBackgroundColor(core::CColorI(0,0,255,0));
		ScopedLock lock(mutex);
		node = camera;
		graphics->getWindow()->getRenderLayer3D()->addCamera(camera);
		// Update parent and position
		// TODO
		return true;
	}
	bool CameraSceneNode::isLoaded()
	{
		return node != 0;
	}
}
