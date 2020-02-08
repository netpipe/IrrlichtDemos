// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#ifndef DEVICE_MOUSE_H
#define DEVICE_MOUSE_H

#include "input_device.h"

#define NUM_BUTTONS_MOUSE 3
#define NUM_AXES_MOUSE 3

class TiXmlElement;

namespace irr
{
    namespace gui
    {
        class ICursorControl;
    }
}

struct DeviceMouseSettings
{
    DeviceMouseSettings();
    void ReadFromXml(const TiXmlElement * settings_);
    void WriteToXml(TiXmlElement * settings_) const;

    float mMaxChange;
};

class DeviceMouse : public IInputDevice
{
public:
    DeviceMouse(irr::gui::ICursorControl * irrlichtCursor_);
    virtual ~DeviceMouse();

    virtual std::string GetTypeName() const;
    virtual std::string GetDeviceName() const;
    virtual InputDeviceType GetType() const;

    virtual void Update();

    virtual int GetNumAxes() const;
    virtual int GetNumButtons() const;

    virtual std::string GetNameAxis(int axis_) const;
    virtual std::string GetNameButton(int button_) const;

    virtual bool IsAnyButtonDown(int & button_) const;
    virtual float IsAnyAxisMoved(int & axis_) const;

    virtual bool IsButtonDown(int button_) const;
    virtual float GetAxisValue(int axis_) const;  // between -1 and 1

    // 0=left, 1=right, 2=middle
    void SetButtonDown(int button_);
    void SetButtonUp(int button_);

    void SetWheel(float val_)   { mWheel = val_; }

    void Reset();

    DeviceMouseSettings mSettings;

private:
    irr::gui::ICursorControl * mIrrlichtCursor;

    float   mWheel;
    float   mOldIrrCursorPos[NUM_AXES_MOUSE];
    float   mAxes[NUM_AXES_MOUSE];
    int     mButtons[NUM_BUTTONS_MOUSE];
};

#endif // DEVICE_MOUSE_H
