#include "irrlicht.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;


void loadMainMenu()
{

    video::IVideoDriver* driver = device->getVideoDriver();
	//ISceneManager* smgr = device->getSceneManager();
	gui::IGUIEnvironment* guienv = device->getGUIEnvironment();

   //show main menu back image
    isBackImg = 1;
    isInGame  = 0;

    //show the cursor
    device->getCursorControl()->setVisible(true);

    screen_fader_black = guienv->addInOutFader();
    screen_fader_black->setColor(video::SColor(0,200,200,200));

    screenShot_fader = guienv->addInOutFader();
    screenShot_fader->setColor(video::SColor(0,255,255,255));

    escFadeToBlack = guienv->addInOutFader();
    escFadeToBlack->setColor(video::SColor(0,0,0,0));

    driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, true);

    //load background for main menu + please wait screen + instructions screens
    mainMenuGUI        = driver->getTexture("data/gui/main_menu/hud/mainmenu.png");
    pleaseWaitImg      = driver->getTexture("data/gui/main_menu/hud/pleasewait.jpg");
	companyInfo        = driver->getTexture("data/gui/main_menu/hud/selogo.png");
	EWlogo             = driver->getTexture("data/gui/main_menu/hud/ewlogo.png");
	levelPreview       = driver->getTexture("data/gui/main_menu/hud/levelPreview.jpg");
	levelSelectBk      = driver->getTexture("data/gui/main_menu/hud/selectMaze.png");
	instructionsBk     = driver->getTexture("data/gui/main_menu/hud/instructions.png");
	optionsBk          = driver->getTexture("data/gui/main_menu/hud/options.png");
	creditsBk          = driver->getTexture("data/gui/main_menu/hud/credits.png");
	winScreen          = driver->getTexture("data/gui/main_menu/hud/winScreen.png");
	gameOverScreen	   = driver->getTexture("data/gui/main_menu/hud/gameOverScreen.png");



    guienv->loadGUI("data/logic/config/main_menu_skin.xml");

    IGUISkin* game_skin = guienv->getSkin();
    IGUIFont* menu_font = guienv->getFont("data/gui/fonts/f1.xml");
        if (menu_font)
                game_skin->setFont(menu_font);


   //create credits window
   credits_window = guienv->addWindow(rect<s32>(0, 0, 800, 600), false, L"");
   guienv->addImage(creditsBk, core::position2d<s32>(0, 0), false, credits_window);
   guienv->addButton(rect<s32>(589, 564, 759, 593), credits_window, 106, L"CLOSE");
   credits_window->setVisible(false);


	//instructions window
	instructions_window = guienv->addWindow(rect<s32>(0, 0, 800 ,600), false, L"");
	instructions_window->getCloseButton()->setVisible(false);
    guienv->addImage(instructionsBk, core::position2d<s32>(0, 0), false, instructions_window);
	guienv->addButton(rect<s32>(589, 564, 759, 593), instructions_window, 109, L"CLOSE");
    instructions_window->setVisible(false);

	//options window
	options_window = guienv->addWindow(rect<s32>(0, 0, 800 ,600), false, L"");
	options_window->getCloseButton()->setVisible(false);
    guienv->addImage(optionsBk, core::position2d<s32>(0, 0), false, options_window);
	guienv->addButton(rect<s32>(500, 564, 759, 593), options_window, 110, L"SAVE AND CLOSE");

	if (wfullscreen==1){ isWFullscreen = 1; }else{ isWFullscreen = 0;}
	guienv->addCheckBox(isWFullscreen, rect<s32>(300, 150, 590, 200), options_window, 301, L"FULLSCREEN");

	if (musicOn==1){ isMusicOn = 1; }else{ isMusicOn = 0;}
	guienv->addCheckBox(isMusicOn, rect<s32>(300, 220, 590, 270), options_window, 302, L"MUSIC ON");

	if (sfxOn==1){ isSfxOn = 1; }else{ isSfxOn = 0;}
	guienv->addCheckBox(isSfxOn, rect<s32>(300, 290, 590, 340), options_window, 303, L"AUDIO SFX ON");

	if (skipIntro==1){ isSkipIntro = 1; }else{ isSkipIntro = 0;}
	guienv->addCheckBox(isSkipIntro, rect<s32>(300, 360, 590, 410), options_window, 304, L"SKIP INTRO");

    options_window->setVisible(false);


    //single player
    singleplayer_window = guienv->addWindow(rect<s32>(0, 0, 800 ,600), false, L"");
    singleplayer_window->getCloseButton()->setVisible(false);
    guienv->addImage(levelSelectBk, core::position2d<s32>(0, 0), false, singleplayer_window);

    //add maps list
    maps_list = guienv->addListBox(rect<s32>(42,67, 395, 530), singleplayer_window, -1, true);

   irr::io::IXMLReader* xml2 = device->getFileSystem()->createXMLReader("data/maps/map_list.xml");
    //load maps list
    while(xml2 && xml2->read())
   {
      if (core::stringw("maps") == xml2->getNodeName())
      {
      XMLmapCount  = xml2->getAttributeValueAsInt(L"count");
      }

      for( s32 j=0; j < XMLmapCount; ++j)//was u32
      {
      if (core::stringw(j) == xml2->getNodeName())
         {
         XMLmapName  = xml2->getAttributeValue(L"map_name");
         maps_list->addItem(XMLmapName.c_str());
         }
      }

   }
    maps_list->setSelected(0); //automaticly select first map

    guienv->addButton(rect<s32>(589, 564, 759, 593), singleplayer_window, 104, L"LOAD MAZE");
    guienv->addButton(rect<s32>(405, 564, 575, 593), singleplayer_window, 105, L"CANCEL");

	guienv->addImage(levelPreview, core::position2d<s32>(404, 66), false, singleplayer_window);

	//maze text description holder
	briefingBox = guienv->addEditBox(L"", core::rect<s32>(406,310,758,528), false, singleplayer_window, 256);
	briefingBox->setMultiLine(true);
	briefingBox->setWordWrap(true);
	briefingBox->setTextAlignment(EGUIA_UPPERLEFT, EGUIA_UPPERLEFT);
	briefingBox->setEnabled(false);

    singleplayer_window->setVisible(false);


   //add the start game button
   startGameBtn    = guienv->addButton(rect<s32>(screenResX/3,screenResY/2+0,screenResX/3+screenResX/3,screenResY/2+40), 0, 101, L"START GAME");

   //add the Instructions button
   instructionsBtn = guienv->addButton(rect<s32>(screenResX/3,screenResY/2+50,screenResX/3+screenResX/3,screenResY/2+90), 0, 107, L"INSTRUCTIONS");

   //add the options screen
   optionsBtn      = guienv->addButton(rect<s32>(screenResX/3,screenResY/2+100,screenResX/3+screenResX/3,screenResY/2+140), 0, 108, L"OPTIONS");

   //add the credits button
   creditsBtn      = guienv->addButton(rect<s32>(screenResX/3,screenResY/2+150,screenResX/3+screenResX/3,screenResY/2+190), 0, 102, L"CREDITS");

   //add the exit game button
   quitGameBtn     = guienv->addButton(rect<s32>(screenResX/3,screenResY/2+200,screenResX/3+screenResX/3,screenResY/2+240), 0, 103, L"QUIT GAME");


   //fade the screen, just a cool effect
   screen_fader_black->fadeIn(1000);


   if (musicOn==1){ systemFMOD->playSound(FMOD_CHANNEL_FREE, bkMainMenuSound, 0, &channel1); }else{}

   printf("[INFO] Main Menu loaded.\n");
}
