#include "stdafx.h"
#include "init.h"




using namespace irr;
using namespace std;
using namespace irrklang;


//add irrlicht and irrklang libraries
#ifdef _IRR_WINDOWS_
#pragma comment(lib, "Irrlicht.lib")
#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#pragma comment(lib,"irrKlang.lib")
#endif





//initializer
initGame::initGame():graphics(0), time(0.f), sound(0)
{

	//initialize the graphics device and the sound engine
	graphics = createDevice(video::EDT_DIRECT3D9, core::dimension2d<u32>(1280,800),32,false,true,false,0);
	sound = createIrrKlangDevice();


	//create key receiver
	receiver = new KeyListener;
	graphics->setEventReceiver(receiver);
	graphics->getVideoDriver()->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS,true);
	//setup sound
	sound->setSoundVolume(1.f);
	sound->setDefault3DSoundMinDistance(200);

	bool start;
	start=false;

	//Heres the menu stuff
	//Setup everything for 3d skybox
	//Add buttons
	//break loop when done
	core::dimension2d<u32> t = graphics->getVideoDriver()->getScreenSize();
	gui::IGUIFont *menu_font= graphics->getGUIEnvironment()->getFont("res/font/large.xml");
	gui::IGUIStaticText *text = graphics->getGUIEnvironment()->addStaticText(L"Space Sim",core::rect<int>(t.Width/2-80,100,t.Width/2+100,200));
	text->setOverrideColor(video::SColor(255,255,255,255));
	text->setOverrideFont(menu_font);
	graphics->setWindowCaption(L"Space Sim");

	gui::IGUIButton *newgame = graphics->getGUIEnvironment()->addButton(core::rect<int>(t.Width/2-50,t.Height/2-40,t.Width/2+50,t.Height/2-20),0,0,L"New Game");
	gui::IGUIButton *quit = graphics->getGUIEnvironment()->addButton(core::rect<int>(t.Width/2-50,t.Height/2,t.Width/2+50,t.Height/2+20),0,0,L"Quit");


	//setup menu skybox
	/*
	scene::ISceneNode *skybox = graphics->getSceneManager()->addSkyBoxSceneNode(
		graphics->getVideoDriver()->getTexture("res/textures/skyboxes/2/space_top3.jpg"),
		graphics->getVideoDriver()->getTexture("res/textures/skyboxes/2/space_bottom4.jpg"),
		graphics->getVideoDriver()->getTexture("res/textures/skyboxes/2/space_left2.jpg"),
		graphics->getVideoDriver()->getTexture("res/textures/skyboxes/2/space_right1.jpg"),
		graphics->getVideoDriver()->getTexture("res/textures/skyboxes/2/space_front5.jpg"),
		graphics->getVideoDriver()->getTexture("res/textures/skyboxes/2/space_back6.jpg"));
		*/
	scene::ICameraSceneNode *cam = graphics->getSceneManager()->addCameraSceneNode();
	while(graphics->run())
	{
		
		if(newgame->isPressed())
		{
			start = true;
			break;
		}
		if(quit->isPressed())
		{
			break;
		}
		
		graphics->getVideoDriver()->beginScene(true, true, video::SColor(255,100,100,140));
		graphics->getSceneManager()->drawAll();
		graphics->getGUIEnvironment()->drawAll();

		graphics->getVideoDriver()->endScene();
	}
	//delete gui elements
	text->remove();
	//skybox->remove();
	cam->remove();
	newgame->remove();
	quit->remove();



	//create game object
	//the start bool is necessary
	//so when the player exits from the menu
	//the game doesn't load everything into memory
	if(start==true)
	{
		Game = new GameLoop(graphics,receiver, sound);
		StartGame();
	}
}

//destructor
initGame::~initGame()
{
	//delete
	graphics->drop();
	sound->drop();
}


int initGame::StartGame()
{


	if(!graphics)
	{
		return 1;	//returns 1
	}
	if(!sound)
	{
		return 2;
	}


	while(graphics->run())
	{
		//main graphics loop


		Game->Run();	//run game


		//disp fps
		int fps = graphics->getVideoDriver()->getFPS();
		core::stringw c(L"Space Sim ");
		c+=fps;
		graphics->setWindowCaption(c.c_str());

		//graphics crap
		graphics->getVideoDriver()->beginScene(true, true, video::SColor(255,100,100,140));
		graphics->getSceneManager()->drawAll();
		graphics->getGUIEnvironment()->drawAll();

		graphics->getVideoDriver()->endScene();

		//delete this later
		if(receiver->IsKeyDown(irr::KEY_ESCAPE))
			break;
	}
	return 0;
}

