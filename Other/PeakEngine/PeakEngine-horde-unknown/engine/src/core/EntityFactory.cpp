
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

#include "core/EntityFactory.h"
#include "core/Entity.h"
#include "core/FileSystem.h"
#include "core/Logger.h"

#include "support/tinyxml.h"

namespace peak
{
	EntityFactory::EntityFactory()
	{
	}
	EntityFactory::~EntityFactory()
	{
	}
	
	Entity *EntityFactory::createEntity(std::string name)
	{
		// Look for entity type
		std::string filename = FileSystem::get()->getFileExists(std::string("entities/") + name + ".xml");
		if (filename == "")
		{
			LERROR("Could not find file %s.\n", (std::string("entities/") + name + ".xml").c_str());
			return 0;
		}
		TiXmlDocument xml(filename.c_str());
		if (!xml.LoadFile() || xml.Error())
		{
			LERROR("Could not load XML file %s.xml: %s.\n", name.c_str(), xml.ErrorDesc());
			return 0;
		}
		TiXmlNode *node = xml.FirstChild("entity");
		if (!node)
		{
			LERROR("Parser error: <entity> not found.\n");
			return 0;
		}
		TiXmlElement *root = node->ToElement();
		std::string entitybase = "";
		if (root->Attribute("base"))
		{
			entitybase = root->Attribute("base");
		}
		
		// Find entity factory
		std::map<std::string, EntityFactory*>::iterator it = factories.find(entitybase);
		if (it != factories.end())
		{
			// Create entity
			return it->second->create(name);
		}
		
		// Create standard entity
		Entity *entity = new Entity;
		return entity;
	}
	
	void EntityFactory::registerFactory(EntityFactory *factory, std::string name)
	{
		factories.insert(std::pair<std::string, EntityFactory*>(name, factory));
	}

	std::map<std::string, EntityFactory*> EntityFactory::factories;
}

