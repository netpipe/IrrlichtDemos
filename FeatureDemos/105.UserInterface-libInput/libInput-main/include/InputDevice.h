/*
 * InputDevice.h
 *
 *  Created on: 2 Dec 2021
 *      Author: ykhan
 */

#ifndef INPUTDEVICE_H_
#define INPUTDEVICE_H_

#include <memory>

class InputDevice;

using InputDevicePtr = std::shared_ptr<InputDevice>;

class InputDevice {

public:
	enum DeviceType {
		KEYBOARD,
		MOUSE
	};

	virtual int deviceId() = 0;
	virtual DeviceType deviceType() = 0;
	virtual bool isAttached() = 0;
	virtual bool isMaster() = 0;
	virtual char* name() = 0;

protected:
	DeviceType m_device_type;
};

#endif /* INPUTDEVICE_H_ */
