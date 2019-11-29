/*
Copyright (C) 2008  Lukas Kropatschek

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in the
Software without restriction, including without limitation the rights to use,
copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the
Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "graphic/MeshSceneNode.h"
#include "graphic/GraphicsEngine.h"
#include "core/FileSystem.h"

#include <irrlicht.h>

namespace peak
{

	MeshSceneNode::MeshSceneNode(std::string mesh, bool isanimated,
			SceneNode *parent)
	{
		animated = isanimated;
		GraphicsEngine *engine = GraphicsEngine::get();
		std::string filename = FileSystem::get()->getFileExists(std::string("models/") + mesh);
		node = engine->getSceneManager()->addAnimatedMeshSceneNode(engine->getSceneManager()->getMesh(filename.c_str()));
		node->setMaterialFlag(irr::video::EMF_LIGHTING, false);
		if (parent)
		{
			setParent(parent);
		}
	}

	MeshSceneNode::~MeshSceneNode()
	{
	}

	int MeshSceneNode::getEndFrame(void)
	{
		return ((irr::scene::IAnimatedMeshSceneNode*)node)->getEndFrame();
	}

	float MeshSceneNode::getFrameNr(void)
	{
		return ((irr::scene::IAnimatedMeshSceneNode*)node)->getFrameNr();
	}

	int MeshSceneNode::getStartFrame(void)
	{
		return ((irr::scene::IAnimatedMeshSceneNode*)node)->getStartFrame();
	}

	void MeshSceneNode::setLoopMode(bool flag)
	{
		((irr::scene::IAnimatedMeshSceneNode*)node)->setLoopMode(flag);
	}

	void MeshSceneNode::setAnimationSpeed(float speed)
	{
		((irr::scene::IAnimatedMeshSceneNode*)node)->setAnimationSpeed(speed);
	}
}

