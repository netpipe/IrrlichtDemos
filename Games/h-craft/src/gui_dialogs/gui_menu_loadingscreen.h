// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#ifndef GUI_MENU_LOADINGSCREEN_H
#define GUI_MENU_LOADINGSCREEN_H

#include "../gui_dialog.h"

enum MenuLoadingMode
{
    MLM_DEFAULT,
    MLM_TUTORIAL1,
    MLM_TUTORIAL2,
};

class GuiMenuLoadingScreen : public GuiDialog
{
public:
    GuiMenuLoadingScreen(const Config& config);
    ~GuiMenuLoadingScreen();

    virtual bool Load(const char* filename_, bool reloadLast_=false);
    virtual void Show();

protected:
    bool OnButtonStart(const irr::SEvent &event_);
    void RemoveFunctors();
    void ShowLoadingMode();

private:
    MenuLoadingMode mLoadingMode;

    irr::gui::IGUIStaticText* mTrackName;
    irr::gui::IGUIStaticText* mLaps;
    irr::gui::IGUIStaticText* mOpponents;
    irr::gui::IGUIStaticText* mTime;
    irr::gui::IGUIStaticText* mStaticLoading;
    irr::gui::IGUIStaticText* mPlayerInfo;
    irr::gui::IGUIButton* mButtonStart;

    irr::gui::IGUIImage*    mImageDefault;
    irr::gui::IGUIImage*    mImageTutorial;

    irr::gui::IGUIStaticText* mStaticTutorialOne1;
    irr::gui::IGUIStaticText* mStaticTutorialOne2;
    irr::gui::IGUIStaticText* mStaticTutorialOne2b;
    irr::gui::IGUIStaticText* mStaticTutorialOne3;

    irr::gui::IGUIStaticText* mStaticTutorialTwo1;
    irr::gui::IGUIStaticText* mStaticTutorialTwo2;
    irr::gui::IGUIStaticText* mStaticTutorialTwo3;
};

#endif // GUI_MENU_LOADINGSCREEN_H
