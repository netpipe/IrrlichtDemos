
#ifndef MAIN_STATE_H
#define MAIN_STATE_H

#include "Litha.h"
#include <vector>
#include "Positioner.h"
#include <deque>

core::stringc level_path_rel_exe(core::stringc levelFile);
io::path get_full_save_path();
io::path get_full_save_path_furthest();
void delete_game_saves();
std::vector<core::stringc> find_levels();
core::stringc find_next_level(bool fromFurthest = false);

gui::IGUIStaticText *add_static_text2(const wchar_t *str);
gui::IGUIStaticText *add_static_text(const wchar_t *str);

class Level;
class Editor;
struct UndoState;

class MainState : public IUpdatable, public IWantEvents
{
	IEngine *engine;
	IWorld *world;
	IRenderSystem *renderSystem;
	
	IrrlichtDevice *device;
	
	Level *level;
	Editor *editor;
	
	ISound2D *menuSound;
	
	bool nextLevel;
	bool isPaused;
	
	// some final scene pointers and other things
	bool inFinalScene;
	bool walkedLeft;//did player walk off to the left or right of beach?
	ISound3D *finalSceneSea;
	std::vector<scene::ISceneNode *> finalSceneNodes;
	
	// this are just the name, no path.
	std::vector<core::stringc> levelFileNames;
	
	// And perhaps some descriptive names for the levels!
	VariantMap levelTitles;
	
	// shown only on first level after game started
	// sometimes used for other purposes too...
	std::vector<gui::IGUIStaticText *> introTexts;
	
	// level name
	std::vector<gui::IGUIStaticText *> startLevelTexts;
	
	// time at which point the game is properly entered...
	f32 initFirstLevelTime;
	
	// reset on each StartLevel.
	// -1.0 if not yet been set.
	f32 startLevelTime;
	
#ifdef TRIAL
	bool shownBetweenLevelTrialScreen;
#endif
	
	enum E_PAUSE_MENU_IDS
	{
		EPM_EXIT = 1,
		EPM_NEW_GAME,
		EPM_CHANGE_LEVEL,
		EPM_RESTART_LEVEL,
		EPM_CONTINUE,
		EPM_SKIP_LEVEL,
		
		// Trial hidden sub menu
		// (appears between levels etc)
#ifdef TRIAL
		EPM_TRIAL_BUY,
#endif
		
		// New Game sub menu
		EPM_NEW_GAME_YES,
		EPM_NEW_GAME_NO,
		
		// Skip level sub menu
		EPM_SKIP_LEVEL_YES,
		EPM_SKIP_LEVEL_NO
	};
	
	RowPositioner *pauseMenuPositioner;
	RowPositioner *levelSelectMenuPositioner;
	RowPositioner *levelConfirmMenuPositioner;
	
	// Location in main.cpp where another pointer to MainState must be stored...
	MainState **mainStatePtrLoc;
	
	// WARNING: This must be followed by a call to StartFirstLevel.
	// (which inits the text fade-off timer)
	void ShowInitTexts();
	void RemoveInitTexts();
	void RemoveStartLevelTexts();
	
	void ShowEndTexts();
	bool gameEnded; // complete end of game (or just trial)
	
	core::stringc GetCurrentLevelName();
	void RemoveLevelAndEditor();
	
	// Get the full path to the save file.
	// CHANGED TO A GLOBAL FUNCTION!
	// for access by StartScreen.
	//io::path GetFullSavePath();
	
public:
	MainState(MainState **mainStatePtrLoc);
	~MainState();
	
	core::stringc TryGetCurrentLevelName()
	{
		if (level)
			return GetCurrentLevelName();
		else
			return "";
	}
	
	// Load the level, but only preview it. (level->Start() is not called yet)
	// filename is *NOT* absolute (must only be the filename, no directory)
	//void PreviewLevel(core::stringc levelFileName);
	
	// Load level (reloads it even if started before with PreviewLevel...), and actually start it,
	// with player control. (level->Start() is called)
	// filename *NOT* absolute
	void StartLevel(core::stringc levelFileName, bool startEditor = false, std::deque<UndoState> *undoHistory = NULL);
	
	// Start playing a level that is currently being previewed (from a call to PreviewLevel)
	//void StartFromPreview();
	
	// Start the game from scratch, wiping out any previous progress.
	void StartFirstLevel();
	
	// End the current level and start playing the next one.
	// Called when touched the end of level portal.
	// Note, this does not end the level immediately (since it may be Level that calls this method).
	// wait - should we wait a frame before actually processing this request?
	// 		(should be set when calling from level, since level should not destruct itself!)
	void NextLevel(bool wait = false);
	
	// Restart the currently playing level.
	// (e.g. on player death, suicide)
	
	// Essentially the same as calling StartLevel again.
	// can optionally use an undo state rather than loading from file.
	void RestartLevel(std::deque<UndoState> *undoHistory = NULL);
	
	void OnPause();
	void OnResume();
	
	void Update(f32 dt);
	
	void OnEvent(const Event &event);
	
	void ShowPauseMenu();
	void HidePauseMenu();
};

#endif


