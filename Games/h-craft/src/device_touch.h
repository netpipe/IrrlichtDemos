// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#ifndef DEVICE_TOUCH_H
#define DEVICE_TOUCH_H

#include "compile_config.h"
#include "input_device.h"
#include "device_touch_settings.h"
#include "config_enums.h"
#include <irrlicht.h>
#include <vector>

class TiXmlElement;

class DeviceTouch : public IInputDevice
{
public:

	enum EDirection
	{
		E_HORIZONTAL,
		E_VERTICAL
	};

    DeviceTouch();
    virtual ~DeviceTouch();

    virtual std::string GetTypeName() const;
    virtual std::string GetDeviceName() const;
    virtual InputDeviceType GetType() const;

    virtual void Update();
    virtual void Reset();

    virtual int GetNumAxes() const;
    virtual int GetNumButtons() const;

    virtual std::string GetNameAxis(int axis_) const;
    virtual std::string GetNameButton(int button_) const;

    virtual bool IsAnyButtonDown(int & button_) const;
    virtual float IsAnyAxisMoved(int & axis_) const;

    virtual bool IsButtonDown(int button_) const;
    virtual float GetAxisValue(int axis_) const;  // between -1 and 1

	void OnEvent(const irr::SEvent &event, ETouchInput touchInput);

	void addButton(const irr::core::recti& area, const std::string& name);
	void removeAllButtons();

	void addAxis(const irr::core::recti& area, const std::string& name, EDirection dir);
	void removeAllAxes();

	void SetSettings(const DeviceTouchSettings& settings) { mSettings = settings; }
	const DeviceTouchSettings& GetSettings() const        { return  mSettings; }

protected:
    void ResetValues();

private:

	DeviceTouchSettings mSettings;

	// getting touch state information out of the events
	struct STouch
	{
        size_t mID;
		irr::s32 mX;
		irr::s32 mY;
	};
	std::vector<STouch> mTouches;

	struct SButton
	{
		irr::core::recti mArea;
		std::string mName;
		bool mButtonDown;
	};
	std::vector<SButton> mButtons;

	struct SAxis
	{
		irr::core::position2di mAreaCenter;
		irr::s32 mAreaRadiusX;
		irr::s32 mAreaRadiusY;
		std::string mName;
		EDirection mDirection;
		float mValue;
		float mOldValue;
	};
	std::vector<SAxis> mAxes;
};

#endif // DEVICE_TOUCH_H
