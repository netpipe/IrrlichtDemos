/*
 * X11InputDevice.cpp
 *
 *  Created on: 2 Dec 2021
 *      Author: ykhan
 */

#include "X11InputDevice.h"

X11InputDevice::X11InputDevice(XIDeviceInfo* device_info)
	: m_device_info{device_info} {
}

InputDevice::DeviceType X11InputDevice::deviceType() {
	return m_device_type;
}

int X11InputDevice::deviceId() {
	return m_device_info->deviceid;
}

bool X11InputDevice::isMaster() {
	return m_master;
}

bool X11InputDevice::isAttached() {
	return m_master;
}

void X11InputDevice::setAsMaster(bool master) {
	m_master = master;
}

char* X11InputDevice::name() {
	return m_device_info->name;
}
