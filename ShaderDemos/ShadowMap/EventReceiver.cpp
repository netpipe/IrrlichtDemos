#include "EventReceiver.h"

bool EventReceiver::OnEvent(const SEvent & event)
{
	bool eventprocessed = false;

	// Keyboard Input Event
	if (event.EventType == EET_KEY_INPUT_EVENT)
	{
		if (processState == STARTED)
		{
			// if key is Pressed Down
			if (event.KeyInput.PressedDown == true)
			{
				// If key was not down before
				if (keyState[event.KeyInput.Key] != DOWN)
					keyState[event.KeyInput.Key] = PRESSED;
				else
					keyState[event.KeyInput.Key] = DOWN;
			}
			else
			{
				// if the key is down
				if (keyState[event.KeyInput.Key] != UP)
					keyState[event.KeyInput.Key] = RELEASED;
			}
		}

		eventprocessed = true;
	}

	// Mouse Input Event
	if (event.EventType == EET_MOUSE_INPUT_EVENT)
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
			if (event.MouseInput.Event == EMIE_MOUSE_WHEEL)
				mouse.wheel += event.MouseInput.Wheel;

			//Left Mouse Button Pressed
			if (event.MouseInput.Event == EMIE_LMOUSE_PRESSED_DOWN)
			{
				if (mouseButtonState[0] == UP || mouseButtonState[0] == RELEASED)
					mouseButtonState[0] = PRESSED;
				else
					mouseButtonState[0] = DOWN;
			}

			//Left Mouse Button Released
			if (event.MouseInput.Event == EMIE_LMOUSE_LEFT_UP)
			{
				if (mouseButtonState[0] != UP)
					mouseButtonState[0] = RELEASED;
			}

			//Middle Mouse Button Pressed
			if (event.MouseInput.Event == EMIE_MMOUSE_PRESSED_DOWN)
			{
				if (mouseButtonState[1] == UP || mouseButtonState[1] == RELEASED)
					mouseButtonState[1] = PRESSED;
				else
					mouseButtonState[1] = DOWN;
			}

			//Middle Mouse Button Rleased
			if (event.MouseInput.Event == EMIE_MMOUSE_LEFT_UP)
				if (mouseButtonState[1] != UP)
					mouseButtonState[1] = RELEASED;

			//Right Mouse Button Pressed
			if (event.MouseInput.Event == EMIE_RMOUSE_PRESSED_DOWN)
			{
				if (mouseButtonState[2] == UP || mouseButtonState[2] == RELEASED)
					mouseButtonState[2] = PRESSED;
				else
					mouseButtonState[2] = DOWN;
			}

				//Right Mouse Button Rleased
			if (event.MouseInput.Event == EMIE_RMOUSE_LEFT_UP)
				if (mouseButtonState[2] != UP)
					mouseButtonState[2] = RELEASED;
		}

		eventprocessed = true;
	}

	return false;
}

float EventReceiver::MouseWheel()
{
	return mouse.wheel;
}

int EventReceiver::MouseX()
{
	return mouse.X;
}

int EventReceiver::MouseY()
{
	return mouse.Y;
}

bool EventReceiver::MouseReleased(int button)
{
	if (mouseButtonState[button] == RELEASED)
		return true;
	else
		return false;
}

bool EventReceiver::MouseUp(int button)
{
	if (mouseButtonState[button] == RELEASED || mouseButtonState[button] == UP)
		return true;
	else
		return false;
}

bool EventReceiver::MousePressed(int button)
{
	if (mouseButtonState[button] == PRESSED)
		return true;
	else
		return false;
}

bool EventReceiver::MouseDown(int button)
{
	if (mouseButtonState[button] == PRESSED || mouseButtonState[button] == DOWN)
		return true;
	else
		return false;
}

bool EventReceiver::keyPressed(int keycode)
{
	if (keyState[keycode] == PRESSED)
		return true;
	else
		return false;
}

bool EventReceiver::keyDown(int keycode)
{
	if (keyState[keycode] == DOWN || keyState[keycode] == PRESSED)
		return true;
	else
		return false;
}

bool EventReceiver::keyUp(int keycode)
{
	if (keyState[keycode] == UP || keyState[keycode] == RELEASED)
		return true;
	else
		return false;
}

bool EventReceiver::keyReleased(int keycode)
{
	if (keyState[keycode] == RELEASED)
		return true;
	else
		return false;
}

// This is used so that the Key States will not be changed during execution of your Main game loop.
// Place this at the very START of your Main Loop
void EventReceiver::endEventProcess()
{
	processState = ENDED;
}

// This is used so that the Key States will not be changed during execution of your Main game loop.
// Place this function at the END of your Main Loop.
void EventReceiver::startEventProcess()
{
	processState = STARTED;

	//Keyboard Key States
	for (int i = 0; i < KEY_KEY_CODES_COUNT; i++)
	{
		if (keyState[i] == RELEASED)
			keyState[i] = UP;

		if (keyState[i] == PRESSED)
			keyState[i] = DOWN;
	}

	//Mouse Button States
	for (int i = 0; i <= 2; i++)
	{
		if (mouseButtonState[i] == RELEASED)
			mouseButtonState[i] = UP;

		if (mouseButtonState[i] == PRESSED)
			mouseButtonState[i] = DOWN;
	}

	//Mouse Wheel state
	mouse.wheel = 0.0f;
}

void EventReceiver::init()
{
	//KeyBoard States.
	for (int i = 0; i <= KEY_KEY_CODES_COUNT; i++)
		keyState[i] = UP;

	//Mouse states
	for (int i = 0; i <= 2; i++)
		mouseButtonState[i] = UP;

	//Mouse X/Y coordenates.
	mouse.X = 0;
	mouse.Y = 0;
	mouse.wheel = 0.0f;
}