#include "InputHandler.h"

bool InputHandler::OnEvent(const SEvent& event)
{
	////////////////////////////// 
	// Keyboard Input Event 
	////////////////////////////// 
	if (event.EventType == EET_KEY_INPUT_EVENT) 
	{ 
		if (processState == STARTED) 
		{ 
			// if key is Pressed Down 
			if (event.KeyInput.PressedDown == true) 
			{ 
				// If key was not down before 
				if (keyState[event.KeyInput.Key] != DOWN) 
				{ 
					keyState[event.KeyInput.Key] = PRESSED; // Set to Pressed 
				} 
				else 
				{ 
					// if key was down before 
					keyState[event.KeyInput.Key] = DOWN; // Set to Down 
				} 
			} 
			else 
			{ 

				// if the key is down 
				if (keyState[event.KeyInput.Key] != UP) 
				{ 
					keyState[event.KeyInput.Key] = RELEASED; // Set to Released 
				} 
			} 
		} 
	} 

	////////////////////////////// 
	// Mouse Input Event 
	////////////////////////////// 
	else if (event.EventType == EET_MOUSE_INPUT_EVENT) 
	{ 
		if (processState == STARTED) 
		{ 
			//Mouse changed position 
			if (event.MouseInput.Event == EMIE_MOUSE_MOVED) 
			{ 
				mouse.Y = event.MouseInput.Y; 
				mouse.X = event.MouseInput.X; 
			} 
			//Wheel moved. 
			else if (event.MouseInput.Event == EMIE_MOUSE_WHEEL) 
			{ 
				mouse.wheel += event.MouseInput.Wheel; 
			} 
			//Left Mouse Button Pressed 
			else if (event.MouseInput.Event == EMIE_LMOUSE_PRESSED_DOWN) 
			{ 
				if (mouseButtonState[0] == UP || mouseButtonState[0] == RELEASED) 
				{ 
					mouseButtonState[0] = PRESSED; 
				} 
				else 
				{ 
					mouseButtonState[0] = DOWN; 
				} 
			} 
			//Left Mouse Button Rleased 
			else if (event.MouseInput.Event == EMIE_LMOUSE_LEFT_UP) 
			{ 
				if (mouseButtonState[0] != UP) 
				{ 
					mouseButtonState[0] = RELEASED; 
				} 
			} 
			//Middle Mouse Button Pressed 
			else if (event.MouseInput.Event == EMIE_MMOUSE_PRESSED_DOWN) 
			{ 
				if (mouseButtonState[1] == UP || mouseButtonState[1] == RELEASED) 
				{ 
					mouseButtonState[1] = PRESSED; 
				} 
				else 
				{ 
					mouseButtonState[1] = DOWN; 
				} 
			} 
			//Middle Mouse Button Rleased 
			else if (event.MouseInput.Event == EMIE_MMOUSE_LEFT_UP) 
			{ 
				if (mouseButtonState[1] != UP) 
				{ 
					mouseButtonState[1] = RELEASED; 
				} 
			} 
			//Right Mouse Button Pressed 
			else if (event.MouseInput.Event == EMIE_RMOUSE_PRESSED_DOWN) 
			{ 
				if (mouseButtonState[2] == UP || mouseButtonState[2] == RELEASED) 
				{ 
					mouseButtonState[2] = PRESSED; 
				} 
				else 
				{ 
					mouseButtonState[2] = DOWN; 
				} 
			} 
			//Right Mouse Button Rleased 
			else if (event.MouseInput.Event == EMIE_RMOUSE_LEFT_UP) 
			{ 
				if (mouseButtonState[2] != UP) 
				{ 
					mouseButtonState[2] = RELEASED; 
				} 
			} 
		}
	} 

	return false; //We didn't catch any events, we only made some easier to acces
}

float InputHandler::mouseWheel()
{ 
	return mouse.wheel; 
} 

int InputHandler::mouseX()
{ 
	return mouse.X; 
} 

int InputHandler::mouseY()
{ 
	return mouse.Y; 
} 

bool InputHandler::leftMouseReleased()
{ 
	return (mouseButtonState[0] == RELEASED);
} 

bool InputHandler::leftMouseUp()
{ 
	return (mouseButtonState[0] == RELEASED || mouseButtonState[0] == UP);
} 

bool InputHandler::leftMousePressed()
{ 
	return (mouseButtonState[0] == PRESSED);
} 

bool InputHandler::leftMouseDown()
{ 
	return (mouseButtonState[0] == PRESSED || mouseButtonState[0] == DOWN);
} 

bool InputHandler::middleMouseReleased()
{ 
	return (mouseButtonState[1] == RELEASED);
} 

bool InputHandler::middleMouseUp()
{
	return (mouseButtonState[1] == RELEASED || mouseButtonState[1] == UP);
} 

bool InputHandler::middleMousePressed()
{ 
	return (mouseButtonState[1] == PRESSED);
} 

bool InputHandler::middleMouseDown()
{ 
	return (mouseButtonState[1] == PRESSED || mouseButtonState[1] == DOWN);
} 

bool InputHandler::rightMouseReleased()
{ 
	return (mouseButtonState[2] == RELEASED);
} 

bool InputHandler::rightMouseUp()
{ 
	return (mouseButtonState[2] == RELEASED || mouseButtonState[2] == UP);
} 

bool InputHandler::rightMousePressed()
{ 
	return (mouseButtonState[2] == PRESSED);
} 

bool InputHandler::rightMouseDown()
{ 
	return (mouseButtonState[2] == PRESSED || mouseButtonState[2] == DOWN);
}

bool InputHandler::keyPressed(char keycode)
{ 
	return (keyState[keycode] == PRESSED);
} 

bool InputHandler::keyDown(char keycode)
{ 
	return (keyState[keycode] == DOWN || keyState[keycode] == PRESSED);
} 

bool InputHandler::keyUp(char keycode)
{ 
	return (keyState[keycode] == UP || keyState[keycode] == RELEASED);
} 

bool InputHandler::keyReleased(char keycode)
{ 
	return (keyState[keycode] == RELEASED);
} 

void InputHandler::endEventProcess()
{ 
	processState = ENDED; 
} 

void InputHandler::startEventProcess()
{ 
	processState = STARTED; 
	//Keyboard Key States 
	for (int i = 0; i < KEY_KEY_CODES_COUNT; i++) 
	{ 
		if (keyState[i] == RELEASED) 
		{ 
			keyState[i] = UP; 
		} 
		else if (keyState[i] == PRESSED) 
		{ 
			keyState[i] = DOWN; 
		} 
	} 
	//Mouse Button States 
	for (int i = 0; i <= 2; i++) 
	{ 
		if (mouseButtonState[i] == RELEASED) 
		{ 
			mouseButtonState[i] = UP; 
		} 
		else if (mouseButtonState[i] == PRESSED) 
		{ 
			mouseButtonState[i] = DOWN; 
		} 
	} 
	//Mouse Wheel state 
	mouse.wheel = 0.0f; 
} 

InputHandler::InputHandler()
{ 
	//KeyBoard States. 
	for (int i = 0; i <= KEY_KEY_CODES_COUNT; i++) 
	{ 
		keyState[i] = UP; 
	} 
	//Mouse states 
	for (int i = 0; i <= 2; i++) 
	{ 
		mouseButtonState[i] = UP; 
	} 
	//Mouse X/Y coordenates. 
	mouse.X = 0; 
	mouse.Y = 0; 
	mouse.wheel = 0.0f; 
} 