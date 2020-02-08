// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "compile_config.h"
#include "input_device.h"
#include "config_enums.h"

class TiXmlElement;
class InputDeviceManager;

enum ControllerAxisType
{
    CAT_NONE,
    CAT_BUTTONS,
    CAT_AXIS,
};

struct ControllerAxis
{
    ControllerAxis();
    void ReadFromXml(const TiXmlElement * settings_);
    void WriteToXml(TiXmlElement * settings_) const;

    void ResetAxis();
    void ResetButtons();

    ControllerAxisType mType;   // default is CAT_NONE
    bool        mAllowAnalog;   // default is true
    bool        mAllowDigital;  // default is true
    int         mDirections;    // currently 1 or 2, where 2 is default
    struct
    {
        InputDeviceType mDeviceType1;
        int mJoystickId1;
        int mButtonId1;
        float mStepSize1;	// TODO: looks frame rate dependent to me, not sure what I thought back then.
        InputDeviceType mDeviceType2;
        int mJoystickId2;
        int mButtonId2;
        float mStepSize2;	// TODO: looks frame rate dependent to me, not sure what I thought back then.
    } Buttons;

    struct
    {
        InputDeviceType mDeviceType;
        int mJoystickId;
        int mAxisId;
        bool mGoofy;
    } Axis;
};

struct ControllerButton
{
    ControllerButton();
    void ReadFromXml(const TiXmlElement * settings_);
    void WriteToXml(TiXmlElement * settings_) const;

    InputDeviceType mDeviceType;
    int mJoystickId;
    int mButtonId;
};

struct ControllerSettings
{
    ControllerSettings();

    void ReadFromXml(const TiXmlElement * settings_);
    void WriteToXml(TiXmlElement * settings_) const;

    ControllerAxis      mAnalogSteering;
    ControllerAxis      mAnalogAcceleration;
    ControllerAxis      mAnalogBrake;
    ControllerAxis      mDigitalLeft;
    ControllerAxis      mDigitalRight;
    ControllerAxis      mDigitalAccelerate;
    ControllerAxis      mDigitalBrake;
    ControllerButton    mButtonPause;
    ControllerButton    mButtonCamera;
};

class Controller
{
public:
    Controller();
    ~Controller();

    void Reset();
    void Update(const InputDeviceManager& idm, ETouchInput touchInput);

    // values in range -1 to 1
    void SetPower(float val_)           { mPower = val_; }
    float GetPower() const              { return mPower; }

    // values in range -1 to 1
    void SetRotation(float val_)        { mRotation = val_; }
    float GetRotation() const           { return mRotation; }

    void SetSwitchCamera(bool active_)  { mSwitchCamera = active_ ? mSwitchCamera+1:0; }
    int  GetSwitchCamera()              { return mSwitchCamera; }

    void SetPause(bool active_)         { mPause = active_; }
    bool GetPause() const               { return mPause; }

    bool  GetMenuUp() const             { return mMenuUp == 1; }
    bool  GetMenuDown() const           { return mMenuDown == 1; }
    bool  GetMenuLeft() const           { return mMenuLeft == 1; }
    bool  GetMenuRight() const          { return mMenuRight == 1; }
    bool  GetMenuExecute() const        { return mMenuExecute == 1; }

    void ReadFromXml(const TiXmlElement * settings_);
    void WriteToXml(TiXmlElement * settings_) const;

    void SetUserSettings(const ControllerSettings& settings) { mUserSettings = settings; }
    const ControllerSettings& GetUserSettings() const        { return mUserSettings; }
    ControllerSettings& GetUserSettings()                    { return mUserSettings; }

protected:
	const ControllerSettings& GetActiveSettings(ETouchInput touchInput) const;
	void MakeTouchSettings(ControllerSettings& settings);

private:
	// settings which can be modified by the user
    ControllerSettings mUserSettings;

    // fixed controls for touch-input
	ControllerSettings mTouchDeviceSettings;

    float mPower;
    float mRotation;
    int   mSwitchCamera;    // 0 for not pressed, otherwise the number of updates it was pressed
    bool  mPause;

    int  mMenuUp;
    int  mMenuDown;
    int  mMenuLeft;
    int  mMenuRight;
    int  mMenuExecute;
};

#endif // CONTROLLER_H
