
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

#ifndef _GAMEOBJECT_H_
#define _GAMEOBJECT_H_

#include <string>
#include <map>

//tolua_begin
namespace peak
{
	class Event;
	class Property;
	
	/**
	 * \brief Class which has properties and events
	 */
	class GameObject
	{
		public:
			GameObject();
			virtual ~GameObject();
			
			/**
			 * \brief Gets the property with the given name
			 * \param name Name of the property
			 * \return Property or 0 if no property with the given name exists
			 */
			Property *getProperty(std::string name);
			/**
			 * \brief Gets the event with the given name
			 * \param name Name of the event
			 * \return Event or 0 if no event with the given name exists
			 */
			Event *getEvent(std::string name);
			
			//tolua_end
			/**
			 * \brief This is called whenever a property changes
			 * \param name Name of the changed property
			 * \todo Pass pointer to property
			 */
			virtual void onPropertyChanged(std::string name);
			
		protected:
			/**
			 * \brief Adds a property
			 *
			 * The passed pointer mustn't be freed by the caller!
			 * \param name Name of the new property
			 * \param property Property to add
			 */
			void addProperty(std::string name, Property *property);
			/**
			 * \brief Deletes a property
			 * \param name Name of the property
			 */
			void deleteProperty(std::string name);
			
			/**
			 * \brief Adds an alias to a property
			 *
			 * Aliases are links to properties of other objects
			 * \param name Name of the new property
			 * \param property Property to link to
			 */
			void addAlias(std::string name, Property *property);
			/**
			 * \brief Deletes an alias
			 * \param name Name of the alias
			 */
			void deleteAlias(std::string name);
			
			/**
			 * \brief Adds an event
			 *
			 * The passed pointer mustn't be freed by the caller!
			 * \param name Name of the new event
			 * \param event Event to add
			 */
			void addEvent(std::string name, Event *event);
			/**
			 * \brief Deletes an event
			 * \param name Name of the event
			 */
			void deleteEvent(std::string name);
			
			std::map<std::string, Property*> properties;
			std::map<std::string, Property*> aliases;
			std::map<std::string, Event*> events;
			//tolua_begin
	};
}
//tolua_end


#endif
