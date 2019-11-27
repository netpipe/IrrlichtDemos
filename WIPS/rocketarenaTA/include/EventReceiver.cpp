/*

Event Receiver - cpp file

*/

#include "EventReceiver.h"

// constructor
cEventReceiver::cEventReceiver() {
	for (int i=0; i<KEY_KEY_CODES_COUNT; i++) {
		Keys[i] = false;
		KeysTemp[i] = false;
		KeysPressed[i] = false;
	}
	
}

// destructor
cEventReceiver::~cEventReceiver() {
}

bool cEventReceiver::OnEvent(const SEvent& event) {
	
	if (event.EventType==irr::EET_KEY_INPUT_EVENT) {
		
		irr::EKEY_CODE c = event.KeyInput.Key;
		
		Keys[c] = event.KeyInput.PressedDown;

		return true;
		
	}
	
	if (event.EventType==irr::EET_MOUSE_INPUT_EVENT) {
	
		mouseX = event.MouseInput.X;
		mouseY = event.MouseInput.Y;
		
		mouseInput = event.MouseInput.Event;
	
		return true;	
	}
	
	return false;
}

bool cEventReceiver::checkKey(irr::EKEY_CODE i) {

	if (Keys[i]==true && KeysPressed[i]==false) {
		KeysPressed[i]=true;
		return true;
	} else {

		if (Keys[i]==false)
			KeysPressed[i]=false;

		return false;
	}
	
}
