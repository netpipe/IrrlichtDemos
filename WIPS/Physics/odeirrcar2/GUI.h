#pragma once
#include "includes.h"

enum
{
	GUI_EXIT_BUTTON,
	GUI_NEW_GAME_BUTTON,
	GUI_CONTINUE_BUTTON
};

class GUI
{
private:
	 IrrlichtDevice*device;
	 int menu;		//-1 - new game 0 - return to game 1 - main menu 2 - pause menu
	
	GUI()
	{
		menu = true;
	}
	static GUI* instance;
public:
	
	void setDevice(IrrlichtDevice* dev)
	 {
		 device = dev;
	 }
	static GUI* getInstance()
	{
		if(instance == NULL)
			instance = new GUI();      
		
		return instance;
	}

	
	void setMenuState(int set)
	{
		menu = set;
		if(menu < 1)
			dropMainMenu();
		if(menu == 1)
			createMainMenu();
		else
			createPauseMenu();
	}
	int getMenuState()
	{
		return menu;
	}
private:
	void dropMainMenu()
	{
	
		IGUIEnvironment* env = device->getGUIEnvironment();
		env->clear();
			device->getCursorControl()->setVisible(false);
		
		
	}
	void createMainMenu();
	void createPauseMenu();
	
};


/*
class GUI
{
public:
	
	static bool menu;

	static IrrlichtDevice* device;
	


	
	
	
	static void createMainMenu();
	
	static void setMenu(bool set)
	{
		menu = set;
	}
	static bool isMenu()
	{
		return menu;
	}
	static void dropMainMenu()
	{
		IGUIEnvironment* env = device->getGUIEnvironment();
		env->clear();
		
	}
};


*/