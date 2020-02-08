// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#ifndef INPUT_DEVICE_MANAGER_H
#define INPUT_DEVICE_MANAGER_H

#include <vector>
#include <string>
#include "input_device.h"

class DeviceKeyboard;
class DeviceMouse;
class IDeviceJoystick;
class DeviceTouch;
class ControllerAxis;
class ControllerButton;
class TiXmlElement;
class Config;

namespace irr
{
    struct SEvent;

    namespace gui
    {
		class ICursorControl;
    }
}

class InputDeviceManager
{
public:
    InputDeviceManager(const Config& config);
    ~InputDeviceManager();

    void CreateAllDevices(irr::gui::ICursorControl * cursorControl);
    void Update();

    bool OnEvent(const irr::SEvent &event);

    DeviceKeyboard* GetKeyboard() const { return mKeyboard; }
    DeviceMouse* GetMouse() const       { return mMouse; }
    DeviceTouch* GetTouch() const       { return mTouch; }

    size_t GetNumJoysticks() const;
    IDeviceJoystick* GetJoystick(size_t index_) const;

    IInputDevice* GetDevice(InputDeviceType type_, int joystickId_=0) const;

    bool WaitForAxis( ControllerAxis &controllerAxis_, bool up_ );
    bool WaitForButton( ControllerButton &controllerButton_ );
    bool GetNameForButton(const ControllerButton &button_, std::wstring &str_, bool withDevice_=false) const;
    bool GetNameForAxis(const ControllerAxis &axis_, bool up_, std::wstring &str_, bool withDevice_=false) const;

    float CheckForAnyAxis( ControllerAxis &controllerAxis_, float minVal_ = 0.f);
    bool CheckForAnyButton( ControllerButton &controllerButton_ );
    float GetAxisValue(const ControllerAxis &axis_, float oldValue_) const;
    bool GetButtonDown(const ControllerButton &button_) const;

    void ReadFromXml(const TiXmlElement * settings_);
    void WriteToXml(TiXmlElement * settings_) const;

protected:
    void UpdateDevices();

private:
	const Config& mConfig;
    DeviceKeyboard  * mKeyboard;
    DeviceMouse     * mMouse;
    DeviceTouch 	* mTouch;
    std::vector<IDeviceJoystick*>  mJoysticks;
};

#endif // INPUT_DEVICE_MANAGER_H
