/*
Copyright (c) 2009, Mathias Gottschlag

Permission to use, copy, modify, and/or distribute this software for any
purpose with or without fee is hereby granted, provided that the above
copyright notice and this permission notice appear in all copies.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

#include "peakengine/entity/QuaternionProperty.hpp"

#include <iostream>

namespace peak
{
	static Quaternion defval;

	QuaternionProperty::QuaternionProperty(Entity *entity) : Property(entity),
		defaultval(defval)
	{
	}
	QuaternionProperty::~QuaternionProperty()
	{
	}

	void QuaternionProperty::init(Quaternion &defaultval)
	{
		this->defaultval = defaultval;
		value = defaultval;
	}

	void QuaternionProperty::serialize(BufferPointer buffer)
	{
		buffer->writeFloat(value.q[0]);
		buffer->writeFloat(value.q[1]);
		buffer->writeFloat(value.q[2]);
		buffer->writeFloat(value.q[3]);
	}
	void QuaternionProperty::deserialize(BufferPointer buffer)
	{
		Quaternion newvalue;
		newvalue.q[0] = buffer->readFloat();
		newvalue.q[1] = buffer->readFloat();
		newvalue.q[2] = buffer->readFloat();
		newvalue.q[3] = buffer->readFloat();
		if (newvalue == value)
			return;
		value = newvalue;
		setChanged();
	}

	bool QuaternionProperty::hasChanged()
	{
		return value != defaultval;
	}

	void QuaternionProperty::set(const Quaternion &value)
	{
		if (value == this->value)
			return;
		this->value = value;
		setChanged();
	}
	Quaternion QuaternionProperty::get()
	{
		return value;
	}

	QuaternionProperty16::QuaternionProperty16(Entity *entity)
		: QuaternionProperty(entity)
	{
	}

	void QuaternionProperty16::serialize(BufferPointer buffer)
	{
		buffer->writeInt((short)(value.q[0] * 32767), 16);
		buffer->writeInt((short)(value.q[1] * 32767), 16);
		buffer->writeInt((short)(value.q[2] * 32767), 16);
		buffer->writeInt((short)(value.q[3] * 32767), 16);
	}
	void QuaternionProperty16::deserialize(BufferPointer buffer)
	{
		Quaternion newvalue;
		newvalue.q[0] = (float)buffer->readInt(16) / 32767;
		newvalue.q[1] = (float)buffer->readInt(16) / 32767;
		newvalue.q[2] = (float)buffer->readInt(16) / 32767;
		newvalue.q[3] = (float)buffer->readInt(16) / 32767;
		if (newvalue == value)
			return;
		value = newvalue;
		setChanged();
	}

	bool QuaternionProperty16::hasChanged()
	{
		for (unsigned int i = 0; i < 4; i++)
		{
			if ((short)(value.q[i] * 32767) != (short)(defaultval.q[i] * 32767))
				return true;
		}
		return false;
	}
}
