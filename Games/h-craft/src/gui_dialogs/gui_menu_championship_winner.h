// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#ifndef GUI_MENU_CHAMP_WINNER_H
#define GUI_MENU_CHAMP_WINNER_H

#include "../gui_dialog.h"

class GuiMenuChampWinner : public GuiDialog
{
public:
    GuiMenuChampWinner(const Config& config);
    ~GuiMenuChampWinner();

    virtual bool Load(const char* filename_, bool reloadLast_=false);

protected:
    void RemoveFunctors();
    bool OnButtonContinue(const irr::SEvent &event_);
};

#endif // GUI_MENU_CHAMP_WINNER_H

