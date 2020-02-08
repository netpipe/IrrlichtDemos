// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#include "gui_menu_controls.h"
#include "gui_menu_options.h"
#include "../main.h"
#include "../gui.h"
#include "../input_device_manager.h"
#include "../device_joystick.h"
#include "../controller.h"
#include "../device_mouse.h"
#include "../config.h"
#include "../profiles.h"
#include "../string_table.h"
#include "../sound.h"

using namespace irr;
using namespace gui;

GuiMenuControls::GuiMenuControls(const Config& config)
: GuiDialog(config)
    ,mButtonAnalogSteer(0)
    ,mButtonAnalogAccelerate(0)
    ,mButtonAnalogBrake(0)
    ,mButtonDigiLeft(0)
    ,mButtonDigiRight(0)
    ,mButtonDigiAccelerate(0)
    ,mButtonDigiBrake(0)
    ,mButtonPause(0)
    ,mButtonCamera(0)
    ,mActiveProfile(0)
    ,mStaticWaiting(0)
    ,mStaticAbort(0)
    ,mImageInsert(0)
{
    SetSuppressSceneRendering(true);
}

GuiMenuControls::~GuiMenuControls()
{
    RemoveFunctors();
}

bool GuiMenuControls::Load(const char* filename_, bool reloadLast_)
{
    RemoveFunctors();
    bool ok = GuiDialog::Load(filename_, reloadLast_);
    if ( ok )
    {
        mSoundClick = APP.GetConfig()->MakeFilenameSound("hc_menu_click01.wav");

        AddGuiEventFunctor( GetIdForName(std::string("id_accept")), new EventFunctor<GuiMenuControls>(this, &GuiMenuControls::OnButtonAccept) );
        AddGuiEventFunctor( GetIdForName(std::string("id_cancel")), new EventFunctor<GuiMenuControls>(this, &GuiMenuControls::OnButtonCancel) );
        AddGuiEventFunctor( GetIdForName(std::string("analogsteer")), new EventFunctor<GuiMenuControls>(this, &GuiMenuControls::OnButtonAnalogSteer) );
        AddGuiEventFunctor( GetIdForName(std::string("analogacc")), new EventFunctor<GuiMenuControls>(this, &GuiMenuControls::OnButtonAnalogAccelerate) );
        AddGuiEventFunctor( GetIdForName(std::string("analogbrake")), new EventFunctor<GuiMenuControls>(this, &GuiMenuControls::OnButtonAnalogBrake) );
        AddGuiEventFunctor( GetIdForName(std::string("steerleft")), new EventFunctor<GuiMenuControls>(this, &GuiMenuControls::OnButtonDigiLeft) );
        AddGuiEventFunctor( GetIdForName(std::string("steerright")), new EventFunctor<GuiMenuControls>(this, &GuiMenuControls::OnButtonDigiRight) );
        AddGuiEventFunctor( GetIdForName(std::string("accelerate")), new EventFunctor<GuiMenuControls>(this, &GuiMenuControls::OnButtonDigiAccelerate) );
        AddGuiEventFunctor( GetIdForName(std::string("brake")), new EventFunctor<GuiMenuControls>(this, &GuiMenuControls::OnButtonDigiBrake) );
        AddGuiEventFunctor( GetIdForName(std::string("pause")), new EventFunctor<GuiMenuControls>(this, &GuiMenuControls::OnButtonPause) );
        AddGuiEventFunctor( GetIdForName(std::string("camera")), new EventFunctor<GuiMenuControls>(this, &GuiMenuControls::OnButtonCamera) );

        IGUIElement * root = GetDialogParent();
        if ( !root )
            return false;

        std::string errorMsg("GuiMenuControls::Load");

        mActiveProfile = static_cast<IGUIStaticText*>(GetElementByName(root, "act_profile2", errorMsg));
        mButtonAnalogSteer = static_cast<IGUIButton*>(GetElementByName(root, "analogsteer", errorMsg));
        mButtonAnalogAccelerate = static_cast<IGUIButton*>(GetElementByName(root, "analogacc", errorMsg));
        mButtonAnalogBrake = static_cast<IGUIButton*>(GetElementByName(root, "analogbrake", errorMsg));
        mButtonDigiLeft = static_cast<IGUIButton*>(GetElementByName(root, "steerleft", errorMsg));
        mButtonDigiRight = static_cast<IGUIButton*>(GetElementByName(root, "steerright", errorMsg));
        mButtonDigiAccelerate = static_cast<IGUIButton*>(GetElementByName(root, "accelerate", errorMsg));
        mButtonDigiBrake = static_cast<IGUIButton*>(GetElementByName(root, "brake", errorMsg));
        mButtonPause = static_cast<IGUIButton*>(GetElementByName(root, "pause", errorMsg));
        mButtonCamera = static_cast<IGUIButton*>(GetElementByName(root, "camera", errorMsg));

        mStaticWaiting = static_cast<IGUIStaticText*>(GetElementByName(root, "id_waiting", errorMsg));
        mStaticAbort = static_cast<IGUIStaticText*>(GetElementByName(root, "id_abort", errorMsg));
        mImageInsert = static_cast<IGUIImage*>(GetElementByName(root, "insert", errorMsg));
    }
    return ok;
}

void GuiMenuControls::Show()
{
    GuiDialog::Show();

    SetWaitingScreen(false);
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

    Controller * controller = APP.GetController();
    if ( !controller )
        return;

	const ControllerSettings& controllerSettings = controller->GetUserSettings();

    std::wstring name;
    if ( mButtonAnalogSteer )
    {
        APP.GetInputDeviceManager()->GetNameForAxis(controllerSettings.mAnalogSteering, /*up_*/ true, name);
        mButtonAnalogSteer->setText( name.c_str() );
    }
    if ( mButtonAnalogAccelerate )
    {
        APP.GetInputDeviceManager()->GetNameForAxis(controllerSettings.mAnalogAcceleration, /*up_*/ true, name);
        mButtonAnalogAccelerate->setText( name.c_str() );
    }
    if ( mButtonAnalogBrake )
    {
        APP.GetInputDeviceManager()->GetNameForAxis(controllerSettings.mAnalogBrake, /*up_*/ true, name);
        mButtonAnalogBrake->setText( name.c_str() );
    }
    if ( mButtonDigiLeft )
    {
        APP.GetInputDeviceManager()->GetNameForAxis(controllerSettings.mDigitalLeft, /*up_*/ true, name);
        mButtonDigiLeft->setText( name.c_str() );
    }
    if ( mButtonDigiRight )
    {
        APP.GetInputDeviceManager()->GetNameForAxis(controllerSettings.mDigitalRight, /*up_*/ true, name);
        mButtonDigiRight->setText( name.c_str() );
    }
    if ( mButtonDigiAccelerate )
    {
        APP.GetInputDeviceManager()->GetNameForAxis(controllerSettings.mDigitalAccelerate, /*up_*/ true, name);
        mButtonDigiAccelerate->setText( name.c_str() );
    }
    if ( mButtonDigiBrake )
    {
        APP.GetInputDeviceManager()->GetNameForAxis(controllerSettings.mDigitalBrake, /*up_*/ true, name);
        mButtonDigiBrake->setText( name.c_str() );
    }
    if ( mButtonPause )
    {
        APP.GetInputDeviceManager()->GetNameForButton(controllerSettings.mButtonPause, name);
        mButtonPause->setText( name.c_str() );
    }
    if ( mButtonCamera )
    {
        APP.GetInputDeviceManager()->GetNameForButton(controllerSettings.mButtonCamera, name);
        mButtonCamera->setText( name.c_str() );
    }
}

void GuiMenuControls::RemoveFunctors()
{
    if ( !IsLoaded() )
        return;

    RemoveGuiEventFunctor( GetIdForName(std::string("id_accept")) );
    RemoveGuiEventFunctor( GetIdForName(std::string("id_cancel")) );
    RemoveGuiEventFunctor( GetIdForName(std::string("analogsteer")) );
    RemoveGuiEventFunctor( GetIdForName(std::string("analogacc")) );
    RemoveGuiEventFunctor( GetIdForName(std::string("analogbrake")) );
    RemoveGuiEventFunctor( GetIdForName(std::string("steerleft")) );
    RemoveGuiEventFunctor( GetIdForName(std::string("steerright")) );
    RemoveGuiEventFunctor( GetIdForName(std::string("accelerate")) );
    RemoveGuiEventFunctor( GetIdForName(std::string("brake")) );
    RemoveGuiEventFunctor( GetIdForName(std::string("pause")) );
    RemoveGuiEventFunctor( GetIdForName(std::string("camera")) );
}

bool GuiMenuControls::OnButtonAccept(const irr::SEvent &event_)
{
    if ( event_.GUIEvent.EventType == EGET_BUTTON_CLICKED )
    {
        PlayerProfile* profile = APP.GetProfileManager()->GetSelectedProfile();
        Controller* controller = APP.GetController();
        if ( profile && controller )
        {
            profile->mControllerSettings = controller->GetUserSettings();
            APP.GetProfileManager()->SaveProfiles();
        }
        APP.GetController()->WriteToXml(APP.GetConfig()->GetInputSettings());

        APP.GetGui()->SetActiveDialog( APP.GetGui()->GetGuiMenuOptions() );
    }

    return false;
}

bool GuiMenuControls::OnButtonCancel(const irr::SEvent &event_)
{
    if ( event_.GUIEvent.EventType == EGET_BUTTON_CLICKED )
    {
        APP.GetController()->ReadFromXml(APP.GetConfig()->GetInputSettings());

        APP.GetGui()->PlayOverrideClickSound(GS_CLICK_CANCEL);
        APP.GetGui()->SetActiveDialog( APP.GetGui()->GetGuiMenuOptions() );
    }

    return false;
}

void GuiMenuControls::RequestAxisInputFromUser(ControllerAxis& axis_, irr::gui::IGUIElement* displayElement)
{
    SetWaitingScreen(true);
    IDeviceJoystick* joystick = APP.GetInputDeviceManager()->GetJoystick(0);
    if ( joystick )
    {
        joystick->CalibrateCenter();
    }

    bool wasMouseEnabled = APP.GetInputDeviceManager()->GetMouse()->IsEnabled();
    APP.GetInputDeviceManager()->GetMouse()->SetEnabled(false);
    bool result = APP.GetInputDeviceManager()->WaitForAxis( axis_, /*up_*/ true );
    APP.GetInputDeviceManager()->GetMouse()->SetEnabled(wasMouseEnabled);
    if ( result )
    {
		std::wstring name;
        APP.GetInputDeviceManager()->GetNameForAxis(axis_, /*up_*/ true, name);
        if ( displayElement )
			displayElement->setText(name.c_str());
        if ( APP.GetSound() )
            APP.GetSound()->PlaySample( APP.GetConfig()->MakeFilenameSound("hc_menu_confirm01.wav").c_str(), 1.f, false );
    }
    else
    {
        if ( APP.GetSound() )
            APP.GetSound()->PlaySample( APP.GetConfig()->MakeFilenameSound("hc_menu_deny01.wav").c_str(), 1.f, false );
    }
    SetWaitingScreen(false);
    APP.GetController()->Reset(); // to avoid chatter
}

void GuiMenuControls::RequestButtonInputFromUser(ControllerButton& button_, irr::gui::IGUIElement* displayElement)
{
    SetWaitingScreen(true);
    bool result = APP.GetInputDeviceManager()->WaitForButton( button_ );
    if ( result )
    {
		std::wstring name;
        APP.GetInputDeviceManager()->GetNameForButton(button_, name);
        if ( displayElement )
			displayElement->setText(name.c_str());
        if ( APP.GetSound() )
            APP.GetSound()->PlaySample( APP.GetConfig()->MakeFilenameSound("hc_menu_confirm01.wav").c_str(), 1.f, false );
    }
    else
    {
        if ( APP.GetSound() )
            APP.GetSound()->PlaySample( APP.GetConfig()->MakeFilenameSound("hc_menu_deny01.wav").c_str(), 1.f, false );
    }
    SetWaitingScreen(false);
    APP.GetController()->Reset(); // to avoid chatter
}

bool GuiMenuControls::OnButtonAnalogSteer(const irr::SEvent &event_)
{
    if ( event_.GUIEvent.EventType == EGET_BUTTON_CLICKED )
    {
        APP.GetGui()->PlayOverrideClickSound(GS_FOCUS);
        Controller * controller = APP.GetController();
		if ( controller )
		{
			RequestAxisInputFromUser(controller->GetUserSettings().mAnalogSteering, mButtonAnalogSteer);
		}
        GetEnvironment()->setFocus(event_.GUIEvent.Caller);
    }

    return false;
}

bool GuiMenuControls::OnButtonAnalogAccelerate(const irr::SEvent &event_)
{
    if ( event_.GUIEvent.EventType == EGET_BUTTON_CLICKED )
    {
        APP.GetGui()->PlayOverrideClickSound(GS_FOCUS);
        Controller * controller = APP.GetController();
        if ( controller )
        {
			RequestAxisInputFromUser(controller->GetUserSettings().mAnalogAcceleration, mButtonAnalogAccelerate);
        }
        GetEnvironment()->setFocus(event_.GUIEvent.Caller);
    }

    return false;
}

bool GuiMenuControls::OnButtonAnalogBrake(const irr::SEvent &event_)
{
    if ( event_.GUIEvent.EventType == EGET_BUTTON_CLICKED )
    {
        APP.GetGui()->PlayOverrideClickSound(GS_FOCUS);
        Controller * controller = APP.GetController();
        if ( controller )
        {
			RequestAxisInputFromUser(controller->GetUserSettings().mAnalogBrake, mButtonAnalogBrake);
        }
        GetEnvironment()->setFocus(event_.GUIEvent.Caller);
    }

    return false;
}

bool GuiMenuControls::OnButtonDigiLeft(const irr::SEvent &event_)
{
    if ( event_.GUIEvent.EventType == EGET_BUTTON_CLICKED )
    {
        APP.GetGui()->PlayOverrideClickSound(GS_FOCUS);
        Controller * controller = APP.GetController();
        if ( controller )
        {
			RequestAxisInputFromUser(controller->GetUserSettings().mDigitalLeft, mButtonDigiLeft);
        }
        GetEnvironment()->setFocus(event_.GUIEvent.Caller);
    }

    return false;
}

bool GuiMenuControls::OnButtonDigiRight(const irr::SEvent &event_)
{
    if ( event_.GUIEvent.EventType == EGET_BUTTON_CLICKED )
    {
        APP.GetGui()->PlayOverrideClickSound(GS_FOCUS);
        Controller * controller = APP.GetController();
        if ( controller )
        {
			RequestAxisInputFromUser(controller->GetUserSettings().mDigitalRight, mButtonDigiRight);
        }
        GetEnvironment()->setFocus(event_.GUIEvent.Caller);
    }

    return false;
}

bool GuiMenuControls::OnButtonDigiAccelerate(const irr::SEvent &event_)
{
    if ( event_.GUIEvent.EventType == EGET_BUTTON_CLICKED )
    {
        APP.GetGui()->PlayOverrideClickSound(GS_FOCUS);
        Controller * controller = APP.GetController();
        if ( controller )
        {
			RequestAxisInputFromUser(controller->GetUserSettings().mDigitalAccelerate, mButtonDigiAccelerate);
        }
        GetEnvironment()->setFocus(event_.GUIEvent.Caller);
    }

    return false;
}

bool GuiMenuControls::OnButtonDigiBrake(const irr::SEvent &event_)
{
    if ( event_.GUIEvent.EventType == EGET_BUTTON_CLICKED )
    {
        APP.GetGui()->PlayOverrideClickSound(GS_FOCUS);
        std::wstring name;
        Controller * controller = APP.GetController();
        if ( controller )
        {
			RequestAxisInputFromUser(controller->GetUserSettings().mDigitalBrake, mButtonDigiBrake);
        }
        GetEnvironment()->setFocus(event_.GUIEvent.Caller);
    }

    return false;
}

bool GuiMenuControls::OnButtonPause(const irr::SEvent &event_)
{
    if ( event_.GUIEvent.EventType == EGET_BUTTON_CLICKED )
    {
        APP.GetGui()->PlayOverrideClickSound(GS_FOCUS);
        Controller * controller = APP.GetController();
        if ( controller )
        {
			RequestButtonInputFromUser(controller->GetUserSettings().mButtonPause, mButtonPause);
        }
        GetEnvironment()->setFocus(event_.GUIEvent.Caller);
    }

    return false;
}

bool GuiMenuControls::OnButtonCamera(const irr::SEvent &event_)
{
    if ( event_.GUIEvent.EventType == EGET_BUTTON_CLICKED )
    {
        APP.GetGui()->PlayOverrideClickSound(GS_FOCUS);
        Controller * controller = APP.GetController();
        if ( controller )
        {
			RequestButtonInputFromUser(controller->GetUserSettings().mButtonCamera, mButtonCamera);
        }
        GetEnvironment()->setFocus(event_.GUIEvent.Caller);
    }

    return false;
}

void GuiMenuControls::SetWaitingScreen(bool enable_)
{
    if ( !mStaticWaiting || !mStaticAbort || !mImageInsert )
        return;

    mStaticWaiting->setVisible(enable_);
    mStaticAbort->setVisible(enable_);
    mImageInsert->setVisible(enable_);

    enable_ = enable_ ? false : true;
    mButtonAnalogSteer->setEnabled(enable_);
    mButtonAnalogAccelerate->setEnabled(enable_);
    mButtonAnalogBrake->setEnabled(enable_);
    mButtonDigiLeft->setEnabled(enable_);
    mButtonDigiRight->setEnabled(enable_);
    mButtonDigiAccelerate->setEnabled(enable_);
    mButtonDigiBrake->setEnabled(enable_);
    mButtonPause->setEnabled(enable_);
    mButtonCamera->setEnabled(enable_);
}
