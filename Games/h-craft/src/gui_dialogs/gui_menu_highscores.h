// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#ifndef GUI_MENU_HIGHSCORES_H
#define GUI_MENU_HIGHSCORES_H

#include "../gui_dialog.h"
#include "../highscores.h"
#include "gui_helper.h"

class GuiMenuHighscores : public GuiDialog
{
public:
    GuiMenuHighscores(const Config& config);
    ~GuiMenuHighscores();

    virtual bool Load(const char* filename_, bool reloadLast_=false);
    virtual void Show();

protected:
    void RemoveFunctors();

    bool OnButtonBack(const irr::SEvent &event_);
    bool OnSliderRacemode(const irr::SEvent &event_);
    bool OnSliderDifficulty(const irr::SEvent &event_);
    bool OnSliderTrack(const irr::SEvent &event_);

    void ChangedMode();
    LevelDifficulty GetDifficulty() const;
    CS_Season GetSeason() const;
    bool IsChampionshipMode() const;

private:
    irr::gui::IGUIStaticText* mActiveProfile;
    irr::gui::IGUIStaticText* mStaticScore[MAX_HIGHSCORES];
    irr::gui::IGUITextSlider* mSliderScoreMode;
    irr::gui::IGUITextSlider* mSliderDifficulty;
    irr::gui::IGUITextSlider* mSliderTrack;

    int     mLevelIndex;
    int     mTrackIndex;
    int     mDifficultyIndex;
    int     mSeasonIndex;
};

#endif // GUI_MENU_HIGHSCORES_H
