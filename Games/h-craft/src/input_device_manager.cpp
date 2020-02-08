// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#include "input_device_manager.h"
#include "irrlicht_manager.h"
#include "logging.h"
#include "main.h"
#include "config.h"
#include "device_keyboard.h"
#include "device_mouse.h"
#include "device_joystick.h"
#include "device_touch.h"
#include "controller.h"
#include "tinyxml/tinyxml.h"

using namespace irr;

InputDeviceManager::InputDeviceManager(const Config& config)
: mConfig(config)
, mKeyboard(NULL)
, mMouse(NULL)
, mTouch(NULL)
{
#ifdef HC1_HAS_SDL
    // For joystick - remove if SDL is initialized somewhere else and make sure joystick is included there
    SDL_Init(SDL_INIT_NOPARACHUTE);
    SDL_InitSubSystem(SDL_INIT_JOYSTICK);
#endif
}

InputDeviceManager::~InputDeviceManager()
{
    for ( unsigned int i=0; i< mJoysticks.size(); ++i )
    {
        delete mJoysticks[i];
    }
    delete mMouse;
    delete mKeyboard;
    delete mTouch;
#ifdef HC1_HAS_SDL
    SDL_Quit(); // for joystick - remove if SDL is closed somewhere else
#endif
}

void InputDeviceManager::CreateAllDevices(irr::gui::ICursorControl * cursorControl)
{
    mKeyboard = new DeviceKeyboard();
    mMouse = new DeviceMouse(cursorControl);
    mTouch = new DeviceTouch();

#ifdef HC1_HAS_SDL
    // joystick
    int numJoysticks = SDL_NumJoysticks();
    if ( numJoysticks > MAX_JOYSTICKS )
        numJoysticks = MAX_JOYSTICKS;

    for ( int i = 0; i < numJoysticks; ++i )
    {
        SDL_Joystick* sldJoystick = SDL_JoystickOpen(i);

        if(sldJoystick)
        {
            IDeviceJoystick * joystick = new DeviceJoystickSDL(sldJoystick);
            joystick->CalibrateCenter();
            mJoysticks.push_back(joystick);
        }
        else
        {
            LOG.LogLn(LP_WARN, "Couldn't open Joystick ", i);
        }
    }
#endif

// TEST
/*
#ifdef _IRR_ANDROID_PLATFORM_
	if ( APP.GetIrrlichtManager()->GetIrrlichtDevice()->activateAccelerometer() )
	{
		LOG.Info("Accelerometer activated");
	}
	else
	{
		LOG.Info("Accelerometer NOT activated");
	}
	if ( APP.GetIrrlichtManager()->GetIrrlichtDevice()->activateGyroscope() )
	{
		LOG.Info("Gyroscope activated");
	}
	else
	{
		LOG.Info("Gyroscope NOT activated");
	}
#endif
// TEST-END
*/
}

void InputDeviceManager::UpdateDevices()
{
	if ( mMouse )
		mMouse->Update();
	if ( mKeyboard )
		mKeyboard->Update();
	if ( mTouch )
		mTouch->Update();

#ifdef HC1_HAS_SDL
    if ( GetNumJoysticks() )
    {
        SDL_JoystickUpdate();
    }
    for ( unsigned int i=0; i< mJoysticks.size(); ++i )
    {
        mJoysticks[i]->Update();
    }
#endif
}

float InputDeviceManager::GetAxisValue(const ControllerAxis &axis_, float oldValue_) const
{
    float value = 0.f;
    if ( axis_.mType == CAT_AXIS )
    {
        const IInputDevice* device = GetDevice(axis_.Axis.mDeviceType, axis_.Axis.mJoystickId);
        if ( device )
        {
            value = device->GetAxisValue(axis_.Axis.mAxisId);
            if ( axis_.Axis.mGoofy )
                value *= -1.f;
            if ( axis_.mDirections == 1 && value < 0.f )
            {
                value = 0.f;
            }
        }
    }
    else if ( axis_.mType == CAT_BUTTONS )
    {
        const IInputDevice* device = GetDevice(axis_.Buttons.mDeviceType1, axis_.Buttons.mJoystickId1);
        if ( device && device->IsButtonDown(axis_.Buttons.mButtonId1 ) )
        {
            value += axis_.Buttons.mStepSize1;
        }
        device = GetDevice(axis_.Buttons.mDeviceType2, axis_.Buttons.mJoystickId2);
        if ( device && axis_.mDirections > 1 && device->IsButtonDown(axis_.Buttons.mButtonId2 ) )
        {
            value -= axis_.Buttons.mStepSize2;
        }
        if ( value != 0.f )
        {
            value += oldValue_;
            if ( value > 1.f )
                value = 1.f;
            if ( value < -1.f )
                value = -1.f;
        }
    }
    return value;
}

bool InputDeviceManager::GetButtonDown(const ControllerButton &button_) const
{
    const IInputDevice* device = GetDevice(button_.mDeviceType, button_.mJoystickId);
    if ( !device )
    {
        return false;
    }

    return device->IsButtonDown(button_.mButtonId);
}

void InputDeviceManager::Update()
{
    UpdateDevices();
}

size_t InputDeviceManager::GetNumJoysticks() const
{
    return mJoysticks.size();
}

IDeviceJoystick* InputDeviceManager::GetJoystick(size_t index_) const
{
    if ( index_ < mJoysticks.size() )
        return mJoysticks[index_];
    return NULL;
}

bool InputDeviceManager::WaitForAxis( ControllerAxis &controllerAxis_, bool up_ )
{
    bool wait = true;
    if ( mMouse )
		mMouse->Reset();
	if ( mKeyboard )
		mKeyboard->Reset();
    while (wait)
    {
        APP.ForceUpdates(/*graphics_*/ true, /*sound_*/ true);
        UpdateDevices();

        ControllerButton buttonCheck;
        if ( CheckForAnyButton( buttonCheck ) )
        {
            if (    buttonCheck.mDeviceType == IDP_KEYBOARD
                &&  buttonCheck.mButtonId == KEY_ESCAPE )
            {
                return false;
            }

            if ( controllerAxis_.mAllowDigital )
            {
                wait = false;
                controllerAxis_.mType = CAT_BUTTONS;
                controllerAxis_.ResetAxis();

                if (up_ || controllerAxis_.mDirections < 2)
                {
                    controllerAxis_.Buttons.mDeviceType1 = buttonCheck.mDeviceType;
                    controllerAxis_.Buttons.mJoystickId1 = buttonCheck.mJoystickId;
                    controllerAxis_.Buttons.mButtonId1 = buttonCheck.mButtonId;
                }
                else
                {
                    controllerAxis_.Buttons.mDeviceType2 = buttonCheck.mDeviceType;
                    controllerAxis_.Buttons.mJoystickId2 = buttonCheck.mJoystickId;
                    controllerAxis_.Buttons.mButtonId2 = buttonCheck.mButtonId;
                }
            }
        }

        if ( wait && controllerAxis_.mAllowAnalog )
        {
            ControllerAxis axisCheck;
            float minVal = 0.1f;
            float val = CheckForAnyAxis(axisCheck, minVal);
            if ( fabs(val) > minVal )
            {
                wait = false;
                controllerAxis_.mType = CAT_AXIS;
                controllerAxis_.ResetButtons();
                controllerAxis_.Axis.mDeviceType = axisCheck.Axis.mDeviceType;
                controllerAxis_.Axis.mJoystickId = axisCheck.Axis.mJoystickId;
                controllerAxis_.Axis.mAxisId = axisCheck.Axis.mAxisId;
                if (up_)
                {
                    if ( val > 0.f )
                        controllerAxis_.Axis.mGoofy = false;
                    else
                        controllerAxis_.Axis.mGoofy = true;
                }
                else
                {
                    if ( val < 0.f )
                        controllerAxis_.Axis.mGoofy = false;
                    else
                        controllerAxis_.Axis.mGoofy = true;
                }
            }
        }
    }
    return true;
}

float InputDeviceManager::CheckForAnyAxis( ControllerAxis &controllerAxis_, float minVal_ )
{
    float val = mMouse ? mMouse->IsAnyAxisMoved(controllerAxis_.Axis.mAxisId) : 0.f;
    if ( fabs(val) > minVal_ )
    {
        controllerAxis_.Axis.mDeviceType = IDP_MOUSE;
        return val;
    }
    else
    {
        for ( unsigned int i=0; i< mJoysticks.size(); ++i )
        {
            val = mJoysticks[i]->IsAnyAxisMoved(controllerAxis_.Axis.mAxisId);
            if ( fabs(val) > minVal_ )
            {
                controllerAxis_.Axis.mDeviceType = IDP_JOYSTICK;
                controllerAxis_.Axis.mJoystickId = i;
                return val;
            }
        }
    }
    return 0.f;
}

bool InputDeviceManager::CheckForAnyButton( ControllerButton &controllerButton_ )
{
    if ( mKeyboard && mKeyboard->IsAnyButtonDown(controllerButton_.mButtonId) )
    {
        controllerButton_.mDeviceType = IDP_KEYBOARD;
        return true;
    }
    else if ( mMouse && mMouse->IsAnyButtonDown(controllerButton_.mButtonId) )
    {
        controllerButton_.mDeviceType = IDP_MOUSE;
        return true;
    }
    else
    {
        for ( unsigned int i=0; i< mJoysticks.size(); ++i )
        {
            if ( mJoysticks[i]->IsAnyButtonDown(controllerButton_.mButtonId) )
            {
                controllerButton_.mDeviceType = IDP_JOYSTICK;
                controllerButton_.mJoystickId = i;
                return true;
            }
        }
    }
    return false;
}

bool InputDeviceManager::WaitForButton( ControllerButton &controllerButton_ )
{
    bool wait = true;
    while (wait)
    {
        APP.ForceUpdates(/*graphics_*/ true, /*sound_*/ true);
        UpdateDevices();

        ControllerButton buttonCheck;
        if ( CheckForAnyButton( buttonCheck ) )
        {
            if (    buttonCheck.mDeviceType == IDP_KEYBOARD
                &&  buttonCheck.mButtonId == KEY_ESCAPE )
            {
                return false;
            }

            wait = false;
            controllerButton_ = buttonCheck;
        }
    }

    return true;
}

void InputDeviceManager::ReadFromXml(const TiXmlElement * settings_)
{
    if ( !settings_ )
        return;

    const TiXmlNode* nodeDevices = settings_->FirstChild("devices");
    if ( nodeDevices )
    {
        const TiXmlNode* node = nodeDevices->FirstChild("joystick");
        if ( node )
        {
            for ( unsigned int i=0; i < mJoysticks.size(); ++i )
            {
                mJoysticks[i]->mSettings.ReadFromXml(node->ToElement());
            }
        }

        node = nodeDevices->FirstChild("touch");
        if ( node && mTouch )
        {
			DeviceTouchSettings settings;
			settings.ReadFromXml(node->ToElement());
			mTouch->SetSettings(settings);
        }
    }
}

void InputDeviceManager::WriteToXml(TiXmlElement * settings_) const
{
    if ( !settings_ )
        return;

    TiXmlNode* nodeDevices = settings_->FirstChild("devices");
    if ( nodeDevices )
    {
        TiXmlNode* node = nodeDevices->FirstChild("joystick");
        if ( node )
        {
            if( mJoysticks.size() ) // TODO: only saving first joystick setting so far
                mJoysticks[0]->mSettings.WriteToXml(node->ToElement());
        }
        node = nodeDevices->FirstChild("touch");
        if ( node )
        {
            if( mTouch )
            {
				DeviceTouchSettings settings = mTouch->GetSettings();
                settings.WriteToXml(node->ToElement());
			}
        }
    }
}

IInputDevice* InputDeviceManager::GetDevice(InputDeviceType type_, int joystickId_) const
{
    switch ( type_ )
    {
        case IDP_KEYBOARD:
        {
            return mKeyboard;
        }
        case IDP_MOUSE:
        {
            return mMouse;
        }
        case IDP_JOYSTICK:
        {
            return GetJoystick(joystickId_);
        }
        default:
        case IDP_TOUCH:
        {
			return mTouch;
        }
        break;
    }
    return NULL;
}

bool InputDeviceManager::GetNameForButton(const ControllerButton &button_, std::wstring &str_, bool withDevice_) const
{
    const IInputDevice * device = GetDevice(button_.mDeviceType, button_.mJoystickId);
    if ( !device )
        return false;

    str_.clear();
    std::string name;
    if ( withDevice_ )
    {
        name = device->GetTypeName();
        str_.append( std::wstring(name.begin(), name.end()) );
        str_.append( L" - ");
    }
    name = device->GetNameButton(button_.mButtonId);
    str_.append( std::wstring(std::wstring(name.begin(), name.end())) );

    return true;
}

bool InputDeviceManager::GetNameForAxis(const ControllerAxis &axis_, bool up_, std::wstring &str_, bool withDevice_) const
{
    InputDeviceType type = IDP_MAX;
    int joystickId = 0;
    if ( axis_.mType == CAT_BUTTONS )
    {
        if ( up_ )
        {
            type = axis_.Buttons.mDeviceType1;
            joystickId = axis_.Buttons.mJoystickId1;
        }
        else
        {
            type = axis_.Buttons.mDeviceType2;
            joystickId = axis_.Buttons.mJoystickId2;
        }
    }
    else if ( axis_.mType == CAT_AXIS )
    {
        type = axis_.Axis.mDeviceType;
        joystickId = axis_.Axis.mJoystickId;
    }

    const IInputDevice * device = GetDevice(type, joystickId);
    if ( !device )
        return false;

    str_.clear();
    std::string name;
    if ( withDevice_ )
    {
        name = device->GetTypeName();
        str_.append( std::wstring(name.begin(), name.end()) );
        str_.append( L" - ");
    }
    if ( axis_.mType == CAT_BUTTONS )
    {
        if ( up_ )
            name = device->GetNameButton(axis_.Buttons.mButtonId1);
        else
            name = device->GetNameButton(axis_.Buttons.mButtonId2);
    }
    else if ( axis_.mType == CAT_AXIS )
    {
        name = device->GetNameAxis(axis_.Axis.mAxisId);
        if ( axis_.Axis.mGoofy )
        {
            name.append(" goofy");  // TODO: i18n
        }
    }
    else
    {
        name = "error";    // TODO: i18n
    }
    str_.append( std::wstring(std::wstring(name.begin(), name.end())) );

    return true;
}

bool InputDeviceManager::OnEvent(const irr::SEvent &event)
{
    switch ( event.EventType )
    {
#ifdef _IRR_ANDROID_PLATFORM_
		case EET_TOUCH_INPUT_EVENT:
		case EET_ACCELEROMETER_EVENT:
		case EET_GYROSCOPE_EVENT:
		{
			if ( mTouch )
			{
				mTouch->OnEvent(event, mConfig.GetUseTouchInput());
			}
		}
		break;
#endif
        case EET_KEY_INPUT_EVENT:
        {
            if ( GetKeyboard() )
            {
                if ( event.KeyInput.PressedDown )
                    GetKeyboard()->SetKeyDown(event.KeyInput.Key);
                else
                    GetKeyboard()->SetKeyUp(event.KeyInput.Key);
            }
        }
        break;
        case EET_MOUSE_INPUT_EVENT:
        {
			if ( mTouch && mConfig.GetUseTouchInput() == ETI_TOUCH_SIMULATION)
			{
				mTouch->OnEvent(event, mConfig.GetUseTouchInput());
			}

            if ( mMouse )
            {
                switch ( event.MouseInput.Event )
                {
                    case EMIE_LMOUSE_PRESSED_DOWN:
                        mMouse->SetButtonDown(0);
                    break;
                    case EMIE_RMOUSE_PRESSED_DOWN:
                        mMouse->SetButtonDown(1);
                    break;
                    case EMIE_MMOUSE_PRESSED_DOWN:
                        mMouse->SetButtonDown(2);
                    break;
                    case EMIE_LMOUSE_LEFT_UP:
                        mMouse->SetButtonUp(0);
                    break;
                    case EMIE_RMOUSE_LEFT_UP:
                        mMouse->SetButtonUp(1);
                    break;
                    case EMIE_MMOUSE_LEFT_UP:
                        mMouse->SetButtonUp(2);
                    break;
                    case EMIE_MOUSE_WHEEL:
                        mMouse->SetWheel( event.MouseInput.Wheel );
                    break;
                    default:
                    break;
                }
            }
        }
        break;

        default:
        break;
    }
    return false;
}
