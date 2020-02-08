// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#include "device_touch.h"
#include "helper_math.h"
#include "logging.h"
#include <irrlicht.h>
#include <sstream>

using namespace irr;
using namespace core;

DeviceTouch::DeviceTouch() : IInputDevice()
{
    ResetValues();
}

DeviceTouch::~DeviceTouch()
{
}

std::string DeviceTouch::GetTypeName() const
{
    return std::string("TOUCH"); // TODO: i18n
}

std::string DeviceTouch::GetDeviceName() const
{
    return GetTypeName();
}

InputDeviceType DeviceTouch::GetType() const
{
    return IDP_TOUCH;
}

void DeviceTouch::Update()
{
    if ( !IsEnabled() )
        return;

	for ( size_t i=0; i < mAxes.size(); ++i )
	{
		mAxes[i].mOldValue = mAxes[i].mValue;
		mAxes[i].mValue = 0.f;
	}
	for ( size_t i=0; i<mButtons.size(); ++i )
	{
		mButtons[i].mButtonDown = false;
	}


	const float maxAxisHit = 2.f;	// x times radius is still ok for axes to be considered a hit
	const int precision = 1;	// A lower precision makes control easier.
	const float deathZone = 0.2f;	// values below that ignored to make centering easier.
	for ( size_t t=0; t<mTouches.size(); ++t )
	{
		bool hit = false;
		core::position2di pos(mTouches[t].mX, mTouches[t].mY);

		// if one button hits take that.
		for ( size_t i=0; i<mButtons.size(); ++i )
		{
			if ( mButtons[i].mArea.isPointInside(pos) )
			{
				mButtons[i].mButtonDown = true;
				hit = true;
				break;
			}
		}

		if ( !hit )
		{
			// More than one axis can be hit at the same time.
			// Also have some lenience so users can move outside - otherwise it's near impossible to hit the max-values.
			// And for now don't care if users has more than one finger inside one axis - that's the users problem.
			for ( size_t i=0; i < mAxes.size(); ++i )
			{
				position2di diff = pos-mAxes[i].mAreaCenter;

				// above and left of center are higher values. below and right are lower values.
				float fx = ExtMath::RoundPrec(-(float)diff.X/(float)mAxes[i].mAreaRadiusX, precision);
				float fy = ExtMath::RoundPrec(-(float)diff.Y/(float)mAxes[i].mAreaRadiusY, precision);
				if ( fabs(fx) <= maxAxisHit && fabs(fy) <= maxAxisHit )
				{
					if ( mAxes[i].mDirection == E_HORIZONTAL )
					{
						if ( fabs(fx) < deathZone )
							fx = 0.f;
						mAxes[i].mValue = core::clamp(fx, -1.f, 1.f);
					}
					else
					{
						if ( fabs(fy) < deathZone )
							fy = 0.f;
						mAxes[i].mValue = core::clamp(fy, -1.f, 1.f);
					}
				}
			}
		}
	}
}

int DeviceTouch::GetNumAxes() const
{
    return (int)mAxes.size();
}

int DeviceTouch::GetNumButtons() const
{
    return (int)mButtons.size();
}

std::string DeviceTouch::GetNameAxis(int axis_) const
{

    if ( axis_ > 0 && (size_t)axis_ < mAxes.size() )
        return mAxes[axis_].mName;

	return std::string("BAD_AXIS");
}

std::string DeviceTouch::GetNameButton(int button) const
{
	if ( button >= 0 && (size_t)button < mButtons.size() )
		return mButtons[size_t(button)].mName;

	return std::string("BAD_BUTTON");
}

bool DeviceTouch::IsAnyButtonDown(int & button_) const
{
	for ( size_t i=0; i < mButtons.size(); ++i )
		if ( mButtons[i].mButtonDown )
			return true;
    return false;
}

float DeviceTouch::IsAnyAxisMoved(int & axis_) const
{
    if ( !IsEnabled() )
        return 0.f;

    axis_ = -1;
    float mostMovedVal = 0.f;
    for ( size_t i=0; i < mAxes.size(); ++i )
    {
        float val = mAxes[i].mValue-mAxes[i].mOldValue;
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

bool DeviceTouch::IsButtonDown(int button) const
{
	if ( !IsEnabled() )
		return false;

	if ( button >= 0 && (size_t)button < mButtons.size() )
		return mButtons[size_t(button)].mButtonDown;

	return false;
}

float DeviceTouch::GetAxisValue(int axis_) const
{
    if ( !IsEnabled() )
        return 0.f;

    if ( axis_ >= 0 && (size_t)axis_ < mAxes.size() )
        return mAxes[axis_].mValue;

    return 0.f;
}

void DeviceTouch::Reset()
{
	ResetValues();
}

void DeviceTouch::ResetValues()
{
	mTouches.clear();
    for ( size_t i=0; i < mAxes.size(); ++i )
    {
        mAxes[i].mValue = 0.f;
        mAxes[i].mOldValue = 0.f;
    }
}

void DeviceTouch::OnEvent(const irr::SEvent &event, ETouchInput touchInput)
{
    switch ( event.EventType )
    {
        case EET_MOUSE_INPUT_EVENT:
        {
        	if ( touchInput == ETI_TOUCH_SIMULATION )
			{
				switch ( event.MouseInput.Event )
				{
					case EMIE_LMOUSE_PRESSED_DOWN:
					{
						STouch touch;
						touch.mID = 0;
						touch.mX = event.MouseInput.X;
						touch.mY = event.MouseInput.Y;
						mTouches.push_back(touch);
					}
					break;
					case EMIE_LMOUSE_LEFT_UP:
					{
						for ( size_t i=0; i<mTouches.size(); ++i )
						{
							if ( mTouches[i].mID == 0 )
							{
								mTouches.erase(mTouches.begin() + i);
								break;
							}
						}
					}
					break;
					case EMIE_MOUSE_MOVED:
					{
						for ( size_t i=0; i<mTouches.size(); ++i )
						{
							if ( mTouches[i].mID == 0 )
							{
								mTouches[i].mX = event.MouseInput.X;
								mTouches[i].mY = event.MouseInput.Y;
								break;
							}
						}
					}
					break;
					default:
					break;
				}
			}
		}
		break;

#ifdef _IRR_ANDROID_PLATFORM_
		case EET_TOUCH_INPUT_EVENT:
		{
			switch ( event.TouchInput.Event)
			{
				case ETIE_PRESSED_DOWN:
				{
					STouch touch;
					touch.mID = event.TouchInput.ID;
					touch.mX = event.TouchInput.X;
					touch.mY = event.TouchInput.Y;
					mTouches.push_back(touch);

					break;
				}
				case ETIE_MOVED:
				{
					for ( size_t i=0; i<mTouches.size(); ++i )
					{
						if ( mTouches[i].mID == event.TouchInput.ID )
						{
							mTouches[i].mX = event.TouchInput.X;
							mTouches[i].mY = event.TouchInput.Y;
							break;
						}
					}

					break;
				}
				case ETIE_LEFT_UP:
				{
					for ( size_t i=0; i<mTouches.size(); ++i )
					{
						if ( mTouches[i].mID == event.TouchInput.ID )
						{
							mTouches.erase(mTouches.begin() + i);
							break;
						}
					}

					break;
				}
				default:
					return;
			}
		}
		break;

		// TEST
		case EET_ACCELEROMETER_EVENT:
		{
			static f64 lastX, lastY, lastZ = 0.;
			if ( !core::equals(lastX, event.AccelerometerEvent.X, 1.)
				|| !core::equals(lastY, event.AccelerometerEvent.Y, 1.)
				|| !core::equals(lastZ, event.AccelerometerEvent.Z, 1.) )
			{
				lastX = event.AccelerometerEvent.X;
				lastY = event.AccelerometerEvent.Y;
				lastZ = event.AccelerometerEvent.Z;
				char buf[100];
				sprintf(buf, "ACCEL: %f %f %f", event.AccelerometerEvent.X, event.AccelerometerEvent.Y, event.AccelerometerEvent.Z);
				LOG.Info(buf);
			}
		}
		break;

		case EET_GYROSCOPE_EVENT:
		{
			static f64 lastX, lastY, lastZ = 0.;
			if ( !core::equals(lastX, event.GyroscopeEvent.X, 1.)
				|| !core::equals(lastY, event.GyroscopeEvent.Y, 1.)
				|| !core::equals(lastZ, event.GyroscopeEvent.Z, 1.) )
			{
				lastX = event.GyroscopeEvent.X;
				lastY = event.GyroscopeEvent.Y;
				lastZ = event.GyroscopeEvent.Z;
				char buf[100];
				sprintf(buf, "GYRO: %f %f %f", event.GyroscopeEvent.X, event.GyroscopeEvent.Y, event.GyroscopeEvent.Z);
				LOG.Info(buf);
			}
		}
		break;
		// TEST - END
#endif
		default:
		break;
	}
}

void DeviceTouch::addButton(const irr::core::recti& area, const std::string& name)
{
	SButton button;

	button.mArea = area;
	button.mName = name;
	button.mButtonDown = false;
	mButtons.push_back(button);
}

void DeviceTouch::removeAllButtons()
{
	mButtons.clear();
}

void DeviceTouch::addAxis(const irr::core::recti& area, const std::string& name, EDirection dir)
{
	SAxis axis;
	axis.mAreaCenter = area.getCenter();
	axis.mAreaRadiusX = area.getWidth() / 2;
	if ( axis.mAreaRadiusX < 1 )
		axis.mAreaRadiusX = 1;
	axis.mAreaRadiusY = area.getHeight() / 2;
	if ( axis.mAreaRadiusY < 1 )
		axis.mAreaRadiusY = 1;
	axis.mName = name;
	axis.mDirection = dir;
	axis.mValue = 0.f;
	axis.mOldValue = 0.f;
	mAxes.push_back(axis);
}

void DeviceTouch::removeAllAxes()
{
	mAxes.clear();
}
