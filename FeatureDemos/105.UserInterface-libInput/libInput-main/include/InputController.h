/*
 * InputController.h
 *
 *  Created on: 2 Dec 2021
 *      Author: ykhan
 */

#ifndef INPUTCONTROLLER_H_
#define INPUTCONTROLLER_H_

#include "InputDevice.h"
#include "Keyboard.h"
#include "Mouse.h"
#include <vector>

class InputController;

using InputControllerPtr = std::shared_ptr<InputController>;

class InputController {
protected:
	std::vector<InputDevicePtr> m_devices;

public:
	virtual bool initialize() = 0;
	virtual void enumerate() = 0;
	std::vector<KeyboardPtr> getKeyboards();
	std::vector<MousePtr> getMice();
	KeyboardPtr getKeyboardById(int id);
	MousePtr getMouseById(int id);
};

#endif /* INPUTCONTROLLER_H_ */
