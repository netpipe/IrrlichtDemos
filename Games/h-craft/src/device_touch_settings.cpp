// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#include "device_touch_settings.h"
#include "tinyxml/tinyxml.h"

DeviceTouchSettings::DeviceTouchSettings()
: mLayout(ETL_DIGITAL_H_LEFT_V_RIGHT)
, mPlacement(3)
{
}

void DeviceTouchSettings::ReadFromXml(const TiXmlElement * settings_)
{
    if ( !settings_ )
        return;

	int val = 0;
    settings_->Attribute("layout", &val);
    mLayout = (ETOUCH_LAYOUT)val;
    settings_->Attribute("placement", &mPlacement);
}

void DeviceTouchSettings::WriteToXml(TiXmlElement * settings_) const
{
    if ( !settings_ )
        return;

	int val = mLayout;
    settings_->SetAttribute("layout", val);
    settings_->SetAttribute("placement", mPlacement);
}
