
#include "core/DirectLink.h"
#include "core/Property.h"
#include "core/GameObject.h"

namespace peak
{
	DirectLink::DirectLink()
	{
	}
	DirectLink::~DirectLink()
	{
	}
	
	
	bool DirectLink::create(Property *source, Property *target)
	{
		source->addLink(this);
		this->source = source;
		this->target = target;
		return true;
	}
	bool DirectLink::destroy(void)
	{
		source->removeLink(this);
		return true;
	}
			
	void DirectLink::update(void)
	{
		//target->setValue(source->getValueArray());
		target->set(source);
	}
}
