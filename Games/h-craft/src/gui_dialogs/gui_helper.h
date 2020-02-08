// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#ifndef GUI_HELPER_H
#define GUI_HELPER_H

#include "../level.h"
#include "../game.h"
#include "../IGUITextSlider.h"

#include <irrlicht.h>
#include <string>


// Stuff in here could've been avoided by an additional layer in gui-dialogs,
// but this has do now for this game

// season's are a pure graphical distinction so i had no structure for them
enum CS_Season
{
    SEASON_ALL,
    SEASON_1,
    SEASON_2,
    SEASON_3,
};

class GuiHelper
{
public:
    static void FillTextSliderWithTrackNames(irr::gui::IGUITextSlider *slider_, LevelDifficulty difficulty_, bool onlyEnabled_=true);
    static void FillTextSliderWithLevelNames(irr::gui::IGUITextSlider *slider_, CS_Season season_);
    static void FillTextSliderWithNumbers(irr::gui::IGUITextSlider *slider_, int from_, int to_, int stepSize_, bool useOffStringForZero_=true);    // include from_ and to_
    static irr::core::stringw MakeLevelName(int index, int racesPerSeason);
    static void SetGameModeMenu(GAME_TYPE gameType_);

    static std::wstring MakeTimeString(irr::s32 time_, bool showPlusSign_=false, int signOnZero_=0);
    static irr::u32 GetTimeFromTimeString(const std::wstring &strTime_);
};



#endif // GUI_HELPER_H
