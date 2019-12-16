
/*
Copyright (C) 2008   Mathias Gottschlag

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

#include "LevelData.h"
#include "Horde3DWidget.h"

#include "core/FileSystem.h"
#include "core/Util.h"

#include <Horde3D.h>
#include <Horde3DUtils.h>

LevelNode::LevelNode()
{
	scenenode = 0;
}
LevelNode::~LevelNode()
{
	for (unsigned int i = 0; i < children.size(); i++)
	{
		delete children[i];
	}
}

bool LevelNode::load(LevelNode *parent)
{
	if (parent)
	{
		scenenode = Horde3D::addGroupNode(parent->getSceneNode(), "Level");
	}
	else
	{
		scenenode = Horde3D::addGroupNode(RootNode, "Level");
	}
	return true;
}

void LevelNode::addChild(LevelNode *node)
{
	children.push_back(node);
}

MeshLevelNode::MeshLevelNode()
{
	scenenode = 0;
	model = 0;
}
MeshLevelNode::~MeshLevelNode()
{
}

bool MeshLevelNode::load(TiXmlElement *element, LevelNode *parent)
{
	std::string mesh = element->Attribute("file");
	model = Horde3D::addResource(ResourceTypes::SceneGraph, mesh.c_str(), 0);
	Horde3DWidget::loadResources();
	scenenode = Horde3D::addNodes(parent->getSceneNode(), model);
	peak::Vector3D scale(1, 1, 1);
	if (element->Attribute("scale"))
	{
	    scale = peak::toVector3D(element->Attribute("scale"));
	}
	peak::Vector3D position(0, 0, 0);
	if (element->Attribute("position"))
	{
	    position = peak::toVector3D(element->Attribute("position"));
	}
	peak::Vector3D rotation(0, 0, 0);
	if (element->Attribute("rotation"))
	{
	    rotation = peak::toVector3D(element->Attribute("rotation"));
	}
	Horde3D::setNodeTransform(scenenode, position.x, position.y, position.z,
		rotation.x, rotation.y, rotation.z,
		scale.x, scale.y, scale.z);
	return true;
}

TerrainLevelNode::TerrainLevelNode() : LevelNode()
{
}
TerrainLevelNode::~TerrainLevelNode()
{
}

bool TerrainLevelNode::load(TiXmlElement *element, LevelNode *parent)
{
	return false;
}

LevelData::LevelData()
{
}
LevelData::~LevelData()
{
}

bool LevelData::load(std::string file)
{
	// Open XML file
	std::string filename = peak::FileSystem::get()->getFileExists(std::string("levels/") + file + ".xml");
	if (filename == "")
	{
		printf("Could not find file %s.\n", (std::string("levels/") + file + ".xml").c_str());
		return false;
	}
	TiXmlDocument xml(filename.c_str());
	if (!xml.LoadFile() || xml.Error())
	{
		printf("Could not load XML file %s.xml: %s.\n", file.c_str(), xml.ErrorDesc());
		return false;
	}

	// Read in file
	TiXmlNode *node = xml.FirstChild("level");
	if (!node)
	{
		printf("Parser error: <level> not found.\n");
		return false;
	}
	TiXmlElement *root = node->ToElement();

	rootnode = new LevelNode;
	rootnode->load(0);

	// Read in scene nodes
	TiXmlNode *scenenode = root->FirstChild("scenenode");
	while (scenenode)
	{
		TiXmlElement *element = scenenode->ToElement();
		if (!element->Attribute("name") || !element->Attribute("type"))
		{
			printf("Missing 'name' or 'type' in scenenode.\n");
			scenenode = node->IterateChildren("scenenode", scenenode);
			continue;
		}
		
		printf("Scene node.\n");
		
		std::string type = element->Attribute("type");
		if (type == "mesh")
		{
			MeshLevelNode *newnode = new MeshLevelNode;
			if (!newnode->load(element, rootnode))
			{
				delete newnode;
			}
			else
			{
				rootnode->addChild(newnode);
			}
		}
		else if (type == "terrain")
		{
			TerrainLevelNode *newnode = new TerrainLevelNode;
			if (!newnode->load(element, rootnode))
			{
				delete newnode;
			}
			else
			{
				rootnode->addChild(newnode);
			}
		}
		else
		{
			printf("Unknown scene node type: %s\n", type.c_str());
		}
		
		// Next pointgroup
		scenenode = node->IterateChildren("scenenode", scenenode);
	}
	
	return true;
}
bool LevelData::save(std::string file)
{
	return false;
}

void LevelData::show(void)
{
}
void LevelData::hide(void)
{
}

