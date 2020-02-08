// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#include "device_joystick_settings.h"
#include "tinyxml/tinyxml.h"

DeviceJoystickSettings::DeviceJoystickSettings()
{
    mDeadArea = 5000;
    mMaxArea = 36767;
}

void DeviceJoystickSettings::ReadFromXml(const TiXmlElement * settings_)
{
    if ( !settings_ )
        return;

    settings_->Attribute("dead_area", &mDeadArea);
    settings_->Attribute("max_area", &mMaxArea);
}

void DeviceJoystickSettings::WriteToXml(TiXmlElement * settings_) const
{
    if ( !settings_ )
        return;

    settings_->SetAttribute("dead_area", mDeadArea);
    settings_->SetAttribute("max_area", mMaxArea);
}
