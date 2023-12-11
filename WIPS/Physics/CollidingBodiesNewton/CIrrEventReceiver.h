/******************************************************************************
 * CIrrEventReceiver
 * =================
 *
 * CIrrEventReceiver has no restrictions. Credit would be appreciated, but not
 * required.
 ******************************************************************************/
 
#ifndef __CIRREVENTRECEIVER_H__
#define __CIRREVENTRECEIVER_H__

#include <Irrlicht.h>
#include <iostream>
#include <sstream>

using namespace irr;
using namespace core;
using namespace gui;
using namespace io;
using namespace std;

/* I doubt the number of keys will go above 255 ;), but it is there for compatibility issues */
#define NUMBER_OF_KEYS KEY_KEY_CODES_COUNT
#define NUMBER_OF_MOUSE_BUTTONS 3

enum buttonState 
{
	BS_UP,
	BS_DOWN,
	BS_PRESSED,
	BS_RELEASED
};

enum mouseButtonState 
{
	MBS_UP,
	MBS_DOWN,
	MBS_PRESSED,
	MBS_RELEASED
};

enum mouseButton
{
	MB_LEFT,
	MB_MIDDLE,
	MB_RIGHT
};

struct mouseInformation 
{
	signed int x, y, lx, ly, cx, cy;
	f32 wheelPos, lwheelPos;
};

// Enumeration for Event Handling State.
enum ProcessEventState
{
	STARTED, 
	ENDED
};

class CIrrEventReceiver : public IEventReceiver
{
	public:
		CIrrEventReceiver();
		~CIrrEventReceiver();
		
		bool isButtonOnePressed();
		bool isButtonTwoPressed();
		bool isButtonThreePressed();
		
		void resetButtonOnePressed();
		void resetButtonTwoPressed();
		void resetButtonThreePressed();

		bool isKeyUp(EKEY_CODE key);
		bool isKeyDown(EKEY_CODE key);
		bool isKeyPressed(EKEY_CODE key);
		bool isKeyReleased(EKEY_CODE key);
		
		bool isMouseButtonPressed(mouseButton mb);
		bool isMouseButtonReleased(mouseButton mb);

		bool isMouseButtonDown(mouseButton mb);
		bool isMouseButtonUp(mouseButton mb);

		bool mouseMoved();
		
		void resetMouseButtonState();

		void startEventProcess();
		void endEventProcess();

		int getDeltaMousePosX();
		int getDeltaMousePosY();

		inline int getMouseX() { return MouseData.x; }
		inline int getMouseY() { return MouseData.y; }
		inline int getLastMouseX() { return MouseData.lx; }
		inline int getLastMouseY() { return MouseData.ly; }

		inline s32 getDeltaMouseX()
		{
			s32 a = MouseData.x - MouseData.lx;
			//u32 b = a < 0 ? -a : a;
			return (s32)(a < 0 ? -a : a);
		}

		inline s32 getDeltaMouseY()
		{
			s32 a = MouseData.y - MouseData.ly;
			//u32 b = a < 0 ? -a : a;
			return (s32)(a < 0 ? -a : a); 
		}

		inline u32 getClickedMouseX() { return MouseData.cx; }
		inline u32 getClickedMouseY() { return MouseData.cy; }

		inline f32 getMouseWheelPosition() { return MouseData.wheelPos; }
		inline f32 getLastMouseWheelPosition() { return MouseData.lwheelPos; }

		inline f32 getDeltaMouseWheelPosition()
		{
			f32 a = MouseData.wheelPos - MouseData.lwheelPos;
			return (f32)(a < 0 ? -a : a);
		}

		bool OnEvent(const SEvent& event);

	protected:
		buttonState Keys[NUMBER_OF_KEYS];
		mouseButtonState Mouse[NUMBER_OF_MOUSE_BUTTONS];
		mouseInformation MouseData;
		ProcessEventState procesState;
		
		bool mouseHasMoved;
		bool btnOnePressed;
		bool btnTwoPressed;
		bool btnThreePressed;
		 
		int deltaMouseX;
		int deltaMouseY;
		
		// for converting anything to string (good for debug to console).
		template <class T>
		inline std::string ToString(const T& t)
		{
			std::stringstream ss;
			ss << t;
			return ss.str();
		}
};

#endif /* __CIRREVENTRECEIVER_HEADER__ */
