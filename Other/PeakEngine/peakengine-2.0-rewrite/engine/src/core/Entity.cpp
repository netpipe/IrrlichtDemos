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

#include "core/Entity.h"
#include "core/Buffer.h"
#include "core/Game.h"

#include <math.h>

namespace peak
{
	void EntityState::create(std::vector<EntityVariable> &typeinfo, int time)
	{
		timestamp = time;
		data.clear();
		for (unsigned int i = 0; i < typeinfo.size(); i++)
		{
			typeinfo[i].write(&data);
		}
	}
	
	void EntityVariable::write(Buffer *buffer)
	{
		switch (type)
		{
			case EVT_Int32:
				buffer->writeInt(*(int*)ptr);
				break;
			case EVT_Int16:
				buffer->writeWord(*(short*)ptr);
				break;
			case EVT_Int8:
				buffer->writeByte(*(char*)ptr);
				break;
			case EVT_Float:
				buffer->writeFloat(*(float*)ptr);
				break;
			case EVT_Double:
				buffer->writeFloat(*(double*)ptr);
				break;
			default:
				break;
		}
	}
	void EntityVariable::read(Buffer *buffer)
	{
		switch (type)
		{
			case EVT_Int32:
				*(int*)ptr = buffer->readInt();
				break;
			case EVT_Int16:
				*(short*)ptr = buffer->readWord();
				break;
			case EVT_Int8:
				*(char*)ptr = buffer->readByte();
				break;
			case EVT_Float:
				*(float*)ptr = buffer->readFloat();
				break;
			case EVT_Double:
				*(double*)ptr = buffer->readFloat();
				break;
			default:
				break;
		}
	}
	
	Entity::Entity()
	{
		lastcommandtime = 0;
		lastcommandid = 0;
	}
	Entity::~Entity()
	{
	}
	
	bool Entity::init(std::string type)
	{
		this->type = type;
		return true;
	}
	bool Entity::destroy(void)
	{
		return false;
	}

	bool Entity::saveState(EntityState *state)
	{
		state->create(variables, Game::get()->getTime());
		return true;
	}
	bool Entity::restoreState(EntityState *state)
	{
		state->data.setPosition(0);
		for (unsigned int i = 0; i < variables.size(); i++)
		{
			variables[i].read(&state->data);
		}
		return false;
	}
	bool Entity::sendDeltaUpdate(Buffer *buffer, EntityState *from, EntityState *to)
	{
		from->data.setPosition(0);
		to->data.setPosition(0);
		// Write empty mask first (we fill it in later)
		unsigned int maskpos = buffer->getDataSize();
		int masksize = (variables.size() + 7) / 8;
		for (int i = 0; i < masksize; i++)
		{
			buffer->writeByte(0);
		}
		// Write variables
		for (unsigned int i = 0; i < variables.size(); i++)
		{
			bool changed = false;
			// Check whether variable changed and write value into buffer
			switch (variables[i].type)
			{
				case EVT_Int32:
					{
						int val1 = from->data.readInt();
						int val2 = to->data.readInt();
						if (val1 != val2)
						{
							changed = true;
							buffer->writeInt(val2);
						}
						break;
					}
				case EVT_Int16:
					{
						short val1 = from->data.readWord();
						short val2 = to->data.readWord();
						if (val1 != val2)
						{
							changed = true;
							buffer->writeWord(val2);
						}
						break;
					}
				case EVT_Int8:
					{
						char val1 = from->data.readByte();
						char val2 = to->data.readByte();
						if (val1 != val2)
						{
							changed = true;
							buffer->writeByte(val2);
						}
						break;
					}
				case EVT_Float:
				case EVT_Double:
					{
						float val1 = from->data.readFloat();
						float val2 = to->data.readFloat();
						if (fabs(val1 - val2) > 0.001)
						{
							changed = true;
							buffer->writeFloat(val2);
						}
						break;
					}
				default:
					break;
			}
			if (changed)
			{
				// Set needed bit in the mask
				unsigned char *data = (unsigned char*)buffer->getData();
				data += maskpos + (i / 8);
				*data |= 1 << (i % 8);
			}
		}
		unsigned char *data = (unsigned char*)buffer->getData();
		data += maskpos;
		return true;
	}
	bool Entity::sendFullUpdate(Buffer *buffer)
	{
		return true;
	}
	bool Entity::sendDeltaUpdate(Buffer *buffer, int time1, int time2)
	{
		return false;
	}
	bool Entity::applyDeltaUpdate(Buffer *buffer, EntityState *from, EntityState *to)
	{
		to->data.setPosition(0);
		// Get mask which contains the changed variables
		int masksize = (variables.size() + 7) / 8;
		unsigned char *mask = (unsigned char*)buffer->getData();
		mask += buffer->getPosition();
		buffer->setPosition(masksize, true);
		// Loop through the variables and set them if they got changed
		for (unsigned int i = 0; i < variables.size(); i++)
		{
			bool changed = (mask[i / 8] & (1 << (i % 8))) != 0;
			if (changed)
			{
				char *dest = to->data.getData() + to->data.getPosition();
				switch (variables[i].type)
				{
					case EVT_Int8:
						to->data.setPosition(1, true);
						*dest = buffer->readByte();
						break;
					case EVT_Int16:
						to->data.setPosition(2, true);
						*(short*)dest = buffer->readWord();
						break;
					case EVT_Int32:
					case EVT_Float:
					case EVT_Double:
						to->data.setPosition(4, true);
						*(int*)dest = buffer->readInt();
						break;
					default:
						break;
				}
			}
			else
			{
				// Skip the variable in the state buffer
				switch (variables[i].type)
				{
					case EVT_Int8:
						to->data.setPosition(1, true);
						break;
					case EVT_Int16:
						to->data.setPosition(2, true);
						break;
					case EVT_Int32:
					case EVT_Float:
					case EVT_Double:
						to->data.setPosition(4, true);
						break;
					default:
						break;
				}
			}
		}
		return true;
	}
	EntityState *Entity::getState(int time)
	{
		EntityState *state = 0;
		std::list<EntityState*>::iterator it;
		// Search past states
		it = past.begin();
		while (it != past.end())
		{
			if ((*it)->timestamp == time)
			{
				state = *it;
				break;
			}
			it++;
		}
		if (!state)
		{
			// Search future state
			it = future.begin();
			while (it != future.end())
			{
				if ((*it)->timestamp == time)
				{
					state = *it;
					break;
				}
				it++;
			}
		}
		return state;
	}
	bool Entity::checkState(EntityState *state, Buffer *updatebuffer)
	{
		int bufferposition = updatebuffer->getPosition();
		state->data.setPosition(0);
		// Get mask which contains the changed variables
		int masksize = (variables.size() + 7) / 8;
		unsigned char *mask = (unsigned char*)updatebuffer->getData();
		mask += updatebuffer->getPosition();
		updatebuffer->setPosition(masksize, true);
		// Loop through the variables and set them if they got changed
		for (unsigned int i = 0; i < variables.size(); i++)
		{
			bool changed = (mask[i / 8] & (1 << (i % 8))) != 0;
			if (changed)
			{
				//char *dest = state->data.getData() + state->data.getPosition();
				switch (variables[i].type)
				{
					case EVT_Int8:
						if (updatebuffer->readByte() != state->data.readByte())
						{
							printf("Byte differs.\n");
							updatebuffer->setPosition(bufferposition);
							return false;
						}
						break;
					case EVT_Int16:
						if (updatebuffer->readWord() != state->data.readWord())
						{
							printf("Word differs.\n");
							updatebuffer->setPosition(bufferposition);
							return false;
						}
						break;
					case EVT_Int32:
					case EVT_Float:
					case EVT_Double:
						{
							int a = updatebuffer->readInt();
							int b = state->data.readInt();
							if (a != b)
							{
								printf("Int differs (%d vs %d).\n", a, b);
								updatebuffer->setPosition(bufferposition);
								return false;
							}
							else
								printf("Int ok (%d vs %d).\n", a, b);
						}
						break;
					default:
						break;
				}
			}
			else
			{
				// Skip the variable in the state buffer
				switch (variables[i].type)
				{
					case EVT_Int8:
						state->data.setPosition(1, true);
						break;
					case EVT_Int16:
						state->data.setPosition(2, true);
						break;
					case EVT_Int32:
					case EVT_Float:
					case EVT_Double:
						state->data.setPosition(4, true);
						break;
					default:
						break;
				}
			}
		}
		updatebuffer->setPosition(bufferposition);
		return true;
	}

	void Entity::discardStates(bool future, bool past)
	{
		std::list<EntityState*>::iterator it;
		// Delete states in the future
		if (future)
		{
			it = this->future.begin();
			while (it != this->future.end())
			{
				delete *it;
				it++;
			}
			this->future.clear();
		}
		// Delete states from the past
		if (past)
		{
			it = this->past.begin();
			while (it != this->past.end())
			{
				delete *it;
				it++;
			}
			this->past.clear();
		}
	}
	
	bool Entity::save(void)
	{
		printf("States: %d\n", (int)(past.size() + future.size()));
		if (past.size() + future.size() >= maxstates)
		{
			// Delete oldest state
			if (past.size() > 0)
			{
				printf("Deleting %d.\n", (*past.rbegin())->timestamp);
				delete *past.rbegin();
				std::list<EntityState*>::iterator it = past.end();
				it--;
				past.erase(it);
			}
			else
			{
				printf("Deleting %d.\n", (*future.begin())->timestamp);
				delete *future.begin();
				future.erase(past.begin());
			}
		}
		// Save current state
		EntityState *newstate = new EntityState;
		saveState(newstate);
		printf("Inserting %d\n", newstate->timestamp);
		past.push_front(newstate);
		return true;
	}
	bool Entity::reset(int time, bool deletefuture)
	{
		EntityState *newstate = 0;
		std::list<EntityState*>::iterator it;
		// Get state to use
		it = past.begin();
		while (it != past.end())
		{
			if ((*it)->timestamp == time)
			{
				newstate = *it;
				break;
			}
			it++;
		}
		if (!newstate)
		{
			it = future.begin();
			while (it != future.end())
			{
				if ((*it)->timestamp == time)
				{
					newstate = *it;
					break;
				}
				it++;
			}
		}
		if (!newstate)
			return false;
		// Switch to selected state
		restoreState(newstate);
		// Rearrange states so that the lists are valid again
		it = past.begin();
		while (it != past.end())
		{
			if ((*it)->timestamp > time)
			{
				EntityState *state = *it;
				it = past.erase(it);
				if (!deletefuture)
					future.push_back(state);
				else
					delete state;
				continue;
			}
			it++;
		}
		it = future.begin();
		while (it != future.end())
		{
			if ((*it)->timestamp <= time)
			{
				EntityState *state = *it;
				it = future.erase(it);
				past.push_front(state);
				continue;
			}
			it++;
		}
		// Delete rest of future states
		if (deletefuture)
		{
			it = future.begin();
			while (it != future.end())
			{
				delete *it;
				it++;
			}
			future.clear();
		}
		return true;
	}
	bool Entity::rewind(void)
	{
		return false;
	}
	bool Entity::update(Buffer *buffer)
	{
		int currenttime = Game::get()->getTime();
		EntityState *state1 = 0;
		EntityState *state2 = 0;
		if (past.size() == 0) return false;
		// Get two states to compare
		std::list<EntityState*>::iterator it = past.begin();
		while (it != past.end())
		{
			if ((*it)->timestamp == currenttime)
			{
				state2 = *it;
				it++;
				if (it == past.end())
					break;
				state1 = *it;
				break;
			}
			it++;
		}
		if (!state1 || !state2) return false;
		// Write delta update into buffer
		sendDeltaUpdate(buffer, state1, state2);
		return true;
	}
	bool Entity::injectUpdate(int time, Buffer *buffer, bool apply)
	{
		EntityState *state1 = 0;
		EntityState *state2 = 0;
		if (past.size() == 0) return false;
		// Get two states to compare
		std::list<EntityState*>::iterator it = past.begin();
		while (it != past.end())
		{
			if ((*it)->timestamp == time)
			{
				state2 = *it;
				it++;
				if (it == past.end())
					break;
				state1 = *it;
				break;
			}
			it++;
		}
		if (!state1 || !state2) return false;
		// Apply update
		applyDeltaUpdate(buffer, state1, state2);
		if (apply)
		{
			restoreState(state2);
		}
		return true;
	}
	
	void Entity::sendCommand(EntityCommand *command)
	{
		// FIXME: Causes problems if both server and client send commands
		if (!Game::get()->isServer())
			command->id = ++lastcommandid;
		else if (command->id)
			lastcommandid = command->id;
		//command->timestamp = Game::get()->getTime();
		lastcommands.push_back(command);
		lastcommandtime = command->timestamp;
		if (!Game::get()->isServer())
		{
			// Send command to server
			Buffer msg;
			msg.writeByte(1);
			msg.writeWord(getID());
			msg.writeWord(command->id);
			msg += command->data;
			Game::get()->sendServerData(&msg, true);
			applyCommand(command);
		}
	}
	void Entity::applyCommand(EntityCommand *command)
	{
	}
	void Entity::applyNextCommand(void)
	{
		if (Game::get()->isServer())
		{
			while (lastcommands.size() != 0)
			{
				EntityCommand *cmd = *lastcommands.begin();
				if (cmd->timestamp <= Game::get()->getTime())
				{
					lastcommandtime = cmd->timestamp;
					applyCommand(cmd);
					lastcommands.erase(lastcommands.begin());
				}
				else break;
			}
		}
		else
		{
		}
	}
	void Entity::applyCommand(int time)
	{
		std::list<EntityCommand*>::iterator it = lastcommands.begin();
		while (it != lastcommands.end())
		{
			EntityCommand *cmd = *it;
			if (cmd->timestamp == time)
			{
				applyCommand(cmd);
			}
			it++;
		}
	}

	void Entity::setID(int id)
	{
		this->id = id;
	}
	int Entity::getID(void)
	{
		return id;
	}
	
	void Entity::update(float msecs)
	{
	}
	
	std::string Entity::getType(void)
	{
		return type;
	}
	
	void Entity::add(Model *model, unsigned int flags)
	{
		variables.push_back(EntityVariable(EVT_Object3D, model));
	}
	void Entity::add(int *variable, unsigned int flags)
	{
		variables.push_back(EntityVariable(EVT_Int32, variable));
	}
	void Entity::add(float *variable, unsigned int flags)
	{
		variables.push_back(EntityVariable(EVT_Float, variable));
	}
	void Entity::add(unsigned char *variable, unsigned int flags)
	{
		variables.push_back(EntityVariable(EVT_Int8, variable));
	}
	void Entity::finalize(void)
	{
	}
	
	unsigned int Entity::maxstates = 20;
}

