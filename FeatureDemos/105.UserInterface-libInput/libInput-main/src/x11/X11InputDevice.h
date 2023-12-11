/*
 * X11InputDevice.h
 *
 *  Created on: 2 Dec 2021
 *      Author: ykhan
 */

#ifndef X11_X11INPUTDEVICE_H_
#define X11_X11INPUTDEVICE_H_

#include <InputDevice.h>
#include <X11/extensions/XInput2.h>

class X11InputDevice;

using XIDeviceInfoPtr = std::shared_ptr<XIDeviceInfo>;
using X11InputDevicePtr = std::shared_ptr<X11InputDevice>;

class X11InputDevice: virtual public InputDevice {
private:
	XIDeviceInfo* m_device_info;
	bool m_master;

public:
	X11InputDevice(XIDeviceInfo* device_info);
	virtual InputDevice::DeviceType deviceType() override;
	int deviceId() override;
	virtual bool isAttached() override;
	virtual bool isMaster() override;
	void setAsMaster(bool master);
	virtual char* name() override;
};

#endif /* X11_X11INPUTDEVICE_H_ */
