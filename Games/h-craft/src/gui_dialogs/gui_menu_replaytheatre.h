// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#ifndef GUI_MENU_REPLAYTHEATRE_H
#define GUI_MENU_REPLAYTHEATRE_H

#include "../gui_dialog.h"

class GuiMenuReplayTheatre : public GuiDialog
{
public:
    GuiMenuReplayTheatre(const Config& config);
    ~GuiMenuReplayTheatre();

    virtual bool Load(const char* filename_, bool reloadLast_=false);
    virtual void Show();

protected:
    void RemoveFunctors();
    bool OnButtonReplay(const irr::SEvent &event_);
    bool OnButtonBack(const irr::SEvent &event_);

    // TODO
    // bool OnSliderRacemode(const irr::SEvent &event_);
    // bool OnSliderDifficulty(const irr::SEvent &event_);
    // bool OnSliderTrack(const irr::SEvent &event_);

private:
    irr::gui::IGUIStaticText* mActiveProfile;

};

#endif // GUI_MENU_REPLAYTHEATRE_H
