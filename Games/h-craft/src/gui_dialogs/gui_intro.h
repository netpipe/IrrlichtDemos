// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#ifndef GUI_INTRO_H
#define GUI_INTRO_H

#include "../gui_dialog.h"

class GuiIntro : public GuiDialog
{
public:
    GuiIntro(const Config& config);
    virtual ~GuiIntro();
};

#endif // GUI_INTRO_H
