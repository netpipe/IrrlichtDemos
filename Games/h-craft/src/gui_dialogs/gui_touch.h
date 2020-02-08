// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#ifndef HC1_GUI_TOUCH_H
#define HC1_GUI_TOUCH_H

#include "../compile_config.h"
#include "../gui_dialog.h"
#include "../device_touch_settings.h"
#include <string>
#include <vector>

enum E_TOUCH_BUTTONS
{
	ETB_CAMERA,
	ETB_PAUSE,
	ETB_UP,
	ETB_DOWN,
	ETB_LEFT,
	ETB_RIGHT
};

// bitfield
enum E_TOUCH_AXES
{
	ETA_HORIZONTAL,
	ETA_VERTICAL,
	ETA_ANY
};

enum E_TOUCH_DLG_STATE
{
	ETDS_PLAY,
	ETDS_SETUP
};

class DeviceTouch;

class GuiTouch : public GuiDialog
{
public:
    GuiTouch(const Config& config);
    ~GuiTouch();

    virtual bool Load(const char* filename_, bool reloadLast_=false);
    virtual void Show();

    void Update(irr::u32 time_, bool enablePauseCamera);

	void SetDialogState(E_TOUCH_DLG_STATE state);

protected:

    bool OnSliderAndroidControl(const irr::SEvent &event_);
    bool OnSliderControlsPos(const irr::SEvent &event_);
	bool OnButtonBack(const irr::SEvent &event_);

	void UpdateControlsPos();
	void SetTouchLayout(ETOUCH_LAYOUT layout);
	irr::core::stringw GetLayoutName(ETOUCH_LAYOUT layout) const;

    void SetTouchPos(float xAxis, float yAxis, ETOUCH_LAYOUT layout);	// value-range [-1;1]
    void UpdateButtonPressed(DeviceTouch & touchDevice, E_TOUCH_BUTTONS button);
	irr::gui::IGUIElement * GetButton(E_TOUCH_BUTTONS button, ETOUCH_LAYOUT layout) const;
	irr::core::recti GetButtonArea(E_TOUCH_BUTTONS button, ETOUCH_LAYOUT layout) const;
	irr::core::recti GetAxisArea(E_TOUCH_AXES axis, ETOUCH_LAYOUT layout) const;

	bool AddButton(irr::gui::IGUIElement * element, ETOUCH_LAYOUT layout, E_TOUCH_BUTTONS type);
	bool AddAxis(irr::gui::IGUIElement * element, ETOUCH_LAYOUT layout, E_TOUCH_AXES type);

	DeviceTouch * GetDeviceTouch() const;

private:

	void RemoveFunctors();

	E_TOUCH_DLG_STATE mDialogState;
	ETOUCH_LAYOUT mTouchLayout;

	struct SButton
	{
		bool HasLayout(ETOUCH_LAYOUT layout) const
		{
			for ( size_t i=0; i< mLayouts.size(); ++i )
				if ( mLayouts[i] == layout || mLayouts[i] == ETL_ANY )
					return true;
			return false;
		}
		irr::gui::IGUIElement * mElement;
		std::vector<ETOUCH_LAYOUT> mLayouts;
		E_TOUCH_BUTTONS mType;
	};
	std::vector<GuiTouch::SButton> mButtons;

	struct SAxis
	{
		bool HasLayout(ETOUCH_LAYOUT layout) const
		{
			for ( size_t i=0; i< mLayouts.size(); ++i )
				if ( mLayouts[i] == layout || mLayouts[i] == ETL_ANY )
					return true;
			return false;
		}
		irr::gui::IGUIElement * mElement;
		std::vector<ETOUCH_LAYOUT> mLayouts;
		E_TOUCH_AXES mType;
	};
	std::vector<GuiTouch::SAxis> mAxes;

	irr::gui::IGUIImage * mImageTouchPosH;
	irr::gui::IGUIImage * mImageTouchPosV;
	irr::core::dimension2di mDimTouchPosHOrig;
	irr::core::dimension2di mDimTouchPosVOrig;

	irr::gui::IGUIElement * mSetupElements;
    irr::gui::IGUITextSlider* mAndroidControlSlider;
    irr::gui::IGUITextSlider* mControlsPosSlider;
    irr::gui::IGUITextSlider* mCushionStrengthSlider; // helper element for physics fine-tuning
    irr::gui::IGUITextSlider* mSlowFallSlider;        // helper element for physics fine-tuning

    irr::gui::IGUIElement* mPosRangeY;		// range in which controls can be moved
    irr::gui::IGUIElement* mLeftControls;	// grouping all input controls on the left
    irr::gui::IGUIElement* mRightControls;	// grouping all input controls on the right

    bool mPauseCameraEnabled;
};

#endif // HC1_GUI_TOUCH_H
