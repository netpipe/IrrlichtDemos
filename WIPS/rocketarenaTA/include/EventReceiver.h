/*

Event Receiver - header file

*/

#ifndef __EventReceiver_h
#define __EventReceiver_h

#include <irrlicht.h>

using namespace std;

using namespace irr;


class cEventReceiver : public IEventReceiver
{
public:

	// constructor
	cEventReceiver();

	// destructor
	~cEventReceiver();

	bool OnEvent(const SEvent& event);
	
	bool checkKey(irr::EKEY_CODE i);

	bool Keys[KEY_KEY_CODES_COUNT];
	bool KeysPressed[KEY_KEY_CODES_COUNT];
	
	s32 mouseX;
	s32 mouseY;
	
	irr::EMOUSE_INPUT_EVENT mouseInput;
  
private:
	bool KeysTemp[KEY_KEY_CODES_COUNT];

};

#endif
