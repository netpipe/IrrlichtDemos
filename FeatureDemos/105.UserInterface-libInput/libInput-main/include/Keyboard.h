/*
 * Keyboard.h
 *
 *  Created on: 6 Dec 2021
 *      Author: ykhan
 */

#ifndef KEYBOARD_H_
#define KEYBOARD_H_

#include "InputDevice.h"

class Keyboard;
using KeyboardPtr = std::shared_ptr<Keyboard>;

struct KeyboardState {
	bool keys_pressed[256];

	KeyboardState() {
		for (int i = 0; i < sizeof(keys_pressed); i++) {
			keys_pressed[i] = false;
		}
	}
};

using KeyboardStatePtr = std::shared_ptr<KeyboardState>;

class Keyboard : virtual public InputDevice {
public:
	enum KeyState{
			Pressed,
			Released
	};

	virtual KeyState getkeystate(int key) = 0;
	virtual KeyboardStatePtr getKeyboardState() = 0;
};

#endif /* KEYBOARD_H_ */
