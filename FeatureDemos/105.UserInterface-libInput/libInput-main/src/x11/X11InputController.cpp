/*
 * X11InputController.cpp
 *
 *  Created on: 2 Dec 2021
 *      Author: ykhan
 */

#include "X11InputController.h"
#include "X11Keyboard.h"
#include "X11Mouse.h"
#include <X11/X.h>

#include <X11/extensions/XInput2.h>
#include <cstdio>

X11InputController::X11InputController(Display* display, Window* window)
	: m_display{display}, m_window{window} {

}

void X11InputController::enumerate() {
	int ndevices;
	XIDeviceInfo *device;
	m_xi_devices = XIDeviceInfoPtr(XIQueryDevice(m_display, XIAllDevices, &ndevices), XIFreeDeviceInfo);

	for (int i = 0; i < ndevices; i++) {
		device = &m_xi_devices.get()[i];
//	    printf("Device %s (id: %d) is a ", device->name, device->deviceid);
//
//	    switch(device->use) {
//	    case XIMasterPointer: printf("master pointer\n"); break;
//	    case XIMasterKeyboard: printf("master keyboard\n"); break;
//	    case XISlavePointer: printf("slave pointer\n"); break;
//	    case XISlaveKeyboard: printf("slave keyboard\n"); break;
//	    case XIFloatingSlave: printf("floating slave\n"); break;
//	    }
//
//	    printf("Device is attached to/paired with %d\n", device->attachment);

	    X11InputDevicePtr x11_device_ptr = createX11InputDevice(device);
	    if (!x11_device_ptr) {
	    	continue;
	    }

	    m_devices.push_back(x11_device_ptr);
	}
}

bool X11InputController::initialize() {
	int opcode, first_event, error;

	if (!XQueryExtension(m_display, "XInputExtension", &opcode, &first_event, &error)) {
		printf("XInputExtension not available\n");
		return false;
	}

	if (XIQueryVersion(m_display, &m_major, &m_minor) == BadRequest) {
		printf("XI2 not available, the X server supports %d.%d\n", m_major, m_minor);
		return false;
	}

	XIEventMask eventmask;
	unsigned char mask[1] = {0};
	eventmask.deviceid = 3;
	eventmask.mask_len = sizeof(mask);
	eventmask.mask = mask;
	XISetMask(mask, XI_ButtonPress);
	XISetMask(mask, XI_Motion);
	XISetMask(mask, XI_KeyPress);

	XISelectEvents(m_display, *m_window, &eventmask, 1);

	return true;
}

X11InputDevicePtr X11InputController::createX11InputDevice(XIDeviceInfo* device_info) {
	X11InputDevicePtr device{nullptr};

	switch (device_info->use) {
	case XIMasterKeyboard:
		device = X11KeyboardPtr{new X11Keyboard(device_info, this)};
		device->setAsMaster(true);
		m_master_keyboard_id = device->deviceId();
		break;
	case XISlaveKeyboard:
		device = X11KeyboardPtr{new X11Keyboard(device_info, this)};
		device->setAsMaster(false);
		break;
	case XIMasterPointer:
		device = X11MousePtr{new X11Mouse(device_info, this)};
		device->setAsMaster(true);
		m_master_pointer_id = device->deviceId();
		break;
	case XISlavePointer:
		device = X11MousePtr{new X11Mouse(device_info, this)};
		device->setAsMaster(false);
		break;
	}

	return device;
}

void X11InputController::getKeymap(char keymap[32]) {
	 XQueryKeymap(m_display, keymap);
}

char X11InputController::getKeysym(char key_code) {
	return XKeycodeToKeysym(m_display, key_code, 0);
}

XDeviceState* X11InputController::getXDeviceState(int device_id) {
	XDevice* device = XOpenDevice(m_display, device_id);
	XDeviceState* state{nullptr};

	if (!device) {

		printf("Failed to open Xinput device with id: %d\n", device_id);
		return state;
	}

	state = XQueryDeviceState(m_display, device);
	XCloseDevice(m_display, device);
	return state;
}

MouseState X11InputController::getMouseState() {
	Window root_win;
	Window child_win;
	int root_x;
	int root_y;
	int win_x;
	int win_y;
	unsigned int btn_mask;

	MouseState state;

	XQueryPointer(m_display, *m_window, &root_win, &child_win,
				  &root_x, &root_y, &win_x, &win_y, &btn_mask);

//	printf("Requested window: %d, root_win: %d, child_win: %d \
//			root_x: %d, root_y: %d, child_x: %d, child_y: %d, btn_mask: %04x\n",
//		   *m_window, root_win, child_win, root_x, root_y, win_x, win_y, btn_mask);

	state.x = win_x;
	state.y = win_y;
	state.btn1_pressed = btn_mask & Button1Mask;
	state.btn2_pressed = btn_mask & Button2Mask;
	state.btn3_pressed = btn_mask & Button3Mask;

	return state;
}

int X11InputController::detachDevice(InputDevicePtr device) {
	detachDeviceById(device->deviceId());
}

int X11InputController::detachDeviceById(int device_id) {
	// see https://www.x.org/releases/X11R7.5/doc/man/man3/XIChangeHierarchy.3.html
	// & https://github.com/freedesktop/xorg-xinput
	// & https://wiki.archlinux.org/title/Multi-pointer_X
	XIAnyHierarchyChangeInfo change;

	change.detach.type = XIDetachSlave;
	change.detach.deviceid = device_id;

	return XIChangeHierarchy(m_display, &change, 1);
}

int X11InputController::attachDevice(InputDevicePtr device) {
	switch (device->deviceType()) {
	case InputDevice::DeviceType::MOUSE:
		attachDeviceById(device->deviceId(), true);
		break;
	case InputDevice::DeviceType::KEYBOARD:
		attachDeviceById(device->deviceId(), false);
		break;
	default:
		printf("ERROR: cannot attach invalid device\n");
	}
}

int X11InputController::attachDeviceById(int device_id, bool is_pointer) {
	// see https://www.x.org/releases/X11R7.5/doc/man/man3/XIChangeHierarchy.3.html
	// & https://github.com/freedesktop/xorg-xinput
	// & https://wiki.archlinux.org/title/Multi-pointer_X
	XIAnyHierarchyChangeInfo change;

	change.attach.type = XIAttachSlave;
	change.attach.deviceid = device_id;
	if (is_pointer) {
		change.attach.new_master = m_master_pointer_id;
		printf("Attaching pointer: %d to master device: %d\n", change.attach.deviceid, change.attach.new_master);
	} else {
		change.attach.new_master = m_master_keyboard_id;
		printf("Attaching keyboard: %d to master device: %d\n", change.attach.deviceid, change.attach.new_master);
	}

	return XIChangeHierarchy(m_display, &change, 1);
}

Display* X11InputController::display() {
	return m_display;
}
