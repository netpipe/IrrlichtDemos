// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#include "device_mouse.h"
#include "tinyxml/tinyxml.h"
#include <irrlicht.h>

using namespace irr;
using namespace core;

DeviceMouseSettings::DeviceMouseSettings()
{
    mMaxChange = 0.05f;
}

void DeviceMouseSettings::ReadFromXml(const TiXmlElement * settings_)
{
    if ( !settings_ )
        return;

    double val;
    if ( settings_->Attribute("max_change", &val) )
        mMaxChange = val;
}

void DeviceMouseSettings::WriteToXml(TiXmlElement * settings_) const
{
    double val = mMaxChange;
    settings_->SetDoubleAttribute("max_change", val);
}

DeviceMouse::DeviceMouse(irr::gui::ICursorControl * irrlichtCursor_) : IInputDevice()
, mIrrlichtCursor(irrlichtCursor_)
{
    Reset();
}

DeviceMouse::~DeviceMouse()
{
}

std::string DeviceMouse::GetTypeName() const
{
    return std::string("MOUSE"); // TODO: i18n
}

std::string DeviceMouse::GetDeviceName() const
{
    return GetTypeName();
}

InputDeviceType DeviceMouse::GetType() const
{
    return IDP_MOUSE;
}

void DeviceMouse::Update()
{
    if ( !IsEnabled() || !mIrrlichtCursor)
        return;

    position2d<f32> cursorpos = mIrrlichtCursor->getRelativePosition();
    float changeX = cursorpos.X - 0.5f;
    float changeY = cursorpos.Y - 0.5f;
    if ( changeX > mSettings.mMaxChange )
        changeX = mSettings.mMaxChange;
    if ( -changeX < -mSettings.mMaxChange )
        changeX = -mSettings.mMaxChange;
    if ( changeY > mSettings.mMaxChange )
        changeY = mSettings.mMaxChange;
    if ( -changeY < -mSettings.mMaxChange )
        changeY = -mSettings.mMaxChange;
    if ( mSettings.mMaxChange != 0.f )
    {
        mAxes[0] = -changeX / mSettings.mMaxChange;
        mAxes[1] = -changeY / mSettings.mMaxChange;
        for ( int i=0; i<NUM_AXES_MOUSE; ++i )
        {
            if ( mAxes[i] > 1.f )
                mAxes[i] = 1.f;
            else if ( mAxes[i] < -1.f )
                mAxes[i] = -1.f;
        }
    }
    else
    {
        mAxes[0] = 0.f;
        mAxes[1] = 0.f;
    }
    if ( mWheel < 0.f )
        mAxes[2] = -1.f;
    else if ( mWheel > 0.f )
        mAxes[2] = 1.f;
    else
        mAxes[2] = 0.f;
    mWheel = 0.f;

    mOldIrrCursorPos[0] = cursorpos.X;
    mOldIrrCursorPos[1] = cursorpos.Y;


    if ( !mIrrlichtCursor->isVisible() )
        mIrrlichtCursor->setPosition(0.5f, 0.5f);

    // buttons are set from the event receiver
}

int DeviceMouse::GetNumAxes() const
{
    return NUM_AXES_MOUSE;
}

int DeviceMouse::GetNumButtons() const
{
    return NUM_BUTTONS_MOUSE;
}

std::string DeviceMouse::GetNameAxis(int axis_) const
{
    // TODO: i18n
    switch ( axis_ )
    {
        case 0: return std::string("MOUSE-X");
        case 1: return std::string("MOUSE-Y");
        case 2: return std::string("MOUSE-WHEEL");
        default: return std::string("");
    }
}

std::string DeviceMouse::GetNameButton(int button_) const
{
    // TODO: i18n
    switch ( button_ )
    {
        case 0: return std::string("MOUSE-LEFT");
        case 1: return std::string("MOUSE-RIGHT");
        case 2: return std::string("MOUSE-MIDDLE");
        default: return std::string("");
    }
}

bool DeviceMouse::IsAnyButtonDown(int & button_) const
{
    if ( !IsEnabled() )
        return false;

    for ( int i=0; i < NUM_BUTTONS_MOUSE; ++i )
    {
        if ( mButtons[i] != 0 )
        {
            button_ = i;
            return true;
        }
    }
    return false;
}

float DeviceMouse::IsAnyAxisMoved(int & axis_) const
{
    if ( !IsEnabled() )
        return 0.f;

    axis_ = -1;
    float mostMovedVal = 0.f;
    for ( int i=0; i < NUM_AXES_MOUSE; ++i )
    {
        float val = GetAxisValue(i);
        if ( fabs(val) > fabs(mostMovedVal) )
        {
            axis_ = i;
            mostMovedVal = val;
        }
    }

    if ( axis_ >= 0 )
        return mostMovedVal;

    return 0.f;
}

bool DeviceMouse::IsButtonDown(int button_) const
{
    if ( !IsEnabled() )
        return false;

    if ( button_ < NUM_BUTTONS_MOUSE )
        return mButtons[button_] != 0;
    return false;
}

float DeviceMouse::GetAxisValue(int axis_) const
{
    if ( !IsEnabled() )
        return 0.f;

    if ( axis_ < NUM_AXES_MOUSE )
        return mAxes[axis_];
    return 0.f;
}

void DeviceMouse::Reset()
{
    for ( int i=0; i < NUM_BUTTONS_MOUSE; ++i )
    {
        mButtons[i] = 0;
    }
    for ( int i=0; i < NUM_AXES_MOUSE; ++i )
    {
        mAxes[i] = 0.f;
        mOldIrrCursorPos[i] = 0.5f;
    }
    mWheel = 0.f;
    if ( mIrrlichtCursor )
		mIrrlichtCursor->setPosition(0.5f, 0.5f);
}

void DeviceMouse::SetButtonDown(int button_)
{
    if ( button_ < NUM_BUTTONS_MOUSE )
        mButtons[button_] = 1;
}

void DeviceMouse::SetButtonUp(int button_)
{
    if ( button_ < NUM_BUTTONS_MOUSE )
        mButtons[button_] = 0;
}
