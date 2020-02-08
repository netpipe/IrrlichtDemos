// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#include "gui_menu_replaytheatre.h"
#include "gui_menu_main.h"
#include "gui_helper.h"
#include "../gui.h"
#include "../main.h"
#include "../profiles.h"
#include "../string_table.h"
#include "../level.h"

using namespace irr;
using namespace gui;


GuiMenuReplayTheatre::GuiMenuReplayTheatre(const Config& config)
    : GuiDialog(config)
    , mActiveProfile(0)
{
}

GuiMenuReplayTheatre::~GuiMenuReplayTheatre()
{
    RemoveFunctors();
}

bool GuiMenuReplayTheatre::Load(const char* filename_, bool reloadLast_)
{
    RemoveFunctors();
    bool ok = GuiDialog::Load(filename_, reloadLast_);
    if ( ok )
    {
        AddGuiEventFunctor( GetIdForName(std::string("id_showreplay")), new EventFunctor<GuiMenuReplayTheatre>(this, &GuiMenuReplayTheatre::OnButtonReplay) );
        AddGuiEventFunctor( GetIdForName(std::string("id_back")), new EventFunctor<GuiMenuReplayTheatre>(this, &GuiMenuReplayTheatre::OnButtonBack) );

        IGUIElement * root = GetDialogParent();
        if ( !root )
            return false;

        std::string errorMsg("GuiMenuReplayTheatre::Load");

        mActiveProfile = static_cast<IGUIStaticText*>(GetElementByName(root, "act_profile2", errorMsg));

        IGUITextSlider* slider = static_cast<IGUITextSlider*>(GetElementByName(root, "racemode", errorMsg));
        if ( !slider )
            return false;
        slider->clearTexts();
        slider->addText( APP.GetStringTable()->Get("id_mode_championship").c_str() );
        slider->addText( APP.GetStringTable()->Get("id_mode_arcade").c_str() );
        slider->addText( APP.GetStringTable()->Get("id_mode_hotlap").c_str() );
        slider->addText( APP.GetStringTable()->Get("id_mode_timerace").c_str() );

        slider = static_cast<IGUITextSlider*>(GetElementByName(root, "trackdifficulty", errorMsg));
        if ( !slider )
            return false;
        slider->clearTexts();
        slider->addText( APP.GetStringTable()->Get("id_dif_all").c_str() );
        slider->addText( APP.GetStringTable()->Get("id_dif_easy").c_str() );
        slider->addText( APP.GetStringTable()->Get("id_dif_medium").c_str() );
        slider->addText( APP.GetStringTable()->Get("id_dif_hard").c_str() );

        slider = static_cast<IGUITextSlider*>(GetElementByName(root, "trackname", errorMsg));
        if ( !slider )
            return false;
        GuiHelper::FillTextSliderWithTrackNames(slider, LD_ALL);
    }
    return ok;
}

void GuiMenuReplayTheatre::Show()
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

    GuiDialog::Show();
}

void GuiMenuReplayTheatre::RemoveFunctors()
{
    if ( !IsLoaded() )
        return;
    RemoveGuiEventFunctor( GetIdForName(std::string("id_showreplay")) );
    RemoveGuiEventFunctor( GetIdForName(std::string("id_back")) );
}

bool GuiMenuReplayTheatre::OnButtonReplay(const irr::SEvent &event_)
{
    // TODO
    return false;
}

bool GuiMenuReplayTheatre::OnButtonBack(const irr::SEvent &event_)
{
    if ( event_.GUIEvent.EventType == EGET_BUTTON_CLICKED )
    {
        APP.GetGui()->PlayOverrideClickSound(GS_CLICK_CANCEL);
        APP.GetGui()->SetActiveDialog( APP.GetGui()->GetGuiMenuMain() );
    }

    return false;
}
