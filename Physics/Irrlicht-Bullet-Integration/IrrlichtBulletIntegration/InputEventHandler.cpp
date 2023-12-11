/****************************************
Copyright 2018  Mahmoud Galal
****************************************/
#include "InputEventHandler.h"


InputEventHandler::InputEventHandler(void)
{
	for (u32 i=0; i<KEY_KEY_CODES_COUNT; ++i)
		KeyIsDown[i] = false;
}

// This is the one method that we have to implement
bool InputEventHandler::OnEvent(const SEvent& event)
{
	// Remember whether each key is down or up
	if (event.EventType == irr::EET_KEY_INPUT_EVENT)
		KeyIsDown[event.KeyInput.Key] = event.KeyInput.PressedDown;

	return false;
}

// This is used to check whether a key is being held down
bool InputEventHandler::IsKeyDown(EKEY_CODE keyCode)const
{
	bool ret = KeyIsDown[keyCode];

	return ret;
}
void InputEventHandler::ResetCode(EKEY_CODE keyCode){
	KeyIsDown[keyCode] = false;
}


InputEventHandler::~InputEventHandler(void)
{
}
