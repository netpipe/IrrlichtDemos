#pragma once
#include <irrlicht.h>
#include <iostream>
#include "Player.h"
#include "AI.h"
using namespace irr;

class CEventReceiver : public IEventReceiver
{
public:
	bool keys[irr::KEY_KEY_CODES_COUNT] ;

	CEventReceiver(IrrlichtDevice* device, Player* p, AI* g);
	virtual bool OnEvent(const SEvent &event);
	bool IsKeyDown(int index);
	IrrlichtDevice* device;
	Player* p;
	AI* g;
	core::vector3df sideDir;
	irr::s32 cx;
	irr::s32 cy;
};
