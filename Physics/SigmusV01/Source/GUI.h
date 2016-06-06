#ifndef GUI_H_INCLUDED
#define GUI_H_INCLUDED

#include "globals.h"
#include "main.h"
#include "sound.h"

class CGUI
{
    public:
    CGUI(IGUIEnvironment* gui, IVideoDriver* driver, ISceneManager* smgr, IrrlichtDevice* device, CEventReceiver* receiver, CSound* Sound);

    core::stringw MainMenu();
    bool QuitMenu();
    void DeathMenu();
    void EndMenu();

    void cleanMainMenu();

    private:
    bool ReloadPreviousMap;
    core::stringw PreviousMapFile;

    ICameraSceneNode* Camera;
    IMeshSceneNode* BackgroundNode;

    IGUIStaticText* HelperText;
    IGUIEditBox* FileOpenD;
    IGUIImage* SIGMUSImage;
    IGUIButton* LoadMapButton;
    IGUIButton* QuitButton;

    IGUIWindow* QuitPromptWindow;
    IGUIStaticText* QuitPromptText;
    IGUIButton* GotoMenu;
    IGUIButton* RestartMap;
    IGUIButton* CancelQuit;

    CEventReceiver* receiver;
    IrrlichtDevice* device;
    IVideoDriver* driver;
    ISceneManager* smgr;
    IGUIEnvironment* gui;

    CSound* Sound;
};

#endif
