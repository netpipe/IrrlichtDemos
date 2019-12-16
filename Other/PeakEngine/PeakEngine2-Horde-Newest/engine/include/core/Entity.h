
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

#ifndef _ENTITY_H_
#define _ENTITY_H_

#include "core/GameObject.h"
#include "core/Vector3D.h"

#include <map>
#include <list>
#include <string>
#include <vector>

class TiXmlElement;

//tolua_begin
namespace peak
{
	class Shape;
	class Body;
	class SceneNode;
	class Buffer;
	class Script;
	class Link;
	class Buffer;
	class Overlay;
	
	/**
	 * \brief Class for all dynamic objects in the game
	 *
	 * An entity is defined through a XML file (entities/name.xml).
	 *
	 * Entities are created via Game::createEntity().
	 */
	class Entity : public GameObject
	{
		public:
			Entity();
			virtual ~Entity();
			
			/**
			 * \brief Loads the entity from a file
			 * \param name Name of the entity
			 * \param data Server data for the entity
			 */
			virtual bool load(std::string name, Buffer *data = 0);
			
			/**
			 * \brief Sets the owner of the entity
			 *
			 * The owner of the entity can have special rights of the entity, for
			 * example the owner is usually the only client which can control a
			 * character.
			 * \param owner New owner
			 */
			void setOwner(int owner);
			/**
			 * \brief Returns the owner of the entity
			 */
			int getOwner(void);
			
			/**
			 * \brief Sets whether the entity is local (=this computer is the owner)
			 * \param local If set to true, the entity is local
			 */
			void setLocal(bool local);
			/**
			 * \brief Returns whether this computer is the owner of the entity
			 */
			bool isLocal(void);
			
			/**
			 * \brief Sets the ID of the entity
			 * \param id New ID
			 */
			void setID(int id);
			/**
			 * \brief Returns the ID of the entity
			 */
			int getID(void);
			
			/**
			 * \brief Saves the data of the entity to a buffer so that clients can
			 * make a local copy of the entity from the buffer
			 * \brief data Buffer to save data to
			 * \return Returns false if data could not be written
			 */
			bool writeStartData(Buffer *data);
			
			/**
			 * \brief Sets the position of the entity if possible
			 *
			 * This function looks for a property named "position" and sets it
			 * to the new value.
			 * \param position New position
			 */
			virtual void setPosition(Vector3D position);
			
			/**
			 * \brief Returns the name of the entity
			 */
			std::string getName(void);
			
			/**
			 * \brief Returns the body with the given name
			 * \param name Name of the body
			 * \return Body or 0, if body doesn't exist
			 */
			Body *getBody(std::string name);
			/**
			 * \brief Returns the scene node with the given name
			 * \param name Name of the scene node
			 * \return Body or 0, if scene node doesn't exist
			 */
			SceneNode *getSceneNode(std::string name);
			/**
			 * \brief Returns the script of the entity
			 */
			Script *getScript(void);
			//tolua_end
			/**
			 * \brief Updates the entity
			 * \param msecs Time since last update
			 */
			virtual void update(float msecs);
			/**
			 * \brief Updates all entities
			 * \param msecs Time since last update
			 */
			static void updateAll(float msecs);
			/**
			 * \brief Updates network link
			 * \param buffer Link data
			 */
			static void updateLink(Buffer *buffer);
			virtual void onPropertyChanged(std::string name);
			/**
			 * \brief Updates property from network data
			 * \param buffer Property data
			 */
			static void updateProperty(Buffer *buffer);
		private:
			
			bool loadBody(TiXmlElement *element);
			Shape *loadShape(TiXmlElement *element);
			SceneNode *loadSceneNode(TiXmlElement *element, bool bodyparent = false);
			Link *loadLink(TiXmlElement *element);
			Overlay *loadOverlay(TiXmlElement *element);
			
			std::map<std::string, Shape*> shapes;
			std::map<std::string, Body*> bodies;
			std::map<std::string, SceneNode*> scenenodes;
			std::map<std::string, Overlay*> overlays;
			std::vector<Link*> links;
			
			std::string name;
			int id;
			static int lastid;
			int owner;
			bool local;
			
			Script *script;
			
			static std::list<Entity*> updateentities;
			//tolua_begin
	};
}
//tolua_end

#endif
