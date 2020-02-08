// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#ifndef GUI_MENU_PAUSE_H
#define GUI_MENU_PAUSE_H

#include "../gui_dialog.h"

class GuiMenuPause : public GuiDialog
{
public:
    GuiMenuPause(const Config& config);
    ~GuiMenuPause();

    virtual bool Load(const char* filename_, bool reloadLast_=false);
    virtual void Show();

protected:

    void SaveSoundSettings();

    void RemoveFunctors();
    bool OnButtonResume(const irr::SEvent &event_);
    bool OnButtonRestart(const irr::SEvent &event_);
    bool OnButtonQuit(const irr::SEvent &event_);
    bool OnSliderSfx(const irr::SEvent &event_);
    bool OnSliderMusic(const irr::SEvent &event_);
    bool OnButtonTouchControls(const irr::SEvent &event_);

private:
    irr::gui::IGUIButton*       mButtonRestart;
    irr::gui::IGUIButton*       mButtonQuit;
    irr::gui::IGUIStaticText*   mStaticTrackName;
    irr::gui::IGUITextSlider*   mSfxVolumeSlider;
    irr::gui::IGUITextSlider*   mMusicVolumeSlider;
};

#endif // GUI_MENU_PAUSE_H
