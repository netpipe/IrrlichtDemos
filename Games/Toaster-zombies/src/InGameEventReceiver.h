#ifndef _IN_GAME_EVENTS_H
#define _IN_GAME_EVENTS_H 1

#include <irrlicht.h>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

class InGameEventReceiver : public IEventReceiver
{
 	// Public variables.
	public:
		bool Quit;

	// Private Variables
	private:
    bool keys[KEY_KEY_CODES_COUNT];

	// Public functions.
	public:
    InGameEventReceiver();

    void Init(void);

		virtual bool OnEvent ( const SEvent &event );

		bool getKeyState(EKEY_CODE key);

		// Mouse coordinates
		unsigned short mousex;
		unsigned short mousey;

		// Mouse click coords
//		std::queue<position2d<s32> > lmouseClicks;
//		std::queue<position2d<s32> > rmouseClicks;

		// Mouse button usage
		bool lmouse;
		bool rmouse;
  bool LEFTBUTTONCLICKED;
  bool RIGHTBUTTONCLICKED;

	// Private functions.
	private:
};

#endif
