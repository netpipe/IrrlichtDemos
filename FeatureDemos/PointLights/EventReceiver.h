#include "main.h"

#ifndef __CEVENTRECEIVER__
#define __CEVENTRECEIVER__

class EventReceiver : public IEventReceiver
{
protected:

	// Enumeration for UP, DOWN, PRESSED and RELEASED key states.
	enum keyStatesENUM
	{
		UP,
		DOWN,
		PRESSED,
		RELEASED
	};

	// Enumeration for Event Handling State.
	enum processStateENUM
	{
		STARTED,
		ENDED
	};

	// Mouse button states.
	keyStatesENUM mouseButtonState[2];

	// Keyboard key states.
	keyStatesENUM keyState[KEY_KEY_CODES_COUNT];

	// Mouse X/Y coordinates and Wheel data.
	struct mouseData
	{
		s32 X;
		s32 Y;
		f32 wheel;
	};

	struct mouseData mouse;

	// STARTED = handling events, ENDED = not handling events.
	processStateENUM processState;

	virtual bool OnEvent(const SEvent & event);

public:

	float MouseWheel();

	int MouseX();
	int MouseY();

	bool MouseReleased(int button);
	bool MouseUp(int button);
	bool MousePressed(int button);
	bool MouseDown(int button);

	bool keyPressed(int keycode);
	bool keyDown(int keycode);
	bool keyUp(int keycode);
	bool keyReleased(int keycode);

	void endEventProcess();
	void startEventProcess();
	void init();

};

#endif