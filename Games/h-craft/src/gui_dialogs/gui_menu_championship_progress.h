// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#ifndef GUI_MENU_CHAMPIONSHIP_PROGRESS_H
#define GUI_MENU_CHAMPIONSHIP_PROGRESS_H

#include "../gui_dialog.h"
#include <vector>

struct PlayerProfile;

class GuiMenuChampionshipProgress : public GuiDialog
{
public:
    GuiMenuChampionshipProgress(const Config& config);
    ~GuiMenuChampionshipProgress();

    virtual bool Load(const char* filename_, bool reloadLast_=false);
    virtual void Show();

protected:
    void RemoveFunctors();

    bool OnButtonRetry(const irr::SEvent &event_);
    bool OnButtonQuit(const irr::SEvent &event_);
    bool OnButtonContinue(const irr::SEvent &event_);
    bool OnButtonSeason1(const irr::SEvent &event_);
    bool OnButtonSeason2(const irr::SEvent &event_);
    bool OnButtonSeason3(const irr::SEvent &event_);

    void SetRetryRace( int selected_ );
    void SetNextRace(int next);
    void StartSelected();
    void ShowActiveSeason();

    int FindNextRace(PlayerProfile* profile) const;

private:
    const unsigned int NUM_QUARTERS;
    const unsigned int NUM_RACES;

    std::vector<irr::gui::IGUIStaticText*>  mRequiredPoints;
    std::vector<irr::gui::IGUIStaticText*>  mQuarterResults;
    std::vector<irr::gui::IGUIStaticText*>  mQuarterResultTexts;

    std::vector<irr::gui::IGUIStaticText*>  mRaceResults;
    std::vector<irr::gui::IGUIStaticText*>  mRaceNames;
    std::vector<irr::gui::IGUIButton*>      mRetryButtons;

    irr::gui::IGUIButton*       mButtonContinue;
    irr::gui::IGUIButton*       mButtonQuit;
    irr::gui::IGUIStaticText*   mTotalPoints;
    irr::gui::IGUIStaticText*   mActiveProfile;

    irr::gui::IGUIButton* mButtonSeason1;
    irr::gui::IGUIButton* mButtonSeason2;
    irr::gui::IGUIButton* mButtonSeason3;
    irr::gui::IGUIStaticText* 	mSeason1;
    irr::gui::IGUIStaticText* 	mSeason2;
    irr::gui::IGUIStaticText* 	mSeason3;

    int mRetryRace;	// manually selected race by user which he want's to retry
    int mNextRace;	// next race in championship
    int mActiveSeason; // range 1-3 (0 for all seasons in case old dialog will be revived)
};


#endif // GUI_MENU_CHAMPIONSHIP_PROGRESS_H
