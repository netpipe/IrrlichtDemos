// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#include "gui_menu_selecthover.h"
#include "gui_menu_main.h"
#include "../gui.h"
#include "../main.h"
#include "../profiles.h"
#include "../string_table.h"
#include "../hover.h"
#include "../config.h"
#include <assert.h>

using namespace irr;
using namespace gui;


GuiMenuSelectHover::GuiMenuSelectHover(const Config& config)
    : GuiDialog(config)
    , mImageHover(0)
    , mSliderHover(0)
    , mUnlock(0)
    , mButtonAccept(0)
    , mActiveProfile(0)
{
    SetSuppressSceneRendering(true);
}

GuiMenuSelectHover::~GuiMenuSelectHover()
{
    RemoveFunctors();
}

bool GuiMenuSelectHover::Load(const char* filename_, bool reloadLast_)
{
    RemoveFunctors();
    bool ok = GuiDialog::Load(filename_, reloadLast_);
    if ( ok )
    {
        AddGuiEventFunctor( GetIdForName(std::string("id_accept")), new EventFunctor<GuiMenuSelectHover>(this, &GuiMenuSelectHover::OnButtonAccept) );
        AddGuiEventFunctor( GetIdForName(std::string("id_cancel")), new EventFunctor<GuiMenuSelectHover>(this, &GuiMenuSelectHover::OnButtonCancel) );
        AddGuiEventFunctor( GetIdForName(std::string("select")), new EventFunctor<GuiMenuSelectHover>(this, &GuiMenuSelectHover::OnSliderHover) );

        IGUIElement * root = GetDialogParent();
        if ( !root )
            return false;

        std::string errorMsg("GuiMenuSelectHover::Load");
        mActiveProfile = static_cast<IGUIStaticText*>(GetElementByName(root, "act_profile2", errorMsg));
        mImageHover = static_cast<IGUIImage*>(GetElementByName(root, "preview", errorMsg));
        mSliderHover= static_cast<IGUITextSlider*>(GetElementByName(root, "select", errorMsg));
        mUnlock = static_cast<IGUIStaticText*>(GetElementByName(root, "unlock", errorMsg));
        mButtonAccept = static_cast<IGUIButton*>(GetElementByName(root, "id_accept", errorMsg));

        if ( mSliderHover )
        {
            mSliderHover->clearTexts();

            for ( unsigned int i=0; i<APP.GetNumHovers(); ++i )
            {
                mSliderHover->addText( APP.GetHover(i).mName.c_str() );
            }
        }
    }
    return ok;
}

void GuiMenuSelectHover::Show()
{
    GuiDialog::Show();
    PlayerProfile* profile = APP.GetProfileManager()->GetSelectedProfile();
    if ( mActiveProfile )
    {
        if ( profile )
        {
            mActiveProfile->setText( profile->mName.c_str() );
        }
        else
        {
            mActiveProfile->setText( APP.GetStringTable()->Get("id_showactiveprofile").c_str() );
        }
    }

    if ( profile && mSliderHover )
    {
        mSliderHover->setCurrentTextId( profile->mHover );
        SelectHover();
    }
}

void GuiMenuSelectHover::SelectHover()
{
    int id = mSliderHover->getCurrentTextId();

    if ( id < 0 || id >= (int)APP.GetNumHovers() )
        return;
    const Hover& hover = APP.GetHover(id);
    PlayerProfile* profile = APP.GetProfileManager()->GetSelectedProfile();
    if ( !profile )
        return;

    std::string texName(APP.GetConfig()->GetPathMedia() + hover.mImage);
    video::ITexture * texture = GetDriver()->getTexture( texName.c_str() );
    if ( texture && mImageHover )
    {
        mImageHover->setImage(texture);
    }
    if ( mUnlock && mButtonAccept )
    {
        mUnlock->setText( hover.mLockText.c_str() );

        mUnlock->setVisible( !profile->IsHoverUnlocked(id) );
        mButtonAccept->setEnabled( profile->IsHoverUnlocked(id) );
    }
}

void GuiMenuSelectHover::RemoveFunctors()
{
    if ( !IsLoaded() )
        return;
    RemoveGuiEventFunctor( GetIdForName(std::string("id_accept")) );
    RemoveGuiEventFunctor( GetIdForName(std::string("id_cancel")) );
    RemoveGuiEventFunctor( GetIdForName(std::string("select")) );
}

bool GuiMenuSelectHover::OnSliderHover(const irr::SEvent &event_)
{
    if ( event_.GUIEvent.EventType == (gui::EGUI_EVENT_TYPE)EGET_TEXTSLIDER_CHANGED )
    {
        SelectHover();
    }
    return false;
}

bool GuiMenuSelectHover::OnButtonAccept(const irr::SEvent &event_)
{
    if ( event_.GUIEvent.EventType == EGET_BUTTON_CLICKED )
    {
        PlayerProfile* profile = APP.GetProfileManager()->GetSelectedProfile();
        if ( !profile || !mSliderHover )
            return false;

        assert( profile->IsHoverUnlocked( mSliderHover->getCurrentTextId() ) );

        profile->mHover = mSliderHover->getCurrentTextId();
        APP.GetProfileManager()->SaveProfiles();

        APP.GetGui()->SetActiveDialog( APP.GetGui()->GetGuiMenuMain() );
    }

    return false;
}

bool GuiMenuSelectHover::OnButtonCancel(const irr::SEvent &event_)
{
    if ( event_.GUIEvent.EventType == EGET_BUTTON_CLICKED )
    {
        APP.GetGui()->PlayOverrideClickSound(GS_CLICK_CANCEL);
        APP.GetGui()->SetActiveDialog( APP.GetGui()->GetGuiMenuMain() );
    }

    return false;
}
