
/// DEVELOPER: jorgerosa - jorge.bigarte@gmail.com
/// HOMEPAGE - http://sites.google.com/site/jorgerosaportfolio

/// ##############################################################################################################################

/// For texts in other idioms...

/// For special characters hardcoded in code:
/// CODE::BLOCKS --> Settings --> Editor --> Other Settings --> Encoding --->  UTF-8, Then C::B must be restarted.
/// Then: text->setText(L"Olééé") will now work fine!

/// For special characters from external files:
/// std::wfstream, std::wstring, stringw all the way!...
/// Example:
/// std::locale defaultLocale(""); // from machine config
/// std::wfstream infile(lyricsPath.c_str());
/// infile.imbue(defaultLocale);   // imbue must be done before opening

/// ##############################################################################################################################


/// System
#include <fstream>
#include <iostream>
#include <exception>
#include <sstream>
#include <vector>
#include <string>
#include <cstring>
#include <stdio.h>
#include <dirent.h>
#include <functional>
#include <algorithm>
#include <ctype.h>
#include <ctime>       // For time()
#include <cstdlib>     // For srand() and rand()
#include <iomanip>
#include <cmath>
#include <stdint.h>
using namespace std;


/// Engine (Irrlicht) --> http://irrlicht.sourceforge.net
#define _IRR_STATIC_LIB_
#include <irrlicht.h>  // Declare Irrlicht
// #include "driverChoice.h"
// #include <IVideoDriver.h>
using namespace irr;
using namespace core;
using namespace video;
using namespace scene;
using namespace io;
using namespace gui;


/// Events (For Irrlicht)
#include "libraries/IrrExtras/myEvents.h"
/// Loading Bar (For Irrlicht)
#include "libraries/IrrExtras/CProgressBar.h"
/// Water: (For Irrlicht)
#include "libraries/IrrExtras/Water/RealisticWater.h" // For ocean effects.
/// Improve performance: (For Irrlicht)
// #include "libraries/IrrExtras/NewMeshCombiner/CMeshCombiner.h"


/// Audio (Audiere)
// #include <audiere.h> // For Audio. ALSO must add "libaudiere.a" in the "Project"->"Build Options"->"Linker" !!!
// using namespace audiere;


/// Bullet: (Physics) --> https://github.com/bulletphysics
/// Bullet: Added linkage in CODE::BLOCKS in "Search directories" (first is Bullet then irrBullet).
/// Bullet: Added linkage in CODE::BLOCKS in "Workspace" (Projects tab - the left tree menu).
/// Bullet: Removed full "Bullet3OpenCL" directory. (Needs CL libraries)
#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>
/// irrBullet: (Bullet wrapper) --> https://github.com/danyalzia/irrBullet
/// irrBullet: Added linkage in CODE::BLOCKS in "Search directories" (first is Bullet then irrBullet).
/// irrBullet: Added linkage in CODE::BLOCKS in "Workspace" (Projects tab - the left tree menu).
/// irrBullet: Removed files "irrBulletKinematicCharacterController", ".h" and ".cpp". (Causes errors)
/// IMPORTANT: "irrBulletCollisionCallBackInformation.cpp" has a BUG! Correct by me at line 21 (where is body1, should be body0) !!!
#include <irrBullet.h>


/*
ERROR: "the procedure entry point _ZNSt8_detail15_List_node_base7_M_hookEPS0_ could not be located in the dynamic link library libstdc -6.dll."
SOLUTION: Wrong "libstdc -6.dll" version (included in MinGW. and in our ".exe" directory) --> https://stackoverflow.com/questions/13360014/the-procedure-entry-point-znst8-detail15-list-node-base7-m-hookeps0-could-not
*/


/// Define Irrlicht Global Variables:
// scene::ICameraSceneNode* camera[2] = {0,0};
IrrlichtDevice* device; // Create Device
irr::video::IVideoDriver* driver; // Create Driver
irr::scene::ISceneManager* smgr; // Create Scene Manager
irr::io::IFileSystem* filesys; // getFileSystem();
gui::IGUIEnvironment* guienv; // For GUI texts
gui::IGUISkin* skin;
gui::IGUIFont* fontMain; // Get Texts Font

MyEventReceiver receiver;

std::vector<gui::IGUIFont*> fonts;
std::vector<gui::IGUIStaticText*> texts;


std::vector<scene::ICameraSceneNode*> cameras;
/// Cameras starting:
vector3df cameraStartPosition;
vector3df cameraStartTarget;
/// Cameras defaults:
vector3df cameraDefaultPosition;
vector3df cameraDefaultTarget;
/// Cameras limits:
f32 maxCamRight  = 0;
f32 maxCamLeft   = 0;
f32 maxCamTop    = 0;
f32 maxCamBot    = 0;
f32 initCamZoom  = 0;
f32 minCamZoom   = 0;
f32 maxCamZoom   = 0;
f32 zoomCamSpeed = 0;
/// Cameras Shake:
f32 camShakeOnY = 0.0;

/// Texts:
int PLAYER_TOTAL_SCORE = 0;
int PLAYER_LEVEL = 0;
stringw PLAYER_NAME = L"";
IGUIStaticText* textInfos1;
IGUIStaticText* textTotalScore;
IGUIStaticText* textAngle;

scene::ITextSceneNode* textScore;
scene::ITextSceneNode* textBonus;
std::vector<ITextSceneNode*> personageName;

/// Window Text:
stringw windowtexts = L"LOADING...";

/// Aspect Ratio Resolution Examples:
/// 4:3   -> 640x480, 800x600, 960x720, 1024x768, 1280x960, 1400x1050, 1440x1080 , 1600x1200, 1856x1392, 1920x1440, 2048x1536
/// 16:9  -> 960x540, 1024x576, 1152x648, 1280x720, 1366x768, 1600x900, 1920x1080, 2000x1125, 2048x1152, 2304x1296
/// 16:10 -> 1280x800, 1440x900, 1680x1050, 1920x1200, 2560x1600
bool FULLSCREEN       = false;
u32 SCREEN_WIDTH      = 1024;
u32 SCREEN_HEIGHT     = 576;
bool BACKGROUND_MUSIC = true;
bool VIEW_TEXTS       = true;
u32 MAX_BALLS         = 2;

u32 BITS              = 32; // NOTE: If in 16 bits, textures will be a mess in fullscreen mode
bool VSYNC            = true;
u32 FRAMES_PER_SECOND = 0; // FOR: If FPS < 60 then do not do "this effect", etc...
bool EXIT_GAME        = false;
bool CAN_EXIT         = false;

/// Start Game?...
bool USER_PRESSED_START = false; /// Normal is false
bool LEVEL_READY = false;
// bool LEVEL_RESTART = false;
bool NAMES_TEXTS = true; /// TODO: Example: CRASHES when prisoner is removed
bool GAME_OVER = false;  /// true -> When all prisoners are free!


/// Initializing Timers: (Part #1)
u32 lastFPS = -1;
u32 then = 0; // MUST be set later... (If set here, will crash!)
f32 frameDeltaTime = 0.0;


/// PHYSICS WORLD:
irrBulletWorld* world;



/// ENABLE DEBUG KEYS?...
bool DEBUG_CAMERA_KEY = false;    /// Key C changes to debug camera
bool RELOAD_LEVEL_KEY = false;    /// Key R reloads level
bool DEBUG_FUNCTIONS_KEY = false; /// Key F to test stuff and functions
/// ENABLE DEBUG EVENTS?...
bool DEBUG_BULLET = false;        /// wireframe view with physics


/// Load external settings from file:
#include "game-settings.cpp"




/// MAIN GAME:
class Game {

    public:

    Game();  // Constructor
    ~Game(); // Destructor
    void Initialize();
    void Exit();

    private:
};

Game::Game(){};  // Constructor (Can accept arguments, but this one does not)
Game::~Game(){}; // Destructor (do not accept arguments)



void Game::Initialize(){


    /// Read external custom settings from file:
    Settings* settings = new Settings();
    settings->ReadSettings();


	/// Loop to chose driver from all available:
    video::E_DRIVER_TYPE driverType;
    for (u32 i=0; i<irr::video::EDT_COUNT; ++i){
    if (irr::IrrlichtDevice::isDriverSupported(irr::video::E_DRIVER_TYPE(i))){ driverType = irr::video::E_DRIVER_TYPE(i); };
    };

    /// Create device from the above found driver:
    /// irr::createDevice(video::E_DRIVER_TYPE deviceType = video::EDT_SOFTWARE, const core::dimension2d< u32 >& windowSize = (core::dimension2d< u32 >(640, 480)), u32 bits = 16, bool fullscreen = false, bool stencilbuffer = false, bool vsync = false, IEventReceiver* receiver = 0)
	/// device = createDevice(driverType, core::dimension2d<u32>(SCREEN_WIDTH, SCREEN_HEIGHT), BITS, FULLSCREEN, false, VSYNC, 0);
	/// if (device == 0){ return 1; }; // If OpenGL doesn´t exist
	SIrrlichtCreationParameters params = SIrrlichtCreationParameters();
    params.AntiAlias = 0;
    params.Bits = BITS;
    params.DriverType = driverType;
    params.WindowSize = core::dimension2d<u32>(SCREEN_WIDTH, SCREEN_HEIGHT);
    params.Fullscreen = FULLSCREEN;
    params.WithAlphaChannel = true;
    params.Vsync = true;
    params.Doublebuffer = true;
    // params.Stencilbuffer = false;
    /// Create device:
    device = createDeviceEx(params);

	driver  = device->getVideoDriver();     // Create Driver
	filesys = device->getFileSystem();
    guienv  = device->getGUIEnvironment();  // For GUI texts
	smgr    = device->getSceneManager();    // Create Scene Manager
	// video::IGPUProgrammingServices* gpu = driver->getGPUProgrammingServices(); // Not used, by now...

	// Add an events receiver: (keyboard)
    device->setEventReceiver(&receiver);

    /// Other Stuff: (Window)
    device->setWindowCaption(windowtexts.c_str());
	/// device->getCursorControl()->setVisible(true); // Mouse cursor visibility
	device->setResizable(true); /// Resizable window?...

	/// Needed for sharpen textures:
    // driver->setTextureCreationFlag(video::ETCF_ALWAYS_32_BIT, true);
    // driver->setTextureCreationFlag(video::ETCF_OPTIMIZED_FOR_QUALITY, true);
	/// Needed for correct transparent textures:
	driver->setTextureCreationFlag(video::ETCF_ALWAYS_32_BIT);
	// driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, true); /// For distant textures quality ( false == more quality )
	// smgr->getParameters()->setAttribute(scene::ALLOW_ZWRITE_ON_TRANSPARENT, true); /// For transparent textures correct z-order

	/// MUCH better 2D smoothly rendered images in all resolutions:
    driver->getMaterial2D().TextureLayer[0].BilinearFilter = true;
    driver->getMaterial2D().AntiAliasing = video::EAAM_FULL_BASIC;

    /// Needed for correct transparent textures:
	/// driver->setTextureCreationFlag(video::ETCF_ALWAYS_32_BIT);
	/// driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, false); /// For distant textures quality
	/// smgr->getParameters()->setAttribute(scene::ALLOW_ZWRITE_ON_TRANSPARENT, true); /// For transparent textures correct z-order


    std::cout << "\n\nInternal Datas:" << std::endl;
	/// Load all needed files: (package is a ZIP file, in encryption level: AES-128)
	// http://irrlicht.sourceforge.net/docu/classirr_1_1io_1_1_i_file_system.html#a05de4e45b4d36c779b9b8f7cb1dccf7c
    // device->getFileSystem()->addFileArchive("data/world.dll", true, false, EFAT_ZIP, ""); // String filename, ignoreCase, ignorePaths, fileType, password
    device->getFileSystem()->addFileArchive("datas/datas.datas", true, true, EFAT_ZIP, "none"); // String filename, ignoreCase, ignorePaths, fileType, password
    std::cout << "file \"datas.datas\"... Ok" << std::endl;
	// Load all from... (deprecated: addZipFileArchive)
	// device->getFileSystem()->addZipFileArchive("data/world.zip");



    /// Loading Bar: (Create)
    /// ex: driver->removeTexture(backgroundImageTexture); - OR - backgroundImageTexture = driver->getTexture("courtain.jpg"); - OR - NULL
    CProgressBar* loadingBar = new CProgressBar(device, driver, guienv, NULL, rect<s32>(300, SCREEN_HEIGHT/2, SCREEN_WIDTH-300, SCREEN_HEIGHT/2+15), true, 7);  // posX, posY, bar width, bar height
    loadingBar->setPercent(10); // 0 to 100
    loadingBar->draw();


    /// CREATE PHYSICS WORLD:
    /// GImpact is set to true, is needed for nodeScenePhysics, else the whole mesh will be like a box in physics world:
    bool USE_GIMPACT = true;
    world = createIrrBulletWorld(device, USE_GIMPACT, DEBUG_BULLET); /// irr::IrrlichtDevice *const Device, bool useGImpact, bool useDebugDrawer
    world->setDebugMode(irrPhysicsDebugMode::EPDM_DrawAabb | irrPhysicsDebugMode::EPDM_EnableCCD | irrPhysicsDebugMode::EPDM_DrawContactPoints);
    // world->setGravity(vector3df(0, -9.8, 0));
    world->setGravity(vector3df(0.0,-9.8,0.0));
    /// world->getSoftBodyWorldInfo().air_density = 0.05;
    /// world->registerGImpactAlgorithm();

    /// ############# CREATE CAMERAS ###################################################################################################

    /// Cameras:
    ICameraSceneNode* cam;
    // f32 SPEED_CAMERA = 0.0003f; // Default: 0.0010f;


    /// Cameras start position, far away from map:
    initCamZoom = -10000;
    cameraStartPosition = vector3df(1000,500,initCamZoom);
    cameraStartTarget = vector3df(1000,400,0);

    /// Cameras default, near the ship:
    cameraDefaultPosition = vector3df(1000,500,-8100);
    cameraDefaultTarget = vector3df(1000,400,0);

    /// LIMITS ARE...
    maxCamRight  = 5200;
    maxCamLeft   = cameraDefaultPosition.X;
    maxCamTop    = 800;
    maxCamBot    = cameraDefaultPosition.Y;
    minCamZoom   = cameraDefaultPosition.Z;
    maxCamZoom   = cameraDefaultPosition.Z-200;
    zoomCamSpeed = 0.3;



   /// Create an orthogonal (orthographic projection) camera
   /// http://irrlicht.sourceforge.net/forum/viewtopic.php?p=135907
   /*
   ICameraSceneNode * camera = smgr->addCameraSceneNode(0, vector3df(0,0,-10), vector3df(0,0,0));
   matrix4 orthoMatrix;
   orthoMatrix.buildProjectionMatrixOrthoLH(20.f, 15.f, 5.f, 100.f);
   camera->setProjectionMatrix(orthoMatrix);
   camera->setIsOrthogonal(true);
   */

    // Camera:
    cam = NULL;
    cam = smgr->addCameraSceneNode(0, vector3df(0,0,0), vector3df(0,0,0)); // Auto animated Camera
    cam->setMaterialFlag(video::EMF_LIGHTING, false); // Node is affected by LIGHT?
    cam->setPosition(cameraStartPosition);   // Set Camera Position
    cam->setTarget(cameraStartTarget);                       // Set Camera Target (band)
    cam->setInputReceiverEnabled(false);
    cam->setNearValue(500.0f);      /// (Must be set in "RealisticWater.cpp" too)
    cam->setFarValue(15000.0f);     /// Must be bigger than water (Must be set in "RealisticWater.cpp" too)
    cam->setFOV(10*core::DEGTORAD); /// Playing with FOV and Z distance (Must be set in "RealisticWater.cpp" too)
    /// Add animator:
    /// virtual ISceneNodeAnimator* irr::scene::ISceneManager::createFlyStraightAnimator(core::vector3df& startPoint, core::vector3df& endPoint, u32 timeForWay, bool loop = false, bool pingpong = false )
    // scene::ISceneNodeAnimator* anim = smgr->createFlyStraightAnimator(core::vector3df(0,0,0), core::vector3df(0,5,5), 2000.0f, true, true);
    // cam->addAnimator(anim);
    // anim->drop();
    cameras.push_back(cam);


    cam = NULL;
    cam = smgr->addCameraSceneNodeFPS();
    cam->setMaterialFlag(video::EMF_LIGHTING, false); // Node is affected by LIGHT?
    cam->setPosition(cameraDefaultPosition);
    cam->setTarget(cameraDefaultTarget);
    cam->setInputReceiverEnabled(true);
    cam->setNearValue(500.0f);
    cam->setFarValue(15000.0f);      /// Must be bigger than water
    cam->setFOV(10*core::DEGTORAD); /// Playing with FOV and Z distance
    cameras.push_back(cam);


    smgr->setActiveCamera(cameras[0]);
    std::cout << "Cameras... Ok" << std::endl;

    /// Loading Bar:
    loadingBar->setPercent(30); // 0 to 100
    loadingBar->draw();


    /// ############# CREATE TEXTS ###################################################################################################

	/// Add GUI images:
    /// IGUIImage* image = guienv->addImage(driver->getTexture("logotipo.png"), position2d<int>(SCREEN_WIDTH-145,10));
	// image[1] = guienv->addImage(driver->getTexture("logotipo.png"),position2d<int>(SCREEN_WIDTH-145,10));
	// guienv->addGUIElement("myButton", IGUIButton(core::rect< s32> (0,0,200,50), 0, 101, wchar_t "mybutton", wchar_t "clickhere")  )
	/// std::cout << "GUI images... Ok" << std::endl;


    std::cout << "\n\nGUI:" << std::endl;

    /// Initialize GUI stuff:
    skin  = guienv->getSkin();
    fontMain = guienv->getFont("font1.bmp"); // Get Texts Font
    // IGUIFont* font2 = guienv->getFont("font2.bmp"); // Get Texts Font


    /// Fonts for lyrics:
    fonts.push_back(guienv->getFont("font_12_bold.bmp"));
    fonts.push_back(guienv->getFont("font_14.bmp"));
	fonts.push_back(guienv->getFont("font_14_bold.bmp"));
    fonts.push_back(guienv->getFont("font_18.bmp"));
    fonts.push_back(guienv->getFont("font_22.bmp"));
    fonts.push_back(guienv->getFont("font_26.bmp"));
    fonts.push_back(guienv->getFont("font_36.bmp"));
    fonts.push_back(guienv->getFont("font_68.bmp"));
    fonts.push_back(guienv->getFont("font_10.bmp"));
    fonts.push_back(guienv->getFont("font_8.bmp"));


    /// Apply font to GUI:
    if(fontMain){ skin->setFont(fontMain); };
    skin->setColor(gui::EGDC_BUTTON_TEXT, video::SColor(255,210,235,255)); // Set Texts Font Color
    // IGUIStaticText->setTextAlignment - EGUIA_CENTER

    // SCREEN_WIDTH  = driver->getScreenSize().Width;
    // SCREEN_HEIGHT = driver->getScreenSize().Height;
    // s32 pos = ((IGUIScrollBar*)GUIEvent.Caller)->getPos();

    /// Set GUI Colors: (transparencies, etc...)
    std::cout << "Total GUI Elements: " << EGDC_COUNT << std::endl; // Write log
    /// --> Paint GUI firsts elements...
    for(s32 i=0; i<8; i++){ guienv->getSkin()->setColor((EGUI_DEFAULT_COLOR)i, SColor(130,50,50,100)); }; /// --> Paint GUI firsts elements... (opacity, red, green, blue)

    /// --> ...Then, these ones...
    guienv->getSkin()->setColor((EGUI_DEFAULT_COLOR)9, SColor(150,255,255,255));  /// --> Text Fields Colors - ID:9 (opacity, red, green, blue)
    guienv->getSkin()->setColor((EGUI_DEFAULT_COLOR)10, SColor(200,50,50,100));   /// --> Selectors Background Color - ID:16 (opacity, red, green, blue)
    // guienv->getSkin()->setColor((EGUI_DEFAULT_COLOR)11, SColor(255,255,0,0));  /// --> Unknown... ID:11 to 15 (opacity, red, green, blue)
    guienv->getSkin()->setColor((EGUI_DEFAULT_COLOR)16, SColor(100,200,200,250)); /// --> Scrollbars - ID:16 (opacity, red, green, blue)
    // guienv->getSkin()->setColor((EGUI_DEFAULT_COLOR)17, SColor(255,255,0,0));  /// --> Unknown... ID:17 (opacity, red, green, blue)
    guienv->getSkin()->setColor((EGUI_DEFAULT_COLOR)18, SColor(200,200,200,200)); /// --> Cursors and Window Close button ID:18 (opacity, red, green, blue)
    guienv->getSkin()->setColor((EGUI_DEFAULT_COLOR)19, SColor(200,200,200,200)); /// --> Directories Icons Colors - ID:19 (opacity, red, green, blue)
    // guienv->getSkin()->setColor((EGUI_DEFAULT_COLOR)20, SColor(255,255,0,0));  /// --> Unknown... ID:20 (opacity, red, green, blue)
    // guienv->getSkin()->setColor((EGUI_DEFAULT_COLOR)21, SColor(255,255,0,0));  /// --> Unknown... ID:21 (opacity, red, green, blue)
    guienv->getSkin()->setColor((EGUI_DEFAULT_COLOR)22, SColor(80,0,0,0));        /// --> Text Path Background Color - ID:22 (opacity, red, green, blue)


    /// --> ...Finally...
    for(s32 i=23; i<gui::EGDC_COUNT; ++i){ guienv->getSkin()->setColor((EGUI_DEFAULT_COLOR)i, SColor(255,255,255,255)); }; /// --> Paint GUI possible other unknown elements... (opacity, red, green, blue)


    /// Disable window (is visible, but cant be used)
    // guienv->getSkin()->setColor(EGDC_WINDOW, SColor(255, 255,0,0));
    // IGUIInOutFader* fader;
    // fader = device->getGUIEnvironment()->addInOutFader();
    // fader->setColor(video::SColor(0,255,255,255));
    // fader->fadeIn(900);

    /// Add texts:
    /*
    IGUIStaticText* text;
    text = guienv->addStaticText(L"1000", core::rect<s32>(0,0,0,0), false, false, 0); // border? wordwrap?
    text->setOverrideColor(video::SColor(255,210,235,255)); // opacity, red, green, blue
    text->setOverrideFont(fonts[0]);
    text->setRelativePosition(core::rect<s32>(200,130,SCREEN_WIDTH,SCREEN_HEIGHT));
    text->setVisible(false);
    texts.push_back(text);
    */

    /// Texts:
    /// IGUIStaticText* textInfos1;
    stringw ti1 = L"";
    ti1 += L"GAME KEYS  -  up: Q  -  down: A  -  fire: SPACE\n";
    ti1 += L"MORE KEYS  -  exit: ESC  -  camera: ARROWS  -  texts: T  -  music: M";
    textInfos1 = guienv->addStaticText(ti1.c_str(), core::rect<s32>(0,0,0,0), false, true, 0); // border? wordwrap?
    textInfos1->setOverrideColor(video::SColor(255,255,255,255)); // opacity, red, green, blue
    textInfos1->setOverrideFont(fonts[8]);
    textInfos1->setRelativePosition(core::rect<s32>(0,0,0,0)); /// Updated in main loop, at "game-level.cpp"
    textInfos1->setTextAlignment(EGUIA_CENTER, EGUIA_CENTER);  // ALIGN: horizontal, vertical -> Enumerator: EGUIA_UPPERLEFT, EGUIA_LOWERRIGHT, EGUIA_CENTER, EGUIA_SCALE
    textInfos1->setVisible(false);

    /// Total Score Texts:
    /// IGUIStaticText* textTotalScore;
    textTotalScore = guienv->addStaticText(L"SCORE: 0", core::rect<s32>(0,0,0,0), false, false, 0); // border? wordwrap?
    textTotalScore->setOverrideColor(video::SColor(255,255,255,255)); // opacity, red, green, blue
    textTotalScore->setOverrideFont(fonts[5]);
    textTotalScore->setRelativePosition(core::rect<s32>(20,20,SCREEN_WIDTH,SCREEN_HEIGHT));
    textTotalScore->setVisible(false);

    /// Angles Texts:
    /// IGUIStaticText* textAngle;
    textAngle = guienv->addStaticText(L"0", core::rect<s32>(0,0,0,0), false, false, 0); // border? wordwrap?
    textAngle->setOverrideColor(video::SColor(255,255,255,255)); // opacity, red, green, blue
    textAngle->setOverrideFont(fonts[2]);
    textAngle->setRelativePosition(core::rect<s32>(125,190,SCREEN_WIDTH,SCREEN_HEIGHT));
    textAngle->setVisible(false);




    /// Score Texts:
    /// scene::ITextSceneNode* textScore;
    textScore = smgr->addTextSceneNode(fonts[2], L"", video::SColor(255,255,255,255), 0, vector3df(0,0,0), 0);
    textScore->setText(L"");
    textScore->setTextColor(SColor(255,255,255,255));
    textScore->setPosition(vector3df(0,0,0));
    textScore->setVisible(false);

    /// Bonus Texts:
    /// scene::ITextSceneNode* textBonus;
    textBonus = smgr->addTextSceneNode(fonts[2], L"", video::SColor(255,255,255,255), 0, vector3df(0,0,0), 0);
    textBonus->setText(L"");
    textBonus->setTextColor(SColor(255,255,255,255));
    textBonus->setPosition(vector3df(0,0,0));
    textBonus->setVisible(false);


    /// Create names for the personages:
    if(NAMES_TEXTS==true){
    video::SColor color = SColor(255,255,170,0);
    if(true){ /// Array: 0
    ITextSceneNode* text;
    text = smgr->addTextSceneNode(fonts[9], L"", color, 0, vector3df(0,0,0), -1);
    text->setText(L""); /// Pirate
    text->setTextColor(color);
    text->setPosition(vector3df(0,0,0));
    text->setVisible(false);
    personageName.push_back(text);
    };
    if(true){ /// Array: 1
    ITextSceneNode* text;
    text = smgr->addTextSceneNode(fonts[9], L"", color, 0, vector3df(0,0,0), -1);
    text->setText(L"Kinuri");
    text->setTextColor(color);
    text->setPosition(vector3df(0,0,0));
    text->setVisible(false);
    personageName.push_back(text);
    };
    if(true){ /// Array: 2
    ITextSceneNode* text;
    text = smgr->addTextSceneNode(fonts[9], L"", color, 0, vector3df(0,0,0), -1);
    text->setText(L"Blacki");
    text->setTextColor(color);
    text->setPosition(vector3df(0,0,0));
    text->setVisible(false);
    personageName.push_back(text);
    };
    if(true){ /// Array: 3
    ITextSceneNode* text;
    text = smgr->addTextSceneNode(fonts[9], L"", color, 0, vector3df(0,0,0), -1);
    text->setText(L"Olivi");
    text->setTextColor(color);
    text->setPosition(vector3df(0,0,0));
    text->setVisible(false);
    personageName.push_back(text);
    };
    if(true){ /// Array: 4
    ITextSceneNode* text;
    text = smgr->addTextSceneNode(fonts[9], L"", color, 0, vector3df(0,0,0), -1);
    text->setText(L"Johni");
    text->setTextColor(color);
    text->setPosition(vector3df(0,0,0));
    text->setVisible(false);
    personageName.push_back(text);
    };
    if(true){ /// Array: 5
    ITextSceneNode* text;
    text = smgr->addTextSceneNode(fonts[9], L"", color, 0, vector3df(0,0,0), -1);
    text->setText(L"Bobbi");
    text->setTextColor(color);
    text->setPosition(vector3df(0,0,0));
    text->setVisible(false);
    personageName.push_back(text);
    };
    if(true){ /// Array: 6
    ITextSceneNode* text;
    text = smgr->addTextSceneNode(fonts[9], L"", color, 0, vector3df(0,0,0), -1);
    text->setText(L"Wizki");
    text->setTextColor(color);
    text->setPosition(vector3df(0,0,0));
    text->setVisible(false);
    personageName.push_back(text);
    };
    if(true){ /// Array: 7
    ITextSceneNode* text;
    text = smgr->addTextSceneNode(fonts[9], L"", color, 0, vector3df(0,0,0), -1);
    text->setText(L"Wizmi");
    text->setTextColor(color);
    text->setPosition(vector3df(0,0,0));
    text->setVisible(false);
    personageName.push_back(text);
    };
    /// Priests:
    if(true){ /// Array: 8
    ITextSceneNode* text;
    text = smgr->addTextSceneNode(fonts[9], L"", color, 0, vector3df(0,0,0), -1);
    text->setText(L"Killi");
    text->setTextColor(color);
    text->setPosition(vector3df(0,0,0));
    text->setVisible(false);
    personageName.push_back(text);
    };
    if(true){ /// Array: 9
    ITextSceneNode* text;
    text = smgr->addTextSceneNode(fonts[9], L"", color, 0, vector3df(0,0,0), -1);
    text->setText(L"Gutti");
    text->setTextColor(color);
    text->setPosition(vector3df(0,0,0));
    text->setVisible(false);
    personageName.push_back(text);
    };
    if(true){ /// Array: 10
    ITextSceneNode* text;
    text = smgr->addTextSceneNode(fonts[9], L"", color, 0, vector3df(0,0,0), -1);
    text->setText(L"Zani");
    text->setTextColor(color);
    text->setPosition(vector3df(0,0,0));
    text->setVisible(false);
    personageName.push_back(text);
    };
    if(true){ /// Array: 11
    ITextSceneNode* text;
    text = smgr->addTextSceneNode(fonts[9], L"", color, 0, vector3df(0,0,0), -1);
    text->setText(L"Keni");
    text->setTextColor(color);
    text->setPosition(vector3df(0,0,0));
    text->setVisible(false);
    personageName.push_back(text);
    };
    /// Soldiers:
    if(true){ /// Array: 12
    ITextSceneNode* text;
    text = smgr->addTextSceneNode(fonts[9], L"", color, 0, vector3df(0,0,0), -1);
    text->setText(L"Kuki");
    text->setTextColor(color);
    text->setPosition(vector3df(0,0,0));
    text->setVisible(false);
    personageName.push_back(text);
    };
    if(true){ /// Array: 13
    ITextSceneNode* text;
    text = smgr->addTextSceneNode(fonts[9], L"", color, 0, vector3df(0,0,0), -1);
    text->setText(L"Nimi");
    text->setTextColor(color);
    text->setPosition(vector3df(0,0,0));
    text->setVisible(false);
    personageName.push_back(text);
    };
    if(true){ /// Array: 14
    ITextSceneNode* text;
    text = smgr->addTextSceneNode(fonts[9], L"", color, 0, vector3df(0,0,0), -1);
    text->setText(L"Alexi");
    text->setTextColor(color);
    text->setPosition(vector3df(0,0,0));
    text->setVisible(false);
    personageName.push_back(text);
    };
    if(true){ /// Array: 15
    ITextSceneNode* text;
    text = smgr->addTextSceneNode(fonts[9], L"", color, 0, vector3df(0,0,0), -1);
    text->setText(L"Vigi");
    text->setTextColor(color);
    text->setPosition(vector3df(0,0,0));
    text->setVisible(false);
    personageName.push_back(text);
    };
    if(true){ /// Array: 16
    ITextSceneNode* text;
    text = smgr->addTextSceneNode(fonts[9], L"", color, 0, vector3df(0,0,0), -1);
    text->setText(L"Gini");
    text->setTextColor(color);
    text->setPosition(vector3df(0,0,0));
    text->setVisible(false);
    personageName.push_back(text);
    };
    if(true){ /// Array: 17
    ITextSceneNode* text;
    text = smgr->addTextSceneNode(fonts[9], L"", color, 0, vector3df(0,0,0), -1);
    text->setText(L"Baki");
    text->setTextColor(color);
    text->setPosition(vector3df(0,0,0));
    text->setVisible(false);
    personageName.push_back(text);
    };
    if(true){ /// Array: 18
    ITextSceneNode* text;
    text = smgr->addTextSceneNode(fonts[9], L"", color, 0, vector3df(0,0,0), -1);
    text->setText(L"Pelli");
    text->setTextColor(color);
    text->setPosition(vector3df(0,0,0));
    text->setVisible(false);
    personageName.push_back(text);
    };
    if(true){ /// Array: 19
    ITextSceneNode* text;
    text = smgr->addTextSceneNode(fonts[9], L"", color, 0, vector3df(0,0,0), -1);
    text->setText(L"Figi");
    text->setTextColor(color);
    text->setPosition(vector3df(0,0,0));
    text->setVisible(false);
    personageName.push_back(text);
    };
    /// Prisoners:
    if(true){ /// Array: 20 - PRISONER
    ITextSceneNode* text;
    text = smgr->addTextSceneNode(fonts[9], L"", color, 0, vector3df(0,0,0), -1);
    text->setText(L""); /// text->setText(L"Hugi");
    text->setTextColor(color);
    text->setPosition(vector3df(0,0,0));
    text->setVisible(false);
    personageName.push_back(text);
    };
    if(true){ /// Array: 21 - PRISONER
    ITextSceneNode* text;
    text = smgr->addTextSceneNode(fonts[9], L"", color, 0, vector3df(0,0,0), -1);
    text->setText(L""); /// text->setText(L"Nuni");
    text->setTextColor(color);
    text->setPosition(vector3df(0,0,0));
    text->setVisible(false);
    personageName.push_back(text);
    };
    if(true){ /// Array: 22 - PRISONER
    ITextSceneNode* text;
    text = smgr->addTextSceneNode(fonts[9], L"", color, 0, vector3df(0,0,0), -1);
    text->setText(L""); /// text->setText(L"Becki");
    text->setTextColor(color);
    text->setPosition(vector3df(0,0,0));
    text->setVisible(false);
    personageName.push_back(text);
    };
    if(true){ /// Array: 23 - PRISONER
    ITextSceneNode* text;
    text = smgr->addTextSceneNode(fonts[9], L"", color, 0, vector3df(0,0,0), -1);
    text->setText(L""); /// text->setText(L"Heldi");
    text->setTextColor(color);
    text->setPosition(vector3df(0,0,0));
    text->setVisible(false);
    personageName.push_back(text);
    };
    if(true){ /// Array: 24
    ITextSceneNode* text;
    text = smgr->addTextSceneNode(fonts[9], L"", color, 0, vector3df(0,0,0), -1);
    text->setText(L"Roxi Band");
    text->setTextColor(color);
    text->setPosition(vector3df(0,0,0));
    text->setVisible(false);
    personageName.push_back(text);
    };
    if(true){ /// Array: 25
    ITextSceneNode* text;
    text = smgr->addTextSceneNode(fonts[9], L"", color, 0, vector3df(0,0,0), -1);
    text->setText(L"Wizani");
    text->setTextColor(color);
    text->setPosition(vector3df(0,0,0));
    text->setVisible(false);
    personageName.push_back(text);
    };
    if(true){ /// Array: 26
    ITextSceneNode* text;
    text = smgr->addTextSceneNode(fonts[9], L"", color, 0, vector3df(0,0,0), -1);
    text->setText(L"Delfi");
    text->setTextColor(color);
    text->setPosition(vector3df(0,0,0));
    text->setVisible(false);
    personageName.push_back(text);
    };
    if(true){ /// Array: 27
    ITextSceneNode* text;
    text = smgr->addTextSceneNode(fonts[9], L"", color, 0, vector3df(0,0,0), -1);
    text->setText(L"Sunni");
    text->setTextColor(color);
    text->setPosition(vector3df(0,0,0));
    text->setVisible(false);
    personageName.push_back(text);
    };
    if(true){ /// Array: 28
    ITextSceneNode* text;
    text = smgr->addTextSceneNode(fonts[9], L"", color, 0, vector3df(0,0,0), -1);
    text->setText(L"Kotti");
    text->setTextColor(color);
    text->setPosition(vector3df(0,0,0));
    text->setVisible(false);
    personageName.push_back(text);
    };
    }; /// Ends: NAMES_TEXTS


    std::cout << "GUI texts... Ok" << std::endl;

    /// Loading Bar:
    loadingBar->setPercent(50); // 0 to 100
    loadingBar->draw();


    /// Set variables:
    lastFPS = -1;
    then = device->getTimer()->getTime();
    frameDeltaTime = 0.0;


    /// Loading Bar:
    loadingBar->setPercent(100);   // Last One: Must be 100
    loadingBar->draw();
    delete loadingBar; /// Remove now
    loadingBar = NULL; /// Remove now

};






#include "game-level.cpp"


/// CORE:
int main(int arc, char* argv[]){

	/// Game:
    Game* game = new Game();
    game->Initialize();

    Level* level = new Level();
    level->Initialize();

        /// Main Loop:
        while(device->run() && EXIT_GAME==false){
        level->Loop();
        };

	/// Exit game:
	game->Exit();
};





bool exitingOnce = true;
void Game::Exit(){
    if(exitingOnce){
    exitingOnce = false;

    /*
    if(audiereDevice){
    audiereDevice->clearCallbacks();
    audiereDevice = NULL;
    // delete audiereDevice;
    };
    */


    /// Free and close Bullet Physics:
    /// Bullet: Removes all from physics world:
    if(world){
    while(world->getNumCollisionObjects()>0){
    world->removeCollisionObject(world->getCollisionObjectByIndex(0));
    };
    // delete world;
    };

    /// Free and close Irrlicht:
    if(device){ device->closeDevice(); };
	if(device){ device->drop();        };
	// if(device){ delete device;         };
	// return 0;
    };
};


