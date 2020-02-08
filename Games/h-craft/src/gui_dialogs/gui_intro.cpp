// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#include "gui_intro.h"

GuiIntro::GuiIntro(const Config& config)
    : GuiDialog(config)
{
    SetSuppressSceneRendering(true);
}

GuiIntro::~GuiIntro()
{
}
