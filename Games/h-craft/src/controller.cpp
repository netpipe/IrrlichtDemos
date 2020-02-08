// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#include "controller.h"
#include "input_device_manager.h"
#include "globals.h"
#include "tinyxml/tinyxml.h"
#include "gui_dialogs/gui_touch.h"	// for the enums

// --------------------------------------------------------
ControllerAxis::ControllerAxis()
{
    mType = CAT_NONE;
    mAllowAnalog = true;
    mAllowDigital = true;
    mDirections = 2;

    ResetAxis();
    ResetButtons();
}

void ControllerAxis::ResetAxis()
{
    Axis.mDeviceType = IDP_NONE;
    Axis.mJoystickId = 0;
    Axis.mAxisId = 0;
    Axis.mGoofy = false;
}

void ControllerAxis::ResetButtons()
{
    Buttons.mDeviceType1 = IDP_NONE;
    Buttons.mJoystickId1 = 0;
    Buttons.mButtonId1 = 0;
    Buttons.mStepSize1 = 0.5f;
    Buttons.mDeviceType2 = IDP_NONE;
    Buttons.mJoystickId2 = 0;
    Buttons.mButtonId2 = 0;
    Buttons.mStepSize2 = 0.5f;
}

void ControllerAxis::ReadFromXml(const TiXmlElement * settings_)
{
    if ( !settings_ )
        return;

    const char * valName = settings_->Attribute("type");
    assert( valName );
    if ( valName )
    {
        int iVal=0;
        settings_->Attribute("analog", &iVal);
        mAllowAnalog = iVal ? true : false;

        settings_->Attribute("digital", &iVal);
        mAllowDigital = iVal ? true : false;

        settings_->Attribute("dir", &mDirections);

        if ( 0 == strcmp("BT", valName) )
        {
            mType = CAT_BUTTONS;

            const char * valDevice = settings_->Attribute("deviceType1");
            if ( valDevice )
                Buttons.mDeviceType1 = IInputDevice::GetDeviceTypeForXmlName(valDevice);
            settings_->Attribute("joystickId1", &Buttons.mJoystickId1);
            settings_->Attribute("buttonId1", &Buttons.mButtonId1);
            double val = 0.f;
            if ( settings_->Attribute("stepSize1", &val) )
                Buttons.mStepSize1 = val;
            const char * valDevice2 = settings_->Attribute("deviceType2");
            if ( valDevice2 )
                Buttons.mDeviceType2 = IInputDevice::GetDeviceTypeForXmlName(valDevice2);
            settings_->Attribute("joystickId2", &Buttons.mJoystickId2);
            settings_->Attribute("buttonId2", &Buttons.mButtonId2);
            if ( settings_->Attribute("stepSize2", &val) )
                Buttons.mStepSize2 = val;
        }
        else if ( 0 == strcmp("AX", valName) )
        {
            mType = CAT_AXIS;
            const char * valDevice = settings_->Attribute("deviceType");
            if ( valDevice )
                Axis.mDeviceType = IInputDevice::GetDeviceTypeForXmlName(valDevice);
            settings_->Attribute("joystickId", &Axis.mJoystickId);
            settings_->Attribute("axisId", &Axis.mAxisId);
            int val=0;
            if ( settings_->Attribute("goofy", &val) )
                Axis.mGoofy = val != 0;
        }
        else
        {
            assert(!"ControllerAxis::ReadFromXml - invalid type");
        }
    }
}

void ControllerAxis::WriteToXml(TiXmlElement * settings_) const
{
    if ( !settings_ )
        return;

    switch ( mType )
    {
        case CAT_BUTTONS:
            settings_->SetAttribute("type", "BT");

            settings_->SetAttribute("deviceType1", IInputDevice::GetXmlNameForDeviceType(Buttons.mDeviceType1) );
            settings_->SetAttribute("joystickId1", Buttons.mJoystickId1);
            settings_->SetAttribute("buttonId1", Buttons.mButtonId1);
            settings_->SetDoubleAttribute("stepSize1", Buttons.mStepSize1);

            settings_->SetAttribute("deviceType2", IInputDevice::GetXmlNameForDeviceType(Buttons.mDeviceType2) );
            settings_->SetAttribute("joystickId2", Buttons.mJoystickId2);
            settings_->SetAttribute("buttonId2", Buttons.mButtonId2);
            settings_->SetDoubleAttribute("stepSize2", Buttons.mStepSize2);

        break;
        case CAT_AXIS:
        {
            settings_->SetAttribute("type", "AX");

            settings_->SetAttribute("deviceType", IInputDevice::GetXmlNameForDeviceType(Axis.mDeviceType) );
            settings_->SetAttribute("joystickId", Axis.mJoystickId);
            settings_->SetAttribute("axisId", Axis.mAxisId);
            int val= Axis.mGoofy ? 1 : 0;
            settings_->SetAttribute("goofy", val);
        }
        break;
        default:
            assert(!"ControllerAxis::WriteToXml - invalid type");
        break;
    }

    settings_->SetAttribute("analog", mAllowAnalog ? 1 : 0);
    settings_->SetAttribute("digital", mAllowDigital ? 1 : 0);
    settings_->SetAttribute("dir", mDirections);
}

// --------------------------------------------------------
ControllerButton::ControllerButton()
{
    mDeviceType = IDP_NONE;
    mJoystickId = 0;
    mButtonId = 0;
}

void ControllerButton::ReadFromXml(const TiXmlElement * settings_)
{
    if ( !settings_ )
        return;

    const char * valName = settings_->Attribute("deviceType");
    assert( valName );
    if ( valName )
        mDeviceType = IInputDevice::GetDeviceTypeForXmlName(valName);
    settings_->Attribute("joystickId", &mJoystickId);
    settings_->Attribute("buttonId", &mButtonId);
}

void ControllerButton::WriteToXml(TiXmlElement * settings_) const
{
    if ( !settings_ )
        return;

    settings_->SetAttribute("deviceType", IInputDevice::GetXmlNameForDeviceType(mDeviceType) );
    settings_->SetAttribute("joystickId", mJoystickId);
    settings_->SetAttribute("buttonId", mButtonId);
}

// --------------------------------------------------------
ControllerSettings::ControllerSettings()
{
    mAnalogSteering.mAllowDigital = false;
    mAnalogAcceleration.mDirections = 1;
    mAnalogAcceleration.mAllowDigital = false;
    mAnalogBrake.mDirections = 1;
    mAnalogBrake.mAllowDigital = false;
    mDigitalLeft.mDirections = 1;
    mDigitalLeft.mAllowAnalog = false;
    mDigitalRight.mDirections = 1;
    mDigitalRight.mAllowAnalog = false;
    mDigitalAccelerate.mDirections = 1;
    mDigitalAccelerate.mAllowAnalog = false;
    mDigitalBrake.mDirections = 1;
    mDigitalBrake.mAllowAnalog = false;
}

void ControllerSettings::ReadFromXml(const TiXmlElement * settings_)
{
    if ( !settings_ )
        return;

    const TiXmlNode * node = NULL;

    node = settings_->FirstChild("asteer");
    if ( node )
        mAnalogSteering.ReadFromXml(node->ToElement());

    node = settings_->FirstChild("aacc");
    if ( node )
        mAnalogAcceleration.ReadFromXml(node->ToElement());

    node = settings_->FirstChild("abrake");
    if ( node )
        mAnalogBrake.ReadFromXml(node->ToElement());

    node = settings_->FirstChild("dleft");
    if ( node )
        mDigitalLeft.ReadFromXml(node->ToElement());

    node = settings_->FirstChild("dright");
    if ( node )
        mDigitalRight.ReadFromXml(node->ToElement());

    node = settings_->FirstChild("dacc");
    if ( node )
        mDigitalAccelerate.ReadFromXml(node->ToElement());

    node = settings_->FirstChild("dbrake");
    if ( node )
        mDigitalBrake.ReadFromXml(node->ToElement());

    node = settings_->FirstChild("pause");
    if ( node )
        mButtonPause.ReadFromXml(node->ToElement());

    node = settings_->FirstChild("camera");
    if ( node )
        mButtonCamera.ReadFromXml(node->ToElement());
}

void ControllerSettings::WriteToXml(TiXmlElement * settings_) const
{
    if ( !settings_ )
        return;

    settings_->Clear();

    TiXmlNode * node = NULL;

    node = settings_->InsertEndChild( TiXmlElement("asteer") );
    if ( node )
        mAnalogSteering.WriteToXml(node->ToElement());

    node = settings_->InsertEndChild( TiXmlElement("aacc") );
    if ( node )
        mAnalogAcceleration.WriteToXml(node->ToElement());

    node = settings_->InsertEndChild( TiXmlElement("abrake") );
    if ( node )
        mAnalogBrake.WriteToXml(node->ToElement());

    node = settings_->InsertEndChild( TiXmlElement("dleft") );
    if ( node )
        mDigitalLeft.WriteToXml(node->ToElement());

    node = settings_->InsertEndChild( TiXmlElement("dright") );
    if ( node )
        mDigitalRight.WriteToXml(node->ToElement());

    node = settings_->InsertEndChild( TiXmlElement("dacc") );
    if ( node )
        mDigitalAccelerate.WriteToXml(node->ToElement());

    node = settings_->InsertEndChild( TiXmlElement("dbrake") );
    if ( node )
        mDigitalBrake.WriteToXml(node->ToElement());

    node = settings_->InsertEndChild( TiXmlElement("pause") );
    if ( node )
        mButtonPause.WriteToXml(node->ToElement());

    node = settings_->InsertEndChild( TiXmlElement("camera") );
    if ( node )
        mButtonCamera.WriteToXml(node->ToElement());
}


// --------------------------------------------------------
Controller::Controller()
{
    Reset();
    MakeTouchSettings(mTouchDeviceSettings);
}

Controller::~Controller()
{
}

void Controller::Reset()
{
    mPower = 0.f;
    mRotation = 0.f;
    mSwitchCamera = 0;
    mPause = false;

    // 2 is false and will avoid chatter
    mMenuUp = 2;
    mMenuDown = 2;
    mMenuLeft = 2;
    mMenuRight = 2;
    mMenuExecute = 2;
}

void Controller::MakeTouchSettings(ControllerSettings& settings)
{
	settings.mAnalogAcceleration.mType = CAT_AXIS;
	settings.mAnalogAcceleration.Axis.mDeviceType = IDP_TOUCH;
	settings.mAnalogAcceleration.Axis.mAxisId = 1;
	settings.mAnalogAcceleration.Axis.mGoofy = false;

	settings.mAnalogBrake.mType = CAT_AXIS;
	settings.mAnalogBrake.Axis.mDeviceType = IDP_TOUCH;
	settings.mAnalogBrake.Axis.mAxisId = 1;
	settings.mAnalogBrake.Axis.mGoofy = true;

	settings.mAnalogSteering.mType = CAT_AXIS;
	settings.mAnalogSteering.Axis.mDeviceType = IDP_TOUCH;
	settings.mAnalogSteering.Axis.mAxisId = 0;
	settings.mAnalogSteering.Axis.mGoofy = false;


	settings.mDigitalLeft.mType = CAT_BUTTONS;
	settings.mDigitalLeft.mDirections = 1;
	settings.mDigitalLeft.Buttons.mDeviceType1 = IDP_TOUCH;
	settings.mDigitalLeft.Buttons.mButtonId1 = ETB_LEFT;
	settings.mDigitalLeft.Buttons.mStepSize1 = 1.f;

	settings.mDigitalRight.mType = CAT_BUTTONS;
	settings.mDigitalRight.Buttons.mDeviceType1 = IDP_TOUCH;
	settings.mDigitalRight.Buttons.mButtonId1 = ETB_RIGHT;
	settings.mDigitalRight.Buttons.mStepSize1 = 1.f;

	settings.mDigitalAccelerate.mType = CAT_BUTTONS;
	settings.mDigitalAccelerate.mDirections = 1;
	settings.mDigitalAccelerate.Buttons.mDeviceType1 = IDP_TOUCH;
	settings.mDigitalAccelerate.Buttons.mButtonId1 = ETB_UP;
	settings.mDigitalAccelerate.Buttons.mStepSize1 = 1.f;

	settings.mDigitalBrake.mType = CAT_BUTTONS;
	settings.mDigitalBrake.mDirections = 1;
	settings.mDigitalBrake.Buttons.mDeviceType1 = IDP_TOUCH;
	settings.mDigitalBrake.Buttons.mButtonId1 = ETB_DOWN;
	settings.mDigitalBrake.Buttons.mStepSize1 = 1.f;

    settings.mButtonPause.mDeviceType = IDP_TOUCH;
    settings.mButtonPause.mButtonId = ETB_PAUSE;
    settings.mButtonCamera.mDeviceType = IDP_TOUCH;;
    settings.mButtonCamera.mButtonId = ETB_CAMERA;
}

const ControllerSettings& Controller::GetActiveSettings(ETouchInput touchInput) const
{
	if ( touchInput == ETI_NO_TOUCH )
		return mUserSettings;
	else
		return mTouchDeviceSettings;
}

void Controller::Update(const InputDeviceManager& idm, ETouchInput touchInput)
{
	const ControllerSettings& settings = GetActiveSettings(touchInput);

    float accelerate = 0.f;
    accelerate = MAX(accelerate, idm.GetAxisValue(settings.mAnalogAcceleration, GetPower()) );
    accelerate = MAX(accelerate, idm.GetAxisValue(settings.mDigitalAccelerate, GetPower()) );
    float brake = 0.f;
    brake = MAX(brake, idm.GetAxisValue(settings.mAnalogBrake, -GetPower()) );
    brake = MAX(brake, idm.GetAxisValue(settings.mDigitalBrake, -GetPower()) );
    SetPower( brake > 0 ? -brake : accelerate );

    float steer = idm.GetAxisValue(settings.mAnalogSteering, GetRotation());
    float left = idm.GetAxisValue(settings.mDigitalLeft, GetRotation());
    float right = idm.GetAxisValue(settings.mDigitalRight, -GetRotation());
    if ( right > 0 )
        steer = MIN( steer, -right );
    if ( left > 0 )
        steer = MAX( steer, left );
    SetRotation( steer );

    SetSwitchCamera( idm.GetButtonDown(settings.mButtonCamera) );
    SetPause( idm.GetButtonDown(settings.mButtonPause) );

    // menu can be controlled by joystick
    if ( settings.mButtonPause.mDeviceType == IDP_JOYSTICK
        && idm.GetButtonDown(settings.mButtonPause))
    {
        ++mMenuExecute;
    }
    else
    {
        mMenuExecute = 0;
    }
    if ( settings.mDigitalLeft.mType == CAT_BUTTONS
        && settings.mDigitalLeft.Buttons.mDeviceType1 == IDP_JOYSTICK
        && idm.GetAxisValue(settings.mDigitalLeft, 0) > 0 )
    {
        ++mMenuLeft;
    }
    else
    {
        mMenuLeft = 0;
    }
    if ( settings.mDigitalRight.mType == CAT_BUTTONS
        &&  settings.mDigitalRight.Buttons.mDeviceType1 == IDP_JOYSTICK
        && idm.GetAxisValue(settings.mDigitalRight, 0) > 0 )
    {
        ++mMenuRight;
    }
    else
    {
        mMenuRight = 0;
    }
    if ( settings.mDigitalAccelerate.mType == CAT_BUTTONS
        &&  settings.mDigitalAccelerate.Buttons.mDeviceType1 == IDP_JOYSTICK
        && idm.GetAxisValue(settings.mDigitalAccelerate, 0) > 0 )
    {
        ++mMenuUp;
    }
    else
    {
        mMenuUp = 0;
    }
    if ( settings.mDigitalBrake.mType == CAT_BUTTONS
        &&  settings.mDigitalBrake.Buttons.mDeviceType1 == IDP_JOYSTICK
        && idm.GetAxisValue(settings.mDigitalBrake, 0) > 0 )
    {
        ++mMenuDown;
    }
    else
    {
        mMenuDown = 0;
    }
}

void Controller::ReadFromXml(const TiXmlElement * settings_)
{
    if ( !settings_ )
        return;

    const TiXmlNode* nodeController = settings_->FirstChild("controller");
    if ( nodeController )
    {
        mUserSettings.ReadFromXml(nodeController->ToElement());
    }
}

void Controller::WriteToXml(TiXmlElement * settings_) const
{
    if ( !settings_ )
        return;

    TiXmlNode* nodeController = settings_->FirstChild("controller");
    if ( !nodeController )
    {
        nodeController = settings_->InsertEndChild( TiXmlElement("controller") );
    }

    if ( nodeController )
    {
        mUserSettings.WriteToXml(nodeController->ToElement());
    }
}
