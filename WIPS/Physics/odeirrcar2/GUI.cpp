#include "GUI.h"

GUI* GUI::instance = NULL;

 void GUI::createMainMenu()
 {
	 IGUIEnvironment* env = device->getGUIEnvironment();

		
		 env->addButton(rect< s32 >(300,430,600,430 + 100), 0, GUI_NEW_GAME_BUTTON,
                        L"New Game", L"Begin new game");
		 env ->addButton(rect< s32 >(300,540,600,540 + 100), 0, GUI_EXIT_BUTTON,
                        L"Exit", L"Close application");
	
		env->getSkin()->setFont(env->getFont("models/font.png") );

		
	device->getCursorControl()->setVisible(true);
      env->drawAll();
	  
 }
  void GUI::createPauseMenu()
 {
	 IGUIEnvironment* env = device->getGUIEnvironment();

		 env->addButton(rect< s32 >(300,320,600,320 + 100), 0, GUI_CONTINUE_BUTTON,
			 L"Continue", L"Continue old game");
		 createMainMenu();

      env->drawAll();
	  
 }