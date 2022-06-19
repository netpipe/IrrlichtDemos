/*
 * X11Keyboard.h
 *
 *  Created on: 6 Dec 2021
 *      Author: ykhan
 */

#ifndef X11_X11KEYBOARD_H_
#define X11_X11KEYBOARD_H_

#include "X11InputDevice.h"
#include "X11InputController.h"
#include <Keyboard.h>

class X11Keyboard;
using X11KeyboardPtr = std::shared_ptr<X11Keyboard>;

class X11Keyboard: public X11InputDevice, public Keyboard {
private:
	X11InputController* m_controller;
public:
	X11Keyboard(XIDeviceInfo* device_info, X11InputController* controller);
	virtual KeyState getkeystate(int key);
	virtual KeyboardStatePtr getKeyboardState() override;
};

#endif /* X11_X11KEYBOARD_H_ */
