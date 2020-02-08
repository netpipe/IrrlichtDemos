// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#ifndef GUI_MENU_GAMEEND_CHAMP_H
#define GUI_MENU_GAMEEND_CHAMP_H

#include "../gui_dialog.h"

class GuiMenuGameEndChampionship : public GuiDialog
{
public:
    GuiMenuGameEndChampionship(const Config& config);
    ~GuiMenuGameEndChampionship();

    virtual bool Load(const char* filename_, bool reloadLast_=false);
    virtual void Show();

protected:

    void RemoveFunctors();

    bool OnButtonContinue(const irr::SEvent &event_);
    bool OnButtonRestart(const irr::SEvent &event_);
    bool OnButtonWatchReplay(const irr::SEvent &event_);
    bool OnButtonSaveReplay(const irr::SEvent &event_);
    bool OnButtonQuit(const irr::SEvent &event_);

private:
    irr::gui::IGUIStaticText*  mStaticResult;
    irr::gui::IGUIStaticText*  mStaticPoints;
    irr::gui::IGUIStaticText *  mStaticTrackName;
    irr::gui::IGUIStaticText *  mStaticTimeCurrentLap;
    irr::gui::IGUIStaticText *  mStaticBestLap;
    irr::gui::IGUIStaticText *  mStaticTrack;
    irr::gui::IGUIImage *       mImageTrackRecord;
    irr::gui::IGUIStaticText *  mStaticTrackRecord;
    irr::gui::IGUIStaticText *  mStaticTimeTrackRecord;
    irr::gui::IGUIStaticText *  mStaticLapRecord;
    irr::gui::IGUIStaticText *  mStaticLapCount;
    irr::gui::IGUIStaticText *  mStaticPosition;
    irr::gui::IGUIStaticText *  mStaticGapTime;

//    irr::gui::IGUIButton * mButtonWatchReplay;
//    irr::gui::IGUIButton * mButtonSaveReplay;
};

#endif // GUI_MENU_GAMEEND_CHAMP_H
