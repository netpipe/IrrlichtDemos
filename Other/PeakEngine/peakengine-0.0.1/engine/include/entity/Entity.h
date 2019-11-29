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

#ifndef _ENTITY_H_
#define _ENTITY_H_

#include <deque>
#include <string>

#include "entity/Component.h"

//tolua_begin
namespace peak
{
	class Buffer;
	class PhysicsComponent;
	class GraphicsComponent;
	class ServerScriptComponent;
	class ClientScriptComponent;
	class EntityVariables;
	
	/**
	 * \brief Class loading and managing a single entity
	 *
	 * Every entity consists of several components and a set of variables.
	 */
	class Entity
	{
		public:
			Entity();
			~Entity();
			
			/**
			 * \brief Loads the entity
			 * \param name Name of the entity
			 * \param server If set to true, server data is created
			 * \param graphics If set to true, client data is created
			 */
			bool load(std::string name, bool server, bool graphics);
			/**
			 * \brief Destroys the entity
			 */
			bool destroy(void);
			
			/**
			 * \brief Returns the name of the entity
			 */
			std::string getName(void);
			
			/**
			 * \brief Sets the ID of the entity
			 *
			 * Do not use this function unless you know what you do!
			 */
			void setID(int id);
			/**
			 * \brief Returns the ID of the entity
			 */
			int getID(void);
			
			/**
			 * \brief Sets whether the entity is local
			 *
			 * Only the owner of the entity can change it
			 */
			void setLocal(bool local);
			/**
			 * \brief Returns whether the entity is local
			 */
			bool isLocal(void);
			/**
			 * \brief Sets the owner of the entity
			 * \param owner Connection ID of the owner
			 */
			void setOwner(int owner);
			/**
			 * \brief Returns the owner of the entity
			 *
			 * Only to be used on the server.
			 */
			int getOwner(void);
			
			/**
			 * \brief Sets the priority of the entity (0 - 10)
			 *
			 * Entities with a high priority get updated more often
			 */
			void setPriority(int priority);
			/**
			 * \brief Returns the priority of the entity
			 */
			int getPriority(void);
			/**
			 * \brief Sets the current priority of the entity
			 *
			 * This value is regularly set by the priority queue of Game.
			 */
			void setCurrentPriority(int priority);
			/**
			 * \brief Returns the current priority of the entity
			 */
			int getCurrentPriority(void);
			
			/**
			 * \brief Returns whether the entity needs an update
			 */
			bool needsUpdate(void);
			/**
			 * \brief Returns the size of the needed update
			 */
			int getUpdateSize(void);
			/**
			 * \brief Writes the update data into a buffer
			 */
			int writeUpdateData(Buffer *buffer);
			/**
			 * \brief Updates the entity with data read from a buffer
			 */
			bool updateFromData(Buffer *buffer);
			
			/**
			 * \brief Writes the complete entity data into a buffer
			 */
			bool writeCompleteData(Buffer *buffer);
			/**
			 * \brief Reads the complete entity data from a buffer
			 */
			bool readCompleteData(Buffer *buffer);
			
			/**
			 * \brief Adds a component to the entity
			 */
			void addComponent(Component *component);
			/**
			 * \brief Deletes a component by type
			 */
			void deleteComponent(ComponentType type);
			/**
			 * \brief Returns the component with the given type
			 */
			Component *getComponent(ComponentType type);
			
			/**
			 * \brief Returns the physics component
			 */
			PhysicsComponent *getPhysicsComponent(void);
			/**
			 * \brief Returns the graphics component
			 */
			GraphicsComponent *getGraphicsComponent(void);
			/**
			 * \brief Returns the server script component
			 */
			ServerScriptComponent *getServerScriptComponent(void);
			
			/**
			 * \brief Returns the variables of the object
			 *
			 * EntityVariables can be used to transmit reliable data.
			 */
			EntityVariables *getVariables(void);
			
			/**
			 * \brief Updates the entity
			 */
			bool doWork(float msecs);
		private:
			//tolua_end
			std::string name;
			
			static int lastid;
			int id;
			
			int owner;
			bool local;
			
			std::deque<Component*> components;
			
			int priority;
			int currentpriority;
			
			EntityVariables *variables;
			//tolua_begin
	};
}
//tolua_end

#endif

