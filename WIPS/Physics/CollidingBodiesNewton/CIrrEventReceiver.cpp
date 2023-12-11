/******************************************************************************
 * CIrrEventReceiver
 * =================
 *
 * CIrrEventReceiver has no restrictions. Credit would be appreciated, but not
 * required.
 ******************************************************************************/
 
#include <stdio.h>
#include "CIrrEventReceiver.h"

CIrrEventReceiver::CIrrEventReceiver()
{
	for(u32 i = 0; i < NUMBER_OF_KEYS; i++)
		Keys[i] = BS_UP;
	
	Mouse[MB_LEFT] = Mouse[MB_MIDDLE] = Mouse[MB_RIGHT] = MBS_UP;

	MouseData.x = MouseData.y = MouseData.lx = MouseData.ly = MouseData.cx = MouseData.cy = 0;
	MouseData.wheelPos = MouseData.lwheelPos = 0;

	deltaMouseX = deltaMouseY = 0;

	mouseHasMoved = false;
	
	btnOnePressed = btnTwoPressed = btnThreePressed = false;
}

CIrrEventReceiver::~CIrrEventReceiver() 
{ 
}

bool CIrrEventReceiver::isKeyUp(EKEY_CODE key)
{
	if (Keys[key] == BS_UP)
		return true;
	return false;
}

bool CIrrEventReceiver::isKeyDown(EKEY_CODE key)
{
	if (Keys[key] == BS_DOWN)
		return true;
	return false;
}

bool CIrrEventReceiver::isKeyPressed(EKEY_CODE key)
{
  if(Keys[key] == BS_PRESSED)
    return true;
  else 
    return false;
}

bool CIrrEventReceiver::isKeyReleased(EKEY_CODE key)
{
  if(Keys[key] == BS_RELEASED)
    return true;
  else
    return false;
}

bool CIrrEventReceiver::isMouseButtonUp(mouseButton mb)
{
	if (Mouse[mb] == MBS_UP)
		return true;
	return false;
}

bool CIrrEventReceiver::isMouseButtonDown(mouseButton mb)
{
	if (Mouse[mb] == MBS_DOWN)
		return true;
	return false;
}


bool CIrrEventReceiver::isMouseButtonPressed(mouseButton mb)
{
	if (Mouse[mb] == MBS_PRESSED)
		return true;
	return false;
}

bool CIrrEventReceiver::isMouseButtonReleased(mouseButton mb)
{
	if (Mouse[mb] == MBS_RELEASED)
		return true;
	return false;
}

int CIrrEventReceiver::getDeltaMousePosX()
{
	return deltaMouseX;
}

int CIrrEventReceiver::getDeltaMousePosY()
{
	return deltaMouseY;
}

bool CIrrEventReceiver::mouseMoved()
{
	if(mouseHasMoved)
	{
		mouseHasMoved = false; // reset its state (if you dont do this it will be always true).
		return true;
	}

	else
		return false;
}

// for graphical user interface (just the buttons):
bool CIrrEventReceiver::isButtonOnePressed()
{
	return btnOnePressed;
}

bool CIrrEventReceiver::isButtonTwoPressed()
{
	return btnTwoPressed;
}

bool CIrrEventReceiver::isButtonThreePressed()
{
	return btnThreePressed;
}

void CIrrEventReceiver::resetButtonOnePressed()
{
	// reset its state (if you dont do this it will be always true).
	btnOnePressed = false;
}

void CIrrEventReceiver::resetButtonTwoPressed()
{
	// reset its state (if you dont do this it will be always true).
	btnTwoPressed = false;
}

void CIrrEventReceiver::resetButtonThreePressed()
{
	// reset its state (if you dont do this it will be always true).
	btnThreePressed = false;
}
// end gui button functions


// This is used so that the Key States will not be changed during execution of your Main game loop.
// Place this function at the END of your Main Loop.
void CIrrEventReceiver::startEventProcess()
{
	procesState = STARTED;

	// Keyboard Key States
	for(int i = 0; i < KEY_KEY_CODES_COUNT; i++)
	{
		if(Keys[i] == BS_RELEASED)
			Keys[i] = BS_UP;
		
		if(Keys[i] == BS_PRESSED)
			Keys[i] = BS_DOWN;
	}

	for(int i = 0; i < NUMBER_OF_MOUSE_BUTTONS; i++)
	{
		if(Mouse[i] == MBS_RELEASED)
			Mouse[i] = MBS_UP;

		if(Mouse[i] == MBS_PRESSED)
			Mouse[i] = MBS_DOWN;
	}
}

// This is used so that the Key States will not be changed during execution of your Main game loop.
// Place this at the very START of your Main Loop
void CIrrEventReceiver::endEventProcess()
{
	procesState = ENDED;
}

bool CIrrEventReceiver::OnEvent(const SEvent& event)
{
	switch (event.EventType) 
	{
		case EET_KEY_INPUT_EVENT:
		{
			if(procesState == STARTED)
			{
				if(event.KeyInput.PressedDown)
				{
					// If key was not down before
					if(Keys[event.KeyInput.Key] != BS_DOWN)
						Keys[event.KeyInput.Key] = BS_PRESSED; // Set key to Pressed
					else
						Keys[event.KeyInput.Key] = BS_DOWN;
				}
				
				else
				{
					// if the key is down
					if(Keys[event.KeyInput.Key] != BS_UP)    
						Keys[event.KeyInput.Key] = BS_RELEASED; // Set key to Released
					else
						Keys[event.KeyInput.Key] = BS_UP;
				}
			}
			
			break;
		}
		
		case EET_MOUSE_INPUT_EVENT:
		{
			//if(procesState == STARTED)
			{
				switch(event.MouseInput.Event)
				{
					case EMIE_MOUSE_MOVED:
					{
						deltaMouseX = event.MouseInput.X - MouseData.lx;
						deltaMouseY = event.MouseInput.Y - MouseData.ly;

						MouseData.x = event.MouseInput.X;
						MouseData.y = event.MouseInput.Y;

						MouseData.lx = MouseData.x;
						MouseData.ly = MouseData.y;
						
						mouseHasMoved = true;

						break;
					}
					
					case EMIE_MOUSE_WHEEL:
					{
						MouseData.lwheelPos = MouseData.wheelPos;
						MouseData.wheelPos += event.MouseInput.Wheel;
					
						break;
					}
					
					case EMIE_LMOUSE_PRESSED_DOWN:
					{
						Mouse[MB_LEFT] = MBS_DOWN;
						MouseData.cx = event.MouseInput.X;
						MouseData.cy = event.MouseInput.Y;
						
						break;
					}
					
					case EMIE_LMOUSE_LEFT_UP:
					{
						Mouse[MB_LEFT] = MBS_UP;
						
						break;
					}
					
					case EMIE_MMOUSE_PRESSED_DOWN:
					{
						Mouse[MB_MIDDLE] = MBS_DOWN;
						MouseData.cx = event.MouseInput.X;
						MouseData.cy = event.MouseInput.Y;
						
						break;
					}
					
					case EMIE_MMOUSE_LEFT_UP:
					{
						Mouse[MB_MIDDLE] = MBS_UP;
						
						break;
					}
					
					case EMIE_RMOUSE_PRESSED_DOWN:
					{
						Mouse[MB_RIGHT] = MBS_DOWN;
						MouseData.cx = event.MouseInput.X;
						MouseData.cy = event.MouseInput.Y;
						
						break;
					}
					
					case EMIE_RMOUSE_LEFT_UP:
					{
						Mouse[MB_RIGHT] = MBS_UP;
						
						break;
					}
				}
			}
		}

		case EET_GUI_EVENT:
		{
			switch(event.GUIEvent.EventType)
			{
				case EGET_BUTTON_CLICKED:
				{
					if(event.GUIEvent.Caller->getID() == 103) // you might wanna chance 103 to your own button ID.
						btnOnePressed = true;
					
					if(event.GUIEvent.Caller->getID() == 104) // you might wanna chance 104 to your own button ID.
						btnTwoPressed = true;

					if(event.GUIEvent.Caller->getID() == 105) // you might wanna chance 105 to your own button ID.
						btnThreePressed = true;
				}
			}
		}

		break;
	}
	
	return false;
}

