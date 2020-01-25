#include <irrlicht.h>
#include "newton.h"
#include "tumle.h"
#include "SDL.h"
//http://irrlicht.sourceforge.net/forum//viewtopic.php?f=1&t=30750&p=174159&hilit=sdl#p174159

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;
using namespace tumle;

//initialize first joystick globaly
SDL_Joystick *joystick1 = NULL;
//input enumerations
enum analog_pos {X_CENTER, Y_CENTER, RIGHT, LEFT, UP, DOWN};

int main(int argc, char *argv[])
{
//{add classes
SDL_Event event;
//}

//{create an irrlicht device
    IrrlichtDevice* device = createDevice(EDT_OPENGL, dimension2d<s32>(1024, 768), 32,
    true, false, true, 0);

    //initialize SDL
    if(SDL_Init(SDL_INIT_JOYSTICK|SDL_INIT_TIMER) < 0)  return 1;

    //if the device can't be created exit the program
    if (!device) return 1;

    //{pointers
     //create and check driver
     IVideoDriver* driver = device->getVideoDriver();
     if (!driver) return 1;
     //create and check smgr
     ISceneManager* smgr = device->getSceneManager();
     if (!smgr) return 1;
     //create and check guienv
     IGUIEnvironment* guienv = device->getGUIEnvironment();
     if (!guienv) return 1;
     //create and check physics manager
     IPhysicsManager* pmgr = createPhysicsManager(device);
     if (!pmgr) return 1;
     //}
     //}

//=======================================================================
//{menu screen variables
int menu_item = 1;

bool in_main_menu = true;

//}

//{add menu screen objects

//{add menu screen background image
IGUIImage* menu_bkg = guienv->addImage(driver->getTexture("menu_background.png"),
position2d<int>(0,0));
//}

//{add logo image
IGUIImage* logo = guienv->addImage(driver->getTexture("logo.png"),
position2d<int>(25,10));
//}

//{sky background
IMeshSceneNode* sky = smgr->addMeshSceneNode(smgr->getMesh("cylinder.obj") );
if (sky)
{
sky->setPosition(vector3df(0,-10,0) );
sky->setScale(vector3df(550,550,550) );
sky->setMaterialFlag(EMF_NORMALIZE_NORMALS, true);
sky->setMaterialFlag(EMF_BACK_FACE_CULLING, false);
sky->setMaterialFlag(EMF_LIGHTING, false);
sky->setMaterialTexture( 0, driver->getTexture("menu_clouds.bmp") );
//rotation animator
ISceneNodeAnimator* rotate = smgr->createRotationAnimator(core::vector3df(0,0.04f,0));
sky->addAnimator(rotate);
rotate->drop();
}
//}

//{battle background
IMeshSceneNode* battle = smgr->addMeshSceneNode(smgr->getMesh("battlefield_stage.obj") );
if (battle)
{
battle->setScale(vector3df(3,3,3) );
battle->setPosition(vector3df(0,-10,0) );
battle->setMaterialFlag(EMF_NORMALIZE_NORMALS, true);
battle->setMaterialFlag(EMF_BACK_FACE_CULLING, false);
battle->setMaterialFlag(EMF_LIGHTING, true);
battle->setMaterialType(EMT_TRANSPARENT_ALPHA_CHANNEL);
battle->setMaterialTexture( 0, driver->getTexture("battlefield_UV_map.png") );
battle->getMaterial(0).EmissiveColor.set(0,100,100,100);
}
//}

//{camera
ICameraSceneNode* camera = smgr->addCameraSceneNode(0, vector3df(0,5,0), vector3df(0,0,0));
ISceneNodeAnimator* fly = smgr->createFlyCircleAnimator (core::vector3df(0,5,0),130.0f, -0.0001f);
camera->addAnimator(fly);
fly->drop();
//}

//{light
//add light
ILightSceneNode* light = smgr->addLightSceneNode(0, vector3df(-15,5,-105), video::SColorf(1.0f, 1.0f, 1.0f));
//}

//{add P1 button is selected image
IGUIImage* P1_brawl_selected = guienv->addImage(driver->getTexture("pressed_button.png"),
position2d<int>(650,10));
//}

//{add P1 button image
IGUIImage* P1_brawl_button = guienv->addImage(driver->getTexture("menu_button.png"),
position2d<int>(650,10));
//}

//{add P2 button is selected image
IGUIImage* P2_brawl_selected = guienv->addImage(driver->getTexture("pressed_button.png"),
position2d<int>(650,120));
//}

//{add P2 button image
IGUIImage* P2_brawl_button = guienv->addImage(driver->getTexture("menu_button.png"),
position2d<int>(650,120));
//}

//{add adventure mode button is selected image
IGUIImage* adventure_selected = guienv->addImage(driver->getTexture("pressed_button.png"),
position2d<int>(650,230));
//}

//{add adventure mode button image
IGUIImage* adventure_button = guienv->addImage(driver->getTexture("menu_button.png"),
position2d<int>(650,230));
//}

//{add trophy button is selected image
IGUIImage* trophy_selected = guienv->addImage(driver->getTexture("pressed_button.png"),
position2d<int>(650,340));
//}

//{add trophy button image
IGUIImage* trophy_button = guienv->addImage(driver->getTexture("menu_button.png"),
position2d<int>(650,340));
//}

//{add quit button is selected image
IGUIImage* quit_button_selected = guienv->addImage(driver->getTexture("pressed_button.png"),
position2d<int>(650,450));
//}

//{add quit button image
IGUIImage* quit_button = guienv->addImage(driver->getTexture("menu_button.png"),
position2d<int>(650,450));
//}

//}

//{error text objects

//{error text
IGUIStaticText* joy_error = guienv->addStaticText(L"Please insert a joystick or gamepad...",rect<s32>(15,1,260,22), true);
joy_error->setTextAlignment(EGUIA_CENTER, EGUIA_CENTER);
joy_error->move(position2d<int>(350,350) );
//}

//}

//{open joysticks

//Open joystick1
SDL_JoystickEventState(SDL_ENABLE);
joystick1 = SDL_JoystickOpen(0);

//}

//main game loop
while( device->run() )
{
//=======================================================================

//{check joysticks
if ( SDL_NumJoysticks() < 1 ) joy_error->setVisible(true); else joy_error->setVisible(false);
//}

//{menu screen logic functions

//{menu item wrapping
if ( menu_item < 1 ) menu_item = 5;

if ( menu_item > 5 ) menu_item = 1;
//}

//{menu item selection image
//=======================================================================
if ( menu_item == 1 )
{
 P1_brawl_button->setVisible(false);
 P2_brawl_button->setVisible(true);
 adventure_button->setVisible(true);
 trophy_button->setVisible(true);
 quit_button->setVisible(true);
}
//=======================================================================

//=======================================================================
if ( menu_item == 2 )
{
 P1_brawl_button->setVisible(true);
 P2_brawl_button->setVisible(false);
 adventure_button->setVisible(true);
 trophy_button->setVisible(true);
 quit_button->setVisible(true);
}
//=======================================================================

//=======================================================================
if ( menu_item == 3 )
{
 P1_brawl_button->setVisible(true);
 P2_brawl_button->setVisible(true);
 adventure_button->setVisible(false);
 trophy_button->setVisible(true);
 quit_button->setVisible(true);
}
//=======================================================================

//=======================================================================
if ( menu_item == 4 )
{
 P1_brawl_button->setVisible(true);
 P2_brawl_button->setVisible(true);
 adventure_button->setVisible(true);
 trophy_button->setVisible(false);
 quit_button->setVisible(true);
}
//=======================================================================

//=======================================================================
if ( menu_item == 5 )
{
 P1_brawl_button->setVisible(true);
 P2_brawl_button->setVisible(false);
 adventure_button->setVisible(true);
 trophy_button->setVisible(true);
 quit_button->setVisible(false);
}
//=======================================================================

//}

//{SDL event receiver
while(SDL_PollEvent(&event))
{
    switch(event.type)
    {
            case SDL_KEYDOWN:
            /* handle keyboard stuff here */
            break;

            case SDL_QUIT:
            /* Set whatever flags are necessary to */
            /* end the main game loop here */
            break;

            case SDL_JOYAXISMOTION:  /* Handle Joystick Motion */

            if ( ( event.jaxis.value < -3200 ) || (event.jaxis.value > 3200 ) )
            {
                 if( event.jaxis.axis == 0)
                 {
                 menu_item++;
                 }

                 if( event.jaxis.axis == 1)
                 {
                 menu_item--;
                 }
           }
           break;


    }

}

//}

//=======================================================================
 //{update the world

driver->beginScene(true, true, SColor(0,200,200,200));

smgr->drawAll();
guienv->drawAll();

driver->endScene();

//update the newton world
pmgr->update();
//}
//=======================================================================

}

device->drop();
SDL_JoystickClose(joystick1);
atexit(SDL_Quit);
return 0;

}

