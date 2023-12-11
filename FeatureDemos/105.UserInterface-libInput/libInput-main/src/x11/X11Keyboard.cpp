/*
 * X11Keyboard.cpp
 *
 *  Created on: 6 Dec 2021
 *      Author: ykhan
 */

#include "X11Keyboard.h"
#include <cstdio>

using namespace std;

X11Keyboard::X11Keyboard(XIDeviceInfo* device_info, X11InputController* controller)
	: X11InputDevice{device_info}, m_controller{controller} {
	// TODO Auto-generated constructor stub
	m_device_type = DeviceType::KEYBOARD;

}

KeyboardStatePtr X11Keyboard::getKeyboardState() {
	XDeviceState* device_state = m_controller->getXDeviceState(deviceId());

	if (!device_state) {
		printf("Failed to get X11 device state\n");
		return nullptr;
	}

	KeyboardStatePtr keyboard_state = KeyboardStatePtr(new KeyboardState);
	XInputClass* cls;
	cls = device_state->data;

	for (int i = 0; i < device_state->num_classes; i++) {
		switch (cls->c_class) {
		case KeyClass: {
			auto key_state = (XKeyState*) cls;
			for (int i = 1; i < key_state->num_keys; i++) {
				keyboard_state->keys_pressed[i] = key_state->keys[i / 8] & (1 << (i % 8));
			}
			break;
		}

		}
		cls = (XInputClass*) ((char*) cls + cls->length);
	}

	//	MouseState state = m_controller->getMouseState();
	XFreeDeviceState(device_state);

	return keyboard_state;

}

X11Keyboard::KeyState X11Keyboard::getkeystate(int key) {
	char keys_pressed[32]; // 256 keys
	int key_code;

	m_controller->getKeymap(keys_pressed);
	for (int i = 0; i < sizeof(keys_pressed); i++) {
		if (keys_pressed[i] != 0) {
			int starting_key_code = i * 8;
			for (int j = 0; j < 8; j++) {
				if ((0x1 & keys_pressed[i] >> j)) {
					key_code = starting_key_code + j;
					char key_sym = m_controller->getKeysym(key_code);
					printf("Pressed key: %d\n", key_code);
					printf("Pressed key sym: %c\n", key_sym);
					if (key_sym == key) {
						return KeyState::Pressed;
					}
				}
			}
		}
	}

	return KeyState::Released;

}
