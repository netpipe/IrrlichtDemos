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

#include "peakengine/entity/Vector2FProperty.hpp"

namespace peak
{
	static Vector2F defval;

	Vector2FProperty::Vector2FProperty(Entity *entity) : Property(entity),
		defaultval(defval)
	{
	}
	Vector2FProperty::~Vector2FProperty()
	{
	}

	void Vector2FProperty::init(Vector2F &defaultval)
	{
		this->defaultval = defaultval;
		value = defaultval;
	}

	void Vector2FProperty::serialize(BufferPointer buffer)
	{
		buffer->writeFloat(value.x);
		buffer->writeFloat(value.y);
	}
	void Vector2FProperty::deserialize(BufferPointer buffer)
	{
		Vector2F newvalue;
		newvalue.x = buffer->readFloat();
		newvalue.y = buffer->readFloat();
		if (newvalue == value)
			return;
		value = newvalue;
		setChanged();
	}

	bool Vector2FProperty::hasChanged()
	{
		return value != defaultval;
	}

	void Vector2FProperty::set(const Vector2F &value)
	{
		if (value == this->value)
			return;
		this->value = value;
		setChanged();
	}
	Vector2F Vector2FProperty::get()
	{
		return value;
	}
}
