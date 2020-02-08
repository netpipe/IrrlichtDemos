// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#include "gui_dlg_okcancel.h"
#include "../gui.h"
#include "../main.h"

using namespace irr;
using namespace gui;

GuiDlgOkCancel::GuiDlgOkCancel(const Config& config)
    : GuiDialog(config)
    , mButtonOk(0)
    , mFunctorOk(0)
    , mFunctorCancel(0)
    , mInfoText(0)
{
}

GuiDlgOkCancel::~GuiDlgOkCancel()
{
    RemoveFunctors();
    delete mFunctorOk;
    delete mFunctorCancel;
}

bool GuiDlgOkCancel::Load(const char* filename_, bool reloadLast_)
{
    RemoveFunctors();
    bool ok = GuiDialog::Load(filename_, reloadLast_);
    if ( ok )
    {
        AddGuiEventFunctor( GetIdForName(std::string("id_ok")), new EventFunctor<GuiDlgOkCancel>(this, &GuiDlgOkCancel::OnButtonOk) );
        AddGuiEventFunctor( GetIdForName(std::string("id_cancel")), new EventFunctor<GuiDlgOkCancel>(this, &GuiDlgOkCancel::OnButtonCancel) );

        IGUIElement * root = GetDialogParent();
        if ( !root )
            return false;
        std::string errorMsg("GuiDlgOkCancel::Load");

        mButtonOk = static_cast<IGUIButton*>(GetElementByName(root, "id_ok", errorMsg));
        mInfoText = static_cast<IGUIStaticText*>(GetElementByName(root, "id_dummytext", errorMsg));
    }
    return ok;
}

void GuiDlgOkCancel::SetInfoText(const wchar_t * text_)
{
    if ( !mInfoText )
        return;

    mInfoText->setText( text_ );
}

void GuiDlgOkCancel::SetOkFunctor(IEventFunctor * functor_ )
{
    delete mFunctorOk;
    mFunctorOk = functor_;
}

void GuiDlgOkCancel::SetCancelFunctor(IEventFunctor * functor_ )
{
    delete mFunctorCancel;
    mFunctorCancel = functor_;
}

void GuiDlgOkCancel::RemoveFunctors()
{
    if ( !IsLoaded() )
        return;

    RemoveGuiEventFunctor( GetIdForName(std::string("id_ok")) );
    RemoveGuiEventFunctor( GetIdForName(std::string("id_cancel")) );
}

bool GuiDlgOkCancel::OnButtonOk(const irr::SEvent &event_)
{
    bool result = false;
    if ( event_.GUIEvent.EventType == EGET_BUTTON_CLICKED )
    {
        APP.GetGui()->SetModalDialog(NULL);
        if ( mFunctorOk && mFunctorOk->CallEvent(event_) )
        {
            result = true;
        }
    }
    return result;
}

bool GuiDlgOkCancel::OnButtonCancel(const irr::SEvent &event_)
{
    bool result = false;
    if ( event_.GUIEvent.EventType == EGET_BUTTON_CLICKED )
    {
        APP.GetGui()->SetModalDialog(NULL);
        APP.GetGui()->PlayOverrideClickSound(GS_CLICK_CANCEL);
        if ( mFunctorCancel && mFunctorCancel->CallEvent(event_) )
        {
            result = true;
        }
    }
    return result;
}

void GuiDlgOkCancel::SetFocusOkButton()
{
    GetEnvironment()->setFocus(mButtonOk);
}
