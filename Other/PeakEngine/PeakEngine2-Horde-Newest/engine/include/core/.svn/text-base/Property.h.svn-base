
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

#ifndef _PROPERTY_H_
#define _PROPERTY_H_

#include "core/ReferenceCounted.h"
#include "core/Vector3D.h"
#include "core/Vector2D.h"
#include "core/Quaternion.h"

#include <vector>

//tolua_begin
namespace peak
{
	enum PropertyType
	{
		EPT_Vector3D,
		EPT_Vector2D,
		EPT_Float,
		EPT_Boolean,
		EPT_Integer,
		EPT_Quaternion,
		EPT_Array
	};

	class Link;
	class Event;
	class GameObject;
	class Buffer;

	/**
	 * \brief Base class for all property classes
	 */
	class Property : public ReferenceCounted
	{
		public:
			Property(std::string name)
			{
				this->name = name;
				updates = true;
				object = 0;
			}
			virtual ~Property() {};
			
			/**
			 * \brief Sets the name of the property
			 */
			virtual void setName(std::string name)
			{
				this->name = name;
			}
			/**
			 * \brief Returns the name of the property
			 */
			virtual std::string getName(void)
			{
				return name;
			}
			
			/**
			 * \brief Returns the type of the property
			 */
			virtual PropertyType getType(void) = 0;
			
			/**
			 * \brief Sets the value as a Vector3D
			 */
			virtual void set(Vector3D v) {};
			/**
			 * \brief Returns the value as a Vector3D
			 */
			virtual Vector3D getVector3D(void) { return Vector3D(0, 0, 0); };
			/**
			 * \brief Sets the value as a Vector2D
			 */
			virtual void set(Vector2D v) {};
			/**
			 * \brief Returns the value as a Vector2D
			 */
			virtual Vector2D getVector2D(void) { return Vector2D(0, 0); };
			/**
			 * \brief Sets the value as a Quaternion
			 */
			virtual void set(Quaternion q) {};
			/**
			 * \brief Returns the value as a Quaternion
			 */
			virtual Quaternion getQuaternion(void) { return Quaternion(0, 0, 0, 0); };
			/**
			 * \brief Sets the value as a bool
			 */
			virtual void set(bool b) {};
			/**
			 * \brief Returns the value as a bool
			 */
			virtual bool getBool(void) { return false; };
			/**
			 * \brief Sets the value as an int
			 */
			virtual void set(int i) {};
			/**
			 * \brief Returns the value as an int
			 */
			virtual int getInt(void) { return 0; };
			/**
			 * \brief Sets the value as a float
			 */
			virtual void set(float f) {};
			/**
			 * \brief Returns the value as a float
			 */
			virtual float getFloat(void) { return 0; };
			
			/**
			 * \brief Sets several parts of the value as floats
			 */
			void set(std::string members, std::vector<float> value) {};
			/**
			 * \brief Returns several parts of the value as floats
			 */
			std::vector<float> getValueArray(std::string members) { return std::vector<float>(); };
			
			/**
			 * \brief Sets the property according to another
			 */
			virtual void set(Property *p) {};
			
			/**
			 * \brief Writes the value into a buffer
			 */
			virtual void serialize(Buffer *buffer) {};
			/**
			 * \brief Reads the value from a buffer
			 */
			virtual void deserialize(Buffer *buffer) {};
			
			/**
			 * \brief Sets the game object of the property
			 */
			void setGameObject(GameObject *object)
			{
				this->object = object;
			}
			/**
			 * \brief Returns the game object of the property
			 */
			GameObject *getGameObject(void)
			{
				return object;
			}
			
			/**
			 * \brief Adds a link to the property
			 */
			void addLink(Link *link);
			/**
			 * \brief Removes a link from the property
			 */
			void removeLink(Link *link);

			/**
			 * \brief Triggers links attached to the property
			 */
			virtual void update(void);
			/**
			 * \brief Disables updates for all set() functions
			 */
			virtual void disableUpdates(void);
			/**
			 * \brief Enables updates for all set() functions
			 */
			virtual void enableUpdates(void);
			
			//tolua_end
		protected:
			std::string name;
			GameObject *object;
			std::vector<Link*> links;
			bool updates;
			//tolua_begin
	};
	/**
	 * \brief Property which holds a Vector3D
	 */
	class Vector3DProperty : public Property
	{
		public:
			Vector3DProperty(std::string name = "") : Property(name)
			{
			}
			
			virtual PropertyType getType(void) { return EPT_Vector3D; };
			
			virtual void set(Vector3D v) { this->v = v; update(); };
			virtual Vector3D getVector3D(void) { return v; };
			
			void set(std::string members, std::vector<float> value);
			std::vector<float> getValueArray(std::string members);
			
			virtual void set(Property *p)
			{
				if (p->getType() == EPT_Vector3D)
				{
					v = ((Vector3DProperty*)p)->v;
				}
				update();
			}
			
			virtual void serialize(Buffer *buffer);
			virtual void deserialize(Buffer *buffer);
			//tolua_end
		private:
			Vector3D v;
			//tolua_begin
	};
	/**
	 * \brief Property which holds a Vector2D
	 */
	class Vector2DProperty : public Property
	{
		public:
			Vector2DProperty(std::string name = "") : Property(name)
			{
			}
			
			virtual PropertyType getType(void) { return EPT_Vector2D; };
			
			virtual void set(Vector2D v) { this->v = v; update(); };
			virtual Vector2D getVector2D(void) { return v; };
			
			void set(std::string members, std::vector<float> value);
			std::vector<float> getValueArray(std::string members);
			
			virtual void set(Property *p)
			{
				if (p->getType() == EPT_Vector2D)
				{
					v = ((Vector2DProperty*)p)->v;
				}
				update();
			}
			
			virtual void serialize(Buffer *buffer);
			virtual void deserialize(Buffer *buffer);
			//tolua_end
		private:
			Vector2D v;
			//tolua_begin
	};
	/**
	 * \brief Property which holds a Quaternion
	 */
	class QuaternionProperty : public Property
	{
		public:
			QuaternionProperty(std::string name = "") : Property(name)
			{
			}
			
			virtual PropertyType getType(void) { return EPT_Quaternion; };
			
			virtual void set(Quaternion q) { this->q = q; update(); };
			virtual Quaternion getQuaternion(void) { return q; };
			
			void set(std::string members, std::vector<float> value);
			std::vector<float> getValueArray(std::string members);
			
			virtual void set(Property *p)
			{
				if (p->getType() == EPT_Quaternion)
				{
					q = ((QuaternionProperty*)p)->q;
				}
				update();
			}
			
			virtual void serialize(Buffer *buffer);
			virtual void deserialize(Buffer *buffer);
			//tolua_end
		private:
			Quaternion q;
			//tolua_begin
	};
	/**
	 * \brief Property which holds an int
	 */
	class IntProperty : public Property
	{
		public:
			IntProperty(std::string name = "") : Property(name)
			{
			}
			
			virtual PropertyType getType(void) { return EPT_Integer; };
			
			virtual void set(int i) { this->i = i; update(); };
			virtual int getInt(void) { return i; };
			virtual void set(float f) { this->i = (int)f; update(); };
			virtual float getFloat(void) { return i; };
			
			virtual void set(Property *p)
			{
				if (p->getType() == EPT_Integer)
				{
					i = ((IntProperty*)p)->i;
				}
				update();
			}
			
			virtual void serialize(Buffer *buffer);
			virtual void deserialize(Buffer *buffer);
			//tolua_end
		private:
			int i;
			//tolua_begin
	};
}
//tolua_end

#endif
