// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#ifndef GUI_MENU_ANALOG_H
#define GUI_MENU_ANALOG_H

#include "../gui_dialog.h"

class GuiMenuAnalog : public GuiDialog
{
public:
    GuiMenuAnalog(const Config& config);
    ~GuiMenuAnalog();

    virtual bool Load(const char* filename_, bool reloadLast_=false);
    virtual void Show();

protected:

    void RemoveFunctors();
    bool OnButtonAccept(const irr::SEvent &event_);
    bool OnButtonCancel(const irr::SEvent &event_);
    bool OnButtonCalibrate(const irr::SEvent &event_);
    bool OnSliderRange(const irr::SEvent &event_);
    bool OnSliderDead(const irr::SEvent &event_);

private:
    irr::gui::IGUITextSlider* mSliderRange;
    irr::gui::IGUITextSlider* mSliderDead;
    irr::gui::IGUIStaticText* mActiveProfile;
};


#endif // GUI_MENU_ANALOG_H
