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

#ifndef _COMPONENT_H_
#define _COMPONENT_H_

#include "core/Buffer.h"

//tolua_begin
namespace peak
{
	enum ComponentType
	{
		ECT_ServerScript = 1,
		ECT_Physics = 2,
		ECT_Graphics = 3,
		ECT_ClientScript = 4
	};
	
	class Entity;
	
	/**
	 * \brief Base class for all components
	 */
	class Component
	{
		public:
			virtual ~Component() {};
			
			/**
			 * \brief Initializes the component
			 */
			virtual bool init(Entity *entity) = 0;
			/**
			 * \brief Destroys the component
			 */
			virtual bool destroy(void) = 0;
			
			/**
			 * \brief Returns whether the component needs to be updated
			 */
			virtual bool needsUpdate(void) = 0;
			/**
			 * \brief Returns the size of the update
			 */
			virtual int getUpdateSize(void) = 0;
			/**
			 * \brief Writes the update data into a buffer
			 */
			virtual int writeUpdateData(Buffer *buffer) = 0;
			/**
			 * \brief Updates the component with data read from a buffer
			 */
			virtual bool updateFromData(Buffer *buffer) = 0;
			
			/**
			 * \brief Writes the complete component into a buffer
			 */
			virtual bool writeCompleteData(Buffer *buffer) = 0;
			/**
			 * \brief Reads the complete component data from a buffer
			 */
			virtual bool readCompleteData(Buffer *buffer) = 0;
			
			/**
			 * \brief Returns the type of the component
			 */
			virtual ComponentType getType(void) = 0;
			
			/**
			 * \brief Updates the component
			 */
			virtual bool doWork(float msecs) = 0;
		private:
	};
}
//tolua_end

#endif

