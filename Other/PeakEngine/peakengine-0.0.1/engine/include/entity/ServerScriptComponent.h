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

#ifndef _SERVERSCRIPTCOMPONENT_H_
#define _SERVERSCRIPTCOMPONENT_H_

#include "entity/Component.h"

//tolua_begin
namespace peak
{
	class Script;
	
	/**
	 * \brief Component holding the server script of the entity
	 */
	class ServerScriptComponent : public Component
	{
		public:
			ServerScriptComponent();
			virtual ~ServerScriptComponent();
			
			virtual bool init(Entity *entity);
			virtual bool destroy(void);
			
			virtual bool needsUpdate(void);
			virtual int getUpdateSize(void);
			virtual int writeUpdateData(Buffer *buffer);
			
			virtual bool updateFromData(Buffer *buffer);
			
			virtual bool writeCompleteData(Buffer *buffer);
			virtual bool readCompleteData(Buffer *buffer);
			
			/**
			 * \brief Loads a file and adds it to the script
			 * \param file File to be loaded
			 */
			bool runScript(std::string file);
			
			virtual ComponentType getType(void);
			
			virtual bool doWork(float msecs);
			
			/**
			 * \brief Returns the script
			 */
			Script *getScript(void);
		private:
			//tolua_end
			Script *script;
			//tolua_begin
	};
}
//tolua_end

#endif

