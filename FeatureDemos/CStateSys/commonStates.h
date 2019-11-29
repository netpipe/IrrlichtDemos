#ifndef _COMMONSTATES_H_
#define _COMMONSTATES_H_

#include <irrlicht.h>
using namespace irr;

#include "CStateSys.h"

class CQuitState : public CState
{
	public:
		CQuitState(CStateManager* man = NULL, CState* par = NULL)
			: CState(man, par)
		{
			device = manager->getDevice();
			guiEnv = device->getGUIEnvironment();
			
			core::dimension2d<s32> screenSize = device->getVideoDriver()->getScreenSize();
			
			window = guiEnv->addWindow(core::rect<s32>((screenSize.Width / 2) - 80, (screenSize.Height / 2) - 50,
				(screenSize.Width / 2) + 80, (screenSize.Height / 2) + 50),
				true, // modal?
				L"Quitting");
			
			window->getCloseButton()->remove();
			
			guiEnv->addStaticText(L"I'm ",
				core::rect<s32>(10, 30, 30, 50),
				false, false, window);
			
			guiEnv->addStaticText(L"sure I'd like to quit.",
				core::rect<s32>(100, 30, 160 - 10, 80),
				false, true, window);
			
			guiEnv->addButton(core::rect<s32>(35, 30, 90, 50), window, -1,
				L"really");
			
			guiEnv->addButton(core::rect<s32>(35, 60, 90, 80), window, -1,
				L"not");
			
			elements.push_back(window);
		}
		
		bool onEvent(const SEvent& event)
		{
			if(event.EventType == EET_GUI_EVENT
				&& event.GUIEvent.EventType == gui::EGET_BUTTON_CLICKED
				&& event.GUIEvent.Caller->getParent() == window)
			{
				core::stringw name = core::stringw(event.GUIEvent.Caller->getText());
				
				if(name == "really")
					device->closeDevice();
				else if(name == "not")
					kill();
			}
			
			if(event.EventType == irr::EET_KEY_INPUT_EVENT
					&& event.KeyInput.PressedDown == false)
				if(event.KeyInput.Key == irr::KEY_ESCAPE)
					return true;
			
			return false;
		}
	
	protected:
		IrrlichtDevice* device;
		gui::IGUIEnvironment* guiEnv;
		
		gui::IGUIWindow* window;
};

#endif
