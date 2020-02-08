// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#ifndef DEVICE_KEYBOARD_H
#define DEVICE_KEYBOARD_H

#include "input_device.h"

#define NUM_KEYS_KEYBOARD 512

class DeviceKeyboard : public IInputDevice
{
public:
    DeviceKeyboard();
    virtual ~DeviceKeyboard();

    virtual std::string GetTypeName() const;
    virtual std::string GetDeviceName() const;
    virtual InputDeviceType GetType() const;

    virtual void Update();

    virtual int GetNumAxes() const;
    virtual int GetNumButtons() const;

    virtual std::string GetNameAxis(int axis_) const;
    virtual std::string GetNameButton(int button_) const;

    virtual bool IsAnyButtonDown(int & button_) const;
    virtual float IsAnyAxisMoved(int & axis_) const;

    virtual bool IsButtonDown(int button_) const;
    virtual float GetAxisValue(int axis_) const;  // between -1 and 1


    void Reset();
    void SetKeyDown(int key_);
    void SetKeyUp(int key_);

private:
    int mKeyState[NUM_KEYS_KEYBOARD];
};

#endif // DEVICE_KEYBOARD_H
