
#ifndef START_SCREEN_H
#define START_SCREEN_H

#include "Litha.h"
#include "SimpleMenu.h"

class MainState;
class Level;

// Start Screen is well and truly hacked in, since I never intended to have one.
// So some stuff may be duplicated from MainState.
class StartScreen : public IUpdatable, public IWantEvents
{
	IEngine *engine;
	IRenderSystem *renderSystem;
	IrrlichtDevice *device;
	IWorld *world;
	
	MainState *mainState;
	
	Level *levelPreview;
	IMesh *titleMesh;
	gui::IGUIStaticText *levelTitleText;
	gui::IGUIStaticText *levelRatingText;
	gui::IGUIStaticText *levelFractionText;
	
	IThirdPersonCameraController *camControl;
	ITransformable *camTarget;
	
	// first time user runs game?
	bool firstRun;
	
	// Location in main.cpp where another pointer to MainState must be stored...
	MainState **mainStatePtrLoc;
	
	SimpleMenu *menu;
	SimpleEitherSideToggleMenu *menuLevelSelect;
	
	std::vector<SimpleMenu *> optionsMenus;
	
	// for options menu
	VariantMap originalConfig;
	VariantMap newConfig;
	
	
	std::vector<core::stringc> levelFileNames;
	VariantMap levelTitles;
	core::stringc furthestLevelReached;
	
	
	bool fadingIntoGame;
	
	void CreatePreviewLevel(core::stringc previewLevelName);
	void UpdateLevelCamera();
	void CreateFirstMenu();
	void CreateLevelSelectButtons();
	
	void AndSoItBegins();
	
	void ShowOptionsMenu(VariantMap settings);
	
public:
	StartScreen(MainState **mainStatePtrLoc);
	~StartScreen();
	
	void Update(f32 dt);
	void OnEvent(const Event &event);
};

#endif
