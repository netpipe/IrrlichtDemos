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

#include "peakengine/entity/FloatProperty.hpp"

namespace peak
{
	FloatProperty::FloatProperty(Entity *entity) : Property(entity),
		defaultval(0.0f), value(0.0f)
	{
	}
	FloatProperty::~FloatProperty()
	{
	}

	void FloatProperty::init(float defaultval)
	{
		this->defaultval = defaultval;
		value = defaultval;
	}

	void FloatProperty::serialize(BufferPointer buffer)
	{
		buffer->writeFloat(value);
	}
	void FloatProperty::deserialize(BufferPointer buffer)
	{
		float newvalue = buffer->readFloat();
		if (newvalue == value)
			return;
		value = newvalue;
		setChanged();
	}

	bool FloatProperty::hasChanged()
	{
		return value != defaultval;
	}

	void FloatProperty::set(float value)
	{
		if (value == this->value)
			return;
		this->value = value;
		setChanged();
	}
	float FloatProperty::get()
	{
		return value;
	}
}
