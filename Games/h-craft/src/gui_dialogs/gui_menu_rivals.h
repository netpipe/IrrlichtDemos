// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#ifndef GUI_MENU_RIVALS_H
#define GUI_MENU_RIVALS_H

#include "../gui_dialog.h"
#include "../globals.h"
#include "../level.h"

// the mode is by now called rivals in the game.


enum NumRacesSelected
{
    NRS_SINGLE,
    NRS_THREE,
    NRS_FIVE,
    NRS_SEVEN,
};

class GuiMenuRivals : public GuiDialog
{
public:
    GuiMenuRivals(const Config& config);
    ~GuiMenuRivals();

    virtual bool Load(const char* filename_, bool reloadLast_=false);
    virtual void Show();

protected:
    void RemoveFunctors();
    LevelDifficulty GetLevelDifficulty() const;
    bool OnSliderLevelDifficulty(const irr::SEvent &event_);
    bool OnSliderNumRaces(const irr::SEvent &event_);
    bool OnSliderPlayer(const irr::SEvent &event_);
    bool OnButtonStart(const irr::SEvent &event_);
    bool OnButtonMain(const irr::SEvent &event_);
    unsigned int GetNumRacesSelected();
    bool CheckCanStart();

private:
    irr::gui::IGUIButton* mButtonStart;
    irr::gui::IGUIButton* mButtonBack;
    irr::gui::IGUITextSlider* mSliderTrackDifficulty;
    irr::gui::IGUITextSlider* mSliderNumRaces;
    irr::gui::IGUITextSlider* mSliderTracks;
    irr::gui::IGUITextSlider* mSliderPlayer[MAX_RIVALS];
};


#endif // GUI_MENU_RIVALS_H
