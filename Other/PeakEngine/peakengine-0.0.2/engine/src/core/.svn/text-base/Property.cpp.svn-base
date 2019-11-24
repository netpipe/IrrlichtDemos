
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

#include "core/Property.h"
#include "core/Link.h"
#include "core/GameObject.h"
#include "core/Buffer.h"

namespace peak
{
	void Property::addLink(Link *link)
	{
		links.push_back(link);
	}
	void Property::removeLink(Link *link)
	{
		// Look for link to remove
		for (unsigned int i = 0; i < links.size(); i++)
		{
			if (links[i] == link)
			{
				links.erase(links.begin() + i);
				return;
			}
		}
	}
	void Property::update(void)
	{
		// Call links
		for (unsigned int i = 0; i < links.size(); i++)
		{
			links[i]->update();
		}
		// Inform owner that property has been updated
		if (object && updates)
		{
			object->onPropertyChanged(name);
		}
	}
	
	void Property::disableUpdates(void)
	{
		updates = false;
	}
	void Property::enableUpdates(void)
	{
		updates = true;
	}

	void Vector3DProperty::set(std::string members, std::vector<float> value)
	{
		for (unsigned int i = 0; i < members.size(); i++)
		{
			if (members[i] == 'x')
			{
				v.x = value[i];
			}
			else if (members[i] == 'y')
			{
				v.y = value[i];
			}
			else if (members[i] == 'z')
			{
				v.z = value[i];
			}
		}
	}
	std::vector<float> Vector3DProperty::getValueArray(std::string members)
	{
		std::vector<float> value;
		for (unsigned int i = 0; i < members.size(); i++)
		{
			if (members[i] == 'x')
			{
				value.push_back(v.x);
			}
			else if (members[i] == 'y')
			{
				value.push_back(v.y);
			}
			else if (members[i] == 'z')
			{
				value.push_back(v.z);
			}
			else
			{
				value.push_back(0);
			}
		}
		return value;
	}
	void Vector3DProperty::serialize(Buffer *buffer)
	{
		buffer->writeVector3D(v);
	}
	void Vector3DProperty::deserialize(Buffer *buffer)
	{
		v = buffer->readVector3D();
		update();
	}
	void Vector2DProperty::set(std::string members, std::vector<float> value)
	{
		for (unsigned int i = 0; i < members.size(); i++)
		{
			if (members[i] == 'x')
			{
				v.x = value[i];
			}
			else if (members[i] == 'y')
			{
				v.y = value[i];
			}
		}
	}
	std::vector<float> Vector2DProperty::getValueArray(std::string members)
	{
		std::vector<float> value;
		for (unsigned int i = 0; i < members.size(); i++)
		{
			if (members[i] == 'x')
			{
				value.push_back(v.x);
			}
			else if (members[i] == 'y')
			{
				value.push_back(v.y);
			}
			else
			{
				value.push_back(0);
			}
		}
		return value;
	}
	void Vector2DProperty::serialize(Buffer *buffer)
	{
		buffer->writeVector2D(v);
	}
	void Vector2DProperty::deserialize(Buffer *buffer)
	{
		v = buffer->readVector2D();
		update();
	}
	void QuaternionProperty::set(std::string members, std::vector<float> value)
	{
		for (unsigned int i = 0; i < members.size(); i++)
		{
			if (members[i] == 'x')
			{
				q.x = value[i];
			}
			else if (members[i] == 'y')
			{
				q.y = value[i];
			}
			else if (members[i] == 'z')
			{
				q.z = value[i];
			}
			else if (members[i] == 'w')
			{
				q.w = value[i];
			}
		}
	}
	std::vector<float> QuaternionProperty::getValueArray(std::string members)
	{
		std::vector<float> value;
		for (unsigned int i = 0; i < members.size(); i++)
		{
			if (members[i] == 'x')
			{
				value.push_back(q.x);
			}
			else if (members[i] == 'y')
			{
				value.push_back(q.y);
			}
			else if (members[i] == 'z')
			{
				value.push_back(q.z);
			}
			else if (members[i] == 'w')
			{
				value.push_back(q.w);
			}
			else
			{
				value.push_back(0);
			}
		}
		return value;
	}
	void QuaternionProperty::serialize(Buffer *buffer)
	{
		buffer->writeQuaternion(q);
	}
	void QuaternionProperty::deserialize(Buffer *buffer)
	{
		q = buffer->readQuaternion();
		update();
	}
	void IntProperty::serialize(Buffer *buffer)
	{
		buffer->writeInt(i);
	}
	void IntProperty::deserialize(Buffer *buffer)
	{
		i = buffer->readInt();
		update();
	}
}
