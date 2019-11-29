#ifndef _MENU_H_
#define _MENU_H_

#include "CStateSys.h"
#include <irrlicht.h>
	using namespace irr;
	using namespace gui;

#include "commonStates.h"
#include "options.h"
#include "scene.h"

class CMenuState : public CState
{
	public:
		CMenuState(CStateManager* man = NULL, CState* par = NULL)
			: CState(man, par)
		{
			device = manager->getDevice();
			guiEnv = device->getGUIEnvironment();
			
			manager->setColor(video::SColor(255, 0, 0, 0));
			manager->setRenderScene(false);
			
			const core::dimension2d<s32>* size = &device->getVideoDriver()->getScreenSize();
			
			s32 offsetX = 10;
			s32 offsetY = 10;
			
			elements.push_back(guiEnv->addButton(core::rect<s32>(offsetX, offsetY, 100 + offsetX, offsetY + 32), 0, -1,
				L"Look at Fascinating Scene", L"Don't. Bad things happen to good people."));
			
			elements.push_back(guiEnv->addButton(core::rect<s32>(offsetX, offsetY + (32 * 3), 100 + offsetX, offsetY + (32 * 4)), 0, -1,
				L"Broken Options", L"This doesn't work."));
			
			elements.push_back(guiEnv->addButton(core::rect<s32>(offsetX, offsetY + (32 * 5), 100 + offsetX, offsetY + (32 * 6)), 0, -1,
				L"Quit", L"Quit."));
		}
		
		bool onEvent(const SEvent& event)
		{
			if(CState::onEvent(event) == true)
				return true;
			
			if(event.EventType == EET_GUI_EVENT)
			{
				core::stringw name = core::stringw(event.GUIEvent.Caller->getText());
				
				if(event.GUIEvent.EventType == gui::EGET_BUTTON_CLICKED)
				{
					if(name == "Quit")
						CQuitState* quitState = new CQuitState(manager, this);
					else if(name == "Broken Options")
						COptionsState* optionsState = new COptionsState(manager, this);
					else if(name == "Look at Fascinating Scene")
					{
						CSceneState* sceneState = new CSceneState(manager);
						manager->setState(sceneState);
					}
				}
			}
			
			return false;
		}
	
	private:
		IrrlichtDevice* device;
		IGUIEnvironment* guiEnv;
};

#endif
