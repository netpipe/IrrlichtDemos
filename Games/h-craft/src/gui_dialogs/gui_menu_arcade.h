// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#ifndef GUI_MENU_ARCADE
#define GUI_MENU_ARCADE

#include "../gui_dialog.h"
#include "../level.h"
#include "../IGUITextSlider.h"

class GuiMenuArcade : public GuiDialog
{
public:
    GuiMenuArcade(const Config& config);
    ~GuiMenuArcade();

    virtual bool Load(const char* filename_, bool reloadLast_=false);
    virtual void Show();

protected:
    void RemoveFunctors();
    bool OnButtonStart(const irr::SEvent &event_);
    bool OnButtonBack(const irr::SEvent &event_);
    bool OnSliderTracks(const irr::SEvent &event_);
    bool OnSliderLevelDifficulty(const irr::SEvent &event_);
    LevelDifficulty GetLevelDifficulty() const;

private:
    irr::gui::IGUIStaticText* mActiveProfile;
    irr::gui::IGUITextSlider* mSliderTracks;
    irr::gui::IGUITextSlider* mSliderLevelDifficulty;
    irr::gui::IGUITextSlider* mSliderBotDifficulty;
    irr::gui::IGUIButton* mButtonStart;
    irr::gui::IGUIButton* mButtonBack;
};


#endif // GUI_MENU_ARCADE
