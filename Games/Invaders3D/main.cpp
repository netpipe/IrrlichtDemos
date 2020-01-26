#include <audiere.h>
using namespace audiere;
#include <iostream>
#include <irrlicht.h>
#include "tinystr.h"
#include "tinyxml.h"
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <string>
//#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include "HighScore.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;
using namespace std;
#pragma comment(lib, "Irrlicht.lib")
IrrlichtDevice *device;
IVideoDriver* driver;
IGUIEnvironment* guienv;
ISceneManager* smgr;
//Tiny XML
#define SERVER_THREAD_SLEEPTIME  10000.0f
float server_refresh;

//Gui Globals
    //Menu Tab's
    IGUITab* tab_Main;
    IGUITab* tab_HighScore;
    IGUITab* tab_Options;
    //Adventure
    IGUIEditBox* cni_txt;
    IGUIWindow* cni_wnd;
	IGUIListBox* arc_name;
    //Adventure - Game
    //Options
    IGUICheckBox* opt_depth;
    IGUICheckBox* opt_fps;
    IGUICheckBox* opt_fullscreen;
    IGUICheckBox* opt_shaders;
    IGUIEditBox* opt_name;
    IGUIListBox *opt_drivers;
    IGUIComboBox *opt_reso;
    IVideoModeList* v_list;
    //Net Game
    IGUIListBox *mul_list;
    IGUIEditBox* mul_port;
    IGUIEditBox* mul_ip;
    //Single Player
    //IGUIStaticText* Options_
#include "game.h"
#include "settings.h"

settings gConfig;
//Global variables
unsigned global_gamestate = 0;
bool global_getout = false;
//Audio Globals
AudioDevicePtr Audio_Device(OpenDevice());
OutputStreamPtr Sound_Click(OpenSound(Audio_Device, "Media/Audio/click.wav", true));
OutputStreamPtr Sound_Background(OpenSound(Audio_Device, "Media/Audio/Techno.ogg", true));
//ITimer* stime;
void saveConfig();
void readConfig();

class MyEventReceiver : public IEventReceiver
{
public:
    void flushTabs()
    {
         tab_Main->setVisible(false);
         tab_Options->setVisible(false);
         tab_HighScore->setVisible(false);
    }

	virtual bool OnEvent(const SEvent& event)
	{
		if (event.EventType == EET_GUI_EVENT)
		{
			s32 id = event.GUIEvent.Caller->getID();
			IGUIEnvironment* guienv = device->getGUIEnvironment();

			switch(event.GUIEvent.EventType)
			{
			case EGET_BUTTON_CLICKED:
                 Sound_Click->play();
                 switch(id)
                 {
                     //Main menu
                     case 101: //Start
                         readConfig();
                         flushTabs();
                         global_gamestate=1;
    				     gConfig.gameType = 1;
    				     device->closeDevice();
    				 break;
                     case 102: //HighScore
                         flushTabs();
                         tab_HighScore->setVisible(true);
                     break;
                     case 103: //Options
                         readConfig();
                         flushTabs();
                    	 tab_Options->setVisible(true);
                      break;
                     case 104: //Exit Game
                     	 device->closeDevice();
					     return true;
                      break;
                     //Sub menu
                     case 200: //All purpose close
                         flushTabs();
                         tab_Main->setVisible(true);
                         return true;
                     //Options
                     case 201: //Close options tab
                         flushTabs();
                         tab_Main->setVisible(true);
                    	 //Reset default options values in gui
                         return true;
                      case 202:
                          //Save Code
                          saveConfig();
                          return true;
                          break;
                      default:
                          break;
                 }
				break;
			}
		}
		return false;
	}
};
IrrlichtDevice* createMenuDevice()
{
  //OpenGL
  IrrlichtDevice *device1;
  //return device1;
  //OpenGL
  device1 = createDevice(EDT_OPENGL, core::dimension2du(640, 480),32);
  if(device1 != 0)
      return device1;
  //Direct 9
  device1 = createDevice(EDT_DIRECT3D9,core::dimension2du(640, 480));
  if(device1 != 0)
      return device1;

      device1 = createDevice(EDT_SOFTWARE,core::dimension2du(640, 480));
  //Software
  if(device1 != 0)
     return device1;
  device1 = createDevice(EDT_SOFTWARE,core::dimension2du(640, 480));

  if(device1 != 0)
     return device1;
}

int main()
{
    //Load audio
    if (!Audio_Device) {
        // failure
    }
    if (!Sound_Click) {
        // failure
    }
    if(Sound_Background){
        Sound_Background->setRepeat(1);
        Sound_Background->play();
    }
    gConfig.Audio_Device = &Audio_Device;

    while(global_gamestate < 99){
        switch(global_gamestate)
        {
            case 0:{
            device = createMenuDevice();
            device->getFileSystem()->addZipFileArchive("Data.pk",false,false);
            if(device == 0)
            {
               global_gamestate = 200;
               break;
            }
                MyEventReceiver receiver;
            	device->setEventReceiver(&receiver);
            	device->setWindowCaption(L"Space Invaders:  Menu");

            	IVideoDriver* driver = device->getVideoDriver();
            	guienv = device->getGUIEnvironment();
					for (s32 i=0; i<EGDC_COUNT ; ++i)
					{
						SColor col = guienv->getSkin()->getColor((EGUI_DEFAULT_COLOR)i);
						col.setAlpha(255);
						guienv->getSkin()->setColor((EGUI_DEFAULT_COLOR)i, col);
					}

                IGUIImage* img = guienv->addImage(driver->getTexture("Media/menu.png"),position2d<int>(0,0));
            	IGUISkin* skin = guienv->getSkin();

                IGUIFont* font = guienv->getFont("Media/Fonts/courier.bmp");
            	if (font)
            		skin->setFont(font);
            	guienv->addButton(rect<s32>(10,35,125,105), 0, 101, L"Start");
            	guienv->addButton(rect<s32>(10,115,125,185), 0, 102, L"High Scores");
            	guienv->addButton(rect<s32>(10,195,125,265), 0, 103, L"Options");
            	guienv->addButton(rect<s32>(10,370,125,440), 0, 104, L"Quit");

            	//Main Menu Tab
            	tab_Main = guienv->addTab(rect<s32>(150,30,600,440));
                    IGUIStaticText* txtIn = guienv->addStaticText(L"Space Invaders v0.1\n"
                 "www.g0dsoft.com\nwebmaster@g0dsoft.com\n"
                 "Created for the Irrlicht Space Invaders Competition"  ,rect<int>(0,0,450,410),false,true,tab_Main);

                tab_Main->setVisible(true);
                //HighScores Tab
            	tab_HighScore = guienv->addTab(rect<s32>(150,30,600,440));
                //Read HighScores
                HighScore mHighScores;
                core::stringw* ScoreStrName = new stringw(L"#  Name\n");
                core::stringw* ScoreStrScore = new stringw(L"Score\n");
                *ScoreStrName += "---------------------\n";
                *ScoreStrScore += "---------------------\n";
                mHighScores.readFile();
                for(unsigned j = 0; j < mHighScores.getNumScores();j++)
                {
                    HighScore_Score s;
                    s = mHighScores.getScore(j);
                    *ScoreStrName += (int)(j+1);
                    *ScoreStrName += "  ";
                    *ScoreStrName += s.mScoreName.c_str();
                    *ScoreStrName += "\n";
                    *ScoreStrScore += (int)s.mScoreVal;
                    *ScoreStrScore += "\n";
                }
                IGUIStaticText* txtHSName = guienv->addStaticText(ScoreStrName->c_str(),
                rect<int>(0,0,200,410),false,true,tab_HighScore);
                IGUIStaticText* txtHSScore = guienv->addStaticText(ScoreStrScore->c_str(),
                rect<int>(200,0,450,410),false,true,tab_HighScore);
            	tab_HighScore->setVisible(false);
                guienv->addButton(rect<s32>(344, 344, 432, 364), tab_HighScore, 200, L"Close");
            	//Options
             	tab_Options = guienv->addTab(rect<s32>(150,30,600,440));
            	tab_Options->setVisible(false);
                    guienv->addButton(rect<s32>(104, 376, 184, 400), tab_Options, 201, L"Close");
                    guienv->addButton(rect<s32>(16, 376, 96, 400), tab_Options, 202, L"Save");
                    guienv->addStaticText(L"Player Name:", rect<s32>(264, 168, 389, 188), false, false, tab_Options, -1);
                    opt_name = guienv->addEditBox(L"Silly Sam", rect<s32>(264, 192, 414, 212), true, tab_Options, -1);
                    opt_fps = guienv->addCheckBox(false, rect<s32>(264, 40, 424, 60), tab_Options, -1, L"Show FPS");
                    opt_shaders = guienv->addCheckBox(false, rect<s32>(264, 136, 444, 156), tab_Options, -1, L"Shaders");
                    opt_depth = guienv->addCheckBox(false, rect<s32>(264, 104, 424, 124), tab_Options, -1, L"32 bit graphics");
                    opt_fullscreen = guienv->addCheckBox(false, rect<s32>(264, 72, 424, 92), tab_Options, -1, L"Full Screen");
                    opt_reso = guienv->addComboBox(rect<s32>(16, 192, 225, 214), tab_Options, -1);
                    v_list = device->getVideoModeList();
                    core::stringw str;
                    dimension2d<s32> tmp;
                    for(s32 i = 0; i < v_list->getVideoModeCount() ;i++)
                    {
                        if(v_list->getVideoModeDepth(i) >= 32) //Dont display 16bit availability
                        {
                        tmp = v_list->getVideoModeResolution(i);
                        if(tmp.Height >= 480) //Dont display ultra low resolution displays
                        {
                        str = L"";
                        str += tmp.Width;
	                    str += L"x";
                        str += tmp.Height;
                        opt_reso->addItem(str.c_str());
                        }
                        }
                    }

                    guienv->addStaticText(L"Display Modes", rect<s32>(16, 168, 112, 188), false, false, tab_Options, -1);
                    guienv->addStaticText(L"3D Drivers", rect<s32>(16, 16, 104, 36), false, false, tab_Options, -1);
                    opt_drivers = guienv->addListBox(rect<s32>(16, 40, 225, 160), tab_Options, -1);
                    opt_drivers->addItem(L"Open GL");
                    opt_drivers->addItem(L"Direct 3D 9");
                    opt_drivers->addItem(L"Direct 3D 8");
                    opt_drivers->addItem(L"Software");
                    opt_drivers->addItem(L"Software 2");
                    guienv->addStaticText(L"Effects Sound:", rect<s32>(264, 240, 389, 255), false, false, tab_Options, -1);
                    guienv->addScrollBar(true,rect<s32>(264,260,414,275),tab_Options,201);
                    guienv->addStaticText(L"Game Music:",rect<s32>(264, 290, 389, 305), false, false, tab_Options, -1);
                    guienv->addScrollBar(true,rect<s32>(264,310,414,325),tab_Options,201);
            	global_gamestate = 9;


                readConfig();

            //End parse settings file//
                while(device->run() && driver)
            	if (device->isWindowActive())
            	{
            		driver->beginScene(true, true, SColor(0,133,190,133));

            		guienv->drawAll();

            		driver->endScene();
             	}
            	device->drop();
            	break;
         }
            case 1:{

                class Game Gamer;
             	// Init game
            	Gamer.Init(gConfig);

            	// Create scene
            	Gamer.Prepare();

            	// Main Loop
            	while(Gamer.gdevice->run())
                {
            		Gamer.MainLoop();
            	}

            	// Clean up memory
                global_gamestate = Gamer.Finish();
            break;
           }
    	    case 9:
    	        return 0;
	        break;
    	}
    }
    return 0;
}

void readConfig()
{
    //Set all variables to their defaults.
    TiXmlDocument config( "config.xml" );
	bool loadOkay = config.LoadFile();
	if ( !loadOkay )
	{
		printf( "Could not load test file 'demotest.xml'. Error='%s'. Exiting.\n", config.ErrorDesc() );
		exit( 1 );
	}

	TiXmlNode* xmlNode = 0;
    TiXmlElement* settingEl = 0;
    int tester = 0;
    int tester2 = 0;
    xmlNode = config.FirstChild( "FullScreen" );
    settingEl = xmlNode->ToElement();
    settingEl->QueryIntAttribute("bool", &tester);

    if(tester == 1)
    {
      opt_fullscreen->setChecked(true);
      gConfig.fullscreen = true;
      //Set the FullScreen variable
    }
    else
    {
      gConfig.fullscreen = false;
      opt_fullscreen->setChecked(false);
    }

    xmlNode = config.FirstChild( "Depth" );

    settingEl = xmlNode->ToElement();
    settingEl->QueryIntAttribute("bool",&tester);
    if(tester == 1)
    {
       opt_depth->setChecked(true);
       gConfig.depth = true;
       //Set the Depth variable
    }
    else
    {
       opt_depth->setChecked(false);
       gConfig.depth = false;
    }

    xmlNode = config.FirstChild( "Fps" );
    settingEl = xmlNode->ToElement();
    settingEl->QueryIntAttribute("bool",&tester);
    if(tester == 1)
    {
       opt_fps->setChecked(true);
       gConfig.fps = true;
       //Set the Shadows variable
    }
    else
    {
       opt_fps->setChecked(false);
       gConfig.fps = false;
       //Set the Shadows variable
    }
    xmlNode = config.FirstChild( "Shaders" );
    settingEl = xmlNode->ToElement();
    settingEl->QueryIntAttribute("bool",&tester);
    if(tester == 1)
    {
       opt_shaders->setChecked(true);
       gConfig.shaders = true;
       //Set the HighDetail variable
    }
    else
    {
       opt_shaders->setChecked(false);
       gConfig.shaders = false;
       //Set the HighDetail variable
    }


    xmlNode = config.FirstChild( "Driver" );
    settingEl = xmlNode->ToElement();
    settingEl->QueryIntAttribute("int",&tester);
    switch (tester)
    {
           case 0:
               gConfig.mode = EDT_OPENGL;
               break;
           case 1:
               gConfig.mode = EDT_DIRECT3D9;
               break;
           case 2:
               gConfig.mode = EDT_SOFTWARE;
               break;
           case 3:
               gConfig.mode = EDT_SOFTWARE;
               break;
           default:
               gConfig.mode = EDT_OPENGL;
               break;
    }
    opt_drivers->setSelected(tester);

    xmlNode = config.FirstChild( "Screen" );
    settingEl = xmlNode->ToElement();
    settingEl->QueryIntAttribute("width",&tester);
    settingEl->QueryIntAttribute("height",&tester2);

    //If shit's borked
    gConfig.width = 640;
    gConfig.height = 480;
    s32 k = 0;
    dimension2d<s32> tmp;
    for(s32 i = 0; i < v_list->getVideoModeCount() ;i++)
    {
        if(v_list->getVideoModeDepth(i) >= 32) //Dont display 16bit availability
        {
            tmp = v_list->getVideoModeResolution(i);
            if(tmp.Height >= 480) //Dont display ultra low resolution displays
            {

                if(tester == tmp.Width && tester2 == tmp.Height)
                {
                    opt_reso->setSelected(k);
                    gConfig.width  = tester;
                    gConfig.height = tester2;
                }
                k++;
            }
        }
    }

       //Set the HighDetail variable
}


void saveConfig()
{
    TiXmlNode* xmlNode=0;
    TiXmlElement* settingEl=0;
    TiXmlDocument config( "config.xml" );
	config.LoadFile();
    xmlNode = config.FirstChild( "FullScreen" );
    settingEl = xmlNode->ToElement();
    if(opt_fullscreen->isChecked())
    {
       settingEl->SetAttribute( "bool", 1);
       //Set variables acordingly
    }
    else
    {
       settingEl->SetAttribute( "bool", 0);
       //Set variables acordingly...
    }


    xmlNode = config.FirstChild( "Depth" );
    settingEl = xmlNode->ToElement();
    if(opt_depth->isChecked())
    {
       settingEl->SetAttribute( "bool", 1);
       //Set variables acordingly
    }
    else
    {
       settingEl->SetAttribute( "bool", 0);
       //Set variables acordingly...
    }

    xmlNode = config.FirstChild( "Fps" );
    settingEl = xmlNode->ToElement();
    if(opt_fps->isChecked())
    {
       settingEl->SetAttribute( "bool", 1);
       //Set variables acordingly
    }
    else
    {
       settingEl->SetAttribute( "bool", 0);
       //Set variables acordingly...
    }

    xmlNode = config.FirstChild( "Shaders" );
    settingEl = xmlNode->ToElement();
    if(opt_shaders->isChecked())
    {
       settingEl->SetAttribute( "bool", 1);
       //Set variables acordingly
    }
    else
    {
       settingEl->SetAttribute( "bool", 0);
       //Set variables acordingly...
    }

    //Driver selection
    xmlNode = config.FirstChild( "Driver" );
    settingEl = xmlNode->ToElement();
    settingEl->SetAttribute("int",opt_drivers->getSelected());

    //Screen resolution
    xmlNode = config.FirstChild( "Screen" );
    settingEl = xmlNode->ToElement();
    dimension2d<s32> tmp;
    s32 j = opt_reso->getSelected();
    s32 k = 0;
    //Just in case, fail safe
    settingEl->SetAttribute("width",640);
    settingEl->SetAttribute("height",480);
    //Find the real entry
    for(s32 i = 0; i < v_list->getVideoModeCount() ;i++)
    {
        if(v_list->getVideoModeDepth(i) >= 32) //Dont display 16bit availability
        {
            tmp = v_list->getVideoModeResolution(i);
            if(tmp.Height >= 480) //Dont display ultra low resolution displays
            {
                if(k == j)
                {
                    settingEl->SetAttribute("width",tmp.Width);
                    settingEl->SetAttribute("height",tmp.Height);
                }
                k++;
            }
         }
    }

    //Save out the file
    config.SaveFile( "config.xml" );
}
