/*
Copyright (C) 2009 Mathias Gottschlag

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

#include <string>
#include <list>
#include <vector>

#include "core/Buffer.h"

namespace peak
{
	class Buffer;
	class Model;
	
	enum VariableType
	{
		EVT_Int32,
		EVT_Int16,
		EVT_Int8,
		EVT_Float,
		EVT_Double,
		EVT_Object3D
	};
	
	struct EntityVariable
	{
		EntityVariable(VariableType type, void *pointer)
		{
			this->type = type;
			ptr = pointer;
		}
		void write(Buffer *buffer);
		void read(Buffer *buffer);
		
		VariableType type;
		void *ptr;
	};
	
	struct EntityState
	{
		void create(std::vector<EntityVariable> &typeinfo, int time);
		int timestamp;
		Buffer data;
	};
	
	struct EntityCommand
	{
		unsigned int timestamp;
		unsigned short id;
		Buffer data;
	};
	
	class Entity
	{
		public:
			Entity();
			~Entity();
			
			virtual bool init(std::string type);
			virtual bool destroy(void);
	
			bool saveState(EntityState *state);
			bool restoreState(EntityState *state);
			bool sendDeltaUpdate(Buffer *buffer, EntityState *from, EntityState *to);
			bool sendFullUpdate(Buffer *buffer);
			bool sendDeltaUpdate(Buffer *buffer, int time1, int time2);
			bool applyDeltaUpdate(Buffer *buffer, EntityState *from, EntityState *to);
			//bool applyDeltaUpdate(Buffer *buffer, EntityState *state);
			EntityState *getState(int time);
			bool checkState(EntityState *state, Buffer *updatebuffer);
			
			void discardStates(bool future, bool past);
			
			bool save(void);
			bool reset(int time, bool deletefuture = false);
			bool rewind(void);
			bool update(Buffer *buffer);
			bool injectUpdate(int time, Buffer *buffer, bool apply = false);
			//void void resetStates(void);
			
			void sendCommand(EntityCommand *command);
			virtual void applyCommand(EntityCommand *command);
			void applyNextCommand(void);
			void applyCommand(int time);
			
			void setID(int id);
			int getID(void);
			
			virtual void update(float msecs);
			
			std::string getType(void);
			
			unsigned short getLastCommandID(void)
			{
				return lastcommandid;
			}
			unsigned int getLastCommandTime(void)
			{
				return lastcommandtime;
			}
			EntityCommand *getCommand(int id)
			{
				std::list<EntityCommand*>::iterator it = lastcommands.begin();
				while (it != lastcommands.end())
				{
					if ((*it)->id == id) return *it;
					it++;
				}
				return 0;
			}
			
			void finalize(void);
		protected:
			void add(Model *model, unsigned int flags = 0);
			void add(int *variable, unsigned int flags = 0);
			void add(float *variable, unsigned int flags = 0);
			void add(unsigned char *variable, unsigned int flags = 0);
		private:
			std::list<EntityState*> past;
			std::list<EntityState*> future;
			
			std::vector<EntityVariable> variables;
			
			std::list<EntityCommand*> lastcommands;
			unsigned int lastcommandtime;
			unsigned short lastcommandid;
			
			int id;
			static unsigned int maxstates;
			std::string type;
	};
}

#endif

