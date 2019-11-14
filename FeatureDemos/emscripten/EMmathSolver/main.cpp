//This can be useful for scripting...
//The classes only works on float (f32) values. (no vectors, no double, no matrices, sorry...)

//Author tom_gamer


//`3.4*PI/sin(4.6+min(4,2)^1.7)`

// may have broke the rand * p1 function  //res = randf * p1;  was replaced with regular rand in resolve function - tecan

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif
//#include <string.h>
#include <stdio.h>

#include "SCalcExpr.h"

#include<iostream>
#include<string> // for string class
using namespace std;


using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;


SIrrlichtCreationParameters params;
IrrlichtDevice *device;
video::IVideoDriver* driver;
scene::ISceneManager* smgr;
gui::IGUIEnvironment* env;
scene::IAnimatedMesh* mesh;

scene::ISceneNode* node;
s32 MouseX, MouseY;
u32 MouseButtonStates;

SCalcExpr *calc = new SCalcExpr();
	IGUIEditBox *formula;
	IGUIEditBox *result;
	IGUIButton *calculated;

//irrString formulatext;


gui::IGUIStaticText* text = 0;
gui::IGUIEnvironment* guienv;

//calculate("min(clamp(PI*2.5*round(3.9)/PI,1,2), 3)"));

float calculate(stringw strings){
			// try to parse the string
			if ( !calc->readExpr(strings) )
			{
					// readExpr() will show a detailed error message
					printf("Invalid expression!\n");
					return(0);
			}

			f32 f;
			// Calculate the result
			if (!calc->calcExpr(NULL, f))
			{
					// calcExpr() will show a detailed error message
					printf("can't calculate expression\n");
					return(0);
			}

printf("Result = %0.3f\n", f);
return f;
}

void main_loop()
{
	device->run();
	driver->beginScene(true, true, video::SColor(255,200,200,200));


	char *sometext = "hello";

	//stringw text;
	//text += 'float (f32) values. (no vectors, no double, no matrices, sorry...)';
	//guienv->addStaticText(text,core::rect<s32>(500,10,600,80),false,true);
	smgr->drawAll();
	env->drawAll();
	driver->endScene();
	device->sleep(10);
}


class MyEventReceiver: public IEventReceiver
{
   public:
       virtual bool OnEvent(const SEvent& event)
       {
       //    if (event.EventType == EET_KEY_INPUT_EVENT)
        //       KeyIsDown[event.KeyInput.Key] = event.KeyInput.PressedDown;
         //   return false;

		   if ( event.EventType == EET_GUI_EVENT )	{
			s32 cid = event.GUIEvent.Caller->getID();
			switch ( event.GUIEvent.EventType )
			{
				case EGET_BUTTON_CLICKED:
					switch ( cid )
					{
				//		levent="button";
	//					eid=cid;
						case 101:
	//						quit = true;
							stringw tests;
							tests += calculate( formula->getText() );
						//	printf("button Clicked");
							result->setText( L"test" );
							result->setText( tests.c_str() );


							return true;
					}
					return false;
			}
			}
			return false;
       }



//       virtual bool IsKeyDown(EKEY_CODE keyCode) const
//       {
//         return KeyIsDown[keyCode];
//       }
//
//       MyEventReceiver()
//       {
//            for (u32 i = 0; i < KEY_KEY_CODES_COUNT; ++i)
//              KeyIsDown[i] = false;
//       }
//  private:
//       bool KeyIsDown[KEY_KEY_CODES_COUNT];
};



int main(){

	MouseX = 0;
	MouseY = 0;
	MouseButtonStates = 0;

		video::E_DRIVER_TYPE driverType;

#ifdef __EMSCRIPTEN__
// only use OGLES2 for Emscripten
driverType = video::EDT_OGLES2;
#endif


	   MyEventReceiver receiver;

	params = SIrrlichtCreationParameters();
	params.DriverType = video::EDT_OGLES2;
	params.WindowSize = core::dimension2d<u32>(640, 480);
	params.Bits = 16;
	params.Fullscreen = false;
	params.Stencilbuffer = false;
	params.Vsync = false;
	params.EventReceiver = &receiver;
	params.OGLES2ShaderPath = std::string("media/Shaders/").c_str();
	device = createDeviceEx(params);

//printf("testtesttesttesttesttesttesttesttesttesttesttesttesttesttesttest");

		device->setWindowCaption(L"Irrlicht Engine - User Interface Demo");

	if (device == 0)
		return 1; // could not create selected driver.

		driver = device->getVideoDriver();
	smgr = device->getSceneManager();
	env = device->getGUIEnvironment();


	//device->setEventReceiver(&receiver);
  //  device->getFileSystem()->changeWorkingDirectoryTo( "./media/" );
	//    gui::IGUIWindow* win = guienv->addWindow( core::rect<s32>(50,20,510,520) );

	//	device->getFileSystem()->addFileArchive("media/map-20kdm2.pk3");
	//	mesh = smgr->getMesh("20kdm2.bsp");
	//node = 0;
//	if (mesh)
//		node = smgr->addOctreeSceneNode(mesh->getMesh(0), 0, -1, 1024);
//		node = smgr->addMeshSceneNode(mesh->getMesh(0));
//	if (node)
//		node->setPosition(core::vector3df(-1300,-144,-1249));

//	smgr->addCameraSceneNodeFPS();
//	device->getCursorControl()->setVisible(false);

// text.setText(float (f32) values. (no vectors, no double, no matrices, sorry...));

//		ITexture *loginWindow = driver->getTexture ( "./data/textures/lobby/loginBg.png" );
//
//vector<int> resolution[1];
	//int buttonPosX = ( device->getScreenWidth() / 2 ) - 60;
	//int buttonPosY = ( device->getScreenHeight() / 100 ) * 80;
		int buttonPosX = ( 640 / 2 ) - 60;
	int buttonPosY = ( 480 / 100 ) * 80;
//


	// Load the button textures
//	ITexture *menuButtonNormal = driver->getTexture ( "./data/textures/lobby/lobbyButtonNormal.png" );
//	ITexture *menuButtonDepressed = driver->getTexture ( "./data/textures/lobby/lobbyButtonDepressed.png" );

	//guienv->addStaticText(L"float f32 values. no vectors, no double, no matrices, sorry...", rect<s32>(10,110,350,130));


formula = env->addEditBox(L"3.4*PI/sin(4.6+min(4,2)^1.7)*1", rect<s32>(10, 10, 210, 50));
result = env->addEditBox(L"result", rect<s32>(10, 50, 210, 100));

	// formula = guienv->addEditBox ( L"", rect<s32> ( buttonPosX - 30, buttonPosY - 100, buttonPosX + 150, buttonPosY - 80 ), true, 0, -1 );
	// result = guienv->addEditBox ( L"", rect<s32> ( buttonPosX - 30, buttonPosY - 60, buttonPosX + 150, buttonPosY - 40 ), true, 0, -1 );

	formula->setText(L"3.4*PI/sin(4.6+min(4,2)^1.7)");
	//stringw formulatext = formula->getText();

		//IGUIButton *calculated = guienv->addButton ( rect<s32> ( 50, 100, 90, 150 ), 0, 101, L"Calc" );
			calculated =env->addButton(rect<s32>(10,240,110,240 + 32), 0, 101,
			L"Calculate", L"Exits Program");
	//exitButton->setImage ( menuButtonNormal );
	//exitButton->setPressedImage ( menuButtonDepressed );
//
//	pword->setPasswordBox ( true, L'*' );

#ifdef __EMSCRIPTEN__
	emscripten_set_main_loop(main_loop,0,1);
#else
while (1){
main_loop();

}


	device->drop();

// we don't need this object anymore
delete calc;


return(0);
}
