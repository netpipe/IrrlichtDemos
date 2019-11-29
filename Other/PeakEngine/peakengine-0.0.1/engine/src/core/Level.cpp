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
#include "graphic/Terrain.h"
#include "physics/Heightfield.h"
#include "graphic/SkyBox.h"
#include "graphic/SkyDome.h"
#include "graphic/MeshSceneNode.h"
#include "physics/Geometry.h"
#include "physics/Cube.h"

namespace peak
{
	void LevelObject::setName(std::string name)
	{
		this->name = name;
	}
	std::string LevelObject::getName(void)
	{
		return name;
	}
	
	SceneNodeObject::SceneNodeObject(SceneNode *node, Geometry *geom) : node(node), geom(geom)
	{
	}
	SceneNodeObject::~SceneNodeObject()
	{
		if (node)
		{
			delete node;
		}
		if (geom)
		{
			geom->destroy();
			delete geom;
		}
	}
	
	PointGroupObject::PointGroupObject()
	{
	}
	PointGroupObject::~PointGroupObject()
	{
	}
	
	void PointGroupObject::addPoint(Vector3D point)
	{
		points.push_back(point);
	}
	int PointGroupObject::getPointCount()
	{
		return points.size();
	}
	Vector3D PointGroupObject::getPoint(int index)
	{
		return points[index];
	}
	void PointGroupObject::deletePoint(int index)
	{
		points.erase(points.begin() + index);
	}
	
	void PointGroupObject::addRotation(Vector3D rotation)
	{
		rotations.push_back(rotation);
	}
	int PointGroupObject::getRotationCount()
	{
		return rotations.size();
	}
	Vector3D PointGroupObject::getRotation(int index)
	{
		return rotations[index];
	}
	void PointGroupObject::deleteRotation(int index)
	{
		rotations.erase(rotations.begin() + index);
	}
	
	TerrainObject::TerrainObject(std::string heightmap, std::string material, Vector3D position, Vector3D scale, bool server, bool client)
	{
		terrain = 0;
		heightfield = 0;
		if (client)
		{
			terrain = new Terrain(heightmap);
			terrain->setMaterial(material);
			terrain->setScale(scale);
			terrain->setPosition(position);
		}
		if (server)
		{
			heightfield = new Heightfield;
			if (!heightfield->init(Game::get()->getWorld(), heightmap, scale))
			{
				delete heightfield;
				heightfield = 0;
			}
			else
			{
				heightfield->setPosition(position);
			}
		}
	}
	TerrainObject::~TerrainObject()
	{
		if (heightfield)
		{
			heightfield->destroy();
			delete heightfield;
		}
		if (terrain)
		{
			delete terrain;
		}
	}
	
	Level *Level::get(void)
	{
		static Level level;
		return &level;
	}
	Level::Level()
	{
	}
	Level::~Level()
	{
	}
	
	bool Level::init(std::string name, bool server, bool client)
	{
		IniFile file;
		if (!file.load(std::string("levels/") + name + ".level"))
		{
			LERROR("Could not open level file \"%s\".\n", name.c_str());
			return false;
		}
		
		// Load objects
		int objectcount = toInt(file.getItemValue("map", "objectcount"));
		
		for (int i = 0; i < objectcount; i++)
		{
			std::string category = std::string("object") + toString(i);
			std::string name = file.getItemValue(category, "name");
			std::string type = file.getItemValue(category, "type");
			
			if (type == "terrain")
			{
				std::string heightmap = file.getItemValue(category, "heightmap");
				std::string material = file.getItemValue(category, "material");
				Vector3D position = file.getItemValue3D(category, "position");
				Vector3D scale = file.getItemValue3D(category, "scale");
				TerrainObject *newobj = new TerrainObject(heightmap, material, position, scale, server, client);
				newobj->setName(name);
				objects.push_back(newobj);
			}
			else if (type == "skybox")
			{
				if (!client) continue;
				std::string top = file.getItemValue(category, "top");
				std::string bottom = file.getItemValue(category, "bottom");
				std::string left = file.getItemValue(category, "left");
				std::string right = file.getItemValue(category, "right");
				std::string front = file.getItemValue(category, "front");
				std::string back = file.getItemValue(category, "back");
				SkyBox *skybox = new SkyBox(top, bottom, left, right,
					front, back);
				SceneNodeObject *newobj = new SceneNodeObject(skybox);
				newobj->setName(name);
				objects.push_back(newobj);
			}
			else if (type == "skydome")
			{
				if (!client) continue;
				std::string texture = file.getItemValue(category, "texture");
				SkyDome *skydome = new SkyDome(texture, file.getItemValueF(category, "height"));
				SceneNodeObject *newobj = new SceneNodeObject(skydome);
				newobj->setName(name);
				objects.push_back(newobj);
			}
			else if (type == "mesh")
			{
				std::string model = file.getItemValue(category, "model");
				std::string material = file.getItemValue(category, "material");
				Vector3D position = file.getItemValue3D(category, "position");
				Vector3D scale = file.getItemValue3D(category, "scale");
				MeshSceneNode *node = new MeshSceneNode(model);
				node->setMaterial(material);
				node->setPosition(position);
				node->setScale(scale);
				std::string physics = file.getItemValue(category, "physics");
				Geometry *geom = 0;
				if (server && (physics == "cube"))
				{
					// TODO: Proper bounding box
					Cube *cube = new Cube;
					cube->init(Game::get()->getWorld(), scale.x * 2, scale.y * 2, scale.z * 2);
					position.y += scale.y;
					cube->setPosition(position);
					geom = cube;
				}
				SceneNodeObject *newobj = new SceneNodeObject(node, geom);
				newobj->setName(name);
				objects.push_back(newobj);
			}
			else if (type == "pointgroup")
			{
				PointGroupObject *newobj = new PointGroupObject;
				int pointcount = file.getItemValueI(category, "count");
				for (int i = 0; i < pointcount; i++)
				{
					Vector3D point = file.getItemValue3D(category, std::string("point") + toString(i));
					newobj->addPoint(point);
					if (file.getItemIndex(category, std::string("rotation") + toString(i)) != -1)
					{
						Vector3D rotation = file.getItemValue3D(category, std::string("rotation") + toString(i));
						rotation *= M_PI / 180;
						newobj->addRotation(rotation);
					}
				}
				newobj->setName(name);
				objects.push_back(newobj);
			}
			else
			{
				LERROR("Wrong object type for terrain object \"%s\": \"%s\".\n", name.c_str(), type.c_str());
				continue;
			}
		}
		return true;
	}
	bool Level::destroy(void)
	{
		std::list<LevelObject*>::iterator it = objects.begin();
		while (it != objects.end())
		{
			delete *it;
			it++;
		}
		objects.clear();
		return true;
	}
	
	LevelObject *Level::getObject(std::string name)
	{
		std::list<LevelObject*>::iterator it = objects.begin();
		while (it != objects.end())
		{
			if ((*it)->getName() == name)
			{
				return *it;
			}
			it++;
		}
		return 0;
	}
	PointGroupObject *Level::getPointGroup(std::string name)
	{
		return (PointGroupObject*)getObject(name);
	}
	
	int Level::getLevelCount(void)
	{
		return FileSystem::get()->listFiles("levels", ".level").size();
	}
	std::string Level::getLevelName(int index)
	{
		std::string filename = FileSystem::get()->listFiles("levels", ".level")[index];
		return filename.substr(0, filename.size() - 6);
	}
}
