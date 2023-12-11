/****************************************
	Copyright 2018  Mahmoud Galal
****************************************/
#pragma once

#include <irrlicht.h>
using namespace irr;
using namespace core;

class InputEventHandler :
	public IEventReceiver
{
public:
	// This is the one method that we have to implement
    virtual bool OnEvent(const SEvent& event);
   

    // This is used to check whether a key is being held down
    virtual bool IsKeyDown(EKEY_CODE keyCode) const;
	//reset last detected state
	void ResetCode(EKEY_CODE keyCode);
   
	InputEventHandler(void);
	~InputEventHandler(void);

	private:
	
    // We use this array to store the current state of each key
    bool KeyIsDown[KEY_KEY_CODES_COUNT];
};

