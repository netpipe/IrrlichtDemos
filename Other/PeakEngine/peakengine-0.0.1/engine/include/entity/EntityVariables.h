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

#ifndef _ENTITY_VARIABLES_H_
#define _ENTITY_VARIABLES_H_

#include "core/Vector3D.h"
#include "core/Vector2D.h"
#include "core/Quaternion.h"

#include <string>
#include <map>

//tolua_begin
namespace peak
{
	class IniFile;
	class Buffer;
	
	enum VariableType
	{
		EVT_None = 0,
		EVT_Integer = 1,
		EVT_Word = 2,
		EVT_Byte = 3,
		EVT_Float = 4,
		EVT_Vector3D = 5,
		EVT_Vector2D = 6,
		EVT_Quaternion = 7,
		EVT_String = 8
	};
	
	struct Variable
	{
		Variable()
		{
			updated = false;
		}
		
		int getInteger(void)
		{
			return intvalue;
		}
		float getFloat(void)
		{
			return floatvalue[0];
		}
		Vector3D getVector3D(void)
		{
			return Vector3D(floatvalue[0], floatvalue[1], floatvalue[2]);
		}
		Vector2D getVector2D(void)
		{
			return Vector2D(floatvalue[0], floatvalue[1]);
		}
		Quaternion getQuaternion(void)
		{
			return Quaternion(floatvalue[0], floatvalue[1], floatvalue[2], floatvalue[3]);
		}
		std::string getString(void)
		{
			return svalue;
		}
		bool wasUpdated(void)
		{
			return updated;
		}
		//tolua_end
		std::string name;
		VariableType type;
		
		union
		{
			int intvalue;
			float floatvalue[4];
		};
		union
		{
			int oldintvalue;
			float oldfloatvalue[4];
		};
		std::string svalue;
		std::string oldsvalue;
		
		bool updated;
		//tolua_begin
	};
	
	/**
	 * \brief Class holding the dynamic variables of an entity
	 *
	 * Variables are synced via network automatically via reliable connections.
	 *
	 * There are different types of variables:
	 * - "vector2d": Vector2D
	 * - "vector3d": Vector3D
	 * - "quaternion": Quaternion
	 * - "integer": int
	 * - "word": short
	 * - "word": char
	 * - "float": float
	 * - "string": std::string
	 */
	class EntityVariables
	{
		public:
			EntityVariables();
			~EntityVariables();
			
			/**
			 * \brief Creates the variables
			 * \param file File of the entity
			 */
			bool load(IniFile *file);
			/**
			 * \brief Deletes the variables
			 */
			bool destroy(void);
			
			/**
			 * \brief Sets a variable to a new value
			 */
			void setInteger(std::string name, int value);
			void setFloat(std::string name, float value);
			void set(std::string name, Vector3D value);
			void set(std::string name, Vector2D value);
			void set(std::string name, Quaternion value);
			void set(std::string name, std::string value);
			
			/**
			 * \brief Returns the value of a variable
			 */
			int getInteger(std::string name);
			float getFloat(std::string name);
			Vector3D getVector3D(std::string name);
			Vector2D getVector2D(std::string name);
			Quaternion getQuaternion(std::string name);
			std::string getString(std::string name);
			
			/**
			 * \brief Returns whether the variables needs an update
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
			 * \brief Updates the variables with data read from a buffer
			 */
			bool updateFromData(Buffer *buffer);
			
			/**
			 * \brief Writes the complete data into a buffer
			 */
			bool writeCompleteData(Buffer *buffer);
			/**
			 * \brief Reads the complete data from a buffer
			 */
			bool readCompleteData(Buffer *buffer);
			
			/**
			 * \brief Returns whether the variable was updated by another computer.
			 */
			bool wasUpdated(std::string name);
			
			Variable *getVariable(std::string name);
			Variable *getVariable(int id);
			//tolua_end
			bool hasChanged(Variable *var);
			void writeVariable(Variable *var, Buffer *buffer);
			void readVariable(Variable *var, Buffer *buffer);
		private:
			std::map<int, Variable*> variables;
			//tolua_begin
	};
}
//tolua_end

#endif

