// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#ifndef GUI_DEVELOPER_SETTINGS_H
#define GUI_DEVELOPER_SETTINGS_H

#include "../irrlicht_manager.h"

class GuiDeveloperSettings
{
public:
    GuiDeveloperSettings(irr::gui::IGUIEnvironment* env_);
    ~GuiDeveloperSettings();

    void CreateDeveloperSettings();
    bool OnEvent(const irr::SEvent &event);

private:
    irr::gui::IGUIEnvironment*   mEnvironment;
    irr::gui::IGUIWindow* mWindow;
    int mLastOpenTab;
};

#endif // GUI_DEVELOPER_SETTINGS_H
