// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#ifndef GUI_MENU_GAMEEND_H
#define GUI_MENU_GAMEEND_H
#include "../gui_dialog.h"

class GuiMenuGameEnd : public GuiDialog
{
public:
    GuiMenuGameEnd(const Config& config);
    ~GuiMenuGameEnd();

    virtual bool Load(const char* filename_, bool reloadLast_=false);
    virtual void Show();

protected:

    void RemoveFunctors();

    bool OnButtonRestart(const irr::SEvent &event_);
    bool OnButtonWatchReplay(const irr::SEvent &event_);
    bool OnButtonSaveReplay(const irr::SEvent &event_);
    bool OnButtonQuit(const irr::SEvent &event_);

private:
    irr::gui::IGUIStaticText*  mStaticResultText;
    irr::gui::IGUIStaticText*  mStaticResult;
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
    irr::gui::IGUIStaticText *  mStaticTimeraceGap;
    irr::gui::IGUIStaticText *  mStaticPassed;
    irr::gui::IGUIStaticText *  mStaticGhostRecord;
    irr::gui::IGUIStaticText *  mStaticGhostModeRecord;
    irr::gui::IGUIStaticText *  mStaticGhostGapInfo;
    irr::gui::IGUIStaticText *  mStaticGhostGap;
    irr::gui::IGUIImage *       mImageGhostRecord;
    irr::gui::IGUIImage *       mImageGhostGap;

//    irr::gui::IGUIButton * mButtonWatchReplay;
//    irr::gui::IGUIButton * mButtonSaveReplay;
};

#endif // GUI_MENU_GAMEEND_H

