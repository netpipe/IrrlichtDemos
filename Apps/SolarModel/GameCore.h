#ifndef __COSMIC_GAMECORE_H_INCLUDED__
#define __COSMIC_GAMECORE_H_INCLUDED__


#include "GameGlobals.h"


struct GameStruct
{
	s32 BitsPerPixel; // [Setup] (s32) Bits per pipxel (fullscreen mode only)
	s32 Width, Height; // [Setup] (s32) Width & Height of the window
	video::E_DRIVER_TYPE DriverType; // [Setup] (video::E_DRIVER_TYPE) Driver Type
	bool Fullscreen; // [Setup] (bool) Fullscreen mode
	bool AntiAlias; // [Setup] (bool) Fullscreen antialiasing
	bool Vsync; // [Setup] (bool) Video synchronization (fullscreen mode only)
	int PerPixelLighting; // [Setup] (int) 0=do not use; 1=use bump mapping; 2=use parallax mapping

	bool ClearZBuffer; // [Runtime] (bool) Perform clearing of z-buffer each frame
	bool ClearBackBuffer; // [Runtime] (bool) Perform clearing of back buffer each frame
	video::SColor ClearColor; // [Runtime] (video::SColor) if ClearBackBuffer is true then this color used
	bool BackgroundRendering; // [Runtime] (bool) if BackgroundRendering is true then perform rendering even when window is inactive
	s32 ScreenshotQuality; // [Runtime] (s32) JPG qulity from 1 to 100
	bool ScreenshotWithoutGUI; // [Runtime] (bool) Do clean screenshots by GameScreenshot()

	IrrlichtDevice* Device; // (IrrlichtDevice*) IrrLicht device
	video::IVideoDriver* Video; // (video::IVideoDriver*) IrrLicht video driver
	scene::ISceneManager* Scene; // (scene::ISceneManager*) IrrLicht scene manager
	scene::ISceneCollisionManager* SceneCollision; // (scene::ISceneCollisionManager*) IrrLicht scene collision manager
	scene::IMeshManipulator* MeshManipulator; // (scene::IMeshManipulator*) Irrlicht scene mesh manipulator
	gui::ICursorControl* Cursor; // (gui::ICursorControl*) IrrLicht cursor control
	gui::IGUIEnvironment* GUI; // (gui::IGUIEnvironment*) IrrLicht GUI environment
	gui::IGUISkin* Skin; // (gui::IGUISkin*) IrrLicht skin for GUI elements
	io::IFileSystem* FS; // (io::IFileSystem*) IrrLicht file system
	IOSOperator* OS; // (IOSOperator*) IrrLicht operation system
	ILogger* Logger; // (ILogger*) IrrLicht logger
	ITimer* Timer; // (ITimer*) IrrLicht timer

	GameEventReceiver* EventReceiver; // (GameEventReceiver*) Event receiver
	GameMap* Map; // (GameMapClass*) Map
};


extern GameStruct Game;


extern void GameSetup();
extern void GameInit();
extern bool GameEvent(SEvent event);
extern bool GameRun();
extern void GamePost();
extern void GameDone();


scene::ICameraSceneNode* GameFPSCamera(const core::vector3df & position, float rotate_speed, float move_speed, scene::ISceneNode* parent=NULL);
void GameLog(char *str, ...);
bool GameScreenshot(GameStruct *pGame, core::stringc &filename);
bool GameIsMaterialSupported(video::E_MATERIAL_TYPE material);
void GameCheckCompatibilities();
void GameCreateIrrLicht(GameStruct *pGame);
void GameDefaults(GameStruct *pGame);
int main();


#endif
