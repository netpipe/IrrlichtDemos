// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#ifndef GUI_DLG_OKCANCEL_H
#define GUI_DLG_OKCANCEL_H

#include "../gui_dialog.h"

class GuiDlgOkCancel : public GuiDialog
{
public:
    GuiDlgOkCancel(const Config& config);
    ~GuiDlgOkCancel();

    virtual bool Load(const char* filename_, bool reloadLast_=false);

    void SetInfoText(const wchar_t * text_);
    void SetOkFunctor(IEventFunctor * functor_ );       // this class will care about the pointer
    void SetCancelFunctor(IEventFunctor * functor_ );   // this class will care about the pointer
    void SetFocusOkButton();

protected:
    void RemoveFunctors();
    bool OnButtonOk(const irr::SEvent &event_);
    bool OnButtonCancel(const irr::SEvent &event_);

private:
    irr::gui::IGUIButton* mButtonOk;
    IEventFunctor * mFunctorOk;
    IEventFunctor * mFunctorCancel;
    irr::gui::IGUIStaticText* mInfoText;
};

#endif // GUI_DLG_OKCANCEL_H
