// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#include "gui_menu_analog.h"
#include "gui_menu_options.h"
#include "../main.h"
#include "../gui.h"
#include "../profiles.h"
#include "../string_table.h"
#include "../input_device_manager.h"
#include "../device_joystick.h"
#include "../config.h"
#include "../logging.h"
#include <sstream>

using namespace irr;
using namespace gui;

GuiMenuAnalog::GuiMenuAnalog(const Config& config)
    : GuiDialog(config)
    , mSliderRange(0)
    , mSliderDead(0)
    , mActiveProfile(0)
{
    SetSuppressSceneRendering(true);
}

GuiMenuAnalog::~GuiMenuAnalog()
{
    RemoveFunctors();
}

bool GuiMenuAnalog::Load(const char* filename_, bool reloadLast_)
{
    RemoveFunctors();
    bool ok = GuiDialog::Load(filename_, reloadLast_);
    if ( ok )
    {
        AddGuiEventFunctor( GetIdForName(std::string("id_accept")), new EventFunctor<GuiMenuAnalog>(this, &GuiMenuAnalog::OnButtonAccept) );
        AddGuiEventFunctor( GetIdForName(std::string("id_cancel")), new EventFunctor<GuiMenuAnalog>(this, &GuiMenuAnalog::OnButtonCancel) );
        AddGuiEventFunctor( GetIdForName(std::string("calibrate")), new EventFunctor<GuiMenuAnalog>(this, &GuiMenuAnalog::OnButtonCalibrate) );
        AddGuiEventFunctor( GetIdForName(std::string("range")), new EventFunctor<GuiMenuAnalog>(this, &GuiMenuAnalog::OnSliderRange) );
        AddGuiEventFunctor( GetIdForName(std::string("deadzone")), new EventFunctor<GuiMenuAnalog>(this, &GuiMenuAnalog::OnSliderDead) );

        IGUIElement * root = GetDialogParent();
        if ( !root )
            return false;

        std::string errorMsg("GuiMenuAnalog::Load");
        mActiveProfile = static_cast<IGUIStaticText*>(GetElementByName(root, "act_profile2", errorMsg));
        mSliderRange = static_cast<IGUITextSlider*>(GetElementByName(root, "range", errorMsg));
        if ( mSliderRange )
        {
            mSliderRange->clearTexts();

            for ( int i=0; i<=10; ++i )
            {
				core::stringw resultTxt(i* 10);
				resultTxt += L" %";
                mSliderRange->addText( resultTxt.c_str() );
            }
        }

        mSliderDead = static_cast<IGUITextSlider*>(GetElementByName(root, "deadzone", errorMsg));
        if ( mSliderDead )
        {
            mSliderDead->clearTexts();

            for ( int i=0; i<=10; ++i )
            {
				core::stringw resultTxt(i* 10);
				resultTxt += L" %";
                mSliderDead->addText( resultTxt.c_str() );
            }
        }
    }
    return ok;
}

void GuiMenuAnalog::Show()
{
    if ( mActiveProfile )
    {
        PlayerProfile* profile = APP.GetProfileManager()->GetSelectedProfile();
        if ( profile )
        {
            mActiveProfile->setText( profile->mName.c_str() );
        }
        else
        {
            mActiveProfile->setText( APP.GetStringTable()->Get("id_showactiveprofile").c_str() );
        }
    }

    const IDeviceJoystick* joystick = APP.GetInputDeviceManager()->GetJoystick(0);
    if ( joystick && mSliderDead && mSliderRange )
    {
        int idDead = (int)round ( ((float)joystick->mSettings.mDeadArea/(float)MAX_JOYSTICK_AREA) * (mSliderDead->getNumTexts()-1) );
        mSliderDead->setCurrentTextId(idDead);

        int idRange = (int)round ( ((float)joystick->mSettings.mMaxArea/(float)MAX_JOYSTICK_AREA) * (mSliderRange->getNumTexts()-1) );
        mSliderRange->setCurrentTextId(idRange);
    }

    GuiDialog::Show();
}

void GuiMenuAnalog::RemoveFunctors()
{
    if ( !IsLoaded() )
        return;
    RemoveGuiEventFunctor( GetIdForName(std::string("id_accept")) );
    RemoveGuiEventFunctor( GetIdForName(std::string("id_cancel")) );
    RemoveGuiEventFunctor( GetIdForName(std::string("calibrate")) );
    RemoveGuiEventFunctor( GetIdForName(std::string("range")) );
    RemoveGuiEventFunctor( GetIdForName(std::string("deadzone")) );
}

bool GuiMenuAnalog::OnSliderRange(const irr::SEvent &event_)
{
    if ( event_.GUIEvent.EventType == (gui::EGUI_EVENT_TYPE)EGET_TEXTSLIDER_CHANGED )
    {
        IDeviceJoystick* joystick = APP.GetInputDeviceManager()->GetJoystick(0);
        if ( joystick && mSliderRange )
        {
            float p = (float)mSliderRange->getCurrentTextId() / (mSliderRange->getNumTexts()-1);
            LOG.LogLn(LP_DEBUG, "GuiMenuAnalog::OnSliderRange P: ", p);
            joystick->mSettings.mMaxArea = (int)((float)MAX_JOYSTICK_AREA*p);
            LOG.LogLn(LP_DEBUG, "GuiMenuAnalog::OnSliderRange max: ", joystick->mSettings.mMaxArea);
        }
    }

    return false;
}

bool GuiMenuAnalog::OnSliderDead(const irr::SEvent &event_)
{
    if ( event_.GUIEvent.EventType == (gui::EGUI_EVENT_TYPE)EGET_TEXTSLIDER_CHANGED )
    {
        IDeviceJoystick* joystick = APP.GetInputDeviceManager()->GetJoystick(0);
        if ( joystick && mSliderDead )
        {
            joystick->mSettings.mDeadArea = (int)((float)MAX_JOYSTICK_AREA*(float)mSliderDead->getCurrentTextId() / (mSliderDead->getNumTexts()-1) );
        }
    }

    return false;
}

bool GuiMenuAnalog::OnButtonCalibrate(const irr::SEvent &event_)
{
    if ( event_.GUIEvent.EventType == EGET_BUTTON_CLICKED )
    {
        // currently just using the first joystick
        IDeviceJoystick* joystick = APP.GetInputDeviceManager()->GetJoystick(0);
        if ( joystick )
        {
            joystick->CalibrateCenter();
        }
        GetEnvironment()->setFocus(event_.GUIEvent.Caller);
    }
    return false;
}

bool GuiMenuAnalog::OnButtonAccept(const irr::SEvent &event_)
{
    if ( event_.GUIEvent.EventType == EGET_BUTTON_CLICKED )
    {
        PlayerProfile* profile = APP.GetProfileManager()->GetSelectedProfile();
        const IDeviceJoystick* joystick = APP.GetInputDeviceManager()->GetJoystick(0);
        if ( profile && joystick )
        {
            profile->mJoystickSettings = joystick->mSettings;
            APP.GetProfileManager()->SaveProfiles();
        }

        APP.GetInputDeviceManager()->WriteToXml(APP.GetConfig()->GetInputSettings());

        APP.GetGui()->SetActiveDialog( APP.GetGui()->GetGuiMenuOptions() );
    }

    return false;
}

bool GuiMenuAnalog::OnButtonCancel(const irr::SEvent &event_)
{
    if ( event_.GUIEvent.EventType == EGET_BUTTON_CLICKED )
    {
        APP.GetGui()->PlayOverrideClickSound(GS_CLICK_CANCEL);
        APP.GetInputDeviceManager()->ReadFromXml(APP.GetConfig()->GetInputSettings());
        APP.GetGui()->SetActiveDialog( APP.GetGui()->GetGuiMenuOptions() );
    }

    return false;
}
