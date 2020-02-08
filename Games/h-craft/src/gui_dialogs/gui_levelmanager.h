// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#ifndef GUI_LEVELMANAGER_H
#define GUI_LEVELMANAGER_H

#include "../irrlicht_manager.h"

class GuiLevelManager
{
public:
    GuiLevelManager(irr::gui::IGUIEnvironment* env_);
    ~GuiLevelManager();

    void ShowLevelSelection();
    void CloseLevelSelection();
    bool OnEvent(const irr::SEvent &event);

private:
    irr::gui::IGUIEnvironment*   mEnvironment;
    irr::gui::IGUIListBox * mListLevels;
};

#endif // GUI_LEVELMANAGER_H
