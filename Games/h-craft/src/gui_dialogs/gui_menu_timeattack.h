// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#ifndef GUI_MENU_TIMEATTACK_H
#define GUI_MENU_TIMEATTACK_H

#include "../gui_dialog.h"
#include "../level.h"
#include "../game.h"

class GuiMenuTimeAttack : public GuiDialog
{
public:
    GuiMenuTimeAttack(const Config& config);
    ~GuiMenuTimeAttack();

    virtual bool Load(const char* filename_, bool reloadLast_=false);
    virtual void Show();

protected:

    void RemoveFunctors();
    bool OnButtonExportGhost(const irr::SEvent &event_);
    bool OnButtonStart(const irr::SEvent &event_);
    bool OnButtonBack(const irr::SEvent &event_);
    bool OnSliderTracks(const irr::SEvent &event_);
    bool OnSliderDifficulty(const irr::SEvent &event_);
    bool OnSliderRaceMode(const irr::SEvent &event_);
    bool OnSliderGhost(const irr::SEvent &event_);
    bool OnExportAcknowledged(const irr::SEvent &event_);

    LevelDifficulty GetDifficulty() const;
    GAME_TYPE GetGameType() const;
    GHOST_TYPE GetGhostType() const;
    void GetAllGhostRecordsFromDisk();
    bool GetSelectedGhostRecordSettings(GhostRecordSettings &settings_);

    // exchangable ghost records will overwrite each other on disk
    bool IsExchangableGhostRecord( const GhostRecordSettings &r1_, const GhostRecordSettings &r2_ );

private:
    irr::gui::IGUIStaticText* mActiveProfile;
    irr::gui::IGUITextSlider* mSliderTracks;
    irr::gui::IGUITextSlider* mSliderDifficulty;
    irr::gui::IGUITextSlider* mSliderRaceMode;
    irr::gui::IGUITextSlider* mSliderGhost;
    irr::gui::IGUIStaticText* mStaticHighscore;
    irr::gui::IGUIStaticText* mStaticScoreGhost;
    irr::gui::IGUIStaticText* mStaticTextTargetTime;
    irr::gui::IGUIStaticText* mStaticTimeTargetTime;
    irr::gui::IGUIButton* mButtonExportGhost;
    irr::gui::IGUIButton* mButtonStart;
    irr::gui::IGUIButton* mButtonBack;

    std::vector<GhostRecordSettings> mGhostLapRecords;
    std::vector<GhostRecordSettings> mGhostTrackRecords;
};


#endif // GUI_MENU_TIMEATTACK_H
