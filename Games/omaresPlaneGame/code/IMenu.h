

#ifndef _IMENU_H_
#define _IMENU_H_

#include <irrlicht.h>
#include "game.h"

class IMenu : public IEventReceiver
{
public:

	IMenu::IMenu()
	{
	 resX = 1280;
	 resY = 1024;
		 fogOfWarEnable = false;
		 fullscreen = true;

		startGame = false;
	}

	IMenu::~IMenu()
	{
	
	}

	bool IMenu::run()
	{
		device = createDevice(video::EDT_SOFTWARE, core::dimension2d<s32>(512, 256), 32, false, true, this);

		driver = device->getVideoDriver();
		guiEnv = device->getGUIEnvironment();
	
		createWindow();

		fontVeranda16 = guiEnv->getFont("../Game Data/font/fontVeranda_16.jpg");

		if (fontVeranda16)
			guiEnv->getSkin()->setFont(fontVeranda16);

		guiEnv->getSkin()->setColor(gui::EGDC_HIGH_LIGHT, video::SColor(255, 30, 30, 70));
		guiEnv->getSkin()->setColor(gui::EGDC_BUTTON_TEXT, video::SColor(255,37,118,171));
	
		while(device->run())
		{
			driver->beginScene(true, true, video::SColor(255,0,0,0));
			guiEnv->drawAll();
			driver->endScene();
		}

		device->drop();
	
		return startGame;
	}

	void createWindow()
	{
		windowBackground =				guiEnv->addImage(core::rect<int>(0,0,512,256));
			windowBackground->setImage(driver->getTexture("../Game Data/menu/menuBackground.jpg"));

		fullScreenCheck = guiEnv->addCheckBox(true, core::rect<int>(60,110,175,140), 0, 1, L"Full Screen");

		resolutionListBox = guiEnv->addListBox( core::rect<int>(25,150,232,220) );
			lowRes = resolutionListBox->addItem(L"1024x768");
			highRes = resolutionListBox->addItem(L"1280x1024");
			resolutionListBox->setSelected(highRes);

		startButton = guiEnv->addButton(core::rect<int>(265,170,490,230), 0, 100, L"Start");
			startButton->setImage(driver->getTexture("../Game Data/menu/startButton.jpg"), irr::core::rect<int>(0,0,224,59));
	}


	virtual bool IMenu::OnEvent(SEvent event)
	{
		if (event.EventType == EET_GUI_EVENT)
		{
			int id = event.GUIEvent.Caller->getID();

			switch(event.GUIEvent.EventType)
			{
			case gui::EGET_BUTTON_CLICKED:

				// *** Triangle Buttons ***

				if (id == 100)
				{
					if ( resolutionListBox->getSelected() == lowRes )
					{
						resX = 1024;
						resY = 768;
					}

					else if ( resolutionListBox->getSelected() == highRes )
					{
						resX = 1280;
						resY = 1024;
					}

					if ( fullScreenCheck->isChecked() == true )
					{
						fullscreen = true;
					}
					else
					{
						fullscreen = false;
					}

					startGame = true;
					device->closeDevice();
				}
			break;
			}

			return true;
		}

		return false;
	}

	

protected:

	irr::IrrlichtDevice *device;
	irr::gui::IGUIEnvironment* guiEnv;
	irr::video::IVideoDriver* driver;
	irr::gui::IGUIImage* windowBackground;



	int lowRes;
	int highRes;

	irr::gui::IGUIListBox*				resolutionListBox;
	irr::gui::IGUISkin*					skin;
	irr::gui::IGUIFont*					fontShruti6;
	irr::gui::IGUIFont*					fontShruti10;
	irr::gui::IGUIFont*					fontTrebucheat14B;
	irr::gui::IGUIFont*					fontVeranda16;
	irr::gui::IGUIButton*				startButton;
	irr::gui::IGUICheckBox*				fullScreenCheck;

};

#endif

