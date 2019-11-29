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

#include "entity/Entity.h"
#include "core/IniFile.h"
#include "core/Util.h"
#include "core/Logger.h"
#include "entity/PhysicsComponent.h"
#include "entity/GraphicsComponent.h"
#include "entity/ServerScriptComponent.h"
#include "entity/ClientScriptComponent.h"
#include "entity/EntityVariables.h"
#include "physics/Body.h"
#include "physics/Cube.h"
#include "physics/Sphere.h"
#include "core/Game.h"
#include "core/GameEngine.h"
#include "core/FileSystem.h"
#include "core/Script.h"
#include "graphic/CubeSceneNode.h"
#include "graphic/SphereSceneNode.h"
#include "graphic/MeshSceneNode.h"

namespace peak
{
	Entity::Entity()
	{
		id = 0;
		priority = 10;
		currentpriority = 10;
		owner = 0;
		local = false;
	}
	Entity::~Entity()
	{
	}
	
	bool Entity::load(std::string name, bool server, bool graphics)
	{
		this->name = name;
		LDEBUG("Loading entity \"%s\".\n", name.c_str());
		IniFile file;
		if (!file.load(std::string("entities/") + name + "/entity.ini"))
		{
			LERROR("Could not load entity file \"%s\"\n", (std::string("entities/") + name + "/entity.ini").c_str());
			return false;
		}
		
		// Get general info
		std::string entname = file.getItemValue("general", "name");
		if (entname != name)
		{
			LWARNING("Entity has wrong name (\"%s\" instead of \"%s\").\n", entname.c_str(), name.c_str());
		}
		priority = file.getItemValueI("general", "priority");
		
		this->id = ++lastid;
	
		// Load variables
		variables = new EntityVariables;
		variables->load(&file);
		
		// Load physics component
		if ((file.getSectionIndex("physics") != -1) && server)
		{
			LDEBUG("PHYSICS!\n");
			PhysicsComponent *physics = new PhysicsComponent;
			if (!physics->init(this))
			{
				LERROR("Could not create physics component.\n");
				return false;
			}
			// Load bodies
			if (file.getItemIndex("physics", "bodies") != -1)
			{
				int bodycount = file.getItemValueI("physics", "bodies");
				for (int i = 0; i < bodycount; i++)
				{
					std::string sectionname = std::string("body") + toString(i);
					if (file.getSectionIndex(sectionname) != -1)
					{
						int id = file.getItemValueI(sectionname, "id");
						Body *newbody = new Body;
						newbody->init(Game::get()->getWorld(), this);
						Vector3D position = file.getItemValue3D(sectionname, "position");
						newbody->setPosition(position);
						bool gravity = true;
						if (file.getItemIndex(sectionname, "gravity") != -1)
						{
							gravity = file.getItemValueB(sectionname, "gravity");
						}
						newbody->setGravity(gravity);
						physics->addBody(newbody, id);
					}
				}
			}
			// Load geoms
			if (file.getItemIndex("physics", "geoms") != -1)
			{
				int geomcount = toInt(file.getItemValue("physics", "geoms"));
				for (int i = 0; i < geomcount; i++)
				{
					std::string sectionname = std::string("geom") + toString(i);
					if (file.getSectionIndex(sectionname) != -1)
					{
						int id = file.getItemValueI(sectionname, "id");
						std::string type = file.getItemValue(sectionname, "type");
						Vector3D scale = Vector3D(1, 1, 1);
						if (file.getItemIndex(sectionname, "scale") != -1)
						{
							scale = file.getItemValue3D(sectionname, "scale");
						}
						Geometry *newgeom = 0;
						if (type == "box")
						{
							newgeom = new Cube;
							((Cube*)newgeom)->init(Game::get()->getWorld(), scale.x, scale.y, scale.z);
						}
						else if (type == "sphere")
						{
							newgeom = new Sphere;
							float radius = 1;
							radius = toFloat(file.getItemValue(sectionname, "radius"));
							((Sphere*)newgeom)->init(Game::get()->getWorld(), radius);
						}
						else
						{
							LERROR("Unknown geom type (%s).\n", type.c_str());
							continue;
						}
						Vector3D position = file.getItemValue3D(sectionname, "position");
						newgeom->setPosition(position);
						physics->addGeom(newgeom, id, file.getItemValueI(sectionname, "parent"));
					}
				}
			}
			
			addComponent(physics);
		}
		
		// Load server script
		if (server)
		{
			ServerScriptComponent *serverscript = new ServerScriptComponent;
			serverscript->init(this);
			std::string filename = FileSystem::get()->getFileExists(std::string("/entities/") + name + "/server.lua");
			if ((filename != "") && serverscript->runScript(filename))
			{
				addComponent(serverscript);
				serverscript->getScript()->callFunction("create", UserClass("peak::Entity", this),
					UserClass("peak::ServerScriptComponent", serverscript));
			}
			else
			{
				delete serverscript;
			}
		}
		
		// Load client script
		ClientScriptComponent *clientscript = 0;
		if (graphics)
		{
			clientscript = new ClientScriptComponent;
			clientscript->init(this);
			std::string filename = FileSystem::get()->getFileExists(std::string("/entities/") + name + "/client.lua");
			if ((filename != "") && clientscript->runScript(filename))
			{
				addComponent(clientscript);
				clientscript->getScript()->callFunction("create", UserClass("peak::Entity", this));
			}
			else
			{
				delete clientscript;
				clientscript = 0;
			}
		}
		
		// Load graphics component
		if (file.getSectionIndex("graphics") != -1)
		{
			LDEBUG("GRAPHICS!\n");
			GraphicsComponent *graphics = new GraphicsComponent;
			if (!graphics->init(this))
			{
				LERROR("Could not create graphics component.\n");
				return false;
			}
			// Load scene nodes
			if (file.getItemIndex("graphics", "scenenodes") != -1)
			{
				int scenenodecount = toInt(file.getItemValue("graphics", "scenenodes"));
				for (int i = 0; i < scenenodecount; i++)
				{
					std::string sectionname = std::string("scenenode") + toString(i);
					if (file.getSectionIndex(sectionname) != -1)
					{
						int id = file.getItemValueI(sectionname, "id");
						std::string type = file.getItemValue(sectionname, "type");
						SceneNode *newnode = 0;
						if (type == "cube")
						{
							newnode = new CubeSceneNode(1);
						}
						else if (type == "sphere")
						{
							newnode = new SphereSceneNode(1);
						}
						else if (type == "mesh")
						{
							std::string filename = file.getItemValue(sectionname, "file");
							newnode = new MeshSceneNode(filename);
							LERROR("Loading mesh %s!\n", filename.c_str());
						}
						else
						{
							LERROR("Unsupported scene node type (%s).\n", type.c_str());
							return false;
						}
						Vector3D position = file.getItemValue3D(sectionname, "position");
						Vector3D scale = Vector3D(1, 1, 1);
						if (file.getItemIndex(sectionname, "scale") != -1)
						{
							scale = file.getItemValue3D(sectionname, "scale");
						}
						newnode->setScale(scale);
						if (file.getItemIndex(sectionname, "material") != -1)
						{
							newnode->setMaterial(file.getItemValue(sectionname, "material"));
						}
						graphics->addSceneNode(newnode, id, file.getItemValueI(sectionname, "parent"));
						newnode->setPosition(position);
					}
				}
			}
			
			addComponent(graphics);
			if (clientscript)
			{
				if (clientscript->getScript()->isFunction("graphics_created"))
				{
					clientscript->getScript()->callFunction("graphics_created", UserClass("peak::GraphicsComponent", graphics));
				}
			}
		}
		
		return true;
	}
	bool Entity::destroy(void)
	{
		std::deque<Component*>::iterator it = components.begin();
		while (it != components.end())
		{
			(*it)->destroy();
			delete (*it);
			it++;
		}
		delete variables;
		return false;
	}
	
	std::string Entity::getName(void)
	{
		return name;
	}
	void Entity::setID(int id)
	{
		this->id = id;
	}
	int Entity::getID(void)
	{
		return id;
	}
	
	void Entity::setLocal(bool local)
	{
		this->local = local;
	}
	bool Entity::isLocal(void)
	{
		return local;
	}
	void Entity::setOwner(int owner)
	{
		this->owner = owner;
	}
	int Entity::getOwner(void)
	{
		return owner;
	}
	
	void Entity::setPriority(int priority)
	{
		this->priority = priority;
		if (currentpriority > priority)
		{
			currentpriority = priority;
		}
	}
	int Entity::getPriority(void)
	{
		return priority;
	}
	void Entity::setCurrentPriority(int priority)
	{
		currentpriority = priority;
	}
	int Entity::getCurrentPriority(void)
	{
		return currentpriority;
	}
	
	bool Entity::needsUpdate(void)
	{
		std::deque<Component*>::iterator it = components.begin();
		while (it != components.end())
		{
			if ((*it)->needsUpdate())
			{
				return true;
			}
			it++;
		}
		return false;
	}
	int Entity::getUpdateSize(void)
	{
		return 0;
	}
	int Entity::writeUpdateData(Buffer *buffer)
	{
		std::deque<Component*>::iterator it = components.begin();
		while (it != components.end())
		{
			(*it)->writeUpdateData(buffer);
			it++;
		}
		return true;
	}
	
	bool Entity::updateFromData(Buffer *buffer)
	{
		std::deque<Component*>::iterator it = components.begin();
		while (it != components.end())
		{
			(*it)->updateFromData(buffer);
			it++;
		}
		return true;
	}
	
	bool Entity::writeCompleteData(Buffer *buffer)
	{
		std::deque<Component*>::iterator it = components.begin();
		while (it != components.end())
		{
			(*it)->writeCompleteData(buffer);
			it++;
		}
		variables->writeCompleteData(buffer);
		return true;
	}
	bool Entity::readCompleteData(Buffer *buffer)
	{
		std::deque<Component*>::iterator it = components.begin();
		while (it != components.end())
		{
			(*it)->readCompleteData(buffer);
			it++;
		}
		variables->readCompleteData(buffer);
		return true;
	}
	
	void Entity::addComponent(Component *component)
	{
		LDEBUG("Adding: %d\n", component->getType());
		components.push_back(component);
	}
	void Entity::deleteComponent(ComponentType type)
	{
		std::deque<Component*>::iterator it = components.begin();
		while (it != components.end())
		{
			if ((*it)->getType() == type)
			{
				components.erase(it);
				return;
			}
			it++;
		}
	}
	Component *Entity::getComponent(ComponentType type)
	{
		LDEBUG("Getting component %d\n", (int)type);
		std::deque<Component*>::iterator it = components.begin();
		while (it != components.end())
		{
			LDEBUG("Type: %d\n", (int)(*it)->getType());
			if ((*it)->getType() == type)
			{
				return *it;
			}
			it++;
		}
		LDEBUG("Not found.\n");
		return 0;
	}
	PhysicsComponent *Entity::getPhysicsComponent(void)
	{
		return (PhysicsComponent*)getComponent(ECT_Physics);
	}
	GraphicsComponent *Entity::getGraphicsComponent(void)
	{
		return (GraphicsComponent*)getComponent(ECT_Graphics);
	}
	ServerScriptComponent *Entity::getServerScriptComponent(void)
	{
		return (ServerScriptComponent*)getComponent(ECT_ServerScript);
	}
	
	EntityVariables *Entity::getVariables(void)
	{
		return variables;
	}
	
	bool Entity::doWork(float msecs)
	{
		std::deque<Component*>::iterator it = components.begin();
		while (it != components.end())
		{
			(*it)->doWork(msecs);
			it++;
		}
		return true;
	}
	
	int Entity::lastid = 0;
}

