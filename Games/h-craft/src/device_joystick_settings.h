// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#ifndef DEVICE_JOYSTICK_SETTINGS_H
#define DEVICE_JOYSTICK_SETTINGS_H

// This is not in joystick to avoid to have include sdl.h
// in other files and avoiding thereby the ugly SDL_Main problems

class TiXmlElement;

struct DeviceJoystickSettings
{
    DeviceJoystickSettings();
    void ReadFromXml(const TiXmlElement * settings_);
    void WriteToXml(TiXmlElement * settings_) const;

    int mDeadArea;
    int mMaxArea;
};

#endif // DEVICE_JOYSTICK_SETTINGS_H
