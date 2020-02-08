// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#include "gui_touch.h"
#include "gui_helper.h"
#include "../main.h"
#include "../gui.h"
#include "../input_device_manager.h"
#include "../device_touch.h"
#include "../config.h"
#include "../physics.h"
#include "../profiles.h"
#include "../string_table.h"
#include <cmath>

// enable to have some controls for quick fine-tuning of physics cushion settings
// #define FINE_TUNE_CUSHIONS

using namespace irr;
using namespace gui;

GuiTouch::GuiTouch(const Config& config)
    : GuiDialog(config)
    , mDialogState(ETDS_PLAY)
	, mImageTouchPosH(0)
	, mImageTouchPosV(0)
	, mSetupElements(0)
    , mAndroidControlSlider(0)
    , mControlsPosSlider(0)
    , mCushionStrengthSlider(0)
    , mSlowFallSlider(0)
    , mPosRangeY(0)
    , mLeftControls(0)
    , mRightControls(0)
    , mPauseCameraEnabled(true)
{
}

GuiTouch::~GuiTouch()
{
	RemoveFunctors();
}

bool GuiTouch::Load(const char* filename_, bool reloadLast_)
{
	RemoveFunctors();
	mButtons.clear();
	mAxes.clear();

    bool ok = GuiDialog::Load(filename_, reloadLast_);
    if ( ok )
    {
        irr::gui::IGUIEnvironment * env = GetEnvironment();
        if ( !env )
            return false;

        AddGuiEventFunctor( GetIdForName(std::string("touchcontrol")), new EventFunctor<GuiTouch>(this, &GuiTouch::OnSliderAndroidControl) );
        AddGuiEventFunctor( GetIdForName(std::string("controlspos_slider")), new EventFunctor<GuiTouch>(this, &GuiTouch::OnSliderControlsPos) );
		AddGuiEventFunctor( GetIdForName(std::string("btn_back")), new EventFunctor<GuiTouch>(this, &GuiTouch::OnButtonBack) );

        IGUIElement * root = env->getRootGUIElement();
        if ( !root )
            return false;

        std::string errorMsg("GuiTouch::Load");

		irr::gui::IGUIImage * image = 0;
		image = static_cast<IGUIImage*>(GetElementByName(root, "touch_background_right_v", errorMsg));
		AddAxis(image, ETL_ANALOG_V_RIGHT_H_LEFT, ETA_VERTICAL);
		image = static_cast<IGUIImage*>(GetElementByName(root, "touch_background_left_h", errorMsg));
		if ( AddAxis(image, ETL_ANALOG_V_RIGHT_H_LEFT, ETA_HORIZONTAL) )
			mAxes.back().mLayouts.push_back(ETL_DIGITAL_V_RIGHT_ANALOG_H_LEFT);

		image = static_cast<IGUIImage*>(GetElementByName(root, "touch_background_right_h", errorMsg));
		if ( AddAxis(image, ETL_ANALOG_H_RIGHT_V_LEFT, ETA_HORIZONTAL) )
			mAxes.back().mLayouts.push_back(ETL_DIGITAL_V_LEFT_ANALOG_H_RIGHT);
		image = static_cast<IGUIImage*>(GetElementByName(root, "touch_background_left_v", errorMsg));
		AddAxis(image, ETL_ANALOG_H_RIGHT_V_LEFT, ETA_VERTICAL);

		irr::gui::IGUIButton* button = 0;
		button = static_cast<IGUIButton*>(GetElementByName(root, "id_up_r", errorMsg));
		if ( AddButton(button, ETL_DIGITAL_H_LEFT_V_RIGHT, ETB_UP) )
			mButtons.back().mLayouts.push_back(ETL_DIGITAL_V_RIGHT_ANALOG_H_LEFT);
		button = static_cast<IGUIButton*>(GetElementByName(root, "id_down_r", errorMsg));
		if ( AddButton(button , ETL_DIGITAL_H_LEFT_V_RIGHT, ETB_DOWN) )
			mButtons.back().mLayouts.push_back(ETL_DIGITAL_V_RIGHT_ANALOG_H_LEFT);
		button = static_cast<IGUIButton*>(GetElementByName(root, "id_left_l", errorMsg));
		AddButton(button, ETL_DIGITAL_H_LEFT_V_RIGHT, ETB_LEFT);
		button = static_cast<IGUIButton*>(GetElementByName(root, "id_right_l", errorMsg));
		AddButton(button, ETL_DIGITAL_H_LEFT_V_RIGHT, ETB_RIGHT);

		button = static_cast<IGUIButton*>(GetElementByName(root, "id_up_l", errorMsg));
		if ( AddButton(button, ETL_DIGITAL_V_LEFT_H_RIGHT, ETB_UP) )
			mButtons.back().mLayouts.push_back(ETL_DIGITAL_V_LEFT_ANALOG_H_RIGHT);
		button = static_cast<IGUIButton*>(GetElementByName(root, "id_down_l", errorMsg));
		if ( AddButton(button, ETL_DIGITAL_V_LEFT_H_RIGHT, ETB_DOWN) )
			mButtons.back().mLayouts.push_back(ETL_DIGITAL_V_LEFT_ANALOG_H_RIGHT);
		button = static_cast<IGUIButton*>(GetElementByName(root, "id_left_r", errorMsg));
		AddButton(button, ETL_DIGITAL_V_LEFT_H_RIGHT, ETB_LEFT);
		button = static_cast<IGUIButton*>(GetElementByName(root, "id_right_r", errorMsg));
		AddButton(button, ETL_DIGITAL_V_LEFT_H_RIGHT, ETB_RIGHT);

		button = static_cast<IGUIButton*>(GetElementByName(root, "id_switch_camera", errorMsg));
		AddButton(button, ETL_ANY, ETB_CAMERA);
		button = static_cast<IGUIButton*>(GetElementByName(root, "id_pause", errorMsg));
		AddButton(button, ETL_ANY, ETB_PAUSE);

		// Sorry, slightly confusing names - h used for vertical display and v for horizontal because the graphics had been horizontal and vertical lines
		// So not a (real) bug - v and h are switched here.
		mImageTouchPosH = static_cast<IGUIImage*>(GetElementByName(root, "touch_pos_v", errorMsg));
		if ( mImageTouchPosH )
			mDimTouchPosHOrig = mImageTouchPosH->getRelativePosition().getSize();
		mImageTouchPosV = static_cast<IGUIImage*>(GetElementByName(root, "touch_pos_h", errorMsg));
		if ( mImageTouchPosV )
			mDimTouchPosVOrig = mImageTouchPosV->getRelativePosition().getSize();

		mSetupElements = GetElementByName(root, "img1", errorMsg);

        mAndroidControlSlider = static_cast<IGUITextSlider*>(GetElementByName(root, "touchcontrol", errorMsg));
        if (GetConfig().GetUseTouchInput() != ETI_NO_TOUCH && mAndroidControlSlider)
        {
			// Only add controls which allow to play the game so well that you can beat hard ai in hard tracks with it.
			// Unfortunately that wasn't the case for any kind of analog touch controls so far. They are requested a lot
			// but it's no point in giving them to players unless they can be used to win the game.
			mAndroidControlSlider->clearTexts();
			mAndroidControlSlider->addText(GetLayoutName(ETL_DIGITAL_H_LEFT_V_RIGHT));
			mAndroidControlSlider->addText(GetLayoutName(ETL_DIGITAL_V_LEFT_H_RIGHT));
        }

		mControlsPosSlider = static_cast<IGUITextSlider*>(GetElementByName(root, "controlspos_slider", errorMsg));
		if ( mControlsPosSlider )
		{
			mControlsPosSlider->clearTexts();
			mControlsPosSlider->addText( APP.GetStringTable()->Get("id_place_1") );
			mControlsPosSlider->addText( APP.GetStringTable()->Get("id_place_2") );
			mControlsPosSlider->addText( APP.GetStringTable()->Get("id_place_3") );
			mControlsPosSlider->addText( APP.GetStringTable()->Get("id_place_4") );
			mControlsPosSlider->addText( APP.GetStringTable()->Get("id_place_5") );
		}
		mCushionStrengthSlider = static_cast<IGUITextSlider*>(GetElementByName(root, "cushion_strength_slider", errorMsg));
		if ( mCushionStrengthSlider )
		{
			GuiHelper::FillTextSliderWithNumbers(mCushionStrengthSlider, 0, 11, 1, true);
		}
		mSlowFallSlider = static_cast<IGUITextSlider*>(GetElementByName(root, "slow_fall_slider", errorMsg));
		if ( mSlowFallSlider )
		{
			GuiHelper::FillTextSliderWithNumbers(mSlowFallSlider, 0, 5, 1, true);
		}
		mPosRangeY = GetElementByName(root, "pos_range_y", errorMsg);
		mLeftControls = GetElementByName(root, "left_controls", errorMsg);
		mRightControls = GetElementByName(root, "right_controls", errorMsg);
    }
    return ok;
}

void GuiTouch::RemoveFunctors()
{
    if ( !IsLoaded() )
        return;
    RemoveGuiEventFunctor( GetIdForName(std::string("touchcontrol")) );
    RemoveGuiEventFunctor( GetIdForName(std::string("controlspos_slider")) );
    RemoveGuiEventFunctor( GetIdForName(std::string("btn_back")) );
}

DeviceTouch * GuiTouch::GetDeviceTouch() const
{
	if ( APP.GetInputDeviceManager() )
	{
		return static_cast<DeviceTouch *>(APP.GetInputDeviceManager()->GetDevice(IDP_TOUCH));
	}
	return 0;
}

bool GuiTouch::OnButtonBack(const irr::SEvent &event_)
{
    if ( event_.GUIEvent.EventType == EGET_BUTTON_CLICKED )
    {
#ifdef FINE_TUNE_CUSHIONS
		Physics * physics = APP.GetPhysics();
		if ( physics )
		{
			PhysicsSettings& settings = physics->GetSettings();

			if ( mCushionStrengthSlider )
			{
				settings.mCushionFactor = (float)mCushionStrengthSlider->getCurrentTextId() / 10.f;
			}

			if ( mSlowFallSlider )
			{
				settings.mCushionSlowFall = (float)mSlowFallSlider->getCurrentTextId() / 10.f;
			}
			settings.WriteToXml(APP.GetConfig()->GetPhysicsSettings());
		}
#endif

		DeviceTouch * touchDevice = GetDeviceTouch();
		if ( touchDevice )
		{
			DeviceTouchSettings settings = touchDevice->GetSettings();

			if ( mAndroidControlSlider )
			{
				settings.mLayout = (ETOUCH_LAYOUT)mAndroidControlSlider->getCurrentTextId();
			}

			if ( mControlsPosSlider )
			{
				settings.mPlacement =  mControlsPosSlider->getCurrentTextId();
			}

			// set settings and write to config and active profile
			touchDevice->SetSettings(settings);
			APP.GetInputDeviceManager()->WriteToXml(APP.GetConfig()->GetInputSettings());
			PlayerProfile* profile = APP.GetProfileManager()->GetSelectedProfile();
			if ( profile )
			{
				profile->mTouchSettings = settings;
				APP.GetProfileManager()->SaveProfiles();
			}
		}

		Config* config = APP.GetConfig();
		if ( config )
		{
			config->Save();
		}

        APP.GetGui()->PlayOverrideClickSound(GS_CLICK_CANCEL);
        APP.GetGui()->SetActiveDialog( APP.GetGui()->GetPreviousActiveDialog() );
    }

    return false;
}

void GuiTouch::SetDialogState(E_TOUCH_DLG_STATE state)
{
	mDialogState = state;
	if ( mSetupElements )
	{
		mSetupElements->setVisible(mDialogState == ETDS_SETUP );
		mSetupElements->setEnabled(mDialogState == ETDS_SETUP );
	}

	IGUIElement * root = GetEnvironment()->getRootGUIElement();
	if ( root && IsLoaded() )
	{
		mPauseCameraEnabled = mDialogState == ETDS_PLAY;
		std::string errorMsg;
		IGUIElement * ele = GetElementByName(root, "bg_camera", errorMsg);
		if ( ele )
		{
			ele->setVisible(mPauseCameraEnabled);
			ele->setEnabled(mPauseCameraEnabled);
		}
		ele = GetElementByName(root, "id_switch_camera", errorMsg);
		if ( ele )
		{
			ele->setVisible(mPauseCameraEnabled);
			ele->setEnabled(mPauseCameraEnabled);
		}
		ele = GetElementByName(root, "bg_pause", errorMsg);
		if ( ele )
		{
			ele->setVisible(mPauseCameraEnabled);
			ele->setEnabled(mPauseCameraEnabled);
		}
		ele = GetElementByName(root, "id_pause", errorMsg);
		if ( ele )
		{
			ele->setVisible(mPauseCameraEnabled);
			ele->setEnabled(mPauseCameraEnabled);
		}
	}
}

void GuiTouch::Show()
{
    GuiDialog::Show();

	if ( !IsLoaded() )
		return;

    DeviceTouchSettings touchSetttings;
	DeviceTouch * touchDevice = GetDeviceTouch();
	if ( touchDevice )
	{
		touchSetttings = touchDevice->GetSettings();
	}

    if ( mAndroidControlSlider )
    {
		mAndroidControlSlider->setCurrentTextId(touchSetttings.mLayout);
	}

	if ( mControlsPosSlider )
	{
		mControlsPosSlider->setCurrentTextId(touchSetttings.mPlacement);
	}

#ifdef FINE_TUNE_CUSHIONS
	// for fine-tuning cushions in physics
	Config* config = APP.GetConfig();
	Physics * physics = APP.GetPhysics();
	if ( config && physics )
	{
		PhysicsSettings& settings = physics->GetSettings();
		settings.ReadFromXml(config->GetPhysicsSettings());	// ensure up-to-date settings

		if ( mCushionStrengthSlider )
		{
			int f = (int)round(settings.mCushionFactor * 10);
			if ( f >= 0 && f < mCushionStrengthSlider->getNumTexts() )
				mCushionStrengthSlider->setCurrentTextId(f);
			else
				mCushionStrengthSlider->setCurrentTextId(0);
		}

		if ( mSlowFallSlider )
		{
			int f = (int)round(settings.mCushionSlowFall * 10);
			if ( f >= 0 && f < mSlowFallSlider->getNumTexts() )
				mSlowFallSlider->setCurrentTextId(f);
			else
				mSlowFallSlider->setCurrentTextId(0);
		}
	}
#else
	IGUIElement * root = GetEnvironment()->getRootGUIElement();
	gui::IGUIElement * ele = GetElementByName(root, "cushion_strength_text");
	if ( ele )
		ele->setVisible(false);
	ele = GetElementByName(root, "slow_fall_text");
	if ( ele )
		ele->setVisible(false);
	if (mCushionStrengthSlider )
		mCushionStrengthSlider->setVisible(false);
	if ( mSlowFallSlider )
		mSlowFallSlider->setVisible(false);
#endif

	SetTouchLayout( touchSetttings.mLayout );
	UpdateControlsPos();

	SetDialogState(mDialogState);

	if ( mPosRangeY )
		mPosRangeY->setVisible(false);
}

void GuiTouch::UpdateButtonPressed(DeviceTouch & touchDevice, E_TOUCH_BUTTONS button)
{
	gui::IGUIElement * element = GetButton(button, touchDevice.GetSettings().mLayout);
	if ( element && element->getType() == irr::gui::EGUIET_BUTTON  )
		static_cast<irr::gui::IGUIButton*>(element)->setPressed( touchDevice.IsButtonDown(button) );
}

void GuiTouch::Update(u32 time_, bool enablePauseCamera)
{
	DeviceTouch * touchDevice = GetDeviceTouch();
	if ( touchDevice )
	{
		SetTouchPos( touchDevice->GetAxisValue((int)DeviceTouch::E_HORIZONTAL),
					 touchDevice->GetAxisValue((int)DeviceTouch::E_VERTICAL),
					 touchDevice->GetSettings().mLayout );

		// Irrlicht only gives one button the focus. So if we want multiple buttons to be pressed at the same time
		// we have to enforce it for those.
		UpdateButtonPressed(*touchDevice, ETB_UP);
		UpdateButtonPressed(*touchDevice, ETB_DOWN);
		UpdateButtonPressed(*touchDevice, ETB_LEFT);
		UpdateButtonPressed(*touchDevice, ETB_RIGHT);

		if ( enablePauseCamera != mPauseCameraEnabled )
		{
			mPauseCameraEnabled = enablePauseCamera;
			ETOUCH_LAYOUT layout = touchDevice->GetSettings().mLayout;
			irr::gui::IGUIElement * ele = GetButton(ETB_CAMERA, layout);
			if ( ele  )
				ele->setEnabled(mPauseCameraEnabled);
			ele = GetButton(ETB_PAUSE, layout);
			if ( ele  )
				ele->setEnabled(mPauseCameraEnabled);
		}

	}
}

bool GuiTouch::AddButton(irr::gui::IGUIElement * element, ETOUCH_LAYOUT layout, E_TOUCH_BUTTONS type)
{
	if ( element )
	{
		SButton button;
		button.mElement = element;
		button.mLayouts.push_back(layout);
		button.mType = type;
		mButtons.push_back(button);
		return true;
	}
	return false;
}

bool GuiTouch::AddAxis(irr::gui::IGUIElement * element, ETOUCH_LAYOUT layout, E_TOUCH_AXES type)
{
	if ( element )
	{
		SAxis axis;
		axis.mElement = element;
		axis.mLayouts.push_back(layout);
		axis.mType = type;
		mAxes.push_back(axis);
		return true;
	}
	return false;
}

void GuiTouch::SetTouchLayout(ETOUCH_LAYOUT layout)
{
	for ( size_t i=0; i < mButtons.size(); ++i )
	{
		if ( mDialogState == ETDS_SETUP && (mButtons[i].mType == ETB_CAMERA  || mButtons[i].mType == ETB_PAUSE) )
			mButtons[i].mElement->setVisible(false);
		else
			mButtons[i].mElement->setVisible( mButtons[i].HasLayout(layout) );
	}

	for ( size_t i=0; i < mAxes.size(); ++i )
	{
		mAxes[i].mElement->setVisible( mAxes[i].HasLayout(layout) );
	}

	DeviceTouch * touchDevice = GetDeviceTouch();
	if ( touchDevice )
	{
		touchDevice->removeAllButtons();
		touchDevice->removeAllAxes();

		ETOUCH_LAYOUT layout = touchDevice->GetSettings().mLayout;

		// Names not from stringtable as they don't matter in this case.
		touchDevice->addButton(GetButtonArea(ETB_CAMERA, layout), std::string("BUTTON_CAMERA"));
		touchDevice->addButton(GetButtonArea(ETB_PAUSE, layout), std::string("BUTTON_PAUSE"));
		touchDevice->addButton(GetButtonArea(ETB_UP, layout), std::string("BUTTON_UP"));
		touchDevice->addButton(GetButtonArea(ETB_DOWN, layout), std::string("BUTTON_DOWN"));
		touchDevice->addButton(GetButtonArea(ETB_LEFT, layout), std::string("BUTTON_LEFT"));
		touchDevice->addButton(GetButtonArea(ETB_RIGHT, layout), std::string("BUTTON_RIGHT"));
		touchDevice->addAxis(GetAxisArea(ETA_HORIZONTAL, layout), std::string("AXIS-H"), DeviceTouch::E_HORIZONTAL);
		touchDevice->addAxis(GetAxisArea(ETA_VERTICAL, layout), std::string("AXIS-V"), DeviceTouch::E_VERTICAL);
	}

	if ( mImageTouchPosV )
		mImageTouchPosV->setVisible(false);
	if ( mImageTouchPosH )
		mImageTouchPosH->setVisible(false);
}

irr::core::stringw GuiTouch::GetLayoutName(ETOUCH_LAYOUT layout) const
{
	switch ( layout )
	{
		case ETL_DIGITAL_H_LEFT_V_RIGHT: 	    return APP.GetStringTable()->Get("id_digi_rh");
		case ETL_DIGITAL_V_LEFT_H_RIGHT: 	    return APP.GetStringTable()->Get("id_digi_lh");
		case ETL_DIGITAL_V_RIGHT_ANALOG_H_LEFT: return APP.GetStringTable()->Get("id_digi_pads_rh");
		case ETL_DIGITAL_V_LEFT_ANALOG_H_RIGHT: return APP.GetStringTable()->Get("id_digi_pads_lh");
		case ETL_ANALOG_V_RIGHT_H_LEFT: 	    return APP.GetStringTable()->Get("id_pads_rh");
		case ETL_ANALOG_H_RIGHT_V_LEFT: 	    return APP.GetStringTable()->Get("id_pads_lh");
		default: return core::stringw(L"UNKNOWN");
	}
}

irr::gui::IGUIElement * GuiTouch::GetButton(E_TOUCH_BUTTONS type, ETOUCH_LAYOUT layout) const
{
	for ( size_t i=0; i < mButtons.size(); ++i )
	{
		if ( mButtons[i].mType == type && mButtons[i].HasLayout(layout) )
		{
			return mButtons[i].mElement;
		}
	}
	return 0;
}

irr::core::recti GuiTouch::GetButtonArea(E_TOUCH_BUTTONS type, ETOUCH_LAYOUT layout) const
{
	gui::IGUIElement * btn = GetButton(type, layout);
	if ( btn )
	{
		return btn->getAbsolutePosition();
	}

	return 	irr::core::recti(0,0,0,0);
}

irr::core::recti GuiTouch::GetAxisArea(E_TOUCH_AXES type, ETOUCH_LAYOUT layout) const
{
	for ( size_t i=0; i < mAxes.size(); ++i )
	{
		if ( (mAxes[i].mType == type || mAxes[i].mType == ETA_ANY) && mAxes[i].HasLayout(layout) )
		{
			return mAxes[i].mElement->getAbsolutePosition();
		}
	}

	return 	irr::core::recti(0,0,0,0);
}

// experiment in visualizing current controls
void GuiTouch::SetTouchPos(float xAxis, float yAxis, ETOUCH_LAYOUT layout)
{
    irr::video::IVideoDriver* driver = GetDriver();
    if ( !driver )
		return;

	bool useFullScreenArea = false;	// TODO: make it a parameter
    core::dimension2d<s32> dimScreen( driver->getScreenSize() );

	if ( mImageTouchPosH )
	{
		core::recti areaH(GetAxisArea(ETA_HORIZONTAL, layout));
		if ( areaH.getArea() > 0)
		{
			if ( useFullScreenArea )
			{
				areaH = core::recti(0,0,dimScreen.Width,dimScreen.Height);
			}

			float scale = (float)areaH.getWidth() / (float)dimScreen.Width;
			s32 markerWidth = (s32)round((float)mDimTouchPosHOrig.Width * scale);
			if ( markerWidth < 1 )
				markerWidth = 1;

			s32 radius = (areaH.getWidth() - markerWidth) / 2;
			s32 p = radius - (s32)(xAxis * (float)radius);
			areaH.UpperLeftCorner.X += p;
			areaH.LowerRightCorner.X = areaH.UpperLeftCorner.X + markerWidth;
			mImageTouchPosH->setRelativePosition(areaH);
			mImageTouchPosH->setVisible(true);
		}
		else
		{
			mImageTouchPosH->setVisible(false);
		}
	}
	if ( mImageTouchPosV )
	{
		core::recti areaV(GetAxisArea(ETA_VERTICAL, layout));
		if (areaV.getArea() > 0)
		{
			if ( useFullScreenArea )
			{
				areaV = core::recti(0,0,dimScreen.Width,dimScreen.Height);
			}

			float scale = (float)areaV.getHeight() / (float)dimScreen.Height;
			s32 markerHeight = (s32)round((float)mDimTouchPosVOrig.Height * scale);
			if ( markerHeight < 1 )
				markerHeight = 1;

			s32 radius = (areaV.getHeight() - markerHeight) / 2;
			s32 p = radius - (s32)(yAxis * (float)radius);
			areaV.UpperLeftCorner.Y += p;
			areaV.LowerRightCorner.Y = areaV.UpperLeftCorner.Y+markerHeight;
			mImageTouchPosV->setRelativePosition(areaV);
			mImageTouchPosV->setVisible(true);
		}
		else
		{
			mImageTouchPosV->setVisible(false);
		}
	}
}

bool GuiTouch::OnSliderAndroidControl(const irr::SEvent &event_)
{
    if ( event_.GUIEvent.EventType != (gui::EGUI_EVENT_TYPE)EGET_TEXTSLIDER_CHANGED )
        return false;

	if ( mAndroidControlSlider )
	{
		SetTouchLayout((ETOUCH_LAYOUT)mAndroidControlSlider->getCurrentTextId());
	}

	return false;
}

void GuiTouch::UpdateControlsPos()
{
	if ( mControlsPosSlider )
	{
		int pos = mControlsPosSlider->getCurrentTextId();
		f32 maxPos = (f32)mControlsPosSlider->getNumTexts();

		if ( mPosRangeY && maxPos > 1 )
		{
			core::rect<s32> rangePos = mPosRangeY->getRelativePosition();
			if ( mLeftControls )
			{
				core::rect<s32> controlsPos = mLeftControls->getRelativePosition();
				s32 controlsHeight = controlsPos.getHeight();
				s32 stepSize = (s32)((rangePos.getHeight() - controlsHeight) / (maxPos-1.f));
				controlsPos.UpperLeftCorner.Y = rangePos.UpperLeftCorner.Y + stepSize * pos;
				controlsPos.LowerRightCorner.Y = controlsPos.UpperLeftCorner.Y + controlsHeight;
				mLeftControls->setRelativePosition(controlsPos);
			}
			if ( mRightControls )
			{
				core::rect<s32> controlsPos = mRightControls->getRelativePosition();
				s32 controlsHeight = controlsPos.getHeight();
				s32 stepSize = (s32)((rangePos.getHeight() - controlsHeight) / (maxPos-1.f));
				controlsPos.UpperLeftCorner.Y = rangePos.UpperLeftCorner.Y + stepSize * pos;
				controlsPos.LowerRightCorner.Y = controlsPos.UpperLeftCorner.Y + controlsHeight;
				mRightControls->setRelativePosition(controlsPos);
			}
		}
	}
}

bool GuiTouch::OnSliderControlsPos(const irr::SEvent &event_)
{
    if ( event_.GUIEvent.EventType != (gui::EGUI_EVENT_TYPE)EGET_TEXTSLIDER_CHANGED )
        return false;

	UpdateControlsPos();

	return false;
}
