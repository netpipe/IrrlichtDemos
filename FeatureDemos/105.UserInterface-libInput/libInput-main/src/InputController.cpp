/*
 * InputController.cpp
 *
 *  Created on: 2 Dec 2021
 *      Author: ykhan
 */

#include "InputController.h"

using namespace std;

std::vector<KeyboardPtr> InputController::getKeyboards() {
	vector<KeyboardPtr> keyboards;

	for (const auto &device : m_devices) {
		if (device->deviceType() == InputDevice::DeviceType::KEYBOARD) {

			auto keyboard = dynamic_pointer_cast<Keyboard>(device);
			keyboards.push_back(keyboard);
		}
	}

	return keyboards;
}

std::vector<MousePtr> InputController::getMice() {
	vector<MousePtr> mice;

	for (const auto &device : m_devices) {
		if (device->deviceType() == InputDevice::DeviceType::MOUSE) {
			auto mouse = dynamic_pointer_cast<Mouse>(device);
			mice.push_back(mouse);
		}
	}

	return mice;
}

KeyboardPtr InputController::getKeyboardById(int id) {
	for (const auto &device : m_devices) {
		if (device->deviceType() == InputDevice::DeviceType::KEYBOARD
			&& device->deviceId() == id) {
			return dynamic_pointer_cast<Keyboard>(device);
		}
	}

	return nullptr;
}

MousePtr InputController::getMouseById(int id) {
	for (const auto &device : m_devices) {
		if (device->deviceType() == InputDevice::DeviceType::MOUSE
			&& device->deviceId() == id) {
			return dynamic_pointer_cast<Mouse>(device);
		}
	}

	return nullptr;
}
