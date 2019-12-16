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

#include "core/Level.h"
#include "core/Util.h"
#include "core/IniFile.h"
#include "core/Logger.h"
#include "core/FileSystem.h"
#include "core/Game.h"
#include "core/Script.h"
#include "graphics/MeshSceneNode.h"
#include "graphics/ParticleSystem.h"
#include "graphics/GraphicsEngine.h"
#include "graphics/Terrain.h"
#include "physics/Shape.h"
#include "physics/Body.h"
#include "physics/Heightfield.h"
#include "physics/Box.h"
#include "physics/Trimesh.h"
#include "physics/Sphere.h"

#include <math.h>
#include "support/tinyxml.h"

namespace peak
{
	PointGroup::PointGroup()
	{
	}
	PointGroup::~PointGroup()
	{
	}

	void PointGroup::addPoint(Vector3D point)
	{
		points.push_back(point);
	}
	int PointGroup::getPointCount()
	{
		return points.size();
	}
	Vector3D PointGroup::getPoint(int index)
	{
		return points[index];
	}
	void PointGroup::deletePoint(int index)
	{
		points.erase(points.begin() + index);
	}

	void PointGroup::addRotation(Vector3D rotation)
	{
		rotations.push_back(rotation);
	}
	int PointGroup::getRotationCount()
	{
		return rotations.size();
	}
	Vector3D PointGroup::getRotation(int index)
	{
		return rotations[index];
	}
	void PointGroup::deleteRotation(int index)
	{
		rotations.erase(rotations.begin() + index);
	}

	Level *Level::get(void)
	{
		static Level level;
		return &level;
	}
	Level::Level()
	{
		script = 0;
	}
	Level::~Level()
	{
	}

	bool Level::init(std::string name, bool server, bool client)
	{
		// Open XML file
		std::string filename = FileSystem::get()->getFileExists(std::string("levels/") + name + ".xml");
		if (filename == "")
		{
			LERROR("Could not find file %s.\n", (std::string("levels/") + name + ".xml").c_str());
			return false;
		}
		TiXmlDocument xml(filename.c_str());
		if (!xml.LoadFile() || xml.Error())
		{
			LERROR("Could not load XML file %s.xml: %s.\n", name.c_str(), xml.ErrorDesc());
			return false;
		}

		script = new Script;
		script->registerVariable(UserClass("peak::Level", this), "level");
		
		// Read in file
		TiXmlNode *node = xml.FirstChild("level");
		if (!node)
		{
			LERROR("Parser error: <level> not found.\n");
			return false;
		}
		TiXmlElement *root = node->ToElement();
		
		// Read in point groups
		TiXmlNode *pointgroup = root->FirstChild("pointgroup");
		while (pointgroup)
		{
			TiXmlElement *pointgroupelement = pointgroup->ToElement();
			if (pointgroupelement)
			{
				loadPointGroup(pointgroupelement);
			}
			// Next pointgroup
			pointgroup = node->IterateChildren("pointgroup", pointgroup);
		}
		
		// Read in scene nodes
		TiXmlNode *scenenode = root->FirstChild("scenenode");
		while (scenenode)
		{
			TiXmlElement *scenenodeelement = scenenode->ToElement();
			if (scenenodeelement)
			{
				loadSceneNode(scenenodeelement);
			}
			// Next pointgroup
			scenenode = node->IterateChildren("scenenode", scenenode);
		}

		// Read in shapes
		TiXmlNode *shapenode = root->FirstChild("shape");
		while (shapenode)
		{
			TiXmlElement *shapeelement = shapenode->ToElement();
			if (shapeelement)
			{
				loadShape(shapeelement);
			}
			// Next pointgroup
			shapenode = node->IterateChildren("shape", shapenode);
		}
		
		GraphicsEngine::get()->saveMessages();
		return true;
	}
	bool Level::destroy(void)
	{
		// Delete point groups
		{
			std::map<std::string, PointGroup*>::iterator it = points.begin();
			while (it != points.end())
			{
				delete it->second;
				it++;
			}
			points.clear();
		}
		// Delete shapes
		{
			std::map<std::string, ShapeData*>::iterator it = shapes.begin();
			while (it != shapes.end())
			{
				it->second->body->destroy();
				delete it->second->body;
				it->second->shape->destroy();
				delete it->second->shape;
				delete it->second;
				it++;
			}
			shapes.clear();
		}
		// Delete terrains
		{
			std::map<std::string, Terrain*>::iterator it = terrains.begin();
			while (it != terrains.end())
			{
				it->second->destroy();
				delete it->second;
				it++;
			}
			terrains.clear();
		}
		// Delete only scene nodes without any parent
		std::list<SceneNode*> topnodes;
		std::map<std::string, SceneNode*>::iterator sit = scenenodes.begin();
		while (sit != scenenodes.end())
		{
			if (!sit->second->getParent())
				topnodes.push_back(sit->second);
			sit++;
		}
		std::list<SceneNode*> ::iterator sit2 = topnodes.begin();
		while (sit2 != topnodes.end())
		{
			delete *sit2;
			sit2++;
		}
		scenenodes.clear();

		return true;
	}

	PointGroup *Level::getPointGroup(std::string name)
	{
		if (points.find(name) != points.end())
		{
			return points[name];
		}
		return 0;
	}
	
	Script *Level::getScript(void)
	{
		return script;
	}

	int Level::getLevelCount(void)
	{
		return FileSystem::get()->listFiles("levels", ".xml").size();
	}
	std::string Level::getLevelName(int index)
	{
		std::string filename = FileSystem::get()->listFiles("levels", ".xml")[index];
		return filename.substr(0, filename.size() - 4);
	}
	
	bool Level::loadPointGroup(TiXmlElement *element)
	{
		if (!element->Attribute("name"))
		{
			LERROR("Missing 'name' in point group.\n");
			return false;
		}
		std::string name = element->Attribute("name");
		
		// Create point group
		PointGroup *pointgroup = new PointGroup;
		
		// Add points
		TiXmlNode *pointnode = element->FirstChild("point");
		while (pointnode)
		{
			TiXmlElement *pointelement = pointnode->ToElement();
			if (pointelement)
			{
				Vector3D position(0, 0, 0);
				if (pointelement->Attribute("position"))
				{
					position = toVector3D(pointelement->Attribute("position"));
				}
				Vector3D rotation(0, 0, 0);
				if (pointelement->Attribute("rotation"))
				{
					rotation = toVector3D(pointelement->Attribute("rotation"));
				}
				pointgroup->addPoint(position);
				pointgroup->addRotation(rotation);
			}

			// Next scenenode
			pointnode = element->IterateChildren("point", pointnode);
		}
		
		// Add point group to level
		points.insert(std::pair<std::string, PointGroup*>(name, pointgroup));
		script->registerVariable(UserClass("peak::PointGroup", pointgroup), name);

		return true;
	}
	SceneNode *Level::loadSceneNode(TiXmlElement *element)
	{
		SceneNode *node = 0;
		if (!element->Attribute("name") || !element->Attribute("type"))
		{
			LERROR("Missing 'name' or 'type' in scenenode.\n");
			return 0;
		}
		std::string name = element->Attribute("name");
		std::string type = element->Attribute("type");

		// Create scene node
		if (type == "mesh")
		{
			std::string mesh = element->Attribute("file");
			node = new MeshSceneNode(mesh);
			Vector3D scale(1, 1, 1);
			if (element->Attribute("scale"))
			{
			    scale = toVector3D(element->Attribute("scale"));
			    node->setScale(scale);
			}

			scenenodes.insert(std::pair<std::string, SceneNode*>(name, node));
			script->registerVariable(UserClass("peak::MeshSceneNode", node), name);
		}
		else if (type == "particlesystem")
		{
			std::string file = element->Attribute("file");
			node = new ParticleSystem(file);
			Vector3D scale(1, 1, 1);
			if (element->Attribute("scale"))
			{
			    scale = toVector3D(element->Attribute("scale"));
			    node->setScale(scale);
			}

			scenenodes.insert(std::pair<std::string, SceneNode*>(name, node));
			script->registerVariable(UserClass("peak::ParticleSystem", node), name);
		}
		if (node != 0)
		{
			// Set position/rotation
			if (element->Attribute("position"))
			{
			    Vector3D position = toVector3D(element->Attribute("position"));
			    node->setPosition(position);
			}
			// Set position/rotation
			if (element->Attribute("rotation"))
			{
			    Vector3D rotation = toVector3D(element->Attribute("rotation"));
			    node->setRotation(rotation);
			}
			// Get children
			std::vector<SceneNode*> scenenodes;
			// Get scenenodes
			TiXmlNode *scenenodenode = element->FirstChild("scenenode");
			while (scenenodenode)
			{
				TiXmlElement *scenenodeelement = scenenodenode->ToElement();
				if (scenenodeelement)
				{
					SceneNode *scenenode = loadSceneNode(scenenodeelement);
					if (scenenode)
					{
						scenenode->setParent(node);
					}
				}

				// Next scenenode
				scenenodenode = element->IterateChildren("scenenode", scenenodenode);
			}
		}
		return node;
	}
	
	ShapeData *Level::loadShape(TiXmlElement *element)
	{
		if (!element->Attribute("name") || !element->Attribute("type"))
		{
			LERROR("Missing 'name' or 'type' in shape.\n");
			return 0;
		}
		std::string name = element->Attribute("name");
		std::string type = element->Attribute("type");

		ShapeData *data = new ShapeData(0, 0);
		// Create shape
		if (type == "box")
		{
			Vector3D size(1, 1, 1);
			if (element->Attribute("size"))
			{
				std::string sizestr = element->Attribute("size");
				size = toVector3D(sizestr);
			}
			size *= 0.5;
			float mass = 0.0f;
			if (element->Attribute("mass"))
			{
				std::string massstr = element->Attribute("mass");
				mass = toFloat(massstr);
			}

			Box *box = new Box();
			box->create(size, mass);
			script->registerVariable(UserClass("peak::Box", box), name);
			data->shape = box;
		}
		else if(type == "trimesh")
		{
			Vector3D scale(1,1,1);
			if (element->Attribute("scale"))
			{
				std::string scalestr = element->Attribute("scale");
				scale = toVector3D(scalestr);
			}
			std::string modelfile;
			if (element->Attribute("mesh"))
			{
				std::string modelfile = element->Attribute("mesh");
			}
			float mass = 0.0f;
			if (element->Attribute("mass"))
			{
				std::string massstr = element->Attribute("mass");
				mass = toFloat(massstr);
			}


			Trimesh *trimesh = new Trimesh();
			trimesh->create(element->Attribute("mesh"), mass, scale);
			script->registerVariable(UserClass("peak::Trimesh", trimesh), name);
			data->shape = trimesh;
		}
		else if (type == "sphere")
		{
			float radius = 1.0f;
			if (element->Attribute("radius"))
			{
				radius = toFloat(element->Attribute("radius"));
			}
			float mass = 0.0f;
			if (element->Attribute("mass"))
			{
				std::string massstr = element->Attribute("mass");
				mass = toFloat(massstr);
			}

			Sphere *sphere = new Sphere();
			sphere->create(radius, mass);
			script->registerVariable(UserClass("peak::Sphere", sphere), name);
			data->shape = sphere;
		}
		else if (type == "heightfield")
		{
			Vector3D scale(1,1,1);
			if (element->Attribute("scale"))
			{
				std::string scalestr = element->Attribute("scale");
				scale = toVector3D(scalestr);
			}
			float mass = 0.0f;
			if (element->Attribute("mass"))
			{
				std::string massstr = element->Attribute("mass");
				mass = toFloat(massstr);
			}
			if (!element->Attribute("file"))
			{
				LERROR("Not heightmap given for heightfield.\n");
				delete data;
				return 0;
			}
			std::string heightmap = element->Attribute("file");

			Heightfield *heightfield = new Heightfield();
			heightfield->create(heightmap, mass, scale);
			script->registerVariable(UserClass("peak::Heightfield", heightfield), name);
			data->shape = heightfield;
		}
		else
		{
			LERROR("Invalid shape type: %s\n", type.c_str());
			delete data;
			return 0;
		}
		
		// Create body
		Body *body = new Body;
		body->create(Game::get()->getWorld(), data->shape);
		script->registerVariable(UserClass("peak::Body", body), name + "_body");
		data->body = body;
		shapes.insert(std::pair<std::string, ShapeData*>(name, data));
		return data;
	}
}

