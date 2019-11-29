#ifndef _OPTIONS_H_
#define _OPTIONS_H_

#include <irrlicht.h>
using namespace irr;

#include "CStateSys.h"

class COptionsState : public CState
{
	public:
		COptionsState(CStateManager* man = NULL, CState* par = NULL)
			: CState(man, par)
		{
			device = manager->getDevice();
			guiEnv = device->getGUIEnvironment();
			
			core::dimension2d<s32> screenSize = device->getVideoDriver()->getScreenSize();
			
			window = guiEnv->addWindow(core::rect<s32>((screenSize.Width / 2) - 200, (screenSize.Height / 2) - 100,
				(screenSize.Width / 2) + 200, (screenSize.Height / 2) + 100),
				true, // modal?
				L"Options");
			
			window->getCloseButton()->remove();
			
			guiEnv->addButton(core::rect<s32>(5, 175, 50, 195), window, -1,
				L"Okay");
			
			guiEnv->addButton(core::rect<s32>(55, 175, 100, 195), window, -1,
				L"Apply");
			
			guiEnv->addButton(core::rect<s32>(105, 175, 150, 195), window, -1,
				L"Cancel");
			
			gui::IGUITabControl* tabCtrl = guiEnv->addTabControl(core::rect<s32>(5, 30, 395, 170),
				window, true, true);
			
			gui::IGUITab* videoTab = tabCtrl->addTab(L"Video");
			gui::IGUITab* keysTab = tabCtrl->addTab(L"Keys");
			
			elements.push_back(window);
		}
		
		bool onEvent(const SEvent& event)
		{
			if(event.EventType == EET_GUI_EVENT
				&& event.GUIEvent.Caller->getParent() == window)
			{
				core::stringw name = core::stringw(event.GUIEvent.Caller->getText());
				
				if(event.GUIEvent.EventType == gui::EGET_BUTTON_CLICKED)
				{
					if(name == "Okay")
						kill();
					else if(name == "Cancel")
						kill();
				}
			}
			
			return false;
		}
	
	protected:
		IrrlichtDevice* device;
		gui::IGUIEnvironment* guiEnv;
		
		gui::IGUIWindow* window;
};

#endif
