/*
Copyright (C) 2008  Mathias Gottschlag

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

#include "graphics/ParticleSystem.h"
#include "graphics/GraphicsEngine.h"
#include "core/FileSystem.h"
#include "core/IniFile.h"
#include "core/Logger.h"
#include "core/GameEngine.h"

#include <Horde3D.h>
#include <Horde3DUtils.h>

namespace peak
{
	ParticleSystem::ParticleSystem(std::string filename, SceneNode *parent)
	{
		GraphicsEngine *engine = GraphicsEngine::get();
		
		resource = Horde3D::addResource(ResourceTypes::SceneGraph, filename.c_str(), 0);
		Horde3DUtils::loadResourcesFromDisk(GameEngine::get()->getRootDirectory().c_str());
		node = Horde3D::addNodes(RootNode, resource);
				
		if (parent)
		{
			setParent(parent);
		}
		particlesystems.push_back(this);
	}
	ParticleSystem::~ParticleSystem()
	{
		std::list<ParticleSystem*>::iterator it = particlesystems.begin();
		while (it != particlesystems.end())
		{
			if (*it == this)
			{
				particlesystems.erase(it);
				break;
			}
			it++;
		}
	}
	
	void ParticleSystem::doWork(float msecs)
	{
		unsigned int count = Horde3D::findNodes(node, "", SceneNodeTypes::Emitter);
		for(unsigned int i = 0; i < count; ++i)
		{
			Horde3D::advanceEmitterTime(Horde3D::getNodeFindResult(i), msecs * 0.001);
		}
	}
	void ParticleSystem::doAllWork(float msecs)
	{
		std::list<ParticleSystem*>::iterator it = particlesystems.begin();
		while (it != particlesystems.end())
		{
			(*it)->doWork(msecs);
			it++;
		}
	}
	
	std::list<ParticleSystem*> ParticleSystem::particlesystems;
}

