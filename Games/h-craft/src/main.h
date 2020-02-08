// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#ifndef MAIN_H
#define MAIN_H

#include "compile_config.h"
#include "hover.h"
#include <string>
#include <vector>

class Config;
class Controller;
class Physics;
class Gui;
class InputDeviceManager;
class LevelManager;
class IrrlichtManager;
class NodeManager;
class Game;
class Editor;
class Physics;
class ISound;
class MusicManager;
class RandomGenerator;
class StringTable;
class FontManager;
class ProfileManager;
class Championship;
class HighscoreManager;
class RivalsMode;
class AppTester;
struct android_app;
class IAdvert;
class IBilling;

enum APP_MODES
{
    MODE_GUI,
    MODE_FREEFLY,
    MODE_GAME,
    MODE_EDITOR,
    MODE_GUI_DLG_TEST,
};

class App
{
    friend int main(int argc, char *argv[]);

public:
    App();
    ~App();

    void StopApp();  // stop running - will quit at end of mainloop

    Config* GetConfig()                     { return mConfig; }
    IrrlichtManager* GetIrrlichtManager()   { return mIrrlichtManager; }
    Gui* GetGui()                           { return mGui; }
    InputDeviceManager* GetInputDeviceManager() { return mInputDeviceManager; }
    Controller* GetController()             { return mController; }
    LevelManager* GetLevelManager()         { return mLevelManager; }
    NodeManager* GetNodeManager()           { return mNodeManager; }
    Game* GetGame()                         { return mGame; }
    Editor* GetEditor()                     { return mEditor; }
    Physics* GetPhysics()                   { return mPhysics; }
    ISound* GetSound()                      { return mSound; }
    MusicManager* GetMusicManager()         { return mMusicManager; }
    RandomGenerator* GetRandomGenerator()   { return mRandomGenerator; }
    StringTable* GetStringTable()           { return mStringTable; }
    FontManager* GetFontManager()           { return mFontManager; }
    ProfileManager* GetProfileManager()     { return mProfileManager; }
    Championship* GetChampionship()         { return mChampionship; }
    HighscoreManager* GetHighscoreManager() { return mHighscoreManager; }
    RivalsMode* GetRivalsMode()             { return mRivalsMode; }
    IBilling* GetBilling() 					{ return mInAppBilling; }

    void LoadSettings();
    void SaveSettings();

    void SetMode(APP_MODES mode_);
    APP_MODES GetMode() const               { return mMode; }

    void SetEditGui(bool enable_);
    bool GetEditGui() const                 { return mEditGui; }
    void SetDrawAiTrack(bool enable_)       { mDrawAiTrack = enable_; }
    bool GetDrawAiTrack() const             { return mDrawAiTrack; }

    unsigned int  GetNumHovers() const              { return mHovers.size(); }
    const Hover& GetHover(unsigned int idx_) const  { return mHovers[idx_]; }

    void ForceUpdates(bool graphics_=true, bool sound_=true);

    // exported ghosts will have this name when it's set.
    std::string GetOverrideExportName() const            { return mOverrideExportName; }

    // Depending on settings an advert is displayed
    void AdvertismentCheck();

protected:

public:
    bool Init(int argc, char *argv[], void * systemData = 0);
    void Run();
    void Quit();
    void UpdateSound();
	android_app* GetAndroidApp() const { return mAndroidApp; }

protected:
    void HackBadFPS();
    void EndOldMode();
    void SetNewMode(APP_MODES mode_);
    void LoadHovers();

private:
    volatile bool       mIsRunning;

    IrrlichtManager*    mIrrlichtManager;
    InputDeviceManager* mInputDeviceManager;
    Controller*         mController;
    NodeManager*        mNodeManager;
    Gui*                mGui;
    Config*             mConfig;
    LevelManager*       mLevelManager;
    Game*               mGame;
    // Scores*          mScores
    Editor*             mEditor;
    Physics*            mPhysics;
    ISound*             mSound;
    MusicManager*       mMusicManager;
    RandomGenerator*    mRandomGenerator;
    StringTable*        mStringTable;
    FontManager*        mFontManager;
    ProfileManager*     mProfileManager;
    Championship*       mChampionship;
    HighscoreManager*   mHighscoreManager;
    RivalsMode*         mRivalsMode;
    AppTester*			mAppTester;
	android_app*		mAndroidApp;
	unsigned int 		mLastAdTime;
	IAdvert*			mAdvert;
	IBilling*			mInAppBilling;

    APP_MODES           mMode;
    bool                mEditGui;
    bool                mDrawAiTrack;

    std::vector<Hover>  mHovers;

    std::string         mOverrideExportName;
};

extern App APP;

#endif // MAIN_H
