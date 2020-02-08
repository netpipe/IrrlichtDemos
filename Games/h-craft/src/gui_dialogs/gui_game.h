// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#ifndef GUI_GAME_H
#define GUI_GAME_H

#include "../irrlicht_manager.h"

class GuiGame
{
public:
    GuiGame(irr::gui::IGUIEnvironment* env_);
    ~GuiGame();

    void CreateGameGui();
    void HideGameGui();

    bool OnEvent(const irr::SEvent &event);

    void SetGameTime(irr::u32 time_);
    void SetTrackTime(irr::u32 time_);
    void SetBestTrackTime(irr::u32 time_);

private:
    irr::gui::IGUIEnvironment*   mEnvironment;
    irr::gui::IGUIStaticText *    mStaticGameTime;
    irr::gui::IGUIStaticText *    mStaticTrackTime;
    irr::gui::IGUIStaticText *    mStaticBestTrackTime;
};

#endif // GUI_GAME_H
