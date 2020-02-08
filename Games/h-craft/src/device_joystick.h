// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#ifndef DEVICE_JOYSTICK_H
#define DEVICE_JOYSTICK_H

#include "compile_config.h"
#include "input_device.h"
#include "device_joystick_settings.h"
#ifdef HC1_HAS_SDL
#include "SDL/SDL.h"
#endif
#include <vector>

#define MAX_JOYSTICKS 4
#define MAX_JOYSTICK_AREA 32767

// just a dummy class
class IDeviceJoystick : public IInputDevice
{
public:
    IDeviceJoystick()  {};
    virtual ~IDeviceJoystick() {}

    virtual void CalibrateCenter() = 0;

    DeviceJoystickSettings   mSettings;
};



#ifdef HC1_HAS_SDL
class DeviceJoystickSDL : public IDeviceJoystick
{
public:
    DeviceJoystickSDL(SDL_Joystick * sdlJoystick_);
    virtual ~DeviceJoystickSDL();

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

    virtual void CalibrateCenter();

private:
    SDL_Joystick * mSdlJoystick;
    float        * mAxisCalibrationCenter;
};
#endif

#endif // DEVICE_JOYSTICK_H
