// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#ifndef GUI_MENU_PROFILES_H
#define GUI_MENU_PROFILES_H

#include "../gui_dialog.h"

class GuiMenuProfiles : public GuiDialog
{
public:
    GuiMenuProfiles(const Config& config);
    ~GuiMenuProfiles();

    virtual bool Load(const char* filename_, bool reloadLast_=false);
    virtual void Show();

protected:
    void RemoveFunctors();

    bool OnButtonNewProfile(const irr::SEvent &event_);
    bool OnButtonDeleteProfile(const irr::SEvent &event_);
    bool OnDeleteAcknowledged(const irr::SEvent &event_);
    bool OnButtonBack(const irr::SEvent &event_);
    bool OnSliderSelectProfile(const irr::SEvent &event_);

    void UpdateProfiles();
    void UpdateSelectedProfile();

private:
    irr::gui::IGUITextSlider* mSliderProfiles;
    irr::gui::IGUIStaticText* mActiveProfile;
    irr::gui::IGUIStaticText* mLevel;
};

#endif // GUI_MENU_PROFILES_H
