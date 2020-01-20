#ifndef _INCLUDED_CCOMPOSITEEVENTRECEIVER_H
#define _INCLUDED_CCOMPOSITEEVENTRECEIVER_H

#include <irrlicht.h>
#include <iostream>
#include <stdexcept>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

struct CCompositeEventReceiver : public IEventReceiver
{
	virtual bool OnEvent(const SEvent& event)=0;

	virtual void add(CCompositeEventReceiver*) {
		throw std::runtime_error("CCompositeEventReceiver: Can't add to a leaf");
	}

	virtual void remove(CCompositeEventReceiver*){
		throw std::runtime_error("CCompositeEventReceiver: Can't remove from a leaf");
	}
};

#endif //_INCLUDED_CCOMPOSITEEVENTRECEIVER_H
