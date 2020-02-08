// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#include "gui_menu_profiles.h"
#include "gui_menu_main.h"
#include "gui_menu_newprofile.h"
#include "gui_dlg_okcancel.h"
#include "../gui.h"
#include "../main.h"
#include "../profiles.h"
#include "../string_table.h"

using namespace irr;
using namespace gui;

GuiMenuProfiles::GuiMenuProfiles(const Config& config)
    : GuiDialog(config)
    , mSliderProfiles(0)
    , mActiveProfile(0)
    , mLevel(0)
{
    SetSuppressSceneRendering(true);
}

GuiMenuProfiles::~GuiMenuProfiles()
{
    RemoveFunctors();
}

bool GuiMenuProfiles::Load(const char* filename_, bool reloadLast_)
{
    RemoveFunctors();
    bool ok = GuiDialog::Load(filename_, reloadLast_);
    if ( ok )
    {
        AddGuiEventFunctor( GetIdForName(std::string("id_newprofile")), new EventFunctor<GuiMenuProfiles>(this, &GuiMenuProfiles::OnButtonNewProfile) );
        AddGuiEventFunctor( GetIdForName(std::string("id_deleteprofile")), new EventFunctor<GuiMenuProfiles>(this, &GuiMenuProfiles::OnButtonDeleteProfile) );
        AddGuiEventFunctor( GetIdForName(std::string("id_back")), new EventFunctor<GuiMenuProfiles>(this, &GuiMenuProfiles::OnButtonBack) );
        AddGuiEventFunctor( GetIdForName(std::string("profile")), new EventFunctor<GuiMenuProfiles>(this, &GuiMenuProfiles::OnSliderSelectProfile) );

        IGUIElement * root = GetDialogParent();
        if ( !root )
            return false;

        std::string errorMsg("GuiMenuProfiles::Load");

        mActiveProfile = static_cast<IGUIStaticText*>(GetElementByName(root, "act_profile2", errorMsg));
        mSliderProfiles = static_cast<IGUITextSlider*>(GetElementByName(root, "profile", errorMsg));
        if ( mSliderProfiles )
        {
            mSliderProfiles->clearTexts();
        }
        mLevel = static_cast<IGUIStaticText*>(GetElementByName(root, "level", errorMsg));
    }
    return ok;
}

void GuiMenuProfiles::UpdateProfiles()
{
    if ( mSliderProfiles )
    {
        mSliderProfiles->clearTexts();
        for ( unsigned int i=0; i < APP.GetProfileManager()->GetNumProfiles(); ++i )
        {
            PlayerProfile* profile = APP.GetProfileManager()->GetProfile(i);
            mSliderProfiles->addText( profile->mName.c_str() );
        }
        int selected = APP.GetProfileManager()->GetSelectedProfileIndex();
        mSliderProfiles->setCurrentTextId( selected );
    }

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

	UpdateSelectedProfile();
}

void GuiMenuProfiles::UpdateSelectedProfile()
{
    if ( mLevel )
	{
		PlayerProfile* profile = APP.GetProfileManager()->GetSelectedProfile();
		if ( profile )
		{
			core::stringw score( APP.GetStringTable()->Get("id_profile_level") );
			score += core::stringw(profile->mChampionship.GetTotalScore());
			mLevel->setText( score.c_str() );
		}
		else
		{
			mLevel->setText(L"");
		}
	}
}

void GuiMenuProfiles::Show()
{
    UpdateProfiles();
    GuiDialog::Show();
}

void GuiMenuProfiles::RemoveFunctors()
{
    if ( !IsLoaded() )
        return;
    RemoveGuiEventFunctor( GetIdForName(std::string("id_newprofile")) );
    RemoveGuiEventFunctor( GetIdForName(std::string("id_deleteprofile")) );
    RemoveGuiEventFunctor( GetIdForName(std::string("id_back")) );
    RemoveGuiEventFunctor( GetIdForName(std::string("profile")) );
}

bool GuiMenuProfiles::OnButtonNewProfile(const irr::SEvent &event_)
{
    if ( event_.GUIEvent.EventType == EGET_BUTTON_CLICKED )
    {
        APP.GetGui()->SetActiveDialog( APP.GetGui()->GetGuiMenuNewProfile() );
    }

    return false;
}

bool GuiMenuProfiles::OnButtonDeleteProfile(const irr::SEvent &event_)
{
    if ( !mSliderProfiles )
        return false;

    if ( event_.GUIEvent.EventType == EGET_BUTTON_CLICKED )
    {
        int selected = APP.GetProfileManager()->GetSelectedProfileIndex();
        if ( selected >= 0 )
        {
            APP.GetGui()->PlayOverrideClickSound(GS_CLICK_CANCEL);
            GuiDlgOkCancel * dlg = static_cast<GuiDlgOkCancel*>(APP.GetGui()->GetGuiDlgOkCancel());
            dlg->SetInfoText( APP.GetStringTable()->Get("id_warning_profile").c_str() );
            dlg->SetOkFunctor( new EventFunctor<GuiMenuProfiles>(this, &GuiMenuProfiles::OnDeleteAcknowledged) );
            dlg->SetCancelFunctor(0);
            APP.GetGui()->SetModalDialog( dlg );
        }
    }

    return false;
}

bool GuiMenuProfiles::OnDeleteAcknowledged(const irr::SEvent &event_)
{
    int selected = APP.GetProfileManager()->GetSelectedProfileIndex();
    if ( selected >= 0 )
    {
        APP.GetProfileManager()->DeleteProfile( selected );
        UpdateProfiles();
    }
    return false;
}

bool GuiMenuProfiles::OnButtonBack(const irr::SEvent &event_)
{
    if ( event_.GUIEvent.EventType == EGET_BUTTON_CLICKED )
    {
        APP.GetProfileManager()->SaveProfiles();
        APP.GetGui()->SetActiveDialog( APP.GetGui()->GetGuiMenuMain() );
    }

    return false;
}

bool GuiMenuProfiles::OnSliderSelectProfile(const irr::SEvent &event_)
{
    if ( event_.GUIEvent.EventType == (gui::EGUI_EVENT_TYPE)EGET_TEXTSLIDER_CHANGED )
    {
        if ( mSliderProfiles )
        {
            int id = mSliderProfiles->getCurrentTextId();
            APP.GetProfileManager()->SelectProfile( id );
			UpdateSelectedProfile();
        }
    }

    return false;
}
