// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#ifndef GUI_H
#define GUI_H

#include "compile_config.h"
#include "irrlicht_manager.h"
#include <string>
#include <map>

class ControllerDisplay;
class GuiUserControls;
class GuiDeveloperSettings;
class GuiLevelManager;
class GuiEditor;
class GuiIntro;
class GuiGame;
class GuiDialog;
class GuiAi;
class GuiHud;
class GuiTouch;
class GuiMenuMain;
class GuiMenuChampionship;
class GuiMenuChampionshipProgress;
class GuiMenuChampWinner;
class GuiMenuArcade;
class GuiMenuTimeAttack;
class GuiMenuRivals;
class GuiMenuRivalsScore;
class GuiMenuSelectHover;
//class GuiMenuReplayTheatre;
class GuiMenuHighscores;
class GuiMenuOptions;
class GuiMenuProfiles;
class GuiMenuControls;
class GuiMenuAnalog;
class GuiMenuNewProfile;
class GuiMenuGameEnd;
class GuiMenuGameEndChampionship;
class GuiMenuGameEndRivals;
class GuiMenuPause;
class GuiMenuCredits;
class GuiMenuLicenses;
class GuiMenuNagscreen;
class GuiMenuGraphics;
class GuiMenuLoadingScreen;
class GuiMenuTutorial3;
class GuiMenuHoverUnlocked;
class GuiMenuKeyboard;
class GuiDlgOkCancel;
class FontManager;
class Config;

class IEventFunctor
{
public:
    virtual ~IEventFunctor() {}
    virtual bool CallEvent(const irr::SEvent &event_) = 0;
};

template <class T>
class EventFunctor : public IEventFunctor
{
public:
    EventFunctor(T* obj_, bool (T::* fpt_)(const irr::SEvent &event_))
    {
        mObj = obj_;
        mFunctionPtr = fpt_;
    }
    virtual ~EventFunctor() {}
    virtual bool CallEvent(const irr::SEvent &event_)
    {
        return (*mObj.*mFunctionPtr)(event_);
    }

private:
    bool (T::*  mFunctionPtr)(const irr::SEvent &event_);
    T*          mObj;
};

enum GUI_SOUNDS
{
    GS_FOCUS,
    GS_CLICK_OK,
    GS_CLICK_CANCEL,
};

// The GUI uses Irrlicht gui system.
// All active gui-elements in a screen are put together into a GuiDialog.
// So there's only one active GuiDialog at a time.
class Gui
{
public:
    Gui(const Config& config);
    ~Gui();

    void Init(irr::gui::IGUIEnvironment * environment_, irr::video::IVideoDriver* videoDriver_, irr::io::IFileSystem* fileSystem_, FontManager* fontManager);
    void Update(irr::u32 timeTick);

    bool OnEvent(const irr::SEvent &event);

    //! Add a functor which is called for the given elementID for irr::EET_GUI_EVENT events
    //! This class will control the memory for the functor_ from now on.
    void AddGuiEventFunctor(int elementId_, IEventFunctor * functor_ );
    void RemoveGuiEventFunctor(int elementId_);

    void PrintDriverFeatures();

    // hovered == mouse is over any gui element
    bool IsHovered() const                         	 		{ return mHoveredElement ? true: false; }

    // which element has the keyboard focus
    void SetFocusElement(irr::gui::IGUIElement * element_)   { mFocusElement = element_; }
    irr::gui::IGUIElement * GetFocusElement() const          { return mFocusElement; }

    // EDIT-Dialogs
#ifdef HC1_ENABLE_EDITOR
    ControllerDisplay* GetControllerDisplay()       { return mControllerDisplay; }
    GuiUserControls* GetUserControls()              { return mUserControls; }
    GuiDeveloperSettings* GetDeveloperSettings()    { return mDeveloperSettings; }
    GuiLevelManager* GetGuiLevelManager()           { return mGuiLevelManager; }
    GuiEditor* GetGuiEditor()                       { return mGuiEditor; }
    GuiAi* GetGuiAi()                               { return mGuiAi; }
    GuiGame* GetGuiGame()                           { return mGuiGame; }

    // For testing gui-dialog layouts quickly
    GuiDialog* GetTestDialog()                    { return mTestDialog; }
#endif

    void LoadIntro();
    GuiIntro* GetGuiIntro()                         { return mGuiIntro; }

    // set the stuff from the editgui which can be accessed here visible/invisible
    void SetEditGuiVisible(bool visible_);

    void DisplayInfoText(const std::string text_, float timeMs_);

    // Game Dialogs
    void LoadMenuDialogs();
    GuiHud* GetGuiHud()                             { return mGuiHud; }
    GuiTouch* GetGuiTouch()							{ return mGuiTouch; }
    GuiMenuMain* GetGuiMenuMain()                   { return mMenuMain; }
    GuiMenuChampionship* GetGuiMenuChampionship()   { return mMenuChampionship; }
    GuiMenuChampionshipProgress* GetGuiMenuChampionshipProgress() { return mMenuChampionshipProgress; }
    GuiMenuChampWinner* GetGuiMenuChampWinner()     { return mMenuChampWinner; }
    GuiMenuArcade* GetGuiMenuArcade()               { return mMenuArcade; }
    GuiMenuTimeAttack* GetGuiMenuTimeAttack()       { return mMenuTimeAttack; }
    GuiMenuRivals* GetGuiMenuRivals()              	{ return mMenuRivals; }
    GuiMenuRivalsScore* GetGuiMenuRivalsScore()     { return mMenuRivalsScore; }
    GuiMenuSelectHover* GetGuiMenuSelectHover()     { return mMenuSelectHover; }
//    GuiMenuReplayTheatre* GetGuiMenuReplayTheatre() { return mMenuReplayTheatre; }
    GuiMenuHighscores* GetGuiMenuHighscores()       { return mMenuHighscores; }
    GuiMenuOptions* GetGuiMenuOptions()             { return mMenuOptions; }
    GuiMenuProfiles* GetGuiMenuProfiles()           { return mMenuProfiles; }
    GuiMenuControls* GetGuiMenuControls()           { return mMenuControls; }
    GuiMenuAnalog* GetGuiMenuAnalog()               { return mMenuAnalog; }
    GuiMenuNewProfile* GetGuiMenuNewProfile()       { return mMenuNewProfile; }
    GuiMenuGameEnd* GetGuiMenuGameEnd()             { return mMenuGameEnd; }
    GuiMenuGameEndChampionship* GetGuiMenuGameEndChampionship() { return mMenuGameEndChampionship; }
    GuiMenuGameEndRivals* GetGuiMenuGameEndRivals()   { return mMenuGameEndRivals; }
    GuiMenuPause* GetGuiMenuPause()                 { return mMenuPause; }
    GuiMenuCredits* GetGuiMenuCredits()             { return mMenuCredits; }
    GuiMenuLicenses* GetGuiMenuLicenses()			{ return mMenuLicenses; }
    GuiMenuNagscreen* GetGuiMenuNagscreen()         { return mMenuNagscreen; }
    GuiMenuGraphics* GetGuiMenuGraphics()           { return mMenuGraphics; }
    GuiMenuLoadingScreen* GetGuiMenuLoadingScreen() { return mMenuLoadingScreen; }
    GuiMenuTutorial3* GetGuiMenuTutorial3()         { return mMenuTutorial3; }
    GuiMenuHoverUnlocked* GetGuiMenuHoverUnlocked() { return mMenuHoverUnlocked; }
    GuiMenuKeyboard* GetGuiMenuKeyboard()			{ return mMenuKeyboard; }
    GuiDlgOkCancel* GetGuiDlgOkCancel()             { return mGuiDlgOkCancel; }

    // this will work as long as always only one dialog at a time is active
    void SetActiveDialog(GuiDialog * dialog_);
    GuiDialog* GetActiveDialog();

    // get the dialog active before this one
    GuiDialog* GetPreviousActiveDialog();

    // modal dialogs.
    void SetModalDialog(GuiDialog * dialog_);
    GuiDialog* GetActiveModalDialog();

    void PlayOverrideClickSound(GUI_SOUNDS sound_);
    void PlayGuiSound(GUI_SOUNDS sound_);

    // Use ShowProfiler instead of GetGuiProfiler()->setVisible as it ensures also it does some additional stuff.
    void SetShowProfiler(bool show);
    bool GetShowProfiler() const;
    irr::gui::IGUIProfiler * GetGuiProfiler();
    irr::gui::IGUIProfiler * GetGuiProfiler2();

protected:
    void SetHoveredElement(irr::gui::IGUIElement* hovered);

private:
	const Config& mConfig;

    irr::gui::IGUIEnvironment*   mEnvironment;
    irr::video::IVideoDriver*    mVideoDriver;

    // EDIT-Dialogs
#ifdef HC1_ENABLE_EDITOR
    irr::gui::IGUIContextMenu*  mMenu;
    irr::gui::IGUIToolBar*      mBar;
    ControllerDisplay*      	mControllerDisplay;
    GuiUserControls*        mUserControls;
    GuiDeveloperSettings*   mDeveloperSettings;
    GuiLevelManager*        mGuiLevelManager;
    GuiEditor*              mGuiEditor;
    GuiAi*                  mGuiAi;
    GuiGame*                mGuiGame;
    GuiDialog *             mTestDialog;
#endif

#if !defined(HOVER_RELEASE)
    irr::gui::IGUIStaticText*   mInfoText;
    irr::gui::IGUIStaticText*   mLevelText;
    irr::gui::IGUIStaticText*   mSpeedText;
    std::string             mInfoTextStr;
    float                   mInfoTextTime;
#endif

	irr::gui::IGUIProfiler* mGuiProfiler;
	irr::gui::IGUIProfiler* mGuiProfiler2;
	irr::gui::IGUIStaticText* mProfilerBackground;

    irr::gui::IGUIElement*	mHoveredElement;
    irr::gui::IGUIElement*	mHoveredElementNoSub;
    irr::gui::IGUIElement*  mFocusElement;

    GuiDialog*              mPreviousActiveDialog;
    GuiDialog*              mActiveDialog;
    GuiDialog*              mModalDialog;

    GuiIntro*               mGuiIntro;
    GuiMenuMain*            mMenuMain;
    GuiMenuChampionship*    mMenuChampionship;
    GuiMenuChampionshipProgress* mMenuChampionshipProgress;
    GuiMenuChampWinner*     mMenuChampWinner;
    GuiMenuArcade*          mMenuArcade;
    GuiMenuTimeAttack*      mMenuTimeAttack;
    GuiMenuRivals*          mMenuRivals;
    GuiMenuRivalsScore*     mMenuRivalsScore;
    GuiMenuSelectHover*     mMenuSelectHover;
//    GuiMenuReplayTheatre*   mMenuReplayTheatre;	// replay feature got cut
    GuiMenuHighscores*      mMenuHighscores;
    GuiMenuOptions*         mMenuOptions;
    GuiMenuProfiles*        mMenuProfiles;
    GuiMenuControls*        mMenuControls;
    GuiMenuAnalog*          mMenuAnalog;
    GuiMenuNewProfile*      mMenuNewProfile;
    GuiMenuGameEnd*         mMenuGameEnd;
    GuiMenuGameEndChampionship*  mMenuGameEndChampionship;
    GuiMenuGameEndRivals*    mMenuGameEndRivals;
    GuiMenuPause*           mMenuPause;
    GuiMenuCredits*         mMenuCredits;
    GuiMenuLicenses*		mMenuLicenses;
    GuiMenuNagscreen*       mMenuNagscreen;
    GuiMenuGraphics*        mMenuGraphics;
    GuiMenuLoadingScreen*   mMenuLoadingScreen;
    GuiMenuTutorial3*       mMenuTutorial3;
    GuiMenuHoverUnlocked*   mMenuHoverUnlocked;
    GuiMenuKeyboard* 		mMenuKeyboard;
    GuiDlgOkCancel*         mGuiDlgOkCancel;

    // Game Dialogs
    GuiHud*                 mGuiHud;
    GuiTouch*				mGuiTouch;

    std::string             mSoundFocus;
    int                     mSoundRefFocus;
    std::string             mSoundClick1;
    int                     mSoundRefClick1;
    std::string             mSoundClick2;
    int                     mSoundRefClick2;
    bool                    mClickSoundOverride;

    typedef std::map<int, IEventFunctor*> EventObjectMap;
    EventObjectMap   mGuiEventFunctors;
};

#endif // GUI_H
