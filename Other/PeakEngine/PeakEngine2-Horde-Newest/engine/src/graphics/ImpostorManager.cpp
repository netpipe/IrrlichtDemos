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

#include "graphics/ImpostorManager.h"
#include "graphics/GraphicsEngine.h"
#include "core/FileSystem.h"

#include <Horde3D.h>

namespace peak
{
	Impostor::Impostor()
	{
	}
	Impostor::~Impostor()
	{
	}
	
	bool Impostor::create(EImpostorType type, std::string mesh)
	{
		this->name = mesh + "_internal";
		
		GraphicsEngine *engine = GraphicsEngine::get();
		// Get mesh
		std::string filename = FileSystem::get()->getFileExists(std::string("models/") + mesh);
		if (filename == "")
		{
			return false;
		}
		scene::IAnimatedMesh *fullmesh = engine->getSceneManager()->getMesh(filename.c_str());
		if (!fullmesh)
		{
			return false;
		}
		
		// Get size of mesh
		scene::IAnimatedMeshSceneNode *node = engine->getSceneManager()->addAnimatedMeshSceneNode(fullmesh);
		core::vector3df size = node->getBoundingBox().getExtent();
		printf("Size: %f/%f/%f.\n", size.X, size.Y, size.Z);
		core::vector3df center = node->getBoundingBox().getCenter();
		printf("Center: %f/%f/%f.\n", center.X, center.Y, center.Z);
		node->remove();
		
		// Render mesh to texture
		video::IVideoDriver *driver = engine->getDriver();
		texture = driver->createRenderTargetTexture(core::dimension2d<int>(256, 256));
		driver->setRenderTarget(texture, true, true, video::SColor(0,0,0,0));
		driver->beginScene(true, true, 0);
		// Set matrices
		core::matrix4 worldmatrix;
		worldmatrix.setTranslation(core::vector3df(-0.5, -0.5, 0));
		driver->setTransform(video::ETS_WORLD, worldmatrix);
		core::matrix4 viewmatrix;
		viewmatrix.buildProjectionMatrixOrthoLH(1, -1, 1, -1);
		driver->setTransform(video::ETS_PROJECTION, viewmatrix);
		driver->setTransform(video::ETS_VIEW, core::matrix4());
		// Render mesh
		//printf("%d meshbuffers.\n", fullmesh->getMesh(0)->getMeshBufferCount());
		driver->draw3DLine(core::vector3df(-1, -1, -1), core::vector3df(1, 1, 1));
		// TODO
		driver->setRenderTarget(0);
		driver->makeColorKeyTexture(texture, video::SColor(0,0,0,0));
		
		// Create new mesh
		scene::SMeshBuffer *buffer = new scene::SMeshBuffer();
		video::SColor colour(255,255,255,255);
		u32 indices[18] = {0,1,2, 0,2,3, 4,5,6, 4,6,7, 8,9,10, 8,10,11};
		buffer->Indices.set_used(18);
		for (int i = 0; i < 18; i++)
			buffer->Indices[i] = indices[i];
		
		buffer->Vertices.set_used(12);
		buffer->Vertices[0]  = video::S3DVertex(0,size.Y / 2,-size.Z / 2, 0,0,0, colour, 0, 1);
		buffer->Vertices[1]  = video::S3DVertex(0,size.Y / 2,size.Z / 2, 0,0,0, colour, 0, 1);
		buffer->Vertices[2]  = video::S3DVertex(0,-size.Y / 2,size.Z / 2, 0,0,0, colour, 0, 1);
		buffer->Vertices[3]  = video::S3DVertex(0,-size.Y / 2,-size.Z / 2, 0,0,0, colour, 0, 1);
		buffer->Vertices[4]  = video::S3DVertex(-size.X / 2,size.Y / 2,0, 0,0,0, colour, 0, 1);
		buffer->Vertices[5]  = video::S3DVertex(size.X / 2,size.Y / 2,0, 0,0,0, colour, 0, 1);
		buffer->Vertices[6]  = video::S3DVertex(size.X / 2,-size.Y / 2,0, 0,0,0, colour, 0, 1);
		buffer->Vertices[7]  = video::S3DVertex(-size.X / 2,-size.Y / 2,0, 0,0,0, colour, 0, 1);
		buffer->Vertices[8]  = video::S3DVertex(size.X / 2,0,-size.Z / 2, 0,0,0, colour, 0, 1);
		buffer->Vertices[9]  = video::S3DVertex(size.X / 2,0,size.Z / 2, 0,0,0, colour, 0, 1);
		buffer->Vertices[10]  = video::S3DVertex(-size.X / 2,0,size.Z / 2, 0,0,0, colour, 0, 1);
		buffer->Vertices[11]  = video::S3DVertex(-size.X / 2,0,-size.Z / 2, 0,0,0, colour, 0, 1);
		
		buffer->BoundingBox.reset(0,0,0);

		for (int i=0; i<12; ++i)
		{
			buffer->Vertices[i].Pos += center;
			buffer->BoundingBox.addInternalPoint(buffer->Vertices[i].Pos);
		}
		
		this->mesh = new scene::SMesh();
		((scene::SMesh*)this->mesh)->addMeshBuffer(buffer);
		
		return true;
	}
	bool Impostor::destroy(void)
	{
		ImpostorManager::get()->deleteImpostor(this);
		return true;
	}
	
	std::string Impostor::getName(void)
	{
		return name;
	}
	
	irr::scene::IMesh *Impostor::getMesh(void)
	{
		return mesh;
	}
	irr::video::ITexture *Impostor::getTexture(void)
	{
		return texture;
	}
	
	ImpostorSceneNode::ImpostorSceneNode(Impostor *impostor, SceneNode *parent)
	{
		GraphicsEngine *engine = GraphicsEngine::get();
		
		impostor->grab();
		this->impostor = impostor;
		
		node = engine->getSceneManager()->addMeshSceneNode(impostor->getMesh());
		video::SMaterial *material = &node->getMaterial(0);
		material->setFlag(irr::video::EMF_LIGHTING, false);
		material->setFlag(video::EMF_BACK_FACE_CULLING, false);
		//material->MaterialType = video::EMT_TRANSPARENT_ADD_COLOR;
		material->MaterialType = video::EMT_TRANSPARENT_ALPHA_CHANNEL;
		material->setTexture(0, impostor->getTexture());
		if (parent)
		{
			setParent(parent);
		}
	}
	ImpostorSceneNode::ImpostorSceneNode(EImpostorType type, std::string mesh, SceneNode *parent)
	{
		impostor->drop();
	}
	ImpostorSceneNode::~ImpostorSceneNode()
	{
	}
	
	ImpostorManager *ImpostorManager::get(void)
	{
		static ImpostorManager impmgr;
		return &impmgr;
	}
	ImpostorManager::~ImpostorManager()
	{
	}
	
	Impostor *ImpostorManager::getImpostor(EImpostorType type, std::string mesh)
	{
		// Return existing impostor
		std::list<Impostor*>::iterator it = impostors.begin();
		while (it != impostors.end())
		{
			if ((*it)->getName() == mesh + "_internal")
			{
				return *it;
			}
			it++;
		}
		
		// Create impostor
		printf("Creating impostor.\n");
		Impostor *impostor = new Impostor;
		if (!impostor->create(type, mesh))
		{
			delete impostor;
			return 0;
		}
		impostors.push_back(impostor);
		
		return impostor;
	}
	void ImpostorManager::deleteImpostor(Impostor *impostor)
	{
		std::list<Impostor*>::iterator it = impostors.begin();
		while (it != impostors.end())
		{
			if (*it == impostor)
			{
				impostors.erase(it);
				return;
			}
			it++;
		}
	}
	
	ImpostorManager::ImpostorManager()
	{
	}
}

