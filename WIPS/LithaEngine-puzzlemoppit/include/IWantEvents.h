
#ifndef I_WANT_EVENTS_H
#define I_WANT_EVENTS_H

#include "litha_internal.h"

class Event;

class IWantEvents
{
public:
	virtual ~IWantEvents(){}
	
	virtual void OnEvent(const Event &event) = 0;
};

#endif
