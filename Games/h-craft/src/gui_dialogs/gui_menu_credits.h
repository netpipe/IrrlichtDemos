// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#ifndef GUI_MENU_CREDITS_H
#define GUI_MENU_CREDITS_H

#include "../gui_dialog.h"

class GuiMenuCredits : public GuiDialog
{
public:
    GuiMenuCredits(const Config& config);
    ~GuiMenuCredits();

    virtual bool Load(const char* filename_, bool reloadLast_=false);

protected:
    void RemoveFunctors();
    bool OnButtonLicenses(const irr::SEvent &event_);
    bool OnButtonBack(const irr::SEvent &event_);
};

#endif // GUI_MENU_CREDITS_H
