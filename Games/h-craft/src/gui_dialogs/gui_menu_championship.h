// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#ifndef GUI_MENU_CHAMPIONSHIP_H
#define GUI_MENU_CHAMPIONSHIP_H

#include "../gui_dialog.h"

class GuiMenuChampionship : public GuiDialog
{
public:
    GuiMenuChampionship(const Config& config);
    ~GuiMenuChampionship();

    virtual bool Load(const char* filename_, bool reloadLast_=false);
    virtual void Show();

protected:
    void RemoveFunctors();
    bool OnButtonContinue(const irr::SEvent &event_);
    bool OnButtonNew(const irr::SEvent &event_);
    bool OnNewAcknowledged(const irr::SEvent &event_);
    bool OnButtonBack(const irr::SEvent &event_);

private:
    irr::gui::IGUIStaticText* mActiveProfile;
    irr::gui::IGUIButton*     mButtonNew;
    irr::gui::IGUIButton*     mButtonContinue;
};

#endif // GUI_MENU_CHAMPIONSHIP_H
