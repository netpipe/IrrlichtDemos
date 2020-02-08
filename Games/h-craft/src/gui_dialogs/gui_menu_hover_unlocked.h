// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#ifndef GUI_MENU_HOVER_UNLOCKED_H
#define GUI_MENU_HOVER_UNLOCKED_H

#include "../gui_dialog.h"

class GuiMenuHoverUnlocked : public GuiDialog
{
public:
    GuiMenuHoverUnlocked(const Config& config);
    ~GuiMenuHoverUnlocked();

    virtual bool Load(const char* filename_, bool reloadLast_=false);
    void SetHover(unsigned int idx_);

protected:
    void RemoveFunctors();
    bool OnButtonContinue(const irr::SEvent &event_);

private:
    irr::gui::IGUIImage*        mImageHover;
};

#endif // GUI_MENU_HOVER_UNLOCKED_H

