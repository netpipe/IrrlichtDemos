/*
 * Input.h
 *
 *  Created on: 13-12-2011
 *      Author: Marcin Załuski
 */

#ifndef Input_H_
#define Input_H_
#include <irrlicht.h>

class Input: public irr::IEventReceiver {
private:
	bool pressed[irr::KEY_KEY_CODES_COUNT];
	float wheel;
public:
	Input();
	virtual ~Input();
	bool OnEvent(const irr::SEvent &event);
	bool isPressed(irr::EKEY_CODE key) const;
	float getWheel();
	void unpress(irr::EKEY_CODE key);
};

#endif /* Input_H_ */
