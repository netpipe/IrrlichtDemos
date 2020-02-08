// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#ifndef GUI_MENU_OPTIONS_H
#define GUI_MENU_OPTIONS_H

#include "../gui_dialog.h"

class GuiMenuOptions : public GuiDialog
{
public:
    GuiMenuOptions(const Config& config);
    ~GuiMenuOptions();

    virtual bool Load(const char* filename_, bool reloadLast_=false);
    virtual void Show();

protected:
    void RemoveFunctors();
    bool OnButtonTouchControls(const irr::SEvent &event_);
    bool OnButtonController(const irr::SEvent &event_);
    bool OnButtonAnalog(const irr::SEvent &event_);
    bool OnButtonAccept(const irr::SEvent &event_);
    bool OnButtonCancel(const irr::SEvent &event_);
    bool OnButtonCredits(const irr::SEvent &event_);
    bool OnButtonGraphics(const irr::SEvent &event_);
    bool OnSliderSfx(const irr::SEvent &event_);
    bool OnSliderMusic(const irr::SEvent &event_);

private:
    irr::gui::IGUITextSlider* mSfxVolumeSlider;
    irr::gui::IGUITextSlider* mMusicVolumeSlider;
    irr::gui::IGUIStaticText* mActiveProfile;
};

#endif // GUI_MENU_OPTIONS_H
