// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#ifndef GUI_CONTROLLER_DISPLAY_H
#define GUI_CONTROLLER_DISPLAY_H

#include "../irrlicht_manager.h"

class Controller;

class ControllerDisplay : public irr::gui::IGUIElement
{
public:
    ControllerDisplay(irr::gui::IGUIEnvironment * environment_, irr::gui::IGUIElement * parent_, irr::s32 id_, irr::core::rect< irr::s32 > rectangle_);
	virtual ~ControllerDisplay();

	virtual void draw();

	void SetController(Controller * controller_);

private:
    Controller * mController;
};

#endif // GUI_CONTROLLER_DISPLAY_H
