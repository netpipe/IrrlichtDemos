#ifndef __COSMIC_GAMEEVENTRECEIVER_H_INCLUDED__
#define __COSMIC_GAMEEVENTRECEIVER_H_INCLUDED__


#include "GameGlobals.h"


class GameEventReceiver : public IEventReceiver
{ 
public:
	virtual bool OnEvent(const SEvent& event);
};


#endif
