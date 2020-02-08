// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#ifndef GUI_MENU_GAMEEND_RIVALS_H
#define GUI_MENU_GAMEEND_RIVALS_H

#include "../gui_dialog.h"
#include "../globals.h"

class GuiMenuGameEndRivals : public GuiDialog
{
public:
    GuiMenuGameEndRivals(const Config& config);
    ~GuiMenuGameEndRivals();

    virtual bool Load(const char* filename_, bool reloadLast_=false);
    virtual void Show();

protected:

    void RemoveFunctors();
    bool OnButtonContinue(const irr::SEvent &event_);

private:
    irr::gui::IGUIStaticText* mStaticTime[MAX_RIVALS];
    irr::gui::IGUIStaticText* mStaticResult[MAX_RIVALS];
    irr::gui::IGUIStaticText* mStaticTimeCurrentLap;
    irr::gui::IGUIStaticText* mStaticBestLap;
    irr::gui::IGUIStaticText* mStaticTrack;
    irr::gui::IGUIStaticText* mStaticTrackRecord;
    irr::gui::IGUIStaticText* mStaticTimeTrackRecord;
    irr::gui::IGUIStaticText* mStaticLapRecord;
    irr::gui::IGUIStaticText* mStaticLapCount;
    irr::gui::IGUIStaticText* mStaticPosition;
    irr::gui::IGUIStaticText* mStaticTrackName;
    irr::gui::IGUIStaticText* mStaticSessionPlayerName;
    irr::gui::IGUIStaticText* mStaticSessionModeRecord;
    irr::gui::IGUIStaticText* mStaticSessionGap;
};

#endif // GUI_MENU_GAMEEND_RIVALS_H

