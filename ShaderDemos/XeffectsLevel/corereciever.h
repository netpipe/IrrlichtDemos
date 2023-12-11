#ifndef __CORE_RECEIVER_H_INCLUDED__
#define __CORE_RECEIVER_H_INCLUDED__

#include <irrlicht.h>


class CoreEventReciever : public irr::IEventReceiver, public irr::IReferenceCounted
{
public:

	virtual bool OnEvent(const irr::SEvent& event)
	{
		if(event.EventType == irr::EET_KEY_INPUT_EVENT) {
			this->keys[event.KeyInput.Key] = event.KeyInput.PressedDown;
		}
		else if(event.EventType == irr::EET_LOG_TEXT_EVENT) {
			printf("%s\n", event.LogEvent.Text);
		}

		return false;
	}

	virtual bool keyDown(irr::EKEY_CODE code)
	{
		return this->keys[code];
	}

	CoreEventReciever(irr::IrrlichtDevice * device)
	{
		this->device = device;

		for(irr::u32 i = 0; i < irr::KEY_KEY_CODES_COUNT; i++)
			this->keys[i] = false;
	}

	//virtual ~CoreEventReceiver() { }

private:

	bool keys[irr::KEY_KEY_CODES_COUNT];

	irr::IrrlichtDevice * device;
};



#endif
