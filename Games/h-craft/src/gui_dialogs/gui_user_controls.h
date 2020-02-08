// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#ifndef GUI_USER_CONTROLS_H
#define GUI_USER_CONTROLS_H

#include "../irrlicht_manager.h"

class GuiUserControls
{
public:
    GuiUserControls(irr::gui::IGUIEnvironment* env_);
    ~GuiUserControls();

    void CreateUserControls();

    void ShowUserControlTabs();
    void ShowWaitForUserAxis();
    void ShowWaitForUserButton();

    bool OnEvent(const irr::SEvent &event);

protected:
    void ClearWndUserControls();

private:
    irr::gui::IGUIEnvironment*  mEnvironment;
    irr::gui::IGUIWindow* 		wndUserControls;
    irr::core::rect<irr::s32>   wndRectUserControls;
};

#endif // GUI_USER_CONTROLS_H
