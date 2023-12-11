#ifndef _INCLUDED_CCOMPOSITEEVENTRECEIVER_H
#define _INCLUDED_CCOMPOSITEEVENTRECEIVER_H

#include <irrlicht.h>

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
		printf("CCompositeEventReceiver: Can't add to a leaf\n");
	}
 
	virtual void remove(CCompositeEventReceiver*){
		printf("CCompositeEventReceiver: Can't remove from a leaf\n");
	}
};

#endif //_INCLUDED_CCOMPOSITEEVENTRECEIVER_H