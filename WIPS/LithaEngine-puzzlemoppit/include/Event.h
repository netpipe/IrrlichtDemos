
#ifndef EVENT_H
#define EVENT_H

#include "litha_internal.h"
#include "IEngine.h"
#include "IWantEvents.h"

// an event is basically a map of Variants
class Event
{
	core::stringc name;
	VariantMap params;
	
	// used for const reference, when key is not present.
	Variant emptyParam;
	
public:
	Event()
	{
	}
	
	Event(const core::stringc &eventName)
		: name(eventName)
	{
	}
	
	const core::stringc &GetTypeName() const
	{
		return name;
	}
	
	bool IsType(const core::stringc &typeName) const
	{
		return typeName == name;
	}
	
	bool HasKey(const core::stringc &key) const
	{
		return params.count(key) > 0;
	}
	
	// Call the given receiver's OnEvent.
	// This is mostly only a convenience function used internally.
	// Generally this function should not be called; events should be posted
	// using the methods provided by Engine.
	void Send(IWantEvents *receiver)
	{
		receiver->OnEvent(*this);
	}
	
	Variant &operator[](const core::stringc &key)
	{
		return params[key];
	}
	
	const Variant &operator[](const core::stringc &key) const
	{
		std::map<core::stringc, Variant>::const_iterator iter = params.find(key);
		
		if (iter == params.end())
			return emptyParam; // not found
		else
			return iter->second;
	}
	
	Variant &operator[](const c8 *key)
	{
		return (*this)[core::stringc(key)];
	}
	
	const Variant &operator[](const c8 *key) const
	{
		return (*this)[core::stringc(key)];
	}
};

#endif

