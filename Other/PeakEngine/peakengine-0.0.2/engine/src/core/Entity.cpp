
/*
Copyright (C) 2008   Mathias Gottschlag, Christian Reiser

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

#include "core/Entity.h"
#include "core/Logger.h"
#include "core/FileSystem.h"
#include "core/Util.h"
#include "core/DirectLink.h"
#include "core/Game.h"
#include "core/Property.h"
#include "core/Buffer.h"
#include "core/Script.h"
#include "core/InputManager.h"
#include "core/EventScriptLink.h"
#include "core/EventRemoteLink.h"
#include "core/Event.h"
#include "core/PropertyScriptLink.h"
#include "core/NetworkData.h"
#include "physics/Box.h"
#include "physics/Body.h"
#include "physics/Trimesh.h"
#include "physics/World.h"
#include "physics/Sphere.h"
#include "graphics/MeshSceneNode.h"
#include "graphics/CameraSceneNode.h"
#include "graphics/GraphicsEngine.h"
#include "graphics/ParticleSystem.h"
#include "graphics/LabelOverlay.h"
#include "network/Connection.h"

#include "support/tinyxml.h"

namespace peak
{
	Entity::Entity()
	{
		id = ++lastid;
		owner = 0;
		local = false;
	}
	Entity::~Entity()
	{
		if (script->isFunction("OnUpdate"))
		{
			// Delete entity from update list
			std::list<Entity*>::iterator it = updateentities.begin();
			while (it != updateentities.end())
			{
				if (*it == this)
				{
					updateentities.erase(it);
					break;
				}
				it++;
			}
		}
		
		// Call destroy function
		if (script->isFunction("OnDestroy"))
		{
			script->callFunction("OnDestroy");
		}
		
		// Delete script
		if (script)
		{
			delete script;
		}

		// Delete rest of entity
		for (unsigned int i = 0; i < links.size(); i++)
		{
			if (links[i])
			{
				links[i]->destroy();
				delete links[i];
			}
		}
		std::map<std::string, Shape*>::iterator it = shapes.begin();
		while (it != shapes.end())
		{
			it->second->destroy();
			delete it->second;
			it++;
		}
		std::map<std::string, Body*>::iterator bit = bodies.begin();
		while (bit != bodies.end())
		{
			bit->second->destroy();
			delete bit->second;
			bit++;
		}
		std::map<std::string, Overlay*>::iterator oit = overlays.begin();
		while (oit != overlays.end())
		{
			delete oit->second;
			oit++;
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
	}

	bool Entity::load(std::string name, Buffer *data)
	{
		this->name = name;

		// Open XML file
		std::string filename = FileSystem::get()->getFileExists(std::string("entities/") + name + ".xml");
		if (filename == "")
		{
			LERROR("Could not find file %s.\n", (std::string("entities/") + name + ".xml").c_str());
			return false;
		}
		TiXmlDocument xml(filename.c_str());
		if (!xml.LoadFile() || xml.Error())
		{
			LERROR("Could not load XML file %s.xml: %s.\n", name.c_str(), xml.ErrorDesc());
			return false;
		}

		script = new Script;
		script->registerVariable(UserClass("peak::Entity", this), "entity");

		// Read in file
		TiXmlNode *node = xml.FirstChild("entity");
		if (!node)
		{
			LERROR("Parser error: <entity> not found.\n");
			return false;
		}
		TiXmlElement *root = node->ToElement();
		// Read properties
		TiXmlNode *property = root->FirstChild("property");
		while (property)
		{
			TiXmlElement *propelement = property->ToElement();
			if (propelement)
			{
				if (!propelement->Attribute("name") || !propelement->Attribute("type"))
				{
					LERROR("Missing 'name' or 'type' in property.\n");
					return false;
				}
				std::string propname = propelement->Attribute("name");
				std::string proptype = propelement->Attribute("type");

				// Create property
				if (proptype == "vector3")
				{
					Property *newprop = new Vector3DProperty(propname);
					addProperty(propname, newprop);
					script->registerVariable(UserClass("peak::Property", newprop), propname);
				}
				else if (proptype == "quaternion")
				{
					Property *newprop = new QuaternionProperty(propname);
					addProperty(propname, newprop);
					script->registerVariable(UserClass("peak::Property", newprop), propname);
				}
				else if (proptype == "integer")
				{
					Property *newprop = new IntProperty(propname);
					addProperty(propname, newprop);
					script->registerVariable(UserClass("peak::Property", newprop), propname);
					if (propelement->Attribute("value"))
					{
						newprop->set(toInt(propelement->Attribute("value")));
					}
				}
			}
			// Next property
			property = node->IterateChildren("property", property);
		}
		// Read physics/scenenodes
		TiXmlNode *bodynode = root->FirstChild("body");
		while (bodynode)
		{
			TiXmlElement *bodyelement = bodynode->ToElement();
			if (bodyelement)
			{
				loadBody(bodyelement);
			}

			// Next body
			bodynode = node->IterateChildren("body", bodynode);
		}
		// Read scenenodes
		TiXmlNode *scenenodenode = root->FirstChild("scenenode");
		while (scenenodenode)
		{
			TiXmlElement *scenenodeelement = scenenodenode->ToElement();
			if (scenenodeelement)
			{
				loadSceneNode(scenenodeelement);
			}

			// Next body
			scenenodenode = node->IterateChildren("scenenode", scenenodenode);
		}
		// Read overlays
		TiXmlNode *overlaynode = root->FirstChild("overlay");
		while (overlaynode)
		{
			TiXmlElement *overlayelement = overlaynode->ToElement();
			if (overlayelement)
			{
				loadOverlay(overlayelement);
			}

			// Next body
			overlaynode = node->IterateChildren("overlay", overlaynode);
		}
		// Read aliases
		property = root->FirstChild("property");
		while (property)
		{
			TiXmlElement *propelement = property->ToElement();
			if (propelement)
			{
				if (!propelement->Attribute("name") || !propelement->Attribute("type"))
				{
					LERROR("Missing 'name' or 'type' in property.\n");
					return false;
				}
				std::string propname = propelement->Attribute("name");
				std::string proptype = propelement->Attribute("type");

				// Create aliases
				if (proptype == "alias")
				{
					std::string target = propelement->Attribute("value");
					std::string object = target.substr(0, target.find('.'));
					std::string prop = target.substr(target.find('.') + 1);
					if (bodies.find(object) != bodies.end())
					{
						Body *body = bodies[object];
						if (body->getProperty(prop))
						{
							addAlias(propname, body->getProperty(prop));
						}
					}
					else if (scenenodes.find(object) != scenenodes.end())
					{
						printf("Scene node alias.\n");
						SceneNode *node = scenenodes[object];
						if (node->getProperty(prop))
						{
							addAlias(propname, node->getProperty(prop));
						}
					}
				}
			}
			// Next property
			property = node->IterateChildren("property", property);
		}

		// Read events

		// Read links
		TiXmlNode *link = root->FirstChild("link");
		while (link)
		{
			TiXmlElement *linkelement = link->ToElement();
			if (linkelement)
			{
				loadLink(linkelement);
			}
			// Next link
			link = node->IterateChildren("link", link);
		}

		// Update from data
		if (data)
		{
			// Update bodies
			unsigned int bodycount = data->readInt();
			if (bodycount != bodies.size())
			{
				LWARNING("Got wrong number of bodies from server (%d instead of %d).\n", bodycount, bodies.size());
			}
			std::map<std::string, Body*>::iterator it = bodies.begin();
			while (it != bodies.end() && bodycount)
			{
				it->second->setID(data->readInt());
				Game::get()->getWorld()->addSynchronizedBody(it->second, it->second->getID());
				bodycount--;
				it++;
			}
			// Update properties
		}

		// Load script
		TiXmlNode *scriptnode = root->FirstChild("script");
		while (scriptnode)
		{
			TiXmlElement *scriptelement = scriptnode->ToElement();
			if (scriptelement)
			{
				bool loadscript = true;
				if (scriptelement->Attribute("place"))
				{
					std::string place = scriptelement->Attribute("place");
					if (place == "server" && !Game::get()->isServer())
					{
						loadscript = false;
					}
					else if (place == "client" && Game::get()->isServer())
					{
						loadscript = false;
					}
					else if (place == "local" && !isLocal())
					{
						loadscript = false;
					}
				}
				if (loadscript)
				{
					TiXmlNode *scripttext = scriptelement->FirstChild();
					if (scripttext && scripttext->ToText())
					{
						TiXmlText *text = scripttext->ToText();
						std::string scriptdata = text->Value();
						script->runString(scriptdata);
					}
				}
			}
			// Next script
			scriptnode = node->IterateChildren("script", scriptnode);
		}

		// Call create function
		if (script->isFunction("OnCreate"))
		{
			script->callFunction("OnCreate");
		}

		// Add to update list if needed
		if (script->isFunction("OnUpdate"))
		{
			updateentities.push_back(this);
		}

		return true;
	}

	void Entity::setOwner(int owner)
	{
		this->owner = owner;
	}
	int Entity::getOwner(void)
	{
		return owner;
	}

	void Entity::setLocal(bool local)
	{
		this->local = local;
	}
	bool Entity::isLocal(void)
	{
		return local;
	}

	void Entity::setID(int id)
	{
		this->id = id;
	}
	int Entity::getID(void)
	{
		return id;
	}

	bool Entity::writeStartData(Buffer *data)
	{
		// Write body data
		data->writeInt(bodies.size());
		std::map<std::string, Body*>::iterator it = bodies.begin();
		while (it != bodies.end())
		{
			data->writeInt(it->second->getID());
			it++;
		}

		// Write properties
		return false;
	}

	void Entity::setPosition(Vector3D position)
	{
		Property *pos = getProperty("position");
		if (pos)
		{
			pos->set(position);
		}
	}

	std::string Entity::getName(void)
	{
		return name;
	}
	
	Body *Entity::getBody(std::string name)
	{
		if (bodies.find(name) != bodies.end())
		{
			return bodies[name];
		}
		return 0;
	}
	SceneNode *Entity::getSceneNode(std::string name)
	{
		if (scenenodes.find(name) != scenenodes.end())
		{
			return scenenodes[name];
		}
		return 0;
	}
	Script *Entity::getScript(void)
	{
		return script;
	}

	void Entity::updateAll(float msecs)
	{
		// Call update callbacks
		std::list<Entity*>::iterator it = updateentities.begin();
		while (it != updateentities.end())
		{
			(*it)->getScript()->callFunction("OnUpdate", msecs);
			it++;
		}
	}

	void Entity::updateLink(Buffer *buffer)
	{
		int entityid = buffer->readInt();
		int link = buffer->readInt();
		// Read event data
		int valuecount = buffer->readInt();
		EventData data("");
		for (int i = 0; i < valuecount; i++)
		{
			std::string valuename = buffer->readString();
			float value = buffer->readFloat();
			data.set(valuename, value);
		}
		// Update link
		Entity *entity = Game::get()->getEntity(entityid);
		if (!entity)
		{
			LWARNING("Link target not found (%X).\n", entityid);
			return;
		}
		if ((int)entity->links.size() <= link)
		{
			LWARNING("Link target link not found (%d).\n", link);
			return;
		}
		
		// FIXME: This is ugly and can crash
		((EventScriptLink*)entity->links[link])->update(&data);
	}
	
	void Entity::onPropertyChanged(std::string name)
	{
		Property *property = 0;
		int propertyindex = 0;
		// Look for property
		// TODO: This is innefficient
		std::map<std::string, Property*>::iterator it = properties.begin();
		while (it != properties.end())
		{
			if (it->first == name)
			{
				property = it->second;
				break;
			}
			propertyindex++;
			it++;
		}
		if (!property) return;
		
		// Send property data
		Buffer *msg = new Buffer;
		msg->writeByte(ENM_UpdateProperty);
		msg->writeInt(getID());
		msg->writeInt(propertyindex);
		property->serialize(msg);
		if (Game::get()->isServer())
		{
			Game::get()->sendDataToClients(msg, true);
		}
		else
		{
			Connection *server = Game::get()->getServerConnection();
			server->sendData(msg, true);
		}
		delete msg;
	}
	void Entity::updateProperty(Buffer *buffer)
	{
		int entityid = buffer->readInt();
		int property = buffer->readInt();
		
		// Look for property
		Entity *entity = Game::get()->getEntity(entityid);
		if (!entity) return;
		std::map<std::string, Property*>::iterator it = entity->properties.begin();
		while (it != entity->properties.end())
		{
			if (property == 0)
			{
				// Update property
				it->second->disableUpdates();
				it->second->deserialize(buffer);
				it->second->enableUpdates();
			}
			property--;
			it++;
		}
	}

	bool Entity::loadBody(TiXmlElement *element)
	{
		if (!element->Attribute("name"))
		{
			LERROR("Missing 'name' in body.\n");
			return false;
		}
		std::string bodyname = element->Attribute("name");
		std::vector<Shape*> shapes;
		// Get shapes
		TiXmlNode *shapenode = element->FirstChild("shape");
		while (shapenode)
		{
			TiXmlElement *shapeelement = shapenode->ToElement();
			if (shapeelement)
			{
				Shape *shape = loadShape(shapeelement);
				if (shape)
				{
					shapes.push_back(shape);
				}
			}

			// Next shape
			shapenode = element->IterateChildren("shape", shapenode);
		}

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
					scenenodes.push_back(scenenode);
				}
			}

			// Next scenenode
			scenenodenode = element->IterateChildren("scenenode", scenenodenode);
		}

		// Create body
		if (shapes.size() == 1)
		{
			Body *body = new Body();
			body->create(Game::get()->getWorld(), shapes[0]);
			bodies.insert(std::pair<std::string, Body*>(bodyname, body));
			script->registerVariable(UserClass("peak::Body", body), bodyname);
			body->setEntity(this);
			if (Game::get()->isServer())
			{
				body->setID(Game::get()->getWorld()->getBodyNetworkID());
				Game::get()->getWorld()->addSynchronizedBody(body, body->getID());
			}
			if (element->Attribute("gravity"))
			{
				body->setGravity(toVector3D(element->Attribute("gravity")));
			}

			// Add links between body and scene nodes
			for (unsigned int i = 0; i < scenenodes.size(); i++)
			{
				DirectLink *link = new DirectLink;
				link->create(body->getProperty("position"), scenenodes[i]->getProperty("position"));
				link = new DirectLink;
				link->create(body->getProperty("rotation"), scenenodes[i]->getProperty("rotation"));
			}
			return true;
		}
		else if (shapes.size() == 0)
		{
			LERROR("No shape specified for body.\n");
			return false;
		}
		else
		{
			LERROR("Multiple shapes per body not yet supported.\n");
			return false;
		}
	}
	Shape *Entity::loadShape(TiXmlElement *element)
	{
		if (!element->Attribute("name") || !element->Attribute("type"))
		{
			LERROR("Missing 'name' or 'type' in shape.\n");
			return 0;
		}
		std::string name = element->Attribute("name");
		std::string type = element->Attribute("type");

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
			float mass = 1.0f;
			if (element->Attribute("mass"))
			{
				std::string massstr = element->Attribute("mass");
				mass = toFloat(massstr);
			}

			Box *box = new Box();
			box->create(size, mass);
			shapes.insert(std::pair<std::string, Shape*>(name, box));
			script->registerVariable(UserClass("peak::Box", box), name);
			return box;
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
			float mass = 1.0f;
			if (element->Attribute("mass"))
			{
				std::string massstr = element->Attribute("mass");
				mass = toFloat(massstr);
			}


			Trimesh *trimesh = new Trimesh();
			trimesh->create(element->Attribute("mesh"), mass, scale);
			shapes.insert(std::pair<std::string, Shape*>(name, trimesh));
			return trimesh;
		}
		else if (type == "sphere")
		{
		    float radius = 1.0f;
		    if (element->Attribute("radius"))
		    {
		        radius = toFloat(element->Attribute("radius"));
		    }
            float mass = 1.0f;
			if (element->Attribute("mass"))
			{
				std::string massstr = element->Attribute("mass");
				mass = toFloat(massstr);
			}


		    Sphere *sphere = new Sphere();
		    sphere->create(radius, mass);
		    return sphere;
		}

		else
		{
			return 0;
		}
	}
	SceneNode *Entity::loadSceneNode(TiXmlElement *element)
	{
		SceneNode *node = 0;
		if (!element->Attribute("name") || !element->Attribute("type"))
		{
			LERROR("Missing 'name' or 'type' in scenenode.\n");
			return 0;
		}
		std::string name = element->Attribute("name");
		std::string type = element->Attribute("type");

		// Create shape
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
		else if (type == "camera")
		{
			//CameraSceneNode *camera = new CameraSceneNode();
			// FIXME!!!
			CameraSceneNode *camera = GraphicsEngine::get()->getStandardCamera();
			scenenodes.insert(std::pair<std::string, SceneNode*>(name, camera));
			script->registerVariable(UserClass("peak::CameraSceneNode", camera), name);
			node = camera;
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
	Link *Entity::loadLink(TiXmlElement *element)
	{
		// Get link attributes
		if (!element->Attribute("type"))
		{
			LERROR("Missing 'type' in link.\n");
			links.push_back(0);
			return 0;
		}
		std::string type = element->Attribute("type");
		std::string name = "";
		if (element->Attribute("name"))
		{
			name = element->Attribute("name");
		}
		if (!element->Attribute("source"))
		{
			LERROR("Missing 'source' in link.\n");
			links.push_back(0);
			return 0;
		}
		std::string source = element->Attribute("source");

		// Create link
		if (type == "script")
		{
			// Event->Script or Property->Script
			std::string place;
			if (element->Attribute("place"))
			{
				place = element->Attribute("place");
			}
			bool owneronly = false;
			if (element->Attribute("owneronly"))
			{
				owneronly = toBool(element->Attribute("owneronly"));
			}
			// Create script function
			std::string functionname = std::string("entity_link_script") + toString((int)links.size());
			std::string scriptdata = std::string("function entity_link_script") + toString((int)links.size()) + "()\n";
			TiXmlNode *scripttext = element->FirstChild();
			if (scripttext && scripttext->ToText())
			{
				TiXmlText *text = scripttext->ToText();
				scriptdata += text->Value();
			}
			else
			{
				LWARNING("No script found for link %s.\n", name.c_str());
			}
			scriptdata += "\nend\n";
			script->runString(scriptdata);

			// Create link
			std::string sourceobject = source.substr(0, source.find("."));
			source = source.substr(source.find(".") + 1);
			if (bodies.find(sourceobject) != bodies.end())
			{
				Body *body = bodies[sourceobject];
				Event *event = body->getEvent(source);
				if (event)
				{
					if (((place == "server") && Game::get()->isServer()) || ((place == "local") && isLocal()) || (place == ""))
					{
						EventScriptLink *link = new EventScriptLink();
						link->create(event, script, functionname, "");
						links.push_back(link);
						return link;
					}
				}
				links.push_back(0);
				return 0;
			}
			else if (getProperty(sourceobject) != 0)
			{
				if ((place == "local") && isLocal())
				{
					PropertyScriptLink *link = new PropertyScriptLink();
					link->create(getProperty(sourceobject), script, functionname);
					links.push_back(link);
					return link;
				}
				else if (place == "")
				{
					PropertyScriptLink *link = new PropertyScriptLink();
					link->create(getProperty(sourceobject), script, functionname);
					links.push_back(link);
					return link;
				}
				else if ((place == "server") && Game::get()->isServer())
				{
					PropertyScriptLink *link = new PropertyScriptLink();
					link->create(getProperty(sourceobject), script, functionname);
					links.push_back(link);
					return link;
				}
				else
				{
					// TODO: PropertyRemoteLink
					links.push_back(0);
					return 0;
				}
			}
			else
			{
				if ((sourceobject == "input"))
				{
					if (!isLocal() && !Game::get()->isServer())
					{
						links.push_back(0);
						return 0;
					}
					// Input event
					Event *event = InputManager::get()->getEvent(source);
					if (!event)
					{
						LERROR("Event does not exist: input.%s\n", source.c_str());
						links.push_back(0);
						return 0;
					}
					else
					{
						std::string filter = "";
						if (element->Attribute("filter"))
						{
							filter = element->Attribute("filter");
						}
						if ((place == "local") && isLocal())
						{
							EventScriptLink *link = new EventScriptLink();
							link->create(event, script, functionname, filter);
							links.push_back(link);
							return link;
						}
						else if ((place == "client") || ((place == "server") && Game::get()->isServer() && Game::get()->isClient()) && (!owneronly || isLocal()))
						{
							EventScriptLink *link = new EventScriptLink();
							link->create(event, script, functionname, filter);
							links.push_back(link);
							return link;
						}
						else if ((place == "server") && Game::get()->isServer())
						{
							EventScriptLink *link = new EventScriptLink();
							link->create(0, script, functionname, filter);
							links.push_back(link);
							return link;
						}
						else if ((place == "server") && Game::get()->isClient() && (!owneronly || isLocal()))
						{
							EventRemoteLink *link = new EventRemoteLink();
							link->create(event, id, links.size(), filter);
							links.push_back(link);
							return link;
						}
						else
						{
							// FIXME
							links.push_back(0);
							return 0;
						}
					}
				}
				else
				{
					LERROR("Unknown link source: %s\n", sourceobject.c_str());
					links.push_back(0);
					return 0;
				}
			}
		}
		else if (type == "direct")
		{
			// Get target
			if (!element->Attribute("target"))
			{
				LERROR("Missing 'target' in direct link.\n");
				links.push_back(0);
				return 0;
			}
			std::string target = element->Attribute("target");
			// Get properties
			Property *prop1 = getProperty(source);
			Property *prop2 = getProperty(target);
			if (!prop1)
			{
				std::string sourceobject = source.substr(0, source.find("."));
				source = source.substr(source.find(".") + 1);
				if (bodies.find(sourceobject) != bodies.end())
				{
					prop1 = bodies.find(sourceobject)->second->getProperty(source);
				}
				else if (scenenodes.find(sourceobject) != scenenodes.end())
				{
					prop1 = scenenodes.find(sourceobject)->second->getProperty(source);
				}
			}
			if (!prop2)
			{
				std::string targetobject = target.substr(0, target.find("."));
				target = target.substr(target.find(".") + 1);
				if (bodies.find(targetobject) != bodies.end())
				{
					prop2 = bodies.find(targetobject)->second->getProperty(target);
				}
				else if (scenenodes.find(targetobject) != scenenodes.end())
				{
					prop2 = scenenodes.find(targetobject)->second->getProperty(target);
				}
			}
			// Create link
			if (!prop1)
			{
				LERROR("Direct link: Source missing (%s).\n", source.c_str());
				links.push_back(0);
				return 0;
			}
			if (!prop2)
			{
				LERROR("Direct link: Target  missing (%s).\n", target.c_str());
				links.push_back(0);
				return 0;
			}
			DirectLink *link = new DirectLink();
			link->create(prop1, prop2);
			links.push_back(link);
			return link;
		}
		else
		{
			LERROR("Unknown link type (%s).\n", type.c_str());
			links.push_back(0);
			return 0;
		}
	}
	Overlay *Entity::loadOverlay(TiXmlElement *element)
	{
		if (!Game::get()->isClient()) return 0;
		// Get attributes
		std::string name = "";
		if (element->Attribute("name"))
		{
			name = element->Attribute("name");
		}
		if (!element->Attribute("owneronly"))
		{
			if (toBool(element->Attribute("owneronly")) && !isLocal()) return 0;
		}
		if (!element->Attribute("type"))
		{
			LERROR("Missing 'type' in overlay.\n");
			return 0;
		}
		std::string type = element->Attribute("type");
		
		// Load overlay
		if (type == "label")
		{
			std::string label = "";
			if (element->Attribute("label"))
			{
				label = element->Attribute("label");
			}
			std::string font = "";
			if (element->Attribute("font"))
			{
				font = element->Attribute("font");
			}
			LabelOverlay *overlay = new LabelOverlay(label, font);
			if (element->Attribute("position"))
			{
				overlay->setPosition(toVector2D(element->Attribute("position")));
			}
			if (element->Attribute("size"))
			{
				overlay->setSize(toVector2D(element->Attribute("size")));
			}
			overlays.insert(std::pair<std::string, Overlay*>(name, overlay));
			script->registerVariable(UserClass("peak::LabelOverlay", overlay), name);
			return overlay;
		}
		else
		{
			LERROR("Unknown overlay type (%s).\n", type.c_str());
			return 0;
		}
	}

	int Entity::lastid = 0;;
	std::list<Entity*> Entity::updateentities;
}

