#pragma once

#include <irrlicht.h>

using namespace irr;
using namespace core;

class CEventReceiver :	public IEventReceiver
{
public:
	CEventReceiver();
	~CEventReceiver();

    virtual bool OnEvent(const SEvent& event);

	virtual bool IsKeyDown(EKEY_CODE keyCode) const;

private:
	bool KeyIsDown[KEY_KEY_CODES_COUNT];
};
