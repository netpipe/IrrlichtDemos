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

#include "graphics/ModelSceneNode.h"
#include "graphics/GraphicsEngine.h"
#include "graphics/MeshSceneNode.h"
#include "core/FileSystem.h"
#include "core/IniFile.h"
#include "core/Logger.h"
#include "core/GameEngine.h"

#include <Horde3D.h>

namespace peak
{
	ModelSceneNode::ModelSceneNode(std::string name, SceneNode *parent)
	{
		GraphicsEngine *engine = GraphicsEngine::get();
		// Create node holding the scene nodes
		/*node = GraphicsEngine::get()->getSceneManager()->addEmptySceneNode();
		this->parent = 0;
		if (parent)
		{
			setParent(parent);
		}
		
		// Open .model file
		IniFile file;
		if (!file.load(std::string("models/") + name + ".model"))
		{
			LERROR("Could not open model file \"%s\".\n", (std::string("models/") + name + ".model").c_str());
			return;
		}
		if (file.getItemIndex("model", "lodcount") != -1)
		{
			lodcount = file.getItemValueI("model", "lodcount");
		}
		else
		{
			lodcount = 0;
		}
		// Create first mesh
		basenode = 0;
		
		std::string type = file.getItemValue("model", "type");
		if (type == "mesh")
		{
			std::string mesh = file.getItemValue("model", "mesh");
			basenode = new MeshSceneNode(mesh);
			basenode->setParent(this);
		}
		else if (type == "simpleimpostor")
		{
			std::string mesh = file.getItemValue("model", "mesh");
			Impostor *baseimpostor = ImpostorManager::get()->getImpostor(EIT_Internal, mesh);
			if (baseimpostor)
			{
				basenode = new ImpostorSceneNode(baseimpostor, this);
			}
		}
		
		// Create other levels of detail
		
		// Set materials
		if (file.getItemIndex("model", "materialcount") != -1)
		{
			int materialcount = file.getItemValueI("model", "materialcount");
			for (int i = 0; i < materialcount; i++)
			{
				std::string material = file.getItemValue("model", std::string("material") + toString(i));
				setMaterial(material, i);
			}
		}
		else
		{
			std::string material = file.getItemValue("model", "material");
			setMaterial(material);
		}*/
	}
	ModelSceneNode::~ModelSceneNode()
	{
	}
}

