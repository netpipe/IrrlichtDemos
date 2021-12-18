// Jorge Rosa (1999 - 2018 Portugal)
// Email: jorge.bigarte@gmail.com
// Portfolio: https://sites.google.com/site/jorgerosaportfolio
// Facebook: https://www.facebook.com/jorge.bigarte
// Youtube: https://www.youtube.com/user/jorgeerosa/videos

/// ################################################################################################################################################################################
///
/// Compiling in my system: (May be different in yours)
///
/// Windows 7 (64 Bits)
/// Code::Blocks 16.01 (32 Bits) - Installed at: "C:\Program Files (x86)\CodeBlocks"
/// The selected compiler: GNU GCC Compiler - Installed at: "C:\MinGW"
/// The "Toolchain executables" are: (set by this order) "mingw32-gcc.exe","mingw32-g++.exe","mingw32-g++.exe","ar.exe","GDB/CDB Debugger: Default","windres.exe","mingw32-make.exe"
///
/// ################################################################################################################################################################################
/// STATIC LIBRARIES in Code::Blocks: add '-static' in settings --> compiler --> Linker settings --> Other linker options

/// TO INSTALL THE NEEDED LIBS, PLEASE TAKE A LOOK AT "header.h" FILE
#include "headers.h"

/// Define Irrlicht Global Variables:
// scene::ICameraSceneNode* camera[2] = {0,0};
IrrlichtDevice* device           = NULL;  // Create Device
irr::video::IVideoDriver* driver = NULL;  // Create Driver
irr::scene::ISceneManager* smgr  = NULL;  // Create Scene Manager
irr::io::IFileSystem* filesys    = NULL;  // getFileSystem();
gui::IGUIEnvironment* guienv     = NULL;  // GUI (For Texts and 2D Images)

/// Other Global Variables:
irr::u32 SCREEN_WIDTH  = 850;   // Will be overrided by settings file.
irr::u32 SCREEN_HEIGHT = 500;   // Will be overrided by settings file.
bool FULLSCREEN        = false; // Will be overrided by settings file.
bool STENCIL_SHADOWS   = false; // NOT set in the settings file, yet.
bool VSYNC             = true;  // Limit rendering to the screen refresh rate (true == to avoid crashes and hardware damages)
int FRAMES_PER_SECOND  = 0;     // FOR: If FPS < 60 then do not do "this effect", etc...

/// Timings:
f32 frameDeltaTime = 0.0;

/// External Code:
#include "world.cpp"





/// Main:
int main(){

    /// ############# CREATE ENGINE ############################################################

	/// Loop to chose driver from all available:
    video::E_DRIVER_TYPE drivertype;
    // for (int i=0; i<irr::video::EDT_COUNT; i++){
    // if (irr::IrrlichtDevice::isDriverSupported(irr::video::E_DRIVER_TYPE(i))){ driverType = irr::video::E_DRIVER_TYPE(i); };
    // };
    /// Force to set video driver to OPENGL:
    drivertype = EDT_OPENGL;

    /// Create device from the above found driver:
	device = createDevice(drivertype, core::dimension2d<u32>(SCREEN_WIDTH, SCREEN_HEIGHT), 32, FULLSCREEN, STENCIL_SHADOWS, VSYNC, 0); /// OK!... Or using the next code...
	/// device = createDevice(deviceType = video::EDT_SOFTWARE, core::dimension2d< u32 >(640, 480), u32 bits = 16, bool fullscreen = false, bool stencilbuffer = false, bool vsync = false, IEventReceiver* receiver = 0

    if (device == 0){ return 1; }; /// If device fails... Do nothing...

	driver = device->getVideoDriver();    // Create Driver
	filesys = device->getFileSystem();    // Access Files
	smgr = device->getSceneManager();     // Create Scene Manager
	guienv = device->getGUIEnvironment(); // For GUI texts and 2D Images
	// video::IGPUProgrammingServices* gpu = driver->getGPUProgrammingServices(); // Not used, by now...

	/// Global light:
	smgr->setAmbientLight(video::SColorf(0.1,0.1,0.1,0.0)); // Ambient Light. SColorf == r, g, b, a (0 = transparent | 255 = opaque)
	/// For STENCIL_SHADOWS:
	/// Set shadows colors:
	smgr->setShadowColor(video::SColor(220,0,0,0)); // a (0 = transparent | 255 = opaque), r, g, b
	/// Some tweaks to accelerate the shadows process:
	// device->IsUseStencilBuffer();
    // scene->setShadowDelayerMax(2);
    // scene->setAutoShadowDelayer(true);

	// Add an events receiver: (keyboard)
	MyEventReceiver receiver;
    device->setEventReceiver(&receiver);

    /// Device: Other Stuff: (Window)
    device->setWindowCaption(L"3D Model: FAIREY IIID - V.0.0.1 | Mouse Controls | Left Button: Rotation | Middle Button: Zoom | Right Button: Position");
	// device->getCursorControl()->setVisible(true);  // Mouse cursor visibility
	device->setResizable(true); // Resizable window
	// Mouse cursor:
    device->getCursorControl()->setVisible(true);

	/// Driver: Other Stuff: (for transparencies)
	driver->setTextureCreationFlag(E_TEXTURE_CREATION_FLAG::ETCF_ALWAYS_32_BIT, true);
	driver->setTextureCreationFlag(E_TEXTURE_CREATION_FLAG::ETCF_CREATE_MIP_MAPS, true);

	/// Fog:
	/// virtual void setFog(SColor color=SColor(0,255,255,255), E_FOG_TYPE fogType=EFT_FOG_LINEAR, f32 start=50.0f, f32 end=100.0f, f32 density=0.01f, bool pixelFog=false, bool rangeFog=false) =0;
	driver->setFog(SColor(0,0,0,0), EFT_FOG_LINEAR, 1000, 9000, 5.0f, false, false);

    /// Materials:
	driver->getMaterialRenderer(video::EMT_PARALLAX_MAP_SOLID);

    /// Driver: Other Stuff: (for smoothness)
	// driver->getMaterial2D().TextureLayer[0].BilinearFilter = true;
	// driver->getMaterial2D().TextureLayer[1].BilinearFilter = true;
	// driver->getMaterial2D().TextureLayer[2].BilinearFilter = true;
	// driver->getMaterial2D().TextureLayer[3].BilinearFilter = true;
    // driver->getMaterial2D().AntiAliasing = video::EAAM_FULL_BASIC;

    /// Load Data:
    device->getFileSystem()->addFileArchive("world/world.zip", true, true, EFAT_ZIP, "none"); // String filename, ignoreCase, ignorePaths, fileType, password


    /// #########################################################################################

    /// All stuff from file "world.cpp":
    createWorld();

    /// #########################################################################################


    /// Initializing Timers:
    // f32 lastFPS = -1;
    // u32 then = device->getTimer()->getTime();
    /// Loop variables:
    bool quit = false;


        /// Main Loop:
        while(device->run() && quit == false){

        /// Clear:
        driver->beginScene(true, true, video::SColor(0,0,0,0));

        /// Work out a frame delta time:
        // const u32 now = device->getTimer()->getTime();
        // frameDeltaTime = (f32)(now - then);
        // then = now;


        /// Keyboard events: (exit)
        if(receiver.IsKeyDown(irr::KEY_ESCAPE)){ quit = true; };


        /// Keyboard events: (camera)
        /*
        if(receiver.IsKeyDown(irr::KEY_LEFT)){
        camera->setPosition(core::vector3df(camera->getPosition().X-8.0, camera->getPosition().Y, camera->getPosition().Z));
        };
        if(receiver.IsKeyDown(irr::KEY_RIGHT)){
        camera->setPosition(core::vector3df(camera->getPosition().X+8.0, camera->getPosition().Y, camera->getPosition().Z));
        };
        if(receiver.IsKeyDown(irr::KEY_UP)){
        camera->setPosition(core::vector3df(camera->getPosition().X, camera->getPosition().Y+8.0, camera->getPosition().Z));
        };
        if(receiver.IsKeyDown(irr::KEY_DOWN)){
        camera->setPosition(core::vector3df(camera->getPosition().X, camera->getPosition().Y-8.0, camera->getPosition().Z));
        };

        float distance = camera->getPosition().getDistanceFrom(nodePlane->getPosition());

        if(receiver.IsKeyDown(irr::KEY_MINUS)){
        camera->setPosition(core::vector3df(camera->getPosition().X, camera->getPosition().Y, camera->getPosition().Z-8.0));
        };
        if(receiver.IsKeyDown(irr::KEY_PLUS)){
        camera->setPosition(core::vector3df(camera->getPosition().X, camera->getPosition().Y, camera->getPosition().Z+8.0));
        };
        */


        /// STARTS RENDER:
        smgr->drawAll();
        /// Updates all 2D materials: (else some will be inverted/distorted when using effects for example)
        driver->setMaterial(driver->getMaterial2D());
        /// Resize 2D graphics:
        image->setMaxSize(dimension2du(driver->getScreenSize().Width, driver->getScreenSize().Height));
        /// Draw 2D Graphics: (NOTE: Comment next line, if you want to hide the TV)
        guienv->drawAll(); /// Draw GUI (2D Graphics, above all)
        /// ENDS RENDER:
        driver->endScene();

        };




    /// Free and close Irrlicht:
    if(device){ device->closeDevice(); };
	if(device){ device->drop(); };
	return 0;

};


