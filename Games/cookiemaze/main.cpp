//#include "stdafx.h" //microsoft
#include <irrlicht.h>
#include "main.h"
#include "load_audio.h"
#include "irrXML.h"
#include "main_menu.h"
#include "clear_scene.h"
#include "load_map.h"


using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;


#pragma comment(lib, "Irrlicht.lib")



class MyEventReceiver : public IEventReceiver
{
public:
   virtual bool OnEvent(const SEvent &event)
   {
   // ------------------------------------- key press events ------------------------------------
   if (event.EventType == irr::EET_KEY_INPUT_EVENT && !event.KeyInput.PressedDown)
   {
         switch(event.KeyInput.Key)
         {

        //create screenshot
        case irr::KEY_F9:
        {
        //get video::IImage from the last rendered frame
        video::IImage* image = device->getVideoDriver()->createScreenShot();

        //construct a filename, consisting of local time and file extension
        irr::c8 filename[32];
        snprintf(filename, 32, "PhotoAlbum/pic_%u.jpg", device->getTimer()->getTime());
        //write screenshot to file
        device->getVideoDriver()->writeImageToFile(image, filename);
        screenShot_fader->fadeIn(300);
        printf("[INFO] Screenshot saved in PhotoAlbum folder.\n");

        }
         return true;

        // user pressed the Escape key
         case irr::KEY_ESCAPE:
        {
         if(isInGame==1)
         {
           //printf("[INFO] Pressed Escape. Is in game.\n");

           isInGame=0;
           clearAllScene();
           loadMainMenu();
           isInGame=0;

         } else {
                 //printf("[INFO] Pressed Escape. Not in game.\n");

                 }
        }
        return true;

         default:
            break;

         }
   }

   // ------------------------------------- mouse click events ------------------------------------
   if (event.EventType == EET_GUI_EVENT)
                {
                        s32 id = event.GUIEvent.Caller->getID();
                        //call a pointer to GUI Environment
                        IGUIEnvironment* guienv = device->getGUIEnvironment();
						video::IVideoDriver* driver = device->getVideoDriver();


                        switch(event.GUIEvent.EventType)
                        {

						case EGET_LISTBOX_CHANGED:
							{
							mapName = maps_list->getListItem(maps_list->getSelected());

							core::stringc newLevelPreviewValue = "data/maps/";
							newLevelPreviewValue+= mapName;
							newLevelPreviewValue+= "/levelPreview.jpg";

							levelPreview = driver->getTexture(newLevelPreviewValue);

                            guienv->addImage(levelPreview, core::position2d<s32>(404, 66), false, singleplayer_window);


							//get the briefing text
							core::stringc newLevelBriefingValue = "data/maps/";
							newLevelBriefingValue+= mapName;
							newLevelBriefingValue+= "/mapCfg.xml";

							irr::io::IXMLReader* xmlBriefing = device->getFileSystem()->createXMLReader(newLevelBriefingValue.c_str());

							if (xmlBriefing==0)
							{  }else{  }
							while(xmlBriefing && xmlBriefing->read())
								{ //get the last profile used and apply it
									if (core::stringw("map") == xmlBriefing->getNodeName())
									{
									mapBriefing = xmlBriefing->getAttributeValue(L"briefing");
									briefingBox->setText(mapBriefing.c_str());
									}
								}

							}

						break;

                        case EGET_BUTTON_CLICKED:
                                switch(id)
                                {


                                        //show level chooser
                                        case 101:
                                        {
                                            singleplayer_window->setVisible(true);
                                            IGUIElement *windowParent = singleplayer_window->getParent();
                                            windowParent->bringToFront(singleplayer_window);

                                        }
                                        return true;


                                        //show credits window
                                        case 102:
                                        {
                                          credits_window->setVisible(true);
										  IGUIElement *windowParent2 = credits_window->getParent();
                                          windowParent2->bringToFront(credits_window);

                                        }
                                        return true;


                                        //quit game
                                        case 103:
                                        {
                                        clearAllScene();
                                        device->closeDevice();
                                        }
                                        return true;


                                        //load a game level
                                        case 104:
                                        {
                                          singleplayer_window->setVisible(false);
                                          startGameBtn->setVisible(false);
                                          creditsBtn->setVisible(false);
                                          quitGameBtn->setVisible(false);
										  optionsBtn->setVisible(false);
										  instructionsBtn->setVisible(false);

                                          mapName = maps_list->getListItem(maps_list->getSelected());
                                          singleplayer_window_on = 0;
                                          isPleaseWaitImg=1;

                                        }
                                        return true;


                                        //'load map' "Cancel" button (hides the 'load map' window)
                                        case 105:
                                        {
                                          singleplayer_window->setVisible(false);

                                        }
                                        return true;


										//'credits' "close" button (hides the 'credits' window)
                                        case 106:
                                        {
                                          credits_window->setVisible(false);

                                        }
                                        return true;


										//show the instructions window
                                        case 107:
                                        {
                                          startGameBtn->setVisible(false);
                                          creditsBtn->setVisible(false);
                                          quitGameBtn->setVisible(false);
										  optionsBtn->setVisible(false);
										  instructionsBtn->setVisible(false);

                                          instructions_window->setVisible(true);

                                        }
                                        return true;

										//show the options window
                                        case 108:
                                        {
                                          startGameBtn->setVisible(false);
                                          creditsBtn->setVisible(false);
                                          quitGameBtn->setVisible(false);
										  optionsBtn->setVisible(false);
										  instructionsBtn->setVisible(false);

                                          options_window->setVisible(true);

                                        }
                                        return true;

										//close the instructions window
                                        case 109:
                                        {
                                          startGameBtn->setVisible(true);
                                          creditsBtn->setVisible(true);
                                          quitGameBtn->setVisible(true);
										  optionsBtn->setVisible(true);
										  instructionsBtn->setVisible(true);

                                          instructions_window->setVisible(false);

                                        }
                                        return true;

										//save and then close the options window
                                        case 110:
                                        {
											IGUIElement *wFullscreenIn = guienv->getRootGUIElement()->getElementFromId(301, true);
											int iswFullscreenIn = ((IGUICheckBox*)wFullscreenIn)->isChecked();

											IGUIElement *musicOnIn = guienv->getRootGUIElement()->getElementFromId(302, true);
											int ismusicOnIn = ((IGUICheckBox*)musicOnIn)->isChecked();

											IGUIElement *sfxOnIn = guienv->getRootGUIElement()->getElementFromId(303, true);
											int issfxOnIn = ((IGUICheckBox*)sfxOnIn)->isChecked();

											IGUIElement *skipIntroIn = guienv->getRootGUIElement()->getElementFromId(304, true);
											int isskipIntroIn = ((IGUICheckBox*)skipIntroIn)->isChecked();

										    ofstream myOptions;
											myOptions.open("data/logic/config/config.xml");
											myOptions << "<?xml version= \"1.0\"?>\n";
											myOptions << "<video screenResX=\"800\" screenResY=\"600\" wfullscreen=\"";
											myOptions << iswFullscreenIn;
											myOptions << "\" />\n";
											myOptions << "<detail farView=\"6000\" />\n";
											myOptions << "<audio musicOn=\"";
											myOptions << ismusicOnIn;
											myOptions << "\" sfxOn=\"";
											myOptions << issfxOnIn;
											myOptions << "\" />\n";
											myOptions << "<debug skipIntro=\"";
											myOptions << isskipIntroIn;
											myOptions << "\" />\n";

											myOptions.close();

                                          startGameBtn->setVisible(true);
                                          creditsBtn->setVisible(true);
                                          quitGameBtn->setVisible(true);
										  optionsBtn->setVisible(true);
										  instructionsBtn->setVisible(true);

                                          options_window->setVisible(false);

                                        }
                                        return true;



                                }
                      default:
                        return false;

                        break;
                        }

                }



    return false;
   }
};


// -------------------------------------- MAIN -----------------------------------


int main()
{

    //First, create a NULL Irrlicht device, read the configuration file,
    //then create a DirectX Irrlicht device using the retrieved variables

   IrrlichtDevice* deviceNULL = createDevice(video::EDT_NULL);
   video::E_DRIVER_TYPE driverType;
   driverType = video::EDT_OPENGL;

   //read the configuration file
   irr::io::IXMLReader* xml  = deviceNULL->getFileSystem()->createXMLReader("data/logic/config/config.xml");


   if (xml==0)
          {
          printf("[ERROR 002] - file config.xml was NOT found in /data/logic/config/ directory\n");
          }else{
                printf("[INFO] - file config.xml was found in /data/logic/config/ directory\n");
                }

   while(xml && xml->read())
   {  //get video Information
      if (core::stringw("video") == xml->getNodeName())
      {
      screenResX  = xml->getAttributeValueAsInt(L"screenResX");
      screenResY  = xml->getAttributeValueAsInt(L"screenResY");
      wfullscreen = xml->getAttributeValueAsInt(L"wfullscreen");
      }

      //get detail Information
      if (core::stringw("detail") == xml->getNodeName())
      {
      farView        = xml->getAttributeValueAsInt(L"farView");
      }

      //get audio Information
      if (core::stringw("audio") == xml->getNodeName())
      {
      musicOn = xml->getAttributeValueAsInt(L"musicOn");
      sfxOn   = xml->getAttributeValueAsInt(L"sfxOn");
      }

      //get debug Information
      if (core::stringw("debug") == xml->getNodeName())
      {
      skipIntro     = xml->getAttributeValueAsInt(L"skipIntro");
      }

   }
    printf("[INFO] - Configuration file parsed. See above for errors (if any).\n");

    deviceNULL->closeDevice();

// Initialise Irrlicht device.

MyEventReceiver eventReceiver;

irr::SIrrlichtCreationParameters device_settings;
device_settings.DriverType = video::EDT_OPENGL;
device_settings.Bits = 32;
device_settings.AntiAlias= true;
device_settings.Stencilbuffer= false;
device_settings.Fullscreen = wfullscreen;
device_settings.Vsync = false;
device_settings.WindowSize = core::dimension2d<s32>(screenResX, screenResY);
device_settings.EventReceiver = &eventReceiver;
device = createDeviceEx(device_settings);


	if (!device)
		return 1;

	device->setWindowCaption(L"cookieMaze v0.4");

	IVideoDriver* driver = device->getVideoDriver();
	ISceneManager* smgr = device->getSceneManager();
	IGUIEnvironment* guienv = device->getGUIEnvironment();

    device->getFileSystem()->addFileArchive("./data.zip");

	showCompanyInfo = 1;

    //load all audio files we need
    loadAudioFiles();

    //load main menu
	loadMainMenu();







	//create FPS
	int lastFPS = -1;



// ---------------------------------------------- MAIN LOOP ---------------------------------------------------------------------




	while(device->run())
	{

		driver->beginScene(true, true, SColor(255,100,101,140));

		smgr->drawAll();


        if (isBackImg==1)      {driver->draw2DImage(mainMenuGUI,core::rect<s32>(0,0,screenResX,screenResY), core::rect<s32>(0,0,800,600), 0,0,false);}
        if (isPleaseWaitImg==1){driver->draw2DImage(pleaseWaitImg,core::rect<s32>(0,0,screenResX,screenResY), core::rect<s32>(0,0,1024,768), 0,0,false);}
        if (isInGame==1)       {driver->draw2DImage(gameHUD, core::rect<s32>(0,0,screenResX,screenResY), core::rect<s32>(0,0,1024,768), 0,0,true);}



       if(showCompanyInfo==1)//display the company and game info for a few seconds
        {
         if(skipIntro==0)
		   {

			startGameBtn->setVisible(false);
            creditsBtn->setVisible(false);
            quitGameBtn->setVisible(false);
			optionsBtn->setVisible(false);
			instructionsBtn->setVisible(false);

        //start counting
		timeNow1 = device->getTimer()->getTime();
		if ((timeThen1 + 1000) < timeNow1)
        {
			introTime = introTime+1;
			timeThen1 = timeNow1;
		}

		if (introTime<=7){ driver->draw2DImage(EWlogo     ,core::rect<s32>(0,0,screenResX,screenResY), core::rect<s32>(0,0,800,600), 0,0, true); }
		if (introTime>7){ driver->draw2DImage(companyInfo,core::rect<s32>(0,0,screenResX,screenResY), core::rect<s32>(0,0,800,600), 0,0, true); }
		if (introTime==14){startGameBtn->setVisible(true); creditsBtn->setVisible(true); quitGameBtn->setVisible(true);	optionsBtn->setVisible(true); instructionsBtn->setVisible(true); showCompanyInfo = 0; }



            }else{printf("[INFO] Intro skipped.\n"); showCompanyInfo = 0;}
        }




		guienv->drawAll();

		driver->endScene();

		//update FPS
		int fps = driver->getFPS();
        if (lastFPS != fps)
        {
           core::stringw str = L"cookieMaze [";
           str += driver->getName();
           str += "] FPS:";
           str += fps;
           device->setWindowCaption(str.c_str());
           lastFPS = fps;
        }



        // this means that we need to load a map
		if (isPleaseWaitImg==1)
        {
        clearAllScene();
        loadMap(mapName);
        isPleaseWaitImg = 0;
        }else{}



        // if we are playing a level, and we are not in the main menu, then:
        if(isInGame==1){



        // start the time counter
        timerSecValue = device->getTimer()->getTime();

                // every second increment the seconds :p

                if ((timeThen + 1000) < timerSecValue)
                {
                  timerActualSec = timerActualSec-1;

                  if (timerActualSec <= 0){timerActualSec = 59; timerActualMin = timerActualMin - 1;} else {}
                  if (timerActualMin <= -1){printf("[INFO] Time is up!\n");

			timerActualSec = 0;
			timerActualMin = 0;

				  } else {}
                  timeThen = timerSecValue;
                }


 		core::stringw timerSecValueFormated = L"";
        timerSecValueFormated += timerActualSec;
        timerSec->setText(timerSecValueFormated.c_str());

        core::stringw timerMinValueFormated = L"";
        timerMinValueFormated += timerActualMin;
        timerMinValueFormated += L":";
        timerMin->setText(timerMinValueFormated.c_str());


		//check to see if the player has reached the exit door
		exitDoorNode = smgr->getSceneNodeFromId(99);
        if (exitDoorNode)
        {
        if (playerCollisionBox->getTransformedBoundingBox().intersectsWithBox(exitDoorNode->getTransformedBoundingBox()))
           {
            printf("[INFO] You have found the exit!\n");
			if (sfxOn == 1){ systemFMOD->playSound(FMOD_CHANNEL_FREE, pickUpSound, 0, &channel3); }else{}
			exitDoorNode->setVisible(false);
            smgr->addToDeletionQueue(exitDoorNode);

            //give 100 points to the player
            core::stringw scoreValueFormated = L"";
            scoreValuePoints = scoreValuePoints + 100;
            scoreValueFormated += scoreValuePoints;
            scoreValue->setText(scoreValueFormated.c_str());

			//show a window with final score and time

            win_window = guienv->addWindow(rect<s32>(200,screenResY/5,screenResX-200,screenResY-100), true, L"");
            guienv->addImage(winScreen, core::position2d<s32>(0, 0), false, win_window);

            guienv->addStaticText(L"Final score:", rect<s32>(30,110,800,140), false, false, win_window);
            guienv->addStaticText(scoreValueFormated.c_str(), rect<s32>(10,150,800,180), false, false, win_window);

            guienv->addStaticText(L"Final time:", rect<s32>(30,190,800,220), false, false, win_window);
            guienv->addStaticText(timerMinValueFormated.c_str(), rect<s32>(10,230,50,260), false, false, win_window);
			guienv->addStaticText(timerSecValueFormated.c_str(), rect<s32>(55,230,200,260), false, false, win_window);

			timerActualSec = 0;
			timerActualMin = 0;

           } else{ /*there is no collision with a node happening */ }

        } else { /*there are no more exit doors */ }


//-------------------------


	core::array<scene::ISceneNode *> nodes2;
	smgr->getSceneNodesFromType(scene::ESNT_MESH, nodes2);

	for (u32 i=0; i < nodes2.size(); ++i)
	{
		scene::ISceneNode *selectedNode = nodes2[i];
		u32 selectedNodeID = selectedNode->getID();


//------------------------
        if (selectedNodeID == 66 && playerCollisionBox->getTransformedBoundingBox().intersectsWithBox(selectedNode->getTransformedBoundingBox()))
		    {
            printf("[INFO] Lost a life!\n");
			if (sfxOn == 1){ systemFMOD->playSound(FMOD_CHANNEL_FREE, ouchSound, 0, &channel3); }else{}

			if(livesValuePoints == 1)
			{
			//game over, player doesn't have any lives left
			//show a window with final score and time

			//give 10 points to the player
            core::stringw scoreValueFormated = L"";
            scoreValuePoints = scoreValuePoints + 10;
            scoreValueFormated += scoreValuePoints;
            scoreValue->setText(scoreValueFormated.c_str());

            win_window = guienv->addWindow(rect<s32>(200,screenResY/5,screenResX-200,screenResY-100), true, L"");
            guienv->addImage(gameOverScreen, core::position2d<s32>(0, 0), false, win_window);

            guienv->addStaticText(L"Final score:", rect<s32>(30,110,800,140), false, false, win_window);
            guienv->addStaticText(scoreValueFormated.c_str(), rect<s32>(10,150,800,180), false, false, win_window);

            guienv->addStaticText(L"Final time:", rect<s32>(30,190,800,220), false, false, win_window);
            guienv->addStaticText(timerMinValueFormated.c_str(), rect<s32>(10,230,50,260), false, false, win_window);
			guienv->addStaticText(timerSecValueFormated.c_str(), rect<s32>(55,230,200,260), false, false, win_window);

			timerActualSec = 0;
			timerActualMin = 0;
			}

			//now delete the spike node
            selectedNode->setVisible(false);
            smgr->addToDeletionQueue(selectedNode);
            //take a life from the player

            core::stringw lifeValueFormated = L"";
            livesValuePoints = livesValuePoints - 1;
            lifeValueFormated += livesValuePoints;
            livesValue->setText(lifeValueFormated.c_str());
			}


//------------------------

		if (selectedNodeID == 33 && playerCollisionBox->getTransformedBoundingBox().intersectsWithBox(selectedNode->getTransformedBoundingBox()))
           {
            printf("[INFO] Picked up a bonus!\n");
			if (sfxOn == 1){ systemFMOD->playSound(FMOD_CHANNEL_FREE, pickUpSound, 0, &channel3); }else{}

            //add a particle system
			scene::IParticleSystemSceneNode* ps = smgr->addParticleSystemSceneNode(false);

        scene::IParticleEmitter* em = ps->createBoxEmitter(
                core::aabbox3d<f32>(-7,0,-7,7,1,7), // emitter size
                core::vector3df(0.0f,0.16f,0.0f),   // initial direction
                10,20,                             // emit rate
                video::SColor(0,255,255,255),       // darkest color
                video::SColor(0,255,255,255),       // brightest color
                800,2000,0,                         // min and max age, angle
                core::dimension2df(10.f,10.f),         // min size
                core::dimension2df(20.f,20.f));        // max size

        ps->setEmitter(em); // this grabs the emitter
        em->drop(); // so we can drop it here without deleting it

        scene::IParticleAffector* paf = ps->createFadeOutParticleAffector();

        ps->addAffector(paf); // same goes for the affector
        paf->drop();

		ps->setPosition(selectedNode->getPosition());
        ps->setScale(core::vector3df(2,4,2));
        ps->setMaterialFlag(video::EMF_LIGHTING, false);
        ps->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
        ps->setMaterialTexture(0, driver->getTexture("data/shaders/spark.bmp"));
        ps->setMaterialType(video::EMT_TRANSPARENT_VERTEX_ALPHA);

            //now delete the bonus node
            selectedNode->setVisible(false);
            smgr->addToDeletionQueue(selectedNode);
            //give ten point to the player

            core::stringw scoreValueFormated = L"";
            scoreValuePoints = scoreValuePoints + 10;
            scoreValueFormated += scoreValuePoints;
            scoreValue->setText(scoreValueFormated.c_str());


        } else { /*there are no more bonus nodes to pick up */ }

}

        //if the player falls out of the map, put him back
        if(playaCamera->getAbsolutePosition().Y<=-1000)
        {
        playaCamera->setPosition(core::vector3df(0,2000,0));
        }else{
             //do nothing
             }
        }else{}



	}


	device->drop();

	return 0;
}
