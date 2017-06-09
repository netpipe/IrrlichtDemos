#include "CEventReceiver.h"

CEventReceiver::CEventReceiver()
{
	for (u32 i = 0; i < KEY_KEY_CODES_COUNT; ++i)
		KeyIsDown[i] = false;
}

CEventReceiver::~CEventReceiver()
{
}

bool CEventReceiver::OnEvent(const SEvent& event)
{
	if (event.EventType == irr::EET_KEY_INPUT_EVENT)
        KeyIsDown[event.KeyInput.Key] = event.KeyInput.PressedDown;

	return false;
}

bool CEventReceiver::IsKeyDown(EKEY_CODE keyCode) const
{
	if (keyCode > KEY_KEY_CODES_COUNT || keyCode < 0)
		return false;

	return KeyIsDown[keyCode];
}