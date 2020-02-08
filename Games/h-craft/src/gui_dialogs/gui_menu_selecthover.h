// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#ifndef GUI_MENU_SELECTHOVER_H
#define GUI_MENU_SELECTHOVER_H

#include "../gui_dialog.h"

class GuiMenuSelectHover : public GuiDialog
{
public:
    GuiMenuSelectHover(const Config& config);
    ~GuiMenuSelectHover();

    virtual bool Load(const char* filename_, bool reloadLast_=false);
    virtual void Show();

protected:

    void RemoveFunctors();
    bool OnButtonAccept(const irr::SEvent &event_);
    bool OnButtonCancel(const irr::SEvent &event_);
    bool OnSliderHover(const irr::SEvent &event_);
    void SelectHover();

private:
    irr::gui::IGUIImage*        mImageHover;
    irr::gui::IGUITextSlider*   mSliderHover;
    irr::gui::IGUIStaticText*   mUnlock;
    irr::gui::IGUIButton*       mButtonAccept;
    irr::gui::IGUIStaticText*   mActiveProfile;
};

#endif // GUI_MENU_SELECTHOVER_H
