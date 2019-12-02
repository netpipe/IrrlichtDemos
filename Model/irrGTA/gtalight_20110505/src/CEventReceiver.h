// Copyright (C) 2007-2011 David Kment
// For license see license.txt

#ifndef _CEVENTRECEIVER_H_
#define _CEVENTRECEIVER_H_

#include "irrlicht.h"
#include "IEventReceiver.h"

using namespace irr;
using namespace core;

class CEventReceiver : public IEventReceiver
{
public:

    CEventReceiver();
	bool OnEvent(const SEvent& event);
	int getFarValue();

private:
    int FarValue;
};

#endif /* _CEVENTRECEIVER_H_ */
