/*
 * Mouse.h
 *
 *  Created on: 7 Dec 2021
 *      Author: ykhan
 */

#ifndef MOUSE_H_
#define MOUSE_H_

#include "InputDevice.h"

class Mouse;

using MousePtr = std::shared_ptr<Mouse>;

struct MouseState {
	long x;
	long y;
	bool btn1_pressed;
	bool btn2_pressed;
	bool btn3_pressed;

	MouseState() : x{0}, y{0}, btn1_pressed{false}, btn2_pressed{false}, btn3_pressed{false} {}
};

using MouseStatePtr = std::shared_ptr<MouseState>;

class Mouse: virtual public InputDevice {
public:
	enum Axis {
		x,
		y
	};

	virtual MouseStatePtr getMouseState() = 0;
	virtual double MinimumX() = 0;
	virtual double MaximumX() = 0;
	virtual double MinimumY() = 0;
	virtual double MaximumY() = 0;
};

#endif /* MOUSE_H_ */
