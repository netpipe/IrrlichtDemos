// Vehement (C) Steven Dorman 2008
#define	 VEHEMENT_STATEMANAGER_CPP
#define loading_state -1	//Only used for loading a new game or a existing game
#define menu_state 0		//Main menu Splash Screen
#define game_state 1		//In game GUI
#define null_state 420		//NULL

// Include the class definition
#include "statemanager.hpp"

// Reference the factory class
#include "factory.hpp"
class dfactory;

// Using the vehe namespace
namespace vehe 
{
	using namespace ctypes;
	//Initializer
	statemanager::statemanager ()	
	{
		mystate = 0;
		newgame = false;
		startingup = false;
		
		// Store device pointers for use
		root = dfactory::instance()->getIrrDevice()->
				getGUIEnvironment()->getRootGUIElement();

		device = dfactory::instance()->getIrrDevice();
		
		guienv = dfactory::instance()->getIrrDevice()->getGUIEnvironment();
		
		driver = dfactory::instance()->getIrrDevice()->getVideoDriver();

		smgr  = dfactory::instance()->getIrrDevice()->getSceneManager();

		audio =  dfactory::instance()->getSoundDevice();

		//Load the GUI
		statemanager::loadGUI();
	};

	//! Process the game state manager 
	void statemanager::doState ()
	{
		switch (statemanager::mystate)
		{
		case loading_state:
			//Set all the loading state GUI elements to visible
			for (u32 i = 0; i < elements.size(); i++)
				if (stateswitch.at(i) == loading_state)
					elements.at(i)->setVisible(true);
				else
					elements.at(i)->setVisible(false);

			//Create a new game or load an existing one
			if (startingup)
				if (newgame)
				{
					dfactory::instance()->getPlayerManager()->newPlayer();
					device->getCursorControl()->setVisible(false);
					dfactory::instance()->setIsPaused(false);
						mystate = game_state;
				}
				else
				{
					dfactory::instance()->getPlayerManager()->loadPlayer("./data/saves/player1.cobj");
					device->getCursorControl()->setVisible(false);
					dfactory::instance()->setIsPaused(false);
						mystate = game_state;
				}
			else
				startingup = true;		
		break;

		case menu_state:
				//Set all the menu state GUI elements to visible
				for (u32 i = 0; i < elements.size(); i++)
					if (stateswitch.at(i) == menu_state)
						elements.at(i)->setVisible(true);
					else
						elements.at(i)->setVisible(false);

					audio->stopAllSounds();
					audio->setSoundVolume(0.5f);
					audio->play2D("./data/audio/mainmenu.wav", true);

				mystate = null_state;
			break;

		case game_state:
				//Set all game state GUI elements to visible
				for (u32 i = 0; i < elements.size(); i++)
					if (stateswitch.at(i) == game_state)
						elements.at(i)->setVisible(true);
					else
						elements.at(i)->setVisible(false);

				//Show the map editor icon
				if (dfactory::instance()->getMapEditor()->getActive())
					root->getElementFromId(202)->setVisible(true);
				else
					root->getElementFromId(202)->setVisible(false);

				//Play music if its not already playing
				if (!audio->isCurrentlyPlaying("./data/audio/ingame.wav"))
				{
					audio->stopAllSounds();
					audio->setSoundVolume(0.3f);
					audio->play2D("./data/audio/ingame.wav", true);
				}
						
				mystate = null_state;
			break;

		case null_state:
			break;

		default:
			break;
		};
		
	}

	//! Load the games GUI components
	void statemanager::loadGUI()
	{
		// Load the loading menu GUI
		guienv->addImage(driver->getTexture("./data/graphics/gui/splash01.jpg"),
		core::position2d<s32>(0,0),true,0,80);
		elements.push_back(root->getElementFromId(80));
		stateswitch.push_back(loading_state);

		guienv->addImage(driver->getTexture("./data/graphics/gui/loading.tga"),
		core::position2d<s32>(800/2-150,600/2-60), true,0,81);
		elements.push_back(root->getElementFromId(81));
		stateswitch.push_back(loading_state);	

		// Load the main menu GUI
		guienv->addImage(driver->getTexture("./data/graphics/gui/splash01.jpg"),
		core::position2d<s32>(0,0),true,0,101);
		elements.push_back(root->getElementFromId(101));
		stateswitch.push_back(menu_state);

		guienv->addImage(driver->getTexture("./data/graphics/gui/logo.tga"),
				core::position2d<s32>(0,0), true,0,102);
		elements.push_back(root->getElementFromId(102));
		stateswitch.push_back(menu_state);

		guienv->addImage(driver->getTexture("./data/graphics/gui/newgame.tga"),
				core::position2d<s32>(800/2-100,600/2-30), true,0,103);
		elements.push_back(root->getElementFromId(103));
		stateswitch.push_back(menu_state);		

		guienv->addImage(driver->getTexture("./data/graphics/gui/loadgame.tga"),
				core::position2d<s32>(800/2-100,600/2+60), true,0,104);
		elements.push_back(root->getElementFromId(104));
		stateswitch.push_back(menu_state);	

		//Load the ingame GUI
		statemanager::loadHUD();

		//Make all of them invisible		
		for (u32 i = 0; i < elements.size(); i++)
			elements.at(i)->setVisible(false);

	}

	//! Load the games GUI components
	void statemanager::loadHUD()
	{
		guienv->addImage(driver->getTexture("./data/graphics/gui/menubar.tga"),
						 core::position2d<s32>(0,0), true,0,201);
		elements.push_back(root->getElementFromId(201));
		stateswitch.push_back(game_state);

		guienv->addImage(driver->getTexture("./data/graphics/gui/targeticon.jpg"),
		core::position2d<s32>(675,25), true,0, 202);
		elements.push_back(root->getElementFromId(202));
		stateswitch.push_back(game_state);

		guienv->addImage(driver->getTexture("./data/graphics/gui/logosm.tga"),
				core::position2d<s32>(0,0), true,0,203);
		elements.push_back(root->getElementFromId(203));
		stateswitch.push_back(game_state);
	}

	//! Process click requests state GUI
	void statemanager::processGUI(int x, int y)
	{
		IGUIElement * mygui = root->getElementFromPoint(position2d<s32>(x,y));
		
		if (mygui)
		switch (mygui->getID())
		{
		case 103: // NEW GAME BUTTON
			newgame = true;
			statemanager::setState(loading_state);
			break;
		case 104: // LOAD GAME BUTTON
			newgame = false;
			statemanager::setState(loading_state);
			break;
		};
	}

};