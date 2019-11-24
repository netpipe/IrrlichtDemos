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

#include "peakengine/entity/IntProperty.hpp"

namespace peak
{
	IntProperty::IntProperty(Entity *entity) : Property(entity),
		defaultval(0), value(0), bits(32)
	{
	}
	IntProperty::~IntProperty()
	{
	}

	void IntProperty::init(int defaultval, unsigned int bits)
	{
		this->defaultval = defaultval;
		value = defaultval;
		this->bits = bits;
	}

	void IntProperty::serialize(BufferPointer buffer)
	{
		buffer->writeInt(value, bits);
	}
	void IntProperty::deserialize(BufferPointer buffer)
	{
		int newvalue = buffer->readInt(bits);
		if (newvalue == value)
			return;
		value = newvalue;
		setChanged();
	}

	bool IntProperty::hasChanged()
	{
		return value != defaultval;
	}

	void IntProperty::set(int value)
	{
		if (value == this->value)
			return;
		this->value = value;
		setChanged();
	}
	int IntProperty::get()
	{
		return value;
	}
}
