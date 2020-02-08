// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#ifndef GUI_MENU_MAIN_H
#define GUI_MENU_MAIN_H

#include "../gui_dialog.h"

class GuiMenuMain : public GuiDialog
{
public:
    GuiMenuMain(const Config& config);
    virtual ~GuiMenuMain();

    virtual bool Load(const char* filename_, bool reloadLast_=false);
    virtual void Show();

protected:
    void RemoveFunctors();

    bool OnButtonRivals(const irr::SEvent &event_);
    bool OnButtonChampionship(const irr::SEvent &event_);
    bool OnButtonArcade(const irr::SEvent &event_);
    bool OnButtonTimeAttack(const irr::SEvent &event_);
    bool OnButtonSelectHover(const irr::SEvent &event_);
    bool OnButtonReplayTheatre(const irr::SEvent &event_);
    bool OnButtonHighscores(const irr::SEvent &event_);
    bool OnButtonOptions(const irr::SEvent &event_);
    bool OnButtonProfiles(const irr::SEvent &event_);
    bool OnButtonQuit(const irr::SEvent&event_);

private:
    irr::gui::IGUIButton * mButtonChampionShip;
    irr::gui::IGUIButton * mButtonArcade;
    irr::gui::IGUIButton * mButtonTimeattack;
    irr::gui::IGUIButton * mButtonSelectHover;
    irr::gui::IGUIButton * mButtonQuit;
    irr::gui::IGUIButton * mButtonOptions;
    irr::gui::IGUIButton * mButtonRivalsMode;
    irr::gui::IGUIStaticText* mActiveProfile;
    irr::gui::IGUIStaticText* mDemoText;
    irr::gui::IGUIStaticText* mDebugText;
};

#endif // GUI_MENU_MAIN_H
