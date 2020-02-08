// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#include "gui_menu_championship.h"
#include "gui_menu_main.h"
#include "gui_menu_championship_progress.h"
#include "gui_menu_tutorial3.h"
#include "gui_dlg_okcancel.h"
#include "gui_helper.h"
#include "../main.h"
#include "../gui.h"
#include "../profiles.h"
#include "../string_table.h"

using namespace irr;
using namespace gui;

GuiMenuChampionship::GuiMenuChampionship(const Config& config)
    : GuiDialog(config)
    , mActiveProfile(0)
    , mButtonNew(0)
    , mButtonContinue(0)
{
    SetSuppressSceneRendering(true);
}

GuiMenuChampionship::~GuiMenuChampionship()
{
    RemoveFunctors();
}

bool GuiMenuChampionship::Load(const char* filename_, bool reloadLast_)
{
    RemoveFunctors();
    bool ok = GuiDialog::Load(filename_, reloadLast_);
    if ( ok )
    {
        AddGuiEventFunctor( GetIdForName(std::string("id_continue")), new EventFunctor<GuiMenuChampionship>(this, &GuiMenuChampionship::OnButtonContinue) );
        AddGuiEventFunctor( GetIdForName(std::string("id_newchamp")), new EventFunctor<GuiMenuChampionship>(this, &GuiMenuChampionship::OnButtonNew) );
        AddGuiEventFunctor( GetIdForName(std::string("id_back")), new EventFunctor<GuiMenuChampionship>(this, &GuiMenuChampionship::OnButtonBack) );


        IGUIElement * root = GetDialogParent();
        if ( !root )
            return false;

        std::string errorMsg("GuiMenuChampionship::Load");

        mActiveProfile = static_cast<IGUIStaticText*>(GetElementByName(root, "act_profile2", errorMsg));
        mButtonNew = static_cast<IGUIButton*>(GetElementByName(root, "id_newchamp", errorMsg));
        mButtonContinue = static_cast<IGUIButton*>(GetElementByName(root, "id_continue", errorMsg));
        SetAlternateDefaultFocus( mButtonNew );
    }
    return ok;
}

void GuiMenuChampionship::Show()
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

        if ( mButtonContinue  )
        {
            if ( !profile->mChampionship.HasSeenChampionshipProgress() )
            {
                mButtonContinue->setEnabled(false);
            }
            else
            {
                mButtonContinue->setEnabled(true);
            }
        }
    }

    GuiDialog::Show();
}

void GuiMenuChampionship::RemoveFunctors()
{
    if ( !IsLoaded() )
        return;

    RemoveGuiEventFunctor( GetIdForName(std::string("id_continue")) );
    RemoveGuiEventFunctor( GetIdForName(std::string("id_newchamp")) );
    RemoveGuiEventFunctor( GetIdForName(std::string("id_back")) );
}

bool GuiMenuChampionship::OnButtonContinue(const irr::SEvent &event_)
{
    if ( event_.GUIEvent.EventType == EGET_BUTTON_CLICKED )
    {
        GuiHelper::SetGameModeMenu(GT_CHAMPIONSHIP);
    }

    return false;
}

bool GuiMenuChampionship::OnButtonNew(const irr::SEvent &event_)
{
    if ( event_.GUIEvent.EventType == EGET_BUTTON_CLICKED )
    {
        PlayerProfile* profile = APP.GetProfileManager()->GetSelectedProfile();
        if ( profile && !profile->mChampionship.HasSeenChampionshipProgress() )
        {
            // When no championship exists we don't need to ask if an old one should be deleted.
            // Therefore proceed directly to the next screen
            return OnNewAcknowledged(event_);
        }

        APP.GetGui()->PlayOverrideClickSound(GS_CLICK_CANCEL);
        GuiDlgOkCancel * dlg = static_cast<GuiDlgOkCancel*>(APP.GetGui()->GetGuiDlgOkCancel());
        dlg->SetInfoText( APP.GetStringTable()->Get("id_warning_champ").c_str() );
        dlg->SetOkFunctor( new EventFunctor<GuiMenuChampionship>(this, &GuiMenuChampionship::OnNewAcknowledged) );
        dlg->SetCancelFunctor(0);
        APP.GetGui()->SetModalDialog( dlg );
    }

    return false;
}

bool GuiMenuChampionship::OnNewAcknowledged(const irr::SEvent &event_)
{
    PlayerProfile* profile = APP.GetProfileManager()->GetSelectedProfile();
    if ( profile )
    {
        profile->mChampionship.Reset();
        profile->mChampionship.SetHasSeenChampionshipProgress();
        APP.GetProfileManager()->SaveProfiles();

        GuiHelper::SetGameModeMenu(GT_CHAMPIONSHIP);
    }
    return false;
}

bool GuiMenuChampionship::OnButtonBack(const irr::SEvent &event_)
{
    if ( event_.GUIEvent.EventType == EGET_BUTTON_CLICKED )
    {
        APP.GetGui()->PlayOverrideClickSound(GS_CLICK_CANCEL);
        APP.GetGui()->SetActiveDialog( APP.GetGui()->GetGuiMenuMain() );
    }

    return false;
}
