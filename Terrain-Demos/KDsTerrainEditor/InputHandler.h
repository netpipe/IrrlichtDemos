/// ================================================================================================== 
/// MastEventReceiver code is © (Copyright) Robert E. Demarest, AKA Mastiff or Mastiff Odit 
/// This file may be used in any non-commercial or commercial project as long as the following conditions are met: 
/// You may not claim this code as being your own. 
/// You may not use this code for any harmful, malicious or otherwise damaging programs. 
/// 
/// This is version 1.2a of the class. 
/// This class is designed for use with the Irrlicht Engine, it was written for version 1.3 of the engine. 
/// ================================================================================================== 
/// Optimized and split into header and footer by Alexander Varwijk © All rights reserved
#ifndef EVENT_RECEIVER_ONCE
#define EVENT_RECEIVER_ONCE
#include "CCompositeEventReceiver.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

class InputHandler : public CCompositeEventReceiver
{
public:
	//typedef void(*callbackfunction)(SEvent, IGUIElement*); //Needs to be here to be used in protected struct
protected: 
	// Enumeration for UP, DOWN, PRESSED and RELEASED key states. Also used for mouse button states. 
	enum keyStatesENUM {UP, DOWN, PRESSED, RELEASED}; 

	// Enumeration for Event Handling State. 
	enum processStateENUM {STARTED, ENDED}; 

	// Mouse button states. 
	keyStatesENUM mouseButtonState[2]; //Left(0), Middle(1) and Right(2) Buttons. 

	// Keyboard key states. 
	keyStatesENUM keyState[KEY_KEY_CODES_COUNT]; 

	// Mouse X/Y coordinates and Wheel data. 
	struct mouseData 
	{ 
		int X; 
		int Y; 
		float wheel; //wheel is how far the wheel has moved 
	}; 
	struct mouseData mouse; 

	processStateENUM processState; // STARTED = handling events, ENDED = not handling events 

	//The event receiver used by irrlicht
	virtual bool OnEvent(const SEvent& event);

	////////////////////// 
	// Public functions 
	////////////////////// 
public: 
	
	InputHandler();

	float mouseWheel();

	int mouseX();

	int mouseY();

	bool leftMouseReleased();
	bool leftMouseUp();
	bool leftMousePressed();
	bool leftMouseDown();

	bool middleMouseReleased();
	bool middleMouseUp();
	bool middleMousePressed();
	bool middleMouseDown();

	bool rightMouseReleased();
	bool rightMouseUp();
	bool rightMousePressed();
	bool rightMouseDown(); 

	bool keyPressed(char keycode);
	bool keyDown(char keycode);
	bool keyUp(char keycode);
	bool keyReleased(char keycode);


	// This is used so that the Key States will not be changed during execution of your Main game loop. 
	// Place this at the very START of your Main Loop 
	void endEventProcess();

	// This is used so that the Key States will not be changed during execution of your Main game loop. 
	// Place this function at the END of your Main Loop. 
	void startEventProcess();
}; 
#endif /*EVENT_RECEIVER_ONCE*/