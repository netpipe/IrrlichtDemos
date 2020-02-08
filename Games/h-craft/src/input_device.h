// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#ifndef INPUT_DEVICE_H
#define INPUT_DEVICE_H

#include <string>

enum InputDeviceType
{
    IDP_NONE,
    IDP_KEYBOARD,
    IDP_MOUSE,
    IDP_JOYSTICK,
    IDP_TOUCH,

	// Add also to g_InputDeviceTypeXmlNames in input_device.cpp when adding here.

    IDP_MAX
};

class IInputDevice
{
public:
    IInputDevice() : mIsEnabled(true)   {};
    virtual ~IInputDevice() {}

    virtual std::string GetTypeName() const = 0;        // like mouse, keyboard, joystick 1
    virtual std::string GetDeviceName() const = 0;      // as delivered by the system
    virtual InputDeviceType GetType() const = 0;

    virtual void Update() = 0;

    // Some devices can have state information
    virtual void Reset() {}

    // returns the max number axis supported
    virtual int GetNumAxes() const = 0;

    // returns the max number buttons supported
    virtual int GetNumButtons() const = 0;

    virtual std::string GetNameAxis(int axis_) const = 0;
    virtual std::string GetNameButton(int button_) const = 0;

    virtual bool IsAnyButtonDown(int & button_) const = 0;
    virtual float IsAnyAxisMoved(int & axis_) const = 0;    // returns the amount of moving

    virtual bool IsButtonDown(int button_) const = 0;
    virtual float GetAxisValue(int axis_) const = 0;  // between -1 and 1

    static const char* GetXmlNameForDeviceType(InputDeviceType type_);
    static InputDeviceType GetDeviceTypeForXmlName(const char *name_);

    void SetEnabled(bool enabled_)      { mIsEnabled = enabled_; }
    bool IsEnabled() const              { return mIsEnabled; }

private:
    bool    mIsEnabled;
};

#endif // INPUT_DEVICE_H
