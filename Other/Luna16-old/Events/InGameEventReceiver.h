#ifndef _IN_GAME_EVENTS_H
#define _IN_GAME_EVENTS_H 1

#include <irrlicht.h>
#include "../Physics/Physics.h"
#include "../Physics/Vehicle.h"

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

	// Private functions.
	private:
};

#endif
