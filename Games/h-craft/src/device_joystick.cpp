// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#include "device_joystick.h"

#ifdef HC1_HAS_SDL
#include <math.h>

DeviceJoystickSDL::DeviceJoystickSDL(SDL_Joystick * sdlJoystick_) : IDeviceJoystick()
, mSdlJoystick(sdlJoystick_)
, mAxisCalibrationCenter(NULL)
{
    if ( mSdlJoystick )
    {
        int axes = SDL_JoystickNumAxes(mSdlJoystick);
        mAxisCalibrationCenter = new float[axes];
        for ( int i=0; i<axes; ++i )
        {
            mAxisCalibrationCenter[i] = 0.f;
        }
    }
}

DeviceJoystickSDL::~DeviceJoystickSDL()
{
    if ( SDL_JoystickOpened(SDL_JoystickIndex(mSdlJoystick)) )
    {
        SDL_JoystickClose(mSdlJoystick);
        mSdlJoystick = NULL;
    }
    delete[] mAxisCalibrationCenter;
}

std::string DeviceJoystickSDL::GetTypeName() const
{
    std::string name("JOYSTICK "); // TODO: i18n
    char buf[100];
    sprintf(buf, "%d", SDL_JoystickIndex(mSdlJoystick));
    name += buf;
    return name;
}

std::string DeviceJoystickSDL::GetDeviceName() const
{
    return std::string( SDL_JoystickName(SDL_JoystickIndex(mSdlJoystick)) );
}

InputDeviceType DeviceJoystickSDL::GetType() const
{
    return IDP_JOYSTICK;
}

void DeviceJoystickSDL::Update()
{
    // nothing todo. joystick update ist done in input_manager (because it should only happen once)
}

int DeviceJoystickSDL::GetNumAxes() const
{
    return SDL_JoystickNumAxes(mSdlJoystick);
}

int DeviceJoystickSDL::GetNumButtons() const
{
    return SDL_JoystickNumButtons(mSdlJoystick);
}

std::string DeviceJoystickSDL::GetNameAxis(int axis_) const
{
    char txt[100];
    sprintf(txt, "JS-AXIS %d", axis_);
    return std::string(txt);
}

std::string DeviceJoystickSDL::GetNameButton(int button_) const
{
    char txt[100];
    sprintf(txt, "JS-BUTTON %d", button_);
    return std::string(txt);
}

bool DeviceJoystickSDL::IsAnyButtonDown(int & button_) const
{
    if ( !IsEnabled() )
        return false;

    int numButtons = GetNumButtons();
    for ( int i=0; i < numButtons; ++i )
    {
        if ( IsButtonDown(i) )
        {
            button_ = i;
            return true;
        }
    }

    return false;
}

float DeviceJoystickSDL::IsAnyAxisMoved(int & axis_) const
{
    if ( !IsEnabled() )
        return 0.f;

    axis_ = -1;
    int numAxes = GetNumAxes();
    float mostMovedVal = 0.f;
    for ( int i=0; i < numAxes; ++i )
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

bool DeviceJoystickSDL::IsButtonDown(int button_) const
{
    if ( !IsEnabled() )
        return false;

    return SDL_JoystickGetButton(mSdlJoystick, button_);
}

float DeviceJoystickSDL::GetAxisValue(int axis_) const
{
    if ( !IsEnabled() )
        return 0.f;

    int value = SDL_JoystickGetAxis(mSdlJoystick, axis_);
    value -= (int)mAxisCalibrationCenter[axis_];
    if ( value > mSettings.mMaxArea )
        value = mSettings.mMaxArea;
    else if ( value < -mSettings.mMaxArea )
        value = -mSettings.mMaxArea;

    if ( abs(value) < mSettings.mDeadArea )
        return 0.f;
    if ( mSettings.mMaxArea == 0 )
        return 0.f;

    if ( value < -mSettings.mDeadArea )
        value += mSettings.mDeadArea;
    if ( value > mSettings.mDeadArea )
        value -= mSettings.mDeadArea;

    value *= -1;

    return (float)value / (float)(mSettings.mMaxArea - mSettings.mDeadArea);
}

void DeviceJoystickSDL::CalibrateCenter()
{
    if ( !mSdlJoystick)
        return;
    for ( int i=0; i<GetNumAxes(); ++i )
    {
        float center = 0.f;
        for ( int a=0; a<5; ++a  )
            center += SDL_JoystickGetAxis(mSdlJoystick, i);
        center /= 5;

        mAxisCalibrationCenter[i] = center;
    }
}

#endif // HC1_HAS_SDL
