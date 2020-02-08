#include "irrlicht.h"
#include <iostream>
#include <fstream>
//#include "load_audio.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;
using namespace std;




// ----------------------- Start declaring variables ---------------------------

// Irrlicht varibles and declarations

   IrrlichtDevice          *device          = 0;                                    //The Irrlicht device
   IVideoDriver            *driver          = 0;                                    //The Irrlicht video driver
   ISceneManager           *smgr            = 0;                                    //The Irrlicht scene manager
   IGUIEnvironment         *guienv          = 0;                                    //The irrlicht gui environment

   ICameraSceneNode        *playaCamera;
   ISceneNode              *playerCollisionBox;
   ISceneNode              *playaIrrNode;
   ISceneNode              *exitDoorNode;
  
   const c8               *nodeNameX;                                               //This stores the bonus items nodes info

   stringw                 mapName;
   s32                     node_ID, mapIDToBeCalled;                                //node ID , TODO: remove
   int                     timeThen = 0, timeThen1 = 0, timeNow1;                   //stores game time values
   int                     timerActualSec = 60, timerActualMin = 0;
   int					   introTime = 0;

   ITexture                *gameHUD;                                                //in-game HUD (or vignette)
   ITexture                *companyInfo, *EWlogo;                                   //image that is displayed on game start
   ITexture                *mainMenuGUI, *pleaseWaitImg, *levelPreview, *levelSelectBk, *creditsBk, *winScreen, *gameOverScreen, *instructionsBk, *optionsBk;

   IGUIButton              *startGameBtn, *creditsBtn, *quitGameBtn;                //the main menu buttons
   IGUIButton			   *instructionsBtn, *optionsBtn;

   int                     showCompanyInfo = 0;                                     //starts the intro animation (company + game info)
   int                     isBackImg       = 1;                                     //used for deciding if it is in main menu, it dislpays the mainMenuGUI
   int                     isPleaseWaitImg = 0;                                     //used for deciding if is loading a level
   int                     escWait         = 1;                                     //escape key was pressed, waiting for the user to choose an option, you can't press escape again
   int                     isInGame = 0;
   int                     updateNetNow = 0;                                        //value (game time) used for updating the network code in the main loop
   int                     singleplayer_window_on = 0;
   int                     lastPhotoID;                                             //used by the screenshot function (takes the last photo name and increments it, then saves a screenshot with that new name)
   int                     timerMinValue = 0, timerSecValue = 0;
   int                     scoreValuePoints = 0;                                    //initial score points
   int                     livesValuePoints = 3;

   IGUIListBox             *maps_list;
   IGUIStaticText          *fpsVal         = 0;                                      //frames per second value -- it is displayed in the console

   IGUIStaticText          *scoreText;
   IGUIStaticText          *scoreValue;
   IGUIStaticText          *livesText;
   IGUIStaticText          *livesValue;
   IGUIStaticText          *timerMin;
   IGUIStaticText          *timerSec;





   IGUIInOutFader          *screen_fader_black, *screenShot_fader, *escFadeToBlack, *escFadeToNormal;
   IGUIWindow              *singleplayer_window, *credits_window, *win_window, *instructions_window, *options_window;
   IGUIWindow              *modal_MEC_window;                                        //MEC = Main menu, Exit game, Cancel
   IGUIEditBox			   *briefingBox;

   IAttributes             *attribs;                                                 //stores attributes from nodes found in .irr file



// Misc variables -- default game values

   int screenResX = 800, screenResY = 600, wfullscreen=0;                           //video
   bool isWFullscreen, isMusicOn, isSfxOn, isSkipIntro;
   int farView = 6000;																//detail
   int musicOn = 1, sfxOn = 1;														//audio
   int skipIntro = 0;																//debug
   int XMLmapCount = 1;                                                             //map count
   stringw XMLmapName;                                                              //map name
   stringw mapBriefing;																//get and store value from XML about the map briefing
