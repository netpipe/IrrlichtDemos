#pragma once

using namespace irr;
using namespace gui;
#include "GUI.h"
class MyEventReceiver : public IEventReceiver
{
	IrrlichtDevice* device;
public:
	void setDevice(IrrlichtDevice* dev)		
	{
		device = dev;
	}
	// This is the one method that we have to implement
	virtual bool OnEvent(const SEvent& event)
	{
		// Remember whether each key is down or up
		if (event.EventType == irr::EET_KEY_INPUT_EVENT)
			KeyIsDown[event.KeyInput.Key] = event.KeyInput.PressedDown;



		  if (event.EventType == EET_GUI_EVENT) // если условие истинно, то это событие интерфейса
                {
                        s32 id = event.GUIEvent.Caller->getID(); // получаем идентификатор элемента, вызвавшего событие
                        IGUIEnvironment* env = device->getGUIEnvironment();

                        switch(event.GUIEvent.EventType) // обрабатываем события относительно типа
                        {
							
								  case EGET_BUTTON_CLICKED:
                                switch(id)
                                {
								case GUI_EXIT_BUTTON: 
                                      device->closeDevice();
                                        return true;

                                case GUI_NEW_GAME_BUTTON: 
									{

										GUI::getInstance()->setMenuState(-1);
									}
									return true;

                                case GUI_CONTINUE_BUTTON: 
								{
									GUI::getInstance()->setMenuState(0);
								}        
								return true;

								

                                default:
                                        return false;
                                }
                                break;

                        
                        }
                }

         



		return false;
	}

	// This is used to check whether a key is being held down
	virtual bool WasKeyDown()const
	{
		for (u32 i=0; i<KEY_KEY_CODES_COUNT; ++i)
			if(KeyIsDown[i])
				return true;
		return false;
	}
	virtual bool IsKeyDown(EKEY_CODE keyCode) const
	{
		return KeyIsDown[keyCode];
	}
	
	MyEventReceiver()
	{
		for (u32 i=0; i<KEY_KEY_CODES_COUNT; ++i)
			KeyIsDown[i] = false;
	}

private:
	// We use this array to store the current state of each key
	bool KeyIsDown[KEY_KEY_CODES_COUNT];
};