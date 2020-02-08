// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#ifndef DEVICE_TOUCH_SETTINGS_H
#define DEVICE_TOUCH_SETTINGS_H

class TiXmlElement;

// No gaps, used in loops
// Also no removing or changing of numbers as used in configs
enum ETOUCH_LAYOUT
{
	ETL_DIGITAL_H_LEFT_V_RIGHT,
	ETL_DIGITAL_V_LEFT_H_RIGHT,
	ETL_DIGITAL_V_RIGHT_ANALOG_H_LEFT,
	ETL_DIGITAL_V_LEFT_ANALOG_H_RIGHT,
	ETL_ANALOG_V_RIGHT_H_LEFT,
	ETL_ANALOG_H_RIGHT_V_LEFT,
	ETL_COUNT,
	ETL_ANY = ETL_COUNT
};

struct DeviceTouchSettings
{
    DeviceTouchSettings();
    void ReadFromXml(const TiXmlElement * settings_);
    void WriteToXml(TiXmlElement * settings_) const;

    ETOUCH_LAYOUT mLayout;
	int mPlacement;
};

#endif // DEVICE_TOUCH_SETTINGS_H
