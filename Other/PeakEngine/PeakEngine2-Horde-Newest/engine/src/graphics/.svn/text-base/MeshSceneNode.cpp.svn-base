/*
Copyright (C) 2008  Lukas Kropatschek, Mathias Gottschlag

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

#include "graphics/MeshSceneNode.h"
#include "graphics/GraphicsEngine.h"
#include "core/FileSystem.h"
#include "core/GameEngine.h"
#include "core/Logger.h"

//#include <Horde3D.h>
#include <Horde3DUtils.h>
#include <list>

namespace peak
{
	
	MeshSceneNode::MeshSceneNode(std::string mesh, bool isanimated,
			SceneNode *parent)
	{
		animated = isanimated;
		GraphicsEngine *engine = GraphicsEngine::get();
		//std::string filename = FileSystem::get()->getFileExists(std::string("models/") + mesh);
		ResHandle model = Horde3D::addResource(ResourceTypes::SceneGraph, mesh.c_str(), 0);
		Horde3DUtils::loadResourcesFromDisk(GameEngine::get()->getRootDirectory().c_str());
		node = Horde3D::addNodes(RootNode, model);
		//node = engine->getSceneManager()->addAnimatedMeshSceneNode(engine->getSceneManager()->getMesh(filename.c_str()));
		//node->setMaterialFlag(irr::video::EMF_LIGHTING, false);
		if (parent)
		{
			setParent(parent);
		}
		meshnodes.push_back(this);
		
		animationscount = 0;
	}
	
	MeshSceneNode::~MeshSceneNode()
	{
		for (unsigned int i = 0; i < meshnodes.size(); i++)
		{
			if (this == meshnodes[i])
			{
				meshnodes.erase(meshnodes.begin() + i);
			}
		}
	}
	
	int MeshSceneNode::getEndFrame(void)
	{
		//return ((irr::scene::IAnimatedMeshSceneNode*)node)->getEndFrame();
	}

	float MeshSceneNode::getFrameNr(void)
	{
		//return ((irr::scene::IAnimatedMeshSceneNode*)node)->getFrameNr();
	}
	
	int MeshSceneNode::getStartFrame(void)
	{
		//return ((irr::scene::IAnimatedMeshSceneNode*)node)->getStartFrame();
	}
	
	void MeshSceneNode::setLoopMode(bool flag)
	{
		//((irr::scene::IAnimatedMeshSceneNode*)node)->setLoopMode(flag);
	}
	
	void MeshSceneNode::setAnimationSpeed(float speed)
	{
		//((irr::scene::IAnimatedMeshSceneNode*)node)->setAnimationSpeed(speed);
	}
	
	void MeshSceneNode::pushAnimation(std::string animation)
	{
		ResHandle newanim;
		anims.push_back(newanim);
		anims[animationscount] = Horde3D::addResource( ResourceTypes::Animation, animation.c_str(), 0);
		Horde3DUtils::loadResourcesFromDisk(GameEngine::get()->getRootDirectory().c_str());
		Horde3D::setupModelAnimStage( node, animationscount, anims[animationscount], "", false );
		animationscount++;
	}
			 
	void MeshSceneNode::popAnimation()
	{
		Horde3D::removeResource(anims[animationscount]);
		anims.erase(anims.end());
		animationscount--;
	}
	
	void MeshSceneNode::update(float msecs)
	{
		animtime += msecs / 1000;
		for (int i = 0; i < animationscount; i++)
		{
			Horde3D::setModelAnimParams( node, i, animtime * 25.0f, 1.0 );
		}
	}

	std::vector<MeshSceneNode*> MeshSceneNode::meshnodes;
	
	void MeshSceneNode::updateAll(float msecs)
	{
		for (unsigned int i = 0; i < meshnodes.size(); i++)
		{
			meshnodes[i]->update(msecs);
		}
	}
	

}

