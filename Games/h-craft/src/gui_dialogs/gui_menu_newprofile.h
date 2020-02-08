// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#ifndef GUI_MENU_NEW_PROFILE_H
#define GUI_MENU_NEW_PROFILE_H

#include "../gui_dialog.h"

class GuiMenuNewProfile : public GuiDialog
{
public:
    GuiMenuNewProfile(const Config& config);
    ~GuiMenuNewProfile();

    virtual bool Load(const char* filename_, bool reloadLast_=false);
    virtual void Show();

    void InfoFirstProfile()     { mFirstProfile = true; }

protected:

    void RemoveFunctors();
    bool OnButtonAccept(const irr::SEvent &event_);
    bool OnButtonCancel(const irr::SEvent &event_);
    bool OnEditName(const irr::SEvent &event_);
    bool OnSliderSelectKb(const irr::SEvent &event_);

private:
    bool mFirstProfile;
    irr::gui::IGUITextSlider* mSliderSelectKb;
    irr::gui::IGUIEditBox *  mEditName;
};

#endif // GUI_MENU_NEW_PROFILE_H
