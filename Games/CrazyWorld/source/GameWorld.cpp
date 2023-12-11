#include "GameWorld.h"
#include "CEventReceiver.h"
#include "Player.h"
#include "GameObject.h"
#include "Enemy.h"
#include "Building.h"
//#include "Effects.h"

GameWorld::GameWorld(void)
{
}
GameWorld::GameWorld(IrrlichtDevice* device)
{
	this->device = device;
	this->driver = device->getVideoDriver();
	this->smgr = device->getSceneManager();
	this->gui = device->getGUIEnvironment();

	//Load GUI stuff
	//load font
	std::string s = "../res/font/fontcourier.bmp";
	font = gui->getFont(s.c_str());

	//setup skybox
	driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, false);
	smgr->addSkyBoxSceneNode(
		driver->getTexture("../res/tex/Sky14/irrlicht2_up.jpg"),//up
		driver->getTexture("../res/tex/Sky14/irrlicht2_dn.jpg"),//down
		driver->getTexture("../res/tex/Sky14/irrlicht2_lf.jpg"),//left
		driver->getTexture("../res/tex/Sky14/irrlicht2_rt.jpg"),//right
		driver->getTexture("../res/tex/Sky14/irrlicht2_ft.jpg"),//front
		driver->getTexture("../res/tex/Sky14/irrlicht2_bk.jpg"));//back

	driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, true);

	this->gomgr = new AI(this->device);

	//gomgr->addObj(new Building(device,"block","a",core::vector3df(120,50,0),core::vector3df(0,0,1),1));
	//gomgr->addObj(new Building(device,"hse","a",core::vector3df(0,50,0),core::vector3df(1,0,1),1));
	//gomgr->addObj(new Building(device,"highr","a",core::vector3df(-120,50,0),core::vector3df(0,0,1),1));
	p = new Player(device, gomgr, "tom", core::vector3df(0,75,75), core::vector3df(0,0,1), 5);

	device->getCursorControl()->setVisible(false);

	CEventReceiver* cer = new CEventReceiver(device, p, gomgr);
	device->setEventReceiver(cer);

	SKeyMap keyMap[9];
	keyMap[0].Action = EKA_MOVE_FORWARD;
	keyMap[0].KeyCode = KEY_UP;
	keyMap[1].Action = EKA_MOVE_FORWARD;
	keyMap[1].KeyCode = KEY_KEY_W;

	keyMap[2].Action = EKA_MOVE_BACKWARD;
	keyMap[2].KeyCode = KEY_DOWN;
	keyMap[3].Action = EKA_MOVE_BACKWARD;
	keyMap[3].KeyCode = KEY_KEY_S;

	keyMap[4].Action = EKA_STRAFE_LEFT;
	keyMap[4].KeyCode = KEY_LEFT;
	keyMap[5].Action = EKA_STRAFE_LEFT;
	keyMap[5].KeyCode = KEY_KEY_A;

	keyMap[6].Action = EKA_STRAFE_RIGHT;
	keyMap[6].KeyCode = KEY_RIGHT;
	keyMap[7].Action = EKA_STRAFE_RIGHT;
	keyMap[7].KeyCode = KEY_KEY_D;

	keyMap[8].Action = EKA_JUMP_UP;
	keyMap[8].KeyCode = KEY_KEY_J;

	camera = smgr->addCameraSceneNodeFPS(0,50,50,-1,keyMap,9,true,.00050);
	camera->setFarValue(2000.0f);
	camera->setPosition(p->positionVec);
	camera->setTarget(p->positionVec-(p->directionVec.normalize()*10.0f));
	smgr->setActiveCamera(camera);
	p->SetCamera(camera);
}
void GameWorld::updateHud()
{
	irr::s32 tSize = 32;
	irr::s32 w = (driver->getScreenSize().Width/2)-tSize;
	irr::s32 h = (driver->getScreenSize().Height/2)-tSize;
	driver->draw2DImage(driver->getTexture("../res/tex/crosshair.tga"),core::position2d<s32>(w,h),core::rect<s32>(0,0,tSize*2,tSize*2),0,video::SColor(255,255,255,255),true);

	std::ostringstream o;
	o << "Populations: " << gomgr->GetCurThreshold() << "/100";
	std::string str=  o.str();
	size_t needed = ::mbstowcs(NULL,&str[0],str.length());
	// allocate
	std::wstring output;
	output.resize(needed);
	::mbstowcs(&output[0],&str[0],str.length());
	const wchar_t *threshold = output.c_str();
	if(font){
		font->draw(threshold,core::rect<s32>(20,65,120,85),	video::SColor(255,255,255,255));
	}else{
		std::cout<<"font loading failed"<<std::endl;
	}

	std::ostringstream a;
	a << "Score: " << gomgr->score << "/" <<gomgr->nxtLvl;;
	str= a.str();
	needed = ::mbstowcs(NULL,&str[0],str.length());
	// allocate
	output.resize(needed);
	::mbstowcs(&output[0],&str[0],str.length());
	const wchar_t *score = output.c_str();
	if(font){
		font->draw(score,core::rect<s32>(20,110,120,85),	video::SColor(255,255,255,255));
	}else{
		std::cout<<"font loading failed"<<std::endl;
	}
	/*
	std::ostringstream b;
	b << "Buildings: " << gomgr->no_of_buildings;
	str= b.str();
	needed = ::mbstowcs(NULL,&str[0],str.length());
	// allocate
	output.resize(needed);
	::mbstowcs(&output[0],&str[0],str.length());
	const wchar_t *build = output.c_str();
	if(font){
		font->draw(build,core::rect<s32>(20,155,120,85),	video::SColor(255,255,255,255));
	}else{
		std::cout<<"font loading failed"<<std::endl;
	}*/

	std::ostringstream c;
	c << "Level: " << gomgr->level;
	str= c.str();
	needed = ::mbstowcs(NULL,&str[0],str.length());
	// allocate
	output.resize(needed);
	::mbstowcs(&output[0],&str[0],str.length());
	const wchar_t *lvl = output.c_str();
	if(font){
		font->draw(lvl,core::rect<s32>(20,20,120,85),	video::SColor(255,255,255,255));
	}else{
		std::cout<<"font loading failed"<<std::endl;
	}
}

void GameWorld::gameLoop()
{
	p->Update();
	p->TickAI();
	this->gomgr->TickManager();
}

bool GameWorld::run()
{
	for(int i = 0; i<100;i++)
	{
		std::cout <<"\n"<<std::endl;
	}
	std::cout <<"================================="<< std::endl;
	std::cout <<"=  Welcome to the Crazy World.  ="<< std::endl;
	std::cout <<"================================="<< std::endl;
	//std::cout <<"\n"<< std::endl;
	std::cout <<" Rule :"<< std::endl;
	std::cout <<"   Do not allow the alien \n  population count reach or else \n  you will explode and die"<< std::endl;
	std::cout <<" Control :"<< std::endl;
	std::cout <<"   ESC        - Exit Game"<< std::endl;
	std::cout <<"   Left Click - Fire"<< std::endl;
	std::cout <<"   W,A,S,D    - Movement"<< std::endl;
	int lastFPS = -1;

	while(device->run())
	{
		this->gameLoop();
		/*
		if(gomgr->GetCurThreshold()<100)
		{
			this->gameLoop();
		}else{
			device->getTimer()->setSpeed(0);
		}
		*/

		driver->beginScene(0,true,0);
		smgr->drawAll();
		gui->drawAll();
		this->updateHud();
		driver->endScene();

		//Uncomment to show FPS  =-
		// display frames per second in window title
		int fps = driver->getFPS();
		if (lastFPS != fps)
		{
			core::stringw str = L"Crazy World's Threshold: [ ";
			str += gomgr->GetCurThreshold();
			str += "/100 ] FPS: [";
			str += fps;
			str += "]";
			device->setWindowCaption(str.c_str());
		}
		//double t = fps;
		//double a = 30/t;
		//std::cout << "Fake: " << a << " FPS: " << fps << std::endl;
		//device->getTimer()->setSpeed(1);


	}

	if(gomgr->GetCurThreshold()<100)
	{
		std::cout <<"================================="<< std::endl;
		std::cout <<"= Hope you enjoyed yourself,Bye ="<< std::endl;
		std::cout <<"================================="<< std::endl;
	}else{
		std::cout <<"================================="<< std::endl;
		std::cout <<"=    GAME OVER Try again,Bye    ="<< std::endl;
		std::cout <<"================================="<< std::endl;
	}
	return true;
}
GameWorld::~GameWorld(void)
{
	delete this->gomgr;
}
