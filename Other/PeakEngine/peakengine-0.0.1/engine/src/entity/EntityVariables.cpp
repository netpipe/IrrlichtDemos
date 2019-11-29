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

#include "entity/EntityVariables.h"
#include "core/IniFile.h"
#include "core/Util.h"
#include "core/Logger.h"
#include "core/Buffer.h"

namespace peak
{
	EntityVariables::EntityVariables()
	{
	}
	EntityVariables::~EntityVariables()
	{
	}
	
	bool EntityVariables::load(IniFile *file)
	{
		if (file->getSectionIndex("variables") == -1)
		{
			return true;
		}
		
		// Read variables from file
		int varcount = file->getItemValueI("variables", "variables");
		for (int i = 0; i < varcount; i++)
		{
			std::string sectionname = std::string("variable") + toString(i);
			if (file->getSectionIndex(sectionname) != -1)
			{
				int id = file->getItemValueI(sectionname, "id");
				std::string name = file->getItemValue(sectionname, "name");
				std::string type = file->getItemValue(sectionname, "type");
				std::string value = file->getItemValue(sectionname, "value");
				
				// Create variable
				Variable newvar;
				newvar.name = name;
				// Get type
				if (type == "integer")
				{
					newvar.type = EVT_Integer;
					newvar.intvalue = toInt(value);
					newvar.oldintvalue = toInt(value);
				}
				else if (type == "word")
				{
					newvar.type = EVT_Word;
					newvar.intvalue = toInt(value);
					newvar.oldintvalue = toInt(value);
				}
				else if (type == "byte")
				{
					newvar.type = EVT_Byte;
					newvar.intvalue = toInt(value);
					newvar.oldintvalue = toInt(value);
				}
				else if (type == "float")
				{
					newvar.floatvalue[0] = toFloat(value);
					newvar.oldfloatvalue[0] = toFloat(value);
				}
				else if (type == "vector2d")
				{
					newvar.type = EVT_Vector2D;
					Vector2D vector = toVector2D(value);
					newvar.floatvalue[0] = vector.x;
					newvar.floatvalue[1] = vector.y;
					newvar.oldfloatvalue[0] = vector.x;
					newvar.oldfloatvalue[1] = vector.y;
				}
				else if (type == "vector3d")
				{
					newvar.type = EVT_Vector3D;
					Vector3D vector = toVector3D(value);
					newvar.floatvalue[0] = vector.x;
					newvar.floatvalue[1] = vector.y;
					newvar.floatvalue[2] = vector.z;
					newvar.oldfloatvalue[0] = vector.x;
					newvar.oldfloatvalue[1] = vector.y;
					newvar.oldfloatvalue[2] = vector.z;
				}
				else if (type == "quaternion")
				{
					newvar.type = EVT_Quaternion;
					newvar.floatvalue[0] = 0;
					newvar.floatvalue[1] = 0;
					newvar.floatvalue[2] = 0;
					newvar.floatvalue[3] = 0;
					newvar.oldfloatvalue[0] = 0;
					newvar.oldfloatvalue[1] = 0;
					newvar.oldfloatvalue[2] = 0;
					newvar.oldfloatvalue[3] = 0;
				}
				else if (type == "string")
				{
					newvar.type = EVT_String;
					newvar.svalue = value;
					newvar.oldsvalue = value;
				}
				else
				{
					LERROR("Invalid variable type: %s.\n", type.c_str());
					continue;
				}
				
				// Create pointer
				Variable *var = new Variable;
				*var = newvar;
				variables.insert(std::pair<int, Variable*>(id, var));
			}
			else
			{
				LERROR("Could not find variable %s.\n", sectionname.c_str());
			}
		}
		
		return true;
	}
	bool EntityVariables::destroy(void)
	{
		std::map<int, Variable*>::iterator it = variables.begin();
		while (it != variables.end())
		{
			delete it->second;
			it++;
		}
		variables.clear();
		return true;
	}
	
	void EntityVariables::setInteger(std::string name, int value)
	{
		Variable *var = getVariable(name);
		if (var)
		{
			var->intvalue = value;
			var->updated = true;
		}
	}
	void EntityVariables::setFloat(std::string name, float value)
	{
		Variable *var = getVariable(name);
		if (var)
		{
			var->floatvalue[0] = value;
			var->updated = true;
		}
	}
	void EntityVariables::set(std::string name, Vector3D value)
	{
		Variable *var = getVariable(name);
		if (var)
		{
			var->floatvalue[0] = value.x;
			var->floatvalue[1] = value.y;
			var->floatvalue[2] = value.z;
			var->updated = true;
		}
	}
	void EntityVariables::set(std::string name, Vector2D value)
	{
		Variable *var = getVariable(name);
		if (var)
		{
			var->floatvalue[0] = value.x;
			var->floatvalue[1] = value.y;
			var->updated = true;
		}
	}
	void EntityVariables::set(std::string name, Quaternion value)
	{
		Variable *var = getVariable(name);
		if (var)
		{
			var->floatvalue[0] = value.q[0];
			var->floatvalue[1] = value.q[1];
			var->floatvalue[1] = value.q[2];
			var->floatvalue[1] = value.q[3];
			var->updated = true;
		}
	}
	void EntityVariables::set(std::string name, std::string value)
	{
		Variable *var = getVariable(name);
		if (var)
		{
			var->svalue = value;
			var->updated = true;
		}
	}
	
	int EntityVariables::getInteger(std::string name)
	{
		Variable *var = getVariable(name);
		if (var)
		{
			return var->intvalue;
		}
		return 0;
	}
	float EntityVariables::getFloat(std::string name)
	{
		Variable *var = getVariable(name);
		if (var)
		{
			return var->floatvalue[0];
		}
		return 0;
	}
	Vector3D EntityVariables::getVector3D(std::string name)
	{
		Variable *var = getVariable(name);
		if (var)
		{
			return Vector3D(var->floatvalue[0], var->floatvalue[1], var->floatvalue[2]);
		}
		return Vector3D(0, 0, 0);
	}
	Vector2D EntityVariables::getVector2D(std::string name)
	{
		Variable *var = getVariable(name);
		if (var)
		{
			return Vector2D(var->floatvalue[0], var->floatvalue[1]);
		}
		return Vector2D(0, 0);
	}
	Quaternion EntityVariables::getQuaternion(std::string name)
	{
		Variable *var = getVariable(name);
		if (var)
		{
			return Quaternion(var->floatvalue[0], var->floatvalue[1], var->floatvalue[2], var->floatvalue[3]);
		}
		return Quaternion(0, 0, 0, 0);
	}
	std::string EntityVariables::getString(std::string name)
	{
		Variable *var = getVariable(name);
		if (var)
		{
			return var->svalue;
		}
		return "";
	}
	
	bool EntityVariables::needsUpdate(void)
	{
		std::map<int, Variable*>::iterator it = variables.begin();
		while (it != variables.end())
		{
			if (hasChanged(it->second)) return true;
			it++;
		}
		return false;
	}
	int EntityVariables::getUpdateSize(void)
	{
		// TODO
		return 0;
	}
	int EntityVariables::writeUpdateData(Buffer *buffer)
	{
		std::map<int, Variable*>::iterator it = variables.begin();
		while (it != variables.end())
		{
			if (hasChanged(it->second))
			{
				buffer->writeByte(it->first);
				writeVariable(it->second, buffer);
			}
			it++;
		}
		buffer->writeByte(-1);
		return 0;
	}
	bool EntityVariables::updateFromData(Buffer *buffer)
	{
		char id = buffer->readByte();
		while (id != -1)
		{
			Variable *var = getVariable(id);
			if (var)
			{
				readVariable(var, buffer);
			}
			else
			{
				LERROR("Variable not found (%d).\n", id);
				return false;
			}
			id = buffer->readByte();
		}
		return true;
	}
	
	bool EntityVariables::writeCompleteData(Buffer *buffer)
	{
		std::map<int, Variable*>::iterator it = variables.begin();
		while (it != variables.end())
		{
			buffer->writeByte(it->first);
			writeVariable(it->second, buffer);
			it++;
		}
		buffer->writeByte(-1);
		return 0;
	}
	bool EntityVariables::readCompleteData(Buffer *buffer)
	{
		char id = buffer->readByte();
		while (id != -1)
		{
			Variable *var = getVariable(id);
			if (var)
			{
				readVariable(var, buffer);
			}
			else
			{
				LERROR("Variable not found (%d).\n", id);
				return false;
			}
			id = buffer->readByte();
		}
		return true;
	}
	
	bool EntityVariables::wasUpdated(std::string name)
	{
		Variable *var = getVariable(name);
		if (!var) return false;
		
		if (var->updated == true)
		{
			var->updated = false;
			return true;
		}
		return false;
	}
	
	Variable *EntityVariables::getVariable(std::string name)
	{
		std::map<int, Variable*>::iterator it = variables.begin();
		while (it != variables.end())
		{
			if (it->second->name == name)
			{
				return it->second;
			}
			it++;
		}
		return 0;
	}
	Variable *EntityVariables::getVariable(int id)
	{
		return variables.find(id)->second;
	}
	
	bool EntityVariables::hasChanged(Variable *var)
	{
		// Search for different values
		switch (var->type)
		{
			case EVT_Integer:
			case EVT_Word:
			case EVT_Byte:
				if (var->intvalue != var->oldintvalue)
				{
					return true;
				}
				break;
			case EVT_Float:
				if (var->floatvalue[0] != var->oldfloatvalue[0])
				{
					return true;
				}
				break;
			case EVT_Vector3D:
				for (int i = 0; i < 3; i++)
				{
					if (var->floatvalue[i] != var->oldfloatvalue[i])
					{
						return true;
					}
				}
				break;
			case EVT_Vector2D:
				for (int i = 0; i < 2; i++)
				{
					if (var->floatvalue[i] != var->oldfloatvalue[i])
					{
						return true;
					}
				}
				break;
			case EVT_Quaternion:
				for (int i = 0; i < 4; i++)
				{
					if (var->floatvalue[i] != var->oldfloatvalue[i])
					{
						return true;
					}
				}
				break;
			case EVT_String:
				if (var->svalue != var->oldsvalue)
				{
					return true;
				}
				break;
			default:
				break;
		}
		return false;
	}
	void EntityVariables::writeVariable(Variable *var, Buffer *buffer)
	{
		switch (var->type)
		{
			case EVT_Integer:
				buffer->writeInt(var->intvalue);
				var->oldintvalue = var->intvalue;
				break;
			case EVT_Word:
				buffer->writeWord(var->intvalue);
				var->oldintvalue = var->intvalue;
				break;
			case EVT_Byte:
				buffer->writeByte(var->intvalue);
				var->oldintvalue = var->intvalue;
				break;
			case EVT_Float:
				buffer->writeFloat(var->floatvalue[0]);
				var->oldfloatvalue[0] = var->floatvalue[0];
				break;
			case EVT_Vector3D:
				buffer->writeVector3D(Vector3D(var->floatvalue[0], var->floatvalue[1], var->floatvalue[2]));
				var->oldfloatvalue[0] = var->floatvalue[0];
				var->oldfloatvalue[1] = var->floatvalue[1];
				var->oldfloatvalue[2] = var->floatvalue[2];
				break;
			case EVT_Vector2D:
				buffer->writeVector2D(Vector2D(var->floatvalue[0], var->floatvalue[1]));
				var->oldfloatvalue[0] = var->floatvalue[0];
				var->oldfloatvalue[1] = var->floatvalue[1];
				break;
			case EVT_Quaternion:
				buffer->writeQuaternion(Quaternion(var->floatvalue[0], var->floatvalue[1], var->floatvalue[2], var->floatvalue[3]));
				var->oldfloatvalue[0] = var->floatvalue[0];
				var->oldfloatvalue[1] = var->floatvalue[1];
				var->oldfloatvalue[2] = var->floatvalue[2];
				var->oldfloatvalue[3] = var->floatvalue[3];
				break;
			case EVT_String:
				buffer->writeString(var->svalue);
				var->oldsvalue = var->svalue;
				break;
			default:
				break;
		}
	}
	void EntityVariables::readVariable(Variable *var, Buffer *buffer)
	{
		switch (var->type)
		{
			case EVT_Integer:
				var->intvalue = buffer->readInt();
				var->oldintvalue = var->intvalue;
				break;
			case EVT_Word:
				var->intvalue = buffer->readWord();
				var->oldintvalue = var->intvalue;
				break;
			case EVT_Byte:
				var->intvalue = buffer->readByte();
				var->oldintvalue = var->intvalue;
				break;
			case EVT_Float:
				var->floatvalue[0] = buffer->readFloat();
				var->oldfloatvalue[0] = var->floatvalue[0];
				break;
			case EVT_Vector3D:
			{
				Vector3D vector = buffer->readVector3D();
				var->oldfloatvalue[0] = var->floatvalue[0] = vector.x;
				var->oldfloatvalue[1] = var->floatvalue[1] = vector.y;
				var->oldfloatvalue[2] = var->floatvalue[2] = vector.z;
				break;
			}
			case EVT_Vector2D:
			{
				Vector2D vector = buffer->readVector2D();
				var->oldfloatvalue[0] = var->floatvalue[0] = vector.x;
				var->oldfloatvalue[1] = var->floatvalue[1] = vector.y;
				break;
			}
			case EVT_Quaternion:
			{
				Quaternion quat = buffer->readQuaternion();
				var->oldfloatvalue[0] = var->floatvalue[0] = quat.q[0];
				var->oldfloatvalue[1] = var->floatvalue[1] = quat.q[1];
				var->oldfloatvalue[2] = var->floatvalue[2] = quat.q[2];
				var->oldfloatvalue[3] = var->floatvalue[3] = quat.q[3];
				break;
			}
			case EVT_String:
				var->svalue = buffer->readString();
				var->oldsvalue = var->svalue;
				break;
			default:
				break;
		}
		var->updated = true;
	}
}

