// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#ifndef GUI_MENU_RIVALS_SCORE_H
#define GUI_MENU_RIVALS_SCORE_H

#include "../gui_dialog.h"
#include "../globals.h"

// the mode is called rivals/tournament by now in the game.

class GuiMenuRivalsScore : public GuiDialog
{
public:
    GuiMenuRivalsScore(const Config& config);
    ~GuiMenuRivalsScore();

    virtual bool Load(const char* filename_, bool reloadLast_=false);
    virtual void Show();

protected:
    void RemoveFunctors();

    bool OnButtonStart(const irr::SEvent &event_);
    bool OnButtonExit(const irr::SEvent &event_);
    bool OnExitAcknowledged(const irr::SEvent &event_);
    bool OnExitCanceled(const irr::SEvent &event_);

private:
    irr::gui::IGUIButton* mButtonStart;
    irr::gui::IGUIButton* mButtonExit;

    irr::gui::IGUIStaticText* mStaticName[MAX_RIVALS];
    irr::gui::IGUIStaticText* mStaticTime[MAX_RIVALS];
    irr::gui::IGUIStaticText* mStaticTimeSum[MAX_RIVALS];
    irr::gui::IGUIStaticText* mStaticScore[MAX_RIVALS];

    irr::gui::IGUIStaticText* mStaticWinner;            // rivals_winner
    irr::gui::IGUIStaticText* mStaticBestTimeSum;       // rivals_besttime
    irr::gui::IGUIStaticText* mStaticRaceCounter;       // racecounter
    irr::gui::IGUIStaticText* mStaticRaceCounterLabel;
    irr::gui::IGUIStaticText* mStaticCurrentPlayerName; // playername
    irr::gui::IGUIStaticText* mStaticCurrentPlayerNameLabel;
    irr::gui::IGUIStaticText* mStaticCurrentTrackName;  // trackname
    irr::gui::IGUIStaticText* mStaticCurrentTrackNameLabel;
};


#endif // GUI_MENU_RIVALS_SCORE_H

